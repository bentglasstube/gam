SOURCES=$(wildcard *.cc)
OBJECTS=$(patsubst %.cc,%.o,$(SOURCES))
LIBRARY=libgam.a

CC=g++
CFLAGS=-O3 --std=c++11 -Wall -Wextra -Werror -pedantic

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

%.o: %.cc
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o $(LIBRARY)

.PHONY: all clean
