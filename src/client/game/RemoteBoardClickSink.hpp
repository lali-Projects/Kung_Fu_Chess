#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "CommandResponse.hpp"
#include "IBoardClickSink.hpp"


namespace client
{
    class IClientCommandGateway;
}


/** Converts GUI board input into asynchronous server CLICK commands. */
class RemoteBoardClickSink : public IBoardClickSink
{
public:
    using ResultHandler =
        std::function<void(const CommandResponse&)>;

    explicit RemoteBoardClickSink(
        client::IClientCommandGateway& commandGateway);
    ~RemoteBoardClickSink() override;

    void click(const Position& position) override;
    void setResultHandler(ResultHandler handler);

private:
    struct CallbackState
    {
        std::mutex mutex;
        std::condition_variable callbacksFinished;
        ResultHandler handler;
        std::size_t activeCallbacks{0};
        std::unordered_map<
            std::thread::id,
            std::size_t> activeCallbacksByThread;
        bool alive{true};
    };

    client::IClientCommandGateway& m_commandGateway;
    std::shared_ptr<CallbackState> m_callbackState;
};
