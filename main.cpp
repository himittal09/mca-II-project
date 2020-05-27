#include "./View/menus.h"
#include <iostream>
#include <pqxx/pqxx>

#ifndef __AUTHH__
    #define __AUTHH__
    #include "./Model/auth/auth.h"
#endif

#ifndef __APH__
    #define __APH__
    #include "./Controller/auth-provider.h"
#endif

#ifndef __DBPH__
    #define __DBPH__
    #include "./Controller/db-provider.h"
#endif

#ifndef sdncjwednwed
#define sdncjwednwed
    #include "./db/database-worker.h"
#endif

using namespace std;

std::unique_ptr<AuthModule> auth::authProvider (new AuthModule());
std::unique_ptr<database> db::dbProvider(new database());

int main ()
{
    try {
        pqxx::result R = db::dbProvider->query("SELECT * FROM notifications");
        std::cout << "Found " << R.size() << " notifications\n";
        for (auto row: R)
        {
            std::cout << row[0].c_str() << '\n';
        }
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
    }

    welcomeBanner ();
    checkForMissedStuff ();
    bool exitRequest = false;
    while (!exitRequest)
    {
        if (auth::authProvider->getIfUserAuthenticated())
        {
            std::cout << "\n\nWelcome " << auth::authProvider->getAuthenticatedUsername() << "\n";
            exitRequest = mainMenu ();
        }
        else
        {
            exitRequest = authMenu ();
        }
    }
    std::cout << "\nSee you soon ;)\n";
    return 0;
}