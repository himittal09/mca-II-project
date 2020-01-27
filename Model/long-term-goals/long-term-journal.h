#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

class LongTermGoalJournal
{
    std::string journal;
    int64_t lodgeDate;
    unsigned int longTermGoalId;

public:
    LongTermGoalJournal () noexcept;
    LongTermGoalJournal (std::string journal, unsigned int ltgId) noexcept;
    void save (unsigned int journalsLodged) const noexcept(false);
    static std::vector<LongTermGoalJournal> getJournals (unsigned int goalId) noexcept(false);

    std::ifstream& operator >> (std::ifstream&);
    std::ofstream& operator << (std::ofstream&);
};