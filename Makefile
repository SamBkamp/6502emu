FLAGS := -Wall -Werror
FILES := opcodes.c

all: build

build:
	cc main6502.c ${FILES} -o s65C02

dev: main6502.c ${FILES}
	cc main6502.c ${FILES} ${FLAGS} -o s65C02

clean:
	rm s65C02
