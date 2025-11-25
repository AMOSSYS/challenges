CC = gcc
CFLAGS = -Wall -Wextra -O2

BIN = gen_sign
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $(BIN) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJ)

cleanall:
	rm -rf $(OBJ) $(BIN)
