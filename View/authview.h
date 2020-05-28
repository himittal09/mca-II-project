#include <stdexcept>
#include <iostream>

#include "../Controller/util.h"
#include "../Model/auth/auth.h"
#include "../Controller/auth-provider.h"

using namespace std;

namespace view {
    bool authMenu ();
    int askAuthMenu ();
    void tryLogin ();
    void trySignup ();
}

#ifndef __AUTHVH__
#define __AUTHVH__

// return true for exit request
inline bool view::authMenu ()
{
    int option = view::askAuthMenu();
    if (option == 1)
    {
        view::tryLogin();
    }
    else if (option == 2)
    {
        view::trySignup();
    }
    else if (option == 3)
    {
        return true;
    }    
    return false;
}

inline int view::askAuthMenu ()
{
    int option=0;
    while (true)
    {
        cout << "\n1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Choose a option: ";
        cin.clear();
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

inline void view::tryLogin ()
{   
    string email, password;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    cin >> password;
    try
    {
        authProvider->loginUser(email, password);
    }
    catch(const std::runtime_error& e)
    {
        cout << endl << e.what();
    }
}

inline void view::trySignup ()
{
    std::string email, password, name;
    cout << "Email: ";
    std::ws(cin);
    cin >> email;
    cout << "Password: ";
    std::ws(cin);
    cin >> password;
    cout << "Your Name: ";
    std::ws(cin);
    std::getline(std::cin, name);
    try
    {
        authProvider->signupUser(email, password, name);
    }
    catch(const std::runtime_error& e)
    {
        cout << endl << e.what();
    }
}

#endif