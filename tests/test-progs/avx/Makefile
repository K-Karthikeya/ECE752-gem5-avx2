CC := gcc
# Assembly files need AVX/FMA; C harness must avoid emitting AVX/VEX
# to prevent unsupported VEX ops in gem5 decoder.
CFLAGS := -Wall -Wextra -std=c11
CFLAGS_ASM := -mavx -mavx2 -mfma
# Disable AVX/VEX in C completely - use -O0 to prevent any compiler optimizations
# that might emit VEX-encoded instructions. The C harness is just test scaffolding.
CFLAGS_NOAVX := -march=x86-64 -mno-avx -mno-avx2 -mno-fma -mno-fma4 -mno-avx512f -mno-sse2avx -fno-tree-vectorize
LDFLAGS :=

SRC := avx_harness.c
ASM := ops/vaddf_128.S ops/vaddf_256.S \
       ops/vsubf_128.S ops/vsubf_256.S \
       ops/vmulf_128.S ops/vmulf_256.S \
       ops/vdivf_128.S ops/vdivf_256.S \
       ops/vxorf_128.S ops/vxorf_256.S \
       ops/vandf_128.S ops/vandf_256.S \
       ops/vfmadd231f_128.S ops/vfmadd231f_256.S \
       ops/vcmpf_eq_128.S ops/vcmpf_eq_256.S \
       ops/vclear.S ops/ldstfp_128.S ops/ldstfp_256.S

OBJ := $(SRC:.c=.o) $(ASM:.S=.o)

BIN := avx_tests

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(CFLAGS_ASM) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_NOAVX) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) $(CFLAGS_ASM) -c -o $@ $<

clean:
	rm -f $(OBJ) $(BIN)
