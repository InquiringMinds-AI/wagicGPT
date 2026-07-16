# Deck-110 wave-15 — development notes (self-contained; layer-routed engine/harness ledger + rotation verdict)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-040138/`, 6 deck110 seat translogs
(`*ai_baka_deck110-0x*.jsonl`). Binary `/tmp/wagic-aad32bc7f`. Shipped fixes under test this corpus
(all wave-14-validated per brief): Galvanic Blast metalcraft `3cb901295`; attackers name->index
reconcile + absent-echo staleness `d0f88326b`; target-cancel use-after-free `aad32bc7f`. Wave-11
guide FROZEN (live `deck110_strategy.txt` md5 `00849aa9`; byte-identical 8th freeze). Answer-after-plan
+ name-echo (`CHOICE: N (name)`). Choices 1-indexed. Card facts vs
`projects/mtg/bin/Res/sets/primitives/mtg.txt`. Every item carries a repro; engine/harness items
belong HERE, never in prompt/guide text.

## E4 — RESOLVED / HIGH — Galvanic Blast metalcraft fix VALIDATED (was the wave-13 flat-2 card-script bug).

STATUS: CLOSED at this seat. The wave-13 bug (`auto=aslongas(artifact|myBattlefield) damage:2 >2`,
static form, missing `oneshot`, never resolved -> flat 2 corpus-wide) is FIXED (`3cb901295`).
CORPUS EVIDENCE (count `Damage:` instances per Blast cast vs surface `Artifacts in play: you N`):
  - deck135 s5, N=3 (ON):  `Damage: 2` + `Damage: 2`, op 20->18->16 = **4**. Correct.
  - deck44 blast,  N>=3 (ON): two `Damage: 2` -> Faerie Miscreant = **4**. Correct (kill).
  - deck17 s15, N=5 (ON):  two `Damage: 2` -> Oona's Gatewarden = **4**. Correct (kill).
  - deck133 s5, N=2 (OFF): single `Damage: 2`, op face = **2**. Correct (metalcraft off at 2).
  - deck133 s21, N=4 (ON): two `Damage: 2`, op 10->6 = **4**. Correct.
FIX SHAPE: metalcraft +2 is delivered as a SECOND resolution instance (base damage:2 + a oneshot
conditional damage:2 that re-targets), so a working metalcraft Blast logs TWO identical `Damage: 2`
lines. VERIFICATION TRAP (record for future waves): a grep that dedupes the event STRING reports a
correct 4-damage Blast as "2" — COUNT instances / read the life delta. The wave-13 `N=2-only`
corpus invariant is GONE. PLAY IMPACT: positive and visible — deck133 flipped loss->win and the
model fired the Blast at the FACE for the guide's intended 4-reach at the exact spot (s21) where
wave-13 committed its Obliterator creature-burn blunder; no creature-burn/toughness blunder recurred
anywhere. NO guide text (guide was always Oracle-correct). Cross-deck: audit any other Galvanic
Blast / metalcraft-oneshot consumer, but deck110's finisher is done.

## E5 — RESOLVED — attackers name->index reconcile VALIDATED (was wave-13 headline-2 MIXED-form silent drop).

STATUS: CLOSED at this seat (`d0f88326b`). 20 attacker windows audited against the prompt's eligible
roster + the log's `You declared attackers:` events:
  - MIXED `ATTACK: A1, <name>` and FULL-NAME (`deck44 s34: Ornithopter, Memnite, Signal Pest, Steel
    Overseer`) forms resolve to the correct eligible set; eligible names unioned in (deck135 s15
    `A1,A2` -> "Signal Pest, Memnite"; deck44 s23/s29 full spreads).
  - Every named creature ABSENT from the eligible roster was SUMMONING-SICK and correctly EXCLUDED
    (deck135 s11 Memnite; deck133 s17 Ornithopter; deck17 s24 Ornithopter, s32 recast Vault Skirge).
  - NO eligible attacker dropped anywhere. The reconcile respects eligibility exactly as the wave-13
    durable-fix spec prescribed.
NO guide text (the model names every attacker; the parser now unions them). See M1(a) for the MODEL
side-signal (summoning-sickness) this exposed.

## E2 — ENGINE/REPRESENTATION (unfixed): equip re-offer no-op marker is TARGET-IDENTITY-keyed, blind to net-zero MOVES. RECURRED as a CLEANER, fully-executed A->B->A bounce.

REPRO (net-zero, in a WIN) — deck135 seat, T6 MP2, my20/op10. ONE Cranial Plating, on Etched
Champion (7/2, protection-from-all, carrier #1).
- s20: options `[Equip Signal Pest, Equip Etched Champion (ALREADY)]`. Model MOVES the Plating OFF
  the Champion onto 0-power Signal Pest ("Champion already equipped ... Signal Pest unattached at 0
  power, +5 next turn"). A DOWNGRADE move — Signal Pest option is UN-marked (a "change") but resulting
  power (5/1) is BELOW the current carrier's (7/2). 1 wasted `{1}`.
- s21: options `[Equip Signal Pest (ALREADY), Equip Etched Champion]`. Model catches it ("Plating
  moved from Champion to Signal Pest ... move it back") and MOVES it back to Champion. 1 wasted `{1}`.
- s22: correctly PASSES.
Full A->B->A executed (2 wasted `{1}`), non-outcome (op at 10, lethal lined up). Wave-13's bounce was
interrupted by the parse guard; this one completes. ROOT CAUSE (unchanged): no-op predicate =
"already attached to THIS target," not the no/low-value predicate. LEVER (unchanged): compute the
marker from OUTCOME DELTA — mark/suppress "Equip ... targeting X" whenever moving the equipment to X
would not raise X's resulting power above the current carrier's. Owner: `AIPlayerGPT.cpp`
equip-option serialization. Priority MEDIUM. NON-CHURN correct contrasts this wave: deck17 s21/s22
(two Platings, two naked fliers), deck44 s23/s29 spreads, deck135 s14 (2nd Plating onto naked Memnite
for lethal). Classify by outcome-delta, not raw same-name count. NO guide text — Rule #1 obeyed 100%
(0 ALREADY-change-NOTHING options taken corpus-wide); the leak is the un-marked MOVE, not Rule #1.

## E6 — REPRESENTATION (new/low-med): "Cast Card Normally with Mox Opal" option shows Mox Opal's OWN text, not the card/action it stands for -> heavy model confusion.

REPROS (both deck44, same game, non-outcome): s5 MP2 (sole option "Cast Card Normally with Mox Opal
{Metalcraft - {T}: Add one mana...}") -> 5.4k-token confusion spiral, model deduces the real action
is "play Glimmervoid," emits a FINAL `CHOICE: 1 (Play Glimmervoid)` (an absent option) -> correct
absent-echo/stale_echo fallback. s30 MP2 (same label) -> "This is confusing. Let me assume it's a
mistake and pass" -> unparsed_reply. The label templating echoes Mox Opal's ability text into the
{card text} slot instead of the card being cast; the model always recovers the intent but at large
reasoning cost (timeout risk under the latency cap). Owner: `AIPlayerGPT.cpp` option serialization
for cast-with-mana-source lines — show the CARD/action, not the mana-rock's text. Priority LOW-MED.
Cross-deck (any deck with Mox Opal / mana rocks). Guide already states "these lines just make mana
with Mox Opal," so this is REPRESENTATION, not guide.

## M1 — MODEL WATCHES.

(a) **Summoning-sickness not tracked (NEW, n=5, harmless).** The model names creatures it cast/re-cast
THAT turn as attackers and folds them into its lethal/damage math (deck133 s17 "attack with both
Ornithopter (4/2) and Memnite for 5 damage, opp to 8" — only Memnite eligible; opp took 1). The
engine roster + reconcile eligibility filter exclude every ineligible name, so no wrong attack and no
eligible drop. Risk is latent: a turn where the model believes a summoning-sick body makes an attack
lethal, and mis-decides (taps out / declines a Blast / sends a needed blocker). All witnesses this
corpus were non-lethal. Harness mitigation if it bites: board-line tag `[can't attack yet — summoning
sick]`. Cross-seat synthesis question.

(b) **Blocker over-block while ahead (deck21 s6, s23) — recurs, LOW stakes.** s6: traded a 1/1
lifelink evasive flier for a ground 1/1 at 18 (mild over-block + a rules-misread — thought its 1/1
Skirge survives blocking a 1/1; both die). s23: took 2 permanent wither counters off a 12/6 to kill a
2/1 at 9 (WITHER reasoning CORRECT — knew Master becomes 10/4; mild over-block). Both defensible,
both non-outcome — no wave-13-style 10-power-clock throwaway. Prompt annotated the correct line in
both -> MODEL/representation-wall, not a guide gap. Cross-seat synthesis question stays open.

(c) **Board-state contradiction (2nd-seat witness of wave-13's board-serialization class, benign).**
deck21 s23: the attacking Scuzzback Scrapper serialized as `[tapped - untaps and can attack next
turn]` while listed in the Attackers block; the model spent reasoning reconciling "tapped but
attacking." It did NOT time out (parsed, choice=1, correct block) -> does NOT complete the wave-13
timeout-as-terminal-cost promotion (needs a contradiction co-located with `unparsed_reply` on a
non-lost decision). Logged as a witness; promotion still pending.

## DC1 — DECK-CONSTRUCTION / META (standing, reaffirmed — owns the loss column).
The single loss (vs140) shares the precondition the guide cannot create: Lightmine Field + repeated
board-wipes AND zero Galvanic Blast drawn (0 casts) = no legal path to damage (attacks suicidal into
Lightmine; the model correctly refused them). Ground out, adjudicated 18/28. Not a guide task. If ever
tuned: trim ~0-power filler + Mox/land toward mid bodies + reach/evasion redundancy to cut
flood + single-threat + grind + wipe exposure at once.

## Positive validations (no action, continuity)
- Name-echo 186/186 CHOICE echoes present, 0 wrong remaps. 1 stale_echo (deck44 s5) = correct
  absent-echo catch (final echo named an absent option), NOT a wrongful downgrade (brief HIGH bar not
  tripped). 3 unparsed_reply = benign long rambles (deck44 s9, s30; deck133 s8), heuristic-answered,
  none discarded a better-than-fallback line, none a lethal miss. empty_reply = 0. defers = 0.
- Rule #1 (equip-once) obeyed 100% — 0 "ALREADY change NOTHING" options taken corpus-wide.
- Lightmine Field, Cruel Edict, wither all handled RULES-CORRECTLY this corpus (no damage-vs-toughness
  gap; the wave-13 Obliterator-inversion blunder did NOT recur).
- E3 re-derived artifact count agrees with surface N in every window; Glimmervoid-fold-in reasoning
  persists but self-corrects, no metalcraft-gated pick flipped.
- Auto-tap color-stranding NOT observed (Glimmervoid + Mox Opal any-color buffer).

## ROTATION VERDICT (per-deck rotation rule) — HOLD: do NOT rotate deck110 out this cycle.
The rule: a veteran rotates out only if its guide takes NO modification AND its seat surfaces NO new
work signal. deck110's guide is FROZEN (8th freeze, terminal) — the first condition is met. But the
seat DID surface new work signal this cycle, so the second is NOT met:
  - E2 equip churn RECURRED as a cleaner, fully-executed A->B->A bounce (still-unfixed engine lever).
  - NEW representation item E6 (Mox Opal option mislabel, 2 witnesses).
  - NEW model watch M1(a) (summoning-sickness in lethal math, n=5).
  - deck110 is the canonical VALIDATOR for two just-shipped fixes (Galvanic 4-dmg, attackers
    reconcile) — both PASS here, and this seat is where the Galvanic wincon and equip-churn behavior
    are exercised at all.
So: HOLD this cycle. deck110 is a NEAR-rotation candidate — once E2 (outcome-delta equip marker) and
E6 (Mox Opal option representation) are addressed and confirmed, and if no new decision-class appears,
it becomes a clean rotation candidate next cycle. The guide itself needs nothing further.
