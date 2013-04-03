CC=g++
CFLAGS=-O3 -Wall -Werror -I. -lcomm

OBJECTS=src/main.o \
        src/vector.o \
        src/target.o \

all: gamelogic

gamelogic: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

pickup: src/pickup.o
	$(CC) src/pickup.o $(CFLAGS) -o $@

%.o: %.cpp
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	-rm gamelogic
	-rm $(OBJECTS)
