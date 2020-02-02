#ifndef __TODOH__
    #define __TODOH__
    #include "./todo.h"
#endif

#ifndef __APH__
    #define __APH__
    #include "../../Controller/auth-provider.h"
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

// #ifndef _FILESYSTEM_
//     #define _FILESYSTEM_
//     #include "../../Controller/filesystem.hpp"
// #endif

std::string todoFilePath = std::string("todo.dat");

unsigned int Todo::getTodoCount () noexcept(false)
{
    std::ifstream wstream;
    wstream.open(todoFilePath, std::ios::in | std::ios::app);

    if (!wstream.is_open())
    {
        throw std::runtime_error("Cannot determine the todos created count!!");
    }
    std::string str;
    unsigned int fileLength = 0;
    
    for (std::string str; std::getline(wstream, str); std::ws(wstream))
    {
        fileLength++;
    }

    wstream.close();
    return fileLength;
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
    this->createrId = auth::authProvider->getAuthenticatedUserId();

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
    stream.open(todoFilePath, std::ios::in | std::ios::app);

    if (!stream.is_open())
    {
        throw new std::runtime_error("Couldn't get all todos for displaying!!");
    }

    Todo obj;
    while (!(stream >> obj).eof())
    {
        if ((obj.createrId == auth::authProvider->getAuthenticatedUserId()) && (obj.completed == getCompleted))
        {
            allTodos.push_back(obj);
        }
    }

    return allTodos;
}

void Todo::save (Todo& obj) noexcept(false)
{
    std::ofstream writestream;
    writestream.open(todoFilePath, std::ios::app | std::ios::out);
    if (!writestream.is_open())
    {
        throw std::runtime_error("Couldn't save the todo in the database");
    }
    writestream << obj;
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

    std::ifstream rstream;
    rstream.open(todoFilePath, std::ios::in | std::ios::app);
    if (!rstream.is_open())
    {
        throw std::runtime_error("Couldn't update the todo!!");
    }

    std::ofstream wstream;
    wstream.open("temp.dat", std::ios::out);
    if (!wstream.is_open())
    {
        throw std::runtime_error("Couldn't update the todo!!");
    }

    Todo obj;

    while (!(rstream >> obj).eof())
    {
        if (obj.todoId == this->todoId)
        {
            obj.completedAt = now.time_since_epoch().count();
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