# Macros
CC = gcc
CFLAGS = -std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes
LDFLAGS = -lm

ALL = selectionselect heapselect quickselect frselect

# Object files
%.o: %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c -o $@ $<

# Executables
all: $(ALL)

selectionselect: main.o IntArray.o SelectionSelect.o 
heapselect: main.o IntArray.o HeapSelect.o
quickselect: main.o IntArray.o QuickSelect.o
frselect: main.o IntArray.o FRSelect.o

$(ALL):
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Phony
.PHONY: all clean run

clean:
	rm -f $(ALL) *.o

run: $(ALL)
	./selectionselect
	./heapselect
	./quickselect
	./frselect

FRSelect.o: FRSelect.c Select.h
HeapSelect.o: HeapSelect.c Select.h
IntArray.o: IntArray.c IntArray.h
QuickSelect.o: QuickSelect.c Select.h
SelectionSelect.o: SelectionSelect.c Select.h
main.o: main.c IntArray.h Select.h
