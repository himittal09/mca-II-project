#include <ctime>
#include <sstream>
#include <regex>
#include <iostream>
#include <chrono>

/**
 * This function will return true if val ∈ [floor, ceil]
 * false otherwise
*/
bool clamp (int val, int floor, int ceil);

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
bool diffBetween (int64_t& lastStreakTime, int64_t& maxAllowedTime);

// to clear the screen contents
void clearScreen (void);

std::string printFriendlyDate (int64_t& myTime);

int64_t getCurrentTime ();

bool isEmailValid (const std::string& email);

std::string getLineWithoutEcho ();




#ifndef __UTILSH__
#define __UTILSH__

inline bool clamp (int val, int floor, int ceil)
{
    return ((val <= ceil) && (val >= floor));
}

inline void clearScreen (void)
{
    std::cout << "\033[2J\033[1;1H";
}

inline long convertTimeToMonths (int64_t& timePoint)
{
    std::chrono::system_clock::time_point inter(std::chrono::seconds((long)timePoint));
    time_t tt = std::chrono::system_clock::to_time_t(inter);
    tm *utc_tm = localtime(&tt);
    return (utc_tm->tm_year * 12) + utc_tm->tm_mon + 1;
}

inline long monthDiffFromNow (int64_t& timePoint)
{
    using namespace std::chrono;

    system_clock::time_point start(seconds((long)timePoint));
    system_clock::time_point end(system_clock::now());

    nanoseconds diff(end - start);

    using monthDef = duration<int64_t, std::ratio<2629746>>;

    return duration_cast<monthDef>(diff).count();
}

inline bool diffBetween (int64_t& lastStreakTime, int64_t& maxAllowedTime)
{
    using namespace std::chrono;

    system_clock::time_point start(seconds((long)lastStreakTime));
    system_clock::time_point end(system_clock::now());

    system_clock::duration diff(end - start);

    // for reference
    // std::chrono::seconds 	duration</*signed integer type of at least 35 bits*/>
    int64_t diffInSystem = duration_cast<duration<int64_t> >(diff).count();

    return (diffInSystem <= maxAllowedTime);
}

inline std::string printFriendlyDate (int64_t& myTime)
{
    if (myTime == 0)
    {
        return std::string("-");
    }
    std::chrono::system_clock::time_point myTimeSys(std::chrono::seconds((long)myTime));

    time_t tt = std::chrono::system_clock::to_time_t(myTimeSys);
    tm *local_tm = localtime(&tt);

    std::stringstream mystr;

    mystr << "[" << local_tm->tm_hour << ":" << local_tm->tm_min << " " << local_tm->tm_mday;
    mystr << "/" << local_tm->tm_mon + 1 << "/" << (local_tm->tm_year + 1900) << "]";

    return mystr.str();
}

inline int64_t getCurrentTime ()
{
    auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
    return now.time_since_epoch().count();
}

inline bool isEmailValid (const std::string& email)
{
    const std::regex pattern("^\\S+@[^\\s@]+\\.[^\\s@.]+$");
    return std::regex_match(email, pattern);
}

inline std::string getLineWithoutEcho ()
{
    
}

#endif