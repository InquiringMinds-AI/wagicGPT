# Wave-53 lane V — Vita vpk12: wrong card art on a transformed double-faced card

Owner report: *"when transformed, the heliod the radiant dawn in my 'aura farming' deck
displays a different untransformed card art."*

## The mechanism, in plain words

Wagic card ids **are Gatherer multiverse ids, and a transform DFC gets one id per FACE**.
In `bin/Res/sets/MOM/_cards.dat` the two faces are consecutive entries, and the back face
is marked `rarity=T`:

```
id=607029  Heliod, the Radiant Dawn    rarity=R    <- regular frame,  MOM #0017
id=607030  Heliod, the Warped Eclipse  rarity=T
...
id=610513  Heliod, the Radiant Dawn    rarity=R    <- borderless,     MOM #0293  (HIS card)
id=610514  Heliod, the Warped Eclipse  rarity=T
```

Two independent defects stacked, and each on its own would have produced a wrong picture.

### Defect 1 — the engine collapsed both printings onto one (art, not rules)

`{3}{p(U)}:flip(backside)` runs `AAFlip::resolve`, which resolves the other face **by name**:
`MTGCollection()->getCardByName("Heliod, the Warped Eclipse", setId=MOM)`. That function
returns exactly ONE printing — and because both candidates carry `rarity=T`, it takes the
token-fallback branch and returns the **lowest matching id, 607030**. `AAFlip` then does
`setMTGId(607030)`, so his borderless Heliod became the *regular* printing's back face and
`WResourceManager::RetrieveCard` asked for `MOM/607030.jpg`.

`CardGui::Render` / `CardGui::RenderBig` then made the same mistake a second time: after a
flip `card->name != card->model->data->name`, so both re-resolved the art with a **name-only**
`getCardByName(card->name)` — no set, no printing — landing on 607030 again even when the
instance's own (correct) id was already sitting right there.

Printing choice never affects RULES (both printings share one primitive, so power/toughness/
text/abilities are identical). It only decides which image file is asked for. That is why this
was invisible to the suite for as long as it has existed.

### Defect 2 — the art pack had no back-face pictures at all

`download_card_images.py`'s `--bulk` path built its id→URL map like this:

```python
if not image_uris or not multiverse_ids:
    faces = card.get("card_faces", [])
    if faces and faces[0].get("image_uris"): image_uris = faces[0]["image_uris"]
...
for mid in multiverse_ids: image_map[mid] = url
```

A transform DFC has one `image_uris` per FACE and one multiverse id per face, in the same
order — this mapped **every** id onto `card_faces[0]`, the FRONT. Measured on disk:
`MOM/607030.jpg` was byte-identical to `607029.jpg`, and `610514.jpg` byte-identical to
`610513.jpg`. **207 of the 740 real back faces in `Res/sets` were front-art duplicates.**

So even with the printing bug fixed, the transformed Heliod would have shown an untransformed
Heliod. With both bugs live it showed the OTHER printing's untransformed Heliod — a visibly
different card (regular frame / different artist / different border) — which is exactly the
"different untransformed card art" he saw.

Deployed pack confirmed identical to the local one before the fix
(`ftp://10.0.0.227:1337/ux0:/data/wagic/sets/MOM/` -> `MOM.zip 35656978`, byte-for-byte the
size of `psp-work/card-packs/vita/sets/MOM/MOM.zip`).

## The fix

**Engine (committed).**
- `MTGAllCards::getOtherFaceCard(name, setId, refId)` — new. Collects every id in `setId`
  carrying the wanted face's name and picks the one belonging to **refId's printing** via the
  pure, PARSETEST-provable `wagicPickFaceSiblingId()`: the NEAREST id in either direction
  (forward on a tie), because a DFC's two faces are adjacent entries in `_cards.dat`. Falls
  back to `getCardByName` when the set has no candidate; results cached under a
  (name,setId,refId) key so it stays cheap.
- `AAFlip::resolve` and `AATurnSide::resolve` use it for FACE swaps only (`backside`,
  `myorigname`, and the turn-back-to-original path). A `chosenname` copy effect is not a face
  swap and keeps the plain lookup.
- `CardGui::Render` / `CardGui::RenderBig`: the instance's OWN id is authoritative when it
  already names this face — skip the name-only re-lookup entirely in that case, and keep it
  only for copy/clone effects where the id genuinely still points at the original card. (This
  also fixed a latent `game->` NULL deref in `Render` and made `RenderBig`'s fallback honour
  its `thumb` argument, which it had been ignoring.)

**Downloader (committed).** `download_bulk_data` now pairs `multiverse_ids[i]` with
`card_faces[i]["image_uris"]` when the counts match and every face has its own art. Cards with
one shared image (meld backs, split/adventure) keep the old behaviour.

**Art (regenerated, NOT uploaded).**
- `strategy-design/wave53/fix-dfc-backface-art.py` re-fetched each wrong/missing back face by
  its OWN multiverse id — Scryfall's per-multiverse-id image endpoint returns the correct face
  for a back-face id, verified by eye on 610514 before writing anything. Kamigawa flip cards
  are excluded (positive `rarity=T` too, but synthetic ids `front*100+11`, and their
  front-duplicate image is CORRECT — one physical card).
  **Result: 207 repaired, 103 misses** (ids Gatherer never assigned — INR/MH3/TDM/YMID/MOM
  incubator tokens; those were already absent before and still render the honest no-art frame).
- `strategy-design/wave53/patch-vita-cardpack.sh` spliced the 207 into the existing STORED
  zips at the exact `make-vita-cards.sh` geometry rather than rebuilding 336 sets from 12 GB.
  **14 zips changed** (CM2 EMN ISD KHM MH3 MID MOC MOM NEO ORI STX TDM VOW ZNR); MOM.zip is
  still 939 entries (replacements, not additions), now 35569993 bytes. The PSP pack was
  patched the same way at PSP geometry.

### Deploy (orchestrator's call — I did NOT upload)

```
/home/magi/Projects/wagicGPT/psp-work/scripts/upload-vita-cards.sh 10.0.0.227 1337
```
It is resumable and skips any set whose remote size already matches, so it will push exactly
the 14 changed zips. Nothing else on the device is touched. Or, for MOM alone:
```
curl -T /home/magi/Projects/wagicGPT/psp-work/card-packs/vita/sets/MOM/MOM.zip \
     "ftp://10.0.0.227:1337/ux0:/data/Wagic/sets/MOM/MOM.zip"
```
The engine change also needs a VPK rebuild to reach the console — **owner-initiated only**.
The art fix alone (no rebuild) already stops the *front-art-on-the-back-face* half; the
*wrong-printing* half needs the binary.

## Falsifiable prediction

On the next Vita build with both halves deployed, activating `{3}{U/P}` on the borderless
Heliod (MOM #0293) shows the **blue Phyrexian God, borderless #0293** back face — not the red
#0017 one, and not the white front. Turning it back shows #0293's front, not #0017's.

## Gate

Worktree `w53-V`, binary rebuilt from a clean link.

| gate | result |
|---|---|
| PARSETEST | **2258 passed, 0 failed** (2249 baseline + 9 new `#W53-V` checks) |
| suite | **1215 tests, 2 failed** — `lifeline.txt` + `merrow_reejerey.txt`, the known concurrency-only pair — **46 AI / 0**, **0 timeouts** |

Both under `MemoryMax=4G MemorySwapMax=0`; no kill under the cap. Eight existing transform
fixtures (`brutal_cathar_daybound_transform`, `delver_of_secrets_*`, `docent_of_perfection_*`,
`grizzled_angler_activated_transform`, `reckless_waif_cast_transform`,
`tovolars_huntmaster_cast_from_hand`, `ludevics_test_subject_four_counters`) exercise the new
`getOtherFaceCard` call sites and stayed green.

The PARSETEST block caught a real error in my first draft: a forward-only sibling rule looked
right on front→back and got back→front wrong the moment a second printing existed further up
the file (`607030` jumped to `610513` instead of `607029`). The nearest-in-either-direction
rule is the shipped one.

## What I did NOT verify

- **I cannot see the Vita screen.** Nothing here is confirmed on the console; the deployed
  pack is unchanged (I did not upload) and no VPK was built.
- **The desktop cannot show it either.** `bin/User/sets` has been renamed to `sets.hidden`, so
  the desktop build has no card art at all — every card renders the generated no-art frame.
  A windowed screenshot would have proved nothing, so I did not take one.
- **`MTGAllCards::getOtherFaceCard`'s live database scan is not directly observed.** Its pure
  core (`wagicPickFaceSiblingId`) is pinned on the real MOM ids in PARSETEST, and the ids /
  rarities / ordering it depends on were read straight out of `MOM/_cards.dat` — but I never
  watched the assembled function return 610514 at runtime. PARSETEST runs before the card
  database loads, and the test suite deliberately SKIPS `setMTGId` on flip
  (`playMode != MODE_TEST_SUITE`), so neither harness can assert a post-flip mtgid today. A
  real pin would need a new `assertmtgid` suite command plus lifting that skip — out of scope
  here, and worth docketing.
- **The 103 back faces that 404'd are still missing art**, exactly as before this lane. They
  are not a regression, but they will render the no-art frame on the console.
- **The 430 back faces already distinct from their front were assumed correct** and not
  re-downloaded.
