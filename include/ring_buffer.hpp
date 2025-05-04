#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <vector>
#include <utility>
#include <stdexcept>
#include <atomic>
#include <memory>

template <typename T>
class RingBuffer {
public:
    static constexpr size_t DEFAULT_CAPACITY = 2000;
    explicit RingBuffer(size_t capacity = DEFAULT_CAPACITY)
        : capacity_(capacity), buffer_size_(capacity + 1), buffer_(capacity + 1), head_(0), tail_(0) {
        if (capacity == 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
    }

    bool push(const T& item) {
        return emplace_push(item);
    }

    bool push(T&& item) {
        return emplace_push(std::move(item));
    }

    template<typename... Args>
    bool emplace(Args&&... args) {
        T item(std::forward<Args>(args)...);
        return emplace_push(std::move(item));
    }
 
    std::pair<T&, bool> pop() {
        size_t current_tail = tail_.load(std::memory_order_relaxed);
        // buffer is empty.
        if (current_tail == head_.load(std::memory_order_acquire)) {  
            T t{};
            return {t, false};
        }

        T& item = buffer_[current_tail];
        tail_.store((current_tail + 1) % buffer_size_, std::memory_order_release);
        return {item, true};
    }

    bool isEmpty() const {
        return tail_.load(std::memory_order_acquire) == 
               head_.load(std::memory_order_acquire);
    }

    bool isFull() const {
        return (head_.load(std::memory_order_acquire) + 1) % buffer_size_ == 
               tail_.load(std::memory_order_acquire);
    }

    size_t capacity() const {
        return capacity_ ;
    }

    void reset() {
        tail_.store(0, std::memory_order_release);
        head_.store(0, std::memory_order_release);
    }

    ~RingBuffer() {}

private:
    const size_t capacity_;
    const size_t buffer_size_;
    std::vector<T> buffer_;
    std::atomic<size_t> head_;  // Write index
    std::atomic<size_t> tail_;  // Read index

    template<typename U>
    bool emplace_push(U&& item) {

        size_t current_head = head_.load(std::memory_order_relaxed);

        size_t next_head = (current_head + 1) % buffer_size_;
        if (next_head == tail_.load(std::memory_order_acquire)) {
            return false;
        }

        buffer_[current_head] = std::forward<U>(item);
        head_.store(next_head, std::memory_order_release);
        return true;
    }
};

#endif // RING_BUFFER_HPP