# Wave-27 deployment manifest (for the integrator -- do NOT deploy from the synthesis seat)

Corpus `matchups-20260725-193608`, binary `ffec79fd8` (-T 3000, REPPENALTY=1.05). Guides live at
`projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` (`bin/Res` is gitignored -> NEW guides need `git add -f`; existing
tracked guides need `-f` on commit too). Core prompt `bin/Res/ai/gpt/system_prompt.txt`: **NO CHANGE** (22nd
consecutive PASS, 7-of-7 seats). The wave artifact `strategy-design/wave27/strategy-writing-skill.md` is the reviewer
instrument, not a Res asset -- it is NOT deployed.

PROVENANCE: the live guides for 18/22/131/137 all carry live mtime `2026-07-25 16:49:50` (predating the 19:36
wave-27 corpus -- clean; no seat self-deployed). This wave: **18/22/131 byte-frozen** (seat file == deployed guide);
**137 revised** (one surgical convoke hunk); **93/134/136 net-new** (no prior deploy).

## Byte-verification of the seat strategy.txt files vs the LIVE deployed guides

Verified with `cmp -s`, byte sizes, a U+FFFD scan (`/usr/bin/grep -c $'\357\277\275'`, must be 0), and md5.

| deck | disposition | cmp vs live | seat bytes | live bytes | FFFD | seat md5 | action |
|------|-------------|-------------|-----------:|-----------:|:----:|----------|--------|
| 18  | byte-frozen | IDENTICAL   | 5940 | 5940 | 0 | `8efec49899015c72aba59193cab42a58` | NONE (already deployed, matches) |
| 22  | byte-frozen | IDENTICAL   | 5385 | 5385 | 0 | `5315a04536b766bf8243df1d55c19ee3` | NONE (already deployed, matches) |
| 131 | frozen (rotating OUT) | IDENTICAL | 13264 | 13264 | 0 | `8791d3ba381ef700e51b81b4c35a8e33` | NONE; keep guide, move deck131 to canary set |
| 137 | REVISED     | DIFFERS (1 hunk) | 9084 | 8491 | 0 | `8d5a54b2bf17e919f62609ee05fb5385` | **DEPLOY** seat -> live (`-f`) |
| 93  | NEW         | n/a (no live) | 5124 | -- | 0 | `1a27e8c4cc93c2884c52ee3dbc60265f` | **DEPLOY NEW** `deck93_strategy.txt` (`git add -f`) |
| 134 | NEW         | n/a (no live) | 5945 | -- | 0 | `8b5fc256e7ec75df16a2194f75725e3e` | **DEPLOY NEW** `deck134_strategy.txt` (`git add -f`) |
| 136 | NEW         | n/a (no live) | 7533 | -- | 0 | `1cc0d4e0bb6f5bfe86e050a6ea904f11` | **DEPLOY NEW** `deck136_strategy.txt` (`git add -f`) |

Live-guide md5 (for the byte-frozen integrity check): 18 `8efec49899015c72aba59193cab42a58`, 22
`5315a04536b766bf8243df1d55c19ee3`, 131 `8791d3ba381ef700e51b81b4c35a8e33` (all == seat, confirmed frozen);
137 live `957717d5e8bfdc4547b23b8f4505f471` (differs from seat -> revised, deploy the seat).

## deck137 revision -- SURGICAL verification (single hunk)

The only revised guide. `diff live seat` = ONE contiguous hunk (lines 43-49 -> 43-55). Content matches the deck137
reviewer's PROPOSAL 2 + the CONVOKE-NONRESOLUTION finding exactly:
- REPLACES the old "when 'Cast X with its convoke cost' is offered, you CAN afford it ... just pick the convoke
  option" block (which steered the model into the non-resolving path).
- WITH: PREFER the PLAIN cast whenever untapped mana covers the full cost (it is the reliable one and leaves creatures
  untapped to attack/block); choose convoke ONLY when you cannot cover the mana otherwise; PLUS a new stop-rule -- if
  a cast you picked is STILL IN HAND on your next decision (did not resolve), do NOT keep re-picking it (deploy a
  cheaper creature, attack, move on).
- Card lines re-worded from "convoke = ..." to plain-cast framing (Loxodon 4/4 body via plain cast; March bigger-X).

This is guide-legitimate (selects among offered legal options; no clamp, no faked result) and is the guide half of a
split fix: the engine half (CONVOKE-NONRESOLUTION) is the wave-28 ledger #1 and is still required for early tempo
convoke where no plain cast is affordable. Surgical = CONFIRMED; safe to deploy.

## New-guide plausibility (Step-0-bis, sizes in band, headline teaches encoded)

- **deck93** (5124 B / 74 lines, mid-band): rule-#1 "SWAMP IS YOUR LAND AND IT TAPS FOR BLACK MANA" tied explicitly
  to the mulligan (the wave-27 basic-land cross-phase witness); MULLIGAN section decides on LAND COUNT with the
  counts-as-land fact repeated inside it; combat section covers "count legal attackers from the A-lines." Grounded in
  engine card facts. Plausible.
- **deck134** (5945 B / 76 lines, top-of-band -- justified per rule 6 by the capability table + lock + survival
  sections): rule-#1 "CRACK EXPEDITION MAP" with the named search-string, fetch-target order, and a closed
  pass-exception; the Stonehorn+Ghostly-Flicker soft-lock written as a repeated per-turn sequence; blink-target
  priority (never a land); Mulldrifter hard-cast-under-pressure gate; role = always defense; counters left to core.
  Grounded (verified against primitives + Scryfall, incl. the N-134a reversal -- the guide uses the correct engine
  `{2}{G}` recastable-fog behavior). Plausible.
- **deck136** (7533 B / 102 lines, top-of-band -- justified by the capability table + midrange dual-role floor +
  survival): board-state-keyed attack floor (beatdown vs slow / control vs aggro); all-black mana trust line;
  Legion's End framed as the cast-target choice ONLY (correctly NO crutch for the phantom-chooser engine bug -- it
  teaches nothing there, per the deck136 reviewer + skill Method headline 3). Grounded, Oracle-checked clean.
  Plausible.

## Deploy order (integrator)

1. `cp` seat 137 -> live `deck137_strategy.txt`; `cp` seat 93/134/136 -> new live `deck93/134/136_strategy.txt`.
2. `git add -f` the four (137 tracked-but-gitignored-dir; 93/134/136 new).
3. Leave 18/22/131 untouched (byte-frozen; live already matches). Move deck131 into the canary set per rotation.
4. Verify each deployed file `/usr/bin/grep -c $'\357\277\275' == 0` (all seat files already 0).
5. Core prompt: NO CHANGE.
