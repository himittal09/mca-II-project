#include <iostream>

#include "./long-term-journal.h"

class LongTermGoals;

#ifndef __LTGH__
#define __LTGH__

class LongTermGoals
{

    // void incrJournalsLogded () noexcept(false);
    static unsigned int getGoalsCount () noexcept(false);

public:
    unsigned int LongTermGoalId;
    unsigned int userId;
    std::string goal;
    int64_t lastProgress;
    int64_t creationDate;
    unsigned int journals_logded;
    bool isCompleted;
    
    LongTermGoals () noexcept;
    LongTermGoals (std::string) noexcept(false);

    void save () noexcept(false);
    static std::vector<LongTermGoals> getAllGoals (bool getCompleted) noexcept(false);
    void lodgeJournal (std::string journalToLodge) noexcept(false);
    void markGoalComplete () noexcept(false);
    std::vector<LongTermGoalJournal> getMyJournals () noexcept(false);

    friend std::ifstream& operator >> (std::ifstream&, LongTermGoals& obj);
    friend std::ofstream& operator << (std::ofstream&, LongTermGoals& obj);
};

#endif