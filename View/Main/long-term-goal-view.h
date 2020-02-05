#ifndef __LTGH__
    #define __LTGH__
    #include "../../Model/long-term-goals/long-term-goals.h"
#endif

bool ltgMenu ();
int askLTGMenu ();
void addLTG ();
void displayLTG ();
int askLTGSubMenu ( bool completed);
void execLTGSubMenu (LongTermGoals& obj, int order);