#pragma once


#include <memory>
#include <string>



class Room;
class EventBus;




class RoomFactory
{

public:


    explicit RoomFactory(
        EventBus& eventBus);



    std::unique_ptr<Room>
    createRoom(
        const std::string& roomId);



private:


    EventBus& m_eventBus;

};