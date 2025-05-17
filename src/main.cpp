#include <iostream>

#include "logger.hpp"

int main() {
  // Initialize the logger
  Logger::getInstance().init("app.log", LogLevel::INFO, true);
  LOG_INFO("This is an info message");
  LOG_DEBUG("Debugging information");
  LOG_INFO("This", "is", "an", "info", "message", 1, 2, 3);
  LOG_WARNING("This is a warning message");
  LOG_WARNING("This", "is", "a", "warning", "message", 1, 2, 3);
  LOG_ERROR("An error occurred");
  LOG_ERROR("This", "is", "an", "error", "message", 1, 2, 3);
  LOG_CRITICAL("Critical error! Immediate attention required");
  LOG_CRITICAL("This", "is", "a", "critical", "message", 1, 2, 3);
  return 0;
}