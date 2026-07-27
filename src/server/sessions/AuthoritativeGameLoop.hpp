#pragma once


#include <atomic>
#include <cstdint>
#include <thread>


class RoomManager;


/**
 * Advances active server-owned games and publishes authoritative frames.
 * It schedules time only; all game mutation remains inside GameSession.
 */
class AuthoritativeGameLoop
{
public:
    explicit AuthoritativeGameLoop(
        RoomManager& roomManager,
        std::uint32_t tickRateHz = 60,
        std::uint32_t broadcastRateHz = 60);


    ~AuthoritativeGameLoop();


    AuthoritativeGameLoop(
        const AuthoritativeGameLoop&) = delete;


    AuthoritativeGameLoop& operator=(
        const AuthoritativeGameLoop&) = delete;


    void start();
    void stop();
    bool isRunning() const;


private:
    void run();


private:
    RoomManager& m_roomManager;
    std::uint32_t m_tickRateHz;
    std::uint32_t m_broadcastRateHz;
    std::atomic<bool> m_running{false};
    std::thread m_worker;
};
