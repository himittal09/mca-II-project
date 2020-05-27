#include <pqxx/pqxx>

// #ifndef __DBWORKER__
// #define __DBWORKER__

class database
{
    pqxx::connection conn;
public:
    database () noexcept(false);
    ~database ();

    pqxx::result queryTransact (std::string sql);
    pqxx::result query (std::string sql);
};

// #endif