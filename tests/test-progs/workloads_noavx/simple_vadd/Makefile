CC = gcc
AS = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -static -mno-avx -msse2
ASMFLAGS = -msse2 -mno-avx

# gem5 m5ops configuration
GEM5_PATH = ../../../..
M5OPS_INCLUDE = $(GEM5_PATH)/include
M5OPS_LIB = $(GEM5_PATH)/util/m5/build/x86/out

CFLAGS += -I$(M5OPS_INCLUDE)
LDFLAGS = -L$(M5OPS_LIB) -lm5

TARGET = vadd_benchmark

all: $(TARGET)

$(TARGET): vadd.o vadd_kernel.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

vadd.o: vadd.c
	$(CC) $(CFLAGS) -c -o $@ $<

vadd_kernel.o: vadd_kernel.S
	$(AS) $(ASMFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
