# Build an x86-64 Linux binary using GCC with AVX2
# Output: ../bin/x86/linux/avx2-smoke

CFLAGS := -O0 -Wall -Wextra -std=c11 -mavx -mavx2
LDFLAGS :=
CC := gcc
OUTDIR := ../bin/x86/linux
OUT := $(OUTDIR)/avx2-smoke
SRC := avx2_smoke.c

all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(OUT)

.PHONY: all clean
