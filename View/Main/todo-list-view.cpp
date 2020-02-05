#ifndef __TODOVH__
    #define __TODOVH__
    #include "./todo-list-view.h"
#endif

#ifndef __UTILSH__
    #define __UTILSH__
    #include "../../Controller/util.h"
#endif

#ifndef __TODOH__
    #define __TODOH__
    #include "../../Model/todos/todo.h"
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

bool todoMenu ()
{
    int option = askTodoMenu();
    if (option == 1)
    {
        displayTodos();
    }
    else if (option == 2)
    {
        addTodo();
    }
    else if (option == 4)
    {
        return true;
    }
    return false;
}

int askTodoMenu ()
{
    int option=0;
    while (true)
    {
        cout << "\n1. Display all Todos:\n";
        cout << "2. Add new Todo\n";
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

void addTodo ()
{
    std::string todo;
    cout << "Enter the todo objective: ";
    std::ws(cin);
    std::getline(cin, todo);
    try
    {
        Todo newTodo = Todo(todo);
        Todo::save(newTodo);
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    cout << "New todo created!!\n";
}

void displayTodos ()
{
    char option;
    int todoNum;
    cout << "\nView completed todos? (y/n)";
    cin >> option;
    bool getCompleted {option == 'y' || option == 'Y'};
    std::vector<Todo> todos{Todo::getAllTodos(getCompleted)};
    if (todos.size() == 0)
    {
        cout << "No todos to display...\n";
        return;
    }
    cout << "S.No.\tTodo";
    cout << setw(60);
    cout << "Created At";
    cout << setw(1);
    cout << "\t\tCompleted At\n";
    for (int i=0; i<todos.size(); i++)
    {
        cout << i+1 << "\t";
        cout << todos[i].todo;
        cout << setw(60);
        cout << printFriendlyDate(todos[i].createdAt);
        cout << setw(1);
        cout << "\t\t" << printFriendlyDate(todos[i].completedAt) << "\n";

    }
    if (getCompleted)
    {
        return;
    }
    cout << "\nEnter todo number to complete it (0 to ignore): ";
    cin >> todoNum;
    try
    {
        if (clamp(todoNum, 1, todos.size()))
        {
            todos[todoNum-1].completeTodo();
        }
    }
    catch(const std::runtime_error& e)
    {
        cout << e.what() << '\n';
    }
}