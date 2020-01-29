#ifndef __AUTHH__
    #define __AUTHH__
    #include "./Model/auth/auth.h"
#endif

#ifndef __MENUMH__
    #define __MENUMH__
    #include "./View/menus.h"
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

int main ()
{
    AuthModule authProvider = AuthModule();
    welcomeBanner ();
    bool exitRequest = false;
    while (!exitRequest)
    {
        if (authProvider.getIfUserAuthenticated())
        {
            exitRequest = mainMenu (authProvider);
        }
        else
        {
            exitRequest = authMenu (authProvider);
        }
    }
    std::cout << "\nSee you soon ;)";
    return 0;
}