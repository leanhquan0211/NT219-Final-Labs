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


def load_rows(input_dir: Path):
    rows = []
    for csv_file in sorted(input_dir.glob("lab1_*.csv")):
        with csv_file.open("r", encoding="utf-8-sig", newline="") as f:
            reader = csv.DictReader(f)
            for row in reader:
                row["source_file"] = csv_file.name
                rows.append(row)
    return rows


def normalize_rows(rows):
    normalized = []
    for row in rows:
        mode = (row.get("mode") or row.get("Mode") or "unknown").strip().lower()
        size_bytes = parse_int(row.get("size_bytes") or row.get("size") or row.get("SizeBytes"))
        throughput = parse_float(row.get("throughput_mib_s") or row.get("throughput_MBps") or row.get("ThroughputMiBs"))
        mean_ms = parse_float(row.get("mean_ms") or row.get("MeanMs"))
        median_ms = parse_float(row.get("median_ms") or row.get("MedianMs"))
        stddev_ms = parse_float(row.get("stddev_ms") or row.get("StddevMs"))
        ci95_low_ms = parse_float(row.get("ci95_low_ms") or row.get("Ci95LowMs"))
        ci95_high_ms = parse_float(row.get("ci95_high_ms") or row.get("Ci95HighMs"))

        if size_bytes <= 0 or mode == "unknown":
            continue

        normalized.append({
            "mode": mode,
            "size_bytes": size_bytes,
            "size_kib": size_bytes / 1024.0,
            "throughput_mib_s": throughput,
            "mean_ms": mean_ms,
            "median_ms": median_ms,
            "stddev_ms": stddev_ms,
            "ci95_low_ms": ci95_low_ms,
            "ci95_high_ms": ci95_high_ms,
            "source_file": row.get("source_file", ""),
        })
    return normalized


def write_summary(rows, output_dir: Path):
    output_dir.mkdir(parents=True, exist_ok=True)
    summary_path = output_dir / "lab1_benchmark_summary.csv"
    fields = [
        "mode", "size_bytes", "size_kib", "mean_ms", "median_ms", "stddev_ms",
        "ci95_low_ms", "ci95_high_ms", "throughput_mib_s", "source_file"
    ]
    with summary_path.open("w", encoding="utf-8", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=fields)
        writer.writeheader()
        for row in sorted(rows, key=lambda r: (r["mode"], r["size_bytes"], r["source_file"])):
            writer.writerow(row)
    return summary_path


def make_plots(rows, output_dir: Path):
    import matplotlib.pyplot as plt

    output_dir.mkdir(parents=True, exist_ok=True)
    grouped = defaultdict(list)
    for row in rows:
        grouped[row["mode"]].append(row)

    for values in grouped.values():
        values.sort(key=lambda r: r["size_bytes"])

    fig = plt.figure()
    for mode, values in sorted(grouped.items()):
        x = [v["size_kib"] for v in values]
        y = [v["throughput_mib_s"] for v in values]
        plt.plot(x, y, marker="o", label=mode.upper())
    plt.xscale("log", base=2)
    plt.xlabel("Payload size (KiB)")
    plt.ylabel("Throughput (MiB/s)")
    plt.title("Lab 1 AES throughput by mode")
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    throughput_path = output_dir / "lab1_aes_throughput.png"
    fig.savefig(throughput_path, dpi=160)
    plt.close(fig)

    fig = plt.figure()
    for mode, values in sorted(grouped.items()):
        x = [v["size_kib"] for v in values]
        y = [v["mean_ms"] for v in values]
        plt.plot(x, y, marker="o", label=mode.upper())
    plt.xscale("log", base=2)
    plt.xlabel("Payload size (KiB)")
    plt.ylabel("Mean latency (ms/op)")
    plt.title("Lab 1 AES latency by mode")
    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    latency_path = output_dir / "lab1_aes_latency.png"
    fig.savefig(latency_path, dpi=160)
    plt.close(fig)

    fig = plt.figure()
    labels = []
    heights = []
    errors = []
    for row in sorted(rows, key=lambda r: (r["size_bytes"], r["mode"])):
        if row["size_bytes"] == max(r["size_bytes"] for r in rows):
            labels.append(row["mode"].upper())
            heights.append(row["mean_ms"])
            err_high = max(0.0, row["ci95_high_ms"] - row["mean_ms"])
            err_low = max(0.0, row["mean_ms"] - row["ci95_low_ms"])
            errors.append(max(err_low, err_high))
    if labels:
        plt.bar(labels, heights, yerr=errors, capsize=4)
        plt.xlabel("Mode")
        plt.ylabel("Mean latency (ms/op)")
        plt.title("Lab 1 AES latency with 95% CI at largest payload")
        plt.tight_layout()
        ci_path = output_dir / "lab1_aes_latency_ci_largest_payload.png"
        fig.savefig(ci_path, dpi=160)
    else:
        ci_path = None
    plt.close(fig)

    return [p for p in [throughput_path, latency_path, ci_path] if p]


def main():
    parser = argparse.ArgumentParser(description="Plot Lab 1 AES benchmark CSV files.")
    parser.add_argument("--input-dir", required=True, type=Path)
    parser.add_argument("--output-dir", required=True, type=Path)
    args = parser.parse_args()

    rows = normalize_rows(load_rows(args.input_dir))
    if not rows:
        raise SystemExit(f"No benchmark rows found in {args.input_dir}")

    summary_path = write_summary(rows, args.output_dir)
    print(f"Summary CSV written to: {summary_path}")

    try:
        plots = make_plots(rows, args.output_dir)
    except ModuleNotFoundError as exc:
        if exc.name == "matplotlib":
            print("matplotlib is not installed. Summary CSV was generated, but plots were skipped.")
            print("Install it with: python -m pip install matplotlib")
            return
        raise

    for path in plots:
        print(f"Plot written to: {path}")


if __name__ == "__main__":
    main()
