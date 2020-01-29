#ifndef __AUTHH__
    #define __AUTHH__
    #include "../Model/auth/auth.h"
#endif

bool authMenu (AuthModule& authProvider);
int askAuthMenu ();
bool tryLogin (AuthModule& authProvider);
bool trySignup (AuthModule& authProvider);