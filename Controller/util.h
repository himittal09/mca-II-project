#ifndef _CHRONO_
    #define _CHRONO_
    #include <chrono>
#endif

/**
 * This function will return true if val ∈ [floor, ceil]
 * false otherwise
*/
template <class T>
bool clamp (T val, T floor, T ceil);

// from seconds to epoch to timestamp
std::chrono::system_clock::time_point convertTime (int64_t& timePoint);

// from timestamp to months till epoch
long getMonthfromEpoch (std::chrono::system_clock::time_point& timePoint);

// from seconds to epoch to months till epoch
long convertTimeToMonths (int64_t& timePoint);

// difference in months between seconds to epoch and now
long monthDiffFromNow (int64_t& timePoint);

/**
 * calculated difference between lastStreakTime and time now
 * returns true if difference lesser equal to maxAllowedTime
 * false otherwise
 * maxAllowed is in seconds
 * lastStreakTime is in seconds
*/
bool diffBetween (int64_t lastStreakTime, int64_t maxAllowedTime);

// to clear the screen contents
void clearScreen (void);