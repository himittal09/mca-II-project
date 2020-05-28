#include <memory>

#include "../Model/auth/auth.h"

#ifndef __AUTHPROVIDERH__
#define __AUTHPROVIDERH__

extern std::unique_ptr<AuthModule> authProvider;

#endif

// possible to use atomic to make the pointer accessible amongst all threads in multithreaded anvironment