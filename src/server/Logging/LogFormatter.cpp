#include "LogFormatter.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

std::string LogFormatter::format(
    const LogMessage& message)
{
    std::ostringstream stream;

    stream
        << formatTimestamp(message.timestamp)
        << " ["
        << toString(message.level)
        << "] [Thread "
        << formatThreadId(message.threadId)
        << "] "
        << message.text;

    return stream.str();
}


std::string LogFormatter::formatTimestamp(
    std::chrono::system_clock::time_point timePoint)
{
    using namespace std::chrono;

   const auto duration_ms =
        duration_cast<milliseconds>(
            timePoint.time_since_epoch()
        ) % 1000;

    const std::time_t time =
        system_clock::to_time_t(timePoint);

    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::ostringstream stream;

    stream
        << std::put_time(
               &localTime,
               "%Y-%m-%d %H:%M:%S"
           )
        << '.'
        << std::setw(3)
        << std::setfill('0')
        << duration_ms.count();

    return stream.str();
}


std::string LogFormatter::formatThreadId(
    std::thread::id id)
{
    std::ostringstream stream;

    stream << id;

    return stream.str();
}