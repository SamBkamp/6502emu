FLAGS := -Wall -Werror
FILES := opcodes.c

all: build

build: main6502.c
	cc main6502.c ${FILES} -o s6C502
dev: main6502.c
	cc main6502.c ${FILES} ${FLAGS} -o s6C502
