#ifndef WRITER_HPP
#define WRITER_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace fs = std::filesystem;

static constexpr const size_t KB = 1024;
static constexpr const size_t MB = 1024 * KB;
static constexpr const size_t GB = 1024 * MB;

class Writer {
  public:
  // Virtual destructor for proper cleanup of derived classes
  virtual ~Writer() = default;

  // Pure virtual function that must be implemented by derived classes
  virtual void write(const std::string& message) = 0;

  // Returns the name of the writer
  virtual const std::string name() const = 0;

  protected:
  // Protected constructor to prevent direct instantiation
  Writer() = default;

  // Delete copy constructor and assignment operator
  Writer(const Writer&) = delete;
  Writer& operator=(const Writer&) = delete;
};

class FileWriter : public Writer {
  static constexpr size_t BUFFER_SIZE = 20 * MB;

  public:
  FileWriter(const std::string& filename) : filename_(filename) {
    if (fs::exists(filename)) {
      throw std::runtime_error("File " + filename + " exists");
    }
    file_ = std::ofstream(filename, std::ios::out | std::ios::app);
    if (!file_.is_open()) {
      throw std::runtime_error("Cannot open file: " + filename);
    }
    buffer_.reserve(BUFFER_SIZE);
  }

  ~FileWriter() {
    if (file_.is_open()) {
      file_ << buffer_;
      file_.flush();
      file_.close();
    }
  }

  const std::string name() const override {
    return "FileWriter";
  }

  void write(const std::string& message) override {
    if (buffer_.size() + message.size() < BUFFER_SIZE) {
      buffer_ += message;
    } else {
      file_ << buffer_;
      buffer_.clear();
      buffer_ += message;
    }
  }

  private:
  std::ofstream file_;
  std::string filename_;
  std::string buffer_;
};

class ConsoleWriter : public Writer {
  public:
  enum class ConsoleType {
    STD_OUT,
    STD_ERROR,
  };

  ConsoleWriter(const ConsoleType& writer_type) : writer_type_(writer_type) {}

  const std::string name() const override {
    return "ConsoleWriter";
  }

  void write(const std::string& message) override {
    if (writer_type_ == ConsoleType::STD_OUT) {
      std::cout << message;
    } else {
      std::cerr << message;
    }
  }

  private:
  ConsoleType writer_type_;
};

class NoneWriter : public Writer {
  const std::string name() const override {
    return "NoneWriter";
  }

  void write(const std::string& message) override {
    // Write to /dev/null (no-op)
  }
};

class WriterFactory {
  public:
  enum class WriterType {
    FILE,
    STDOUT,
    STDERR,
    NONE,
  };

  static std::string to_string(const WriterType& type) {
    switch (type) {
      case WriterType::FILE:
        return "FILE";
      case WriterType::STDOUT:
        return "STDOUT";
      case WriterType::STDERR:
        return "STDERR";
      case WriterType::NONE:
        return "NONE";
      default:
        throw std::invalid_argument("Unknown writer type");
    }
  }
  static std::unique_ptr<Writer> create_writer(const WriterType& type,
                                               const std::string& filename = "") {
    if (type == WriterType::FILE) {
      if (filename.empty()) {
        throw std::invalid_argument("Filename required for file writer");
      }
      return std::make_unique<FileWriter>(filename);
    } else if (type == WriterType::STDOUT) {
      return std::make_unique<ConsoleWriter>(ConsoleWriter::ConsoleType::STD_OUT);
    } else if (type == WriterType::STDERR) {
      return std::make_unique<ConsoleWriter>(ConsoleWriter::ConsoleType::STD_ERROR);
    } else if (type == WriterType::NONE) {
      return std::make_unique<NoneWriter>();
    }
    throw std::invalid_argument("Unknown writer type: " + to_string(type));
  }
};

#endif  // WRITER_HPP
