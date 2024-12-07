# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -g

# Executable names
MAIN_EXEC = main
TEST_EXEC = tests/test_input_parser
MATCH_TEST_EXEC = tests/test_matching_engine
OUTPUT_TEST_EXEC = tests/test_output_writer

# Source files
SRC = input_parser.c matching_engine.c output_writer.c
MAIN_SRC = main.c
TEST_SRC = tests/test_input_parser.c
MATCH_TEST_SRC = tests/test_matching_engine.c
OUTPUT_TEST_SRC = tests/test_output_writer.c

# Object files
OBJS = $(SRC:.c=.o)

# Default target
all: $(MAIN_EXEC) $(TEST_EXEC) $(MATCH_TEST_EXEC) $(OUTPUT_TEST_EXEC)

# Rule to compile the main program
$(MAIN_EXEC): $(MAIN_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $(MAIN_EXEC) $(MAIN_SRC) $(SRC)

# Rule to compile the input parser test program
$(TEST_EXEC): $(TEST_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $(TEST_EXEC) $(TEST_SRC) $(SRC)

# Rule to compile the matching engine test program
$(MATCH_TEST_EXEC): $(MATCH_TEST_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $(MATCH_TEST_EXEC) $(MATCH_TEST_SRC) $(SRC)

# Rule to compile the output writer test program
$(OUTPUT_TEST_EXEC): $(OUTPUT_TEST_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT_TEST_EXEC) $(OUTPUT_TEST_SRC) $(SRC)

# Clean up compiled files
clean:
	rm -f $(MAIN_EXEC) $(TEST_EXEC) $(MATCH_TEST_EXEC) $(OUTPUT_TEST_EXEC) *.o test_output.csv

# PHONY targets
.PHONY: all clean
