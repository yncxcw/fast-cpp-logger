#include "benchmark.hpp"

#include <chrono>
#include <iostream>

#include "logger.hpp"

int main() {
  // Initialize the logger
  Logger::getInstance().init("app.log", LogLevel::INFO, false);
  Benchmark benchmark(
      20, 1000, 100000);  // 4 threads, 20 bytes per message, 1000 messages per thread
  benchmark.run();
  return 0;
}
