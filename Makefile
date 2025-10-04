FLAGS := -Wall -Werror
FILES := opcodes.c

all: s65C02

ROM.o: modules/ROM.c
	cc -c modules/ROM.c ${FLAGS} -o ROM.o
prot.o: prot.c
	cc -c prot.c ${FLAGS} -o prot.o
main.o: main6502.c
	cc -c main6502.c ${FLAGS} -o main.o
opcodes.o: opcodes.c
	cc -c opcodes.c ${FLAGS} -o opcodes.o

s65C02: main.o opcodes.o prot.o ROM.o
	cc main.o opcodes.o prot.o ROM.o -o s65C02

clean:
	rm s65C02 main.o opcodes.o prot.o ROM.o
