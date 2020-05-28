#include <pqxx/pqxx>

class database;

#ifndef __DBWORKER__
#define __DBWORKER__

class database
{
    std::string connectionString;
public:
    database () noexcept(false);

    // mo schema or data modifiction work with trnsact
    
    pqxx::result queryTransact (std::string sql) noexcept(false);

    // basically only select with query
    pqxx::result query (std::string sql) noexcept(false);
};

#endif