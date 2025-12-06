# AVX Testing Guide for gem5

This document shows how to create and run AVX instruction tests in gem5, following the pattern used by your teammates.

## Quick Start: Running Existing Tests

### 1. Build gem5 (if not already built)
```bash
cd /path/to/ECE752-gem5-avx2
scons build/X86/gem5.opt -j$(nproc)
```

### 2. Compile test binary
```bash
cd tests/test-progs/avx
make clean && make
# Output: avx_tests (static binary with all AVX ops)
```

### 3. Run test in gem5 with debug output
```bash
cd /path/to/ECE752-gem5-avx2
build/X86/gem5.opt \
  --debug-flags=AVXVerbose \
  --debug-file=trace.log \
  configs/deprecated/example/se.py \
  --cmd "$(pwd)/tests/test-progs/avx/avx_tests"
```

**What happens:**
- gem5 launches `avx_tests` in simulated x86 CPU
- Prints PASS/FAIL for each instruction test to stdout
- AVX execution traces written to `trace.log`

---

## Anatomy of a Test (Existing Example)

### Directory Structure
```
tests/test-progs/avx/
├── Makefile           # Build rules
├── avx_harness.c      # C test driver (calls asm routines, validates results)
├── README.md          # Documentation
└── ops/
    ├── vaddf_128.S    # Assembly: VADDPS 128-bit
    ├── vaddf_256.S    # Assembly: VADDPS 256-bit
    ├── vmulf_128.S    # Assembly: VMULPS 128-bit
    ├── vmulf_256.S    # Assembly: VMULPS 256-bit
    └── ... (more ops)
```

### Assembly File Pattern (e.g., `ops/vaddf_128.S`)
```asm
    .text
    .globl avx_vaddf_128
avx_vaddf_128:
    # rdi = pointer to first input array (a)
    # rsi = pointer to second input array (b)
    # rdx = pointer to output array (out)
    
    vmovups  (%rdi), %xmm0       # load a[0:3] into xmm0
    vmovups  (%rsi), %xmm1       # load b[0:3] into xmm1
    vaddps   %xmm1, %xmm0, %xmm0 # add in-place: xmm0 = a + b
    vmovups  %xmm0, (%rdx)       # store result to out
    ret
```

Key points:
- **128-bit ops**: use `%xmm*` registers (4 x 32-bit floats)
- **256-bit ops**: use `%ymm*` registers (8 x 32-bit floats)
- **Calling convention**: System V AMD64
  - `rdi` = 1st arg (input a)
  - `rsi` = 2nd arg (input b)
  - `rdx` = 3rd arg (output)

### C Harness Pattern (avx_harness.c snippet)
```c
#include <stdio.h>
#include <math.h>

// Forward declare asm routine
extern void avx_vaddf_128(const float *a, const float *b, float *out);

// Helper: check if lane values match
static int check_vec_f32(const float *got, const float *exp, int lanes)
{
    for (int i = 0; i < lanes; i++) {
        float diff = got[i] - exp[i];
        if (diff < 0) diff = -diff;
        if (diff > 1e-6f) {
            printf("FAIL lane %d: got=%g exp=%g\n", i, got[i], exp[i]);
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    float a[4] = {1.0f, -2.5f, 3.0f, 0.0f};
    float b[4] = {2.0f, 4.5f, -1.0f, 7.0f};
    float out[4];
    
    // Compute expected result
    float exp[4];
    for (int i = 0; i < 4; i++) exp[i] = a[i] + b[i];
    
    // Call assembly routine
    avx_vaddf_128(a, b, out);
    
    // Validate
    if (!check_vec_f32(out, exp, 4)) {
        printf("FAIL vaddf_128\n");
        return 1;
    }
    printf("PASS vaddf_128\n");
    return 0;
}
```

### Makefile Pattern
```makefile
CC := gcc
CFLAGS_ASM := -mavx -mavx2 -mfma
CFLAGS_NOAVX := -march=x86-64 -mno-avx -fno-tree-vectorize

ASM := ops/vaddf_128.S ops/vaddf_256.S ...
OBJ := $(ASM:.S=.o) avx_harness.o
BIN := avx_tests

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $^

%.o: %.S
	$(CC) $(CFLAGS_ASM) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS_NOAVX) -c -o $@ $<

clean:
	rm -f $(OBJ) $(BIN)
```

---

## How to Create a New Test

### Scenario: Adding VMINPS/VMAXPS tests

#### Step 1: Create Assembly Files

**`tests/test-progs/avx/ops/vminf_128.S`**
```asm
    .text
    .globl avx_vminf_128
avx_vminf_128:
    vmovups  (%rdi), %xmm0       # xmm0 = a[0:3]
    vmovups  (%rsi), %xmm1       # xmm1 = b[0:3]
    vminps   %xmm1, %xmm0, %xmm0 # xmm0 = min(a,b)
    vmovups  %xmm0, (%rdx)       # store
    ret
```

**`tests/test-progs/avx/ops/vminf_256.S`**
```asm
    .text
    .globl avx_vminf_256
avx_vminf_256:
    vmovups  (%rdi), %ymm0
    vmovups  (%rsi), %ymm1
    vminps   %ymm1, %ymm0, %ymm0
    vmovups  %ymm0, (%rdx)
    ret
```

**`tests/test-progs/avx/ops/vmaxf_128.S`**
```asm
    .text
    .globl avx_vmaxf_128
avx_vmaxf_128:
    vmovups  (%rdi), %xmm0
    vmovups  (%rsi), %xmm1
    vmaxps   %xmm1, %xmm0, %xmm0
    vmovups  %xmm0, (%rdx)
    ret
```

**`tests/test-progs/avx/ops/vmaxf_256.S`**
```asm
    .text
    .globl avx_vmaxf_256
avx_vmaxf_256:
    vmovups  (%rdi), %ymm0
    vmovups  (%rsi), %ymm1
    vmaxps   %ymm1, %ymm0, %ymm0
    vmovups  %ymm0, (%rdx)
    ret
```

#### Step 2: Update Makefile

In `tests/test-progs/avx/Makefile`, add to the `ASM` variable:
```makefile
ASM := ops/vaddf_128.S ops/vaddf_256.S \
       ... other ops ...
       ops/vminf_128.S ops/vminf_256.S \
       ops/vmaxf_128.S ops/vmaxf_256.S
```

#### Step 3: Update Harness (avx_harness.c)

Add prototypes:
```c
extern void avx_vminf_128(const float *a, const float *b, float *out);
extern void avx_vminf_256(const float *a, const float *b, float *out);
extern void avx_vmaxf_128(const float *a, const float *b, float *out);
extern void avx_vmaxf_256(const float *a, const float *b, float *out);
```

In `main()`, in the 128-bit test section, add:
```c
        // Min/Max 128-bit
        float exp_min[4], exp_max[4];
        for (int i = 0; i < 4; i++) {
            exp_min[i] = fminf(a[i], b[i]);
            exp_max[i] = fmaxf(a[i], b[i]);
        }
        avx_vminf_128(a, b, out);
        if (!check_vec_f32(out, exp_min, 4)) pass = 0;
        else printf("PASS vminf_128\n");
        
        avx_vmaxf_128(a, b, out);
        if (!check_vec_f32(out, exp_max, 4)) pass = 0;
        else printf("PASS vmaxf_128\n");
```

In the 256-bit test section, add analogous code for 8 lanes using `exp_min[8]`, `exp_max[8]`, etc.

#### Step 4: Build and Run
```bash
cd tests/test-progs/avx
make clean && make

# Run in gem5
cd ../..
build/X86/gem5.opt \
  --debug-flags=AVXVerbose \
  --debug-file=trace.log \
  configs/deprecated/example/se.py \
  --cmd "$(pwd)/tests/test-progs/avx/avx_tests"
```

Expected output (on success):
```
...
PASS vminf_128
PASS vminf_256
PASS vmaxf_128
PASS vmaxf_256
...
```

---

## Understanding the Debug Output

### Checking `trace.log` (with `--debug-flags=AVXVerbose`)

Each microop execution logs lane-by-lane data:
```
[AVX-TRACE] vminf enter VL=16 size=4
[AVX-PRE] lane=0 pre_s1=0x3f800000 pre_s2=0x40000000
[AVX-PRE] lane=1 pre_s1=0xc0200000 pre_s2=0x40900000
...
[AVX-TRACE] minf chunk=0 raw_s1=0x3f8000003fa00000 raw_s2=0x4000000040900000 result={1.0,1.25}
```

**Key fields:**
- `VL=16` → 128-bit (2×64b chunks), `VL=32` → 256-bit (4×64b chunks)
- `lane=N` → which 64-bit subregister chunk
- `raw_s1/raw_s2` → hex bit patterns before operation
- `result={}` → computed floats post-op

### Understanding Results

If your test fails:
```
FAIL lane 2: got=3.5 exp=2.0
```

Check `trace.log` for lane 2; compare raw bit patterns and operation semantics (min vs max, etc.).

---

## Complete Workflow Summary

1. **Write assembly**: One `.S` file per instruction variant (128/256-bit)
2. **Write C test**: Prototype + asm call + expected vs actual check
3. **Add to Makefile**: Include new `.S` in `ASM` variable
4. **Build locally**: `make` in test directory
5. **Build gem5**: `scons build/X86/gem5.opt` (if needed)
6. **Run in gem5**: Use `se.py` config with `--cmd` pointing to your binary
7. **Inspect traces**: Look at `trace.log` if AVXVerbose is enabled

---

## Common Pitfalls

| Issue | Solution |
|-------|----------|
| Compiler emits unexpected VEX opcodes | Use `-mno-avx` on C harness to avoid auto-vectorization |
| Wrong register size | Check calling convention: `rdi`=a, `rsi`=b, `rdx`=out; use `%xmm`/`%ymm` correctly |
| Values don't match | Allow epsilon for float ops (1e-6f); check bit patterns in trace.log |
| Test doesn't run | Ensure static build (`-static` flag if needed) so gem5 can execute it |
| gem5 crashes on unknown instruction | Verify decoder has entry for your opcode; rebuild gem5 |

---

## Running Multiple Tests

To run both `avx_tests` and `avx_subset_test`:

```bash
# Terminal 1: avx_tests
build/X86/gem5.opt configs/deprecated/example/se.py \
  --cmd "$(pwd)/tests/test-progs/avx/avx_tests"

# Terminal 2: avx_subset_test
build/X86/gem5.opt configs/deprecated/example/se.py \
  --cmd "$(pwd)/tests/test-progs/avx_subset/avx_subset_test"
```

---

## Next Steps

Now that you understand the test structure:
1. Create your min/max assembly files (see "How to Create a New Test" above)
2. Update the harness to test them
3. Build and run locally first (`make`)
4. Then run in gem5 using the command above
5. Check `trace.log` for correctness

