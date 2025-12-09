#!/bin/bash
# Compare AVX vs SSE Benchmark Results
# Computes speedup factors from benchmark results

AVX_DIR="${1:-benchmark_results}"
SSE_DIR="${2:-benchmark_results_sse}"

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}AVX vs SSE Performance Comparison${NC}"
echo -e "${CYAN}========================================${NC}"
echo ""

# Check if result directories exist
if [ ! -d "$AVX_DIR" ]; then
    echo -e "${RED}Error: AVX results directory not found: $AVX_DIR${NC}"
    echo "Run ./run_benchmarks.sh first"
    exit 1
fi

if [ ! -d "$SSE_DIR" ]; then
    echo -e "${RED}Error: SSE results directory not found: $SSE_DIR${NC}"
    echo "Run ./run_benchmarks_sse.sh first"
    exit 1
fi

# Create comparison CSV
comparison_csv="benchmark_comparison.csv"
echo "Workload,Size,SSE_Ticks,AVX_Ticks,Speedup,Theoretical_Max" > "$comparison_csv"

echo -e "${BOLD}Performance Comparison:${NC}"
echo "============================================================================================================"
printf "%-15s %-10s %-15s %-15s %-10s %-15s\n" "Workload" "Size" "SSE Ticks" "AVX Ticks" "Speedup" "Theoretical"
echo "============================================================================================================"

# Workload configurations
workloads="simple_vadd saxpy matmul"
declare -A sizes=(
    ["simple_vadd"]="64 256 1024 4096"
    ["saxpy"]="1024 4096 8192 16384"
    ["matmul"]="32 64 96 128"
)

declare -A theoretical=(
    ["simple_vadd"]="2.0"
    ["saxpy"]="2.5"
    ["matmul"]="2.5"
)

total_speedup=0
count=0

for workload in $workloads; do
    for size in ${sizes[$workload]}; do
        sse_stats="$SSE_DIR/${workload}_${size}_stats.txt"
        avx_stats="$AVX_DIR/${workload}_${size}_stats.txt"
        
        if [ -f "$sse_stats" ] && [ -f "$avx_stats" ]; then
            # Extract simulation ticks
            sse_ticks=$(grep "simTicks" "$sse_stats" | head -1 | awk '{print $2}')
            avx_ticks=$(grep "simTicks" "$avx_stats" | head -1 | awk '{print $2}')
            
            if [ -n "$sse_ticks" ] && [ -n "$avx_ticks" ] && [ "$avx_ticks" != "0" ]; then
                # Calculate speedup
                speedup=$(echo "scale=3; $sse_ticks / $avx_ticks" | bc)
                theo=${theoretical[$workload]}
                
                # Color code speedup based on performance
                if (( $(echo "$speedup >= $theo * 0.9" | bc -l) )); then
                    color=$GREEN
                elif (( $(echo "$speedup >= $theo * 0.7" | bc -l) )); then
                    color=$YELLOW
                else
                    color=$RED
                fi
                
                printf "%-15s %-10s %-15s %-15s ${color}%-10s${NC} %-15s\n" \
                    "$workload" "$size" "$sse_ticks" "$avx_ticks" "${speedup}x" "${theo}x"
                
                # Save to CSV
                echo "$workload,$size,$sse_ticks,$avx_ticks,$speedup,$theo" >> "$comparison_csv"
                
                total_speedup=$(echo "$total_speedup + $speedup" | bc)
                count=$((count + 1))
            fi
        fi
    done
done

echo "============================================================================================================"

# Calculate average speedup
if [ $count -gt 0 ]; then
    avg_speedup=$(echo "scale=3; $total_speedup / $count" | bc)
    echo ""
    echo -e "${BOLD}Summary:${NC}"
    echo "  Total benchmarks compared: $count"
    echo -e "  Average speedup (AVX vs SSE): ${GREEN}${avg_speedup}x${NC}"
    echo ""
fi

# Generate detailed report
report_file="benchmark_comparison_report.txt"

cat > "$report_file" <<EOF
AVX vs SSE Performance Comparison Report
=========================================
Generated: $(date '+%Y-%m-%d %H:%M:%S')

AVX Results Directory: $AVX_DIR
SSE Results Directory: $SSE_DIR

Performance Summary:
--------------------
Total benchmarks: $count
Average AVX speedup: ${avg_speedup}x

Detailed Results:
-----------------
EOF

column -t -s',' "$comparison_csv" >> "$report_file"

cat >> "$report_file" <<EOF

Theoretical Maximum Speedups:
------------------------------
simple_vadd: 2.0x  (double SIMD width: 8 floats vs 4 floats)
saxpy:       2.5x  (2x SIMD width + FMA efficiency)
matmul:      2.5x  (2x SIMD width + FMA efficiency, compute-bound)

Analysis:
---------
- Vector Addition: Memory-bound, benefits mainly from 2x SIMD width
- SAXPY: Balanced compute/memory, benefits from both width and FMA
- Matrix Multiply: Compute-bound, maximum benefit from width + FMA

Color Legend (in terminal output):
-----------------------------------
GREEN:  >= 90% of theoretical maximum (excellent)
YELLOW: >= 70% of theoretical maximum (good)
RED:    < 70% of theoretical maximum (needs investigation)

Files Generated:
----------------
- $comparison_csv: Machine-readable CSV data
- $report_file: This human-readable report
EOF

echo -e "${GREEN}Comparison report saved to: $report_file${NC}"
echo -e "${GREEN}CSV data saved to: $comparison_csv${NC}"
echo ""

# Show key insights
echo -e "${CYAN}Key Insights:${NC}"
echo "-------------"

for workload in $workloads; do
    # Calculate average for this workload
    workload_sum=0
    workload_count=0
    
    for size in ${sizes[$workload]}; do
        speedup=$(grep "^$workload,$size," "$comparison_csv" | cut -d',' -f5)
        if [ -n "$speedup" ]; then
            workload_sum=$(echo "$workload_sum + $speedup" | bc)
            workload_count=$((workload_count + 1))
        fi
    done
    
    if [ $workload_count -gt 0 ]; then
        workload_avg=$(echo "scale=2; $workload_sum / $workload_count" | bc)
        theo=${theoretical[$workload]}
        efficiency=$(echo "scale=1; ($workload_avg / $theo) * 100" | bc)
        
        echo -e "  ${BOLD}$workload:${NC} ${workload_avg}x average (${efficiency}% of ${theo}x theoretical)"
    fi
done

echo ""
echo -e "${YELLOW}To visualize results:${NC}"
echo "  python3 -c \"import pandas as pd; import matplotlib.pyplot as plt; df = pd.read_csv('$comparison_csv'); df.groupby('Workload')['Speedup'].mean().plot(kind='bar'); plt.ylabel('Speedup'); plt.title('AVX vs SSE Average Speedup'); plt.savefig('speedup_comparison.png'); print('Saved to speedup_comparison.png')\""
