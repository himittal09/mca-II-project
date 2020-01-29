#ifndef __UTILS__
    #define __UTILS__
    #include "../../Controller/util.cpp"
#endif

#ifndef __AUTHH__
    #define __AUTHH__
    #include "../../Model/auth/auth.h"
#endif

#ifndef __MMMH__
    #define __MMMH__
    #include "./mainmenu.h"
#endif

using namespace std;

// return true for exit request
bool mainMenu (AuthModule& authProvider)
{
    bool success = false;
    int option;
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
        // cout << "5. Logout\n";
        }
        else if (option == 6)
        {
            return true;
        }
    } while (!success);
    return false;
}

int askmainMenu ()
{
    int option=0;
    while (true)
    {
        cout << "\n1. To-do list\n";
        cout << "2. Monthly Goal Tracker\n";
        cout << "3. go on streak\n";
        cout << "4. Long term goal tracker\n";
        cout << "5. Logout\n";
        cout << "6. Exit\n";
        cout << "Choose a option: ";
        cin >> option;
        if (clamp(option, 1, 6))
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