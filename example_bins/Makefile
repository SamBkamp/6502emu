FLAGS := -Fbin -dotdir -o
SOURCE := src
ASSEMBLER := vasm

all: dow.bin multiplication.bin jmpret.bin brk.bin

dow.bin: ${SOURCE}/dow.vasm
	echo "\e[32m COMPILING dow.vasm \e[0m"
	${ASSEMBLER} ${FLAGS} dow.bin ${SOURCE}/dow.vasm
	echo "\n"	
multiplication.bin: ${SOURCE}/multiplication.vasm
	echo "\e[32m COMPILING example.vasm \e[0m"
	${ASSEMBLER} ${FLAGS} multiplication.bin ${SOURCE}/multiplication.vasm
	echo "\n"
jmpret.bin: ${SOURCE}/jmpret.vasm
	echo "\e[32m COMPILING jmpret.vasm \e[0m"	
	${ASSEMBLER} ${FLAGS} jmpret.bin ${SOURCE}/jmpret.vasm
	echo "\n"
brk.bin: ${SOURCE}/brk.vasm
	echo "\e[32m COMPILING brk.vasm \e[0m"	
	${ASSEMBLER} ${FLAGS} brk.bin ${SOURCE}/brk.vasm
	echo "\n"
