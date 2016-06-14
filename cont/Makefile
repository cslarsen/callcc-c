TARGETS := aux.o cont

CFLAGS := -W -Wall -g -O0
NASM := nasm
NASMFLAGS := -fmacho64

run: cont
	./cont

cont: aux.o cont.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o$@

%.o: %.asm
	$(NASM) $(NASMFLAGS) $^ -o$@

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
