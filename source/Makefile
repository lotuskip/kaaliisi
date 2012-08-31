CXX=clang++
RM=rm -f
CPPFLAGS=-O2 -Wall -Wextra -Wno-char-subscripts -pedantic
LDLIBS=-lncurses

SRCS=coords.cpp figure.cpp game.cpp io.cpp kaaliisi.cpp map.cpp monster.cpp rng.cpp stuff.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: kaaliisi

kaaliisi: $(OBJS)
	$(CXX) -o ../kaaliisi $(CPPFLAGS) $(OBJS) $(LDLIBS) 

coords.o: coords.cpp
figure.o: figure.cpp
game.o: game.cpp
io.o: io.cpp
kaaliisi.o: kaaliisi.cpp
map.o: map.cpp
monster.o: monster.cpp
rng.o: rng.cpp
stuff.o: stuff.cpp

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) ../kaaliisi

