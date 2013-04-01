CC=g++
CFLAGS=-O3 -Wall -Werror -lcomm -ltinyxml2

OBJECTS=main.o \
        mapper.o

all: mapper

mapper: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

main.o: main.cpp mapper.o
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.cpp
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	-rm mapper
	-rm $(OBJECTS)
