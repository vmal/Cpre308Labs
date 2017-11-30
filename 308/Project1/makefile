CC=gcc
CFLAGS=-I.
DEPS = 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: shell.o 
	$(CC) -o shell shell.o  -I.

run: shell
	shell
