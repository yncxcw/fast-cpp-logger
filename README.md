# My C++ Project

## Overview
This project is a simple C++ application that demonstrates the structure and organization of a C++ project using CMake. It includes utility functions that can be reused throughout the application.


## Requirements
- CMake (version 3.20 or higher)
- A C++ compiler that supports C++11 or higher

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