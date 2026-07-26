#pragma once

#include <memory>
#include <string>

#include "MoveResult.hpp"


class EventBus;
class RoomFactory;
class RoomManager;
class CommandHandler;
class Server;



class Application
{

public:

    Application();

    ~Application();



    Application(
        const Application&) = delete;


    Application& operator=(
        const Application&) = delete;



public:


    void start();


    void stop();



    MoveResult sendCommand(
        const std::string& message);



    Server& getServer();


    RoomManager& getRoomManager();



private:


    void initialize();



private:


    std::unique_ptr<EventBus>
        m_eventBus;



    std::unique_ptr<RoomFactory>
        m_roomFactory;



    std::unique_ptr<RoomManager>
        m_roomManager;



    std::unique_ptr<CommandHandler>
        m_commandHandler;



    std::unique_ptr<Server>
        m_server;



    bool m_running{false};

};