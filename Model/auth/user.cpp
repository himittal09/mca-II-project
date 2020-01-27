#ifndef __USERH__
    #define __USERH__
    #include "./user.h"
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

std::string userFilename = std::string("user.txt");

unsigned int User::getUsersCount () noexcept(false)
{
    std::ifstream fileptr;
    fileptr.open(userFilename, std::ios::in);
    if (!fileptr.good())
    {
        throw std::runtime_error("Couldn't get the journals to display!!");
    }

    fileptr.seekg(0, std::ios::end);
    unsigned int fileLength = fileptr.tellg();
    fileptr.close();
    fileLength /= sizeof(User);
    return fileLength;
}

User::User (std::string email, std::string password, std::string name) noexcept(false)
{
    this->email = email;
    this->password = password;
    this->name = name;
    try
    {
        this->userId = User::getUsersCount();
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
}

User::User () noexcept
{
    this->userId = 0;
}

/*
    * Searches for an email in the database, if found, return userId, else returns 0
    * User location in database + 1 = userId
    * @internal, donot use directly
    */
unsigned int User::findOne (std::string email) noexcept(false)
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
unsigned int User::save () const noexcept(false)
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

User User::findById (unsigned int userId) noexcept(false)
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

unsigned int User::findByCredentials (std::string email, std::string password) noexcept(false)
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
            if (st.password == password)
            {
                fp.close();
                return st.userId;
            }
            else
            {
                throw std::runtime_error("Password Incorrect!!");
            }
            
        }
    }

    fp.close();
    return 0;
}

std::ifstream& User::operator >> (std::ifstream& stream)
{
    stream.read((char *)this, sizeof(this));
    return stream;
}

std::ofstream& User::operator<<(std::ofstream& stream)
{
    stream.write((char *)this, sizeof(this));
    return stream;
}
