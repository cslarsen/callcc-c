CFLAGS := -g -O0
TARGETS := callcc

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
