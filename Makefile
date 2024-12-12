# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -g -fsanitize=address

# Executable names
MAIN_EXEC = main

# Source files
SRC = input_parser.c matching_engine.c solution_selector.c synchronization.c output_writer.c
MAIN_SRC = main.c
TEST_INPUT_SRC = tests/test_input_parser.c

# Default target
all: $(MAIN_EXEC) $(TEST_INPUT_EXEC) $(TEST_MATCHING_EXEC) $(TEST_SOLUTION_EXEC)

# Rule to compile the main program
$(MAIN_EXEC): $(MAIN_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $(MAIN_EXEC) $(MAIN_SRC) $(SRC)

# Clean up compiled files
clean:
	rm -f $(MAIN_EXEC) *.o

# PHONY targets
.PHONY: all clean
