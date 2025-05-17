#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include "ring_buffer.hpp"
#include "sink.hpp"

#define LOG_INFO(...) Logger::getInstance().info(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::getInstance().debug(__VA_ARGS__)
#define LOG_WARNING(...) Logger::getInstance().warning(__VA_ARGS__)
#define LOG_ERROR(...) Logger::getInstance().error(__VA_ARGS__)
#define LOG_CRITICAL(...) Logger::getInstance().critical(__VA_ARGS__)

enum class LogLevel : uint8_t { DEBUG = 1, INFO, WARNING, ERROR, CRITICAL };

class Logger {
  public:
  // Singleton pattern to ensure one global instance
  // Static local is also thread-safe in C++11 and later
  static Logger& getInstance() {
    static Logger instance;
    return instance;
  }

  // Delete copy constructor and assignment operator
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  // Initialize logger with configuration
  void init(const std::string& filename = "",
            LogLevel level = LogLevel::INFO,
            bool consoleOutput = true,
            bool override = false);

  // Logging methods
  template <typename... Args>
  void debug(Args&&... args) {
    std::stringstream ss;
    // Fold the args into a stringstream
    (ss << ... << std::forward<Args>(args));
    _debug(ss.str());
  }

  template <typename... Args>
  void info(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    _info(ss.str());
  }

  template <typename... Args>
  void warning(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    _warning(ss.str());
  }

  template <typename... Args>
  void error(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    _error(ss.str());
  }

  template <typename... Args>
  void critical(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    _critical(ss.str());
  }
  void _debug(const std::string& message);
  void _info(const std::string& message);
  void _warning(const std::string& message);
  void _error(const std::string& message);
  void _critical(const std::string& message);

  // Set minimum log level
  void setLogLevel(LogLevel level);

  // Notify the sink to finish.
  void finish();

  private:
  Logger();  // Private constructor
  ~Logger();

  // Core logging function
  void _log(LogLevel level, const std::string& message);
  // Helper functions
  std::string getCurrentTime();
  std::string levelToString(LogLevel level);
  std::string formatMessage(LogLevel level, const std::string& message);

  LogLevel minLogLevel;
  bool consoleOutput;

  std::shared_ptr<RingBuffer<std::string>> buffer;
  std::unique_ptr<Sink> sink;
};

#endif  // LOGGER_H