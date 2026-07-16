# Deck-110 wave-13 — development notes (self-contained; layer-routed engine/harness ledger)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-183228/`, 6 deck110 seat translogs
(`*ai_baka_deck110-0x*.jsonl`). Binary `/tmp/wagic-a75e2625e` (Force r2/r3 + auto-tap two-pass
+ c3e6d95b2 name-echo + 42e8bc180 BLOCKS:none + 240s timeout). Wave-11 guide FROZEN (live
`deck110_strategy.txt` md5 `00849aa9`). Answer-after-plan + name-echo (`CHOICE: N (name)`).
Choices 1-indexed. Card facts vs `projects/mtg/bin/Res/sets/primitives/mtg.txt`. Every item
carries a repro; engine/harness items belong HERE, never in prompt/guide text.

## E4 — NEW, HIGH — CARD-SCRIPT BUG: Galvanic Blast metalcraft +2 never applies; the Blast deals a flat 2 corpus-wide.

STATUS: newly CONFIRMED this wave (missed in waves ≤11, which trusted the model's asserted "4
damage" rather than the engine's `Damage:` events). OUTCOME-BEARING on the deck's core wincon.

REPRO (clean): deck133 seat, T10 MP2, my19/op16.
- s21 prompt surface: `Artifacts in play: you 4` (battlefield = 2× Darksteel Citadel [artifact]
  + 2× Cranial Plating [artifact]; 3× Glimmervoid correctly untagged). Metalcraft ON → Oracle
  Galvanic Blast = 4.
- s23 game log: `- Damage: 2 dealt by Galvanic Blast to Phyrexian Obliterator`. Bonus not
  applied.
CORPUS INVARIANT: grep of `Damage: N dealt by Galvanic Blast` across all 6 deck110 game logs =
ONLY `N=2`, in every game, on artifact-flooded boards. It NEVER dealt 4.

CARD SCRIPT (`mtg.txt`, Galvanic Blast):
```
target=anytarget
auto=damage:2
auto=aslongas(artifact|myBattlefield) damage:2 >2
text=... deals 2 ... Metalcraft - deals 4 ... instead if you control three or more artifacts.
```
ROOT CAUSE: the metalcraft line uses `aslongas(...) EFFECT >2`, the engine's CONTINUOUS
static-effect construct (correct for Ardent Recruit `2/2`, Etched Champion `protection from
red`, Auriok Edgewright `double strike` — all in this file). A one-shot `damage:2` under
`aslongas` is instantiated as a dead continuous effect and never resolves as damage. The
engine's ONE-SHOT metalcraft idiom carries `oneshot`:
  - Blade-Tribe Berserkers: `auto=aslongas(artifact|myBattlefield) 3/3 >2 ueot oneshot`
  - Bleak Coven Vampires:   `auto=aslongas(...) ...life:-4... >2 oneshot`
Galvanic Blast's metalcraft line has NO `oneshot` and no explicit target inheritance for the
second `damage:2`.
FIX PATH (data, no recompile — hand to wagic-cardscript skill, DO NOT ship blind): express the
Oracle as either (a) a metalcraft-gated single damage value, or (b) base `damage:2` + a
`oneshot` conditional `damage:2` that inherits/re-targets the same `anytarget`. Verify the fixed
card actually logs `Damage: 4` at 3+ artifacts and `Damage: 2` at ≤2 before considering it done
(the wave-11 miss was believing the intent; require the game-log RESULT).
IMPACT: the guide's LETHAL CHECK counts "+4 per Galvanic Blast in hand (metalcraft)"; the engine
delivers 2, so every Blast-inclusive lethal reach is over-counted by 2×(Blasts). Guide is
Oracle-CORRECT — fixing the card restores guide correctness. NO guide text. Priority HIGH.
Cross-deck: any deck running Galvanic Blast (and audit other metalcraft ONE-SHOT cards for the
same missing-`oneshot` shape).

## E2 — ENGINE/REPRESENTATION (unfixed, ledger #4): equip re-offer no-op marker is TARGET-IDENTITY-keyed, blind to net-zero MOVES between carriers. RECURRED milder (non-outcome).

REPRO (net-zero, in a WIN) — deck131 seat, T9 MP1, opp at 5. Plating-A on Signal Pest,
Plating-B on Ornithopter (both 6-power).
- s28: options [Equip→Ornithopter, Equip→Signal Pest (ALREADY), Equip→Signal Pest(dup)]. Model
  equips Ornithopter (the naked-of-second one). Correct, marker obeyed.
- s29: options [Equip→Ornithopter (ALREADY), Equip→Signal Pest]. Model MOVES Plating-B off
  Ornithopter onto Signal Pest ("move it to Signal Pest makes it a 6/1"). NET-ZERO relocation —
  Signal Pest option is UNmarked (a "change") but the change adds 0 net board power. 1 wasted
  `{1}`.
- s30: model begins weighing moving it BACK ("Case A on Ornithopter: 7. Case B on Signal Pest:
  7 — identical"): the A→B→A bounce, INTERRUPTED by the parse guard (s30 unparsed, choice -1,
  heuristic fallback). Both configs lethal → no outcome change.
ROOT CAUSE (unchanged): no-op predicate = "already attached to THIS target," not the
no/low-value predicate. LEVER: compute the marker from OUTCOME DELTA — mark/suppress
"Equip … targeting X" whenever moving the equipment to X would not raise X's resulting power
above the current carrier's; specifically suppress moving a second Plating onto an
already-equipped creature when a naked/higher-value carrier exists. Owner: `AIPlayerGPT.cpp`
equip-option serialization. Priority MEDIUM. NON-CHURN correct contrasts this wave: deck109
s21/s22/s23 (three Platings, three naked carriers, ALREADY re-picks DECLINED), deck135 s14/s15
(two-Plating stack), deck131 s17/s18/s21 (lethal alpha spread). Classify by outcome-delta, not
raw same-name count. NO guide text — Rule #1 already forbids it; pilot has no surfaced gain
signal.

## E5 — HARNESS: attackers `ATTACK: <name>` shape unparsed; add a name→A-index reconcile arm.

The `ATTACK:` protocol wants A-indices, but the model sometimes emits creature NAMES.
REPROS: deck135 s9 T3 `ATTACK: Ornithopter, Signal Pest` → unparsed (choice -1) → heuristic
fallback (deck135 WON, non-outcome). deck131 s20 T5 `ATTACK: A1, A2, Memnite` → MIXED index+name
→ PARSED (choice 2). Mirrors the brief's noted `ATTACK: Hellrider` shape and the name-echo
reconcile that already exists for CHOICE lines. LEVER: on the attackers parse path, after
A-index extraction, reconcile any residual NAME token to its unique battlefield-attacker match
(same logic as parseChoice's name arm). Owner: attackers parse path. Priority LOW-MED. NO guide
text.

## E3 — REPRESENTATION: artifact-tag count AGREES with surface N (no offset); fold-in reasoning persists, NON-biting.

Surface `Artifacts in play: you N` correct in every window; `[artifact]` on all battlefield
artifacts (creatures + Darksteel Citadel + Mox Opal + Cranial Plating), Glimmervoid + basics
untagged. Residual fold-in persists (model re-derives by hand and folds Glimmervoids in, e.g.
deck133 s21 "2 Citadels, 2 Glimmervoids, 2 Platings? ... recount") but self-corrects to the
right metalcraft verdict — NO metalcraft-gated pick flipped on a wrong count. NOTE: the deck133
s21 Blast shortfall is E4 (engine bug), NOT an E3 miscount — surface N=4 was correct and the
model's metalcraft-ON conclusion was correct; the engine simply failed to add the +2.
Representation-beats-instruction wall, now 5 waves, non-outcome. GUIDE FROZEN. Hand-card
type-tag sub-gap (artifact CREATURES in hand still show only P/T) UNCHANGED, no new mis-typing
instance — low-priority micro-lever, `AIPlayerGPT.cpp` hand serialization.

## M1 — MODEL / REPLY-PROTOCOL WATCHES.

(a) **Target sub-prompt disorientation — name-echo index-fallback neutralized it (correct by
design).** deck131 s6→s7 T1: s6 commits "Cast Galvanic Blast targeting the opponent"
(`CHOICE: 1`); s7 the engine asks the Blast's TARGET (prompt explicitly headed "Choose the
target for Galvanic Blast"). Model loses the thread, reasons as a fresh main phase, emits
`CHOICE: 1 (Cast Cranial Plating)` — an echo naming NO option in the target list. Reconcile
found no echo match → fell back to INDEX (index wins) → index 1 = "The opponent" = the s6
target. Right target, wrong reasoning. POSITIVE for index-wins design (absorbed a model
confusion) but FRAGILE — would misfire if the intended target were a non-index-1 slot. WATCH,
n=1. No wrong echo-remap (brief HIGH bar not tripped).

(b) **Model rules gaps (deck133 s21/s22) — burn-vs-toughness + inverted trigger.** Fired
Galvanic Blast at a 5/5 Phyrexian Obliterator believing (i) damage reduces toughness ("to
1/5"), (ii) blasting it AVOIDS its "sacrifice permanents" trigger. Both wrong: marked damage
wears off (still a 5/5), and Wagic's Obliterator ("Whenever a source deals damage ... that
source's controller sacrifices that many permanents") forced DECK110 to sacrifice (s23:
"Choose target 1 of exactly 2 for Sacrifice 1: Island / Darksteel Citadel"). Wasted its only
interaction + self-sacrificed, in a LOSS (-5). MODEL knowledge gap (representation gave `(5/5)`
+ a truncated Obliterator text; model reasoned past both). Cross-deck; harness could annotate
target toughness. NOT guide-fixable.

(c) **Blocker over-block (deck135 s17) — MODEL, representation-wall on the block seam.** At 20
life vs a lone 2/2, blocked with the 10/2 equipped attacker (both die) DESPITE the prompt
annotating "block only where the trade favors you; taking damage while ahead is often correct"
+ chump option marked "[deals 0 — only absorbs damage]" + "NOT lethal." "I must block the
listed attacker" (blocking is never mandatory). Plausibly cost the comfortable win (deck135
adjudicated 10/9). Single-seat → synthesis cross-seat question (do other aggro seats over-block
on defense now that the seam fires?). If cross-seat, sharpen the block-value prompt annotation,
NOT a guide line. Benign "must-block" recurrences: deck135 s10 (even trade), deck44 s44
(defensible chumps).

## DC1 — DECK-CONSTRUCTION / META (standing, reaffirmed — owns the loss column).
deck110's 3 losses share the precondition the guide cannot create: few creatures / ground-out
or board-wiped. vs133 (empty board into Obliterator+Liliana, -5). vs109 (Hellrider race, at 4
at the cap — correctly behind). vs140 (Wipe Them Out! board control — only 2 attacker windows
in 24 turns, adjudicated 14/18). Not a guide task. If ever tuned: trim ~0-power filler + Mox/
land toward mid bodies + reach/evasion redundancy to cut flood + single-threat + grind + wipe
exposure at once.

## Positive validations (no action, continuity)
- Name-echo protocol SHIPPED and works: 177/177 CHOICE echoes present at this seat, 0 wrong
  remaps; the one echo-index conflict (M1a) resolved correctly to index.
- empty_reply = 0 (Spark healthy this corpus); 12 unparsed rambles all heuristic-answered, none
  discarded a better-than-fallback line; 4 defers all benign Glimmervoid land auto-plays (seq3).
- BLOCKS: None (deck109 s24) = deliberate all-decline race call, not a misparse.
- Auto-tap color-stranding NOT observed at this seat (Glimmervoid + Mox Opal any-color buffer;
  every listed cast fired).
- E3 re-derived artifact count agrees with surface N in every window.
