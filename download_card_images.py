#!/usr/bin/env python3
"""
Download card art images for Wagic from Scryfall.

Parses all _cards.dat files to find multiverse IDs, then downloads
card images from Scryfall in the directory structure Wagic expects:
    <output>/sets/<SETCODE>/<MTGID>.jpg

Usage:
    python3 download_card_images.py [options]

    # Download all card images (normal quality, ~488x680)
    python3 download_card_images.py -o ./card_images

    # Download smaller images (better for Vita memory)
    python3 download_card_images.py -o ./card_images --size small

    # Download only specific sets
    python3 download_card_images.py -o ./card_images --sets 10E,M15,LEA

    # Resume a partial download (skips existing files)
    python3 download_card_images.py -o ./card_images --resume

    # Use bulk data for faster download (downloads 300MB index first)
    python3 download_card_images.py -o ./card_images --bulk

    # Then build the full VPK:
    #   cmake ... -DCARD_IMAGES_DIR=./card_images && make vpk-full
"""

import argparse
import json
import os
import sys
import threading
import time
import urllib.request
import urllib.error
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed
from pathlib import Path

# Force unbuffered output so progress is visible in real time
sys.stdout.reconfigure(line_buffering=True) if hasattr(sys.stdout, 'reconfigure') else None

# Scryfall direct image endpoint (returns JPEG via 302 redirect)
# https://scryfall.com/docs/api/cards/collector
SCRYFALL_IMAGE_URL = (
    "https://api.scryfall.com/cards/multiverse/{}?format=image&version={}"
)
SCRYFALL_BULK_URL = "https://api.scryfall.com/bulk-data"

SIZE_MAP = {
    "small": "small",      # ~146x204  (~15 KB each, ~1.1 GB total)
    "normal": "normal",    # ~488x680  (~80 KB each, ~6 GB total)
    "large": "large",      # ~672x936  (~150 KB each, ~11 GB total)
}

USER_AGENT = "WagicVitaPort/1.0 (card image downloader; github.com/WagicProject/wagic)"

# Thread-safe rate limiter to stay under Scryfall's 10 req/s limit
_rate_lock = threading.Lock()
_last_request_time = 0.0
RATE_LIMIT_INTERVAL = 0.1  # 100ms between requests = 10 req/s max


def _rate_limit():
    """Ensure at least RATE_LIMIT_INTERVAL seconds between API requests."""
    global _last_request_time
    with _rate_lock:
        now = time.monotonic()
        wait = RATE_LIMIT_INTERVAL - (now - _last_request_time)
        if wait > 0:
            time.sleep(wait)
        _last_request_time = time.monotonic()


def parse_cards_dat(sets_dir):
    """Parse all _cards.dat files and return {set_code: [list of ids]}."""
    sets_dir = Path(sets_dir)
    cards_by_set = defaultdict(list)

    for dat_file in sorted(sets_dir.glob("*/_cards.dat")):
        set_code = dat_file.parent.name
        with open(dat_file, "r", encoding="utf-8", errors="replace") as f:
            for line in f:
                line = line.strip()
                if line.startswith("id="):
                    try:
                        card_id = int(line[3:])
                        cards_by_set[set_code].append(card_id)
                    except ValueError:
                        pass

    return dict(cards_by_set)


def get_image_filename(card_id):
    """Return the filename Wagic expects for a given card ID."""
    if card_id < 0:
        return f"{abs(card_id)}t.jpg"  # Token
    return f"{card_id}.jpg"


def download_direct(card_id, filepath, size, retries=5):
    """Download card image using Scryfall's direct image redirect API."""
    mid = abs(card_id)
    version = SIZE_MAP.get(size, "normal")
    url = SCRYFALL_IMAGE_URL.format(mid, version)

    for attempt in range(retries):
        _rate_limit()
        try:
            req = urllib.request.Request(url, headers={
                "User-Agent": USER_AGENT,
                "Accept": "image/jpeg,image/*",
            })
            with urllib.request.urlopen(req, timeout=30) as resp:
                data = resp.read()
                if len(data) < 100:
                    return False
                os.makedirs(os.path.dirname(filepath), exist_ok=True)
                with open(filepath, "wb") as f:
                    f.write(data)
                return True
        except urllib.error.HTTPError as e:
            if e.code == 404:
                return False  # Card not in Scryfall
            if e.code == 429:
                # Rate limited. Back off exponentially.
                wait = 2 ** attempt
                time.sleep(wait)
                continue
            if attempt < retries - 1:
                time.sleep(1 + attempt)
        except (urllib.error.URLError, OSError, TimeoutError):
            if attempt < retries - 1:
                time.sleep(1 + attempt)
    return False


def download_from_url(url, filepath, retries=5):
    """Download image from a known URL (used with bulk data mode)."""
    for attempt in range(retries):
        try:
            req = urllib.request.Request(url, headers={"User-Agent": USER_AGENT})
            with urllib.request.urlopen(req, timeout=30) as resp:
                data = resp.read()
                os.makedirs(os.path.dirname(filepath), exist_ok=True)
                with open(filepath, "wb") as f:
                    f.write(data)
                return True
        except urllib.error.HTTPError as e:
            if e.code == 429:
                time.sleep(2 ** attempt)
                continue
            if attempt < retries - 1:
                time.sleep(1 + attempt)
        except (urllib.error.URLError, OSError, TimeoutError):
            if attempt < retries - 1:
                time.sleep(1 + attempt)
    return False


def download_bulk_data(size, cache_dir=None):
    """Download Scryfall bulk data to build multiverse_id -> image URL map.

    Caches the image_map to disk so subsequent --resume runs skip the 500MB download.
    """
    version = SIZE_MAP.get(size, "normal")

    # Check for cached map
    if cache_dir:
        cache_file = Path(cache_dir) / f"_bulk_map_{version}.json"
        if cache_file.exists():
            age_hours = (time.time() - cache_file.stat().st_mtime) / 3600
            if age_hours < 24:
                print(f"Loading cached bulk map ({age_hours:.1f}h old)...")
                with open(cache_file, "r") as f:
                    image_map = {int(k): v for k, v in json.load(f).items()}
                print(f"  Loaded {len(image_map)} multiverse ID mappings from cache")
                return image_map
            else:
                print(f"  Cache is {age_hours:.0f}h old, re-downloading...")

    for attempt in range(5):
        try:
            print("Fetching Scryfall bulk data catalog...")
            req = urllib.request.Request(SCRYFALL_BULK_URL, headers={
                "User-Agent": USER_AGENT, "Accept": "application/json",
            })
            with urllib.request.urlopen(req, timeout=30) as resp:
                catalog = json.loads(resp.read())
            break
        except (urllib.error.HTTPError, urllib.error.URLError) as e:
            wait = 2 ** (attempt + 1)
            print(f"  Catalog fetch failed ({e}), retrying in {wait}s...")
            time.sleep(wait)
    else:
        raise RuntimeError("Could not fetch bulk data catalog after 5 attempts")

    bulk_url = None
    for item in catalog.get("data", []):
        if item.get("type") == "default_cards":
            bulk_url = item.get("download_uri")
            break

    if not bulk_url:
        raise RuntimeError("Could not find default_cards bulk data URL")

    for attempt in range(3):
        try:
            print(f"Downloading card database (~500 MB, this takes a few minutes)...")
            req = urllib.request.Request(bulk_url, headers={"User-Agent": USER_AGENT})
            with urllib.request.urlopen(req, timeout=300) as resp:
                data = resp.read()
                print(f"  Downloaded {len(data) / 1024 / 1024:.1f} MB")
                bulk_cards = json.loads(data)
            break
        except (urllib.error.HTTPError, urllib.error.URLError) as e:
            wait = 10 * (attempt + 1)
            print(f"  Bulk download failed ({e}), retrying in {wait}s...")
            time.sleep(wait)
    else:
        raise RuntimeError("Could not download bulk data after 3 attempts")

    image_map = {}
    for card in bulk_cards:
        multiverse_ids = card.get("multiverse_ids", [])
        image_uris = card.get("image_uris")

        if not image_uris or not multiverse_ids:
            faces = card.get("card_faces", [])
            if faces and faces[0].get("image_uris"):
                image_uris = faces[0]["image_uris"]
            else:
                continue

        url = image_uris.get(version) or image_uris.get("normal")
        if url:
            for mid in multiverse_ids:
                image_map[mid] = url

    print(f"  Mapped {len(image_map)} multiverse IDs to image URLs")

    # Cache to disk
    if cache_dir:
        os.makedirs(cache_dir, exist_ok=True)
        with open(cache_file, "w") as f:
            json.dump({str(k): v for k, v in image_map.items()}, f)
        print(f"  Cached map to {cache_file}")

    return image_map


def main():
    parser = argparse.ArgumentParser(
        description="Download card art images for Wagic from Scryfall"
    )
    parser.add_argument(
        "-o", "--output", default="./card_images",
        help="Output directory (default: ./card_images)"
    )
    parser.add_argument(
        "--wagic-dir", default=None,
        help="Path to Wagic source (default: auto-detect from script location)"
    )
    parser.add_argument(
        "--size", choices=["small", "normal", "large"], default="normal",
        help="Image size: small (~15KB), normal (~80KB), large (~150KB) per card"
    )
    parser.add_argument(
        "--sets", default=None,
        help="Comma-separated set codes to download (default: all)"
    )
    parser.add_argument(
        "--resume", action="store_true",
        help="Skip files that already exist"
    )
    parser.add_argument(
        "--workers", type=int, default=8,
        help="Parallel download threads (default: 8)"
    )
    parser.add_argument(
        "--bulk", action="store_true",
        help="Download Scryfall bulk data first for faster image URL resolution "
             "(downloads ~300 MB JSON index, then images use direct URLs)"
    )
    args = parser.parse_args()

    # Find Wagic source directory
    wagic_dir = Path(args.wagic_dir) if args.wagic_dir else Path(__file__).resolve().parent
    sets_dir = wagic_dir / "projects" / "mtg" / "bin" / "Res" / "sets"
    if not sets_dir.is_dir():
        print(f"ERROR: Could not find sets directory at {sets_dir}")
        print("Use --wagic-dir to specify the Wagic source directory")
        sys.exit(1)

    output_dir = Path(args.output)

    # Parse all _cards.dat files
    print(f"Parsing card data from {sets_dir}...")
    cards_by_set = parse_cards_dat(sets_dir)

    # Filter to requested sets
    if args.sets:
        requested = set(s.strip().upper() for s in args.sets.split(","))
        cards_by_set = {k: v for k, v in cards_by_set.items() if k in requested}

    total_cards = sum(len(ids) for ids in cards_by_set.values())
    total_sets = len(cards_by_set)
    print(f"Found {total_cards} cards across {total_sets} sets")

    if total_cards == 0:
        print("No cards to download!")
        sys.exit(0)

    # Build download plan (skip existing if --resume)
    downloads = []
    skipped = 0
    for set_code, card_ids in sorted(cards_by_set.items()):
        set_out = output_dir / "sets" / set_code
        for card_id in card_ids:
            filename = get_image_filename(card_id)
            filepath = set_out / filename
            if args.resume and filepath.exists() and filepath.stat().st_size > 100:
                skipped += 1
                continue
            downloads.append((card_id, set_code, str(filepath)))

    if skipped:
        print(f"Resuming: skipping {skipped} existing files")
    if not downloads:
        print("All images already downloaded!")
        sys.exit(0)

    est_kb = {"small": 15, "normal": 80, "large": 150}[args.size]
    est_mb = len(downloads) * est_kb / 1024
    print(f"Will download {len(downloads)} images (~{est_mb:.0f} MB at {args.size} quality)")

    # Optionally load bulk data for direct image URLs
    image_map = {}
    if args.bulk:
        try:
            image_map = download_bulk_data(args.size, cache_dir=str(output_dir))
        except Exception as e:
            print(f"WARNING: Bulk data failed ({e}), using direct API instead")

    # Split downloads into CDN (bulk-mapped) and API (fallback) lists
    cdn_downloads = []
    api_downloads = []
    for item in downloads:
        card_id, set_code, filepath = item
        mid = abs(card_id)
        url = image_map.get(mid)
        if url:
            cdn_downloads.append((card_id, set_code, filepath, url))
        else:
            api_downloads.append((card_id, set_code, filepath))

    succeeded = 0
    failed = 0
    failed_ids = []
    start_time = time.time()

    def _run_phase(label, items, download_fn, workers, report_every=500):
        nonlocal succeeded, failed
        if not items:
            return
        phase_ok = 0
        phase_fail = 0
        phase_start = time.time()
        print(f"\n{label}: {len(items)} images with {workers} threads...")
        print("-" * 60)
        with ThreadPoolExecutor(max_workers=workers) as executor:
            futures = {executor.submit(download_fn, item): item for item in items}
            for i, future in enumerate(as_completed(futures), 1):
                ok, card_id = future.result()
                if ok:
                    succeeded += 1
                    phase_ok += 1
                else:
                    failed += 1
                    phase_fail += 1
                    failed_ids.append(card_id)
                if i % report_every == 0 or i == len(items):
                    elapsed = time.time() - phase_start
                    rate = i / elapsed if elapsed > 0 else 0
                    eta = (len(items) - i) / rate if rate > 0 else 0
                    pct = i * 100 // len(items)
                    print(
                        f"  [{pct:3d}%] {i}/{len(items)} | "
                        f"{phase_ok} ok, {phase_fail} failed | "
                        f"{rate:.1f}/s | ETA {int(eta//60)}m{int(eta%60):02d}s"
                    )
        print(f"  Phase done: {phase_ok} ok, {phase_fail} failed")

    # Phase 1: CDN downloads. No rate limit, use many workers.
    cdn_workers = max(args.workers, 50)
    _run_phase(
        "Phase 1 (CDN, fast)",
        cdn_downloads,
        lambda item: (download_from_url(item[3], item[2]), item[0]),
        cdn_workers,
        report_every=1000,
    )

    # Phase 2: API fallback. Rate-limited, fewer workers.
    api_workers = min(args.workers, 12)
    _run_phase(
        "Phase 2 (API, rate-limited)",
        api_downloads,
        lambda item: (download_direct(item[0], item[2], args.size), item[0]),
        api_workers,
        report_every=200,
    )

    elapsed = time.time() - start_time
    print("-" * 60)
    print(f"Done in {int(elapsed//60)}m {int(elapsed%60)}s")
    print(f"  Downloaded: {succeeded}")
    print(f"  Failed:     {failed}")
    print(f"  Skipped:    {skipped}")

    if failed_ids:
        fail_log = output_dir / "failed_downloads.txt"
        os.makedirs(output_dir, exist_ok=True)
        with open(fail_log, "w") as f:
            for cid in failed_ids:
                f.write(f"{cid}\n")
        print(f"  Failed IDs saved to: {fail_log}")
        print(f"  Re-run with --resume to retry only failed/missing images")

    # Calculate total size
    total_size = 0
    file_count = 0
    for dirpath, _, filenames in os.walk(output_dir / "sets"):
        for fn in filenames:
            if fn.endswith(".jpg"):
                total_size += os.path.getsize(os.path.join(dirpath, fn))
                file_count += 1

    print(f"\nTotal: {file_count} images, {total_size / 1024 / 1024:.0f} MB")
    print(f"Output: {output_dir.resolve()}")
    print(f"\nTo build the full VPK:")
    print(f"  cmake ... -DCARD_IMAGES_DIR={output_dir.resolve()}")
    print(f"  make vpk-full")


if __name__ == "__main__":
    main()
