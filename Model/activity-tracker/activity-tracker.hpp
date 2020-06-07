#include <vector>
#include <iostream>

class ActivityTracker;

#ifndef __ATH__
#define __ATH__

class ActivityTracker
{
public:
    // after init, push notifications for all streaks (uncompleted)
    unsigned int activityId;
    unsigned int longestStreak;
    std::string lastCheckIn; // input by system in seconds
    int64_t streakDuration; // input in minutes, store in seconds
    std::string createdAt;
    std::string activity;
    unsigned int userId;
    
    ActivityTracker () noexcept;
    // input streak duration in minutes in this constructor
    ActivityTracker (std::string plan, int64_t streakTime) noexcept(false);

    void save () noexcept(false);
    static std::vector<ActivityTracker> getAllActivity () noexcept(false);
    static void checkForAllStreakMiss ();

    void checkIn ();
};

#endif