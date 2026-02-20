cmake_minimum_required(VERSION 3.28)
project(sha3-example LANGUAGES CXX)

# --- Fetch sha3 ---
# If sha3 is installed on the system, find_package will find it.
# Otherwise, fetch it from the parent directory (or a remote repository).
find_package(sha3 QUIET)

if(NOT sha3_FOUND)
  message(STATUS "sha3 not found via find_package, using FetchContent...")
  include(FetchContent)
  FetchContent_Declare(
    sha3
    # Option A: For local development, point to the parent directory:
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..
    # Option B: For production use, point to the GitHub repository:
    #
    # GIT_REPOSITORY https://github.com/itzmeanjan/sha3.git
    # GIT_TAG master
    # GIT_SHALLOW TRUE
  )
  FetchContent_MakeAvailable(sha3)
endif()

# --- Example Executable ---
add_executable(sha3_256_example sha3_256.cpp)
target_link_libraries(sha3_256_example PRIVATE sha3)
target_compile_features(sha3_256_example PRIVATE cxx_std_20)
