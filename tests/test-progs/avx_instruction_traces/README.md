# AVX Instruction Trace Test

This directory contains a comprehensive test for all implemented AVX instructions in gem5.

## Tested Instructions

- **VMOVUPS**: Unaligned move (single-precision)
- **VMOVAPS**: Aligned move (single-precision)
- **VMINPS**: Minimum (packed single-precision)
- **VMAXPS**: Maximum (packed single-precision)
- **VMOVSLDUP**: Duplicate low elements
- **VMOVSHDUP**: Duplicate high elements
- **VBROADCASTSS**: Broadcast single scalar
- **VZEROUPPER**: Zero upper bits of YMM registers

## Files

- `avx_trace_test.S` - Assembly test covering all instructions
- `avx_trace_harness.c` - C harness that validates results
- `Makefile` - Build system
- `run_gem5_trace.sh` - Script to run test in gem5 with tracing

## Building

```bash
make
```

This will compile the test binary with AVX2 support.

## Running Natively (if your host supports AVX)

```bash
make run
```

## Running in gem5 with Tracing

```bash
bash run_gem5_trace.sh
```

This will:
1. Build gem5 if needed (expects `build/X86/gem5.opt`)
2. Run the test with `--debug-flags=Exec,AVXVerbose`
3. Save traces to `trace_output/`
4. Display instruction statistics

## Checking Disassembly

```bash
make disasm
```

Shows the disassembled AVX instructions.

## Checking Instruction Count

```bash
make check
```

Counts and lists all AVX instructions in the binary.

## Output

The test validates:
- Reg-reg forms of all instructions
- Reg-mem forms (where applicable)
- RIP-relative addressing (where applicable)
- Correct results for each operation

The gem5 trace will show detailed execution of each AVX instruction, including:
- Instruction decode
- Macroop expansion
- Microop execution
- Register values

## Trace Analysis

After running `run_gem5_trace.sh`, check:
- `trace_output/trace.txt` - Full execution trace
- `trace_output/gem5_stdout.txt` - gem5 output and test results

The script automatically extracts AVX instruction statistics from the trace.
