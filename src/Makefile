# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

# Target name
TARGET := systemcc

# Path to your systemc installation (adjust as needed)
SCPATH = ../../systemc

# Additional flags for the compiler
CXXFLAGS := -std=c++14  -I$(SCPATH)/include -L$(SCPATH)/lib -lsystemc -lm -Wall -Wextra

# ---------------------------------------
# CONFIGURATION END
# ---------------------------------------

# Add rpath according to OS
UNAME_S := $(shell uname -s)

ifneq ($(UNAME_S), Darwin)
    CXXFLAGS += -Wl,-rpath=$(SCPATH)/lib
endif

ifeq ($(UNAME_S), Darwin)
    CXXFLAGS += -Wl,-rpath,$(SCPATH)/lib
endif

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

.PHONY: all debug release clean
