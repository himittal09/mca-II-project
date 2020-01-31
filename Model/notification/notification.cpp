#ifndef __NOTIFICATIONH__
    #define __NOTIFICATIONH__
    #include "./notification.h"
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

#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

// #ifndef _FILESYSTEM_
//     #define _FILESYSTEM_
//     #include "../../Controller/filesystem.hpp"
// #endif

std::string notificationFilePath = std::string("notifications.dat");

NotificationService::NotificationService ()
{
    this->userId = 0;
}

NotificationService::NotificationService (std::string notification, unsigned int userId)
{
    this->notification = notification;
    this->userId = userId;
}

void NotificationService::pushNotification (std::string notification, unsigned int userId) noexcept(false)
{
    std::ofstream writestream;
    writestream.open(notificationFilePath, std::ios::app);
    if (!writestream.good())
    {
        throw std::runtime_error("Couldn't push the notification");
    }
    NotificationService obj(notification, userId);
    writestream << obj;
    writestream.close();
}

std::vector<std::string> NotificationService::getAllNotifications (unsigned int userId) noexcept(false)
{
    std::vector<std::string> notifications;

    std::ifstream stream;
    stream.open(notificationFilePath, std::ios::in);
    if (!stream.good())
    {
        throw new std::runtime_error("Couldn't get all notifications for displaying!!");
    }

    std::ofstream writeStream;
    writeStream.open("temp.dat", std::ios::out);
    if (!writeStream.good())
    {
        throw new std::runtime_error("Couldn't get all notifications for displaying!!");
    }

    NotificationService obj;
    while (!(stream >> obj).eof())
    {
        if (obj.userId == userId)
        {
            notifications.push_back(obj.notification);
        }
        else
        {
            writeStream << obj;
        }
    }
    stream.close();
    writeStream.close();

    if (notifications.size())
    {
        // std::filesystem::remove(notificationFilePath);
        // std::filesystem::rename("temp.dat", notificationFilePath);
        remove(notificationFilePath.c_str());
        rename("temp.dat", notificationFilePath.c_str());
    }
    else
    {
        // std::filesystem::remove("temp.dat");
        remove("temp.dat");
    }

    return notifications;
}

std::ifstream& operator >> (std::ifstream& stream, NotificationService& obj)
{
    std::getline(stream, obj.notification, '$');
    stream >> obj.userId;
    return stream;
}

std::ofstream& operator << (std::ofstream& stream, NotificationService& obj)
{
    // append a dollar ($) after every variable sized string
    stream << obj.notification << "$" << obj.userId << "\n";
    return stream;
}
