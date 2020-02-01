#ifndef __AUTHH__
    #define __AUTHH__
    #include "./Model/auth/auth.h"
#endif

#ifndef __APH__
    #define __APH__
    #include "./Controller/auth-provider.h"
#endif

#ifndef __MENUMH__
    #define __MENUMH__
    #include "./View/menus.h"
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

std::unique_ptr<AuthModule> auth::authProvider (new AuthModule());

int main ()
{
    welcomeBanner ();
    bool exitRequest = false;
    while (!exitRequest)
    {
        if (auth::authProvider->getIfUserAuthenticated())
        {
            exitRequest = mainMenu ();
        }
        else
        {
            exitRequest = authMenu ();
        }
    }
    std::cout << "\nSee you soon ;)";
    return 0;
}