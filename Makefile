SOURCES=$(filter-out generate_controllerdb.cc, $(wildcard *.cc)) controllerdb.cc
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

generate_controllerdb: generate_controllerdb.cc
	$(CC) $(CFLAGS) -o $@ $<

controllerdb.cc: controllerdb/gamecontrollerdb.txt generate_controllerdb
	./generate_controllerdb $< > $@

%.o: %.cc
	$(CC) -c $(CFLAGS) -o $@ $<

audio.o: audio.cc audio.h
backdrop.o: backdrop.cc backdrop.h graphics.o
box.o: box.cc box.h graphics.o
client.o: client.cc client.h net.o
controllerdb.o: controllerdb.cc controllerdb.h
game.o: game.cc game.h audio.o graphics.o input.o screen.o
graphics.o: graphics.cc graphics.h
input.o: input.cc input.h controllerdb.o util.o
menu.o: menu.cc menu.h graphics.o text.o
net.o: net.cc net.h
parallax_backdrop.o: parallax_backdrop.cc parallax_backdrop.h graphics.o
particle.o: particle.cc particle.h graphics.o
rect.o: rect.cc rect.h graphics.o
screen.o: screen.cc screen.h audio.o graphics.o input.o
server.o: server.cc server.h net.h
sprite.o: sprite.cc sprite.h graphics.o
spritemap.o: spritemap.cc spritemap.h graphics.o
text.o: text.cc text.h graphics.o
util.o: util.cc util.h

clean:
	$(RM) controllerdb.cc generate_controllerdb $(OBJECTS)

dist-clean:
	$(RM) $(SNAME) $(DNAME) $(LIBRARY)-v*.tgz
	$(RM) -rf $(LIBRARY)-v*/

.PHONY: all clean dist-clean
