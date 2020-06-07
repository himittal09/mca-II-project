#include <iostream>
#include <vector>

class Todo;

#ifndef __TODOH__
#define __TODOH__

class Todo
{
public:
    unsigned int todoId;
    unsigned int userId;
    std::string todo;
    bool completed;
    std::string createdAt;
    std::string completedAt;
    
    Todo () noexcept;
    Todo (std::string todoBody) noexcept(false);

    void save () noexcept(false);
    static std::vector<Todo> getAllTodos (bool getCompleted) noexcept(false);
    void completeTodo () noexcept(false);
};

#endif