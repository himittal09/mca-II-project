#include <memory>

#ifndef __AUTHH__
    #define __AUTHH__
    #include "../Model/auth/auth.h"
#endif

#ifndef __AUTHPROVIDERH__
    #define __AUTHPROVIDERH__

    namespace auth
    {
        extern std::unique_ptr<AuthModule> authProvider;
    }
#endif

// possible to use atomic to make the pointer accessible amongst all threads in multithreaded anvironment