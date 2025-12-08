CC = gcc
AS = as
CFLAGS = -O2 -mavx2 -g -Wall
LDFLAGS = -no-pie

TARGET = avx_int_test
HARNESS = avx_int_harness.c

# Assembly source files
ASM_SRCS = ops/vpaddd_128.S ops/vpaddd_256.S \
           ops/vpandd_128.S ops/vpandd_256.S \
           ops/vpminsd_128.S ops/vpminsd_256.S \
           ops/vpminsq_128.S ops/vpminsq_256.S \
           ops/vpbroadcastd_128.S ops/vpbroadcastd_256.S \
           ops/vmovdqu_128.S ops/vmovdqu_256.S

ASM_OBJS = $(ASM_SRCS:.S=.o)

all: $(TARGET)

$(TARGET): $(HARNESS) $(ASM_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o: %.S
	$(AS) -o $@ $<

clean:
	rm -f $(TARGET) $(ASM_OBJS)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
