.DEFAULT_GOAL := help

.PHONY: help
help: 
	@for file in $(MAKEFILE_LIST); do \
		grep -E '^[a-zA-Z_-]+:.*?## .*$$' $${file} | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}';\
	done


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

include tests/test.mk
include benches/bench.mk
include examples/example.mk

$(GTEST_PARALLEL):
	git submodule update --init gtest-parallel

.PHONY: clean
clean: ## Remove build directory
	rm -rf $(BUILD_DIR)

.PHONY: format
format: $(SHA3_SOURCES) $(TEST_SOURCES) $(TEST_HEADERS) $(BENCHMARK_SOURCES) $(BENCHMARK_HEADERS) $(EXAMPLE_SOURCES) $(EXAMPLE_HEADERS) ## Format source code
	clang-format -i $^
