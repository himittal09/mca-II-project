#include <iostream>

class User;

#ifndef __USERH__
#define __USERH__

class User
{
public:
    std::string password;
    std::string name;
    unsigned int userId;
    std::string email;
    
    User (std::string email, std::string password, std::string name, unsigned int userId) noexcept;
    User (std::string email, std::string password, std::string name) noexcept;
    User () noexcept;

    /*
     * Creates a user in the database, doesnot checks if already retundant email
     * @internal, donot use directly
     */
    void save () noexcept(false);
    static User findByCredentials (std::string email, std::string password) noexcept(false);

    explicit operator bool () const noexcept;
};

#endif