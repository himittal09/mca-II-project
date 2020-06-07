#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <pqxx/pqxx>

#include "./notification.hpp"
#include "../../Controller/db-provider.hpp"

NotificationService::NotificationService () noexcept : userId {0} { }

NotificationService::NotificationService (std::string notification, unsigned int userId) noexcept
    :notification{notification},
    userId {userId}
{ }

void NotificationService::pushNotification (std::string notification, unsigned int userId) noexcept(false)
{
    std::string insertSQL = " \
        INSERT INTO notifications (notification, userId) \
        VALUES ($1, $2);";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec_params0(insertSQL, notification, userId);
    W.commit();
}

std::vector<std::string> NotificationService::getAllNotifications (unsigned int userId) noexcept(false)
{
    const int limit = 100;

    std::string findSQL = " \
        SELECT notification from notifications \
        WHERE userId = $1 \
        LIMIT $2;";
    
    // make this a trigger in database
    std::string deleteSQL = " \
        DELETE FROM notifications \
        WHERE ctid IN ( \
            SELECT ctid \
            from notifications \
            WHERE userId = $1 \
            LIMIT $2 \
        );";
    
    std::vector<std::string> notifications;

    pqxx::connection C(connectionString);
    pqxx::nontransaction N{C};
    pqxx::result R{ N.exec_params(findSQL, userId, limit) };

    if (!R.size())
    {
        return notifications;
    }
    for (pqxx::row rowData: R)
    {
        notifications.push_back(rowData[0].as<std::string>());
    }
    N.commit();

    pqxx::work W{C};
    W.exec_params0(deleteSQL, userId, limit);
    W.commit();

    return notifications;
}