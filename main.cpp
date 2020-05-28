#include <iostream>
#include <pqxx/pqxx>

#include "./Model/auth/auth.h"
#include "./db/database-worker.h"
#include "./Controller/auth-provider.h"
#include "./Controller/db-provider.h"
#include "./View/menus.h"

using namespace std;

std::unique_ptr<AuthModule> authProvider (new AuthModule());
std::unique_ptr<database> dbProvider(new database());

int main ()
{
    try {
        pqxx::result R = dbProvider->query("SELECT * FROM notifications");
        std::cout << "Found " << R.size() << " notifications\n";
        for (auto row: R)
        {
            std::cout << row[0].c_str() << '\n';
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
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