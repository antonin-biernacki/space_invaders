# Makefile for project S5

TARGET= project
CFLAGS=-g -W -Wall -Wextra
LDFLAGS= -LGraphique -LListeChainee
LDLIBS= -lgraph -lSDL -lSDL_ttf
all: local_libs $(TARGET)

default: $(TARGET)

main.o: main.c
	gcc $(CFLAGS) -c main.c #-02


$(TARGET): main.o
	gcc $(LDFLAGS)  main.o $(LDLIBS) -o $(TARGET)

local_libs:
	cd Graphique && make && cd ..


.PHONY: clean

clean:
	rm -f *.o
	rm -f $(TARGET)

