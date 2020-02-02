#ifndef __UTILSH__
    #define __UTILSH__
    #include "../Controller/util.h"
#endif

#ifndef __APH__
    #define __APH__
    #include "../Controller/auth-provider.h"
#endif

#ifndef __AUTHVH__
    #define __AUTHVH__
    #include "./authview.h"
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
bool authMenu ()
{
    int option = askAuthMenu();
    if (option == 1)
    {
        tryLogin();
    }
    else if (option == 2)
    {
        trySignup();
    }
    else if (option == 3)
    {
        return true;
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

void tryLogin ()
{   
    string email, password;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    cin >> password;
    try
    {
        auth::authProvider->loginUser(email, password);
    }
    catch(const std::runtime_error& e)
    {
        cout << endl << e.what();
    }
}

void trySignup ()
{
    std::string email, password, name;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    cin >> password;
    cout << "Your Name: ";
    std::ws(cin);
    std::getline(std::cin, name);
    try
    {
        auth::authProvider->signupUser(email, password, name);
    }
    catch(const std::runtime_error& e)
    {
        cout << endl << e.what();
    }
}