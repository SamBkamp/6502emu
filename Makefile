FLAGS := -Wall -Werror
FILES := opcodes.c

all: build

build:
	cc main6502.c ${FILES} -o s6C502

dev: main6502.c ${FILES}
	cc main6502.c ${FILES} ${FLAGS} -o s6C502
