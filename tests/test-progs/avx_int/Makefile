CC = gcc
AS = as
CFLAGS = -O2 -mno-avx -g -Wall
LDFLAGS = -no-pie

TARGET = avx_int_test
HARNESS = avx_int_harness.c

# Assembly source files
ASM_SRCS = ops/vpaddd_128.S ops/vpaddd_256.S \
           ops/vpandd_128.S ops/vpandd_256.S \
           ops/vpminsd_128.S ops/vpminsd_256.S \
           ops/vpbroadcastd_128.S ops/vpbroadcastd_256.S \
           ops/vmovdqu_128.S ops/vmovdqu_256.S
# VPMINSQ is AVX-512 only, not included: ops/vpminsq_128.S ops/vpminsq_256.S

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

disasm: $(TARGET)
	objdump -d --show-raw-insn $(TARGET) | grep -A 5 "avx_vpaddd_128\|avx_vpandd_128\|avx_vpminsd_128\|avx_vpminsq_128\|avx_vpbroadcastd_128\|avx_vmovdqu_128"

disasm-all: $(TARGET)
	objdump -d $(TARGET) > $(TARGET).disasm
	@echo "Disassembly written to $(TARGET).disasm"

.PHONY: all clean run disasm disasm-all
