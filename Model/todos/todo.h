#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _VECTOR_
    #define _VECTOR_
    #include <vector>
#endif

class Todo
{

    static unsigned int getTodoCount () noexcept(false);
public:
    unsigned int todoId;
    unsigned int createrId;
    std::string todo;
    bool completed;
    int64_t createdAt;
    int64_t completedAt;
    
    Todo () noexcept;
    Todo (std::string todoBody) noexcept(false);

    static void save (Todo& obj) noexcept(false);
    static std::vector<Todo> getAllTodos (bool getCompleted) noexcept(false);
    void completeTodo () noexcept(false);
    
    friend std::ifstream& operator >> (std::ifstream&, Todo& obj);
    friend std::ofstream& operator << (std::ofstream&, Todo& obj);
};



