CC=g++
CFLAGS=-Wall -O3 -funroll-loops -c -std=gnu++0x -I /usr/local/boost_1_47_0
LDFLAGS=-O2 -lm
SOURCES=Bot.cc MyBot.cc Location.cc State.cc 
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=MyBot

#Uncomment the following to enable debugging
#CFLAGS+=-g -DDEBUG

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cc.o: *.h
	$(CC) $(CFLAGS) $< -o $@

clean: 
	-rm -f ${EXECUTABLE} ${OBJECTS} *.d
	-rm -f debug.txt

.PHONY: all clean

