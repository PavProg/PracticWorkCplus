#ifndef LOGGER_HPP
#define LOGGER_HPP


#include <string>
#include <fstream>

enum class LogLevel {
    Info,
    Warning,
    Error
};

class Logger {
    public:
        static void Init(const std::string& filename = "log.txt");
        static void ShutDown();
        static void Log(LogLevel level, const std::string& message);
        static void Warning(const std::string& message);
        static void Info(const std::string& message);
        static void Error(const std::string& message);
    private:
        static bool s_initialized;
        static std::ofstream s_file;
        static std::string GetCurrentTime();
};

#endif