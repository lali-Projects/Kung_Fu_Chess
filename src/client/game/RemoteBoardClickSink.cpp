#include "RemoteBoardClickSink.hpp"

#include <sstream>
#include <utility>

#include "IClientCommandGateway.hpp"


RemoteBoardClickSink::RemoteBoardClickSink(
    client::IClientCommandGateway& commandGateway)
    : m_commandGateway(commandGateway),
      m_callbackState(std::make_shared<CallbackState>())
{
}


RemoteBoardClickSink::~RemoteBoardClickSink()
{
    const auto callbackState =
        m_callbackState;

    std::unique_lock<std::mutex> lock(
        callbackState->mutex);

    callbackState->alive = false;
    callbackState->handler = nullptr;

    const auto currentThread =
        std::this_thread::get_id();
    const auto currentThreadEntry =
        callbackState->activeCallbacksByThread.find(
            currentThread);
    const std::size_t callbacksOnCurrentThread =
        currentThreadEntry ==
                callbackState->activeCallbacksByThread.end()
            ? 0
            : currentThreadEntry->second;

    callbackState->callbacksFinished.wait(
        lock,
        [&callbackState, callbacksOnCurrentThread]()
        {
            return
                callbackState->activeCallbacks <=
                callbacksOnCurrentThread;
        });
}


void RemoteBoardClickSink::click(
    const Position& position)
{
    std::ostringstream command;
    command
        << "CLICK "
        << position.row
        << " "
        << position.col;

    const auto callbackState = m_callbackState;
    m_commandGateway.sendCommand(
        command.str(),
        [callbackState](const CommandResponse& response)
        {
            ResultHandler handler;
            const auto callbackThread =
                std::this_thread::get_id();

            {
                std::lock_guard<std::mutex> lock(
                    callbackState->mutex);
                if(!callbackState->alive ||
                   !callbackState->handler)
                {
                    return;
                }

                handler = callbackState->handler;
                ++callbackState->activeCallbacks;
                ++callbackState
                    ->activeCallbacksByThread[callbackThread];
            }

            const auto finishCallback =
                [&callbackState, callbackThread]()
                {
                    {
                        std::lock_guard<std::mutex> lock(
                            callbackState->mutex);

                        --callbackState->activeCallbacks;

                        auto threadEntry =
                            callbackState
                                ->activeCallbacksByThread.find(
                                    callbackThread);

                        if(threadEntry !=
                           callbackState
                               ->activeCallbacksByThread.end())
                        {
                            if(--threadEntry->second == 0)
                            {
                                callbackState
                                    ->activeCallbacksByThread.erase(
                                        threadEntry);
                            }
                        }
                    }

                    callbackState->callbacksFinished.notify_all();
                };

            try
            {
                handler(response);
            }
            catch(...)
            {
                finishCallback();
                throw;
            }

            finishCallback();
        });
}


void RemoteBoardClickSink::setResultHandler(
    ResultHandler handler)
{
    std::lock_guard<std::mutex> lock(
        m_callbackState->mutex);
    if(m_callbackState->alive)
    {
        m_callbackState->handler = std::move(handler);
    }
}
