# SSE Baseline Workloads (No AVX)

This directory contains SSE-based implementations of the same workloads for comparison with AVX versions.

## Purpose

These workloads use **SSE (128-bit SIMD)** instead of AVX (256-bit SIMD) to:
1. Run on mainline gem5 without AVX support
2. Provide baseline performance for comparison
3. Demonstrate AVX speedup over SSE

## Workloads

### 1. Simple Vector Addition (`simple_vadd/`)
**Operation:** `Z = X + Y`

**SSE Implementation:**
- Uses `MOVUPS` (unaligned load), `ADDPS` (packed add)
- Processes **4 floats per iteration** (vs 8 in AVX)
- 16-byte alignment (vs 32-byte in AVX)

**Expected Speedup:** ~2x with AVX (double SIMD width)

---

### 2. SAXPY (`saxpy/`)
**Operation:** `Y = a*X + Y`

**SSE Implementation:**
- Uses `SHUFPS` for scalar broadcast (vs `VBROADCASTSS` in AVX)
- Uses separate `MULPS` + `ADDPS` (vs `VFMADD231PS` in AVX)
- Processes **4 floats per iteration** (vs 8 in AVX)

**Expected Speedup:** ~2-3x with AVX (2x from width, extra from FMA)

---

### 3. Matrix Multiplication (`matmul/`)
**Operation:** `C = A × B` (NxN matrices)

**SSE Implementation:**
- Uses `MOVSS` + `SHUFPS` for broadcast (vs `VBROADCASTSS` in AVX)
- Uses separate `MULPS` + `ADDPS` (vs `VFMADD231PS` in AVX)
- Processes **4 columns per iteration** (vs 8 in AVX)

**Expected Speedup:** ~2-3x with AVX (2x from width, extra from FMA)

---

## Key Differences: SSE vs AVX

| Feature | SSE (this folder) | AVX (../workloads/) |
|---------|------------------|---------------------|
| SIMD Width | 128-bit (4 floats) | 256-bit (8 floats) |
| Registers | XMM0-XMM15 | YMM0-YMM15 |
| Alignment | 16-byte | 32-byte |
| FMA Support | No (mul + add) | Yes (fused) |
| Instruction Prefix | None | VEX prefix |
| Broadcast | `SHUFPS` | `VBROADCASTSS` |

---

## Building

```bash
cd workloads_noavx/simple_vadd
make

cd ../saxpy
make

cd ../matmul
make
```

---

## Running

### On Mainline gem5 (no AVX support)
```bash
./build/X86/gem5.opt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads_noavx/simple_vadd/vadd_benchmark \
  --options="1024" \
  --cpu-type=AtomicSimpleCPU
```

### On AVX-enabled gem5
```bash
./build/X86/gem5.opt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads_noavx/simple_vadd/vadd_benchmark \
  --options="1024" \
  --cpu-type=AtomicSimpleCPU
```

---

## Comparison Methodology

Run the same workloads with the same sizes on both versions:

### SSE Baseline (this folder)
```bash
# Simple vector addition
./build/X86/gem5.opt --stats-file=sse_vadd_1024.txt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads_noavx/simple_vadd/vadd_benchmark \
  --options="1024" --cpu-type=AtomicSimpleCPU

# SAXPY
./build/X86/gem5.opt --stats-file=sse_saxpy_8192.txt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads_noavx/saxpy/saxpy_benchmark \
  --options="8192" --cpu-type=AtomicSimpleCPU

# Matrix multiply
./build/X86/gem5.opt --stats-file=sse_matmul_128.txt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads_noavx/matmul/matmul_benchmark \
  --options="128" --cpu-type=AtomicSimpleCPU
```

### AVX Version (../workloads/)
```bash
# Simple vector addition
./build/X86/gem5.opt --stats-file=avx_vadd_1024.txt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/simple_vadd/vadd_benchmark \
  --options="1024" --cpu-type=AtomicSimpleCPU

# SAXPY
./build/X86/gem5.opt --stats-file=avx_saxpy_8192.txt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/saxpy/saxpy_benchmark \
  --options="8192" --cpu-type=AtomicSimpleCPU

# Matrix multiply
./build/X86/gem5.opt --stats-file=avx_matmul_128.txt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/matmul/matmul_benchmark \
  --options="128" --cpu-type=AtomicSimpleCPU
```

---

## Computing Speedup

```bash
# Extract simulation ticks
sse_ticks=$(grep "simTicks" m5out/sse_vadd_1024.txt | awk '{print $2}')
avx_ticks=$(grep "simTicks" m5out/avx_vadd_1024.txt | awk '{print $2}')

# Calculate speedup
speedup=$(echo "scale=2; $sse_ticks / $avx_ticks" | bc)
echo "AVX Speedup: ${speedup}x"
```

---

## Expected Results

### Vector Addition (vadd)
- **Theoretical:** 2x (double SIMD width)
- **Actual:** ~1.8-2.0x (accounting for overhead)

### SAXPY
- **Theoretical:** 2-3x (2x width + FMA efficiency)
- **Actual:** ~2.2-2.8x

### Matrix Multiplication
- **Theoretical:** 2-3x (2x width + FMA efficiency)
- **Actual:** ~2.5-3.0x (more compute-bound, benefits more from FMA)

---

## Notes

1. **Instruction Counts:** AVX should have ~half the instruction count for the main loops
2. **Memory Bandwidth:** Both use same memory bandwidth (same data size)
3. **FMA Impact:** SAXPY and matmul benefit more from FMA than vadd
4. **Alignment:** SSE uses 16-byte, AVX uses 32-byte alignment

---

## Verification

All workloads verify their results against C reference implementations:
- ✅ `PASSED: Results match` - Correct implementation
- ❌ `FAILED: X mismatches found` - Implementation error

---

## Compatibility

- **Mainline gem5:** ✅ Fully compatible (SSE is standard x86-64)
- **AVX gem5:** ✅ Fully compatible (SSE subset of AVX)
- **Real Hardware:** ✅ Runs on any x86-64 CPU

---

## File Structure

```
workloads_noavx/
├── README.md
├── simple_vadd/
│   ├── vadd_kernel.S      # SSE assembly
│   ├── vadd.c             # Test harness
│   ├── Makefile
│   └── vadd_benchmark     # Binary
├── saxpy/
│   ├── saxpy_kernel.S     # SSE assembly
│   ├── saxpy.c            # Test harness
│   ├── Makefile
│   └── saxpy_benchmark    # Binary
└── matmul/
    ├── matmul_kernel.S    # SSE assembly
    ├── matmul.c           # Test harness
    ├── Makefile
    └── matmul_benchmark   # Binary
```

---

## References

- Intel® 64 and IA-32 Architectures Software Developer's Manual, Volume 2
- SSE Programming Reference
- gem5 Documentation: http://www.gem5.org/documentation/
