#ifndef __UTILSH__
    #define __UTILSH__
    #include "./util.h"
#endif

#ifndef __DATEH__
    #define __DATEH__
    #include "./date.h"
#endif

#ifndef _CHRONO_
    #define _CHRONO_
    #include <chrono>
#endif

#ifndef _CTIME_
    #define _CTIME_
    #include <ctime>
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _CSTDLIB_
    #define _CSTDLIB_
    #include <cstdlib>
#endif

bool clamp (int val, int floor, int ceil)
{
    return ((val <= ceil) && (val >= floor));
}

std::chrono::system_clock::time_point convertTime (int64_t& timePoint)
{
    std::chrono::system_clock::time_point start(std::chrono::seconds((long)timePoint));
    return start;
}

long getMonthfromEpoch (std::chrono::system_clock::time_point& timePoint)
{
    time_t tt = std::chrono::system_clock::to_time_t(timePoint);
    tm utc_tm = *gmtime(&tt);
    return (utc_tm.tm_year * 12) + utc_tm.tm_mon + 1;
}

long convertTimeToMonths (int64_t& timePoint)
{
    auto inter = convertTime(timePoint);
    return getMonthfromEpoch(inter);
}

long monthDiffFromNow (int64_t& timePoint)
{
    using namespace std::chrono;

    system_clock::time_point start(seconds((long)timePoint));
    system_clock::time_point end(system_clock::now());

    nanoseconds diff(end - start);

    using monthDef = duration<int64_t, std::ratio<2629746>>;

    return duration_cast<monthDef>(diff).count();
}

bool diffBetween (int64_t& lastStreakTime, int64_t& maxAllowedTime)
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

void clearScreen (void)
{
    std::cout << "\033[2J\033[1;1H";
}

std::string printFriendlyDate (int64_t& myTime)
{
    if (myTime == 0)
    {
        return std::string("-");
    }
    return date::format("[%R %D]", convertTime(myTime));
}