# Deck-135 wave-19 — development notes (engine / harness / model) + ROTATION VERDICT

Run: PRIMARY `matchups-20260719-092058`. Binary `/tmp/wagic-72b05535d` (wave-19 batch: combat name
tolerance; TRADE OUTCOME block annotations; alt-cast pitch surfacing; salvageLoopedChoice; **GPT REVEAL
SEAM f4a2ea891 = `Player::decideReveal`, translog kind `reveal`, 18 decisions ALL at this seat**;
suppression->annotation replacement; Oracle sweep incl. planeswalker-damage errata). deck135 jsonl
game->file map in findings.md. Doctrine: win column context-only; layer-routing enforced (engine items
live HERE with repros, never in guide text). Each item self-contained with seq repros.

## ENGINE LEDGER

- **ENGINE-R1 (NEW this corpus, deck135-OWNED, HIGH VALUE, GAME-AFFECTING) — Glacial Revelation's
  interactive reveal drops the entire "to-hand" partition; every revealed card is milled to graveyard.**
  - REPRO: 3/3 parsed Glacial Revelation reveals lost 100% of the to-hand picks.
    - d110 s18: `PUT: 2,6` (Snow-Covered Island + Ohran Viper). Events (next rec): `"You revealed 6 and
      put Snow-Covered Island, Ohran Viper to get snow"` THEN all 6 incl. both `"... goes to graveyard"`.
      At 11 life vs lethal Master of Etherium; the milled Ohran Viper was its ONLY answer. -> collapse to
      1 life, adj LOSS.
    - d49 s30: `PUT: 1,2,3,4,6` (5 creatures incl. Abominable Treefolk the finisher). Same shape — all 6
      to graveyard. At 10 life vs 12. adj LOSS.
    - d27 s12: `PUT: 1,3,4,5,6`. All 6 to graveyard (incl. legal snow permanents Snow-Covered Island,
      Arcum's Astrolabe). 18 life, non-critical.
  - NOT eligibility: d110 s18 milled Snow-Covered Island (unambiguously a snow permanent).
  - HYPOTHESIS (verify before fixing; wagic-cardscript layer-first): interactive `auto=` line at
    `Res/sets/primitives/borderline.txt:45643`:
    `optionone name(Get Snow) target(<anyamount>*[snow]|reveal) moveto(hand) optiononeend
     optiontwo all(*|reveal) moveto(graveyard) optiontwoend`. optiontwo's `all(*|reveal)` sweeps EVERY
    revealed card to graveyard, apparently re-grabbing what optionone's `<anyamount>` multi-select moved
    to hand. Working reveals (Search-for-Azcanta upkeep, line 98940) use `optiontwo target(<1>*|reveal)`
    (targets the REMAINDER, not `all`). The NON-interactive `aicode` line for the same card is CORRECT
    (`all(snow[zpos<=6]|mylibrary) moveto(hand) && all(*[-snow;zpos<=6]|mylibrary) moveto(mygraveyard)`)
    — so the reveal-seam routing (GPT->interactive `auto=` per ishuman lens) EXPOSED a latent
    interactive-script bug Baka never hit. FIX DIRECTION: change Glacial Revelation optiontwo to target
    only the unchosen reveal remainder (e.g. `target(<anyamount>*|reveal) moveto(graveyard)` matching the
    working idiom), OR fix the engine so `<anyamount>` moveto(hand) consumes cards from the reveal set
    before optiontwo. `all(*|reveal)` is used 538x corpus-wide, many with single-target optionone that
    may be fine — scope the fix to the multi-to-hand shape, do NOT blanket-ban `all`. VERIFY on the
    binary with a probe deck (stack Glacial Revelation + snow permanents, one selfplay game, read the
    translog next-record zone moves) before AND after. DETECT REGRESSION: a Glacial Revelation reveal
    whose "get snow" picks appear in the subsequent `"... goes to graveyard"` list instead of reaching
    hand.

- **ENGINE-R3 (NEW, deck135-OWNED, REPRESENTATION) — Into the North SEARCH mis-rendered as a
  whole-library "choose subset to hand" menu.** Into the North (`Search your library for a snow land ...
  put onto battlefield tapped`) is shown through the reveal seam as `"you looked at the top 49 cards ...
  which go to 'choose card' vs 'put back'"` — the entire library, no snow-land filter, verbs describing a
  hand-partition not a battlefield search. OUTCOME is CORRECT (engine puts the FIRST legal snow land in
  the chosen set onto the battlefield: d35 s18->Forest, d62 s17->Island, d62 s23->Forest, d49 s11->Forest,
  4/4). But the misrendering caused a 12k-char confusion spiral at every instance + 1 unparsed fallback
  (d35 s10). FIX (representation): offer ONLY legal snow-land search targets, and label it a
  search-to-battlefield (not a choose-subset-to-hand). DETECT: a `reveal` whose options_text is ~the whole
  library while the driving spell is a targeted search/tutor.

- **ENGINE-R4 (partial CLOSE + residual, deck135-OWNED) — Azcanta reveal path.** (a) The wave-17/18
  Azcanta OPAQUE "Choose target N of exactly 4" ORDERING menu is GONE — the transform+activation now
  renders as a clean reveal seam (d27 s24). CLOSED. (b) NEW residual: the activation optionone
  (`borderline.txt:7118`, `target(<upto:1>*[-land;-creature]|reveal) moveto(hand)`) restricts the to-hand
  card to NONCREATURE-NONLAND, but the reveal seam does NOT surface the restriction. d27 s24 options
  [Windswept Heath, Diamond Faerie, Boreal Druid, Gelid Shackles]; model picked Diamond Faerie + Boreal
  Druid (both CREATURES = ineligible) -> 0 to hand, all 4 to bottom. The zero-to-hand here is the MODEL
  picking ineligible cards (representation gap), NOT provably the R1 all-sweep — no eligible card
  (only Gelid Shackles) was tested; the same all-sweep MAY also lurk in the Azcanta activation optiontwo
  (`target(<4>*|reveal) bottomoflibrary`) and should be probed when a legal noncreature-nonland is picked.
  FIX (representation): mark/filter eligible cards on the to-hand partition (same family as R3, gen-sugg #3).
  VALIDATION TODO: an Azcanta activation where the model picks an eligible noncreature-nonland — confirm
  it reaches hand (rules out R1-style drop) and the rest reach bottom.

- **ENGINE-R2-VERDICT (the brief's non-contiguous-ordering residual) — REFUTED as an ordering bug.** The
  hypothesized "chose indices 2,3 / engine moved index 1" skip-first ORDERING fault is NOT the mechanism.
  What occupies the seam is the R1 partition drop: the whole to-hand set is milled to graveyard regardless
  of index contiguity. All 3 exercised picks were non-contiguous/skip-first (d110 `2,6` skip-1; d49
  `1,2,3,4,6` skip-5; d27 `1,3,4,5,6` skip-2) and every chosen card went to the WRONG zone (graveyard).
  The pre-corpus "engine moved index 1" glimpse is consistent with an early view of the `all(*|reveal)`
  sweep (index 1 -> graveyard because ALL -> graveyard), not an off-by-one. The one seam that honors the
  chosen SET — Into the North (R3) — takes the first legal target in index order, correctly. STATUS:
  CONFIRMED-BUG = ENGINE-R1 (partition drop); non-contiguous ORDERING bug = NOT-A-BUG / not exercised as
  such. Seqs: d110 s18, d49 s30, d27 s12.

- **ENGINE #4b (optionone reveal-to-X AUTO-DECLINE) — CLOSED at this seat.** The wave-17/18 fingerprint
  (engine auto-declines the optional reveal chooser before the model can answer) is GONE: the model now
  DRIVES all 8 Search-for-Azcanta upkeep looks, and both branches resolve correctly — put-to-graveyard
  fires (d14 s11 Astrolabe, d14 s29 Vista; events `... goes to graveyard`), keep-in-library fires
  (`PUT: none` -> card stays and is drawn next turn: d14 s18/24/34, d49 s24/28). The upkeep script uses
  the safe `target(<upto:1>*|reveal)` idiom. Old KNOWN-OPEN item resolved by the reveal seam.

- **ENGINE-1d-followup (CARRIED, deck135-OWNED, mostly-RESOLVED) — Force free-pitch card naming.** The
  wave-18 fix landed: the alt-cast option now NAMES the auto-exile pool. d62 s19 option:
  `Cast Force of Negation with its exile a blue card from hand cost  (auto-exiles ONE of: Search for
  Azcanta, Ice-Fang Coatl, Search for Azcanta - the engine picks for you here) - can target on the stack:
  Gaea's Anthem`. The engine auto-picked Search for Azcanta (a dig), NOT a finisher — the wave-18
  finisher-eating problem did NOT recur. Residual (LOW priority): the engine still AUTO-picks the pitch;
  when the caster holds >1 blue card the model cannot choose WHICH to pitch. The "exiles your ONLY
  eligible card" warning correctly did not fire (3 blue cards). Surface the pitch-card SELECTION only if
  a future case shows the auto-pick eating a finisher when a worse pitch existed. DETECT: a Force free
  cast whose auto-exile eats a listed finisher while a lower-value blue card was in hand.

- **ENGINE #2 (CARRIED from wave-13; stale `[attacking]` tag) — regression-watch, benign.** Not
  specifically re-audited this corpus (attention went to the reveal seam), but no wrong decision traced
  to a board tag. Carried unchanged; low priority. FIX (unchanged): clear `[attacking]` on any creature
  not in the current declared-attackers set.

- **ENGINE #4 (Azcanta ORDERING menu, wave-17 HIGH-VALUE) — SUPERSEDED by ENGINE-R4.** The opaque
  "Choose target N of exactly 4" menu no longer exists (reveal seam replaced it). The put-to-hand concern
  migrates to R4 (eligibility surfacing) + the R1-style-drop VALIDATION TODO under R4.

## HARNESS

- **HARNESS-1 (NEW) — combat-name-tolerance residual: a well-formed single BLOCKS line discarded on the
  disambiguation suffix.** d62 s27 (blockers, 923 chars, NOT a repeat-loop): reply ended
  `BLOCKS: Ice-Fang Coatl: Saproling (1/1) #1` — a legal single block — but fell to `unparsed_reply`.
  The wave-19 combat name->label second pass did not map `Saproling (1/1) #1`: the model appended a
  `(P/T) #N` count-suffix to disambiguate two identical Saprolings and the parser choked on it. FIX:
  strip trailing `(P/T)` / `#N` annotations before the name match in parseBlockAssignments. This is a
  NEW shape (short + well-formed), not the old computed-P/T-distrust pocket. Non-fatal (adj-LOSS game)
  but a real dropped legal block. DETECT: an unparsed `blockers` reply carrying a syntactically valid
  `BLOCKS:` line with `(P/T)`/`#N` suffixes on the attacker name.

- **HARNESS-2 (CARRIED) — salvageLoopedChoice does NOT cover the seat's fallback shapes.** 0 of 9
  fallbacks at this seat were salvaged. The salvage keys on `CHOICE:`/`ATTACK:` and misses (a) reveal
  `PUT:` lines (d35 s10, d27 s8 stated a tentative pick early before spiraling) and (b) `BLOCKS:` lines
  (d62 s27). EXTEND the salvage scanner to recover the last well-formed `PUT:` and `BLOCKS:` line. ->
  gen-sugg #2.

- **HARNESS-3 (CARRIED) — decode-time REPEAT-LOOP tax, worst-seat.** 4 of 9 fallbacks are >12k-char
  repetition spirals (d35 s16/s21/s30, d110 s25) on lethal-math / mana-confusion at hard spots; none
  flipped a winnable game. Route to a decode-time repetition-penalty / max-token guard. Plus 2 NEW
  reveal-seam-comprehension spirals (d35 s10, d27 s8) driven by the seam's eligibility opacity (fix via
  R3/R4 representation). Model/decode item, NOT guide-fixable.

## MODEL (route to model-experiments; NOT guide text) — see general-suggestions #1/#2/#3.
- Reflexive off-whitelist Force (5th corpus; Gaea's Anthem this wave, non-fatal; finisher-eating half
  now fixed by the naming annotation). Card-knowledge: snow-supertype confusion during reveals (which of
  the 6/4/49 are snow permanents / noncreature-nonland). Repeat-loop spirals.
- IMPROVEMENT: vs14 clean real kill (curve -> Treefolk/Coatl/Diamond Faerie); vs35 flipped LOSS->adj-WIN
  (stabilized vs all-creature Slivers without Force). The deck's plan executes cleanly when ENGINE-R1
  doesn't strip its gas.

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)
- Under-powered small-creature clock — STANDS (6th corpus); Treefolk/Diamond Faerie the only real
  closers; R1 makes it worse by milling them.
- Force DEAD vs all-creature aggro — softened this wave (vs35 Slivers stabilized into an adj WIN).
- Thin blue base + fetch-color mismatch (Windswept Heath cannot fetch Island) — STANDS.

## VALIDATION TODO (for the next run)
1. **ENGINE-R1 (Glacial Revelation to-hand drop):** after the card-script/engine fix, confirm the model's
   "get snow" picks REACH HAND and only the rest are milled. Probe deck: stack Glacial Revelation + snow
   permanents; read the next-record zone moves.
2. **ENGINE-R4 (Azcanta activation):** an activation where the model picks an ELIGIBLE noncreature-nonland
   — confirm it reaches hand (rules out an R1-style drop in the activation optiontwo) and rest -> bottom.
3. **ENGINE-R3 (Into the North rendering):** confirm the search offers only legal snow-land targets and is
   labeled a search-to-battlefield; confirm the reveal-comprehension spirals (d35 s10-class) disappear.
4. **HARNESS-1 (block name tolerance):** confirm a `BLOCKS: <name> (P/T) #N` line now parses.
5. **HARNESS-2 (salvage coverage):** confirm salvageLoopedChoice recovers a truncated `PUT:` / `BLOCKS:`.
6. **ENGINE #4b (auto-decline):** regression-watch — the model should keep driving Search-for-Azcanta
   upkeep looks (both branches).

## PER-DECK ROTATION VERDICT: **DO NOT ROTATE deck135 out. KEEP IN.**

Rotation retires a veteran whose guide takes NO modification AND whose seat surfaces NO new work signal.
deck135's guide took no modification (FROZEN, byte-identical, `cmp`-verified) — but the seat surfaced the
STRONGEST new-work signal of the wave:
- **It OWNS and answered the entire reveal-seam charge** (all 18 decisions), and in doing so found the
  wave's single highest-value engine bug: **ENGINE-R1 (Glacial Revelation to-hand drop)** — a
  game-affecting, card-script-fixable defect the reveal-seam routing exposed, which worsened 2 of 3
  adjudicated losses. No other seat exercises the reveal seam (0 reveals elsewhere).
- **It CLOSED two long-carried items** (ENGINE #4b optionone auto-decline; the wave-17 Azcanta opaque
  ordering menu) and **VALIDATED the wave-18 pitch-card-naming fix (ENGINE-1d)** — this seat is the
  designated proving ground for the Force + Azcanta + reveal seams and it paid off on all three.
- **It surfaced two NEW engine/harness items** (ENGINE-R3 Into-the-North rendering; HARNESS-1 block
  name-tolerance residual) plus open VALIDATION TODOs (R1/R4 fix confirmation, salvage coverage).
Rotating deck135 out would orphan the ENGINE-R1 fix confirmation and the reveal-seam validation loop that
only this seat runs. KEEP deck135 IN through at least the ENGINE-R1/R4 fix confirmations.
