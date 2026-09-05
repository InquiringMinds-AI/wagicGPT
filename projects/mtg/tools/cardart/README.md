# cardart — card-art res-set toolkit

One pipeline: **fetch once into a local pool, then pack per platform.**

```
./fetch-art.py                 # fill/refresh the pool from Scryfall (skip-existing)
./pack-psp.sh                  # -> ./wagic-card-packs/psp/sets/<SET>/<SET>.zip
./pack-vita.sh                 #    same layout, per platform
./pack-android.sh
./pack-windows.sh
./pack-linux.sh
```

Requirements: `python3` (stdlib only) for the fetcher; `bash`, ImageMagick
(`magick`) and `zip` for the packers.

Each pack zip carries full art (`<id>.jpg`) **and** thumbnails
(`thumbnails/<id>.jpg`) and drops into the platform's User dir as
`sets/<SET>/<SET>.zip` — the engine reads art from inside the zip with no
further setup. Thumbnails are always derived from the full-size pool art.

| platform | full art | thumb | why |
|---|---|---|---|
| psp | fit 180x250 q85 | 64x93 q85 | pow2 texture buckets: 256 KB / 32 KB per card |
| vita | fit 360x514 q88 | 128x186 q85 | pilot-measured on set 10E |
| android | fit 488x680 q88 | 114x166 q85 | high-dpi screen, roomy storage |
| windows | pool copy | 114x166 q85 | no recompress loss on desktop |
| linux | pool copy | 114x166 q85 | ditto |

- Pool location: auto-detected at `projects/mtg/bin/User/sets[/sets.hidden]`
  relative to the repo root, or point anywhere with `--pool` (fetcher) /
  `POOL_BASE` (packers). Layout: `<SET>/<id>.jpg`, full size.
- Fetch source: Scryfall (per the WotC Fan Content Policy), by multiverse id
  from `Res/sets/<SET>/_cards.dat`, `image_uris.large`, throttled
  >=120 ms/request — leave the throttle alone, it's the polite minimum.
  Foreign-only ids borrow the English printing by name; cards with no English
  printing on Scryfall are skipped (the engine renders a text frame).
  Misses are listed in `misses.txt` in the pool.
- Id->URL map: a local cache (`_bulk_map_large.json`) maps multiverse ids to
  image URLs in bulk. Without it the fetcher resolves ids one request at a
  time, which works but is slower; `--rebuild-map` streams Scryfall's bulk
  data file (~2 GB, not kept) to build it — worth it for a full-pool run,
  and needed again only when new sets appear.
- Useful env for packers: `SETS="10E TSP"` (whitelist), `POOL_BASE`/`SRC`,
  `OUT`, `JOBS`, `FULL_GEOM`/`FULL_Q`/`THUMB_GEOM`/`THUMB_Q`. Already-built
  set zips are skipped — delete a zip to rebuild it.
- Getting packs onto a device: copy each `sets/<SET>/<SET>.zip` into the
  platform's user tree keeping the layout — `PSP/GAME/WAGIC/User/sets/` (PSP,
  USB), `ux0:data/Wagic/sets/` (Vita, FTP), `/sdcard/Wagic/User/sets/`
  (Android). Desktop can also skip packing entirely: fetch straight into the
  install's `User/sets` and the loose jpgs render as-is.

**Card scans are WotC-derived content: fetch them for your own use. They are
never committed to this repository and never included in a release
artifact.**

## Set symbols (shop booster pack emblem)

```
./fetch-symbols.py             # Scryfall set icons (Keyrune fallback) -> User/sets/<SET>/symbol.png
./fetch-symbols.py --dry-run   # show which source each set resolves to, no downloads
```
64 px-high PNGs rasterized from the SVGs (rsvg-convert or ImageMagick). Scryfall serves the
icons under the WotC Fan Content Policy; Keyrune's glyphs are GPL-3.0 / SIL OFL. pack-*.sh
copies symbol.png into each platform's set zip. The shop draws it on the procedural booster
pack; sets without one get a generated emblem. Misses -> symbols-misses.txt.
