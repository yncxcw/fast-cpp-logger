#include <gtest/gtest.h>
#include "logger.hpp"
#include <fstream>
#include <filesystem>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

// Test fixture for Writer tests
class LoggerTest : public ::testing::Test {
  protected:
  void SetUp() override {
    // Create a temporary directory for test files
    test_dir = std::filesystem::temp_directory_path() / "logger_test";
    std::filesystem::create_directories(test_dir);
  }

  void TearDown() override {
    // Clean up test files
   std::filesystem::remove_all(test_dir);
  }

  std::filesystem::path test_dir;
};

TEST_F(LoggerTest, LogLevels) {
    auto test_file = test_dir / "apps.log";
    Logger::getInstance().init(test_file.string(), LogLevel::WARNING, true);
    // Test all log levels
    LOG_DEBUG("Debug message");
    LOG_INFO("Info message");
    LOG_WARNING("Warning message");
    LOG_ERROR("Error message");
    LOG_CRITICAL("Critical message");
    // The logger is a singleton, it syncs the log to the file when it is destroyed.
    // In unit tests, we need to call finish() to ensure the log is flushed to the file.
    Logger::getInstance().finish();
    std::ifstream file(test_file, std::ios::in);
    std::string content{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    EXPECT_TRUE(content.find("DEBUG") == std::string::npos);
    EXPECT_TRUE(content.find("INFO") == std::string::npos);
    EXPECT_TRUE(content.find("WARNING") != std::string::npos);
    EXPECT_TRUE(content.find("ERROR") != std::string::npos);
    EXPECT_TRUE(content.find("CRITICAL") != std::string::npos);
} 