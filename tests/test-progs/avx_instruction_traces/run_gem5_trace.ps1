# AVX Instruction Trace Test - PowerShell Runner
# Windows version of run_gem5_trace.sh

# Configuration
$GEM5_ROOT = "..\..\..\"
$GEM5_BIN = Join-Path $GEM5_ROOT "build\X86\gem5.opt.exe"
# configs/example/se.py moved; use deprecated path
$CONFIG_SCRIPT = Join-Path $GEM5_ROOT "configs\deprecated\example\se.py"
$TEST_BINARY = ".\avx_trace_test.exe"
$OUTPUT_DIR = ".\trace_output"

Write-Host "=== AVX Instruction Trace Test Runner ===" -ForegroundColor Cyan
Write-Host ""

# Create output directory
if (-not (Test-Path $OUTPUT_DIR)) {
    New-Item -ItemType Directory -Path $OUTPUT_DIR | Out-Null
}

# Check if gem5 binary exists
if (-not (Test-Path $GEM5_BIN)) {
    Write-Host "ERROR: gem5 binary not found at $GEM5_BIN" -ForegroundColor Red
    Write-Host "Please build gem5 first: scons build/X86/gem5.opt"
    exit 1
}

# Check if test binary exists
if (-not (Test-Path $TEST_BINARY)) {
    Write-Host "ERROR: Test binary not found at $TEST_BINARY" -ForegroundColor Red
    Write-Host "Please build the test first: make (or use WSL/MinGW)"
    exit 1
}

Write-Host "gem5 binary: $GEM5_BIN"
Write-Host "Test binary: $TEST_BINARY"
Write-Host "Output directory: $OUTPUT_DIR"
Write-Host ""

# Run with full execution trace
Write-Host "Running test with execution trace..." -ForegroundColor Yellow
$traceFile = Join-Path $OUTPUT_DIR "trace.txt"
$stdoutFile = Join-Path $OUTPUT_DIR "gem5_stdout.txt"

& $GEM5_BIN `
    --debug-flags=Exec,AVXVerbose `
    --debug-file=$traceFile `
    $CONFIG_SCRIPT `
    -c $TEST_BINARY `
    *> $stdoutFile

$exitCode = $LASTEXITCODE

Write-Host ""
if ($exitCode -eq 0) {
    Write-Host "✓ Test completed successfully" -ForegroundColor Green
} else {
    Write-Host "✗ Test failed with exit code $exitCode" -ForegroundColor Red
}

Write-Host ""
Write-Host "=== Output Files ===" -ForegroundColor Cyan
Write-Host "  - Full trace:     $traceFile"
Write-Host "  - gem5 output:    $stdoutFile"
Write-Host ""

# Extract AVX instruction statistics
Write-Host "=== AVX Instruction Statistics ===" -ForegroundColor Cyan
if (Test-Path $traceFile) {
    Write-Host "Counting AVX instructions in trace..."
    $avxInstructions = Select-String -Path $traceFile -Pattern "VMOVUPS|VMOVAPS|VMINPS|VMAXPS|VMOVSLDUP|VMOVSHDUP|VBROADCASTSS|VZEROUPPER" -AllMatches
    if ($avxInstructions) {
        $avxInstructions.Matches | Select-Object -ExpandProperty Value | Group-Object | Sort-Object Count -Descending | Format-Table Count, Name -AutoSize
    } else {
        Write-Host "No AVX instructions found in trace"
    }
} else {
    Write-Host "Trace file not found"
}

Write-Host ""
Write-Host "Done! Check $OUTPUT_DIR\ for detailed results." -ForegroundColor Cyan
