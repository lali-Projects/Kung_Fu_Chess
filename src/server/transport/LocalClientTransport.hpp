#pragma once

#include <optional>

#include "IClientTransport.hpp"

/**
 * @brief Local transport used for tests.
 *
 * Instead of network communication,
 * snapshots are simply stored in memory.
 */
class LocalClientTransport
    : public IClientTransport
{
public:

    LocalClientTransport() = default;

    ~LocalClientTransport() override = default;

public:

    void sendSnapshot(
        const GameSnapshot& snapshot) override;

    const std::optional<GameSnapshot>&
    getLastSnapshot() const;

private:

    std::optional<GameSnapshot>
        m_lastSnapshot;
};