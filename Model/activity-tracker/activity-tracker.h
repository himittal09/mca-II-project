#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

/**
 * An activity tracker, where the user will not have any time duration to complete the
task, but he will have to do it/ complete it in some fixed interval. Users can be notified
to complete the task, or they not be notified. Users will have to check in for their task
to mark an interval complete. We can have streaks for user to further motivate them
to complete their goals in time.
*/
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
