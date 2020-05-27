#include <iostream>
#include <vector>

class MonthlyPlanner
{
    static unsigned int getPlannerCount () noexcept(false);
public:
    unsigned int plannerId;
    unsigned int userId;
    std::string monthlyPlan;
    int64_t createdAt;
    bool isCompleted;
    
    MonthlyPlanner () noexcept;
    MonthlyPlanner (std::string plan) noexcept(false);

    void completePlan () noexcept(false);
    
    void save () noexcept(false);
    static std::vector<MonthlyPlanner> getallPlans (bool getCompleted) noexcept(false);
    static void checkAnRemoveExpiredPlan () noexcept(false);

    friend std::ifstream& operator >> (std::ifstream&, MonthlyPlanner& obj);
    friend std::ofstream& operator << (std::ofstream&, MonthlyPlanner& obj);
};