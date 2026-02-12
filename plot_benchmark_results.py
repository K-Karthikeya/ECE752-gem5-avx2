#!/usr/bin/env python3
"""
Plot AVX vs SSE Performance Comparison
Generates bar charts comparing simTicks for each workload across different input sizes.
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys
import os

def plot_workload_comparison(avx_csv, sse_csv, output_dir='plots'):
    """
    Create comparison plots for each workload from AVX and SSE benchmark results.
    
    Args:
        avx_csv: Path to AVX results CSV file
        sse_csv: Path to SSE results CSV file
        output_dir: Directory to save plots
    """
    
    # Create output directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)
    
    # Read CSV files
    try:
        avx_df = pd.read_csv(avx_csv)
        sse_df = pd.read_csv(sse_csv)
    except FileNotFoundError as e:
        print(f"Error: {e}")
        print("\nMake sure you've run the benchmarks first:")
        print("  ./run_benchmarks.sh")
        print("  ./run_benchmarks_sse.sh")
        sys.exit(1)
    
    # Get unique workloads
    workloads = avx_df['Workload'].unique()
    
    print(f"Found {len(workloads)} workloads: {', '.join(workloads)}")
    print(f"Creating plots in '{output_dir}/' directory...\n")
    
    # Create a plot for each workload
    for workload in workloads:
        # Filter data for this workload
        avx_data = avx_df[avx_df['Workload'] == workload].copy()
        sse_data = sse_df[sse_df['Workload'] == workload].copy()
        
        # Merge on Size to ensure matching sizes
        merged = pd.merge(
            avx_data[['Size', 'SimTicks']], 
            sse_data[['Size', 'SimTicks']], 
            on='Size', 
            suffixes=('_AVX', '_SSE')
        )
        
        if merged.empty:
            print(f"Warning: No matching data for {workload}, skipping...")
            continue
        
        # Sort by size
        merged = merged.sort_values('Size')
        
        # Map workload names for display
        display_name = workload.replace('simple_vadd', 'VecAdd').replace('_', ' ').title()
        
        # Convert SimTicks to numeric (handle 'N/A' values)
        merged['SimTicks_AVX'] = pd.to_numeric(merged['SimTicks_AVX'], errors='coerce')
        merged['SimTicks_SSE'] = pd.to_numeric(merged['SimTicks_SSE'], errors='coerce')
        
        # Remove rows with missing data
        merged = merged.dropna()
        
        if merged.empty:
            print(f"Warning: No valid numeric data for {workload}, skipping...")
            continue
        
        # Calculate speedup
        merged['Speedup'] = merged['SimTicks_SSE'] / merged['SimTicks_AVX']
        
        # Create Figure 1: Double bar chart
        fig1, ax1 = plt.subplots(figsize=(10, 6))
        
        x = np.arange(len(merged))
        width = 0.35
        
        bars1 = ax1.bar(x - width/2, merged['SimTicks_AVX'], width, 
                        label='AVX-256', color='#2E86AB', alpha=0.8)
        bars2 = ax1.bar(x + width/2, merged['SimTicks_SSE'], width, 
                        label='SSE-128', color='#A23B72', alpha=0.8)
        
        ax1.set_xlabel('Input Size [n]', fontsize=12, fontweight='bold')
        ax1.set_ylabel('Simulation Ticks [1e-7]', fontsize=12, fontweight='bold')
        ax1.set_title(f'{display_name} - Performance Comparison', 
                     fontsize=14, fontweight='bold')
        ax1.set_xticks(x)
        ax1.set_xticklabels(merged['Size'].astype(str))
        ax1.legend(fontsize=11)
        ax1.grid(axis='y', alpha=0.3, linestyle='--')
        
        # Add value labels on bars
        for bars in [bars1, bars2]:
            for bar in bars:
                height = bar.get_height()
                ax1.text(bar.get_x() + bar.get_width()/2., height,
                        f'{int(height):,}',
                        ha='center', va='bottom', fontsize=8, rotation=0)
        
        plt.tight_layout()
        
        # Save bar chart
        output_file_bars = os.path.join(output_dir, f'{workload}_performance.png')
        plt.savefig(output_file_bars, dpi=300, bbox_inches='tight')
        print(f"✓ Created: {output_file_bars}")
        plt.close()
        
        # Create Figure 2: Speedup chart
        fig2, ax2 = plt.subplots(figsize=(10, 6))
        
        ax2.plot(merged['Size'], merged['Speedup'], marker='o', linewidth=2, 
                markersize=8, color='#F18F01', label='AVX Speedup')
        ax2.axhline(y=2.0, color='green', linestyle='--', alpha=0.5, label='2x baseline')
        ax2.fill_between(merged['Size'], 1.5, 2.5, alpha=0.1, color='green')
        
        ax2.set_xlabel('Input Size [n]', fontsize=12, fontweight='bold')
        ax2.set_ylabel('Speedup (AVX over SSE)', fontsize=12, fontweight='bold')
        ax2.set_title(f'{display_name} - AVX Speedup', 
                     fontsize=14, fontweight='bold')
        ax2.grid(True, alpha=0.3, linestyle='--')
        ax2.legend(fontsize=11)
        
        # Add speedup values on points
        for i, row in merged.iterrows():
            ax2.text(row['Size'], row['Speedup'], f'{row["Speedup"]:.2f}x',
                    ha='center', va='bottom', fontsize=9)
        
        plt.tight_layout()
        
        # Save speedup chart
        output_file_speedup = os.path.join(output_dir, f'{workload}_speedup.png')
        plt.savefig(output_file_speedup, dpi=300, bbox_inches='tight')
        print(f"✓ Created: {output_file_speedup}")
        plt.close()
        
        # Print statistics
        print(f"  {workload}:")
        print(f"    Average AVX speedup: {merged['Speedup'].mean():.2f}x")
        print(f"    Min speedup: {merged['Speedup'].min():.2f}x (size {merged.loc[merged['Speedup'].idxmin(), 'Size']})")
        print(f"    Max speedup: {merged['Speedup'].max():.2f}x (size {merged.loc[merged['Speedup'].idxmax(), 'Size']})")
        print()
    
    # Create combined summary plot
    create_summary_plot(avx_df, sse_df, workloads, output_dir)
    
    print(f"\n✓ All plots saved to '{output_dir}/' directory")

def create_summary_plot(avx_df, sse_df, workloads, output_dir):
    """Create a summary plot showing average speedup for all workloads."""
    
    fig, ax = plt.subplots(figsize=(10, 6))
    
    speedups = []
    labels = []
    
    for workload in workloads:
        avx_data = avx_df[avx_df['Workload'] == workload].copy()
        sse_data = sse_df[sse_df['Workload'] == workload].copy()
        
        merged = pd.merge(
            avx_data[['Size', 'SimTicks']], 
            sse_data[['Size', 'SimTicks']], 
            on='Size', 
            suffixes=('_AVX', '_SSE')
        )
        
        merged['SimTicks_AVX'] = pd.to_numeric(merged['SimTicks_AVX'], errors='coerce')
        merged['SimTicks_SSE'] = pd.to_numeric(merged['SimTicks_SSE'], errors='coerce')
        merged = merged.dropna()
        
        if not merged.empty:
            merged['Speedup'] = merged['SimTicks_SSE'] / merged['SimTicks_AVX']
            avg_speedup = merged['Speedup'].mean()
            speedups.append(avg_speedup)
            # Map workload names for display
            display_label = workload.replace('simple_vadd', 'VecAdd').replace('_', ' ').title()
            labels.append(display_label)
    
    # Create bar chart
    colors = ['#2E86AB', '#A23B72', '#F18F01']
    bars = ax.bar(labels, speedups, color=colors[:len(labels)], alpha=0.8, edgecolor='black')
    
    # Add reference lines
    ax.axhline(y=2.0, color='green', linestyle='--', alpha=0.5, label='2x baseline')
    ax.axhline(y=2.5, color='orange', linestyle='--', alpha=0.5, label='2.5x target')
    
    ax.set_ylabel('Average Speedup (AVX vs SSE)', fontsize=12, fontweight='bold')
    ax.set_title('AVX-256 vs SSE-128 Performance Summary', fontsize=14, fontweight='bold')
    ax.set_ylim(0, max(speedups) * 1.2)
    ax.grid(axis='y', alpha=0.3, linestyle='--')
    ax.legend(fontsize=10)
    
    # Add value labels on bars
    for bar, speedup in zip(bars, speedups):
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width()/2., height,
               f'{speedup:.2f}x',
               ha='center', va='bottom', fontsize=11, fontweight='bold')
    
    plt.tight_layout()
    output_file = os.path.join(output_dir, 'summary_speedup.png')
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"✓ Created: {output_file}")
    plt.close()

def main():
    """Main function."""
    
    # Default file paths
    avx_csv = 'benchmark_results/results.csv'
    sse_csv = 'benchmark_results_sse/results.csv'
    output_dir = 'plots'
    
    # Parse command line arguments
    if len(sys.argv) > 1:
        avx_csv = sys.argv[1]
    if len(sys.argv) > 2:
        sse_csv = sys.argv[2]
    if len(sys.argv) > 3:
        output_dir = sys.argv[3]
    
    print("=" * 60)
    print("AVX vs SSE Performance Plotting Tool")
    print("=" * 60)
    print(f"AVX results: {avx_csv}")
    print(f"SSE results: {sse_csv}")
    print(f"Output directory: {output_dir}")
    print("=" * 60)
    print()
    
    # Create plots
    plot_workload_comparison(avx_csv, sse_csv, output_dir)

if __name__ == '__main__':
    main()
