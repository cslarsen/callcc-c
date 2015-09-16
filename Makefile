CFLAGS := -g -Os -W -Wall
TARGETS := callcc

all: $(TARGETS)

run: all
	./callcc

dis: all
	objdump -d callcc | $$PAGER

clean:
	rm -f $(TARGETS)
