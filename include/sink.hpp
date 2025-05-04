#ifndef SINK_HPP
#define SINK_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <memory>
#include <string>
#include <thread>
#include <chrono>
#include "ring_buffer.hpp"
#include "writer.hpp"

class Sink {
public:
    explicit Sink(
        std::shared_ptr<RingBuffer<std::string>> buffer,
        std::vector<WriterFactory::WriterType> writer_types,
        const std::string& loger_filename)
        : buffer_(buffer), finished_(false) {
        // Build writers
        for (const auto& writer_type : writer_types) {
            writers_.push_back(WriterFactory::create_writer(writer_type, loger_filename));
        }
        // Start processing in a separate thread
        process_thread_ = std::thread(&Sink::process, this);
    }
    ~Sink() { 
        if (process_thread_.joinable()) {
            process_thread_.join();
        }
    }

    void finish () {
        finished_.store(true, std::memory_order_release);
    }

private:
    // Process items from the buffer until empty
    void process() {
        while (true) {
            auto [item, success] = buffer_->pop();
            // std::cout << "Processing item: " << item << std::endl;
            if (!success) {
                // Empty the buffer, if finished_ is set, we exit the loop
                if(finished_.load(std::memory_order_relaxed)) {
                    break;
                }
                // Buffer is empty, wait for 100ms before checking again
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            for (const auto& writer : writers_) {
                writer->write(item);
            }
        }
    }

private:
    std::shared_ptr<RingBuffer<std::string>> buffer_;
    std::vector<std::unique_ptr<Writer>> writers_;
    std::thread process_thread_;
    std::atomic<bool> finished_;
};

#endif // SINK_HPP
