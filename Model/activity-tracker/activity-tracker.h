#include <vector>
#include <iostream>

class ActivityTracker;

#ifndef __ATH__
#define __ATH__

class ActivityTracker
{

    static unsigned int getActivityCount () noexcept(false);
public:
    // after init, push notifications for all streaks (uncompleted)
    unsigned int activityId;
    unsigned int longestStreak;
    int64_t lastCheckIn; // input by system in seconds
    int64_t streakDuration; // input in minutes, store in seconds
    int64_t createdAt;
    std::string activity;
    unsigned int userId;
    
    ActivityTracker () noexcept;
    // input streak duration in minutes in this constructor
    ActivityTracker (std::string plan, int64_t streakTime) noexcept(false);

    void save () noexcept(false);
    static std::vector<ActivityTracker> getAllActivity () noexcept(false);
    static void checkForAllStreakMiss ();

    void checkIn ();

    friend std::ifstream& operator >> (std::ifstream& stream, ActivityTracker& obj);
    friend std::ofstream& operator << (std::ofstream& stream, ActivityTracker& obj);
};

#endif