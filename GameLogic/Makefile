CC=g++
CFLAGS=-O3 -Wall -Werror -I. -lcomm

OBJECTS=src/main.o

all: gamelogic

gamelogic: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm gamelogic
	-rm $(OBJECTS)
