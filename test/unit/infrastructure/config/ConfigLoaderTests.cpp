#include <doctest/doctest.hpp>

#include "ConfigLoader.hpp"


TEST_CASE("ConfigLoader loads valid configuration")
{
    auto config =
        ConfigLoader::load(
            "TestData/Config/valid_config.json"
        );


    CHECK(config.server.port == 7777);

    CHECK(config.server.maxClients == 1000);

    CHECK(config.server.maxSessions == 500);


    CHECK(config.timing.tickRateHz == 60);

    CHECK(config.timing.broadcastRateHz == 20);


    CHECK(
        config.database.databasePath ==
        "Database/KungFuChess.db"
    );


    CHECK(
        config.logging.logDirectory ==
        "Logs"
    );
}



TEST_CASE("ConfigLoader throws when file does not exist")
{
    CHECK_THROWS_AS(
        ConfigLoader::load(
            "TestData/Config/not_exist.json"
        ),
        std::runtime_error
    );
}



TEST_CASE("ConfigLoader throws on invalid json")
{
    CHECK_THROWS_AS(
        ConfigLoader::load(
            "TestData/Config/invalid_json.json"
        ),
        std::runtime_error
    );
}



TEST_CASE("ConfigLoader throws when required field missing")
{
    CHECK_THROWS_AS(
        ConfigLoader::load(
            "TestData/Config/missing_field_config.json"
        ),
        std::runtime_error
    );
}



TEST_CASE("ConfigLoader rejects invalid timing values")
{
    CHECK_THROWS_AS(
        ConfigLoader::load(
            "TestData/Config/invalid_value_config.json"
        ),
        std::runtime_error
    );
}