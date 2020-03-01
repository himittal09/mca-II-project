#ifndef __MONTHLYVH__
    #define __MONTHLYVH__
    #include "./monthly-planner-view.h"
#endif

#ifndef __MONTHLYH__
    #define __MONTHLYH__
    #include "../../Model/monthly-planner/monthly-planner.h"
#endif

#ifndef __UTILSH__
    #define __UTILSH__
    #include "../../Controller/util.h"
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _STDEXCEPT_
    #define _STDEXCEPT_
    #include <stdexcept>
#endif

#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

#ifndef _IOMANIAP_
    #define _IOMANIAP_
    #include <iomanip>
#endif

using namespace std;

bool mpMenu ()
{
    int option = askMPMenu();
    if (option == 1)
    {
        displayMP();
    }
    else if (option == 2)
    {
        addMP();
    }
    else if (option == 4)
    {
        return true;
    }
    return false;
}

int askMPMenu ()
{
    int option=0;
    while (true)
    {
        cout << "\n1. Display all Plans:\n";
        cout << "2. Add new Plan\n";
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

void addMP ()
{
    std::string planner;
    cout << "Enter the planner objective: ";
    std::ws(cin);
    std::getline(cin, planner);
    try
    {
        MonthlyPlanner newPlan = MonthlyPlanner(planner);
        MonthlyPlanner::save(newPlan);
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    cout << "New plan created!!\n";
}

void displayMP ()
{
    char option;
    cout << "\nView completed plans? (y/n)";
    cin >> option;
    bool getCompleted {option == 'y' || option == 'Y'};
    std::vector<MonthlyPlanner> plans{MonthlyPlanner::getallPlans(getCompleted)};
    if (plans.size() == 0)
    {
        cout << "No plans to display...\n";
        return;
    }

    cout << "S.No.\tPlan";
    cout << setw(60);
    cout << "Created At";
    cout << "\n";
    for (int i=0; i<plans.size(); i++)
    {
        cout << i+1 << "\t";
        cout << plans[i].monthlyPlan;
        cout << setw(60);
        cout << printFriendlyDate(plans[i].createdAt);
        cout << "\n";

    }
    if (getCompleted)
    {
        return;
    }
    cout << "\nEnter plan number to complete it (0 to ignore): ";
    int planNum;
    cin >> planNum;
    try
    {
        if (clamp(planNum, 1, plans.size()))
        {
            plans[planNum-1].completePlan();
        }
    }
    catch(const std::runtime_error& e)
    {
        cout << e.what() << '\n';
    }
}

void checkForMissedMP ()
{
    try
    {
        MonthlyPlanner::checkAnRemoveExpiredPlan();
    }
    catch(const std::runtime_error& e)
    {
        std::cout << e.what() << "\n";
    }
    
}