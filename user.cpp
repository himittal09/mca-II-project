#include <iostream>

using std::string;

char * userFilename = "user.txt";

class User
{
    static unsigned int userCount;
    
    string name;
    string password;

public:
    unsigned int userId;
    string email;
    User (string email, string password, string name)
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
     * @internal , donot use directly
     */
    static unsigned int findOne (string email)
    {
        FILE *fp = fopen(userFilename, "rb+");
        if (!fp)
        {
            throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
        }
        
        int records=0;
        User st;

        while (fread(&st, sizeof(User), 1, fp))
        {
            records++;
            if (st.email == email)
            {
                fclose(fp);
                return st.userId;
            }
        }
        fclose(fp);
        return 0;
    }

    /*
     * Creates a user in the database, doesnot checks if already retundant email
     * @internal , donot use directly
     */
    unsigned int save () const
    {
        FILE *fp = fopen(userFilename, "ab+");
        if (!fp)
        {
            throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
        }
        
        fwrite(this, sizeof(this), 1, fp);
        fclose(fp);
        return this->userId;
    }

    static User findById (unsigned int userId)
    {
        FILE *fp = fopen(userFilename, "rb+");
        if (!fp)
        {
            throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
        }
        
        fseek(fp, sizeof(User) * (userId - 1), SEEK_SET);

        User user;
        fread(&user, sizeof(User), 1, fp);
        fclose(fp);
        return user;
    }

};
