# Makefile for FMA-only AVX test
CC ?= gcc
CFLAGS_ASM ?= -O2 -mavx -Wall -Wextra -masm=intel
CFLAGS_C   ?= -O2 -mno-avx -mno-sse2avx -Wall -Wextra -fno-tree-vectorize -fno-builtin
LDFLAGS ?= -lm
OBJDUMP ?= objdump

SRC_ASM := avx_fma.S
SRC_C   := harness_fma.c
OBJ     := $(SRC_ASM:.S=.o) $(SRC_C:.c=.o)
BIN     := avx_fma_test

.PHONY: all run clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.S
	$(CC) $(CFLAGS_ASM) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS_C) -c -o $@ $<

run: $(BIN)
	./$(BIN)

# Disassembly helpers (Intel syntax)
disasm-bin: $(BIN)
	$(OBJDUMP) -d -M intel $(BIN) 

disasm-obj: $(OBJ)
	$(OBJDUMP) -d -M intel avx_fma.o\

clean:
	$(RM) -f $(OBJ) $(BIN)
