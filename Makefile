CXX = g++
CXX_FLAGS = -std=c++20
WARN_FLAGS = -Wall -Wextra -pedantic
OPT_FLAGS = -O3 -march=native -mtune=native
I_FLAGS = -I ./include

all: tests

wrapper/libsha3.so: wrapper/sha3.cpp include/*.hpp
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(I_FLAGS) -fPIC --shared $< -o $@

lib: wrapper/libsha3.so

test/a.out: test/main.cpp include/*.hpp include/test/*.hpp
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(I_FLAGS) $< -o $@

tests: lib test/a.out
	cd wrapper/python; python3 -m pytest -v; cd ..
	./test/a.out

clean:
	find . -name '*.out' -o -name '*.o' -o -name '*.so' -o -name '*.gch' | xargs rm -rf

format:
	find . -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i --style="Mozilla" && python3 -m black wrapper/python/*.py

benchmarks/bench.out: benchmarks/main.cpp include/*.hpp include/benchmarks/*.hpp
	# In case you haven't built google-benchmark with libPFM support.
	# More @ https://github.com/google/benchmark/blob/b323288cbac5fd1dd35f153e767497a23c337742/docs/perf_counters.md
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(I_FLAGS) $< -lbenchmark -lpthread -o $@

benchmark: benchmarks/bench.out
	./$< --benchmark_counters_tabular=true

benchmarks/perf.out: benchmarks/main.cpp include/*.hpp include/benchmarks/*.hpp
	# In case you've built google-benchmark with libPFM support.
	# More @ https://github.com/google/benchmark/blob/b323288cbac5fd1dd35f153e767497a23c337742/docs/perf_counters.md
	$(CXX) $(CXX_FLAGS) $(WARN_FLAGS) $(OPT_FLAGS) $(I_FLAGS) -DCYCLES_PER_BYTE $< -lbenchmark -lpthread -lpfm -o $@

perf: benchmarks/perf.out
	./$< --benchmark_counters_tabular=true --benchmark_perf_counters=CYCLES
