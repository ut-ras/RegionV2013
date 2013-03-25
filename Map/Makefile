CC=g++
CFLAGS=-O3 -Wall -Werror -lcomm -lxml2

OBJECTS=main.o

all: mapper

mapper: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm mapper
	-rm $(OBJECTS)
