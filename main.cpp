#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#include "./util.cpp"
#include "./auth.cpp"

using std::cin;
using std::cout;

using auth::auth;

int login_menu ()
{
    int option;
    do
    {
        cout << "\n1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Choose a option: ";
        cin >> option;
    } while (!clamp(option, 1, 3));
    return option;
}

int main_menu ()
{
    int option;
    do
    {
        cout << "\n1. To-do list\n";
        cout << "2. Monthly Goal Tracker\n";
        cout << "3. go on streak\n";
        cout << "4. Long term goal tracker\n";
        cout << "5. Logout\n";
        cout << "6. Exit\n";
        cout << "Choose a option: ";
        cin >> option;
    } while (!clamp(option, 1, 6));
    return option;
}

int main ()
{

    return 0;
}