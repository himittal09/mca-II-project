#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

class AuthModule
{
    bool isUserAuthenticated = false;
    unsigned int authenticatedUserId = 0;
    void authenticateUser (unsigned int userId) noexcept;
    void unauthenticateUser () noexcept;
public:
    AuthModule () noexcept;
    void logoutUser () noexcept(false);
    void loginUser (std::string email, std::string password) noexcept(false);
    unsigned int signupUser (std::string email, std::string password, std::string name) noexcept(false);
    bool getIfUserAuthenticated () noexcept;
    unsigned int getAuthenticatedUserId () noexcept;
};

namespace auth
{
    AuthModule auth;
}