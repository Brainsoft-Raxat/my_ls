# Makefile
.PHONY = all clean

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=address
DEPS = my_ls.h
OBJ = main.o my_ls.o 
TARGET = my_ls

all: $(TARGET)

%.o: %.c $(DEPS)
	@echo "Creating object.."
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	@echo "Compiling..."
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	@echo "Cleaning up..."
	rm -rvf *.o my_ls