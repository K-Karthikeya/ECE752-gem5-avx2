# Quick Start: Running Benchmarks with M5ops

## Prerequisites

1. **Build libm5.a:**
   ```bash
   cd util/m5
   scons build/x86/out/m5
   cd ../..
   ```

2. **Build workloads:**
   ```bash
   # AVX workloads
   cd tests/test-progs/workloads
   for dir in simple_vadd saxpy matmul; do
       (cd $dir && make clean && make)
   done
   cd ../../..

   # SSE baseline workloads
   cd tests/test-progs/workloads_noavx
   for dir in simple_vadd saxpy matmul; do
       (cd $dir && make clean && make)
   done
   cd ../../..
   ```

## Running Benchmarks

### AVX Benchmarks
```bash
./run_benchmarks.sh
```

Output directory: `benchmark_results/`

### SSE Baseline Benchmarks
```bash
./run_benchmarks_sse.sh
```

Output directory: `benchmark_results_sse/`

### Compare Results
```bash
./compare_results.sh
```

## Understanding the Output

Each benchmark run creates these files:

```
benchmark_results/
├── simple_vadd_1024_stats.txt      # Full simulation (3 regions)
├── simple_vadd_1024_stats1.txt     # ROI only (auto-extracted) ✓
├── simple_vadd_1024_output.txt     # Console output
├── saxpy_4096_stats.txt
├── saxpy_4096_stats1.txt           # ROI only (auto-extracted) ✓
├── ...
├── results.csv                      # Summary CSV
└── summary.txt                      # Human-readable summary
```

**Key files:**
- `*_stats.txt` - Full simulation with 3 regions
- `*_stats1.txt` - **ROI only** (kernel execution) - Used for comparison
- `results.csv` - All ROI measurements in CSV format

## How M5ops Work

### Code Instrumentation
```c
// Before kernel
m5_dump_reset_stats(0, 0);  // Marks start of ROI, resets counters

// Kernel execution (only this is measured in ROI)
vadd_avx256(z_avx, x, y, n);

// After kernel  
m5_dump_reset_stats(0, 0);  // Marks end of ROI, dumps stats
```

### Stats File Structure
```
---------- Begin Simulation Statistics ----------
Region 1: Initialization (memory allocation, array setup)
---------- End Simulation Statistics   ----------

---------- Begin Simulation Statistics ----------
Region 2: ROI - Kernel execution only ← Extracted to *_stats1.txt
---------- End Simulation Statistics   ----------

---------- Begin Simulation Statistics ----------
Region 3: Post-kernel (verification, cleanup)
---------- End Simulation Statistics   ----------
```

## Automatic ROI Extraction

The benchmark scripts automatically:
1. Run gem5 simulation
2. Check for multiple regions in stats.txt (should be 3)
3. Extract region 2 (ROI) to *_stats1.txt
4. Use *_stats1.txt for performance metrics

**You don't need to manually extract ROI** - it's done automatically!

## Manual ROI Extraction (if needed)

If you need to manually extract ROI from existing stats files:

```bash
# Single file
./extract_roi_stats.sh input_stats.txt output_stats1.txt

# Batch process directory
./extract_all_roi_stats.sh benchmark_results
./extract_all_roi_stats.sh benchmark_results_sse
```

## Expected Results

### Sample Output
```
Running: simple_vadd with size 1024
  Command: ./build/X86/gem5.opt ...
  ✓ Completed successfully - PASSED
  ✓ Extracted ROI statistics (region 2 of 3)
  ROI Simulation ticks: 123456 (kernel only)
  ROI Simulation time: 0.000123 seconds (kernel only)
```

### Speedup Comparison
```bash
./compare_results.sh
```

Expected speedups (AVX vs SSE):
- **simple_vadd:** ~2.0x (memory-bound, 2× SIMD width)
- **saxpy:** ~2.5x (2× SIMD width + FMA benefit)
- **matmul:** ~2.5-3.0x (compute-bound, maximum FMA benefit)

## Troubleshooting

### Problem: Only 1 statistics region found
**Cause:** libm5.a not linked or m5ops not being called

**Solution:**
1. Verify libm5.a exists: `ls -la util/m5/build/x86/out/libm5.a`
2. Rebuild workloads: `cd tests/test-progs/workloads/simple_vadd && make clean && make`
3. Check Makefile has m5ops configuration

### Problem: ROI extraction produces empty file
**Cause:** Stats file format unexpected

**Solution:**
1. Check stats file: `grep -c "Begin Simulation" benchmark_results/simple_vadd_1024_stats.txt`
2. Should return 3 (three regions)
3. Manually inspect: `less benchmark_results/simple_vadd_1024_stats.txt`

### Problem: Workload doesn't compile with m5ops
**Cause:** Missing include path or library

**Solution:**
Check Makefile has:
```makefile
GEM5_PATH := ../../../..
M5OPS_INCLUDE := $(GEM5_PATH)/include
M5OPS_LIB := $(GEM5_PATH)/util/m5/build/x86/out
CFLAGS += -I$(M5OPS_INCLUDE)
LDFLAGS := -L$(M5OPS_LIB) -lm5
```

## Files Created by This Setup

```
ECE752-gem5-avx2/
├── extract_roi_stats.sh           # Extract single file
├── extract_all_roi_stats.sh       # Batch extract directory
├── run_benchmarks.sh              # AVX benchmark suite
├── run_benchmarks_sse.sh          # SSE baseline suite
├── compare_results.sh             # Compare AVX vs SSE
├── M5OPS_INTEGRATION.md           # Detailed documentation
└── BENCHMARK_QUICKSTART.md        # This file
```

## Complete Workflow

```bash
# 1. Build everything
cd util/m5 && scons build/x86/out/m5 && cd ../..
cd tests/test-progs/workloads/simple_vadd && make clean && make && cd ../../../..
cd tests/test-progs/workloads/saxpy && make clean && make && cd ../../../..
cd tests/test-progs/workloads/matmul && make clean && make && cd ../../../..
cd tests/test-progs/workloads_noavx/simple_vadd && make clean && make && cd ../../../..
cd tests/test-progs/workloads_noavx/saxpy && make clean && make && cd ../../../..
cd tests/test-progs/workloads_noavx/matmul && make clean && make && cd ../../../..

# 2. Run benchmarks
./run_benchmarks.sh              # AVX version
./run_benchmarks_sse.sh          # SSE baseline

# 3. Compare results
./compare_results.sh

# 4. View results
cat benchmark_comparison.csv
cat benchmark_comparison_report.txt
```

That's it! The scripts handle all ROI extraction automatically.
