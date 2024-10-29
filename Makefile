CXX ?= clang++
CXX_DEFS +=
CXX_FLAGS := -std=c++20
WARN_FLAGS := -Wall -Wextra -Wpedantic
DEBUG_FLAGS := -O1 -g
RELEASE_FLAGS := -O3 -march=native
LINK_OPT_FLAGS := -flto

I_FLAGS := -I ./include
PERF_DEFS = -DCYCLES_PER_BYTE

SRC_DIR := include
SHA3_SOURCES := $(shell find $(SRC_DIR) -name '*.hpp')
BUILD_DIR := build

all: test

include tests/test.mk
include benches/bench.mk
include examples/example.mk

$(GTEST_PARALLEL):
	git submodule update --init gtest-parallel

.PHONY: format clean

clean:
	rm -rf $(BUILD_DIR)

format: $(SHA3_SOURCES) $(TEST_SOURCES) $(TEST_HEADERS) $(BENCHMARK_SOURCES) $(BENCHMARK_HEADERS) $(EXAMPLE_SOURCES) $(EXAMPLE_HEADERS)
	clang-format -i $^
