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
    std::ifstream fileptr {userFilename, std::ios::in | std::ios::app};
    if (!fileptr.is_open())
    {
        throw std::runtime_error("Couldn't get the journals to display!!");
    }

    unsigned int fileLength = 0;
    for (std::string str; std::getline(fileptr, str); std::ws(fileptr))
    {
        fileLength++;
    }

    return fileLength;
}

User::User (std::string email, std::string password, std::string name) noexcept(false)
:email {email}, password{password}, name {name}, userId {User::getUsersCount() + 1}
{ }

User::User () noexcept : userId {0} { }

User User::findOne (std::string email) noexcept(false)
{
    std::ifstream fp {userFilename, std::ios::in | std::ios::app};
    if (!fp.is_open())
    {
        throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
    }
    
    for (User st; !(fp >> st).eof(); )
    {
        if (st.email == email)
        {
            return st;
        }
    }
    return User();
}

void User::save () noexcept(false)
{
    std::ofstream fp {userFilename, std::ios::app | std::ios::out};
    if (!fp.is_open())
    {
        throw std::runtime_error("Unable to save user data, cannot perform operation now!!");
    }
    fp << *this;
}

User User::findByCredentials (std::string email, std::string password) noexcept(false)
{
    std::ifstream fp {userFilename, std::ios::in | std::ios::app};
    if (!fp.is_open())
    {
        throw std::runtime_error("Unable to access user data, cannot perform operation now!!");
    }
    
    for (User st; !(fp >> st).eof(); )
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

    // return dummy object in case of no match
    return User();
}

User::operator bool () const noexcept
{
    return (this->userId != 0);
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
