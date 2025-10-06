FLAGS := -Fbin -dotdir -o
SOURCE := src
ASSEMBLER := vasm

all: brk.bin dow.bin hello_world.bin jmpret.bin multiplication.bin

%.bin: ${SOURCE}/%.vasm
	echo "\e[32m COMPILING $@ \e[0m"
	${ASSEMBLER} ${FLAGS} $@ $<
	echo "\n"	
