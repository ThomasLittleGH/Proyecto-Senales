from pathlib import Path
import argparse
import csv
import math
import re

import matplotlib.pyplot as plt


def block_number(path):
    match = re.search(r"_(\d+)\.csv$", path.name)
    return int(match.group(1)) if match else -1


def load_spectrum(path):
    freqs = []
    mags = []

    with path.open(newline="") as file:
        reader = csv.DictReader(file)
        for row in reader:
            freq = float(row["freq"])
            mag = float(row["mag"])
            freqs.append(freq)
            mags.append(mag)

    half = len(freqs) // 2 + 1
    return freqs[:half], mags[:half]


def mag_to_db(mag):
    return 20.0 * math.log10(max(mag, 1e-12))


def main():
    parser = argparse.ArgumentParser(description="Plot exported Fourier block spectrograms.")
    parser.add_argument("--dir", default="export", help="Folder with fourier_*.csv files.")
    parser.add_argument("--pattern", default="fourier_*.csv", help="CSV pattern to load.")
    parser.add_argument("--no-show", action="store_true", help="Save PNG files without opening windows.")
    args = parser.parse_args()

    export_dir = Path(args.dir)
    files = sorted(export_dir.glob(args.pattern), key=block_number)

    if not files:
        raise SystemExit(f"No files found in {export_dir / args.pattern}")

    spectra = [load_spectrum(path) for path in files]
    min_bins = min(len(freqs) for freqs, _ in spectra)

    freqs = spectra[0][0][:min_bins]
    mags_db = [
        [mag_to_db(mag) for mag in mags[:min_bins]]
        for _, mags in spectra
    ]

    fig, ax = plt.subplots(figsize=(11, 6))
    image = ax.imshow(
        mags_db,
        aspect="auto",
        origin="lower",
        extent=[freqs[0], freqs[-1], 0, len(files) - 1],
        cmap="magma",
    )
    ax.set_title("Spectrogram by Fourier block")
    ax.set_xlabel("Frequency (Hz)")
    ax.set_ylabel("Block")
    fig.colorbar(image, ax=ax, label="Magnitude (dB)")
    fig.tight_layout()
    fig.savefig(export_dir / "spectrogram.png", dpi=160)

    cols = min(4, len(files))
    rows = math.ceil(len(files) / cols)
    fig2, axes = plt.subplots(rows, cols, figsize=(4 * cols, 2.6 * rows), squeeze=False)

    for ax, path, (_, mags) in zip(axes.flat, files, spectra):
        ax.plot(freqs, [mag_to_db(mag) for mag in mags[:min_bins]])
        ax.set_title(path.stem)
        ax.set_xlabel("Hz")
        ax.set_ylabel("dB")
        ax.grid(True, alpha=0.25)

    for ax in axes.flat[len(files):]:
        ax.axis("off")

    fig2.tight_layout()
    fig2.savefig(export_dir / "spectra_by_block.png", dpi=160)

    if not args.no_show:
        plt.show()


if __name__ == "__main__":
    main()
