.DEFAULT_GOAL := all

assembler: main.o firstAssembler.o secAssembler.o nodes.o
	gcc *.c -Wall -ansi -pedantic -o assembler -lm
main.o: main.c genericH.h
	gcc main.c -Wall -ansi -pedantic -c
# preAssembler.o: preAssembler.c genericH.h
# 	gcc preAssembler.c -Wall -ansi -pedantic -c
firstAssembler.o: firstAssembler.c genericH.h
	gcc firstAssembler.c -Wall -ansi -pedantic -c
secAssembler.o: secAssembler.c genericH.h
	gcc secAssembler.c -Wall -ansi -pedantic -c
nodes.o: nodes.c genericH.h
	gcc nodes.c -Wall -ansi -pedantic -c


clean:
	rm -f *.o

.PHONY: all
all: assembler clean