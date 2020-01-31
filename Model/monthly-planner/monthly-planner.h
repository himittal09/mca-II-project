#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

class MonthlyPlanner
{
    unsigned int plannerId;
    unsigned int userId;
    std::string monthlyPlan;
    int64_t createdAt;
    bool isCompleted;

    static unsigned int getPlannerCount () noexcept(false);
public:
    MonthlyPlanner () noexcept;
    MonthlyPlanner (std::string plan) noexcept(false);

    void completePlan () noexcept(false);
    
    static void save (MonthlyPlanner& obj) noexcept(false);
    static std::vector<MonthlyPlanner> getallPlans (bool getCompleted) noexcept(false);
    static void checkAnRemoveExpiredPlan () noexcept(false);

    friend std::ifstream& operator >> (std::ifstream&, MonthlyPlanner& obj);
    friend std::ofstream& operator << (std::ofstream&, MonthlyPlanner& obj);
};