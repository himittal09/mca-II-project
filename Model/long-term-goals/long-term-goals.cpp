#ifndef __LTGH__
    #define __LTGH__
    #include "./long-term-goals.h"
#endif

#ifndef __LTJH__
    #define __LTJH__
    #include "./long-term-journal.h"
#endif

#ifndef __AP__
    #define __AP__
    #include "../auth/auth-provider.cpp"
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

#ifndef _STRING_
    #define _STRING_
    #include <string>
#endif

std::string ltgFilePath = std::string("longtermgoal.dat");

void LongTermGoals::incrJournalsLogded () noexcept(false)
{
    std::ofstream wstream;
    wstream.open(ltgFilePath, std::ios::app | std::ios::out);

    if (!wstream.good())
    {
        throw std::runtime_error("Cannot determine the journals lodged count!!");
    }

    this->journals_logded++;
    wstream.seekp((this->LongTermGoalId - 1) * sizeof(LongTermGoals), std::ios::beg);
    wstream.write((char *)this, sizeof(this));
    wstream.close();
}

unsigned int LongTermGoals::getGoalsCount () noexcept(false)
{
    std::ifstream fileptr;
    fileptr.open(ltgFilePath, std::ios::in);
    if (!fileptr.good())
    {
        throw std::runtime_error("Couldn't get the journals to display!!");
    }

    fileptr.seekg(0, std::ios::end);

    unsigned int fileLength = fileptr.tellg();

    fileptr.close();
    return fileLength / sizeof(LongTermGoals);
}

LongTermGoals::LongTermGoals () noexcept
{
    this->LongTermGoalId = 0;
}

LongTermGoals::LongTermGoals (std::string goal) noexcept(false)
{
    auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());

    this->isCompleted = false;
    this->journals_logded = 0;
    this->goal = goal;
    this->creationDate = now.time_since_epoch().count();
    this->lastProgress = 0;
    this->userId = auth::authProvider->getAuthenticatedUserId();

    try
    {
        this->LongTermGoalId = LongTermGoals::getGoalsCount()+1;
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
}

unsigned int LongTermGoals::save () noexcept(false)
{
    std::ofstream writestream;
    writestream.open(ltgFilePath, std::ios::app | std::ios::out);
    if (!writestream.good())
    {
        throw std::runtime_error("Couldn't save the goal in the database");
    }
    writestream << this;
    writestream.close();
    return this->LongTermGoalId;
}

std::vector<LongTermGoals> LongTermGoals::getAllGoals (bool getCompleted) noexcept(false)
{
    std::vector<LongTermGoals> myltg;

    std::ifstream stream;
    stream.open(ltgFilePath, std::ios::in);

    if (!stream.good())
    {
        throw new std::runtime_error("Couldn't get all goals for displaying!!");
    }

    LongTermGoals obj;
    while (!stream.eof())
    {
        stream >> obj;
        if ((obj.userId == auth::authProvider->getAuthenticatedUserId()) && (obj.isCompleted == getCompleted))
        {
            myltg.push_back(obj);
        }
    }
    stream.close();
    return myltg;
}

void LongTermGoals::lodgeJournal (std::string jour) noexcept(false)
{
    LongTermGoalJournal journal(jour, this->LongTermGoalId);
    try
    {
        // this->incrJournalsLogded();
        // TODO: increment journal_lodged counter on file everytime a new journal is lodged
        journal.save(10);
        // TODO: update last progress on longtermgoal every time a new journal for a goal is posted
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
}

void LongTermGoals::markGoalComplete () noexcept(false)
{
    std::ofstream wstream;
    wstream.open(ltgFilePath, std::ios::app | std::ios::out);
    if (!wstream.good())
    {
        throw std::runtime_error("Cannot determine the journals lodged count!!");
    }
    this->isCompleted = true;
    wstream.seekp((this->LongTermGoalId - 1) * sizeof(LongTermGoals), std::ios::beg);
    wstream << this;
    wstream.close();
}

std::vector<LongTermGoalJournal> LongTermGoals::getMyJournals () noexcept(false)
{
    try
    {
        return LongTermGoalJournal::getJournals(this->LongTermGoalId);
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
}

std::ifstream& operator >> (std::ifstream& stream, const LongTermGoals& obj)
{
    stream.read((char *)&obj, sizeof(obj));
    return stream;
}

std::ofstream& operator<<(std::ofstream& stream, const LongTermGoals& obj)
{
    stream.write((char *)&obj, sizeof(obj));
    return stream;
}