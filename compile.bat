mkdir Build

g++ -std=c++14 -c ./main.cpp -o ./Build/__MAINFILE__.o
g++ -std=c++14 -c ./Model/auth/user.cpp -o ./Build/__USER__.o
g++ -std=c++14 -c ./Model/auth/auth.cpp -o ./Build/__AUTH__.o
g++ -std=c++14 -c ./Model/monthly-planner/monthly-planner.cpp -o ./Build/__MONTHLY__.o
g++ -std=c++14 -c ./Model/notification/notification.cpp -o ./Build/__NOTIFICATION__.o
g++ -std=c++14 -c ./Model/todos/todo.cpp -o ./Build/__TODO__.o
g++ -std=c++14 -c ./Model/activity-tracker/activity-tracker.cpp -o ./Build/__AT__.o
g++ -std=c++14 -c ./Model/long-term-goals/long-term-goals.cpp -o ./Build/__LTG__.o

cd Build

g++ -std=c++14 ./__MAINFILE__.o ./__USER__.o ./__AUTH__.o ./__MONTHLY__.o ./__NOTIFICATION__.o ./__TODO__.o ./__AT__.o ./__LTG__.o -lpqxx -lpq -o ../ProductivityTracker.exe

del *.o
cd ..
rmdir Build
