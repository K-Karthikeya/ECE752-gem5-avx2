# Simple Makefile to build AVX subset test
# Targets:
#   make            -> build avx_subset_test
#   make run        -> run executable
#   make clean      -> remove artifacts

CC ?= gcc
# Separate flags for ASM vs C so the harness does NOT use AVX (prevents VEX scalar v* ops).
# - ASM: keep -mavx so the explicit AVX mnemonics assemble as VEX.
# - C:   use -mno-avx and disable vectorization/builtins to avoid emitting unsupported AVX.
CFLAGS_ASM ?= -O2 -mavx -Wall -Wextra
CFLAGS_C   ?= -O2 -mno-avx -mno-sse2avx -Wall -Wextra -fno-tree-vectorize -fno-builtin
LDFLAGS ?= 

SRC_ASM := avx_subset.S
SRC_C   := harness.c
OBJ     := $(SRC_ASM:.S=.o) $(SRC_C:.c=.o)
BIN     := avx_subset_test

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

disasm: $(BIN)
	@echo "== Expected AVX mnemonics present (by name) =="
	objdump -d $(BIN) | grep -Ei "\b(vmovups|vmovaps|vaddps|vmulps|vxorps|vzeroupper)\b" || true
	@echo "\n== Any other V* vector mnemonics (should be empty) =="
	objdump -d $(BIN) | grep -Ei "\bv[a-z0-9_]+\b" | \
	  grep -Evi "\b(vmovups|vmovaps|vaddps|vmulps|vxorps|vzeroupper)\b" || true
	@echo "\n(If lines appear above, the compiler emitted extra AVX/VEX instructions.\nAdjust CFLAGS or lower optimization.)"

clean:
	rm -f $(OBJ) $(BIN)
