#include "logger.hpp"
#include <stdexcept>
#include <filesystem>

Logger::Logger() : minLogLevel(LogLevel::INFO), consoleOutput(true) {}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void Logger::init(const std::string& filename, LogLevel level, bool console, bool override) {
    std::lock_guard<std::mutex> lock(logMutex);
    
    minLogLevel = level;
    consoleOutput = console;

    if (!filename.empty()) {
        if (override && std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
        logFile.open(filename, std::ios::out | std::ios::app);
        if (!logFile.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < minLogLevel) {
        return;
    }

    std::lock_guard<std::mutex> lock(logMutex);
    std::string formatted = formatMessage(level, message);

    if (consoleOutput) {
        std::cout << formatted << std::endl;
    }

    if (logFile.is_open()) {
        // logFile << formatted << std::endl;
        // logFile.flush();  // Ensure messages are written immediately
    }
}

std::string Logger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto local_time = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&local_time), "%Y-%m-%d %X");
    return ss.str();
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:    return "DEBUG";
        case LogLevel::INFO:     return "INFO";
        case LogLevel::WARNING:  return "WARNING";
        case LogLevel::ERROR:    return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default:                 return "UNKNOWN";
    }
}

std::string Logger::formatMessage(LogLevel level, const std::string& message) {
    std::stringstream ss;
    ss << "[" << getCurrentTime() << "]"
       << "[" << levelToString(level) << "] "
       << message;
    return ss.str();
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::critical(const std::string& message) {
    log(LogLevel::CRITICAL, message);
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex);
    minLogLevel = level;
}