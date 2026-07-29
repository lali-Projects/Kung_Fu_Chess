#pragma once

#include <chrono>
#include <functional>
#include <string>

#include "CommandResponse.hpp"


namespace client
{
    class IClientCommandGateway
    {
    public:
        using ResponseHandler =
            std::function<void(const CommandResponse&)>;

        virtual ~IClientCommandGateway() = default;

        virtual void sendCommand(
            std::string command,
            ResponseHandler handler = nullptr) = 0;

        virtual CommandResponse sendCommandAndWait(
            std::string command,
            std::chrono::milliseconds timeout =
                std::chrono::seconds(5)) = 0;
    };
}
