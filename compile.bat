g++ -std=c++14 -c ./Controller/util.cpp -o ./Build/__UTILS__.o
g++ -std=c++14 -c ./Model/auth/user.cpp -o ./Build/__USER__.o
g++ -std=c++14 -c ./Model/auth/auth.cpp -o ./Build/__AUTH__.o
g++ -std=c++14 -c ./Model/monthly-planner/monthly-planner.cpp -o ./Build/__MONTHLY__.o
g++ -std=c++14 -c ./Model/notification/notification.cpp -o ./Build/__NOTIFICATION__.o
g++ -std=c++14 -c ./Model/todos/todo.cpp -o ./Build/__TODO__.o
g++ -std=c++14 -c ./Model/activity-tracker/activity-tracker.cpp -o ./Build/__AT__.o
g++ -std=c++14 -c ./Model/long-term-goals/long-term-goals.cpp -o ./Build/__LTG__.o
g++ -std=c++14 -c ./Model/long-term-goals/long-term-journal.cpp -o ./Build/__LTJ__.o
g++ -std=c++14 -c ./View/auth.cpp -o ./Build/__AUTHV__.o
g++ -std=c++14 -c ./View/menus.cpp -o ./Build/__MENUV__.o
g++ -std=c++14 -c ./View/Main/mainmenu.cpp -o ./Build/__MMENUV__.o
g++ -std=c++14 -c ./main.cpp -o ./Build/__MAINFILE__.o

del *.o