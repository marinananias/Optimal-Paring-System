# Compiler and flags
CC = gcc
# CFLAGS = -Wall -Wextra -pthread -g
CFLAGS = -Wall -Wextra -pthread -g -fsanitize=address

# Executable names
MAIN_EXEC = main
TEST_INPUT_EXEC = tests/test_input_parser
# TEST_MATCHING_EXEC = tests/test_matching_engine
# TEST_SOLUTION_EXEC = tests/test_solution_selector

# Source files
SRC = input_parser.c matching_engine.c solution_selector.c synchronization.c output_writer.c
MAIN_SRC = main.c
TEST_INPUT_SRC = tests/test_input_parser.c
# TEST_MATCHING_SRC = tests/test_matching_engine.c
# TEST_SOLUTION_SRC = tests/test_solution_selector.c

# Default target
all: $(MAIN_EXEC) $(TEST_INPUT_EXEC) $(TEST_MATCHING_EXEC) $(TEST_SOLUTION_EXEC)

# Rule to compile the main program
$(MAIN_EXEC): $(MAIN_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $(MAIN_EXEC) $(MAIN_SRC) $(SRC)

# Rule to compile the input parser test program
$(TEST_INPUT_EXEC): $(TEST_INPUT_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $(TEST_INPUT_EXEC) $(TEST_INPUT_SRC) $(SRC)

# Rule to compile the matching engine test program
# $(TEST_MATCHING_EXEC): $(TEST_MATCHING_SRC) $(SRC)
# 	$(CC) $(CFLAGS) -o $(TEST_MATCHING_EXEC) $(TEST_MATCHING_SRC) $(SRC)

# Rule to compile the solution selector test program
# $(TEST_SOLUTION_EXEC): $(TEST_SOLUTION_SRC) $(SRC)
# 	$(CC) $(CFLAGS) -o $(TEST_SOLUTION_EXEC) $(TEST_SOLUTION_SRC) $(SRC)

# Clean up compiled files
clean:
	rm -f $(MAIN_EXEC) $(TEST_INPUT_EXEC) *.o

# PHONY targets
.PHONY: all clean
