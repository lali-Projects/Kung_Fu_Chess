#include "SQLiteDatabase.hpp"


#include "sqlite3.h"


#include <utility>




//================================================
// Constructor
//================================================

SQLiteDatabase::SQLiteDatabase(
    const std::string& filename)
:
m_filename(filename)
{
}






//================================================
// Destructor
//================================================

SQLiteDatabase::~SQLiteDatabase()
{
    close();
}








//================================================
// Open Database
//================================================

bool SQLiteDatabase::open()
{

    std::lock_guard<std::mutex> lock(
        m_mutex);

    m_lastError.clear();



    if(m_database)
    {
        return true;
    }




    int result =
        sqlite3_open(
            m_filename.c_str(),
            &m_database);




    if(result != SQLITE_OK)
    {

        if(m_database)
        {
            m_lastError =
                sqlite3_errmsg(
                    m_database);
        }
        else
        {
            m_lastError =
                "Failed opening database";
        }



        if(m_database)
        {
            sqlite3_close(
                m_database);


            m_database = nullptr;
        }



        return false;
    }



    return true;
}








//================================================
// Close Database
//================================================

void SQLiteDatabase::close()
{

    std::lock_guard<std::mutex> lock(
        m_mutex);

    if(!m_database)
    {
        return;
    }




    int result =
        sqlite3_close(
            m_database);




    if(result != SQLITE_OK)
    {
        m_lastError =
            sqlite3_errmsg(
                m_database);


        return;
    }




    m_database = nullptr;

}








//================================================
// Is Open
//================================================

bool SQLiteDatabase::isOpen() const
{
    std::lock_guard<std::mutex> lock(
        m_mutex);

    return
        m_database != nullptr;
}









//================================================
// Execute Without Parameters
//================================================

bool SQLiteDatabase::execute(
    const std::string& sql)
{

    std::lock_guard<std::mutex> lock(
        m_mutex);

    m_lastError.clear();



    if(!m_database)
    {
        m_lastError =
            "Database is not open";

        return false;
    }





    char* errorMessage = nullptr;




    int result =
        sqlite3_exec(
            m_database,
            sql.c_str(),
            nullptr,
            nullptr,
            &errorMessage);





    if(result != SQLITE_OK)
    {

        if(errorMessage)
        {
            m_lastError =
                errorMessage;

            sqlite3_free(
                errorMessage);
        }
        else
        {
            m_lastError =
                sqlite3_errmsg(
                    m_database);
        }



        return false;
    }




    return true;
}









//================================================
// Execute With Parameters
//================================================

bool SQLiteDatabase::execute(
    const std::string& sql,
    const std::vector<std::string>& params)
{

    std::lock_guard<std::mutex> lock(
        m_mutex);

    m_lastError.clear();



    if(!m_database)
    {
        m_lastError =
            "Database is not open";

        return false;
    }




    sqlite3_stmt* statement =
        nullptr;





    int prepareResult =
        sqlite3_prepare_v2(
            m_database,
            sql.c_str(),
            -1,
            &statement,
            nullptr);





    if(prepareResult != SQLITE_OK)
    {

        m_lastError =
            sqlite3_errmsg(
                m_database);

        return false;
    }






    if(!bindParameters(
            statement,
            params))
    {

        sqlite3_finalize(
            statement);

        return false;
    }







    int stepResult =
        sqlite3_step(
            statement);





    sqlite3_finalize(
        statement);






    if(stepResult != SQLITE_DONE)
    {

        m_lastError =
            sqlite3_errmsg(
                m_database);

        return false;
    }




    return true;
}









//================================================
// Query One
//================================================

std::optional<DatabaseRow>
SQLiteDatabase::queryOne(
    const std::string& sql,
    const std::vector<std::string>& params)
{

    std::lock_guard<std::mutex> lock(
        m_mutex);

    m_lastError.clear();



    if(!m_database)
    {
        m_lastError =
            "Database is not open";

        return std::nullopt;
    }







    sqlite3_stmt* statement =
        nullptr;





    int prepareResult =
        sqlite3_prepare_v2(
            m_database,
            sql.c_str(),
            -1,
            &statement,
            nullptr);





    if(prepareResult != SQLITE_OK)
    {

        m_lastError =
            sqlite3_errmsg(
                m_database);

        return std::nullopt;
    }








    if(!bindParameters(
            statement,
            params))
    {

        sqlite3_finalize(
            statement);


        return std::nullopt;
    }









    int result =
        sqlite3_step(
            statement);






    if(result != SQLITE_ROW)
    {

        sqlite3_finalize(
            statement);


        return std::nullopt;
    }








    DatabaseRow row;





    int columns =
        sqlite3_column_count(
            statement);






    for(int i = 0;
        i < columns;
        i++)
    {

        const char* name =
            sqlite3_column_name(
                statement,
                i);




        const char* value =
            reinterpret_cast<const char*>(
                sqlite3_column_text(
                    statement,
                    i));





        row[
            name ? name : ""
        ]
        =
        value ? value : "";

    }








    sqlite3_finalize(
        statement);




    return row;

}









//================================================
// Bind Parameters
//================================================

bool SQLiteDatabase::bindParameters(
    sqlite3_stmt* statement,
    const std::vector<std::string>& params)
{

    for(size_t i = 0;
        i < params.size();
        i++)
    {


        int result =
            sqlite3_bind_text(
                statement,
                static_cast<int>(i + 1),
                params[i].c_str(),
                -1,
                SQLITE_TRANSIENT);





        if(result != SQLITE_OK)
        {

            m_lastError =
                sqlite3_errmsg(
                    m_database);


            return false;
        }

    }



    return true;
}









//================================================
// Last Error
//================================================

std::string
SQLiteDatabase::getLastError() const
{
    std::lock_guard<std::mutex> lock(
        m_mutex);

    return m_lastError;
}
