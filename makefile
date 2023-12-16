.DEFAULT_GOAL := all

assembler: main.o preAssembler.o
	gcc *.c -Wall -ansi -pedantic -o assembler -lm
main.o: main.c preAssembler.h
	gcc main.c -Wall -ansi -pedantic -c
preAssembler.o: preAssembler.c preAssembler.h
	gcc preAssembler.c -Wall -ansi -pedantic -c

clean:
	rm -f *.o

.PHONY: all
all: assembler clean