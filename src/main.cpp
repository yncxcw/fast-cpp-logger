#include <iostream>

#include "logger.hpp"

int main() {
  // Initialize the logger
  Logger::getInstance().init("app.log", LogLevel::INFO, true);
  LOG_INFO("This is an info message");
  Logger::getInstance().debug("Debugging information");
  Logger::getInstance().debug("This", "is", "an", "info", "message", 1, 2, 3);
  Logger::getInstance().warning("This is a warning message");
  Logger::getInstance().warning("This", "is", "a", "warning", "message", 1, 2, 3);
  Logger::getInstance().error("An error occurred");
  Logger::getInstance().error("This", "is", "an", "error", "message", 1, 2, 3);
  Logger::getInstance().critical("Critical error! Immediate attention required");
  Logger::getInstance().critical("This", "is", "a", "critical", "message", 1, 2, 3);
  // Logger::getInstance().finish();
  return 0;
}