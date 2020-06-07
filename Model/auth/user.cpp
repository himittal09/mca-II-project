#include <fstream>
#include <iostream>
#include <stdexcept>
#include <pqxx/pqxx>

#include "../../Controller/db-provider.hpp"
#include "./user.hpp"

User::User (std::string email, std::string password, std::string name, unsigned int userId) noexcept
    :email {email},
    password {password},
    name {name},
    userId {userId}
{ }

User::User (std::string email, std::string password, std::string name) noexcept
    :email {email},
    password {password},
    name {name},
    userId {0}
{ }

User::User () noexcept : userId {0} { }

void User::save () noexcept(false)
{
    std::string insertSQL = " \
        INSERT INTO users (name, email, password) \
        VALUES ($1, $2, $3) \
        RETURNING userId;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    pqxx::row R = W.exec_params1(insertSQL, this->name, this->email, this->password);
    W.commit();
    // finally setting ID on the user
    this->userId = R[0].as<unsigned int>();
}

User User::findByCredentials (std::string email, std::string password) noexcept(false)
{
    std::string findSQL = " \
        SELECT userId, name, email, password FROM users \
        where email = $1 and password = $2 \
        LIMIT 1;";

    try
    {
        pqxx::connection C(connectionString);
        pqxx::nontransaction N{C};
        pqxx::row R{ N.exec_params1(findSQL, email, password) };

        unsigned int userId = R[0].as<unsigned int>();
        std::string name = R[1].as<std::string>();
        std::string emailnn = R[2].as<std::string>();
        std::string passwordnn = R[3].as<std::string>();
        User user = User(emailnn, passwordnn, name, userId);
        return user;
    }
    catch (const pqxx::unexpected_rows& e)
    {
       // if no row returned
        return User();
    }
    catch(const std::exception& e)
    {
        throw e;
    }
}

User::operator bool () const noexcept
{
    return (this->userId != 0);
}