# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinclude

# Source files
SRC = $(wildcard src/*.c)

# Object files (placed in the build/ directory)
OBJ = $(SRC:src/%.c=build/%.o)

# Target executable
TARGET = build/sensor_simulator

# Default rule: build the executable
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

# Rule to compile .c files into .o files in the build/ directory
build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f build/*.o $(TARGET)

.PHONY: clean
