#!/bin/bash
# Batch extract ROI statistics from all benchmark results
# Processes all stats.txt files in a directory and creates corresponding stats1.txt files

print_usage() {
    echo "Usage: $0 [results_directory]"
    echo ""
    echo "Default directory: benchmark_results"
    echo ""
    echo "This script finds all *_stats.txt files and extracts the ROI (2nd region)"
    echo "to corresponding *_stats1.txt files."
}

RESULTS_DIR="${1:-benchmark_results}"

if [ ! -d "$RESULTS_DIR" ]; then
    echo "Error: Directory not found: $RESULTS_DIR"
    print_usage
    exit 1
fi

echo "Processing ROI extraction for: $RESULTS_DIR"
echo "=============================================="

# Color codes
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m'

success_count=0
fail_count=0
skip_count=0

# Find all stats files (but not stats1.txt, stats2.txt, etc.)
for stats_file in "$RESULTS_DIR"/*_stats.txt; do
    # Skip if no files found
    [ -e "$stats_file" ] || continue

    # Skip if it's already a numbered stats file
    if [[ "$stats_file" =~ _stats[0-9]+\.txt$ ]]; then
        continue
    fi

    # Generate output filename by inserting '1' before .txt
    base_name="${stats_file%.txt}"
    roi_file="${base_name}1.txt"

    # Check if ROI file already exists
    if [ -f "$roi_file" ]; then
        echo -e "${YELLOW}⊙${NC} Skipping (already exists): $(basename "$stats_file")"
        skip_count=$((skip_count + 1))
        continue
    fi

    echo -n "Processing: $(basename "$stats_file") ... "

    # Extract ROI
    ./extract_roi_stats.sh "$stats_file" "$roi_file" > /dev/null 2>&1

    if [ $? -eq 0 ] && [ -s "$roi_file" ]; then
        echo -e "${GREEN}✓${NC}"
        success_count=$((success_count + 1))
    else
        echo -e "${RED}✗${NC}"
        fail_count=$((fail_count + 1))
        # Clean up failed extraction
        rm -f "$roi_file"
    fi
done

echo ""
echo "=============================================="
echo "Extraction Summary:"
echo -e "  ${GREEN}Success: $success_count${NC}"
echo -e "  ${RED}Failed:  $fail_count${NC}"
echo -e "  ${YELLOW}Skipped: $skip_count${NC}"
echo ""

if [ $success_count -gt 0 ]; then
    echo "ROI stats files created with '_stats1.txt' suffix"
    echo "These contain only kernel execution statistics (between m5_dump_reset_stats calls)"
fi
