CC := gcc
CFLAGS := -Wall -Wextra -O2 -std=c11 -static
# Disable AVX in C code but allow SSE2 (standard on x86-64)
CFLAGS_NOAVX := -mno-avx -mno-avx2 -mno-fma -mno-sse2avx -msse2
# Enable AVX only for assembly
ASMFLAGS := -mavx2 -mfma

TARGET := saxpy_benchmark
SRC := saxpy.c
ASM := saxpy_kernel.S

.PHONY: all clean

all: $(TARGET)

# Compile C code without AVX, then link with AVX assembly
$(TARGET): $(SRC) $(ASM)
	$(CC) $(CFLAGS) $(CFLAGS_NOAVX) $(SRC) -c -o saxpy.o
	$(CC) $(ASMFLAGS) $(ASM) -c -o saxpy_kernel.o
	$(CC) $(CFLAGS) saxpy.o saxpy_kernel.o -o $@

clean:
	rm -f $(TARGET) saxpy.o saxpy_kernel.o
