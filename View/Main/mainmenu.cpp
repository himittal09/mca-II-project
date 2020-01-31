#ifndef __UTILSH__
    #define __UTILSH__
    #include "../../Controller/util.h"
#endif

#ifndef __APH__
    #define __APH__
    #include "../../Controller/auth-provider.h"
#endif

#ifndef __MMENUVH__
    #define __MMENUVH__
    #include "./mainmenu.h"
#endif

using namespace std;

// return true for exit request
bool mainMenu ()
{
    bool success = false;
    unsigned int option;
    do
    {
        option = askmainMenu();
        if (option == 1)
        {
            // cout << "\n1. To-do list\n";
        }
        else if (option == 2)
        {
        // cout << "2. Monthly Goal Tracker\n";
        }
        else if (option == 3)
        {
        // cout << "3. go on streak\n";
        }
        else if (option == 4)
        {
        // cout << "4. Long term goal tracker\n";
        }
        else if (option == 5)
        {
            // notifications
        }
        else if (option == 6)
        {
        // cout << "5. Logout\n";
        }
        else if (option == 7)
        {
            return true;
        }
    } while (!success);
    return false;
}

unsigned int askmainMenu ()
{
    unsigned int option=0;
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
        if (clamp(option, 1u, 7u))
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