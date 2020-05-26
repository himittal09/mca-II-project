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
    std::ifstream stream {atFilePath, std::ios::in | std::ios::app};
    if (!stream.is_open())
    {
        throw std::runtime_error("Couldn't get the activities to display!!");
    }
    
    unsigned int fileLength = 0;

    for (std::string str; std::getline(stream, str); std::ws(stream))
    {
        fileLength++;
    }
    return fileLength;
}

ActivityTracker::ActivityTracker () noexcept
:activityId {0}, longestStreak {0}, userId {0}
{ }

ActivityTracker::ActivityTracker (std::string plan, int64_t streakTime) noexcept(false)
:activityId {ActivityTracker::getActivityCount()+1}, longestStreak {0}, userId {auth::authProvider->getAuthenticatedUserId()},
activity {plan}, streakDuration {streakTime * 60}, createdAt {getCurrentTime()}, lastCheckIn {0}
{ }

void ActivityTracker::save () noexcept(false)
{
    std::ofstream writestream{atFilePath, std::ios::app | std::ios::out};
    if (!writestream.is_open())
    {
        throw std::runtime_error("Couldn't save the activity in the database");
    }
    writestream << *this;
}

std::vector<ActivityTracker> ActivityTracker::getAllActivity () noexcept(false)
{
    std::ifstream stream {atFilePath, std::ios::in | std::ios::app};
    if (!stream.is_open())
    {
        throw new std::runtime_error("Couldn't get all activities for displaying!!");
    }

    std::vector<ActivityTracker> myActivities;
    
    for (ActivityTracker obj; !(stream >> obj).eof(); )
    {
        if (obj.userId == auth::authProvider->getAuthenticatedUserId())
        {
            myActivities.push_back(obj);
        }
    }
    return myActivities;
}

void ActivityTracker::checkForAllStreakMiss ()
{
    std::ifstream rstream {atFilePath, std::ios::in | std::ios::app};
    if (!rstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to check in for a Streak!!");
    }

    std::ofstream wstream {"temp.dat", std::ios::out};
    if (!wstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to check in for a Streak!!");
    }

    for (ActivityTracker obj; !(rstream >> obj).eof(); )
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
    std::ifstream rstream {atFilePath, std::ios::in | std::ios::app};
    if (!rstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to check in for a Streak!!");
    }

    std::ofstream wstream {"temp.dat", std::ios::out};
    if (!wstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to check in for a Streak!!");
    }

    int64_t now = getCurrentTime();

    this->longestStreak++;
    this->lastCheckIn = now;

    for (ActivityTracker obj; !(rstream >> obj).eof(); )
    {
        if (obj.activityId == this->activityId)
        {
            obj.longestStreak++;
            obj.lastCheckIn = now;
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
    stream << " " << obj.streakDuration << " " << obj.userId << "\n";
    return stream;
}