AVX Test Suite

This test suite provides a comprehensive set of checks for the AVX instruction implementations added to gem5. It includes:
- A single C harness (`avx_harness.c`) that invokes multiple assembly routines.
- Assembly files under `ops/` that exercise different instruction variants (128-bit and 256-bit), data patterns, and sequencing.
- Self-checks after each call to validate correctness lane-by-lane.

Build:
- Use the provided `Makefile` to build a static Linux binary with GCC that supports AVX/AVX2/FMA.

Run:
- The binary prints PASS/FAIL per test and exits non-zero on failure.
