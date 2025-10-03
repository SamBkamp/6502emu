FLAGS := -Wall -Werror
FILES := opcodes.c

all: s65C02

main.o:
	cc -c main6502.c ${FLAGS} -o main.o
opcodes.o:
	cc -c opcodes.c ${FLAGS} -o opcodes.o

s65C02: main.o opcodes.o
	cc main.o opcodes.o -o s65C02

clean:
	rm s65C02 main.o opcodes.o
