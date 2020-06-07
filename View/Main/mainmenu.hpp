#include "../../Controller/util.hpp"
#include "./activity-tracker-view.hpp"
#include "./long-term-goal-view.hpp"
#include "./monthly-planner-view.hpp"
#include "./todo-list-view.hpp"
#include "../../Model/notification/notification.hpp"
#include "../../Controller/auth-provider.hpp"

using namespace std;

namespace view {
    bool mainMenu ();
    int askmainMenu ();
    void printNotifications ();
    void checkForMissedStuff ();
}

#ifndef __MMENUVH__
#define __MMENUVH__

// return true for exit request
inline bool view::mainMenu ()
{
    int option = view::askmainMenu();
    if (option == 1)
    {
        return view::todoMenu();
    }
    else if (option == 2)
    {
        return view::mpMenu ();
    }
    else if (option == 3)
    {
        return view::atMenu ();
    }
    else if (option == 4)
    {
        return view::ltgMenu ();
    }
    else if (option == 5)
    {
        view::printNotifications();
    }
    else if (option == 6)
    {
        authProvider->logoutUser();
    }
    else if (option == 7)
    {
        return true;
    }
    return false;
}

inline int view::askmainMenu ()
{
    int option=0;
    while (true)
    {
        cout << "\n1. To-do list\n";
        cout << "2. Monthly Goal Tracker\n";
        cout << "3. go on streak\n";
        cout << "4. Long term goal tracker\n";
        cout << "5. Get all Notifications\n";
        cout << "6. Logout\n";
        cout << "7. Exit\n";
        cout << "Choose a option: ";
        cin >> option;
        if (clamp(option, 1, 7))
        {
            break;
        }
        else
        {
            cout << "\nPlease choose an option amongst the following: ";
        }
    }
    return option;
}

inline void view::printNotifications ()
{
    std::vector<std::string> notifications {NotificationService::getAllNotifications(authProvider->getAuthenticatedUserId())};
    if (!notifications.size())
    {
        cout << "No new Notifications!!\n";
        return;
    }
    for (std::string& notification: notifications)
    {
        cout << notification << "\n";
    }
}

inline void view::checkForMissedStuff ()
{
    view::checkForMissedAT ();
    view::checkForMissedMP ();
}

#endif