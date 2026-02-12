# AVX Workload Benchmarks

This directory contains four real-world benchmarks that use the implemented AVX-256 instruction subset.

## Implemented AVX Instructions

All workloads use **only** the following verified instructions:

### Floating-Point Arithmetic (Single Precision)
- `VADDPS` - Packed single-precision add
- `VSUBPS` - Packed single-precision subtract
- `VMULPS` - Packed single-precision multiply
- `VDIVPS` - Packed single-precision divide
- `VFMADD231PS` - Fused multiply-add (a*b+c)
- `VXORPS` - Bitwise XOR (for zeroing registers)

### Data Transfer
- `VMOVUPS` - Unaligned packed single-precision move
- `VMOVAPS` - Aligned packed single-precision move
- `VBROADCASTSS` - Broadcast scalar to all lanes
- `VZEROUPPER` - Zero upper 128 bits of YMM registers

### Comparison & Logical
- `VCMPPS` - Packed single-precision compare
- `VANDPS` - Bitwise AND
- `VMINPS` - Minimum of packed floats
- `VMAXPS` - Maximum of packed floats

## Workloads

### 1. Simple Vector Addition (`simple_vadd/`)
**Operation:** `Z = X + Y`

**Instructions Used:**
- `VMOVUPS` - Load/store vectors
- `VADDPS` - Vector addition
- `VZEROUPPER` - Cleanup

**Characteristics:**
- Memory-bound
- Simple SIMD pattern
- 8 floats per iteration (32 bytes)
- Perfect for validating basic AVX functionality

**Build & Run:**
```bash
cd simple_vadd
make
../../../../../../build/X86/gem5.opt --stats-file=stats.txt \
  configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/simple_vadd/vadd_benchmark \
  --options="1024" \
  --cpu-type=AtomicSimpleCPU
```

**Array Sizes:** 64, 256, 1024, 4096, 16384 floats (default: 64)

---

### 2. SAXPY (`saxpy/`)
**Operation:** `Y = a*X + Y` (Single-precision A·X Plus Y)

**Instructions Used:**
- `VBROADCASTSS` - Broadcast scalar `a` to all lanes
- `VMOVUPS` - Load vectors X and Y
- `VFMADD231PS` - Fused multiply-add: Y = a*X + Y
- `VZEROUPPER` - Cleanup

**Characteristics:**
- Compute-bound
- Demonstrates FMA benefits
- Classic BLAS Level 1 operation
- 8 floats per iteration

**Build & Run:**
```bash
cd saxpy
make
../../../../../../build/X86/gem5.opt --stats-file=stats.txt \
  configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/saxpy/saxpy_benchmark \
  --options="8192" \
  --cpu-type=AtomicSimpleCPU
```

**Array Sizes:** Default 8192 floats, configurable via command-line

---

### 3. Matrix Multiplication (`matmul/`)
**Operation:** `C = A × B` (NxN matrices, row-major)

**Instructions Used:**
- `VXORPS` - Zero accumulator
- `VBROADCASTSS` - Broadcast A[i][k] to all lanes
- `VMOVUPS` - Load row of B
- `VFMADD231PS` - Accumulate products
- `VZEROUPPER` - Cleanup

**Characteristics:**
- Compute-intensive (O(N³) operations)
- Cache-aware access patterns
- BLAS Level 3 operation
- Processes 8 columns of B simultaneously

**Build & Run:**
```bash
cd matmul
make
../../../../../../build/X86/gem5.opt --stats-file=stats.txt \
  configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/matmul/matmul_benchmark \
  --options="128" \
  --cpu-type=AtomicSimpleCPU
```

**Matrix Sizes:** 64x64, 128x128, 256x256 (default: 128)
**Note:** Use smaller sizes for gem5 (128 or less recommended)

---

### 4. Image Blur (`image_blur/`)
**Operation:** 3x3 Gaussian blur convolution

**Gaussian Kernel:**
```
1/16 * [ 1  2  1 ]
       [ 2  4  2 ]
       [ 1  2  1 ]
```

**Instructions Used:**
- `VMOVD` + `VBROADCASTSS` - Load/broadcast kernel weights
- `VMOVUPS` - Load pixel rows
- `VMULPS` - Multiply pixels by kernel weights
- `VADDPS` - Accumulate weighted sums
- `VZEROUPPER` - Cleanup

**Characteristics:**
- Memory-bound with moderate compute
- 2D convolution pattern
- Typical image processing workload
- Processes 8 pixels per iteration

**Build & Run:**
```bash
cd image_blur
make
../../../../../../build/X86/gem5.opt --stats-file=stats.txt \
  configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/image_blur/image_blur_benchmark \
  --options="256 256" \
  --cpu-type=AtomicSimpleCPU
```

**Image Sizes:** 128x128, 256x256, 512x512 pixels (default: 256x256)

---

## Compilation Details

### C Code Compilation Flags
All C harness code is compiled with AVX **disabled** to prevent the compiler from generating unimplemented AVX instructions:

```makefile
CFLAGS_NOAVX := -mno-avx -mno-avx2 -mno-fma -mno-sse2avx -msse2
```

This ensures:
- No automatic vectorization with AVX
- No AVX instructions in C library code
- Only explicitly written assembly uses AVX
- SSE2 allowed (baseline x86-64 requirement)

### Assembly Compilation Flags
Assembly kernels enable AVX:

```makefile
CFLAGS_ASM := -mavx -mavx2 -mfma
```

### Static Linking
All binaries use `-static` to avoid dynamic loader dependencies in gem5.

---

## Verification

Each workload includes:
1. **C Reference Implementation** - For correctness verification
2. **AVX Assembly Kernel** - Optimized implementation
3. **Result Comparison** - Epsilon-based floating-point comparison
4. **Sample Output** - Shows first few results

All workloads output:
- `PASSED: All results match!` - Success
- `FAILED: X mismatches found` - Failure with details

---

## Performance Metrics

All workloads report simulation statistics:
- `simTicks` - Total simulation ticks
- `simSeconds` - Simulated time
- Workload-specific metrics (FLOP/tick, throughput, etc.)

Use `--stats-file=filename.txt` to capture statistics for analysis.

---

## CPU Model Compatibility

### ✅ AtomicSimpleCPU
**Status:** Fully working
- All workloads run successfully
- Uses atomic memory model
- No packet-based memory transactions
- Recommended for initial testing

### ⚠️ TimingSimpleCPU / O3CPU / MinorCPU
**Status:** Known issue with 256-bit loads/stores
- Packet size assertion failure in memory system
- Issue: `ldfp256` creating 8-byte packets instead of 32-byte
- Root cause: Memory size parameter not propagating correctly
- **Use AtomicSimpleCPU for now**

---

## Quick Start - Run All Benchmarks

```bash
# From project root
cd tests/test-progs/workloads

# Build all workloads
for dir in simple_vadd saxpy matmul image_blur; do
    (cd $dir && make clean && make)
done

# Run simple vector addition
./build/X86/gem5.opt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/simple_vadd/vadd_benchmark \
  --options="1024" --cpu-type=AtomicSimpleCPU

# Run SAXPY
./build/X86/gem5.opt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/saxpy/saxpy_benchmark \
  --options="8192" --cpu-type=AtomicSimpleCPU

# Run matrix multiplication (small size for gem5)
./build/X86/gem5.opt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/matmul/matmul_benchmark \
  --options="64" --cpu-type=AtomicSimpleCPU

# Run image blur
./build/X86/gem5.opt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/image_blur/image_blur_benchmark \
  --options="256 256" --cpu-type=AtomicSimpleCPU
```

---

## Assembly Syntax

All assembly files use **Intel syntax**:
```assembly
.intel_syntax noprefix
```

- Destination first: `vmovups ymm0, [rdi]`
- No `%` prefix on registers: `mov rax, rbx`
- Memory: `YMMWORD PTR [base + offset]`

---

## Troubleshooting

### Build Errors
- Ensure gcc supports `-mavx2` (gcc 4.7+)
- Check that assembler supports AVX instructions
- Verify all source files present

### Runtime Errors
- Use `AtomicSimpleCPU` until timing CPU issue resolved
- Check array sizes are multiples of 8 for alignment
- Verify gem5 build includes AVX instruction implementations

### Verification Failures
- Small FP differences are normal (epsilon = 1e-5 to 1e-3)
- Check for NaN or Inf values in output
- Verify input data initialization

---

## File Structure

Each workload directory contains:
```
workload_name/
├── Makefile              # Build configuration
├── workload_name.c       # C harness + reference implementation
├── workload_name_kernel.S # AVX assembly kernel
└── workload_name_benchmark # Compiled binary (after make)
```

---

## Future Work

1. **Fix timing CPU support** - Resolve packet size issue for ldfp256/stfp256
2. **Add more workloads** - FFT, reduction operations, etc.
3. **Optimize kernels** - Loop unrolling, prefetching
4. **Add AVX-512 versions** - When AVX-512 support added to gem5
5. **Benchmarking suite** - Automated performance comparison

---

## References

- Intel® 64 and IA-32 Architectures Software Developer's Manual, Volume 2
- Intel® Advanced Vector Extensions Programming Reference
- gem5 Documentation: http://www.gem5.org/documentation/
