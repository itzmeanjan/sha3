EXAMPLE_BUILD_DIR := $(BUILD_DIR)/example

EXAMPLE_DIR := examples
EXAMPLE_SOURCES := $(wildcard $(EXAMPLE_DIR)/*.cpp)
EXAMPLE_HEADERS := $(wildcard $(EXAMPLE_DIR)/*.hpp)
EXAMPLE_EXECS := $(addprefix $(EXAMPLE_BUILD_DIR)/, $(notdir $(EXAMPLE_SOURCES:.cpp=.exe)))

$(EXAMPLE_BUILD_DIR):
	mkdir -p $@

$(EXAMPLE_BUILD_DIR)/%.exe: $(EXAMPLE_DIR)/%.cpp $(EXAMPLE_BUILD_DIR)
	$(CXX) $(CXX_DEFS) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_FLAGS) $(I_FLAGS) $< -o $@

example: $(EXAMPLE_EXECS) ## Build and run all example programs, demonstrating usage of SHA3 API
	$(foreach exec,$^,./$(exec); echo "--- --- ---";)
