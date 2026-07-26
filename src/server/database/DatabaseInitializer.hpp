#pragma once


class IDatabase;


/**
 * @brief Initializes database schema.
 *
 * Responsible only for creating
 * required database structure.
 *
 * Does NOT know:
 *
 *  - Users
 *  - Authentication
 *  - Server
 *  - Network
 *  - SQLite
 */
class DatabaseInitializer
{

public:


    explicit DatabaseInitializer(
        IDatabase& database);



public:


    bool initialize();



private:


    bool createUsersTable();



private:


    IDatabase& m_database;

};