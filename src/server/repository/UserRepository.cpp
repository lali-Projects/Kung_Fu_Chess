#include "UserRepository.hpp"


#include "IDatabase.hpp"



UserRepository::UserRepository(
    IDatabase& database)
:
m_database(database)
{
}







//================================================
// Create User
//================================================

bool UserRepository::create(
    const User& user)
{

    const std::string sql =
        "INSERT INTO users "
        "(username,password) "
        "VALUES (?,?);";



    return
        m_database.execute(
            sql,
            {
                user.getUsername(),
                user.getPassword()
            });

}









//================================================
// Find By Username
//================================================

std::optional<User>
UserRepository::findByUsername(
    const std::string& username)
{

    const std::string sql =
        "SELECT id, username, password "
        "FROM users "
        "WHERE username=?;";



    auto row =
        m_database.queryOne(
            sql,
            {
                username
            });



    if(!row)
    {
        return std::nullopt;
    }




    return User(
        row->at("id"),
        row->at("username"),
        row->at("password"));

}









//================================================
// Exists
//================================================

bool UserRepository::exists(
    const std::string& username)
{

    return
        findByUsername(username)
        .has_value();

}