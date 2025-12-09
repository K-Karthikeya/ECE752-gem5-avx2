#!/bin/bash
# AVX Workload Benchmark Runner
# Runs all workloads with AtomicSimpleCPU and collects statistics

# Default configuration
GEM5_BIN="${1:-./build/X86/gem5.opt}"
OUTPUT_DIR="${2:-benchmark_results}"
SKIP_BUILD="${3:-false}"

# Configuration
WORKLOADS_DIR="tests/test-progs/workloads"
CONFIG_SCRIPT="configs/deprecated/example/se.py"
CPU_TYPE="AtomicSimpleCPU"

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}AVX Workload Benchmark Suite${NC}"
echo -e "${CYAN}========================================${NC}"
echo "gem5 binary: $GEM5_BIN"
echo "CPU type: $CPU_TYPE"
echo "Output directory: $OUTPUT_DIR"
echo ""

# Create output directory
if [ ! -d "$OUTPUT_DIR" ]; then
    mkdir -p "$OUTPUT_DIR"
    echo -e "${GREEN}Created output directory: $OUTPUT_DIR${NC}"
fi

# Build all workloads
if [ "$SKIP_BUILD" != "true" ]; then
    echo -e "${YELLOW}Building workloads...${NC}"
    for workload in simple_vadd saxpy matmul image_blur; do
        workload_dir="$WORKLOADS_DIR/$workload"
        echo -n "  Building $workload..."
        
        (cd "$workload_dir" && make clean > /dev/null 2>&1 && make > /dev/null 2>&1)
        
        if [ $? -eq 0 ]; then
            echo -e " ${GREEN}✓${NC}"
        else
            echo -e " ${RED}✗${NC}"
            echo -e "    ${RED}Build error - check $workload_dir${NC}"
        fi
    done
    echo ""
fi

# Initialize results CSV
results_csv="$OUTPUT_DIR/results.csv"
echo "Workload,Size,Status,SimTicks,SimSeconds,SimFreq" > "$results_csv"

# Run benchmarks
total_count=0
passed_count=0
failed_count=0
error_count=0

# Workload configurations: name:binary:sizes
declare -A workload_configs=(
    ["simple_vadd"]="vadd_benchmark:64,256,1024,4096"
    ["saxpy"]="saxpy_benchmark:1024,4096,8192,16384"
    ["matmul"]="matmul_benchmark:32,64,96,128"
    ["image_blur"]="image_blur_benchmark:128,256,384"
)

for workload_name in simple_vadd saxpy matmul image_blur; do
    config="${workload_configs[$workload_name]}"
    binary_name="${config%%:*}"
    sizes="${config#*:}"
    binary_path="$WORKLOADS_DIR/$workload_name/$binary_name"
    
    echo -e "${CYAN}========================================${NC}"
    echo -e "${CYAN}Workload: $workload_name${NC}"
    echo -e "${CYAN}========================================${NC}"
    
    IFS=',' read -ra size_array <<< "$sizes"
    for size in "${size_array[@]}"; do
        # Determine options format
        if [ "$workload_name" = "image_blur" ]; then
            options="$size $size"
        else
            options="$size"
        fi
        
        # gem5 prepends m5out/ to stats file path, so use just filename
        stats_file="${workload_name}_${size}_stats.txt"
        output_file="$OUTPUT_DIR/${workload_name}_${size}_output.txt"
        
        echo -e "${YELLOW}Running: $workload_name with size $size${NC}"
        echo "  Command: $GEM5_BIN --stats-file=\"$stats_file\" $CONFIG_SCRIPT --cmd=\"$binary_path\" --options=\"$options\" --cpu-type=$CPU_TYPE"
        
        # Run gem5 - use eval to properly handle quoted arguments
        eval "$GEM5_BIN --stats-file=\"$stats_file\" $CONFIG_SCRIPT --cmd=\"$binary_path\" --options=\"$options\" --cpu-type=$CPU_TYPE" > "$output_file" 2>&1
        exit_code=$?
        
        # Move stats file from m5out/ to our output directory
        if [ -f "m5out/$stats_file" ]; then
            mv "m5out/$stats_file" "$OUTPUT_DIR/$stats_file"
        fi
        stats_file="$OUTPUT_DIR/$stats_file"
        
        total_count=$((total_count + 1))
        
        if [ $exit_code -eq 0 ]; then
            # Check if workload passed verification
            if grep -q "PASSED" "$output_file"; then
                echo -e "  ${GREEN}✓ Completed successfully - PASSED${NC}"
                status="PASSED"
                passed_count=$((passed_count + 1))
            elif grep -q "FAILED" "$output_file"; then
                echo -e "  ${RED}✗ Completed but FAILED verification${NC}"
                status="FAILED"
                failed_count=$((failed_count + 1))
            else
                echo -e "  ${YELLOW}⚠ Completed but no verification result found${NC}"
                status="UNKNOWN"
            fi
            
            # Extract statistics
            if [ -f "$stats_file" ]; then
                sim_ticks=$(grep "simTicks" "$stats_file" | head -1 | awk '{print $2}')
                sim_seconds=$(grep "simSeconds" "$stats_file" | head -1 | awk '{print $2}')
                sim_freq=$(grep "simFreq" "$stats_file" | head -1 | awk '{print $2}')
                
                echo "  Simulation ticks: $sim_ticks"
                echo "  Simulation time: $sim_seconds seconds"
                
                # Store results
                echo "$workload_name,$size,$status,$sim_ticks,$sim_seconds,$sim_freq" >> "$results_csv"
            else
                echo "$workload_name,$size,$status,N/A,N/A,N/A" >> "$results_csv"
            fi
        else
            echo -e "  ${RED}✗ gem5 failed with exit code $exit_code${NC}"
            status="ERROR"
            error_count=$((error_count + 1))
            echo "$workload_name,$size,$status,N/A,N/A,N/A" >> "$results_csv"
        fi
        
        echo ""
    done
done

# Generate summary report
echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}Benchmark Summary${NC}"
echo -e "${CYAN}========================================${NC}"

summary_file="$OUTPUT_DIR/summary.txt"

cat > "$summary_file" <<EOF
AVX Workload Benchmark Summary
==============================
Date: $(date '+%Y-%m-%d %H:%M:%S')
CPU Type: $CPU_TYPE
gem5 Binary: $GEM5_BIN

Results:
--------
EOF

# Append table to summary
column -t -s',' "$results_csv" >> "$summary_file"

echo -e "${GREEN}Summary saved to: $summary_file${NC}"
echo -e "${GREEN}CSV data saved to: $results_csv${NC}"
echo ""

# Display summary
echo "Results Table:"
column -t -s',' "$results_csv"
echo ""

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}Final Results${NC}"
echo -e "${CYAN}========================================${NC}"
echo "Total benchmarks: $total_count"
echo -e "Passed: ${GREEN}$passed_count${NC}"
if [ $failed_count -gt 0 ]; then
    echo -e "Failed: ${RED}$failed_count${NC}"
else
    echo -e "Failed: ${GREEN}$failed_count${NC}"
fi
if [ $error_count -gt 0 ]; then
    echo -e "Errors: ${RED}$error_count${NC}"
else
    echo -e "Errors: ${GREEN}$error_count${NC}"
fi
echo ""

if [ $passed_count -eq $total_count ]; then
    echo -e "${GREEN}🎉 All benchmarks passed successfully!${NC}"
else
    echo -e "${YELLOW}⚠️  Some benchmarks did not pass. Check output files for details.${NC}"
fi

echo ""
echo -e "${CYAN}Results directory: $OUTPUT_DIR${NC}"
