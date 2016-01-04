CC=gcc
CFLAGS=-g -Wall -std=c99
LDFLAGS=-lSDL_image -lm `sdl-config --libs --cflags` -lESDL
EXEC=main

$(EXEC): main.o
	@$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LDFLAGS)