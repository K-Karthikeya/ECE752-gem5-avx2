# Simple Makefile to build AVX subset test
# Targets:
#   make            -> build avx_subset_test
#   make run        -> run executable
#   make clean      -> remove artifacts

CC ?= gcc
# Disable auto-vectorization and builtins so the compiler does not emit
# unsupported AVX instructions (e.g., VBROADCASTSS) beyond our minimal subset.
# You can raise O2->O0 if you want even stricter avoidance, but -fno-tree-vectorize
# is typically sufficient. Retain -mavx to allow our explicit AVX mnemonics.
CFLAGS ?= -O2 -mavx -Wall -Wextra -fno-tree-vectorize -fno-builtin
LDFLAGS ?= -mavx

SRC_ASM := avx_subset.S
SRC_C   := harness.c
OBJ     := $(SRC_ASM:.S=.o) $(SRC_C:.c=.o)
BIN     := avx_subset_test

.PHONY: all run clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(BIN)
	./$(BIN)

disasm: $(BIN)
	objdump -d $(BIN) | grep -E "0f(10|11|28|29|57|58|59|77)" || true
	@echo "(If other AVX/VEX opcodes appear above, adjust CFLAGS to suppress them.)"

clean:
	rm -f $(OBJ) $(BIN)
