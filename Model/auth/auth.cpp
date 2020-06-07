#include <iostream>
#include <fstream>
#include <stdexcept>
#include <pqxx/pqxx>

#include "./user.hpp"
#include "./auth.hpp"
#include "../../Controller/util.hpp"

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
    
    User user = User(email, password, name);
    
    try
    {
        user.save();
    }
    catch (const pqxx::unique_violation& e)
    {
        throw std::runtime_error("User already exists!!");
    }
    catch(const std::exception& e)
    {
        throw e;
    }
    
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