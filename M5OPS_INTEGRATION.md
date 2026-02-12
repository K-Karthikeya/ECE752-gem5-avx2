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

2. **Multiple Statistics Regions:** Each m5_dump_reset_stats() call creates a new statistics region within the **same** stats file:
   - Region 1: Initialization and setup (before first m5_dump_reset_stats)
   - Region 2: **ROI - Kernel execution only** (between the two m5_dump_reset_stats calls)
   - Region 3: Post-kernel execution (after second m5_dump_reset_stats to program end)
   - Regions are separated by markers: `---------- Begin Simulation Statistics ----------`

3. **Clean Comparison:** Both AVX and SSE versions measure exactly the same code region (the SIMD kernel)

## Verification

To verify m5ops are working:

```bash
# Run a simple test
./build/X86/gem5.opt configs/deprecated/example/se.py \
    --cmd=tests/test-progs/workloads/simple_vadd/vadd_benchmark \
    --options='1024' \
    --cpu-type=AtomicSimpleCPU

# Check the stats file
cat m5out/stats.txt | grep "Begin Simulation Statistics" | wc -l
```

You should see:
1. **Console output:**
   ```
   Running C reference version...
   Running AVX-256 version...
   Verifying results...
   PASSED: All results match!
   ```

2. **Stats file with 3 regions:**
   ```bash
   # Count should be 3 (three "Begin Simulation Statistics" markers)
   $ grep -c "Begin Simulation Statistics" m5out/stats.txt
   3
   ```

The stats file structure:
```
---------- Begin Simulation Statistics ----------
<Region 1: Initialization stats>
---------- End Simulation Statistics   ----------
---------- Begin Simulation Statistics ----------
<Region 2: ROI - Kernel execution only> ← We want this!
---------- End Simulation Statistics   ----------
---------- Begin Simulation Statistics ----------
<Region 3: Post-kernel stats>
---------- End Simulation Statistics   ----------
```

## Updated Benchmark Scripts

The benchmark scripts (`run_benchmarks.sh` and `run_benchmarks_sse.sh`) will **automatically extract** the ROI statistics from region 2 and save them to `*_stats1.txt` files.

## Extracting ROI Statistics

The benchmark scripts automatically extract ROI stats, but you can also do it manually:

### Automatic Extraction (Recommended)
The benchmark scripts extract region 2 automatically. Just run:
```bash
./run_benchmarks.sh          # Creates *_stats1.txt files with ROI data
./run_benchmarks_sse.sh      # Creates *_stats1.txt files with ROI data
```

### Manual Extraction
To manually extract ROI from a stats file:

```bash
# Extract ROI from a single file
./extract_roi_stats.sh benchmark_results/simple_vadd_1024_stats.txt \
                       benchmark_results/simple_vadd_1024_stats1.txt

# Or batch extract all files in a directory
./extract_all_roi_stats.sh benchmark_results
./extract_all_roi_stats.sh benchmark_results_sse
```

### View ROI Statistics
```bash
# View ROI measurements (kernel only)
grep "simTicks" benchmark_results/simple_vadd_1024_stats1.txt
grep "numCycles" benchmark_results/simple_vadd_1024_stats1.txt
grep "committedInsts" benchmark_results/simple_vadd_1024_stats1.txt
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
