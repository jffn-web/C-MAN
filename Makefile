CC = gcc

CFLAGS = -I cli-lib/include

SRC = src/main.c cli-lib/src/keyboard.c cli-lib/src/screen.c cli-lib/src/timer.c

OUT = cman

all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)