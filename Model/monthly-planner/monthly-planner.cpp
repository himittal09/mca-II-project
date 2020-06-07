#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <pqxx/pqxx>

#include "../../Controller/util.hpp"
#include "./monthly-planner.hpp"
#include "../../Controller/auth-provider.hpp"
#include "../../Controller/db-provider.hpp"

MonthlyPlanner::MonthlyPlanner () noexcept :plannerId {0} { }

MonthlyPlanner::MonthlyPlanner (std::string monthlyPlan) noexcept(false)
    :monthlyPlan {monthlyPlan},
    userId {authProvider->getAuthenticatedUserId()},
    isCompleted {false},
    createdAt {getPostgresDate()},
    plannerId {0}
{ }

void MonthlyPlanner::save() noexcept(false)
{
    std::string insertSQL = " \
        INSERT INTO monthlyplanners (plan, userId) \
        VALUES ($1, $2) \
        RETURNING plannerId;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    pqxx::row R = W.exec_params1(insertSQL, this->monthlyPlan, this->userId);
    W.commit();
    // finally setting ID on the plan
    this->plannerId = R[0].as<unsigned int>();
}

void MonthlyPlanner::completePlan () noexcept(false)
{
    std::string updateSQL = " \
        UPDATE monthlyplanners \
        SET isCompleted = true \
        WHERE plannerId = $1 AND isCompleted = false;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec_params0(updateSQL, this->plannerId);
    W.commit();
}

std::vector<MonthlyPlanner> MonthlyPlanner::getallPlans (bool getCompleted) noexcept(false)
{
    const int limit = 100;

    std::string findSQL = " \
        SELECT plannerId, userId, isCompleted, plan, createdAt from monthlyplanners \
        WHERE userId = $1 AND isCompleted = $2 \
        LIMIT $3;";

    std::vector<MonthlyPlanner> plans;

    pqxx::connection C(connectionString);
    pqxx::nontransaction N{C};
    pqxx::result R{ N.exec_params(findSQL, authProvider->getAuthenticatedUserId(), getCompleted, limit) };

    for (pqxx::row rowData: R)
    {
        MonthlyPlanner temp;

        temp.plannerId = rowData[0].as<unsigned int>();
        temp.userId = rowData[1].as<unsigned int>();
        temp.isCompleted = rowData[2].as<bool>();
        temp.monthlyPlan = rowData[3].as<std::string>();
        temp.createdAt = rowData[4].as<std::string>();

        plans.push_back(temp);
    }

    return plans;
}

void MonthlyPlanner::checkAnRemoveExpiredPlan () noexcept(false)
{
    std::string findSQL = "SELECT remove_monthly_plans()";
    
    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec1(findSQL);
    W.commit();
}