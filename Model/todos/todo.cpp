#ifndef __TODOH__
    #define __TODOH__
    #include "./todo.h"
#endif

#ifndef __AUTHH__
    #define __AUTHH__
    #include "../auth/auth.h"
#endif

#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _CHRONO_
    #define _CHRONO_
    #include <chrono>
#endif

#ifndef _FSTREAM_
    #define _FSTREAM_
    #include <fstream>
#endif

#ifndef _STDEXCEPT_
    #define _STDEXCEPT_
    #include <stdexcept>
#endif

#ifndef _LIMITS_
    #define _LIMITS_
    #include <limits>
#endif

// #ifndef _FILESYSTEM_
//     #define _FILESYSTEM_
//     #include "../../Controller/filesystem.hpp"
// #endif

std::string todoFilePath = std::string("todo.dat");

unsigned int Todo::getTodoCount () noexcept(false)
{
    std::ifstream wstream;
    wstream.open(todoFilePath, std::ios::in);

    if (!wstream.good())
    {
        throw std::runtime_error("Cannot determine the todos created count!!");
    }
    std::string str;
    unsigned int fileLength = 0;
    
    while (!wstream.eof())
    {
        std::getline(wstream, str);
        fileLength++;
    }

    wstream.close();
    return fileLength-1;
}

Todo::Todo () noexcept
{
    this->todoId = 0;
}

Todo::Todo (std::string todoBody) noexcept(false)
{
    auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());

    this->todo = todoBody;
    this->completed = false;
    this->createdAt = now.time_since_epoch().count();
    this->completedAt = 0;
    this->createrId = AuthModule::getAuthenticatedUserId();

    try
    {
        this->todoId = Todo::getTodoCount() + 1;
    }
    catch(const std::runtime_error& e)
    {
        throw e;
    }
}

std::vector<Todo> Todo::getAllTodos (bool getCompleted) noexcept(false)
{
    std::vector<Todo> allTodos;

    std::ifstream stream;
    stream.open(todoFilePath, std::ios::in);

    if (!stream.good())
    {
        throw new std::runtime_error("Couldn't get all todos for displaying!!");
    }

    Todo obj;
    while (!stream.eof())
    {
        stream >> obj;
        if ((obj.createrId == AuthModule::getAuthenticatedUserId()) && (obj.completed == getCompleted))
        {
            allTodos.push_back(obj);
        }
    }

    return allTodos;
}

void Todo::save () noexcept(false)
{
    std::ofstream writestream;
    writestream.open(todoFilePath, std::ios::app | std::ios::out);
    if (!writestream.good())
    {
        throw std::runtime_error("Couldn't save the todo in the database");
    }
    writestream << this;
    writestream.close();
}

void Todo::completeTodo () noexcept(false)
{
    if (this->completed)
    {
        return;
    }
    auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
    this->completedAt = now.time_since_epoch().count();
    this->completed = true;

    std::ofstream wstream;
    wstream.open(todoFilePath, std::ios::app | std::ios::out);
    if (!wstream.good())
    {
        throw std::runtime_error("Couldn't update the todo!!");
    }
    wstream.seekp((this->todoId - 1) * sizeof(Todo), std::ios::beg);
    wstream << this;
    wstream.close();
}

std::ifstream& operator >> (std::ifstream& stream, Todo& obj)
{
    std::getline(stream, obj.todo, '$');
    stream >> obj.todoId >> obj.createrId >> obj.completed >> obj.createdAt >> obj.completedAt;
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return stream;
}

std::ofstream& operator << (std::ofstream& stream, Todo& obj)
{
    stream << obj.todo << "$" << obj.todoId << " " << obj.createrId << " ";
    stream << obj.completed << " " << obj.createdAt << " " << obj.completedAt << "\n";
    return stream;
}