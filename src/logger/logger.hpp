#ifndef LOGGER_HPP
#define LOGGER_HPP


#include <string>

enum class LogLevel {
    Info,
    Warning,
    Error
};

class Logger {
    public:
        static void Init();
        static void ShutDown();
        static void Log(LogLevel level, const std::string& message);
        static void Warning(const std::string& message);
        static void Info(const std::string& message);
        static void Error(const std::string& message);
    private:
        static bool s_initialized;
        static std::string GetCurrentTime();
};

#endif