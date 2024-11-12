# Compiler and flags
CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wunused-variable -Wunused-function \
         -Wsign-compare -Wpedantic -Wunreachable-code -Wtype-limits \
         -Wmissing-prototypes

# Directories
BUILD_DIR = build
SRC_DIR = src
OBJFILES = $(BUILD_DIR)/main.o
# Target output file
TARGET = $(BUILD_DIR)/tictactoe

all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile .c to .o file
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

# Link the object files
$(TARGET): $(BUILD_DIR) $(OBJFILES)
	$(CC) $(OBJFILES) -o $(TARGET)

# Clean generated build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean