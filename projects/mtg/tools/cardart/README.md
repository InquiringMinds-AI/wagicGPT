# cardart — card-art res-set toolkit

One pipeline: **fetch once into the master pool, then pack per platform.**

```
./fetch-art.py                 # fill/refresh the pool from Scryfall (skip-existing)
./pack-psp.sh                  # -> ~/Projects/wagic-card-packs/psp/sets/<SET>/<SET>.zip
./pack-vita.sh                 #    same layout, per platform
./pack-android.sh
./pack-windows.sh
./pack-linux.sh
```

Each pack zip carries full art (`<id>.jpg`) **and** thumbnails
(`thumbnails/<id>.jpg`) and drops into the platform's User dir as
`sets/<SET>/<SET>.zip` — the engine's AttachZipFile path finds it with no
code change. Thumbnails are always derived from the full-size pool art.

| platform | full art | thumb | why |
|---|---|---|---|
| psp | fit 180x250 q85 | 64x93 q85 | pow2 texture buckets: 256 KB / 32 KB per card |
| vita | fit 360x514 q88 | 128x186 q85 | pilot-measured on set 10E |
| android | fit 488x680 q88 | 114x166 q85 | high-dpi screen, roomy storage |
| windows | pool copy | 114x166 q85 | no recompress loss on desktop |
| linux | pool copy | 114x166 q85 | ditto |

- Pool: `projects/mtg/bin/User/sets[/sets.hidden]/<SET>/<id>.jpg` (auto-detected;
  the dev pool hides as `sets.hidden` so the dev build boots artless).
- Fetch source: Scryfall (WotC Fan Content Policy), by multiverse id from
  `Res/sets/<SET>/_cards.dat`, `image_uris.large`, throttled >=120 ms/request.
  Foreign-only ids borrow the English printing by name; no-English cards skip
  (engine text fallback). Misses land in `misses.txt`.
- Id->URL map: reuses `User/_bulk_map_large.json` from the 2026-08 campaign;
  `--rebuild-map` streams a fresh ~2 GB Scryfall bulk file (needed when new
  sets are added).
- Useful env: `SETS="10E TSP"` (whitelist), `SRC`/`OUT`, `JOBS`,
  `FULL_GEOM`/`FULL_Q`/`THUMB_GEOM`/`THUMB_Q`. Already-built set zips are
  skipped — delete a zip to rebuild it.
- Device copy: `psp-work/fix-card-zips.sh` (PSP stick), `deploy-vita.sh`/FTP
  (Vita), adb push (Android).

Supersedes `psp-work/make-psp-cards.sh` + `make-vita-cards.sh` (kept; their
headers hold the full geometry derivations).

**WOTC-derived scans: local + his devices only. Never committed, never in a
release artifact.**
