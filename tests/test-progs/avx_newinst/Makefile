# Makefile for AVX new instructions test
CC ?= gcc
CFLAGS_ASM ?= -O2 -mavx -Wall -Wextra
CFLAGS_C   ?= -O2 -mno-avx -mno-sse2avx -Wall -Wextra -fno-tree-vectorize -fno-builtin
LDFLAGS ?= 

SRC_ASM := avx_newinst.S
SRC_C   := harness_newinst.c
OBJ     := $(SRC_ASM:.S=.o) $(SRC_C:.c=.o)
BIN     := avx_newinst_test

.PHONY: all run clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.S
	$(CC) $(CFLAGS_ASM) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS_C) -c -o $@ $<

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(OBJ) $(BIN)
