#include "AuthoritativeGameLoop.hpp"


#include "GameSession.hpp"
#include "Room.hpp"
#include "RoomManager.hpp"


#include <algorithm>
#include <chrono>
#include <exception>
#include <iostream>
#include <stdexcept>


AuthoritativeGameLoop::AuthoritativeGameLoop(
    RoomManager& roomManager,
    std::uint32_t tickRateHz,
    std::uint32_t broadcastRateHz)
:
m_roomManager(roomManager),
m_tickRateHz(tickRateHz),
m_broadcastRateHz(broadcastRateHz)
{
    if(m_tickRateHz == 0 ||
       m_broadcastRateHz == 0 ||
       m_broadcastRateHz > m_tickRateHz)
    {
        throw std::invalid_argument(
            "Invalid authoritative game-loop rates");
    }
}


AuthoritativeGameLoop::~AuthoritativeGameLoop()
{
    stop();
}


void AuthoritativeGameLoop::start()
{
    bool expected = false;


    if(!m_running.compare_exchange_strong(
            expected,
            true))
    {
        return;
    }


    try
    {
        m_worker =
            std::thread(
                &AuthoritativeGameLoop::run,
                this);
    }
    catch(...)
    {
        m_running = false;
        throw;
    }
}


void AuthoritativeGameLoop::stop()
{
    m_running = false;


    if(m_worker.joinable())
    {
        m_worker.join();
    }
}


bool AuthoritativeGameLoop::isRunning() const
{
    return m_running;
}


void AuthoritativeGameLoop::run()
{
    using Clock = std::chrono::steady_clock;


    const auto tickInterval =
        std::chrono::microseconds(
            1000000 / m_tickRateHz);


    const auto broadcastInterval =
        std::chrono::microseconds(
            1000000 / m_broadcastRateHz);


    auto previous = Clock::now();
    auto nextTick = previous + tickInterval;
    std::chrono::microseconds gameTimeRemainder{0};
    std::chrono::microseconds broadcastElapsed{0};


    while(m_running)
    {
        std::this_thread::sleep_until(nextTick);


        if(!m_running)
        {
            break;
        }


        const auto now = Clock::now();
        const auto elapsed =
            std::chrono::duration_cast<std::chrono::microseconds>(
                now - previous);


        previous = now;


        const auto boundedElapsed =
            std::min(
                elapsed,
                std::chrono::microseconds{250000});


        gameTimeRemainder += boundedElapsed;


        const int elapsedMilliseconds =
            static_cast<int>(
                gameTimeRemainder.count() / 1000);


        gameTimeRemainder -=
            std::chrono::milliseconds(
                elapsedMilliseconds);


        broadcastElapsed += boundedElapsed;


        const bool broadcastSnapshot =
            broadcastElapsed >= broadcastInterval;


        if(broadcastSnapshot)
        {
            broadcastElapsed %= broadcastInterval;
        }


        auto rooms =
            m_roomManager.getRoomsSnapshot();


        for(const auto& room : rooms)
        {
            if(!room ||
               elapsedMilliseconds <= 0)
            {
                continue;
            }


            try
            {
                room->getSession().tick(
                    elapsedMilliseconds,
                    broadcastSnapshot);
            }
            catch(const std::exception& exception)
            {
                std::cerr
                    << "Authoritative tick failed for room "
                    << room->getId()
                    << ": "
                    << exception.what()
                    << std::endl;
            }
            catch(...)
            {
                std::cerr
                    << "Authoritative tick failed for room "
                    << room->getId()
                    << std::endl;
            }
        }


        nextTick += tickInterval;


        if(nextTick < now)
        {
            nextTick = now + tickInterval;
        }
    }
}
