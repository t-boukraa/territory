CC=gcc
EXEC=goTerritory
OPT=-O2
CFLAGS=-Wall -std=c99 -o

all: $(EXEC)

$(EXEC): goTerritory.o play.o strategy.o
	$(CC) $(OPT) $(CFLAGS) $@ $^ -lm
	
goTerritory.o: goTerritory.c
	$(CC) $(OPT) -c -g $(CFLAGS) $@ $<

play.o: play.c play.h
	$(CC) $(OPT) -c -g $(CFLAGS) $@ $<
	
strategy.o: strategy.c strategy.h play.h
	$(CC) $(OPT) -c -g $(CFLAGS) $@ $<

clean:
	rm -r -f *.o

mrproper: clean
	rm -f $(EXEC)
