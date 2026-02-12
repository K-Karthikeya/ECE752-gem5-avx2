# Benchmark Runner Usage

## Linux/Unix (Bash)

### Make executable
```bash
chmod +x run_benchmarks.sh
```

### Run with defaults
```bash
./run_benchmarks.sh
```

### Specify custom gem5 binary
```bash
./run_benchmarks.sh /path/to/gem5.opt
```

### Specify custom output directory
```bash
./run_benchmarks.sh ./build/X86/gem5.opt my_results
```

### Skip building workloads (if already built)
```bash
./run_benchmarks.sh ./build/X86/gem5.opt benchmark_results true
```

### Full usage
```bash
./run_benchmarks.sh [gem5_binary] [output_dir] [skip_build]

Arguments:
  gem5_binary  - Path to gem5 executable (default: ./build/X86/gem5.opt)
  output_dir   - Directory for results (default: benchmark_results)
  skip_build   - Set to "true" to skip building workloads (default: false)
```

## What it does

1. **Builds all workloads** (unless skip_build=true)
   - simple_vadd
   - saxpy
   - matmul
   - image_blur

2. **Runs each workload** with multiple sizes
   - simple_vadd: 64, 256, 1024, 4096 floats
   - saxpy: 1024, 4096, 8192, 16384 floats
   - matmul: 32x32, 64x64, 96x96, 128x128 matrices
   - image_blur: 128x128, 256x256, 384x384 images

3. **Collects statistics** for each run
   - simTicks
   - simSeconds
   - simFreq

4. **Verifies correctness**
   - PASSED: Results match reference
   - FAILED: Mismatches detected
   - ERROR: gem5 crashed

5. **Generates reports**
   - `benchmark_results/summary.txt` - Human-readable summary
   - `benchmark_results/results.csv` - Machine-readable data
   - `benchmark_results/*_output.txt` - Full output for each run
   - `benchmark_results/*_stats.txt` - gem5 statistics for each run

## Output Files

### Per-workload files
```
benchmark_results/
├── simple_vadd_64_output.txt
├── simple_vadd_64_stats.txt
├── simple_vadd_256_output.txt
├── simple_vadd_256_stats.txt
├── ...
├── matmul_128_output.txt
├── matmul_128_stats.txt
└── ...
```

### Summary files
```
benchmark_results/
├── summary.txt    # Human-readable report
└── results.csv    # CSV for analysis/plotting
```

## Example Run

```bash
$ chmod +x run_benchmarks.sh
$ ./run_benchmarks.sh

========================================
AVX Workload Benchmark Suite
========================================
gem5 binary: ./build/X86/gem5.opt
CPU type: AtomicSimpleCPU
Output directory: benchmark_results

Building workloads...
  Building simple_vadd... ✓
  Building saxpy... ✓
  Building matmul... ✓
  Building image_blur... ✓

========================================
Workload: simple_vadd
========================================
Running: simple_vadd with size 64
  Command: ./build/X86/gem5.opt configs/deprecated/example/se.py ...
  ✓ Completed successfully - PASSED
  Simulation ticks: 1234567
  Simulation time: 0.001234567 seconds

...

========================================
Final Results
========================================
Total benchmarks: 14
Passed: 14
Failed: 0
Errors: 0

🎉 All benchmarks passed successfully!

Results directory: benchmark_results
```

## Analyzing Results

### View summary
```bash
cat benchmark_results/summary.txt
```

### View CSV data
```bash
cat benchmark_results/results.csv
```

### Import to spreadsheet
```bash
# Open in LibreOffice Calc, Excel, or similar
libreoffice benchmark_results/results.csv
```

### Plot with Python
```python
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('benchmark_results/results.csv')
df = df[df['Status'] == 'PASSED']

for workload in df['Workload'].unique():
    data = df[df['Workload'] == workload]
    plt.plot(data['Size'], data['SimTicks'], marker='o', label=workload)

plt.xlabel('Problem Size')
plt.ylabel('Simulation Ticks')
plt.legend()
plt.grid(True)
plt.savefig('benchmark_results.png')
```

## Troubleshooting

### Permission denied
```bash
chmod +x run_benchmarks.sh
```

### gem5 not found
```bash
# Build gem5 first
scons build/X86/gem5.opt -j$(nproc)

# Or specify path explicitly
./run_benchmarks.sh /path/to/gem5.opt
```

### Build errors
```bash
# Check workload Makefiles
cd tests/test-progs/workloads/simple_vadd
make clean
make

# Check compiler version (need gcc 4.7+ for AVX2)
gcc --version
```

### All tests ERROR
- Check that workload binaries exist
- Verify gem5 includes AVX instruction support
- Try running a single workload manually

### Tests FAIL verification
- Check gem5 implementation of AVX instructions
- Review `*_output.txt` files for error messages
- May indicate instruction implementation bug

## Quick Test

Run just one workload manually:
```bash
# Build
cd tests/test-progs/workloads/simple_vadd
make

# Run
cd ../../../../
./build/X86/gem5.opt configs/deprecated/example/se.py \
  --cmd=tests/test-progs/workloads/simple_vadd/vadd_benchmark \
  --options="64" \
  --cpu-type=AtomicSimpleCPU
```

Should output:
```
...
Simple Vector Addition Benchmark
=================================
...
PASSED: All results match!
...
```
