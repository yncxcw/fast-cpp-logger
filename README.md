# fast-cpp-logger

## Overview
This project is a simple C++ implementaion of a logger system. It decouples the writer and reader using a lock-free ring-buffer to achieve high performance. Right now, it is still a toy exmaple for fun.


## Requirements
- CMake (version 3.20 or higher)
- A C++ compiler that supports C++17 or higher

## Building the Project

Create a build directory and build every thing from scratch
   ```
   mkdir build
   cd build
   # Clang is recommended
   cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
   make

   # Run clang-format and clang-tidy
   cmake --build . --target format
   cmake --build . --target tidy
   ```

## License
This project is licensed under the MIT License. See the LICENSE file for details.