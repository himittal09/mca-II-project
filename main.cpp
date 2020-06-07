#include <iostream>

#include "./Model/auth/auth.hpp"
#include "./Controller/auth-provider.hpp"
#include "./Controller/db-provider.hpp"
#include "./View/menus.hpp"
#include "./db/dbinit.hpp"

using namespace std;

std::unique_ptr<AuthModule> authProvider (new AuthModule());
std::string connectionString("postgresql://himanshu:mynewpass@127.0.0.1:5432/test-db");

int main ()
{
    try {
        initDB::initDB();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << "\n";
    }
    view::welcomeBanner ();
    view::checkForMissedStuff ();
    bool exitRequest = false;
    while (!exitRequest)
    {
        if (authProvider->getIfUserAuthenticated())
        {
            std::cout << "\n\nWelcome " << authProvider->getAuthenticatedUsername() << "\n";
            exitRequest = view::mainMenu ();
        }
        else
        {
            exitRequest = view::authMenu ();
        }
    }
    std::cout << "\nSee you soon ;)\n";
    return 0;
}