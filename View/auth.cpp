#ifndef __UTILS__
    #define __UTILS__
    #include "../Controller/util.cpp"
#endif

#ifndef __AUTHH__
    #define __AUTHH__
    #include "../Model/auth/auth.h"
#endif

#ifndef __AUTHMH__
    #define __AUTHMH__
    #include "./auth.h"
#endif

#ifndef _STDEXCEPT_
    #define _STDEXCEPT_
    #include <stdexcept>
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

using namespace std;

// return true for exit request
bool authMenu (AuthModule& authProvider)
{
    bool success = false;
    int option=0;
    while (!success)
    {
        option = askAuthMenu();
        if (option == 1)
        {
            success = tryLogin(authProvider);
        }
        else if (option == 2)
        {
            success = trySignup(authProvider);
        }
        else if (option == 3)
        {
            return true;
        }
    }
    
    return false;
}

int askAuthMenu ()
{
    int option=0;
    while (true)
    {
        cout << "\n1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Choose a option: ";
        cin >> option;
        if (clamp(option, 1, 3))
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

bool tryLogin (AuthModule& authProvider)
{   
    string email, password;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    cin >> password;
    try
    {
        authProvider.loginUser(email, password);
    }
    catch(const std::runtime_error& e)
    {
        cout << endl << e.what();
        return false;
    }
    return true;
}

bool trySignup (AuthModule& authProvider)
{
    std::string email, password, name;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    cin >> password;
    cout << "Your Name: ";
    cin.clear();
    std::getline(std::cin, name);
    try
    {
        authProvider.signupUser(email, password, name);
    }
    catch(const std::runtime_error& e)
    {
        cout << endl << e.what();
        return false;
    }
    return true;
}