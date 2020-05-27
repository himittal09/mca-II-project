#include <memory>

#ifndef sdncjwednwed
#define sdncjwednwed
    #include "../db/database-worker.h"
#endif

namespace db
{
    extern std::unique_ptr<database> dbProvider;
}