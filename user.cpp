#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _FSTREAM_
    #define _FSTREAM_
    #include <fstream>
#endif

std::string userFilename = std::string("user.txt");

class User
{
    static unsigned int userCount;
    
    std::string name;
    std::string password;

public:
    unsigned int userId;
    std::string email;
    User (std::string email, std::string password, std::string name)
    {
        this->email = email;
        this->password = password;
        this->name = name;
        this->userId = userCount;
        userCount++;
    }

    User ()
    {
        this->userId = userCount;
    }

    ~User ()
    {
        userCount--;
    }

    static unsigned int getUserCount ()
    {
        return userCount;
    }

    /*
     * Searches for an email in the database, if found, return userId, else returns 0
     * User location in database + 1 = userId
     * @internal, donot use directly
     */
    static unsigned int findOne (std::string email)
    {
        std::ifstream fp;
        fp.open(userFilename, std::ios::in);
        if (!fp.good())
        {
            throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
        }
        
        User st;

        while (fp.read((char *)&st, sizeof(st)))
        {
            if (st.email == email)
            {
                fp.close();
                return st.userId;
            }
        }

        fp.close();
        return 0;
    }

    /*
     * Creates a user in the database, doesnot checks if already retundant email
     * @internal, donot use directly
     */
    unsigned int save () const
    {
        std::ofstream fp;
        fp.open(userFilename, std::ios::out);
        if (!fp.good())
        {
            throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
        }
        
        fp.write((char *)this, sizeof(this));
        fp.close();
        return this->userId;
        
    }

    static User findById (unsigned int userId)
    {
        std::ifstream fp;
        fp.open(userFilename, std::ios::in);
        if (!fp.good())
        {
            throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
        }

        User st;
        
        fp.seekg(sizeof(User) * (userId - 1), std::ios_base::beg);
        fp.read((char *)&st, sizeof(st));
        fp.close();
        return st;
    }

};
