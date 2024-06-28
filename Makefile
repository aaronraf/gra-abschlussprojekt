# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

# Entry point for the program
C_SRCS = main.c
CPP_SRCS = simulation.cpp

# Object files
C_OBJS = $(C_SRCS:.c=.o)
CPP_OBJS = $(CPP_SRCS:.cpp=.o)

# Assignment task files
# HEADERS := 

# Target name
TARGET := cachesim

# Path to your systemc installation (adjust as needed)
SCPATH = ../../systemc

# Additional flags for the compiler
CXXFLAGS := -std=c++14  -I$(SCPATH)/include -L$(SCPATH)/lib -lsystemc -lm -Wall -Wextra

# ---------------------------------------
# CONFIGURATION END
# ---------------------------------------

# Determine if clang or gcc as well as clang++ or g++ are available
CXX := $(shell command -v g++ || command -v clang++)
ifeq ($(strip $(CXX)),)
    $(error Neither clang++ nor g++ is available. Exiting.)
endif

CC := $(shell command -v gcc || command -v clang)
ifeq ($(strip $(CC)),)
    $(error Neither clang nor g is available. Exiting.)
endif

# Add rpath according to OS
UNAME_S := $(shell uname -s)

ifneq ($(UNAME_S), Darwin)
    CXXFLAGS += -Wl,-rpath=$(SCPATH)/lib
endif

ifeq ($(UNAME_S), Darwin)
    CXXFLAGS += -Wl,-rpath $(SCPATH)/lib
endif

# Default to release build for both app and library
all: debug

# Rule to compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS += -g
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O2
release: $(TARGET)

# Rule to link object files to executable
$(TARGET): $(C_OBJS) $(CPP_OBJS)
	$(CXX) $(CXXFLAGS) $(C_OBJS) $(CPP_OBJS) $(LDFLAGS) -o $(TARGET)

# Clean up
clean:
	rm -f $(TARGET)
	rm -rf *.o

.PHONY: all debug release clean
