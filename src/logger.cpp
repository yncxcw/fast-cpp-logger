#include "logger.hpp"

#include <filesystem>
#include <stdexcept>

Logger::Logger() : minLogLevel(LogLevel::INFO), consoleOutput(true) {}

Logger::~Logger() {
  // Notify the witer to finish.
  sink->finish();
}

void Logger::finish() {
  sink->finish();
}

void Logger::init(const std::string& filename, LogLevel level, bool console, bool override) {
  minLogLevel = level;
  consoleOutput = console;

  // Initialize the buffer using the default capacity which is 2000.
  buffer = std::make_shared<RingBuffer<std::string>>();
  std::vector<WriterFactory::WriterType> writer_types;

  // Initialize the sink which will write to the console and/or a file.
  if (console) {
    writer_types.push_back(WriterFactory::WriterType::STDOUT);
  }
  if (!filename.empty()) {
    writer_types.push_back(WriterFactory::WriterType::FILE);
  }
  sink = std::make_unique<Sink>(buffer, writer_types, filename);
}

void Logger::log(LogLevel level, const std::string& message) {
  if (level < minLogLevel) {
    return;
  }
  std::string formatted = formatMessage(level, message);
  buffer->push(formatted);
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
    case LogLevel::DEBUG:
      return "DEBUG";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::CRITICAL:
      return "CRITICAL";
    default:
      return "UNKNOWN";
  }
}

std::string Logger::formatMessage(LogLevel level, const std::string& message) {
  std::stringstream ss;
  ss << "[" << getCurrentTime() << "]"
     << "[" << levelToString(level) << "] " << message << std::endl;
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
  // TODO: This needs to be thread safe.
  minLogLevel = level;
}