#ifndef __LTJH__
    #define __LTJH__
    #include "./long-term-journal.h"
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

std::string ltgjournals = std::string("ltgjournal.dat");

LongTermGoalJournal::LongTermGoalJournal () noexcept
{
    this->longTermGoalId = 0;
}

LongTermGoalJournal::LongTermGoalJournal (std::string journal, unsigned int ltgId) noexcept
{
    this->lodgeDate = getCurrentTime();
    this->journal = journal;
    this->longTermGoalId = ltgId;
}

// TODO: take input number of journals logded, and delete excess journals
// keep only 10
void LongTermGoalJournal::save (unsigned int journalsLodged=0) noexcept(false)
{
    std::ofstream wfileptr {ltgjournals, std::ios::app | std::ios::out};
    if (!wfileptr.is_open())
    {
        throw std::runtime_error("Couldnot save the journal for the Goal!!");
    }

    wfileptr << *this;

    // if (journalsLodged > 10)
    // {
    //     // delete a journal
    //     std::ifstream rfileptr {ltgjournals, std::ios::in | std::ios::app};
    //     if (!rfileptr.is_open())
    //     {
    //         throw std::runtime_error("Unable to process deletion of extra journals");
    //     }

    //     bool recordToDeleteMissed = false;

    //     wfileptr.open("temp.dat",std::ios::out);
    //     if (!wfileptr.is_open())
    //     {
    //         throw std::runtime_error("Unable to process deletion of extra journals");
    //     }

    //     LongTermGoalJournal ltgJournal;
    //     while (rfileptr.read((char *)&ltgJournal, sizeof(ltgJournal)))
    //     {
    //         if (ltgJournal.longTermGoalId == goalId)
    //         {
    //             if (!recordToDeleteMissed)
    //             {
    //                 recordToDeleteMissed = true;
    //             }
    //             else
    //             {
    //                 wfileptr.write((char *)&ltgJournal, sizeof(ltgJournal));
    //             }
                
    //         }
    //     }

    //     wfileptr.close();
    //     rfileptr.close();

    //     remove(ltgjournals);
    //     rename("temp.dat", ltgjournals);
    // }
}

std::vector<LongTermGoalJournal> LongTermGoalJournal::getJournals (unsigned int goalId) noexcept(false)
{
    std::ifstream fileptr {ltgjournals, std::ios::in | std::ios::app};
    if (!fileptr.is_open())
    {
        throw std::runtime_error("Couldn't get the journals to display!!");
    }

    std::vector<LongTermGoalJournal> journals;

    for (LongTermGoalJournal obj; !(fileptr >> obj).eof(); )
    {        
        if (obj.longTermGoalId == goalId)
        {
            journals.push_back(obj);
        }
    }
    return journals;
}

std::ifstream& operator >> (std::ifstream& stream, LongTermGoalJournal& obj)
{
    std::ws(stream);
    std::getline(stream, obj.journal, '$');
    stream >> obj.lodgeDate >> obj.longTermGoalId;
    return stream;
}

std::ofstream& operator<<(std::ofstream& stream, LongTermGoalJournal& obj)
{
    stream << obj.journal << "$" << obj.lodgeDate << " " << obj.longTermGoalId << "\n";
    return stream;
}
