NASM = nasm

ASMFLAGS += -g -f elf32

CFLAGS ?= -O2 -g

CFLAGS += -std=gnu99

CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wabsolute-value

CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

CC += -m32 -no-pie -fno-pie

LDLIBS = -lm

.PHONY: all clean test

all: integral

integral: integral.c funcs.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

funcs.o: funcs.asm
	$(NASM) $(ASMFLAGS) $< -o $@

test: all
	./integral --test-root 4:5:0.5:2:0.0000001:1.105
	./integral --test-root 4:6:0.8:1.3:0.0000001:1.141
	./integral --test-root 5:6:0.6:1.2:0.0000001:1.000000
	./integral --test-integral 4:-1.5:2:0.00001:13.708333
	./integral --test-integral 5:1:2:0.00001:7.833333
	./integral --test-integral 6:0.5:3:0.00001:53.59375

clean:
	rm -rf *.o
