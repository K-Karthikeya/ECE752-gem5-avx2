CC := gcc
CFLAGS := -Wall -Wextra -O2 -std=c11
# Disable AVX in C code to prevent compiler from generating AVX
CFLAGS_NOAVX := -mno-avx -mno-avx2 -mno-fma -mno-sse2avx
# Allow AVX in assembly
CFLAGS_ASM := -mavx -mavx2 -mfma

TARGET := saxpy_benchmark
SRC := saxpy.c
ASM := saxpy_kernel.S
OBJ := $(SRC:.c=.o) $(ASM:.S=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_NOAVX) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS_ASM) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
