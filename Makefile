CFLAGS := -g -O0 -W -Wall -rdynamic -march=native -mtune=native
TARGETS := callcc

all: $(TARGETS)

run: all
	./callcc

clean:
	rm -f $(TARGETS)
