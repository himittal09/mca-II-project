#ifndef __LTGH__
    #define __LTGH__
    #include "./long-term-goals.h"
#endif

#ifndef __LTJH__
    #define __LTJH__
    #include "./long-term-journal.h"
#endif

#ifndef __APH__
    #define __APH__
    #include "../../Controller/auth-provider.h"
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

#ifndef _STRING_
    #define _STRING_
    #include <string>
#endif

std::string ltgFilePath = std::string("longtermgoal.dat");

// void LongTermGoals::incrJournalsLogded () noexcept(false)
// {
//     std::ofstream wstream;
//     wstream.open(ltgFilePath, std::ios::app | std::ios::out);

//     if (!wstream.is_open())
//     {
//         throw std::runtime_error("Cannot determine the journals lodged count!!");
//     }

//     this->journals_logded++;
//     wstream.seekp((this->LongTermGoalId - 1) * sizeof(LongTermGoals), std::ios::beg);
//     wstream.write((char *)this, sizeof(this));
//     wstream.close();
// }

unsigned int LongTermGoals::getGoalsCount () noexcept(false)
{
    std::ifstream fileptr;
    fileptr.open(ltgFilePath, std::ios::in | std::ios::app);
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

LongTermGoals::LongTermGoals () noexcept
{
    this->LongTermGoalId = 0;
}

LongTermGoals::LongTermGoals (std::string goal) noexcept(false)
{
    this->isCompleted = false;
    this->journals_logded = 0;
    this->goal = goal;
    this->creationDate = getCurrentTime();
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

void LongTermGoals::save (LongTermGoals& obj) noexcept(false)
{
    std::ofstream writestream;
    writestream.open(ltgFilePath, std::ios::app | std::ios::out);
    if (!writestream.is_open())
    {
        throw std::runtime_error("Couldn't save the goal in the database");
    }
    writestream << obj;
    writestream.close();
}

std::vector<LongTermGoals> LongTermGoals::getAllGoals (bool getCompleted) noexcept(false)
{
    std::vector<LongTermGoals> myltg;

    std::ifstream stream;
    stream.open(ltgFilePath, std::ios::in | std::ios::app);

    if (!stream.is_open())
    {
        throw new std::runtime_error("Couldn't get all goals for displaying!!");
    }

    LongTermGoals obj;
    while (!(stream >> obj).eof())
    {
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
        LongTermGoalJournal::save(journal, 10);
        // TODO: update last progress on longtermgoal every time a new journal for a goal is posted
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
}

void LongTermGoals::markGoalComplete () noexcept(false)
{
    if (this->isCompleted)
    {
        return;
    }
    
    this->isCompleted = true;

    std::ifstream rstream;
    rstream.open(ltgFilePath, std::ios::in | std::ios::app);
    if (!rstream.is_open())
    {
        throw std::runtime_error("Couldn't update the todo!!");
    }

    std::ofstream wstream;
    wstream.open("temp.dat", std::ios::out);
    if (!wstream.is_open())
    {
        throw std::runtime_error("Couldn't update the todo!!");
    }

    LongTermGoals obj;

    while (!(rstream >> obj).eof())
    {
        if (obj.LongTermGoalId == this->LongTermGoalId)
        {
            obj.isCompleted = true;
        }
        wstream << obj;
    }

    wstream.close();
    rstream.close();

    remove(ltgFilePath.c_str());
    rename("temp.dat", ltgFilePath.c_str());
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

std::ifstream& operator >> (std::ifstream& stream, LongTermGoals& obj)
{
    std::ws(stream);
    std::getline(stream, obj.goal, '$');
    stream >> obj.LongTermGoalId >> obj.userId >> obj.lastProgress >> obj.creationDate;
    stream >> obj.journals_logded >> obj.isCompleted;
    return stream;
}

std::ofstream& operator<<(std::ofstream& stream, LongTermGoals& obj)
{
    stream << obj.goal << "$" << obj.LongTermGoalId << " " << obj.userId << " " << obj.lastProgress << " ";
    stream <<  obj.creationDate << " " << obj.journals_logded << " " << obj.isCompleted << "\n";
    return stream;
}