#makefile for lab 5

#compiler to use
CC=gcc

#compiler flags
CFLAGS=-g

#executables
ALL=scheduling

all: $(ALL)

scheduling: scheduling.c
	$(CC) $(CFLAGS) -o scheduling scheduling.c

clean:
	rm -f $(ALL) *.o