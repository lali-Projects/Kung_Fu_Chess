#pragma once

#include <functional>
#include <mutex>
#include <condition_variable>
#include <string>

#include <ixwebsocket/IXWebSocket.h>


class ClientSocket
{

public:

    using MessageHandler =
        std::function<void(const std::string&)>;



public:

    ClientSocket();

    ~ClientSocket();



    ClientSocket(
        const ClientSocket&) = delete;


    ClientSocket& operator=(
        const ClientSocket&) = delete;



public:


    bool connect(
        const std::string& url);



    void disconnect();



    bool isConnected() const;



    bool send(
        const std::string& message);



    void setMessageHandler(
        MessageHandler handler);



private:


    ix::WebSocket m_socket;



    MessageHandler m_messageHandler;



    mutable std::mutex m_mutex;



    std::condition_variable
        m_connectionCondition;



    bool m_connected{false};



    bool m_failed{false};

};