#include <iostream>
#include "./authview.hpp"
#include "./Main/mainmenu.hpp"

using namespace std;

namespace view {
    void welcomeBanner ();
}

#ifndef __MENUVH__
#define __MENUVH__

inline void view::welcomeBanner ()
{
    cout << "*" << std::string(155, '*') << "*\n";
    cout << "*" << std::string(155, ' ') << "*\n";
    cout << "*" << std::string(62, ' ') << "Welcome to Productivity Tracker" << std::string(62, ' ') << "*\n";
    cout << "*" << std::string(155, ' ') << "*\n";
    cout << "*" << std::string(155, '*') << "*\n";
    cout << "\n";
}

#endif