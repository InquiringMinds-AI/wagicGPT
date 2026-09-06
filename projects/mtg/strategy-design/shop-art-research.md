# Shop art research — booster pack images and set symbols

*Survey, 2026-09-05. No code written, nothing downloaded in bulk; every source below was
probed with single requests at >=3 s/host. Sample images fetched for measurement only.*

## 0. The pattern this has to mirror

Card art is **deferred, user-side, keyed by set code**, and the engine already searches the
user tree before the shipped `Res/` tree:

- `JFileSystem::openForRead` tries `mUserFS` first, then `mSystemFS`
  (`JGE/src/JFileSystem.cpp:484`), and `FileExists` checks both (`:372`).
- `ResourceManagerImpl::cardFile` (`src/WResourceManager.cpp:775`) resolves
  `<SET>/<id>.jpg` by attaching `sets/<SET>/<SET>.zip` (in-zip keys `<id>.jpg` and
  `thumbnails/<id>.jpg`) and falling back to loose `sets/<SET>/<id>.jpg`.
- `ResourceManagerImpl::graphicsFile` (`:672`) resolves a plain name through
  theme -> `graphics/<name>` -> **`sets/<name>`** -> raw path. So `"<CODE>/pack.jpg"`
  already resolves to `sets/<CODE>/pack.jpg`, in the user tree or in `Res/`, with **no
  engine change** other than asking for it.
- Acquisition tooling: `tools/cardart/fetch-art.py` (Scryfall, throttled, pool) +
  `pack-*.sh` (ImageMagick resize + STORED zip per platform). Documented for users in
  `docs/ALPHA-GUIDE.md` §4.

Corpus scale here: `bin/Res/sets/` holds **336 set directories** (plus `primitives`).

Important nuance for the new assets: the zip attach in `cardFile()` is on the *card* path
only. `graphicsFile()` never looks inside `sets/<SET>/<SET>.zip`. Two small files per set
do not need a zip anyway — **keep pack/symbol loose**.

---

## 1. Booster pack art

### Ranked sources

| # | Source | URL pattern | Coverage | Size / format | Licence & posture | Verdict |
|---|---|---|---|---|---|---|
| 1 | **Card-Forge `forge-extras`** (Forge's own booster image store) | `https://raw.githubusercontent.com/Card-Forge/forge-extras/refs/heads/main/images/boosters/<CODE>.jpg` or `<CODE>_<n>.jpg` / `.png` | **489 files, 195 set codes; 157 of our 336 set dirs.** Misses are almost all products that never had a wrapper (Commander/duel/precon sets) | legacy `192x350` JPEG (~50 KB), modern `350x665` PNG (~400 KB); 68 MB whole dir | repo licence GPL-3.0 (nominal — it does not and cannot grant rights in WotC pack art). README: *"not intended for this data to be downloaded manually from this github"* | **Recommended primary.** Only source that is set-code-keyed, complete-ish, live (pushed 2026-09-03), and free of a paywall/API key. Fetch is 1 request per set, same shape as `fetch-art.py`. |
| 2 | **mtg.wiki** (the community MTG Wiki — now *operated by Scryfall*) | API `https://mtg.wiki/api.php?action=query&prop=imageinfo&titles=File:<...>` -> direct `https://files.mtg.wiki/<Title_With_Underscores>` | ~252 files whose title contains "Booster"; only **31** follow `"<CODE> Play Booster.png"`. Older sets are `"<CODE> booster.jpg"`, `"Antiquities booster.jpg"`, `"Alpha booster.jpg"` — **no reliable set-code key** | large, e.g. `BLB_Play_Booster.png` 900x1637, 1.8 MB | content Scryfall may lawfully license is **CC BY-NC-SA 4.0**; WotC material stays WotC's. `robots.txt` **Disallows `/api.php`** for all agents | **Secondary / gap-filler.** Best *quality*, worst *addressability*. Needs a hand-built title map, and the robots line means no automated API sweep — resolve titles once, offline, then fetch files. |
| 3 | **TCGplayer CDN** via MTGJSON ids | `SetList.json` -> `sealedProduct[category=="booster_pack"].identifiers.tcgplayerProductId` -> `https://tcgplayer-cdn.tcgplayer.com/product/<id>_in_1000x1000.jpg` (also `https://product-images.tcgplayer.com/fit-in/437x437/<id>.jpg`) | 492 booster_pack entries, **189 sets** with a tcgplayer id | 1000x1000 and 437-fit JPEG, verified 200 | no licence grant; storefront product photography, marketplace ToS (`robots.txt` Crawl-Delay 10, no explicit image terms) | **Not recommended.** Photos of packs on white, not wrapper art; commercial-marketplace assets with no permission story. Keep only as a last-resort manual fill. |
| 4 | **Scryfall** | — | — | — | — | **Does not host pack images.** A set object's only image field is `icon_svg_uri` (fields: `arena_code, card_count, code, digital, foil_only, icon_svg_uri, id, mtgo_code, name, ..., tcgplayer_id`). |
| 5 | **archive.org** | search API | nothing usable — hits are unboxing *videos* | — | — | **Rule out.** |
| 6 | **Community archive (slightlymagic.net thread 9825)** | Google Drive link posted 04/2022 | the original Forge collection | — | — | **Dead** — later posts in the same thread report the link invalid. Its content is what now lives in `forge-extras`. |
| 7 | **mtg.fandom.com** | MediaWiki API | search returns nothing for `intitle:booster` in NS6; the community moved to mtg.wiki in 2024 | — | Fandom ToS + hashed `static.wikia.nocookie.net` paths | **Rule out.** |

### Notes
- Multiple wrapper arts per set are real and Forge encodes them exactly as we would want:
  `ZEN_1..ZEN_5`. Keep the index: `pack.jpg` = art 1, `pack2.jpg`, `pack3.jpg`…
- 489 files / 68 MB total is small enough that a user can take the whole set of boosters in
  one short run — unlike the 12 GB card-art pool. That changes the UX: "fetch pack art" can
  reasonably be one button with no per-set selection.

---

## 2. Set symbols

| # | Source | URL pattern | Coverage | Format | Licence | Verdict |
|---|---|---|---|---|---|---|
| 1 | **Scryfall set icons** | `https://api.scryfall.com/sets` (one request, 1049 sets) -> `icon_svg_uri` = `https://svgs.scryfall.io/sets/<lowercase code>.svg?<ts>`; **the `?ts` is optional — bare URL returns 200** | 1049 sets; **305 of our 336 dirs match case-insensitively**; the 31 misses are Wagic legacy codes (`RV`, `NMS`, `MRQ`, `FVD`, `EVK`, the `xVy` duel-deck codes, `PDS`, `PLIST`…) | monochrome SVG, single `<path>`, `viewBox 0 0 848 1024`, ~775 bytes | WotC Fan Content Policy; Scryfall's image rules (no watermarking/distortion, no paywalling, must add value, attribution of source) | **Recommended primary** — freshest, widest, one index request, tiny payloads. |
| 2 | **Keyrune** (Andrew Gioia) | font glyphs + `https://raw.githubusercontent.com/andrewgioia/keyrune/master/svg/<code>.svg`; jsDelivr CDN for the font | 425 CSS codes / **434 SVGs**; **270 of our 336** | SVG + TTF/WOFF2 font; rarity styling is CSS (`ss-common/uncommon/rare/mythic`, `ss-grad`), the glyph itself is monochrome | **glyphs GPL-3.0, font files SIL OFL 1.1**, hand-redrawn under the WPN Marketing Materials Policy; attribution appreciated, not required | **Recommended fallback *and* the only shippable option.** OFL/GPL means we could legally put `keyrune.ttf` (or pre-rendered PNGs) **in the repo/release** — no user fetch at all. Coverage is the tradeoff. |
| 3 | **MTGJSON `SetList.json`** | `https://mtgjson.com/api/v5/SetList.json` (11.6 MB, 869 sets) | every set carries **`keyruneCode`**; 527 sets have `code != keyruneCode` | JSON | free data project | **Recommended as the alias table**, not as an image source. It is exactly the "which glyph does this printing use" map, and it also carries `sealedProduct` (see pack art #3). |
| 4 | Set symbol PNGs on mtg.wiki | `File:<CODE> expansion symbol.png` -> `https://files.mtg.wiki/<CODE>_expansion_symbol.png` | 117 hits | PNG 800x800 | CC BY-NC-SA 4.0 where licensable | Useful as a **raster** stopgap for a handful of sets Scryfall/Keyrune miss; robots caveat as above. |

### Raster route for PSP / Vita (no SVG renderer)
Two options, both verified feasible:

1. **Offline rasterization in the packers (recommended).** `pack-*.sh` already require
   ImageMagick; `magick -background none <code>.svg -resize 64x64 symbol.png` works today on
   the Scryfall SVGs (verified: 10E -> 53x64 PNG, 953 bytes). `rsvg-convert` is the same job
   without ImageMagick's delegate. Symbols are single-path monochrome, so they downscale
   cleanly and rarity colour can be applied at render time by tinting the quad, or baked per
   rarity into 4 PNGs if tinting is inconvenient on a given platform.
2. **In-engine rasterization** — `nanosvg` (single-header C, zlib licence) would let the
   engine consume `.svg` directly on PSP/Vita. Only worth it if we ever want arbitrary
   user-supplied symbols; otherwise it is new code for no user-visible gain.

Since a symbol is ~1 KB as SVG and ~1 KB as a 64 px PNG, **ship/fetch PNG everywhere** and
skip SVG on-device entirely.

---

## 3. Recommended acquisition design

Mirror `tools/cardart/` exactly. New tool `tools/shopart/fetch-shopart.py`, same manners
(explicit User-Agent, >=120 ms throttle, `misses.txt`, `--dry-run`, set codes as argv).

**On-disk layout** (loose, no zip — `graphicsFile()` finds these unchanged):

```
User/sets/<CODE>/pack.jpg      # booster wrapper, art index 1   (desktop ~350x665)
User/sets/<CODE>/pack2.jpg     # optional additional wrapper arts
User/sets/<CODE>/symbol.png    # set symbol, transparent, square (see per-platform sizes)
```

Engine-side lookup is then `RetrieveTempQuad("<CODE>/pack.jpg")` /
`("<CODE>/symbol.png")` — one string, no new resolver.

**Per-platform sizes** (add to the existing `pack-*.sh` size ladder):

| platform | pack.jpg | symbol.png |
|---|---|---|
| desktop / Windows | source (350x665 / 192x350) | 64x64 |
| Android | 300x570 | 64x64 |
| Vita | 240x456 | 48x48 |
| PSP | 160x304 | 32x32 |

**Pipeline**

1. Fetch `https://api.scryfall.com/sets` once -> `code -> icon_svg_uri` map;
   fetch MTGJSON `SetList.json` once -> `code -> keyruneCode` alias map.
2. Per set: symbol = Scryfall SVG, else Keyrune SVG by `keyruneCode`, else hand alias
   (~30 Wagic legacy codes: `RV->3ED`, `NMS->NEM`, `MRQ->MMQ`, `FVD->DRB`, the `xVy`
   duel decks -> their `DDx` code, …). Rasterize to `symbol.png` locally.
3. Per set: pack = `forge-extras/images/boosters/<CODE>*.{jpg,png}` (directory listing via
   one GitHub contents call, or a checked-in manifest so the tool needs no API); miss ->
   `misses.txt`, engine falls back to a generic wrapper.
4. `pack-*.sh` resize both into the platform trees alongside the card-art zips.

Because the whole booster corpus is 68 MB and the whole symbol corpus is well under 1 MB,
a "fetch shop art" run is minutes, not hours — worth surfacing as its own step in
ALPHA-GUIDE §4 rather than a footnote to card art.

---

## 4. Copyright notes

- **Booster wrapper art is Wizards' copyright**, in exactly the same class as the card scans
  the project already refuses to ship. Nothing here changes the standing rule: the repo and
  every release artifact stay free of WotC-derived imagery; the *user* fetches, for personal
  use, on their own machine. The precedent is already documented in README ("This repository
  ships no Wizards of the Coast card art") and ALPHA-GUIDE §4.
- **The card-scan precedent transfers, but not perfectly.** Card art has an explicit
  community-blessed channel: Scryfall serves it *under the WotC Fan Content Policy* and
  publishes terms we can point at. Pack art has **no such channel** — `forge-extras` is
  another fan project's asset store with no stated permission over the images and a README
  asking people not to pull from it by hand. Two consequences:
  - the fetch tool should be polite and *low-volume by default* (one set at a time is the
    natural UX anyway), and identify itself honestly in its User-Agent;
  - if the owner wants a stronger footing, the right move is to ask the Forge maintainers
    directly whether a per-user fetch against `forge-extras` is acceptable, and/or to mirror
    only what a user asks for rather than mirroring the directory.
- **Set symbols are the exception worth using.** Keyrune's glyphs are *hand-redrawn* and
  distributed under GPL-3.0 (icons) / SIL OFL 1.1 (fonts), explicitly for identifying WotC
  products under the WPN Marketing Materials Policy. That makes a Keyrune-derived
  `symbol.png` (or the bundled TTF) the one shop-art asset that could legitimately **ship in
  the box**. Scryfall's icons are the better dataset but come with Fan-Content framing and
  Scryfall's image rules, so they belong on the user-fetch side.
- Set symbols are also **trademarks**; use is identification-of-product, which is what both
  Keyrune's licence text and the WPN policy contemplate. Keep them presented as set
  identifiers, never as our own marks.
- Standing disclaimer already in README (unaffiliated, MTG is a WotC trademark) covers the
  new assets; no new legal text needed, but the ALPHA-GUIDE section should say plainly that
  pack art is WotC's and fetched per-user.

---

## 5. Open questions

1. **Ship Keyrune, or fetch symbols?** OFL/GPL permits shipping ~500 KB of PNGs (or the TTF)
   for 270/336 sets and removes a whole fetch step. Do we want the box bigger and the setup
   simpler, or keep the "no third-party art in the release" line absolutely clean?
2. **Ask Forge?** A short message to the Card-Forge maintainers would settle the posture on
   `forge-extras` fetching. Owner's call — external-project contact.
3. **The ~30 legacy set codes.** Wagic's `RV`, `NMS`, `MRQ`, `FVD/FVE/FVL/FVR`, `xVy` duel
   decks, `PDS`, `PLIST`, `HTR*`, `PI13/PI14` predate/diverge from Scryfall+MTGJSON codes. A
   hand alias table is ~30 lines and fixes symbols *and* would fix any future per-set asset —
   worth doing once, in the repo, as data.
4. **Where does the art actually appear?** `GameStateShop` currently draws generic
   `shop.jpg`/`pspshop.jpg` and `Res/packs/*.txt` are colour/theme packs, not per-set
   boosters. Showing per-set wrappers implies shop content keyed by set — a design question
   ahead of the art question.
5. **Rarity colouring of symbols**: tint at draw time (one PNG per set) or bake four PNGs per
   set? Tinting is 4x smaller and matches how Keyrune itself does it (monochrome glyph +
   colour), but needs a coloured-quad draw path on PSP.
6. **Sets with several wrappers** (`ZEN_1..5`): pick one deterministically, or rotate per
   pack opened? The latter is charming and free once the files are there.
7. **Missing-pack fallback**: 179 of 336 sets will have no wrapper. A generic wrapper tinted
   by set colour identity, or the set symbol on a plain wrapper, both work — decide before
   the fetch tool, since it changes what "a miss" costs.
