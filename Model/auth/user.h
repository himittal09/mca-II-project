#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

class User
{
    unsigned int getUsersCount () noexcept(false);
public:
    std::string password;
    std::string name;
    unsigned int userId;
    std::string email;
    
    User (std::string email, std::string password, std::string name) noexcept (false);
    User () noexcept;

    /*
     * Searches for an email in the database, if found, return userId, else returns 0
     * User location in database + 1 = userId
     * @internal, donot use directly
     */
    static unsigned int findOne (std::string email) noexcept(false);

    /*
     * Creates a user in the database, doesnot checks if already retundant email
     * @internal, donot use directly
     */
    static void save (User user) noexcept(false);
    static User findByCredentials (std::string email, std::string password) noexcept(false);

    friend std::ifstream& operator >> (std::ifstream&, User& obj);
    friend std::ofstream& operator << (std::ofstream&, User& obj);
};
