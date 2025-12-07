#===========================================================================
# Makefile for AVX Instruction Trace Test
#===========================================================================

CC = gcc
CFLAGS = -mavx2 -O2 -Wall -Wextra -g
LDFLAGS = -lm

TARGET = avx_trace_test
OBJS = avx_trace_test.o avx_trace_harness.o

.PHONY: all clean run disasm check

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

avx_trace_test.o: avx_trace_test.S
	$(CC) $(CFLAGS) -c $< -o $@

avx_trace_harness.o: avx_trace_harness.c
	$(CC) $(CFLAGS) -c $< -o $@

# Disassemble to verify AVX instructions
disasm: $(TARGET)
	@echo "=== Disassembly of AVX instructions ==="
	objdump -d $(TARGET) | grep -A2 -E "vmov|vmin|vmax|vbroadcast|vzero" || true

# Check which AVX instructions are present
check: $(TARGET)
	@echo "=== AVX Instructions Found ==="
	@objdump -d $(TARGET) | grep -Ei "\b(vmovups|vmovaps|vminps|vmaxps|vmovsldup|vmovshdup|vbroadcastss|vzeroupper)\b" | wc -l
	@echo "Detailed breakdown:"
	@objdump -d $(TARGET) | grep -o -E "vmovups|vmovaps|vminps|vmaxps|vmovsldup|vmovshdup|vbroadcastss|vzeroupper" | sort | uniq -c || true

# Run natively (if host supports AVX)
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

# Help
help:
	@echo "Available targets:"
	@echo "  all     - Build the test binary (default)"
	@echo "  disasm  - Show disassembly of AVX instructions"
	@echo "  check   - Count and list AVX instructions in binary"
	@echo "  run     - Run test natively (requires AVX support)"
	@echo "  clean   - Remove build artifacts"
