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

std::string userFilename = std::string("user.dat");

unsigned int User::getUsersCount () noexcept(false)
{
    std::ifstream fileptr;
    fileptr.open(userFilename, std::ios::in | std::ios::app);
    if (!fileptr.is_open())
    {
        throw std::runtime_error("Couldn't get the journals to display!!");
    }

    unsigned int fileLength = 0;
    for (std::string str; std::getline(fileptr, str); std::ws(fileptr))
    {
        fileLength++;
    }

    fileptr.close();
    return fileLength;
}

User::User (std::string email, std::string password, std::string name) noexcept(false)
{
    this->email = email;
    this->password = password;
    this->name = name;
    try
    {
        this->userId = User::getUsersCount()+1;
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

unsigned int User::findOne (std::string email) noexcept(false)
{
    std::ifstream fp;
    
    // fp.open(userFilename, std::ios::in);
    // if (!fp.is_open())
    // {
    //     fp.clear();
    //     fp.open(userFilename, std::ios::trunc);
    //     fp.close();
    // }

    fp.open(userFilename, std::ios::in | std::ios::app);
    if (!fp.is_open())
    {
        throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
    }
    
    User st;

    while (!(fp >> st).eof())
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

void User::save (User user) noexcept(false)
{
    std::ofstream fp;
    fp.open(userFilename, std::ios::app | std::ios::out);
    if (!fp.is_open())
    {
        throw std::runtime_error("Unable to save user data, cannot perform operation now!!");
    }
    fp << user;
    fp.close();
}

User User::findByCredentials (std::string email, std::string password) noexcept(false)
{
    std::ifstream fp;
    fp.open(userFilename, std::ios::in | std::ios::app);
    if (!fp.is_open())
    {
        throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
    }
    
    User st;

    while (!(fp >> st).eof())
    {
        if (st.email == email)
        {
            if (st.password == password)
            {
                return st;
            }
            else
            {
                throw std::runtime_error("Password Incorrect!!");
            }
        }
    }

    fp.close();

    // hackaround to problem of creating new user object and returning if a user not found
    st.userId = 0;
    return st;
}

std::ifstream& operator >> (std::ifstream& stream, User& obj)
{
    std::ws(stream);
    std::getline(stream, obj.name, '$');
    std::ws(stream);
    std::getline(stream, obj.email, '$');
    std::ws(stream);
    std::getline(stream, obj.password, '$');
    stream >> obj.userId;
    return stream;
}

std::ofstream& operator << (std::ofstream& stream, User& obj)
{
    stream << obj.name << "$" << obj.email << "$" << obj.password << "$" << obj.userId << "\n";
    return stream;
}
