# SHA3 Example

A standalone example project demonstrating how to use the [sha3](https://github.com/itzmeanjan/sha3) library.

## Quick Start

```bash
cd examples

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Run
./build/sha3_256_example
```

## Using `sha3` in Your Own Project

The simplest way to integrate `sha3` into your CMake project is via `FetchContent`:

```cmake
cmake_minimum_required(VERSION 3.28)
project(my_app LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  sha3
  GIT_REPOSITORY https://github.com/itzmeanjan/sha3.git
  GIT_TAG master
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(sha3)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE sha3)
target_compile_features(my_app PRIVATE cxx_std_20)
```

Alternatively, install `sha3` system-wide and use `find_package`:

```bash
# From the sha3 root directory
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
cmake --build build
sudo cmake --install build
```

```cmake
find_package(sha3 REQUIRED)
add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE sha3)
```
