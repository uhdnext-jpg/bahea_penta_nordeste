CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11 -g
INCLUDE= -Iinclude
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
BIN=helpdesk

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

src/%.o: src/%.c include/%.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

src/main.o: src/main.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

src/logger.o: src/logger.c include/logger.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

src/utils.o: src/utils.c include/utils.h include/fila.h include/pilha.h include/chamado.h
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f src/*.o $(BIN) log.txt

run: all
	./$(BIN)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN)

.PHONY: all clean run valgrind
