CFLAGS := -g -O0 -W -Wall -rdynamic
TARGETS := callcc

all: $(TARGETS)

run: all
	./callcc

clean:
	rm -f $(TARGETS)
