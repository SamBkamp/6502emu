all: build

build: main6502.c
	cc main6502.c -o s6C502
dev: main6502.c
	cc -Wall -Werror main6502.c -o s6C502
