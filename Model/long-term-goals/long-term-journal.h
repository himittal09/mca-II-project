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
public:
    std::string journal;
    int64_t lodgeDate;
    unsigned int longTermGoalId;
    LongTermGoalJournal () noexcept;
    LongTermGoalJournal (std::string journal, unsigned int ltgId) noexcept;
    
    void save (unsigned int journalsLodged) noexcept(false);
    static std::vector<LongTermGoalJournal> getJournals (unsigned int goalId) noexcept(false);

    friend std::ifstream& operator >> (std::ifstream&, LongTermGoalJournal& obj);
    friend std::ofstream& operator << (std::ofstream&, LongTermGoalJournal& obj);
};