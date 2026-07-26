#pragma once


#include <memory>
#include <string>
#include <vector>


#include "MoveResult.hpp"
#include "PieceTypes.hpp"



class GameContext;
class EventBus;
class PlayerSession;
class ClickCommand;




class GameSession
{

public:


    enum class State
    {
        WAITING,
        RUNNING,
        FINISHED
    };




public:


    GameSession(
        const std::string& id,
        GameContext& context,
        EventBus& eventBus);



    ~GameSession();



    GameSession(
        const GameSession&) = delete;



    GameSession& operator=(
        const GameSession&) = delete;





public:


//=================================
// Players
//=================================


    bool addPlayer(
        std::shared_ptr<PlayerSession> player);



    bool removePlayer(
        const PlayerSession& player);



    bool containsPlayer(
        const PlayerSession& player) const;



    bool isObserver(
        const PlayerSession& player) const;



    size_t getPlayerCount() const;



    size_t getObserverCount() const;



    const std::shared_ptr<PlayerSession>&
    getWhitePlayer() const;



    const std::shared_ptr<PlayerSession>&
    getBlackPlayer() const;





//=================================
// Commands
//=================================


    MoveResult handleClick(
        PlayerSession& player,
        const ClickCommand& command);





//=================================
// State
//=================================


    State getState() const;



    void setState(
        State state);



    bool isRunning() const;





//=================================
// Identity
//=================================


    const std::string&
    getId() const;





private:


    Side assignSide();



    void publishSnapshot();





private:


    std::string m_id;



    GameContext& m_context;



    EventBus& m_eventBus;



    std::shared_ptr<PlayerSession>
        m_whitePlayer;



    std::shared_ptr<PlayerSession>
        m_blackPlayer;



    std::vector<
        std::shared_ptr<PlayerSession>>
        m_observers;



    State m_state{
        State::WAITING
    };

};