# Build an x86-64 Linux binary using GCC with AVX/AVX2
# Output: ../bin/x86/linux/avx-mini

OUTDIR := ../bin/x86/linux
OUT := $(OUTDIR)/avx-mini
SRC := avx_mini.c
CFLAGS := -O0 -Wall -Wextra -std=c11 -mavx -mavx2

.PHONY: all clean dirs

all: dirs $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

dirs:
	@mkdir -p $(OUTDIR)

clean:
	rm -f $(OUT)
