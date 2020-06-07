#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <pqxx/pqxx>

#include "../../Controller/util.hpp"
#include "./long-term-goals.hpp"
#include "../../Controller/auth-provider.hpp"
#include "../../Controller/db-provider.hpp"

LongTermGoals::LongTermGoals () noexcept :LongTermGoalId {0} { }

LongTermGoals::LongTermGoals (std::string goal) noexcept(false)
    :isCompleted {false},
    journals_logded {0},
    goal {goal},
    creationDate {getPostgresDate()},
    userId {authProvider->getAuthenticatedUserId()},
    LongTermGoalId {0}
{ }

void LongTermGoals::save () noexcept(false)
{
    std::string insertSQL = " \
        INSERT INTO longtermgoals (userId, goal) \
        VALUES ($1, $2) \
        RETURNING LongTermGoalId;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    pqxx::row R = W.exec_params1(insertSQL, this->userId, this->goal);
    W.commit();
    this->LongTermGoalId = R[0].as<unsigned int>();
}

std::vector<LongTermGoals> LongTermGoals::getAllGoals (bool getCompleted) noexcept(false)
{
    const int limit = 100;

    std::string findSQL = " \
        SELECT LongTermGoalId, goal, journals_logded, lastProgress, createdAt from longtermgoals \
        WHERE userId = $1 AND isCompleted = $2 \
        LIMIT $3;";
    
    std::vector<LongTermGoals> goals;

    auto authenticatedUser = authProvider->getAuthenticatedUserId();

    pqxx::connection C(connectionString);
    pqxx::nontransaction N{C};
    pqxx::result R{ N.exec_params(findSQL, authenticatedUser, getCompleted, limit) };

    for (pqxx::row rowData: R)
    {
        LongTermGoals temp;
        
        temp.LongTermGoalId = rowData[0].as<unsigned int>();
        temp.userId = authenticatedUser;
        temp.goal = rowData[1].as<std::string>();
        temp.journals_logded = rowData[2].as<int>();
        temp.isCompleted = getCompleted;
        temp.creationDate = rowData[4].as<std::string>();

        if (!rowData[3].is_null())
        {
            temp.lastProgress = rowData[3].as<std::string>();
        }

        goals.push_back(temp);
    }

    return goals;
}

void LongTermGoals::lodgeJournal (std::string jour) noexcept(false)
{    
    std::string selectSQL = "SELECT lodge_journal($1, $2)";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec_params1(selectSQL, this->LongTermGoalId, jour);
    W.commit();
}

void LongTermGoals::markGoalComplete () noexcept(false)
{
    if (this->isCompleted)
    {
        return;
    }
    
    std::string updateSQL = " \
        update longtermgoals \
        set isCompleted = true \
        where LongTermGoalId = $1 and isCompleted = false;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec_params0(updateSQL, this->LongTermGoalId);
    W.commit();
}

std::vector<LongTermGoalJournalPartial> LongTermGoals::getMyJournals () noexcept(false)
{
    std::string findSQL = " \
        SELECT journal, lodgeDate from longtermjournals \
        WHERE LongTermGoalId = $1;";

    pqxx::connection C(connectionString);
    pqxx::nontransaction N{C};
    pqxx::result R{ N.exec_params(findSQL, this->LongTermGoalId) };

    std::vector<LongTermGoalJournalPartial> journals;

    for (pqxx::row rowData: R)
    {
        LongTermGoalJournalPartial temp;
        temp.journal = rowData[0].as<std::string>();
        temp.lodgeDate = rowData[1].as<std::string>();
        journals.push_back(temp);
    }

    return journals;
}