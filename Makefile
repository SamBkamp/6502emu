FLAGS := -Wall -Werror
FILES := opcodes.c

all: s65C02

RAM.o: modules/RAM.c
	cc -c -ggdb modules/RAM.c ${FLAGS} -o RAM.o
ROM.o: modules/ROM.c
	cc -c -ggdb modules/ROM.c ${FLAGS} -o ROM.o
prot.o: prot.c
	cc -c -ggdb prot.c ${FLAGS} -o prot.o
main.o: main6502.c
	cc -c -ggdb main6502.c ${FLAGS} -o main.o
opcodes.o: opcodes.c
	cc -c -ggdb opcodes.c ${FLAGS} -o opcodes.o

s65C02: main.o opcodes.o prot.o ROM.o RAM.o
	cc main.o opcodes.o prot.o ROM.o RAM.o -ggdb -o s65C02

clean:
	rm s65C02 main.o opcodes.o prot.o ROM.o RAM.o
