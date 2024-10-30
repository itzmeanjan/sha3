EXAMPLE_BUILD_DIR := $(BUILD_DIR)/example

EXAMPLE_DIR := examples
EXAMPLE_SOURCES := $(wildcard $(EXAMPLE_DIR)/*.cpp)
EXAMPLE_HEADERS := $(wildcard $(EXAMPLE_DIR)/*.hpp)
EXAMPLE_EXECS := $(addprefix $(EXAMPLE_BUILD_DIR)/, $(notdir $(patsubst %.cpp,%.exe,$(EXAMPLE_SOURCES))))

$(EXAMPLE_BUILD_DIR):
	mkdir -p $@

$(EXAMPLE_BUILD_DIR)/%.exe: $(EXAMPLE_DIR)/%.cpp $(EXAMPLE_BUILD_DIR)
	$(CXX) $(CXX_DEFS) $(CXX_FLAGS) $(WARN_FLAGS) $(RELEASE_FLAGS) $(I_FLAGS) $< -o $@

example: $(EXAMPLE_EXECS)
	$(foreach exec,$^,./$(exec); echo "--- --- ---";)