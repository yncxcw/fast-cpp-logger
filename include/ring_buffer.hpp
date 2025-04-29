#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <vector>
#include <utility>
#include <stdexcept>

template <typename T>
class RingBuffer {
public:
    explicit RingBuffer(size_t capacity)
        : capacity_(capacity), buffer_(capacity), head_(0), tail_(0), size_(0) {
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
        if (isEmpty()) {
            T t{};
            return {t, false};
        }
        T& item = buffer_[tail_];
        tail_ = (tail_ + 1) % capacity_;
        --size_;
        return {item, true};
    }

    const bool isEmpty() const {
        return size_ == 0;
    }

    const bool isFull() const {
        return size_ == capacity_;
    }

    const size_t size() const {
        return size_;
    }

    const size_t capacity() const {
        return capacity_;
    }

    void reset() {
        head_ = 0;
        tail_ = 0;
        size_ = 0;
    }

private:
    size_t capacity_;
    std::vector<T> buffer_;
    std::atomic<size_t> head_;
    std::atomic<size_t> tail_;
    std::atomic<size_t> size_;

    template<typename U>
    bool emplace_push(U&& item) {
        if (isFull()) {
            return false;
        }
        buffer_[head_] = std::forward<U>(item);
        head_ = (head_ + 1) % capacity_;
        ++size_;
        return true;
    }
};

#endif // RING_BUFFER_HPP