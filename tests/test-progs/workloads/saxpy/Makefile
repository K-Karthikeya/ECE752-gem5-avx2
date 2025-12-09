CC := gcc
CFLAGS := -Wall -Wextra -O2 -std=c11
ASMFLAGS := -mavx2 -mfma

TARGET := saxpy_benchmark
SRC := saxpy.c
ASM := saxpy_kernel.S

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC) $(ASM)
	$(CC) $(CFLAGS) $(ASMFLAGS) -o $@ $(SRC) $(ASM)

clean:
	rm -f $(TARGET)
