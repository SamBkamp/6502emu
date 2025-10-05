FLAGS := -Wall -Werror -ggdb
FILES := main6502.c opcodes.c prot.c ${MODULES}/ROM.c ${MODULES}/RAM.c
OBJ_FOLDER := obj
OBJECTS := $(foreach var, ${FILES},${OBJ_FOLDER}/$(basename $(notdir ${var})).o)

.PHONY: clean all cleanobj

all: ${OBJ_FOLDER} s65C02

${OBJ_FOLDER}:
	mkdir -p $@
${OBJ_FOLDER}/%.o: modules/%.c
	cc -c  $< ${FLAGS} -o $@
${OBJ_FOLDER}/%.o: %.c
	cc -c  $< ${FLAGS} -o $@

s65C02: ${OBJECTS}
	cc ${OBJECTS} ${FLAGS} -o s65C02

clean:
	rm -rf s65C02 ${OBJ_FOLDER}
cleanobj:
	rm -rf ${OBJ_FOLDER}
