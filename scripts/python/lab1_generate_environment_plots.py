#!/usr/bin/env python3
import argparse
import csv
import math
from collections import defaultdict
from pathlib import Path

def parse_float(value, default=0.0):
    try:
        return float(value)
    except Exception:
        return default

def parse_int(value, default=0):
    try:
        return int(float(value))
    except Exception:
        return default

def infer_environment(source_file: str) -> str:
    name = source_file.lower()
    if "msvc" in name:
        return "msvc"
    if "mingw" in name:
        return "mingw"
    if "linux" in name:
        return "linux"
    return "unknown"

def load_rows(input_dir: Path):
    rows = []
    for csv_file in sorted(input_dir.glob("lab1_*.csv")):
        with csv_file.open("r", encoding="utf-8-sig", newline="") as f:
            reader = csv.DictReader(f)
            for row in reader:
                source = csv_file.name
                env = (row.get("environment") or row.get("Environment") or infer_environment(source)).strip().lower()
                mode = (row.get("mode") or row.get("Mode") or "unknown").strip().lower()
                size_bytes = parse_int(row.get("size_bytes") or row.get("size") or row.get("SizeBytes"))
                if not env or env == "unknown" or not mode or mode == "unknown" or size_bytes <= 0:
                    continue
                rows.append({
                    "environment": env,
                    "mode": mode,
                    "size_bytes": size_bytes,
                    "size_kib": size_bytes / 1024.0,
                    "runs": parse_int(row.get("runs") or row.get("Runs")),
                    "mean_ms": parse_float(row.get("mean_ms") or row.get("MeanMs")),
                    "median_ms": parse_float(row.get("median_ms") or row.get("MedianMs")),
                    "stddev_ms": parse_float(row.get("stddev_ms") or row.get("StddevMs")),
                    "ci95_low_ms": parse_float(row.get("ci95_low_ms") or row.get("Ci95LowMs")),
                    "ci95_high_ms": parse_float(row.get("ci95_high_ms") or row.get("Ci95HighMs")),
                    "throughput_mib_s": parse_float(row.get("throughput_mib_s") or row.get("throughput_MBps") or row.get("ThroughputMiBs")),
                    "source_file": source,
                })
    return rows

def write_summary(rows, output_dir: Path):
    output_dir.mkdir(parents=True, exist_ok=True)
    csv_path = output_dir / "lab1_benchmark_environment_summary.csv"
    fields = [
        "environment", "mode", "size_bytes", "size_kib", "runs",
        "mean_ms", "median_ms", "stddev_ms", "ci95_low_ms", "ci95_high_ms",
        "throughput_mib_s", "source_file"
    ]
    with csv_path.open("w", encoding="utf-8", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fields)
        writer.writeheader()
        for row in sorted(rows, key=lambda r: (r["environment"], r["mode"], r["size_bytes"], r["source_file"])):
            writer.writerow(row)

    max_size = max((r["size_bytes"] for r in rows), default=0)
    md_path = output_dir / "lab1_benchmark_environment_summary.md"
    latest = [r for r in rows if r["size_bytes"] == max_size]
    with md_path.open("w", encoding="utf-8") as f:
        f.write("# Lab 1 Benchmark Summary by Environment\n\n")
        f.write(f"Largest payload used for compact comparison: {max_size} bytes.\n\n")
        f.write("| Environment | Mode | Mean latency (ms/op) | Median latency (ms/op) | Stddev (ms) | 95% CI low | 95% CI high | Throughput (MiB/s) |\n")
        f.write("|---|---:|---:|---:|---:|---:|---:|---:|\n")
        for r in sorted(latest, key=lambda x: (x["environment"], x["mode"])):
            f.write(
                f"| {r['environment']} | {r['mode'].upper()} | "
                f"{r['mean_ms']:.6g} | {r['median_ms']:.6g} | {r['stddev_ms']:.6g} | "
                f"{r['ci95_low_ms']:.6g} | {r['ci95_high_ms']:.6g} | {r['throughput_mib_s']:.6g} |\n"
            )
    return csv_path, md_path

def make_plots(rows, output_dir: Path):
    import matplotlib.pyplot as plt

    output_dir.mkdir(parents=True, exist_ok=True)
    max_size = max(r["size_bytes"] for r in rows)
    latest = [r for r in rows if r["size_bytes"] == max_size]

    envs = sorted({r["environment"] for r in latest})
    modes = ["ecb", "cbc", "cfb", "ofb", "ctr", "xts", "ccm", "gcm"]
    modes = [m for m in modes if any(r["mode"] == m for r in latest)]

    x = list(range(len(modes)))
    width = 0.8 / max(1, len(envs))

    fig = plt.figure()
    for idx, env in enumerate(envs):
        y = []
        for mode in modes:
            vals = [r["throughput_mib_s"] for r in latest if r["environment"] == env and r["mode"] == mode]
            y.append(vals[-1] if vals else 0.0)
        positions = [v - 0.4 + width / 2 + idx * width for v in x]
        plt.bar(positions, y, width=width, label=env.upper())
    plt.xticks(x, [m.upper() for m in modes])
    plt.xlabel("AES mode")
    plt.ylabel("Throughput (MiB/s)")
    plt.title("Lab 1 AES throughput by environment at largest payload")
    plt.legend()
    plt.tight_layout()
    throughput_path = output_dir / "lab1_aes_throughput_by_environment.png"
    fig.savefig(throughput_path, dpi=160)
    plt.close(fig)

    fig = plt.figure()
    for idx, env in enumerate(envs):
        y = []
        for mode in modes:
            vals = [r["mean_ms"] for r in latest if r["environment"] == env and r["mode"] == mode]
            y.append(vals[-1] if vals else 0.0)
        positions = [v - 0.4 + width / 2 + idx * width for v in x]
        plt.bar(positions, y, width=width, label=env.upper())
    plt.xticks(x, [m.upper() for m in modes])
    plt.xlabel("AES mode")
    plt.ylabel("Mean latency (ms/op)")
    plt.title("Lab 1 AES latency by environment at largest payload")
    plt.legend()
    plt.tight_layout()
    latency_path = output_dir / "lab1_aes_latency_by_environment.png"
    fig.savefig(latency_path, dpi=160)
    plt.close(fig)

    return [throughput_path, latency_path]

def main():
    parser = argparse.ArgumentParser(description="Generate Lab 1 benchmark summary and environment comparison plots.")
    parser.add_argument("--input-dir", required=True, type=Path)
    parser.add_argument("--output-dir", required=True, type=Path)
    parser.add_argument("--tables-dir", required=True, type=Path)
    args = parser.parse_args()

    rows = load_rows(args.input_dir)
    if not rows:
        raise SystemExit(f"No benchmark rows found in {args.input_dir}")

    csv_path, md_path = write_summary(rows, args.tables_dir)
    print(f"Summary CSV written to: {csv_path}")
    print(f"Summary Markdown written to: {md_path}")

    try:
        plots = make_plots(rows, args.output_dir)
    except ModuleNotFoundError as exc:
        if exc.name == "matplotlib":
            print("matplotlib is not installed; summary tables were generated but plots were skipped.")
            return
        raise

    for path in plots:
        print(f"Plot written to: {path}")

if __name__ == "__main__":
    main()
