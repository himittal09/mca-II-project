mkdir Build

g++ -std=c++14 -c ./main.cpp -o ./Build/__MAINFILE__.o
g++ -std=c++14 -c ./Controller/util.cpp -o ./Build/__UTILS__.o
g++ -std=c++14 -c ./Model/auth/user.cpp -o ./Build/__USER__.o
g++ -std=c++14 -c ./Model/auth/auth.cpp -o ./Build/__AUTH__.o
g++ -std=c++14 -c ./Model/monthly-planner/monthly-planner.cpp -o ./Build/__MONTHLY__.o
g++ -std=c++14 -c ./Model/notification/notification.cpp -o ./Build/__NOTIFICATION__.o
g++ -std=c++14 -c ./Model/todos/todo.cpp -o ./Build/__TODO__.o
g++ -std=c++14 -c ./Model/activity-tracker/activity-tracker.cpp -o ./Build/__AT__.o
g++ -std=c++14 -c ./Model/long-term-goals/long-term-goals.cpp -o ./Build/__LTG__.o
g++ -std=c++14 -c ./Model/long-term-goals/long-term-journal.cpp -o ./Build/__LTJ__.o
g++ -std=c++14 -c ./View/authview.cpp -o ./Build/__AUTHV__.o
g++ -std=c++14 -c ./View/menus.cpp -o ./Build/__MENUV__.o
g++ -std=c++14 -c ./View/Main/mainmenu.cpp -o ./Build/__MMENUV__.o
g++ -std=c++14 -c ./View/Main/activity-tracker-view.cpp -o ./Build/__ATV__.o
g++ -std=c++14 -c ./View/Main/long-term-goal-view.cpp -o ./Build/__LTGV__.o
g++ -std=c++14 -c ./View/Main/monthly-planner-view.cpp -o ./Build/__MONTHLYV__.o
g++ -std=c++14 -c ./View/Main/todo-list-view.cpp -o ./Build/__TODOV__.o

cd Build

g++ -std=c++14 ./__MAINFILE__.o ./__UTILS__.o ./__USER__.o ./__AUTH__.o ./__MONTHLY__.o ./__NOTIFICATION__.o ./__TODO__.o ./__AT__.o ./__LTG__.o ./__LTJ__.o ./__AUTHV__.o ./__MENUV__.o ./__MMENUV__.o ./__ATV__.o ./__LTGV__.o ./__MONTHLYV__.o ./__TODOV__.o -o ../ProductivityTracker.exe

del *.o
cd ..
rmdir Build
