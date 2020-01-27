#ifndef _IOSTREAM_
    #define _IOSTREAM_
    #include <iostream>
#endif

#ifndef _CHRONO_
    #define _CHRONO_
    #include <chrono>
#endif

class Todo
{
    static unsigned int todoCount;

    unsigned int todoId;
    unsigned int createrId;
    std::string todo;
    bool completed;
    std::chrono::system_clock::time_point createdAt;
    std::chrono::system_clock::time_point completedAt;

    static unsigned int createTodo (std::string todoBody)
    {
        
        // new todo
    }

    // find all todos of a user

    // find one todo of a user

    void completeTodo ()
    {
    /*
    if(_.isBoolean(body.completed) && body.completed)
    {
        body.completedAt = new Date().getTime();
    }
    else
    {
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
