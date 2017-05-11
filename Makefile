SOURCES=$(wildcard *.cc)
OBJECTS=$(patsubst %.cc,%.o,$(SOURCES))
LIBRARY=libgam
SNAME=$(LIBRARY).a
DNAME=$(LIBRARY).so

CC=g++
CFLAGS=-O3 --std=c++11 -Wall -Wextra -Werror -pedantic -fPIC

all: $(SNAME) $(DNAME)

$(SNAME): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(DNAME): LDFLAGS += -shared
$(DNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^


%.o: %.cc
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJECTS) $(SNAME) $(DNAME)

.PHONY: all clean
