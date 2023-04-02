CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
OPTFLAGS = -O3 -march=native -mtune=native
IFLAGS = -I ./include

all: tests

wrapper/libsha3.so: wrapper/sha3.cpp include/*.hpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(IFLAGS) -fPIC --shared $< -o $@

lib: wrapper/libsha3.so

test/a.out: test/main.cpp include/*.hpp include/test/*.hpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(IFLAGS) $< -o $@

tests: lib test/a.out
	cd wrapper/python; python3 -m pytest -v; cd ..
	./test/a.out

clean:
	find . -name '*.out' -o -name '*.o' -o -name '*.so' -o -name '*.gch' | xargs rm -rf

format:
	find . -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i --style="Mozilla" && python3 -m black wrapper/python/*.py

bench/a.out: bench/main.cpp include/*.hpp include/bench/*.hpp
	# make sure you've google-benchmark globally installed;
	# see https://github.com/google/benchmark/tree/60b16f1#installation
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(IFLAGS) $< -lbenchmark -o $@

benchmark: bench/a.out
	./$< --benchmark_counters_tabular=true
