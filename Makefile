# Makefile for project S5

SOURCES= $(wildcard *.c)
OBJECTS= $(SOURCES:.c=.o)
TARGET= space_invaders
CFLAGS=-g -W -Wall -Wextra -MMD
LDFLAGS= -LGraphique -LListeChainee
LDLIBS= -lgraph -lSDL -lSDL_ttf
DEPS= $(SOURCES:.c=.d)


all: local_libs $(TARGET)


$(TARGET): $(OBJECTS) ListeChainee/liste.o
	gcc $(LDFLAGS) $(OBJECTS) ListeChainee/liste.o $(LDLIBS) -o $(TARGET)

ListeChainee/liste.o: ListeChainee/liste.c
	gcc $(CFLAGS) -c $< -o $@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@


local_libs:
	cd Graphique && make && cd ..

-include $(DEPS)

clean:
	rm -f $(OBJECTS)
	rm -f $(DEPS)
	rm -f $(TARGET)
	cd Graphique && make clean && cd ..


rebuild: clean all

