#!/usr/bin/env bash
#===========================================================================
# Script to run AVX instruction trace test in gem5
#===========================================================================

# Configuration
GEM5_ROOT="../../.."
GEM5_BIN="${GEM5_ROOT}/build/X86/gem5.opt"
CONFIG_SCRIPT="${GEM5_ROOT}/configs/example/se.py"
TEST_BINARY="./avx_trace_test"
# Use an absolute output dir so gem5 can write the debug file
OUTPUT_DIR="$(pwd)/trace_output"

# Create output directory (used as gem5 outdir)
mkdir -p "${OUTPUT_DIR}"

echo "=== AVX Instruction Trace Test Runner ==="
echo ""

# Check if gem5 binary exists
if [ ! -f "${GEM5_BIN}" ]; then
    echo "ERROR: gem5 binary not found at ${GEM5_BIN}"
    echo "Please build gem5 first: scons build/X86/gem5.opt -j$(nproc)"
    exit 1
fi

# Check if test binary exists
if [ ! -f "${TEST_BINARY}" ]; then
    echo "ERROR: Test binary not found at ${TEST_BINARY}"
    echo "Please build the test first: make"
    exit 1
fi

echo "gem5 binary: ${GEM5_BIN}"
echo "Test binary: ${TEST_BINARY}"
echo "Output directory: ${OUTPUT_DIR}"
echo ""

# Run with full execution trace
echo "Running test with execution trace..."
"${GEM5_BIN}" \
    --outdir="${OUTPUT_DIR}" \
    --debug-flags=Exec,AVXVerbose \
    --debug-file="trace.txt" \
    "${CONFIG_SCRIPT}" \
    -c "${TEST_BINARY}" \
    > "${OUTPUT_DIR}/gem5_stdout.txt" 2>&1

EXIT_CODE=$?

echo ""
if [ ${EXIT_CODE} -eq 0 ]; then
    echo "✓ Test completed successfully"
else
    echo "✗ Test failed with exit code ${EXIT_CODE}"
fi

echo ""
echo "=== Output Files ==="
echo "  - Full trace:     ${OUTPUT_DIR}/trace.txt"
echo "  - gem5 output:    ${OUTPUT_DIR}/gem5_stdout.txt"
echo ""

# Extract AVX instruction statistics
echo "=== AVX Instruction Statistics ==="
if [ -f "${OUTPUT_DIR}/trace.txt" ]; then
    echo "Counting AVX instructions in trace..."
    grep -i "vmov\|vmin\|vmax\|vbroadcast\|vzero" "${OUTPUT_DIR}/trace.txt" | \
        grep -o -E "VMOVUPS|VMOVAPS|VMINPS|VMAXPS|VMOVSLDUP|VMOVSHDUP|VBROADCASTSS|VZEROUPPER" | \
        sort | uniq -c | sort -rn
else
    echo "Trace file not found"
fi

echo ""
echo "=== Quick Trace Sample (first 50 AVX instructions) ==="
grep -i "vmov\|vmin\|vmax\|vbroadcast\|vzero" "${OUTPUT_DIR}/trace.txt" | head -50 || echo "No AVX instructions found"

echo ""
echo "Done! Check ${OUTPUT_DIR}/ for detailed results."
