#ifndef __USERH__
    #define __USERH__
    #include "./user.h"
#endif

#ifndef __AUTHH__
    #define __AUTHH__
    #include "./auth.h"
#endif

#include "../../Controller/util.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

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
    :isUserAuthenticated {false},
    authenticatedUserId {0},
    authenticatedUsername {std::string("User")}
{ }

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

    if (!user)
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
    
    User userExists {User::findOne(email)};

    if (userExists)
    {
        throw std::runtime_error("User already exists!!");
    }
    
    User user = User(email, password, name);
    
    user.save();
    
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