#include <ctime>
#include <sstream>
#include <regex>
#include <iostream>
#include <chrono>
#include <iomanip>

/**
 * This function will return true if val ∈ [floor, ceil]
 * false otherwise
*/
bool clamp (int val, int floor, int ceil);

// to clear the screen contents
void clearScreen (void);

bool isEmailValid (const std::string& email);

std::string getLineWithoutEcho ();

std::string getPostgresDate ();

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

inline bool isEmailValid (const std::string& email)
{
    const std::regex pattern("^\\S+@[^\\s@]+\\.[^\\s@.]+$");
    return std::regex_match(email, pattern);
}

inline std::string getLineWithoutEcho ()
{
    
}

inline std::string getPostgresDate ()
{
    auto now = std::chrono::system_clock::now();

    time_t tt = std::chrono::system_clock::to_time_t(now);

    tm utc_tm = *gmtime(&tt);
    
    std::stringstream ss;

    ss << (utc_tm.tm_year + 1900) << "-";
    ss << std::setfill('0') << std::setw(2);
    ss << (utc_tm.tm_mon + 1) << "-";
    ss << std::setfill('0') << std::setw(2);
    ss << utc_tm.tm_mday;

    return ss.str();
}

#endif