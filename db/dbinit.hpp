#include <iostream>
#include <pqxx/pqxx>

#include "../Controller/db-provider.hpp"

namespace initDB {
  void initDB();
  void initUser();
  void initNotification();
  void initTodo();
  void initLongTermGoal();
  void initMonthlyPlanner();
  void initActivityTracker();
  void monthlyPlanRemover();
  void lodgeJournal();
  void activityTrackerRemover();
}

#ifndef __INITDBH__
#define __INITDBH__

inline void initDB::initDB ()
{
    initDB::initUser();
    initDB::initNotification();
    initDB::initTodo();
    initDB::initLongTermGoal();
    initDB::initMonthlyPlanner();
    initDB::initActivityTracker();
    initDB::monthlyPlanRemover();
    initDB::lodgeJournal();
    initDB::activityTrackerRemover();
}

inline void initDB::initUser ()
{
    std::string createUserTable = " \
        CREATE TABLE IF NOT EXISTS users ( \
            userId serial PRIMARY KEY, \
            name varchar(100) NOT NULL DEFAULT 'user', \
            email varchar(100) UNIQUE NOT NULL, \
            password varchar(64) NOT NULL \
        );";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec0(createUserTable);
    W.commit();
}

inline void initDB::initNotification ()
{
    std::string createNotificationTable = " \
        CREATE TABLE IF NOT EXISTS notifications ( \
            notification varchar(200) NOT NULL, \
            userId integer REFERENCES users(userId) ON DELETE CASCADE \
        );";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec0(createNotificationTable);
    W.commit();
}

inline void initDB::initTodo ()
{
    std::string createTodoTable = " \
        CREATE TABLE IF NOT EXISTS todos ( \
            todoId serial PRIMARY KEY, \
            todo varchar(500) NOT NULL, \
            userId integer REFERENCES users(userId) ON DELETE CASCADE, \
            isCompleted boolean DEFAULT false, \
            createdAt date DEFAULT CURRENT_DATE, \
            completedAt date \
        );";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec0(createTodoTable);
    W.commit();
}

inline void initDB::initLongTermGoal ()
{
    std::string createLTGTable = " \
        CREATE TABLE IF NOT EXISTS longtermgoals ( \
            LongTermGoalId serial PRIMARY KEY, \
            userId integer REFERENCES users(userId) ON DELETE CASCADE, \
            goal varchar(500) NOT NULL, \
            journals_logded integer DEFAULT 0, \
            isCompleted boolean DEFAULT false, \
            lastProgress timestamp, \
            createdAt date DEFAULT CURRENT_DATE\
        );";
    // int64_t lastProgress <- set default

    std::string createLTGJTable = " \
        CREATE TABLE IF NOT EXISTS longtermjournals ( \
            journal varchar(200) NOT NULL, \
            lodgeDate timestamp DEFAULT current_timestamp, \
            LongTermGoalId integer REFERENCES longtermgoals(LongTermGoalId) ON DELETE CASCADE \
        );";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec0(createLTGTable);
    W.exec0(createLTGJTable);
    W.commit();

}

inline void initDB::initMonthlyPlanner ()
{
    std::string createMonthlyPlannerTable = " \
        CREATE TABLE IF NOT EXISTS monthlyplanners ( \
            plannerId serial PRIMARY KEY, \
            userId integer REFERENCES users(userId) ON DELETE CASCADE, \
            isCompleted boolean DEFAULT false, \
            plan varchar(500) NOT NULL, \
            createdAt date DEFAULT CURRENT_DATE\
        );";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec0(createMonthlyPlannerTable);
    W.commit();
}

inline void initDB::initActivityTracker ()
{
    std::string createActivityTrackerTable = " \
        CREATE TABLE IF NOT EXISTS activitytrackers ( \
            activityId serial PRIMARY KEY, \
            userId integer REFERENCES users(userId) ON DELETE CASCADE, \
            activity varchar(500) NOT NULL, \
            longestStreak integer DEFAULT 0, \
            streakDuration interval NOT NULL, \
            lastCheckIn timestamp, \
            createdAt date DEFAULT CURRENT_DATE\
        );";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec0(createActivityTrackerTable);
    W.commit();
}

inline void initDB::monthlyPlanRemover()
{
    std::string createFunction = " \
        CREATE OR REPLACE FUNCTION public.remove_monthly_plans() \
            RETURNS void \
            LANGUAGE 'plpgsql' \
        AS $BODY$ \
        begin  \
            INSERT into notifications (notification, userId) \
            ( \
                SELECT plan, userId from monthlyplanners \
                WHERE EXTRACT(MONTH from AGE(createdAt)) >= 1 \
            ); \
            if found then \
                truncate table monthlyplanners; \
            end if; \
        end; \
        $BODY$;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec0(createFunction);
    W.commit();
}

inline void initDB::lodgeJournal ()
{
    std::string createFunction = " \
        create or replace function lodge_journal ( \
            goalId integer, \
            journal varchar(500), \
            offset_count integer default 10 \
        ) \
        returns void \
        language 'plpgsql' \
        as $$ \
        begin \
            INSERT INTO longtermjournals (journal, LongTermGoalId) \
            VALUES (journal, goalId); \
            delete from longtermjournals \
            where ctid in ( \
                select ctid from longtermjournals \
                where LongTermGoalId = goalId \
                order by lodgeDate desc \
                offset offset_count \
            ); \
            update longtermgoals \
            set journals_logded = journals_logded + 1, lastProgress = current_timestamp \
            where LongTermGoalId = goalId; \
        end; \
        $$;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec0(createFunction);
    W.commit();
}

inline void initDB::activityTrackerRemover()
{
    std::string createFunction = " \
        CREATE OR REPLACE FUNCTION public.remove_activities() \
            RETURNS void \
            LANGUAGE 'plpgsql' \
        AS $BODY$ \
        begin \
            insert into notifications (userId, notification) \
            SELECT userId, 'You failed the activity ' || activity || '! Your streak this time was: ' || longestStreak as notification from activitytrackers \
            WHERE lastCheckIn is not null and age(lastCheckIn) <= streakDuration; \
            delete from activitytrackers \
            WHERE lastCheckIn is not null and age(lastCheckIn) <= streakDuration; \
        end; \
        $BODY$;";

    pqxx::connection C(connectionString);
    pqxx::work W{C};
    W.exec0(createFunction);
    W.commit();
}

#endif