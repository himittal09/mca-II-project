#include <iostream>

#ifndef __USERH__
    #define __USERH__
    #include "./user.h"
#endif

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