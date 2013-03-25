CC=g++
DEST=/usr/local
LIB=libcomm.so
CFLAGS=-O3 -Wall -I. -DCOMM_PATH=\"/home/spokebot/spokebot/Comm/comms\"

OBJECTS=src/master.o \
        src/slave.o  \
        src/block_slave.o

all: $(LIB) driver commtest

install: all
	cp driver $(DEST)/bin
	cp commtest $(DEST)/bin
	cp $(LIB) $(DEST)/lib
	-rm -r $(DEST)/include/comm
	cp -r comm $(DEST)/include/comm
	ldconfig

$(LIB): $(OBJECTS)
	$(CC) $(CFLAGS) -shared $(OBJECTS) -o $@

driver: $(LIB) src/driver.o
	$(CC) $(CFLAGS) -L. src/driver.o -lcomm -o $@

commtest: $(LIB) src/commtest.o
	$(CC) $(CFLAGS) -L. src/commtest.o -lcomm -o $@

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	-rm driver
	-rm src/driver.o
	-rm $(LIB)
	-rm $(OBJECTS)
