#include "logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

bool Logger::s_initialized = false;

void Logger::Init() {
    s_initialized = true;
    Info("Logger initialized");
}

void Logger::ShutDown() {
    Info("Logger shutdown");
    s_initialized = false;
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (!s_initialized) return;
    std::string levelStr;
    switch(level) {
        case LogLevel::Info: levelStr = "INFO"; break; 
        case LogLevel::Warning: levelStr = "WARNING"; break;
        case LogLevel::Error: levelStr = "ERROR"; break;
    }
    std::cout << "[" << GetCurrentTime() << "] [" << levelStr << "] " << message << std::endl;
}

void Logger::Info(const std::string& message) {
    Log(LogLevel::Info, message);
}

void Logger::Warning(const std::string& message) {
    Log(LogLevel::Warning, message);
}

void Logger::Error(const std::string& message) {
    Log(LogLevel::Error, message);
}

std::string Logger::GetCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm bt;
    localtime_r(&in_time_t, &bt);
    char buf[20];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", &bt);
    return std::string(buf);
}