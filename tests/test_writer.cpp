#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <filesystem>


#include "writer.hpp"

// Mock Writer for testing
class MockWriter : public Writer {
public:
    void write(const std::string& message) override {
        written_messages.push_back(message);
    }

    const std::string name() const override {
        return "MockWriter";
    }
    std::vector<std::string> written_messages;
};

// Test fixture for Writer tests
class WriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary directory for test files
        test_dir = std::filesystem::temp_directory_path() / "writer_test";
        std::filesystem::create_directories(test_dir);
        std::cout << "Setup Test directory: " << test_dir << std::endl;
    }

    void TearDown() override {
        // Clean up test files
        std::filesystem::remove_all(test_dir);
        std::cout << "Tear down Test directory: " << test_dir << std::endl;
    }

    std::filesystem::path test_dir;
};

// Test MockWriter
TEST_F(WriterTest, MockWriterWritesMessages) {
    MockWriter writer;
    std::string message = "Test message";
    
    writer.write(message);
    
    ASSERT_EQ(writer.written_messages.size(), 1);
    EXPECT_EQ(writer.written_messages[0], message);
}

// Test FileWriter
TEST_F(WriterTest, FileWriterCreatesFile) {
    std::string filename = (test_dir / "test.txt").string();
    FileWriter writer(filename);    
    EXPECT_TRUE(std::filesystem::exists(filename));
}

TEST_F(WriterTest, FileWriterWritesContent) {
    std::string filename = (test_dir / "test.txt").string();
    {
        FileWriter writer(filename);
        writer.write("Test message\n");
        writer.write("Another message\n");
    }
    // Read the file and verify content
    std::ifstream file(filename, std::ios::in);
    std::string line;
    std::vector<std::string> lines;
    
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    ASSERT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "Test message");
    EXPECT_EQ(lines[1], "Another message");
}

TEST_F(WriterTest, FileWriterWritesLargeContent) {
    std::string filename = (test_dir / "test.txt").string();
    {
        FileWriter writer(filename);
        // Write 100MB of data
        for (int i = 0; i < 100; i++) {
            std::string large_message(MB, 'a');
            writer.write(large_message);
        }
    }
    // Read the file and verify content
    std::ifstream file(filename, std::ios::in); 
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line;
    }

    // Verify file size is 100MB
    EXPECT_EQ(content.size(), 100 * MB);
    
    // Verify content is all 'a' characters
    EXPECT_TRUE(std::all_of(content.begin(), content.end(), 
                           [](const char& c) { return c == 'a'; }));
}

TEST_F(WriterTest, FileWriterThrowsOnExistingFile) {
    std::string filename = (test_dir / "test.txt").string();
    
    // Create the file first
    std::ofstream file(filename);
    file << "Some content";
    file.close();
    
    EXPECT_THROW(FileWriter writer(filename), std::runtime_error);
}

TEST_F(WriterTest, ConsoleWriterStdOut) {
    testing::internal::CaptureStdout();
    
    {
        ConsoleWriter writer(ConsoleWriter::ConsoleType::STD_OUT);
        writer.write("Test message");
        writer.write("Another message");
    }

    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Test message\nAnother message\n");
}

TEST_F(WriterTest, ConsoleWriterStdErr) {
    testing::internal::CaptureStderr();
    
    {
        ConsoleWriter writer(ConsoleWriter::ConsoleType::STD_ERROR); 
        writer.write("Test error");
        writer.write("Another error");
    }

    std::string output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(output, "Test error\nAnother error\n");
}
