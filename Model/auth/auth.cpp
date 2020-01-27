#ifndef __USERH__
    #define __USERH__
    #include "./user.h"
#endif

#ifndef __AUTHH__
    #define __AUTHH__
    #include "./auth.h"
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

void AuthModule::authenticateUser (unsigned int userId) noexcept
{
    isUserAuthenticated = true;
    authenticatedUserId = userId;
}

void AuthModule::unauthenticateUser () noexcept
{
    isUserAuthenticated = false;
    authenticatedUserId = 0;
}

AuthModule::AuthModule () noexcept
{
    isUserAuthenticated = false;
    authenticatedUserId = 0;
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

    unsigned int userId;

    try
    {
        userId = User::findByCredentials(email, password);
    }
    catch(const std::exception& e)
    {
        throw e;
    }
    
    if (userId == 0)
    {
        throw std::runtime_error("No user found with current credentials!!");
    }

    authenticateUser(userId);
}

unsigned int AuthModule::signupUser (std::string email, std::string password, std::string name) noexcept(false)
{
    if (isUserAuthenticated)
    {
        throw std::runtime_error("Logout current user first!!");
    }
    
    int userExists = 0;

    try
    {
        userExists = User::findOne(email);
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
    
    if (userExists)
    {
        throw std::runtime_error("User already exists!!");
    }
    
    User *user = new User(email, password, name);
    unsigned int userId = 0;
    
    try
    {
        userId = user->save();
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
    
    authenticateUser(userId);

    return userId;
}

bool AuthModule::getIfUserAuthenticated () noexcept
{
    return this->isUserAuthenticated;
}

unsigned int AuthModule::getAuthenticatedUserId () noexcept
{
    return this->isUserAuthenticated ? this->authenticatedUserId : 0;
}

int main ()
{

    return 0;
}