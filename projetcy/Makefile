# Compiler and flags
CC = gcc
CFLAGS = -IHeaders  # Include the Headers directory

# Object files
OBJ = main.o \
			codeC/functions_AVL.o \
			codeC/read.o

# Target
all: main

# Rule to compile the object files
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

codeC/functions_AVL.o: codeC/functions_AVL.c
	$(CC) $(CFLAGS) -c codeC/functions_AVL.c -o codeC/functions_AVL.o

codeC/read.o: codeC/read.c
	$(CC) $(CFLAGS) -c codeC/read.c -o codeC/read.o

# Rule to link object files into an executable
main: $(OBJ)
	$(CC) -o main $(OBJ) -lm

# Clean up object files and the executable
clean:
	rm -f $(OBJ) main

# Run the shell script (if applicable)
run-shell:
	./c-wire.sh
