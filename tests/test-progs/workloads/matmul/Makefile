CC := gcc
CFLAGS := -Wall -Wextra -O2 -std=c11 -static
CFLAGS_NOAVX := -mno-avx -mno-avx2 -mno-fma -mno-sse2avx
CFLAGS_ASM := -mavx -mavx2 -mfma

# gem5 m5ops configuration
GEM5_PATH := ../../../..
M5OPS_INCLUDE := $(GEM5_PATH)/include
M5OPS_LIB := $(GEM5_PATH)/util/m5/build/x86/out

CFLAGS += -I$(M5OPS_INCLUDE)
LDFLAGS := -L$(M5OPS_LIB) -lm5

TARGET := matmul_benchmark
SRC := matmul.c
ASM := matmul_kernel.S
OBJ := $(SRC:.c=.o) $(ASM:.S=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS_NOAVX) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS_ASM) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)
