#include <iostream>
#include <chrono>

using std::string;

class Todo
{
    static unsigned int todoCount;

    unsigned int todoId;
    unsigned int createrId;
    string todo;
    bool completed;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point completedAt;

    static unsigned int createTodo (string todoBody)
    {
        
        // new todo
    }

    // find all todos of a user

    // find one todo of a user

    void completeTodo ()
    {
        /*
        if(_.isBoolean(body.completed) && body.completed)
        body.completedAt = new Date().getTime();
    else {
        body.completed = false;
        body.completedAt = null;
    }

    Todo.findOneAndUpdate({
        _id: id,
        _creator: request.user._id
    }, {$set: body}, {new: true}).then((todo) => {
        if (!todo)
            return response.status(404).send();
        response.status(200).send({todo});
    }).catch((error) => response.status(400).send());
        */
    }

    void deleteTodo ()
    {

    }


};
