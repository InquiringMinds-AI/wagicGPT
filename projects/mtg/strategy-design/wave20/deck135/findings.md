# Deck-135 (Modern Snow) — Wave-20 game review

Corpus: PRIMARY `matchups-20260719-151849` (21-game round-robin over 135,62,49,35,14,27,102;
binary `/tmp/wagic-95e89c204` = wave-20 batch: **ENGINE-R1 reveal-partition fix** (driveInteractiveReveal
finalizes option-one picks same-tick); attacker-sparing mana payment; perception bundle incl. reveal
ELIGIBILITY surfacing + combat options_text + #N ordinals). deck110 rotated OUT (canary); deck102 (Tergrid)
entered guideless. **deck135 OWNS THE REVEAL PARTITION CHECK** — its 5 reveal records are the live validation
of the wave-19 ENGINE-R1 bug fix that THIS SEAT found. All six deck135 seat games read decision-by-decision.
Doctrine: win column CONTEXT ONLY; unit of analysis is the DECISION with seq repros; guide FROZEN since wave-11.

## RECORD (context only — NOT a verdict) — 4W / 2L  (up from wave-19's 2/6)

| game    | opp                  | outcome (deck135)   | my/opp life | turn | class |
|---------|----------------------|---------------------|-------------|------|-------|
| 135v102 | 102 Tergrid (guideless, NEW) | **WIN** (real kill) | 9 / -4  | 12 | curve + closed; pool's weakest deck |
| 135v62  | 62 Enchantresses     | **WIN** (real kill) | 13 / 0  | 10 | **FLIPPED from wave-19 adj-LOSS**; NO reveals this game |
| 135v35  | 35 Mighty Slivers    | **WIN** (real kill) | 4 / -8  | 10 | Treefolk 9/9 closed; was adj-WIN wave-19 |
| 135v14  | 14 Deep Blue         | **WIN** (adj, ahead)| 15 / 3  | 15 | ground-out ahead; was real-WIN wave-19 |
| 135v49  | 49 Dragons           | LOSS (adj, behind)  | 2 / 14  | 10 | raced by Dragon clock; LOSS both waves |
| 135v27  | 27 Zombies           | LOSS (real, dead)   | -7 / 3  | 11 | out-ground; the 5-fallback game; LOSS both waves |

Win table assigns deck135 4/6; matches. **The five repeated matchups (62/49/35/14/27) had the SAME opponents
as wave-19**, so the record is directly comparable game-for-game (see the JUMP section — this is the clean
control that makes the attribution airtight).

---

# PRIMARY DELIVERABLE — THE REVEAL PARTITION FIX (ENGINE-R1) IS VALIDATED: 5/5 reveals LANDED, 0 mismatches, 0 fallbacks

deck135 fired **5 reveal records** this corpus (down from 18 in wave-19 — draw variance, not a triggering
regression; see "reveal volume" below). Every one is quoted below with `chosen_text` and the following
record's engine `events`, with a per-record LAND/MISMATCH verdict. **All five LANDED. There is no residual
mismatch. The wave-19 ENGINE-R1 partition-drop bug is FIXED.**

## R1-VALIDATION — Glacial Revelation (vs27 s18, turn 8, 11/19) — **LANDED. The fix works.**

This is THE test. In wave-19 this exact card, 3/3 times, silently milled the model's chosen snow permanents
to graveyard (the `all(*|reveal)` optiontwo overwrote the to-hand partition). This corpus it was cast ONCE
(vs27) and resolved **correctly**:

- **Prompt** now surfaces eligibility: `ELIGIBILITY: only a snow card may go to "get snow"...` with each of the
  6 revealed cards marked `[eligible for "get snow"]` (Ohran Viper x2, Abominable Treefolk, Ice-Fang Coatl) or
  `[does NOT qualify - goes to "option two"]` (Misty Rainforest, Flooded Strand — the two non-snow fetches).
- **chosen_text**: `"Ohran Viper, Ohran Viper, Abominable Treefolk, Ice-Fang Coatl"` (`PUT: 1, 2, 4, 5`) — the
  model picked exactly the four eligible snow permanents, skipping the two non-snow fetches.
- **Engine events (next record, seq 19)** — verbatim:
  ```
  - You revealed 6 and put Ohran Viper, Ohran Viper, Abominable Treefolk, Ice-Fang Coatl to get snow
  - Your revealed Ohran Viper goes to hand
  - Your revealed Ohran Viper goes to hand
  - Your revealed Abominable Treefolk goes to hand
  - Your revealed Ice-Fang Coatl goes to hand
  - Your revealed Misty Rainforest goes to graveyard
  - Your revealed Flooded Strand goes to graveyard
  ```
- **VERDICT: LANDED PERFECTLY.** All four chosen snow permanents reached HAND; only the two unchosen non-snow
  cards were milled. This is exactly the Oracle behavior ("put any number of snow permanent cards into your
  hand, put the rest into your graveyard"). Contrast wave-19 d27 s12 / d49 s30 / d110 s18 where the identical
  pick shape sent ALL SIX to graveyard. **ENGINE-R1 = CLOSED at the decision surface.**
  - CAVEAT (honest sample size): this is **n=1** for the Glacial Revelation path — one cast, decisive and
    clean, backed by the code change. 3/3 broken (wave-19) -> 1/1 fixed (wave-20). Strong but thin; one more
    corpus with a Glacial Revelation would harden it beyond a single instance (kept as an open validation TODO).

## R2 — Into the North search (vs102 s14; vs14 s12; vs49 s11) — **3/3 LANDED (outcome correct).**

Into the North (`Search your library for a snow land card and put it onto the battlefield tapped`) still
renders through the reveal seam as a whole-library "choose card / put back" menu (the wave-19 R3 rendering
label is unchanged), BUT the perception bundle's **eligibility surfacing now marks every card** `[eligible for
"choose card"]` (snow lands only) vs `[does NOT qualify - goes to "put back"]`. The model read the marks and
picked only eligible snow lands in all three; the engine put the first legal snow land onto the battlefield:

| seq | chosen_text (all snow lands) | engine events (next record) | verdict |
|-----|------------------------------|-----------------------------|---------|
| vs102 s14 | Snow-Covered Forest, Snow-Covered Forest | `Your Snow-Covered Forest: zone -> battlefield` | LANDED |
| vs14 s12  | Snow-Covered Forest, Plains, Island, Scrying Sheets, Forest, Forest | `Your Snow-Covered Forest: zone -> battlefield` | LANDED |
| vs49 s11  | Snow-Covered Plains, Scrying Sheets, Forest, Island, Plains, Forest, Forest | `Your Snow-Covered Plains: zone -> battlefield` | LANDED |

All three replies were SHORT and clean (421 / 482 / 609 chars) — **zero of the wave-19 12k-char "which are snow
lands" spirals, and zero fallbacks**. In wave-19 every Into the North reveal produced a giant-reply spiral and
two of them fell to `unparsed_reply`. The eligibility marking (the exact representation fix this seat asked for
in wave-19 gen-sugg #3 / ENGINE-R3/R4) SHIPPED and killed the spiral. (Minor cosmetic residual: the effect is
still LABELLED "choose card" not "search a snow land to battlefield"; vs14 s12 even mislabeled it a "Glacial
Revelation resolution" — harmless, the model picked correctly off the eligibility marks anyway. Non-actionable.)

## R3 — Search for Azcanta upkeep look-top-1, the 'none' record (vs49 s27, turn 10 upkeep, **2 life** / 14) — **LANDED.**

- Options: `[Icehide Golem]` (one card). **chosen_text**: `"none"` (`PUT: none`).
- Plan (verbatim tail): *"...I should not discard this card. I will keep it in my library to draw it next turn
  (or later) where it can be cast. ... PUT: none."*
- **Engine events (seq 28)**: `- You revealed 1 and kept them all (put in library)` / `- Choose an option for
  Search for Azcanta: -> put in library`.
- **VERDICT: LANDED.** At 2 life the model correctly kept its next-turn blocker (Icehide Golem) in library
  rather than binning it — the keep-in-library branch resolved exactly as chosen. Good decision AND correct
  resolution. (This is the corpus's one `none` reveal.)

## Reveal-seam bottom line (the whole charge, discharged)

- **ENGINE-R1 Glacial Revelation to-hand drop: FIXED / VALIDATED** (1/1 landed, n=1 thin — hold open for a
  second confirmation). This was the wave-19 top engine bug; the seam this seat owns caught it AND now confirms
  the fix.
- **Eligibility surfacing (perception bundle item c): SHIPPED + FIRED** on BOTH Glacial Revelation and Into the
  North prompts — closes the wave-19 R3/R4 eligibility-marking half; directly removed the reveal-spiral tax.
- **Into the North search: 3/3 outcome-correct**, zero fallbacks (was 4-spirals + 2 fallbacks in wave-19).
- **Reveal fallbacks: 0** (down from 2 at this seat in wave-19).
- **Azcanta ACTIVATION path: 0 windows this corpus (UNTESTED).** No Search for Azcanta transformed -> activated,
  so the wave-19 R4 activation-eligibility residual (model picked ineligible creatures) and the "is R1 also in
  the activation optiontwo?" question got **no test** this wave — untested, not vindicated. Stays open.
- **Non-contiguous ordering "bug": remains REFUTED** (was never an ordering fault; it was R1, now fixed).

### Reveal volume 18 -> 5: DRAW VARIANCE, not a triggering regression.
Every seam TYPE that had cards available fired and resolved correctly (Glacial Revelation 1, Into the North 3,
Search-for-Azcanta upkeep 1). The drop is that Search for Azcanta stayed in play far less (8 upkeep looks ->1)
and Glacial Revelation was cast once (4->1) — both are how-many-copies-you-draw variance. The brief asked
"was Glacial Revelation even cast?" — **YES, once (vs27 s18), and it worked.** No evidence of a reveal-TRIGGER
regression: both Search-for-Azcanta branches (put-to-graveyard / keep) are known-good from wave-19 and the keep
branch fired correctly here (vs49 s27).

---

# THE 2/6 -> 4/6 JUMP — attribution: MATCHUP-SLATE + single-game VARIANCE. NOT the engine batch, NOT the guide.

The five repeated opponents give a clean game-for-game control:

| opp | wave-19 | wave-20 | delta | attributable to |
|-----|---------|---------|-------|-----------------|
| 35  | WIN (adj)  | WIN (real) | same (WIN) | — |
| 14  | WIN (real) | WIN (adj)  | same (WIN) | — |
| 62  | **LOSS (adj)** | **WIN (real)** | **+1 FLIP** | variance — see below |
| 49  | LOSS (adj) | LOSS (adj) | same (LOSS) | — |
| 27  | LOSS (real)| LOSS (real)| same (LOSS) | — |
| swap | vs110 **LOSS (adj)** | vs102 **WIN** | **+1 SWAP** | matchup slate |

So the **+2 wins decompose cleanly**:

1. **+1 = MATCHUP SLATE SWAP (deck110 OUT -> deck102 IN).** Wave-19 deck135 lost to deck110 (Etched Affinity,
   adj at 1 life — a wave-19 loss the R1 bug had worsened). deck110 rotated out; in its place deck135 draws
   deck102 (guideless Tergrid, the pool's WEAKEST deck at 1/6) and wins the real kill (9/-4). This is a pure
   slate-composition gain — nothing about deck135's play or guide changed.

2. **+1 = vs62 FLIP (LOSS->WIN), and it is VARIANCE, not the engine fix.** The vs62 game contains **zero reveal
   records** — no Glacial Revelation, no Into the North resolved through the seam — so the ENGINE-R1 fix
   **cannot** be the cause. deck135 closed the game (opp to 0, real kill T10) where wave-19 it got out-raced by
   Enchantress lifegain to an adj loss at the cap. Draw/sequencing variance on n=1 (and deck62 itself was on a
   revised round-4 guide, a moving target). Not a guide or engine signal.

3. **The ENGINE BATCH (R1 fix) did NOT drive the record.** Glacial Revelation resolved only ONCE all corpus
   (vs27), and that game was **still a loss** (-7). The two wave-19 games R1 had actively harmed — vs49 and the
   now-absent vs110 — could not benefit: vs49 drew no Glacial Revelation this wave (still an adj loss), and
   vs110 is gone from the slate. So the fix **removed a latent self-mill liability but did not manifest as any
   win this corpus.** It is validated-correct with a near-zero win-rate footprint (under-exercised by draw luck).

**Conclusion (decision-evidence attribution): the jump is MATCHUP (dominant, clean via the repeated-slate
control) + VARIANCE (the single vs62 flip on a reveal-free game). The engine batch is validated but did not
move the record; the guide did not change.** Fully consistent with the win-rate-is-context doctrine and the
n<=6 / adj-heavy variance discipline — a 2-game swing here is slate + noise, not a play-quality step change.

---

# THE 7 FALLBACKS — all the carried decode-time REPEAT-LOOP / giant-reply tax (HARNESS-3). No new shape. Plus one HIDDEN loop the fallback count MISSES.

Profile: 7 fallbacks, **all `unparsed_reply`**, **5 in vs27** + **2 in vs49** — both LOSSES. Zero at the four
wins. Zero reveal fallbacks. Zero blocker/combat fallbacks. salvageLoopedChoice rescued **0 of 7** (these
spirals never emit a clean trailing CHOICE line to salvage).

## The vs27 5-fallback cluster (the brief's "5 in one game" — it is the giant-reply tax pocket, concentrated)

All five are >12k-char decode-repetition spirals in a hopeless grind (deck135 4-11 life, Zombies ahead, game
lost at -7). This is the SAME pocket as wave-18/19, not a new shape:

| seq | kind | turn | len | loop content (verbatim tail) |
|-----|------|------|-----|------------------------------|
| s5  | ask  | 2  | 15780 | land-sequencing: *"I play Prismatic Vista. Then sacrifice it to find a Snow-Covered Forest. Wait, I can only play one land per turn. So..."* |
| s24 | priority | 10 | 13293 | blocker-analysis: *"Lord of the Undead is on the battlefield. It is not tapped. So Lord of the..."* (protection-from-green reasoning) |
| s25 | ask  | 10 | 12762 | lethal-math: *"Treefolk deals 8 to player... Total damage to player: 8. Still not 13. I need to deal 13..."* |
| s26 | ask  | 10 | 12451 | the "deals 0?" enumeration: *"Maybe I can cast Force of Negation and it deals 0 damage? No. Maybe Ohran Viper... deals 0? No..."* |
| s30 | ask  | 10 | 15188 | survival enumeration: *"cast Gelid Shackles... to prevent it from blocking next turn? No, that doesn't help me survive. Wait,..."* |

Read: the clustering is not a new failure mode — it is **one hopeless game absorbing the whole cluster.** vs27
raced deck135 into repeated hard lethal-math / survival decisions with no good answer, and each spiraled to the
token cap. Consistent with the wave-19 finding that these loops cluster in already-lost contexts and none flip
a winnable game. The one early-game outlier (s5, turn 2 land-sequencing) is the same decode pathology, not a
strategic error. **NOT a new shape — it is the carried decode-loop tax, and the vs27 concentration is variance
in WHERE the hard spots fell.**

## vs49 (2 fallbacks) — same shape

- s22 (turn 8, 6/14, 15087 ch): lethal/blocker math loop (*"if Whelp can't block, then Coatl and Golem both
  attack unblocked... Total 3. Opponent goes to 11."*).
- s24 (turn 8, 6/14, 14153 ch): Gelid-Shackles-to-stop-a-blocker loop (*"enchant Bloodmark Mentor... it can't
  block. Next turn attack with Golem."*).
Both at the same turn-8 losing spot, same decode-loop family.

## HIDDEN LOOP the fallback metric UNDERCOUNTS — vs35 s10 (parsed "cleanly" to a RETRACTED false start)

The fallback count (7) **undercounts** the true decode-loop tax. vs35 s10 (turn 4, 19/18, `fallback: null`) is
a 15k-char repeat-loop whose reasoning **converged correctly on "cast Icehide Golem"** (~8 verbatim repeats of
*"the best play is to cast Icehide Golem"*), but the reply opened with a false-start `CHOICE: 4 (Cast nothing
right now)` — immediately retracted in prose (*"Wait, I made a mistake in my reasoning"*) — then spiraled to the
token cap **without ever re-emitting a final CHOICE line.** The parser took the one CHOICE line present (the
retracted `CHOICE: 4`), logged **cast-nothing**, and deck135 deployed no creature that turn (it only had the
existing Golem attack). Because a parseable-but-wrong digit existed, this loop **did not count as a fallback** —
it is invisible to the fallback tally. This is the skill's wave-10 "obeyed-in-reasoning, leaked-at-number"
class: the guide (Rule #1 deploy-a-creature) was OBEYED in the derivation; the leak is pure reply-protocol /
decode. Non-fatal (vs35 was won 4/-8) but a real dropped creature-deploy AND an instrument correction: the
decode-loop tax is at least 8, not 7. -> notes HARNESS-1 (parser: prefer the LAST CHOICE line / detect a
retracted early CHOICE) + HARNESS-3 (decode-time repetition guard). NOT guide-fixable.

---

# OBEDIENCE CHECK (turnaround/OBEYED discipline — the record IMPROVED, so verify before crediting or churning)

- **Attacker participation (Rule #2):** attackers declared at every profitable window across all six games; the
  only "no attackers" was vs49 at low life (a defensible defensive hold). **No 0-attacker durdle signature.**
  Wins closed via the real threats (vs35 Treefolk 9/9 -> opp -8; vs62 opp 0; vs102 opp -4). Rule #2 obeyed.
- **Cast-a-creature discipline (Rule #1):** obeyed in reasoning everywhere. A crude scan flagged several
  "cast-nothing with a creature option present"; on inspection ALL were correct or false positives —
  post-combat second-mains, lethal-attack turns where a summoning-sick body adds nothing (vs35 s25/s26,
  vs27 s28), or a "Cast Gelid Shackles" option my filter mis-matched on a creature NAME in its target list
  (vs27 s19/s22). The ONE genuine miss (vs35 s10) is the decode-loop above — obeyed in reasoning, leaked at the
  number. No strategic Rule #1 violation.
- **Force of Negation:** cast **0 times** this corpus (no whitelist window arose). The wave-19 reflexive
  off-whitelist-Force watch therefore had **no test window** -> stays at WATCH per the pause-aware discipline
  (absence != vindication). No finisher-eating, because no Force fired.
- **Mana / attacker-sparing (deck35 owns the check):** no evidence of a would-be attacker tapped for mana with
  lands available at this seat.

---

# CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)
- **Under-powered small-creature clock — softened but STANDS (7th corpus).** With R1 fixed, Glacial Revelation
  is a card-advantage engine again (vs27 s18 raked 4 creatures to hand), and 4/6 with three real kills shows the
  plan closing more often. But the two losses (vs27, vs49) are still "raced before the finisher lands" — the
  colored-mana fragility below is the throttle (vs27 s19: four creatures in hand, uncastable for lack of an
  untapped G/U source). Treefolk / Diamond Faerie remain the only real closers.
- **Thin blue base + fetch-color mismatch — STANDS.** Windswept Heath still cannot fetch Island; gold/double-pip
  dependence remains the fragility that turns "creatures in hand" into "creatures stranded" (vs27 s19).
- **Force DEAD vs all-creature aggro — dormant this wave** (0 Force casts; vs35 Slivers won without it).

# GUIDE DISPOSITION: FROZEN (byte-identical to live `Res/ai/baka/deck135_strategy.txt`; `cmp`-verified).
Every residual this corpus routes OFF the guide. The reveal wins are ENGINE (R1 fix) + REPRESENTATION
(eligibility surfacing) — the guide was always Oracle-correct and the engine has now caught up to it (Glacial
Revelation IS the card-advantage engine the guide describes). The 7+1 fallbacks are decode-time repeat-loops
(harness/decode). The record jump is matchup + variance. Obedience is healthy. No decision-level leak attaches
to any guide line. Guide diff = 0 bytes. See notes for the layer-routed ledger + rotation verdict.

## GAME -> FILE MAP (run = matchups-20260719-151849)
| opp | outcome | final (me v opp) | deck135 seat file |
|-----|---------|------------------|-------------------|
| vs27  | LOSS (real) | -7 v 3  | `1784492333-ai_baka_deck135-0x56294a7a09d0-vs-ai_baka_deck27.jsonl` |
| vs102 | WIN (real)  | 9 v -4  | `1784495813-ai_baka_deck135-0x5601a1aa07e0-vs-ai_baka_deck102.jsonl` |
| vs14  | WIN (adj)   | 15 v 3  | `1784497445-ai_baka_deck135-0x556c04f7e850-vs-ai_baka_deck14.jsonl` |
| vs49  | LOSS (adj)  | 2 v 14  | `1784498849-ai_baka_deck135-0x55bd5cbe4a10-vs-ai_baka_deck49.jsonl` |
| vs62  | WIN (real)  | 13 v 0  | `1784499446-ai_baka_deck135-0x55fb7bd7f900-vs-ai_baka_deck62.jsonl` |
| vs35  | WIN (real)  | 4 v -8  | `1784501196-ai_baka_deck135-0x55b774609930-vs-ai_baka_deck35.jsonl` |
