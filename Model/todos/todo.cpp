#include <stdexcept>
#include <iostream>
#include <fstream>
#include <pqxx/pqxx>

#include "../../Controller/auth-provider.hpp"
#include "../../Controller/db-provider.hpp"
#include "../../Controller/util.hpp"
#include "./todo.hpp"

Todo::Todo () noexcept : todoId {0} { }

Todo::Todo (std::string todoBody) noexcept(false)
    :todo {todoBody},
    todoId {0},
    userId {authProvider->getAuthenticatedUserId()},
    completed {false},
    createdAt {getPostgresDate()}
{ }

std::vector<Todo> Todo::getAllTodos (bool getCompleted) noexcept(false)
{
    const int limit = 100;

    std::string findSQL = " \
        SELECT todoId, todo, createdAt, completedAt from todos \
        WHERE userId = $1 AND isCompleted = $2 \
        LIMIT $3;";
    
    std::vector<Todo> todos;

    auto authenticatedUser = authProvider->getAuthenticatedUserId();

    pqxx::connection C(connectionString);
    pqxx::nontransaction N{C};
    pqxx::result R{ N.exec_params(findSQL, authenticatedUser, getCompleted, limit) };

    for (pqxx::row rowData: R)
    {
        Todo temp;
        
        temp.todoId = rowData[0].as<unsigned int>();
        temp.todo = rowData[1].as<std::string>();
        temp.userId = authenticatedUser;
        temp.completed = getCompleted;
        temp.createdAt = rowData[2].as<std::string>();

        if (!rowData[5].is_null())
        {
            temp.completedAt = rowData[3].as<std::string>();
        }

        todos.push_back(temp);
    }

    return todos;
}

void Todo::save () noexcept(false)
{
    std::string insertSQL = " \
        INSERT INTO todos (todo, userId) \
        VALUES ($1, $2) \
        RETURNING todoId;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    pqxx::row R = W.exec_params1(insertSQL, this->todo, this->userId);
    W.commit();
    // finally setting ID on the todo
    this->todoId = R[0].as<unsigned int>();
}

void Todo::completeTodo () noexcept(false)
{
    std::string updateSQL = " \
        UPDATE todos \
        SET isCompleted = true, completedAt = CURRENT_DATE \
        WHERE todoId = $1 AND isCompleted = false;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec_params0(updateSQL, this->todoId);
    W.commit();
}