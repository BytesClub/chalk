#################################
# Makefile						#
# Build Recipie for Executable  #
#								#
# Author: Progyan Bhattacharya  #
# Maintainer: Bytes Club		#
#################################

# Define Compiler
CC=gcc

# Compiler Flags
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c11

# Linker Flags
LFLAGS= 

# Executable
PROG=chalk
EXE=$(BIN)/$(PROG)

# Directories
BIN=build
INC=inc
SRC=src

# Header File(s)
HEADERS=$(wildcard $(INC)/*.h)

# Source File(s)
SOURCES=$(wildcard $(SRC)/*.c)

# Object File(s)
OBJECTS=$(addprefix $(BIN)/, $(notdir $(SOURCES:.c=.o)))

# Default Target
all: $(EXE)
.PHONY: all

# Target Executable and Objects
$(EXE): $(OBJECTS)
	@mkdir -p $(BIN)
	$(CC) $^ -o $@

$(BIN)/%.o: $(SRC)/%.c $(HEADERS)
	@mkdir -p $(BIN)
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

# Help Option
help:
	@echo "Header Files: $(HEADERS)"
	@echo "Source Files: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"
.PHONY: help

# House-keeping
clean:
	rm -rf core $(EXE) $(BIN) *.o *.exe
.PHONY: clean