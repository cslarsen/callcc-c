TARGETS := aux.o cont

CFLAGS := -W -Wall -g -O0
NASM := nasm

OS := $(shell uname)
ifeq ($(OS),Linux)
	NASMFLAGS := -felf64
else
	NASMFLAGS := -fmacho64
endif

run: cont
	./cont

cont: aux.o cont.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o$@

%.o: %.asm
	$(NASM) $(NASMFLAGS) $^ -o$@

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
