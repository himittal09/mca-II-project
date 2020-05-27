#include <iostream>
#include <stdexcept>
#include <vector>
#include <iomanip>

using namespace std;

#include "../../Controller/util.h"

#ifndef __ATH__
    #define __ATH__
    #include "../../Model/activity-tracker/activity-tracker.h"
#endif

bool atMenu ();
int askATMenu ();
void addAT ();
void displayAT ();
void checkForMissedAT ();

#ifndef __ATVH__
#define __ATVH__


inline bool atMenu ()
{
    int option = askATMenu();
    if (option == 1)
    {
        displayAT();
    }
    else if (option == 2)
    {
        addAT();
    }
    else if (option == 4)
    {
        return true;
    }
    return false;
}

inline int askATMenu ()
{
    int option=0;
    while (true)
    {
        cout << "\n1. Display all Activities:\n";
        cout << "2. Add new Activity\n";
        cout << "3. Main Menu\n";
        cout << "4. Exit\n";
        cout << "Choose a option: ";
        cin >> option;
        if (clamp(option, 1, 4))
        {
            break;
        }
        else
        {
            cout << "\nPlease choose an option amongst the following: ";
        }
    }
    return option;
}

inline void addAT ()
{
    std::string activityT;
    cout << "Enter the todo objective: ";
    std::ws(cin);
    std::getline(cin, activityT);
    int64_t streakDuration = 0;
    cout << "Enter the streak duration (in minutes):";
    cin >> streakDuration;
    try
    {
        ActivityTracker newActivity = ActivityTracker(activityT, streakDuration);
        newActivity.save();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    cout << "New activity created!!\n";
}

inline void displayAT ()
{
    std::vector<ActivityTracker> activities{ActivityTracker::getAllActivity()};
    if (activities.size() == 0)
    {
        cout << "No activity to display...\n";
        return;
    }

    cout << "S.No.\tActivity";
    cout << setw(60);
    cout << "Longest Streak";
    cout << "\t\tLast CheckIn At\t\tStreak Duration\n";
    for (int i=0; i<activities.size(); i++)
    {
        cout << i+1 << "\t" << activities[i].activity;
        cout << setw(60);
        cout << activities[i].longestStreak;
        cout << "\t\t" << printFriendlyDate(activities[i].lastCheckIn) << "\t\t";
        cout << activities[i].streakDuration / 60 << "\n";
    }
    cout << "\nEnter activity number to Check In (0 to ignore): ";
    int activityNum;
    cin >> activityNum;
    try
    {
        if (clamp(activityNum, 1, activities.size()))
        {
            activities[activityNum-1].checkIn();
        }
    }
    catch(const std::runtime_error& e)
    {
        cout << e.what() << '\n';
    }
}

inline void checkForMissedAT ()
{
    try
    {
        ActivityTracker::checkForAllStreakMiss();
    }
    catch(const std::runtime_error& e)
    {
        std::cout << e.what() << "\n";
    }
    
}

#endif