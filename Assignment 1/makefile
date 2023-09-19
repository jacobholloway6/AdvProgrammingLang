# Compiler to use
CC = gcc



# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# Source files
SRCS = solution.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = assign1

# Rule to link object files to create executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Rule to compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Rule to clean intermediate files and executable
clean:
	rm -f $(OBJS) $(EXEC) rotate.pgm rotate_again.pgm threshold.pgm

# Rule to clean everything and build from scratch
rebuild: clean $(EXEC)

# Specify dependencies of object files on header files


