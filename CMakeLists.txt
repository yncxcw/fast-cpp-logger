project(cpp-logger)
cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

# Build the logger library
set(SOURCE src/logger.cpp)
add_library(logger STATIC ${SOURCE})
target_include_directories(logger PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Build the main target
add_executable(main src/main.cpp)
target_link_libraries(main logger)
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Build the benchmark target
add_executable(benchmark src/benchmark.cpp)
target_link_libraries(benchmark logger)
target_include_directories(benchmark PRIVATE ${CMAKE_SOURCE_DIR}/include)

option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    # Download gtest if it is not already downloaded.
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    add_subdirectory(tests)
endif()
