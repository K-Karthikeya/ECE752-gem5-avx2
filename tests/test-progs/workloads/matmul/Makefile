CC := gcc
CFLAGS := -Wall -Wextra -O2 -std=c11
CFLAGS_NOAVX := -mno-avx -mno-avx2 -mno-fma -mno-sse2avx
CFLAGS_ASM := -mavx -mavx2 -mfma

TARGET := matmul_benchmark
SRC := matmul.c
ASM := matmul_kernel.S
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
