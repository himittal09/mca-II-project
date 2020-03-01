#ifndef __USERH__
    #define __USERH__
    #include "./user.h"
#endif

#ifndef __AUTHH__
    #define __AUTHH__
    #include "./auth.h"
#endif

#ifndef __UTILSH__
    #define __UTILSH__
    #include "../../Controller/util.h"
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _FSTREAM_
    #define _FSTREAM_
    #include <fstream>
#endif

#ifndef _STDEXCEPT_
    #define _STDEXCEPT_
    #include <stdexcept>
#endif

void AuthModule::authenticateUser (User& user) noexcept
{
    isUserAuthenticated = true;
    authenticatedUserId = user.userId;
    authenticatedUsername = user.name;
}

void AuthModule::unauthenticateUser () noexcept
{
    isUserAuthenticated = false;
    authenticatedUserId = 0;
    authenticatedUsername = std::string("User");
}

AuthModule::AuthModule () noexcept
{
    isUserAuthenticated = false;
    authenticatedUserId = 0;
    authenticatedUsername = std::string("User");
}

void AuthModule::logoutUser () noexcept(false)
{
    if (!isUserAuthenticated)
    {
        throw std::runtime_error("No User Logged in currently");
    }

    unauthenticateUser();
}

void AuthModule::loginUser (std::string email, std::string password) noexcept(false)
{
    if (isUserAuthenticated)
    {
        throw std::runtime_error("Logout current user first!!");
    }

    if (!isEmailValid(email))
    {
        throw std::runtime_error("Email is not valid!!");
    }

    User user {User::findByCredentials(email, password)};

    if (user.userId == 0)
    {
        throw std::runtime_error("No user found with current credentials!!");
    }

    authenticateUser(user);
}

void AuthModule::signupUser (std::string email, std::string password, std::string name) noexcept(false)
{
    if (isUserAuthenticated)
    {
        throw std::runtime_error("Logout current user first!!");
    }

    if (!isEmailValid(email))
    {
        throw std::runtime_error("Email is not valid!!");
    }
    
    int userExists {User::findOne(email)};

    if (userExists)
    {
        throw std::runtime_error("User already exists!!");
    }
    
    User user = User(email, password, name);
    
    User::save(user);
    
    authenticateUser(user);
}

bool AuthModule::getIfUserAuthenticated () noexcept
{
    return isUserAuthenticated;
}

unsigned int AuthModule::getAuthenticatedUserId () noexcept
{
    return authenticatedUserId;
}

std::string AuthModule::getAuthenticatedUsername () noexcept
{
    return authenticatedUsername;
}