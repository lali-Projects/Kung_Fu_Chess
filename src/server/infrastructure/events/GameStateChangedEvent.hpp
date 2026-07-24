#pragma once


#include "Event.hpp"
#include "GameSnapshot.hpp"



/**
 * @brief Event published when game state changes.
 *
 * Carries immutable game snapshot.
 */
class GameStateChangedEvent
    :
    public Event
{
public:


    explicit GameStateChangedEvent(
        const GameSnapshot& snapshot);



    const GameSnapshot& getSnapshot() const;



private:


    GameSnapshot m_snapshot;
};