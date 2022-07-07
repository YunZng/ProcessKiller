CC=gcc

all: sysMngr procKiller child configreader.o

sysMngr: sysMngr.o
configreader.o: configreader.c
procKiller: procKiller.o
child: child.o

clean:
	rm -f *.o sysMngr procKiller child

