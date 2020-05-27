#include <iostream>
#include <pqxx/pqxx>

#ifndef sdncjwednwed
#define sdncjwednwed
    #include "./database-worker.h"
#endif

database::database () noexcept(false)
    :conn("postgresql://himanshu:mynewpass@127.0.0.1:5432/test-db")
{
    // not only connect to a database here, but also create database, and also create tables here
    // after checking if tables doesn't exists
    try
    {
        if (conn.is_open())
        {
            std::cout << "Opened database successfully: " << conn.dbname() << "\n";
        }
        else
        {
            std::cout << "Can't open database\n";
        }
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

database::~database () {}

// mo schema or data modifiction work with trnsact
pqxx::result database::queryTransact (std::string sql) {
    pqxx::work W{conn};

    pqxx::result res{ W.exec(sql) };

    W.commit();

    return res;
}

// basically only select with query
pqxx::result database::query (std::string sql) {
    pqxx::nontransaction N{conn};

    pqxx::result res{ N.exec(sql) };

    return res;
}