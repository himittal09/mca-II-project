#include <stdexcept>
#include <vector>
#include <iomanip>
#include <iostream>

#include "../../Controller/util.hpp"
#include "../../Model/todos/todo.hpp"

using namespace std;

namespace view {
    bool todoMenu ();
    int askTodoMenu ();
    void addTodo ();
    void displayTodos ();
}

#ifndef __TODOVH__
#define __TODOVH__

inline bool view::todoMenu ()
{
    int option = view::askTodoMenu();
    if (option == 1)
    {
        view::displayTodos();
    }
    else if (option == 2)
    {
        view::addTodo();
    }
    else if (option == 4)
    {
        return true;
    }
    return false;
}

inline int view::askTodoMenu ()
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

inline void view::addTodo ()
{
    std::string todo;
    cout << "Enter the todo objective: ";
    std::ws(cin);
    std::getline(cin, todo);
    try
    {
        Todo newTodo = Todo(todo);
        newTodo.save();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << '\n';
    }
    cout << "New todo created!!\n";
}

inline void view::displayTodos ()
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
    cout << "\t\tCompleted At\n";
    for (int i=0; i<todos.size(); i++)
    {
        cout << i+1 << "\t";
        cout << todos[i].todo;
        cout << setw(60);
        cout << todos[i].createdAt;
        cout << "\t\t" << todos[i].completedAt << "\n";

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

#endif