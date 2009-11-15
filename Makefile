# TODO: Use MKDEPEND
#

.PHONY: default all clean

default: all

C99 = gcc -std=c99


rts/vm.c: rts/opcodes.h rts/vm.h rts/limits.h
rts/opcodes.c: rts/opcodes.h rts/limits.h
rts/decode.c: rts/decode.h rts/opcodes.h rts/vm.h
rts/vm.o: rts/vm.c
	$(C99) -c -Wall $< -o $@
rts/opcodes.o: rts/opcodes.c
	$(C99) -c -Wall $< -o $@
rts/decode.o: rts/decode.c
	$(C99) -c -Wall $< -o $@

rts/vm: rts/vm.o rts/opcodes.o rts/decode.o
	$(C99) $^ -o $@

all: rts/vm

run: rts/vm
	./$<

clean:
	rm -f rts/*.o rts/vm