cmake_minimum_required(VERSION 3.28)
project(benchcmp_xkcp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Fetch Google Benchmark
include(FetchContent)
FetchContent_Declare(
  benchmark
  URL https://github.com/google/benchmark/archive/refs/tags/v1.9.5.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(benchmark)

# Paths
set(SHA3_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(XKCP_LIB "${CMAKE_CURRENT_SOURCE_DIR}/xkcp/bin/generic64/libXKCP.a")
set(XKCP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/xkcp/bin/generic64/libXKCP.a.headers")

# Compiler optimization flags
add_compile_options(-O3 -march=native -flto)
add_link_options(-flto)

# Combined benchmark: XKCP (plain64) + portable in a single executable
add_executable(benchmarks bench_combined.cpp)
target_include_directories(benchmarks PRIVATE ${XKCP_INCLUDE_DIR} ${SHA3_INCLUDE_DIR})
target_compile_definitions(benchmarks PRIVATE XKCP_BENCH_NAME="xkcp_turboshake128")
target_link_libraries(benchmarks PRIVATE ${XKCP_LIB} benchmark::benchmark_main)
