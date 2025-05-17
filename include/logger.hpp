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

#define LOG_INFO(x) Logger::getInstance().info(x)
#define LOG_DEBUG(x) Logger::getInstance().debug(x)
#define LOG_WARNING(x) Logger::getInstance().warning(x)
#define LOG_ERROR(x) Logger::getInstance().error(x)
#define LOG_CRITICAL(x) Logger::getInstance().critical(x)

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
    debug(ss.str());
  }

  template <typename... Args>
  void info(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    info(ss.str());
  }

  template <typename... Args>
  void warning(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    warning(ss.str());
  }

  template <typename... Args>
  void error(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    error(ss.str());
  }

  template <typename... Args>
  void critical(Args&&... args) {
    std::stringstream ss;
    (ss << ... << std::forward<Args>(args));
    critical(ss.str());
  }
  void debug(const std::string& message);
  void info(const std::string& message);
  void warning(const std::string& message);
  void error(const std::string& message);
  void critical(const std::string& message);

  // Set minimum log level
  void setLogLevel(LogLevel level);

  // Notify the sink to finish.
  void finish();

  private:
  Logger();  // Private constructor
  ~Logger();

  // Core logging function
  void log(LogLevel level, const std::string& message);
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