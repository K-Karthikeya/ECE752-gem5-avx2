CC = gcc
AS = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -static -mno-avx -mno-avx2 -msse2
ASMFLAGS = -msse2 -mno-avx

TARGET = vadd_benchmark

all: $(TARGET)

$(TARGET): vadd.o vadd_kernel.o
	$(CC) $(CFLAGS) -o $@ $^

vadd.o: vadd.c
	$(CC) $(CFLAGS) -c -o $@ $<

vadd_kernel.o: vadd_kernel.S
	$(AS) $(ASMFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
