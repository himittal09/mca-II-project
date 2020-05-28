#include <iostream>
#include <pqxx/pqxx>

#include "./database-worker.h"

database::database () noexcept(false)
    :connectionString("postgresql://himanshu:mynewpass@127.0.0.1:5432/test-db")
{
    // not only connect to a database here, but also create database, and also create tables here
    // after checking if tables doesn't exists
}

pqxx::result database::queryTransact (std::string sql) noexcept(false) {
    pqxx::connection C(connectionString);

    pqxx::work W{C};

    pqxx::result res{ W.exec(sql) };

    W.commit();

    return res;
}

pqxx::result database::query (std::string sql) noexcept(false) {
    pqxx::connection C(connectionString);

    pqxx::nontransaction N{C};

    pqxx::result res{ N.exec(sql) };

    return res;
}