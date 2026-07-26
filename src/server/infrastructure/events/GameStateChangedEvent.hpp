#pragma once


#include <string>


#include "Event.hpp"
#include "GameSnapshot.hpp"



/**
 * @brief Event published when game state changes.
 *
 * Carries:
 *
 *  - Immutable game snapshot.
 *  - Room identifier.
 */
class GameStateChangedEvent
    :
    public Event
{

public:


    GameStateChangedEvent(
        const std::string& roomId,
        const GameSnapshot& snapshot);



    const GameSnapshot& getSnapshot() const;



    const std::string& getRoomId() const;



private:


    std::string m_roomId;



    GameSnapshot m_snapshot;

};