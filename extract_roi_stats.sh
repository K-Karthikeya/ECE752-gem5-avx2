#!/bin/bash
# Extract ROI (Region of Interest) statistics from gem5 stats files
#
# gem5 with m5_dump_reset_stats() generates multiple statistics regions
# within a single stats.txt file, separated by markers:
# ---------- Begin Simulation Statistics ----------
# <stats data>
# ---------- End Simulation Statistics   ----------
#
# This script extracts the middle region (ROI) which contains only
# the kernel execution statistics between the two m5_dump_reset_stats() calls.

print_usage() {
    echo "Usage: $0 <input_stats_file> <output_roi_file>"
    echo ""
    echo "Example:"
    echo "  $0 benchmark_results/simple_vadd_1024_stats.txt benchmark_results/simple_vadd_1024_stats1.txt"
    echo ""
    echo "This extracts the 2nd statistics region (ROI) from the input file."
}

if [ $# -ne 2 ]; then
    print_usage
    exit 1
fi

INPUT_FILE="$1"
OUTPUT_FILE="$2"

# Check if input file exists
if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: Input file not found: $INPUT_FILE"
    exit 1
fi

# Count the number of statistics regions
region_count=$(grep -c "^---------- Begin Simulation Statistics ----------$" "$INPUT_FILE")

if [ "$region_count" -lt 2 ]; then
    echo "Error: Expected at least 2 statistics regions, found $region_count"
    echo "Make sure m5_dump_reset_stats() is called twice (before and after kernel)"
    exit 1
fi

echo "Found $region_count statistics regions in $INPUT_FILE"
echo "Extracting region 2 (ROI - kernel execution only)..."

# Extract the second region (between 2nd Begin and 2nd End markers)
# Using awk to extract the specific region
awk '
    BEGIN { region = 0; capture = 0 }
    /^---------- Begin Simulation Statistics ----------$/ {
        region++
        if (region == 2) capture = 1
        next
    }
    /^---------- End Simulation Statistics   ----------$/ {
        if (region == 2) {
            capture = 0
            exit
        }
        next
    }
    capture { print }
' "$INPUT_FILE" > "$OUTPUT_FILE"

if [ $? -eq 0 ] && [ -s "$OUTPUT_FILE" ]; then
    lines=$(wc -l < "$OUTPUT_FILE")
    echo "Successfully extracted ROI statistics ($lines lines)"
    echo "Output: $OUTPUT_FILE"

    # Show a sample of key metrics
    echo ""
    echo "Sample ROI metrics:"
    grep -E "^(simTicks|simSeconds|numCycles|committedInsts)" "$OUTPUT_FILE" | head -5
else
    echo "Error: Failed to extract ROI statistics"
    exit 1
fi
