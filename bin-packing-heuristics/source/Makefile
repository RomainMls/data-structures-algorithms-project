# Macros
CC = gcc
CFLAGS = -std=c99 --pedantic -Wall -Wextra -Wmissing-prototypes
LDFLAGS = -lm

ALL = bpnaive bpnextfit bpworstfit bpbestfit bpfirstfit

# Object files
%.o: %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c -o $@ $<

# Executables
all: $(ALL)

bpnaive: main.o BP_naive.o Disk.o File.o LinkedList.o
bpnextfit: main.o BP_nextfit.o Disk.o File.o LinkedList.o
bpworstfit: main.o BP_worstfit.o PQ.o Disk.o File.o LinkedList.o
bpbestfit: main.o BP_bestfit.o BST_bestfit.o Disk.o File.o LinkedList.o
bpfirstfit: main.o BP_firstfit.o BST_firstfit.o Disk.o File.o LinkedList.o

$(ALL):
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Phony
.PHONY: all clean run

clean:
	rm -f $(ALL) *.o

run: $(ALL)
	@echo
	@echo "============= Naive ==================="
	@echo
	./bpnaive 1000000 test_20.csv
	@echo
	@echo "============= Next-Fit ================"
	@echo
	./bpnextfit 1000000 test_20.csv
	@echo
	@echo "============= Worst-Fit ==============="
	@echo
	./bpworstfit 1000000 test_20.csv
	@echo
	@echo "============= Best-Fit ================"
	@echo
	./bpbestfit 1000000 test_20.csv
	@echo
	@echo "============= First-Fit ==============="
	@echo
	./bpfirstfit 1000000 test_20.csv

BP_naive.o: BP_naive.c BP.h LinkedList.h File.h Disk.h
BP_nextfit.o: BP_nextfit.c BP.h LinkedList.h File.h Disk.h
BP_worstfit.o: BP_worstfit.c PQ.h BP.h LinkedList.h File.h Disk.h
BP_bestfit.o: BP_firstfit.c BP.h BST_bestfit.h LinkedList.h File.h Disk.h
BP_firstfit.o: BP_firstfit.c BP.h BST_firstfit.h LinkedList.h File.h Disk.h
Disk.o: Disk.c Disk.h LinkedList.h File.h
File.o: File.c File.h
LinkedList.o: LinkedList.c LinkedList.h
PQ.o: PQ.c PQ.h
BST_bestfit.o: BST_bestfit.c BST_bestfit.h
BST_firstfit.o: BST_firstfit.c BST_firstfit.h
main.o: main.c Disk.h LinkedList.h File.h BP.h
