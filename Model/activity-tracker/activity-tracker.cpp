#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <pqxx/pqxx>
#include <string>

#include "../../Controller/util.hpp"
#include "./activity-tracker.hpp"
#include "../notification/notification.hpp"
#include "../../Controller/auth-provider.hpp"
#include "../../Controller/db-provider.hpp"

ActivityTracker::ActivityTracker () noexcept
    :activityId {0},
    longestStreak {0},
    userId {0}
{ }

ActivityTracker::ActivityTracker (std::string plan, int64_t streakTime) noexcept(false)
    :activityId {0},
    longestStreak {0},
    userId {authProvider->getAuthenticatedUserId()},
    activity {plan},
    streakDuration {streakTime},
    createdAt {getPostgresDate()}
{ }

void ActivityTracker::save () noexcept(false)
{
    std::string insertSQL = " \
        INSERT INTO activitytrackers (activity, userId, streakDuration) \
        VALUES ($1, $2, $3) \
        RETURNING activityId;";
    
    std::string para = "'";
    para += std::to_string(this->streakDuration);
    para += " Minutes'";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    pqxx::row R = W.exec_params1(insertSQL, this->activity, this->userId, para);
    W.commit();
    // finally setting ID on the todo
    this->activityId = R[0].as<unsigned int>();
}

std::vector<ActivityTracker> ActivityTracker::getAllActivity () noexcept(false)
{
    const int limit = 100;

    std::string findSQL = " \
        SELECT activityId, activity, longestStreak, \
        EXTRACT(EPOCH FROM streakDuration) / 60, \
        lastCheckIn, createdAt from activitytrackers \
        WHERE userId = $1 \
        LIMIT $2;";

    std::vector<ActivityTracker> activities;

    auto authenticatedUser = authProvider->getAuthenticatedUserId();

    pqxx::connection C(connectionString);
    pqxx::nontransaction N{C};
    pqxx::result R{ N.exec_params(findSQL, authenticatedUser, limit) };

    for (pqxx::row rowData: R)
    {
        ActivityTracker temp;

        temp.activityId = rowData[0].as<unsigned int>();
        temp.longestStreak = rowData[2].as<int>();
        temp.activity = rowData[1].as<std::string>();
        temp.userId = authenticatedUser;
        temp.createdAt = rowData[5].as<std::string>();
        temp.streakDuration = rowData[3].as<int64_t>();

        if (!rowData[4].is_null())
        {
            temp.lastCheckIn = rowData[4].as<std::string>();
        }

        activities.push_back(temp);
    }

    return activities;
}

void ActivityTracker::checkForAllStreakMiss ()
{
    std::string selectSQL = "SELECT remove_activities();";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec1(selectSQL);
    W.commit();
}

void ActivityTracker::checkIn ()
{
    std::string updateSQL = " \
        update activitytrackers \
        set longestStreak = longestStreak + 1, lastCheckIn = current_timestamp \
        where activityId = $1;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec_params0(updateSQL, this->activityId);
    W.commit();
}