#include <doctest/doctest.hpp>

#include "Config.hpp"



TEST_CASE("Config creates successfully")
{
    Config config(
        "TestData/Config/valid_config.json"
    );


    CHECK(
        config.server().port == 7777
    );


    CHECK(
        config.timing().tickRateHz == 60
    );
}



TEST_CASE("Config returns immutable configuration")
{
    Config config(
        "TestData/Config/valid_config.json"
    );


    const ServerConfig& data =
        config.get();


    CHECK(
        data.server.maxClients == 1000
    );
}