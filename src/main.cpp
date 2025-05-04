#include <iostream>

#include "logger.hpp"

int main() {
  // Initialize the logger
  Logger::getInstance().init("app.log", LogLevel::INFO, true);
  LOG_INFO("This is an info message");
  Logger::getInstance().debug("Debugging information");
  Logger::getInstance().warning("This is a warning message");
  Logger::getInstance().error("An error occurred");
  Logger::getInstance().critical("Critical error! Immediate attention required");
  // Logger::getInstance().finish();
  return 0;
}