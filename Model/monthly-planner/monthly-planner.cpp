#ifndef __MONTHLYH__
    #define __MONTHLYH__
    #include "./monthly-planner.h"
#endif

#ifndef __AP__
    #define __AP__
    #include "../auth/auth-provider.cpp"
#endif

#ifndef __NOTIFICATIONH__
    #define __NOTIFICATIONH__
    #include "../notification/notification.h"
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _CHRONO_
    #define _CHRONO_
    #include <chrono>
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

static unsigned int MonthlyPlanner::getPlannerCount () noexcept(false)
{
    std::ifstream stream;
    stream.open(mpFilePath, std::ios::in);
    if (!stream.good())
    {
        throw std::runtime_error("Couldn't get the monthly plans to display!!");
    }

    stream.seekg(0, std::ios::end);

    unsigned int fileLength = stream.tellg();

    stream.close();
    return fileLength / sizeof(MonthlyPlanner);
}

MonthlyPlanner::MonthlyPlanner () noexcept
{
    this->plannerId = 0;
}

MonthlyPlanner::MonthlyPlanner (std::string monthlyPlan) noexcept(false)
{
    auto now = std::chrono::system_clock::now();

    this->monthlyPlan = monthlyPlan;
    this->isCompleted = false;
    this->userId = auth::authProvider->getAuthenticatedUserId();
    this->createdAt = now.time_since_epoch().count;
    
    try
    {
        this->plannerId = MonthlyPlanner::getPlannerCount() + 1;
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }    
}

unsigned int MonthlyPlanner::save () noexcept(false)
{
    std::ofstream writestream;
    writestream.open(mpFilePath, std::ios::app | std::ios::out);
    if (!writestream.good())
    {
        throw std::runtime_error("Couldn't save the plan in the database");
    }
    writestream << this;
    writestream.close();
    return this->plannerId;
}

void MonthlyPlanner::completePlan () noexcept(false)
{
    std::ofstream wstream;
    wstream.open(mpFilePath, std::ios::app | std::ios::out);
    if (!wstream.good())
    {
        throw std::runtime_error("Cannot reach the database to complete the Plan now!!");
    }
    this->isCompleted = true;
    wstream.seekp((this->plannerId - 1) * sizeof(MonthlyPlanner), std::ios::beg);
    wstream << this;
    wstream.close();
}

static std::vector<MonthlyPlanner> MonthlyPlanner::getallPlans (bool getCompleted) noexcept(false)
{
    std::vector<MonthlyPlanner> myPlans;

    std::ifstream stream;
    stream.open(mpFilePath, std::ios::in);

    if (!stream.good())
    {
        throw new std::runtime_error("Couldn't get all plans for displaying!!");
    }

    MonthlyPlanner obj;
    while (!stream.eof())
    {
        stream >> obj;
        if ((obj.userId == auth::authProvider->getAuthenticatedUserId()) && (obj.isCompleted == getCompleted))
        {
            myPlans.push_back(obj);
        }
    }
    stream.close();
    return myPlans;
}

static void MonthlyPlanner::checkAnRemoveExpiredPlan () noexcept(false)
{
    
}

friend std::ifstream& operator >> (std::ifstream&, const MonthlyPlanner& obj)
{
    stream.read((char *)&obj, sizeof(obj));
    return stream;
}

friend std::ofstream& operator << (std::ofstream&, const MonthlyPlanner& obj)
{
    stream.write((char *)&obj, sizeof(obj));
    return stream;
}