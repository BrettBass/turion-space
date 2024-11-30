# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinclude
#CFLAGS = -Wall -fsanitize=address -Iinclude
#LDFLAGS = -fsanitize=address

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

# Run the program with Valgrind (for memory leak testing)
valgrind-test: CFLAGS += -g   # Add -g only when running this target
valgrind-test: clean $(TARGET)
	valgrind --leak-check=full -s --suppressions=valgrind.supp --show-leak-kinds=all --track-origins=yes ./$(TARGET) 23 1 2 4 2 10

.PHONY: clean
