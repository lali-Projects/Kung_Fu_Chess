#pragma once

#include <functional>
#include <string>

namespace client
{
    enum class TransportState
    {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,
        FAILED
    };

    /** Byte-oriented client transport boundary. */
    class ITransport
    {
    public:
        using MessageHandler =
            std::function<void(const std::string&)>;

        using StateHandler =
            std::function<void(TransportState, const std::string&)>;

        virtual ~ITransport() = default;

        virtual bool connect(const std::string& url) = 0;
        virtual void disconnect() = 0;
        virtual bool isConnected() const = 0;
        virtual TransportState getState() const = 0;
        virtual bool send(const std::string& message) = 0;
        virtual void setMessageHandler(MessageHandler handler) = 0;
        virtual void setStateHandler(StateHandler handler) = 0;
    };
}
