#ifndef __ATH__
    #define __ATH__
    #include "./activity-tracker.h"
#endif

#ifndef __APH__
    #define __APH__
    #include "../../Controller/auth-provider.h"
#endif

#ifndef __NOTIFICATIONH__
    #define __NOTIFICATIONH__
    #include "../notification/notification.h"
#endif

#ifndef __UTILSH__
    #define __UTILSH__
    #include "../../Controller/util.h"
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _FSTREAM_
    #define _FSTREAM_
    #include <fstream>
#endif

#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

#ifndef _STDEXCEPT_
    #define _STDEXCEPT_
    #include <stdexcept>
#endif

#ifndef _SSTREAM_
    #define _SSTREAM_
    #include <sstream>
#endif

std::string atFilePath = std::string("activity-tracker.dat");

unsigned int ActivityTracker::getActivityCount () noexcept(false)
{
    std::ifstream stream;
    stream.open(atFilePath, std::ios::in | std::ios::app);
    if (!stream.is_open())
    {
        throw std::runtime_error("Couldn't get the activities to display!!");
    }
    
    unsigned int fileLength = 0;

    for (std::string str; std::getline(stream, str); std::ws(stream))
    {
        fileLength++;
    }
    stream.close();
    return fileLength;
}

ActivityTracker::ActivityTracker () noexcept
{
    this->activityId = 0;
    this->longestStreak = 0;
    this->userId = 0;
}

// inout streak duration in minutes in this constructor
ActivityTracker::ActivityTracker (std::string plan, int64_t streakDuration) noexcept(false)
{
    try
    {
        this->activityId = ActivityTracker::getActivityCount()+1;
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
    
    this->longestStreak = 0;
    this->userId = auth::authProvider->getAuthenticatedUserId();
    this->activity = plan;
    this->streakDuration = streakDuration * 60;
    this->createdAt = getCurrentTime();
    this->lastCheckIn = 0;
}

void ActivityTracker::save (ActivityTracker& obj) noexcept(false)
{
    std::ofstream writestream;
    writestream.open(atFilePath, std::ios::app | std::ios::out);
    if (!writestream.is_open())
    {
        throw std::runtime_error("Couldn't save the activity in the database");
    }
    writestream << obj;
    writestream.close();
}

std::vector<ActivityTracker> ActivityTracker::getAllActivity () noexcept(false)
{
    std::vector<ActivityTracker> myActivities;

    std::ifstream stream;
    stream.open(atFilePath, std::ios::in | std::ios::app);

    if (!stream.is_open())
    {
        throw new std::runtime_error("Couldn't get all activities for displaying!!");
    }

    ActivityTracker obj;
    while (!(stream >> obj).eof())
    {
        if (obj.userId == auth::authProvider->getAuthenticatedUserId())
        {
            myActivities.push_back(obj);
        }
    }
    stream.close();
    return myActivities;
}

unsigned int ActivityTracker::checkForAllStreakMiss ()
{
    std::ifstream rstream;
    rstream.open(atFilePath, std::ios::in | std::ios::app);
    if (!rstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to check in for a Streak!!");
    }

    std::ofstream wstream;
    wstream.open("temp.dat", std::ios::out);
    if (!wstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to check in for a Streak!!");
    }

    ActivityTracker obj;

    while (!(rstream >> obj).eof())
    {
        if (diffBetween(obj.lastCheckIn, obj.streakDuration))
        {
            wstream << obj;
        }
        else
        {
            std::stringstream str;
            str << "You failed the activity ";
            str << obj.activity;
            str << "! Your streak this time was: ";
            str << obj.longestStreak;
            NotificationService::pushNotification(str.str(), obj.userId);
        }
    }

    wstream.close();
    rstream.close();

    remove(atFilePath.c_str());
    rename("temp.dat", atFilePath.c_str());
}

void ActivityTracker::checkIn ()
{
    std::ifstream rstream;
    rstream.open(atFilePath, std::ios::in | std::ios::app);
    if (!rstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to check in for a Streak!!");
    }

    std::ofstream wstream;
    wstream.open("temp.dat", std::ios::out);
    if (!wstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to check in for a Streak!!");
    }

    ActivityTracker obj;
    int64_t now = getCurrentTime();

    this->longestStreak++;
    this->lastCheckIn = now;

    while (!(rstream >> obj).eof())
    {
        if (obj.activityId == this->activityId)
        {
            obj.longestStreak++;
            obj.longestStreak = now;
        }
        wstream << obj;
    }

    wstream.close();
    rstream.close();

    remove(atFilePath.c_str());
    rename("temp.dat", atFilePath.c_str());
}

std::ifstream& operator >> (std::ifstream& stream, ActivityTracker& obj)
{
    std::ws(stream);
    std::getline(stream, obj.activity, '$');
    stream >> obj.activityId >> obj.longestStreak >> obj.lastCheckIn;
    stream >> obj.streakDuration >> obj.userId;
    return stream;
}

std::ofstream& operator << (std::ofstream& stream, ActivityTracker& obj)
{
    stream << obj.activity << "$" << obj.activityId << " " << obj.longestStreak << " " << obj.lastCheckIn;
    stream << " " << obj.streakDuration << " " << obj.userId;
    return stream;
}