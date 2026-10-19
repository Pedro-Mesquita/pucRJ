CC = gcc
CFLAGS = -Wall -Wextra -g

all: kernel intercontroller app

kernel: kernel.c common.h
	$(CC) $(CFLAGS) -o kernel kernel.c

intercontroller: intercontroller.c common.h
	$(CC) $(CFLAGS) -o intercontroller intercontroller.c

app: app.c common.h
	$(CC) $(CFLAGS) -o app app.c

clean:
	rm -f kernel intercontroller app

.PHONY: all clean
