# Compiler
CC = gcc

# Flags
CFLAGS = -Wall -Wextra -std=c11

# Executable name
TARGET = scheduler.exe

# Source files
SRCS = main.c file_extract.c arraylist.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default build
all: $(TARGET)

# Link everything
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile each .c file
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Optional: run shortcut
run:
	./$(TARGET) input.txt output.txt