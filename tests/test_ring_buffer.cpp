#include <gtest/gtest.h>
#include <string>
#include "ring_buffer.hpp"

class TestItem {
public:
    TestItem(): i_value(0), f_value(0.0f) {}
    TestItem(int i_value, float f_value):
        i_value(i_value), f_value(f_value) {}
    ~TestItem() = default;
    int i_value;
    float f_value;
};

TEST(RingBufferTest, DefaultConstructor) {
    RingBuffer<int> buffer(10);
    EXPECT_EQ(buffer.capacity(), 10);
    EXPECT_TRUE(buffer.isEmpty());
}

TEST(RingBufferTest, PushAndPop) {
    RingBuffer<int> buffer(3);
    EXPECT_TRUE(buffer.push(1));
    EXPECT_TRUE(buffer.push(2));
    EXPECT_TRUE(buffer.push(3));
    EXPECT_FALSE(buffer.push(4));
    EXPECT_TRUE(buffer.isFull());
    
    EXPECT_EQ(buffer.pop().first, 1);
    EXPECT_EQ(buffer.pop().first, 2);
    EXPECT_EQ(buffer.pop().first, 3);
    auto [item, success] = buffer.pop();
    EXPECT_FALSE(success);
    EXPECT_TRUE(buffer.isEmpty());
}

TEST(RingBufferTest, PushAndPopWithTestItem) {
    RingBuffer<TestItem> buffer(3);
    // emplace
    EXPECT_TRUE(buffer.emplace(1, 0.2));
    // rvalue
    EXPECT_TRUE(buffer.push(TestItem(2, 0.3)));
    // lvalue
    auto item = TestItem(3, 0.4);
    EXPECT_TRUE(buffer.emplace(item));
    EXPECT_TRUE(buffer.isFull());
    EXPECT_EQ(buffer.pop().first.i_value, 1);
    EXPECT_EQ(buffer.pop().first.i_value, 2);
    EXPECT_EQ(buffer.pop().first.i_value, 3);
}

TEST(RingBufferTest, BufferfullAndEmpty) {
    RingBuffer<std::string> buffer(2);
    EXPECT_TRUE(buffer.push("1"));
    EXPECT_TRUE(buffer.push("2"));
    EXPECT_TRUE(buffer.isFull());
    // Should not push as buffer is full
    EXPECT_FALSE(buffer.push("3"));
    
    EXPECT_EQ(buffer.pop().first, "1");
    EXPECT_EQ(buffer.pop().first, "2");
    EXPECT_TRUE(buffer.isEmpty());
    // Should not pop as buffer is empty
    auto [item, success] = buffer.pop();
    EXPECT_FALSE(success);
}

TEST(RingBufferTest, Reset) {
    RingBuffer<int> buffer(3);
    buffer.push(1);
    buffer.push(2);
    buffer.reset();
    EXPECT_TRUE(buffer.isEmpty());
    EXPECT_FALSE(buffer.isFull());
    EXPECT_EQ(buffer.pop().second, false);
}