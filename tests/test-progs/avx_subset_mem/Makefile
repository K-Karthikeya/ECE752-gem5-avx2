# Makefile for avx_subset_mem test
# Separate flags for ASM and C so the harness does NOT accidentally emit AVX scalars.

CC ?= gcc
# Keep assembly AVX on, but ensure C harness does not emit AVX.
CFLAGS_ASM ?= -O2 -mavx -Wall -Wextra
CFLAGS_C   ?= -O2 -std=c11 -mno-avx -mno-sse2avx -Wall -Wextra -fno-tree-vectorize -fno-builtin
LDFLAGS ?=

SRC_ASM := avx_subset_mem.S
SRC_C   := harness_mem.c
OBJ     := $(SRC_ASM:.S=.o) $(SRC_C:.c=.o)
BIN     := avx_subset_mem_test

.PHONY: all run clean disasm

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.S
	$(CC) $(CFLAGS_ASM) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS_C) -c -o $@ $<

run: $(BIN)
	./$(BIN)

disasm: $(BIN)
	@echo "== AVX mnemonics present (by name) =="
	objdump -d $(BIN) | grep -Ei "\b(vmovups|vmovaps|vaddps|vmulps|vxorps|vzeroupper)\b" || true

clean:
	rm -f $(OBJ) $(BIN)
