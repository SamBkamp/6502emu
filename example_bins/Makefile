FLAGS := -Fbin -dotdir -o
SOURCE := src
ASSEMBLER := vasm

all: dow example jmpret brk

dow:
	echo "\e[32m COMPILING dow.vasm \e[0m"
	${ASSEMBLER} ${FLAGS} dow.bin ${SOURCE}/dow.vasm
	echo "\n"	
example:
	echo "\e[32m COMPILING example.vasm \e[0m"
	${ASSEMBLER} ${FLAGS} example.bin ${SOURCE}/example.vasm
	echo "\n"
jmpret:
	echo "\e[32m COMPILING jmpret.vasm \e[0m"	
	${ASSEMBLER} ${FLAGS} jmpret.bin ${SOURCE}/jmpret.vasm
	echo "\n"
brk:
	echo "\e[32m COMPILING brk.vasm \e[0m"	
	${ASSEMBLER} ${FLAGS} brk.bin ${SOURCE}/brk.vasm
	echo "\n"
