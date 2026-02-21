cmake_minimum_required(VERSION 3.28)

project(sha3 VERSION 0.1.0 LANGUAGES CXX)

# --- Options ---
option(SHA3_BUILD_TESTS "Build tests" OFF)
option(SHA3_BUILD_EXAMPLES "Build examples" OFF)
option(SHA3_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(SHA3_FETCH_DEPS "Fetch missing dependencies (GTest, Benchmark)" OFF)

# --- Top-level-only settings (skipped when consumed via FetchContent/add_subdirectory) ---
if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

  option(SHA3_ENABLE_LTO "Enable Interprocedural Optimization (LTO)" ON)
  option(SHA3_NATIVE_OPT "Enable -march=native (not suitable for cross-compilation)" OFF)
  option(SHA3_ASAN "Enable AddressSanitizer" OFF)
  option(SHA3_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)

  # --- Tools ---
  find_program(CLANG_TIDY_EXE clang-tidy)
  find_program(CLANG_FORMAT_EXE clang-format)

  if(CLANG_TIDY_EXE)
    file(GLOB_RECURSE TIDY_SOURCES "examples/*.cpp")

    add_custom_target(tidy
      COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} --config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy --header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/sha3/.* ${TIDY_SOURCES}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Running clang-tidy"
    )
  endif()

  if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE FORMAT_SOURCES
         "include/**/*.hpp"
         "examples/*.cpp"
         "benches/*.hpp"
         "benches/*.cpp"
         "tests/*.hpp"
         "tests/*.cpp")

    add_custom_target(format
      COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${FORMAT_SOURCES}
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      COMMENT "Running clang-format"
    )
  endif()

  # --- Standard settings ---
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)

  # --- Compiler Warnings ---
  if(MSVC)
    set(SHA3_WARNING_FLAGS /W4)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
  else()
    set(SHA3_WARNING_FLAGS -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wformat=2 -Wcast-qual -Wold-style-cast -Wundef -Werror)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      list(APPEND SHA3_WARNING_FLAGS -Wno-pass-failed)
    endif()
  endif()

  # --- Sanitizers ---
  if(SHA3_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls)
    add_link_options(-fsanitize=address)
  endif()

  if(SHA3_UBSAN)
    add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-sanitize-recover=undefined)
    add_link_options(-fsanitize=undefined)
  endif()

  # --- Native Optimization ---
  if(SHA3_NATIVE_OPT)
    add_compile_options(-march=native)
    message(STATUS "Enabled -march=native (machine-specific optimization)")
  endif()

  # --- LTO ---
  if(SHA3_ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if(result)
      set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
      message(WARNING "LTO is not supported: ${output}")
    endif()
  endif()
endif()

# --- Library (header-only → INTERFACE) ---
add_library(sha3 INTERFACE)
target_include_directories(sha3 INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_compile_features(sha3 INTERFACE cxx_std_20)

# --- Tests ---
if(SHA3_BUILD_TESTS)
  enable_testing()
  find_package(GTest QUIET)

  if(NOT GTest_FOUND)
    if(SHA3_FETCH_DEPS)
      message(STATUS "GTest not found locally. Fetching...")
      include(FetchContent)
      FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
      )
      set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
      FetchContent_MakeAvailable(googletest)
    else()
      message(FATAL_ERROR "GTest not found. Install it locally or set SHA3_FETCH_DEPS=ON")
    endif()
  endif()

  file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")

  add_executable(sha3_tests ${TEST_SOURCES})
  target_link_libraries(sha3_tests PRIVATE sha3 GTest::gtest_main)
  target_include_directories(sha3_tests PRIVATE tests)
  target_compile_options(sha3_tests PRIVATE ${SHA3_WARNING_FLAGS})

  include(GoogleTest)
  gtest_discover_tests(sha3_tests)

  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/kats" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

# --- Benchmarks ---
if(SHA3_BUILD_BENCHMARKS)
  find_package(benchmark QUIET)

  if(NOT benchmark_FOUND)
    if(SHA3_FETCH_DEPS)
      message(STATUS "Google Benchmark not found locally. Fetching...")
      include(FetchContent)
      FetchContent_Declare(
        benchmark
        URL https://github.com/google/benchmark/archive/refs/tags/v1.9.5.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
      )
      set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
      set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
      FetchContent_MakeAvailable(benchmark)
    else()
      message(FATAL_ERROR "Google Benchmark not found. Install it locally or set SHA3_FETCH_DEPS=ON")
    endif()
  endif()

  file(GLOB BENCHMARK_SOURCES CONFIGURE_DEPENDS "benches/*.cpp")
  find_library(LIBPFM pfm)

  add_executable(sha3_benchmarks ${BENCHMARK_SOURCES})
  target_link_libraries(sha3_benchmarks PRIVATE sha3 benchmark::benchmark_main)

  if(LIBPFM)
    target_link_libraries(sha3_benchmarks PRIVATE ${LIBPFM})
    message(STATUS "Found libpfm: ${LIBPFM} - linking it to benchmarks")
  endif()

  target_include_directories(sha3_benchmarks PRIVATE benches)
  target_compile_options(sha3_benchmarks PRIVATE ${SHA3_WARNING_FLAGS})
endif()

# --- Examples ---
if(SHA3_BUILD_EXAMPLES)
  file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS "examples/*.cpp")
  foreach(ex_src ${EXAMPLE_SOURCES})
    get_filename_component(ex_name ${ex_src} NAME_WE)
    add_executable(${ex_name} ${ex_src})
    target_link_libraries(${ex_name} PRIVATE sha3)
    target_compile_options(${ex_name} PRIVATE ${SHA3_WARNING_FLAGS})
  endforeach()
endif()

# --- Install ---
include(GNUInstallDirs)
install(TARGETS sha3 EXPORT sha3-config INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT sha3-config NAMESPACE sha3:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/sha3)
