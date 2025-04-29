#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "logger.hpp"

class Benchmark {
public:
    Benchmark(const size_t& thread_pool_size, const size_t& message_size, const size_t& message_count): 
        thread_pool_size(thread_pool_size), 
        message_size(message_size), 
        message_count(message_count) {
        this->threads.reserve(thread_pool_size);
    }
    
    void run() {
        size_t log_size = message_size * message_count * thread_pool_size;
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < thread_pool_size; ++i) {
            threads.emplace_back([this]() {
                for (size_t j = 0; j < message_count; ++j) {
                    // Log the message
                    LOG_INFO(std::string(message_size, 'a'));
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Throughput: " << (static_cast<double>(log_size) / (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() * 1000 * 1000) )<< " MB/s" << std::endl;
    }

    ~Benchmark() {}

private:
    size_t thread_pool_size;
    size_t message_size;
    size_t message_count;
    std::vector<std::thread> threads;
};

#endif // BENCHMARK_H