#include <iostream>
#include <vector>

class MonthlyPlanner;

#ifndef __MONTHLYH__
#define __MONTHLYH__

class MonthlyPlanner
{
public:
    unsigned int plannerId;
    unsigned int userId;
    std::string monthlyPlan;
    std::string createdAt;
    bool isCompleted;
    
    MonthlyPlanner () noexcept;
    MonthlyPlanner (std::string plan) noexcept(false);

    void completePlan () noexcept(false);
    void save () noexcept(false);
    
    static std::vector<MonthlyPlanner> getallPlans (bool getCompleted) noexcept(false);
    static void checkAnRemoveExpiredPlan () noexcept(false);
};

#endif