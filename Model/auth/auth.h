#include <iostream>

#include "./user.h"

class AuthModule;

#ifndef __AUTHH__
#define __AUTHH__

class AuthModule
{
    bool isUserAuthenticated;
    unsigned int authenticatedUserId;
    std::string authenticatedUsername;

    void authenticateUser (User& user) noexcept;
    void unauthenticateUser () noexcept;
public:
    AuthModule () noexcept;

    void logoutUser () noexcept(false);
    void loginUser (std::string email, std::string password) noexcept(false);
    void signupUser (std::string email, std::string password, std::string name) noexcept(false);

    bool getIfUserAuthenticated () noexcept;
    unsigned int getAuthenticatedUserId () noexcept;
    std::string getAuthenticatedUsername () noexcept;
};

#endif