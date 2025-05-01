
# Compiler
CXX = g++

# Project Name
TARGET = Mandel

# Source and object files
SRCS = Main.cpp ComplexPlane.cpp
OBJS = Main.o ComplexPlane.o

# Compiler Flags
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
