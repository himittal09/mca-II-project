#ifndef __LTGH__
    #define __LTGH__
    #include "./long-term-goals.h"
#endif

#ifndef __LTJH__
    #define __LTJH__
    #include "./long-term-journal.h"
#endif

#ifndef __AUTHH__
    #define __AUTHH__
    #include "../auth/auth.h"
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

void LongTermGoals::incrJournalsLogded ()
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

LongTermGoals::LongTermGoals ()
{
    this->LongTermGoalId = 0;
}

LongTermGoals::LongTermGoals (std::string goal)
{
    auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());

    this->isCompleted = false;
    this->journals_logded = 0;
    this->goal = goal;
    this->creationDate = now.time_since_epoch().count();
    this->lastProgress = 0;
    this->userId = auth::auth.getAuthenticatedUserId();

    // init unsigned int LongTermGoalId;

    
    
}

unsigned int LongTermGoals::save ()
{

}

void LongTermGoals::lodgeJournal (std::string jour)
{
    LongTermGoalJournal journal(jour, this->LongTermGoalId);
    try
    {
        // this->incrJournalsLogded();
        // TODO: increment journal_lodged counter on file everytime a new journal is lodged
        journal.save();
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
}

void LongTermGoals::markGoalComplete ()
{

}

std::vector<LongTermGoalJournal> LongTermGoals::getMyJournals () noexcept(false)
{
    return LongTermGoalJournal::getJournals(this->LongTermGoalId);
}

std::ifstream& LongTermGoals::operator >> (std::ifstream& stream)
{
    stream.read((char *)this, sizeof(this));
    return stream;
}

std::ofstream& LongTermGoals::operator<<(std::ofstream& stream)
{
    stream.write((char *)this, sizeof(this));
    return stream;
}
