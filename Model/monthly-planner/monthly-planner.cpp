#ifndef __MONTHLYH__
    #define __MONTHLYH__
    #include "./monthly-planner.h"
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

#ifndef _STDEXCEPT_
    #define _STDEXCEPT_
    #include <stdexcept>
#endif

#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

std::string mpFilePath = std::string("monthlyplanner.dat");

unsigned int MonthlyPlanner::getPlannerCount () noexcept(false)
{
    std::ifstream stream {mpFilePath, std::ios::in | std::ios::app};
    if (!stream.is_open())
    {
        throw std::runtime_error("Couldn't get the monthly plans to display!!");
    }
    
    unsigned int fileLength = 0;
    for (std::string str; std::getline(stream, str); std::ws(stream))
    {
        fileLength++;
    }
    return fileLength;
}

MonthlyPlanner::MonthlyPlanner () noexcept
{
    this->plannerId = 0;
}

MonthlyPlanner::MonthlyPlanner (std::string monthlyPlan) noexcept(false)
{
    this->monthlyPlan = monthlyPlan;
    this->isCompleted = false;
    this->userId = auth::authProvider->getAuthenticatedUserId();
    this->createdAt = getCurrentTime();
    this->plannerId = MonthlyPlanner::getPlannerCount() + 1;
}

void MonthlyPlanner::save(MonthlyPlanner& obj) noexcept(false)
{
    std::ofstream writestream {mpFilePath, std::ios::app | std::ios::out};
    if (!writestream.is_open())
    {
        throw std::runtime_error("Couldn't save the plan in the database");
    }
    writestream << obj;
}

void MonthlyPlanner::completePlan () noexcept(false)
{
    if (this->isCompleted)
    {
        return;
    }
    this->isCompleted = true;

    std::ifstream rstream {mpFilePath, std::ios::in | std::ios::app};
    if (!rstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to complete the Plan now!!");
    }

    std::ofstream wstream {"temp.dat", std::ios::out};
    if (!wstream.is_open())
    {
        throw std::runtime_error("Cannot reach the database to complete the Plan now!!");
    }

    for (MonthlyPlanner obj; !(rstream >> obj).eof(); )
    {
        if (obj.plannerId == this->plannerId)
        {
            obj.isCompleted = true;
            this->isCompleted = true;
        }
        wstream << obj;
    }

    wstream.close();
    rstream.close();

    remove(mpFilePath.c_str());
    rename("temp.dat", mpFilePath.c_str());
}

std::vector<MonthlyPlanner> MonthlyPlanner::getallPlans (bool getCompleted) noexcept(false)
{
    std::ifstream stream {mpFilePath, std::ios::in | std::ios::app};
    if (!stream.is_open())
    {
        throw std::runtime_error("Couldn't get all plans for displaying!!");
    }

    std::vector<MonthlyPlanner> myPlans;
    unsigned int authenticatedUserId {auth::authProvider->getAuthenticatedUserId()};

    for (MonthlyPlanner obj; !(stream >> obj).eof(); )
    {
        if ((obj.userId == authenticatedUserId) && (obj.isCompleted == getCompleted))
        {
            myPlans.push_back(obj);
        }
    }
    return myPlans;
}

void MonthlyPlanner::checkAnRemoveExpiredPlan () noexcept(false)
{
    std::ifstream rstream {mpFilePath, std::ios::in | std::ios::app};
    if (!rstream.is_open())
    {
        throw std::runtime_error("Cannot access monthly plans at the movement");
    }

    bool toRemovePlans {false};
    
    for (MonthlyPlanner obj; !(rstream >> obj).eof(); )
    {
        // to execute block only once
        if (!toRemovePlans && monthDiffFromNow(obj.createdAt) != 0)
        {
            toRemovePlans = true;
        }
        if (toRemovePlans)
        {
            NotificationService::pushNotification(obj.monthlyPlan, obj.userId);
        }
    }

    if (toRemovePlans)
    {
        // open file in truncate mode and delete all content
        std::ofstream ofs {mpFilePath, std::ios::trunc | std::ios::out};
    }
}

std::ifstream& operator >> (std::ifstream& stream, MonthlyPlanner& obj)
{
    std::ws(stream);
    std::getline(stream, obj.monthlyPlan, '$');
    stream >> obj.plannerId >> obj.userId >> obj.createdAt >> obj.isCompleted;
    return stream;
}

std::ofstream& operator << (std::ofstream& stream, MonthlyPlanner& obj)
{
    stream << obj.monthlyPlan << "$" << obj.plannerId << " " << obj.userId << " ";
    stream << obj.createdAt << " " << obj.isCompleted << "\n";
    return stream;
}