CC := gcc
CFLAGS := -Wall -Wextra -O2 -std=c11 -static
# Disable AVX in C code but allow SSE2 (standard on x86-64)
CFLAGS_NOAVX := -mno-avx -mno-avx2 -mno-fma -mno-sse2avx -msse2
# Enable SSE only for assembly (no AVX)
ASMFLAGS := -msse2 -mno-avx

# gem5 m5ops configuration
GEM5_PATH := ../../../..
M5OPS_INCLUDE := $(GEM5_PATH)/include
M5OPS_LIB := $(GEM5_PATH)/util/m5/build/x86/out

CFLAGS += -I$(M5OPS_INCLUDE)
LDFLAGS := -L$(M5OPS_LIB) -lm5

TARGET := saxpy_benchmark
SRC := saxpy.c
ASM := saxpy_kernel.S

.PHONY: all clean

all: $(TARGET)

# Compile C code without AVX, then link with SSE assembly
$(TARGET): $(SRC) $(ASM)
	$(CC) $(CFLAGS) $(CFLAGS_NOAVX) $(SRC) -c -o saxpy.o
	$(CC) $(ASMFLAGS) $(ASM) -c -o saxpy_kernel.o
	$(CC) $(CFLAGS) saxpy.o saxpy_kernel.o -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET) saxpy.o saxpy_kernel.o
