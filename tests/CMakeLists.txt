add_executable(test_ring_buffer test_ring_buffer.cpp)
message(STATUS "gtest_SOURCE_DIRS: ${GTEST_SOURCE_DIRS}")
target_link_libraries(test_ring_buffer GTest::gtest_main pthread)
target_include_directories(test_ring_buffer PRIVATE ${CMAKE_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})

add_test(NAME test_ring_buffer COMMAND test_ring_buffer)