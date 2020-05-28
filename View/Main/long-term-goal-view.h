#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

#include "../../Controller/util.h"
#include "../../Model/long-term-goals/long-term-goals.h"
#include "../../Model/long-term-goals/long-term-journal.h"

using namespace std;

namespace view {
    bool ltgMenu ();
    int askLTGMenu ();
    void addLTG ();
    void displayLTG ();
    int askLTGSubMenu ( bool completed);
    void execLTGSubMenu (LongTermGoals& obj, int order);
}

#ifndef __LTGVH__
#define __LTGVH__

inline bool view::ltgMenu ()
{
    int option = view::askLTGMenu();
    if (option == 1)
    {
        view::displayLTG();
    }
    else if (option == 2)
    {
        view::addLTG();
    }
    else if (option == 4)
    {
        return true;
    }
    return false;
}

inline int view::askLTGMenu ()
{
    int option=0;
    while (true)
    {
        cout << "\n1. Display all Goals:\n";
        cout << "2. Add new Goal\n";
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

inline void view::addLTG ()
{
    std::string goal;
    cout << "Enter the goal objective: ";
    std::ws(cin);
    std::getline(cin, goal);
    try
    {
        LongTermGoals newTodo = LongTermGoals(goal);
        newTodo.save();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    cout << "New goal created!!\n";
}

inline void view::displayLTG ()
{
    char option;
    cout << "\nView completed goals? (y/n)";
    cin >> option;
    bool getCompleted {option == 'y' || option == 'Y'};
    std::vector<LongTermGoals> goals{LongTermGoals::getAllGoals(getCompleted)};
    if (goals.size() == 0)
    {
        cout << "No goals to display...\n";
        return;
    }

    cout << "S.No.\tGoal";
    cout << setw(60);
    cout << "Created At";
    cout << "\n";
    for (int i=0; i<goals.size(); i++)
    {
        cout << i+1 << "\t";
        cout << goals[i].goal;
        cout << setw(60);
        cout << printFriendlyDate(goals[i].creationDate);
        cout << "\n";
    }
    cout << "\nEnter goal number to choose actions for it (0 to ignore): ";
    int goalNum, subOrder;
    cin >> goalNum;
    if (!clamp(goalNum, 1, goals.size()))
    {
        return;
    }
    try
    {
        view::execLTGSubMenu (goals[goalNum-1], view::askLTGSubMenu(goals[goalNum-1].isCompleted));
    }
    catch(const std::runtime_error& e)
    {
        cout << e.what() << '\n';
    }
}

inline int view::askLTGSubMenu (bool completed)
{
    int option=0;
    while (true)
    {
        cout << "\n1. View all journals for the goal\n";
        if (!completed)
        {
            cout << "2. Add new journal for goal\n";
            cout << "3. Mark the Goal Complete\n";
        }
        cout << "Choose a option: ";
        cin >> option;
        if (clamp(option, 1, completed ? 1 : 3))
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

inline void view::execLTGSubMenu (LongTermGoals& obj, int order)
{
    if (order == 1)
    {
        std::vector<LongTermGoalJournal> journals{obj.getMyJournals()};
        if (journals.size() == 0)
        {
            cout << "No journals to display...\n";
            return;
        }

        cout << "Journal";
        cout << setw(80);
        cout << "Lodge Date";
        cout << "\n";

        for (LongTermGoalJournal& jour: journals)
        {
            cout << jour.journal;
            cout << setw(80);
            cout << printFriendlyDate(jour.lodgeDate);
            cout << "\n";
        }
    }
    else if (order == 2)
    {
        string str;
        cout << "Enter journal: ";
        ws(cin);
        getline(cin, str);
        obj.lodgeJournal(str);
        cout << "\nJournal Logded!!\n";
    }
    else
    {
        obj.markGoalComplete();
    }
}

#endif