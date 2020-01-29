#ifndef __LTJH__
    #define __LTJH__
    #include "./long-term-journal.h"
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

class LongTermGoals
{
    unsigned int LongTermGoalId;
    unsigned int userId;
    std::string goal;
    int64_t lastProgress;
    int64_t creationDate;
    unsigned int journals_logded;
    bool isCompleted;

    void incrJournalsLogded () noexcept(false);
    static unsigned int getGoalsCount () noexcept(false);

public:
    LongTermGoals () noexcept;
    LongTermGoals (std::string) noexcept(false);
    unsigned int save () noexcept(false);
    static std::vector<LongTermGoals> getAllGoals (bool getCompleted) noexcept(false);
    void lodgeJournal (std::string journalToLodge) noexcept(false);
    void markGoalComplete () noexcept(false);
    std::vector<LongTermGoalJournal> getMyJournals () noexcept(false);

    friend std::ifstream& operator >> (std::ifstream&, const LongTermGoals& obj);
    friend std::ofstream& operator << (std::ofstream&, const LongTermGoals& obj);
};
