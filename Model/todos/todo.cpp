#ifndef __TODOH__
    #define __TODOH__
    #include "./todo.h"
#endif

#ifndef __APH__
    #define __APH__
    #include "../../Controller/auth-provider.h"
#endif

#include "../../Controller/util.h"

#include <stdexcept>
#include <iostream>
#include <fstream>

// #ifndef _FILESYSTEM_
//     #define _FILESYSTEM_
//     #include "../../Controller/filesystem.hpp"
// #endif

std::string todoFilePath = std::string("todo.dat");

unsigned int Todo::getTodoCount () noexcept(false)
{
    std::ifstream wstream {todoFilePath, std::ios::in | std::ios::app};
    if (!wstream.is_open())
    {
        throw std::runtime_error("Cannot determine the todos created count!!");
    }

    unsigned int fileLength = 0;    
    for (std::string str; std::getline(wstream, str); std::ws(wstream))
    {
        fileLength++;
    }

    return fileLength;
}

Todo::Todo () noexcept : todoId {0} { }

Todo::Todo (std::string todoBody) noexcept(false)
    :todo {todoBody},
    todoId {Todo::getTodoCount() + 1},
    createrId {auth::authProvider->getAuthenticatedUserId()},
    completed {false},
    createdAt {getCurrentTime()},
    completedAt {0}
{ }

std::vector<Todo> Todo::getAllTodos (bool getCompleted) noexcept(false)
{
    std::ifstream stream {todoFilePath, std::ios::in | std::ios::app};
    if (!stream.is_open())
    {
        throw new std::runtime_error("Couldn't get all todos for displaying!!");
    }

    std::vector<Todo> allTodos;
    unsigned int authenticatedUserId {auth::authProvider->getAuthenticatedUserId()};

    for (Todo obj; !(stream >> obj).eof(); )
    {
        if ((obj.createrId == authenticatedUserId) && (obj.completed == getCompleted))
        {
            allTodos.push_back(obj);
        }
    }

    return allTodos;
}

void Todo::save () noexcept(false)
{
    std::ofstream writestream {todoFilePath, std::ios::app | std::ios::out};
    if (!writestream.is_open())
    {
        throw std::runtime_error("Couldn't save the todo in the database");
    }

    writestream << *this;
}

void Todo::completeTodo () noexcept(false)
{
    if (this->completed)
    {
        return;
    }
    int64_t now = getCurrentTime();
    this->completedAt = now;
    this->completed = true;

    std::ifstream rstream {todoFilePath, std::ios::in | std::ios::app};
    if (!rstream.is_open())
    {
        throw std::runtime_error("Couldn't update the todo!!");
    }

    std::ofstream wstream {"temp.dat", std::ios::out};
    if (!wstream.is_open())
    {
        throw std::runtime_error("Couldn't update the todo!!");
    }

    for (Todo obj; !(rstream >> obj).eof(); )
    {
        if (obj.todoId == this->todoId)
        {
            obj.completedAt = now;
            obj.completed = true;
        }
        wstream << obj;
    }

    wstream.close();
    rstream.close();

    remove(todoFilePath.c_str());
    rename("temp.dat", todoFilePath.c_str());
}

std::ifstream& operator >> (std::ifstream& stream, Todo& obj)
{
    std::ws(stream);
    std::getline(stream, obj.todo, '$');
    stream >> obj.todoId >> obj.createrId >> obj.completed >> obj.createdAt >> obj.completedAt;
    return stream;
}

std::ofstream& operator << (std::ofstream& stream, Todo& obj)
{
    stream << obj.todo << "$" << obj.todoId << " " << obj.createrId << " ";
    stream << obj.completed << " " << obj.createdAt << " " << obj.completedAt << "\n";
    return stream;
}