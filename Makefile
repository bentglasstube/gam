SOURCES=$(wildcard *.cc)
HEADERS=$(wildcard *.h)
OBJECTS=$(patsubst %.cc,%.o,$(SOURCES))
LIBRARY=libgam
SNAME=$(LIBRARY).a
DNAME=$(LIBRARY).so
VERSION=$(shell git describe)

CC=g++
CFLAGS=-O3 --std=c++11 -Wall -Wextra -Werror -pedantic -fPIC

all: $(SNAME) $(DNAME)

package: $(LIBRARY)-$(VERSION).tgz

$(LIBRARY)-$(VERSION).tgz: $(SNAME) $(DNAME) $(HEADERS)
	mkdir -p $(LIBRARY)-$(VERSION)/{lib,include}
	cp $(HEADERS) $(LIBRARY)-$(VERSION)/include/
	cp $(SNAME) $(DNAME) $(LIBRARY)-$(VERSION)/lib
	tar zcvf $@ $(LIBRARY)-$(VERSION)

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
