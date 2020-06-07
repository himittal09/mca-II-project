#include <iostream>

class LongTermGoals;

struct LongTermGoalJournalPartial;

#ifndef __LTGH__
#define __LTGH__

class LongTermGoals
{
public:
    unsigned int LongTermGoalId;
    unsigned int userId;
    std::string goal;
    std::string lastProgress;
    std::string creationDate;
    unsigned int journals_logded;
    bool isCompleted;
    
    LongTermGoals () noexcept;
    LongTermGoals (std::string) noexcept(false);

    void save () noexcept(false);
    static std::vector<LongTermGoals> getAllGoals (bool getCompleted) noexcept(false);
    void lodgeJournal (std::string journalToLodge) noexcept(false);
    void markGoalComplete () noexcept(false);
    std::vector<LongTermGoalJournalPartial> getMyJournals () noexcept(false);
};

struct LongTermGoalJournalPartial
{
    std::string journal;
    std::string lodgeDate;
};

#endif