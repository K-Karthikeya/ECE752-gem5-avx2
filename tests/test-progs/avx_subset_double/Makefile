# Simple Makefile to build AVX double subset test
# Targets:
#   make            -> build avx_subset_test
#   make run        -> run executable
#   make clean      -> remove artifacts

CC ?= gcc
CCFLAGS_COMMON ?= -O2 -Wall -Wextra
# Assemble AVX-intended code with AVX enabled; compile harness without AVX to avoid compiler-emitted VEX.
CFLAGS_ASM ?= $(CCFLAGS_COMMON) -mavx
CFLAGS_C   ?= $(CCFLAGS_COMMON) -mno-avx -mno-sse2avx -fno-tree-vectorize -fno-builtin
LDFLAGS ?= 

SRC_ASM := avx_subset.S
SRC_C   := harness.c
OBJ     := $(SRC_ASM:.S=.o) $(SRC_C:.c=.o)
BIN     := avx_subset_double_test

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
		objdump -d $(BIN) | grep -Ei "\b(vmovapd|vaddpd|vsubpd|vmulpd|vdivpd|vmovddup|vzeroupper)\b" || true
		@echo "\n== Any other V* vector mnemonics (should be empty) =="
	objdump -d $(BIN) | grep -Ei "\bv[a-z0-9_]+\b" | \
			grep -Evi "\b(vmovapd|vaddpd|vsubpd|vmulpd|vdivpd|vmovddup|vzeroupper)\b" || true
	@echo "\n(If lines appear above, the compiler emitted extra AVX/VEX instructions.\nAdjust CFLAGS or lower optimization.)"

clean:
	rm -f $(OBJ) $(BIN)
