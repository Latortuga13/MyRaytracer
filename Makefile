CXX = g++           # Compiler
CXXFLAGS = -std=c++11 -Wall -Wextra  # Compiler flags
SRC_DIR = ./       # Source files directory
BUILD_DIR = ./executable   # Build files directory
TARGET = main.cpp  # Executable name

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
# Dependency files
DEPS = $(OBJS:.o=.d)

# Phony targets
.PHONY: all clean

all: $(BUILD_DIR) $(TARGET)

# Rule to make the build directory
$(BUILD_DIR):
    mkdir -p $(BUILD_DIR)

# Rule to make the executable
$(TARGET): $(OBJS)
    $(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include dependency files
-include $(DEPS)

clean:
    $(RM) -r $(BUILD_DIR) $(TARGET)