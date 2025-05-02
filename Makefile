CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread

SRC = src
OBJS = $(SRC)/main.o $(SRC)/validator_pthread.o $(SRC)/io.o $(SRC)/test_runner.o

EXEC = sudoku_single

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(SRC)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(SRC)/*.o
