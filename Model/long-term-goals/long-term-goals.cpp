#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

#include "../../Controller/util.h"
#include "./long-term-goals.h"
#include "./long-term-journal.h"
#include "../../Controller/auth-provider.h"

std::string ltgFilePath = std::string("longtermgoal.dat");

// void LongTermGoals::incrJournalsLogded () noexcept(false)
// {
//     std::ofstream wstream {ltgFilePath, std::ios::app | std::ios::out};
//     if (!wstream.is_open())
//     {
//         throw std::runtime_error("Cannot determine the journals lodged count!!");
//     }

//     this->journals_logded++;
//     wstream.seekp((this->LongTermGoalId - 1) * sizeof(LongTermGoals), std::ios::beg);
//     wstream.write((char *)this, sizeof(this));
// }

unsigned int LongTermGoals::getGoalsCount () noexcept(false)
{
    std::ifstream fileptr {ltgFilePath, std::ios::in | std::ios::app};
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

LongTermGoals::LongTermGoals () noexcept :LongTermGoalId {0} { }

LongTermGoals::LongTermGoals (std::string goal) noexcept(false)
    :isCompleted {false},
    journals_logded {0},
    goal {goal},
    creationDate {getCurrentTime()},
    lastProgress {0},
    userId {authProvider->getAuthenticatedUserId()},
    LongTermGoalId {LongTermGoals::getGoalsCount() + 1}
{ }

void LongTermGoals::save () noexcept(false)
{
    std::ofstream writestream {ltgFilePath, std::ios::app | std::ios::out};
    if (!writestream.is_open())
    {
        throw std::runtime_error("Couldn't save the goal in the database");
    }

    writestream << *this;
}

std::vector<LongTermGoals> LongTermGoals::getAllGoals (bool getCompleted) noexcept(false)
{
    std::ifstream stream {ltgFilePath, std::ios::in | std::ios::app};
    if (!stream.is_open())
    {
        throw new std::runtime_error("Couldn't get all goals for displaying!!");
    }

    std::vector<LongTermGoals> myltg;
    unsigned int authenticatedUserId {authProvider->getAuthenticatedUserId()};

    for (LongTermGoals obj; !(stream >> obj).eof(); )
    {
        if ((obj.userId == authenticatedUserId) && (obj.isCompleted == getCompleted))
        {
            myltg.push_back(obj);
        }
    }
    return myltg;
}

void LongTermGoals::lodgeJournal (std::string jour) noexcept(false)
{
    LongTermGoalJournal journal(jour, this->LongTermGoalId);
    // this->incrJournalsLogded();
    // TODO: increment journal_lodged counter on file everytime a new journal is lodged
    journal.save(10);
    // TODO: update last progress on longtermgoal every time a new journal for a goal is posted
}

void LongTermGoals::markGoalComplete () noexcept(false)
{
    if (this->isCompleted)
    {
        return;
    }
    
    this->isCompleted = true;

    std::ifstream rstream {ltgFilePath, std::ios::in | std::ios::app};
    if (!rstream.is_open())
    {
        throw std::runtime_error("Couldn't update the todo!!");
    }

    std::ofstream wstream {"temp.dat", std::ios::out};
    if (!wstream.is_open())
    {
        throw std::runtime_error("Couldn't update the todo!!");
    }

    for (LongTermGoals obj; !(rstream >> obj).eof(); )
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
    return LongTermGoalJournal::getJournals(this->LongTermGoalId);
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