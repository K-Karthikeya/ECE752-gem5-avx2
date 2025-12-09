# M5ops Integration Guide

## Overview

All AVX and SSE baseline workloads have been instrumented with gem5's `m5_dump_reset_stats()` operations to provide accurate performance measurements. The m5ops are placed immediately before and after the critical assembly kernel execution to isolate the region of interest (ROI).

## What Was Changed

### Code Modifications

All 6 workloads (3 AVX + 3 SSE) have been updated:

#### 1. **Header Inclusion**
Added `#include <gem5/m5ops.h>` to all C source files:
- `tests/test-progs/workloads/simple_vadd/vadd.c`
- `tests/test-progs/workloads/saxpy/saxpy.c`
- `tests/test-progs/workloads/matmul/matmul.c`
- `tests/test-progs/workloads_noavx/simple_vadd/vadd.c`
- `tests/test-progs/workloads_noavx/saxpy/saxpy.c`
- `tests/test-progs/workloads_noavx/matmul/matmul.c`

#### 2. **M5ops Instrumentation**
Added `m5_dump_reset_stats(0, 0)` calls before and after the assembly kernel:

**AVX Example (vadd.c):**
```c
// Run AVX version
printf("Running AVX-256 version...\n");
m5_dump_reset_stats(0, 0);      // ← Reset stats, start measuring
vadd_avx256(z_avx, x, y, n);    // ← Critical region
m5_dump_reset_stats(0, 0);      // ← Dump stats, reset for next run
```

**SSE Example (vadd.c):**
```c
// Run SSE version
printf("Running SSE-128 version...\n");
m5_dump_reset_stats(0, 0);      // ← Reset stats, start measuring
vadd_sse(z_sse, x, y, n);       // ← Critical region
m5_dump_reset_stats(0, 0);      // ← Dump stats, reset for next run
```

#### 3. **Makefile Updates**
All Makefiles updated to link against `libm5.a`:

```makefile
# gem5 m5ops configuration
GEM5_PATH := ../../../..
M5OPS_INCLUDE := $(GEM5_PATH)/include
M5OPS_LIB := $(GEM5_PATH)/util/m5/build/x86/out

CFLAGS += -I$(M5OPS_INCLUDE)
LDFLAGS := -L$(M5OPS_LIB) -lm5

# Linking command
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
```

## Building libm5.a

Before compiling the workloads, you must build the m5ops library:

```bash
cd util/m5
scons build/x86/out/m5
```

This creates:
- `util/m5/build/x86/out/libm5.a` - Static library for linking
- `util/m5/build/x86/out/m5` - Command-line utility (optional)

## Compiling Workloads

After building `libm5.a`, compile the workloads as usual:

```bash
# AVX workloads
cd tests/test-progs/workloads/simple_vadd && make clean && make
cd ../saxpy && make clean && make
cd ../matmul && make clean && make

# SSE workloads
cd ../../workloads_noavx/simple_vadd && make clean && make
cd ../saxpy && make clean && make
cd ../matmul && make clean && make
```

## How M5ops Work

### Function: `m5_dump_reset_stats(uint64_t delay, uint64_t period)`

- **First call (before kernel):** Resets all simulation statistics to zero, then starts fresh measurement
- **Second call (after kernel):** Dumps current statistics to a file, then resets for next measurement
- **Parameters:**
  - `delay`: Nanoseconds to wait before executing (we use 0)
  - `period`: Period for repeated execution (we use 0 - single execution)

### Benefits

1. **Accurate ROI Measurement:** Only measures the assembly kernel execution, excluding:
   - Memory allocation and initialization
   - C reference implementation
   - Verification checks
   - I/O operations

2. **Multiple Statistics Files:** Each m5_dump_reset_stats() call creates a separate stats file:
   - First call: Resets counters (no output, just initialization overhead)
   - Second call: Dumps stats for the ROI (kernel execution only)
   - Stats files are numbered sequentially: `stats.txt`, `stats1.txt`, `stats2.txt`, etc.

3. **Clean Comparison:** Both AVX and SSE versions measure exactly the same code region (the SIMD kernel)

## Verification

To verify m5ops are working:

```bash
# Run a simple test
./build/X86/gem5.opt configs/deprecated/example/se.py \
    --cmd=tests/test-progs/workloads/simple_vadd/vadd_benchmark \
    --options='1024' \
    --cpu-type=AtomicSimpleCPU

# Check for multiple stats files
ls -la m5out/stats*.txt
```

You should see output like:
```
Running C reference version...
Running AVX-256 version...
Verifying results...
PASSED: All results match!
```

And multiple stats files in `m5out/`:
- `m5out/stats.txt` - Stats up to first m5_dump_reset_stats()
- `m5out/stats1.txt` - Stats for the kernel ROI (use this one!)
- `m5out/stats2.txt` - Stats after kernel to program end

## Updated Benchmark Scripts

The benchmark scripts (`run_benchmarks.sh` and `run_benchmarks_sse.sh`) will automatically collect the ROI statistics. Look for the stats file with the kernel execution metrics.

## Extracting ROI Statistics

To extract just the kernel execution statistics:

```bash
# The second stats file (stats1.txt) contains the ROI measurements
grep "simTicks" m5out/stats1.txt
grep "numCycles" m5out/stats1.txt
grep "committedInsts" m5out/stats1.txt
```

## Troubleshooting

### Error: "undefined reference to `m5_dump_reset_stats'"
- **Cause:** libm5.a not built or not found
- **Solution:** Build libm5.a first: `cd util/m5 && scons build/x86/out/m5`

### Error: "gem5/m5ops.h: No such file or directory"
- **Cause:** Incorrect GEM5_PATH in Makefile
- **Solution:** Verify `GEM5_PATH` points to gem5 root (should be `../../../..` from workload directories)

### Stats files not created
- **Cause:** Running in SE mode without proper m5ops support
- **Solution:** Ensure you're using a gem5 build that supports m5ops (standard builds do)

## Performance Analysis

With m5ops instrumentation, you can now get highly accurate measurements:

### AVX vs SSE Speedup
```bash
# Run both benchmarks
./run_benchmarks.sh          # AVX results in benchmark_results/
./run_benchmarks_sse.sh      # SSE results in benchmark_results_sse/

# Compare ROI statistics
avx_ticks=$(grep "simTicks" benchmark_results/simple_vadd_1024_stats1.txt | awk '{print $2}')
sse_ticks=$(grep "simTicks" benchmark_results_sse/simple_vadd_1024_stats1.txt | awk '{print $2}')
speedup=$(echo "scale=2; $sse_ticks / $avx_ticks" | bc)
echo "AVX Speedup: ${speedup}x"
```

### Expected Results
- **simple_vadd:** ~2.0x speedup (purely memory bandwidth × SIMD width)
- **saxpy:** ~2.5x speedup (memory + compute, benefits from FMA)
- **matmul:** ~2.5-3.0x speedup (compute-bound, maximum FMA benefit)

## References

- [gem5 M5ops Documentation](https://www.gem5.org/documentation/general_docs/m5ops/)
- gem5 Tutorial: [Using M5ops for ROI](https://youtu.be/TeHKMVOWUAY)
- Source code: `util/m5/` directory
- Header: `include/gem5/m5ops.h`
