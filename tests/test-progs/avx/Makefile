CC := gcc
CFLAGS := -O2 -Wall -Wextra -std=c11 -mavx -mavx2 -mfma
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
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(BIN)
