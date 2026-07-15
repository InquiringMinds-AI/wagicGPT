# Deck-140 review — wave 10 (RESUME wave; first corpus under answer-after-plan protocol)

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus. Sweep every creature board, deploy
a finisher, grind the face to 0 on reach.

Corpus: PRIMARY `matchups-20260715-031825`, 6 deck140 seat games, binary `/tmp/wagic-c8c054be8`,
wave-9 guide, ANSWER-LAST reply protocol (scratch reasoning -> PLAN: -> CHOICE: final line),
max_tokens 2048.

**Record: 4 W / 2 L** (a REBOUND — wave 7 was 0/4+2TO, wave 9 was 0/3L+3TO). NOT the verdict
(user doctrine); context only. Decision-level story: **the count-anchored SWEEP TRIGGER validates a
THIRD corpus with ZERO held-sweeper misses (freeze); the SHROUD belief-kill line finally got real
data and was OBEYED (2 windows); the wave-9 grind-timeouts are now ADJUDICATED WINS; the whole loss
column is still reach-starvation (deck construction); and the answer-after-plan protocol KILLED the
head-first intent-collapse but exposed a MIRROR residual — a trailing PLAN/CHOICE mismatch that
systematically casts a spell when the plan wanted to hold (reply-protocol layer, 4 windows,
non-fatal).**

| game | opp | outcome | deck140 state at end | class |
|---|---|---|---|---|
| vs110 | affinity     | **WIN (adj0)** 18-17 T18 | AHEAD | swept clean; capped-adjudicated |
| vs131 | UWx control  | **WIN (adj0)** 27-26 T32 | AHEAD | control grind; capped-adjudicated |
| vs135 | snow control | **WIN** 32-0 T23        | dominant | Rakdos lethal X=6 |
| vs44  | UB faeries   | **WIN** 13-16 T39       | ahead (opp decked) | shroud handled, Blightsteel deployed |
| vs133 | mono-B blitz | LOSS -6 T15             | dead | reach starvation |
| vs109 | mono-R aggro | LOSS 0 T13              | dead | reach starvation |

Tags: [GUIDE] model-fixable · [STRUCTURAL] deck/infra · [REPLY-PROTOCOL] harness layer.

## Harness / executability sweep (FIRST)
- **Label compliance (answer-after-plan)**: 200/215 ask+priority decisions carry a parseable
  `CHOICE:/ATTACK:/BLOCKS:` final line; the ~15 that don't are the truncation fallbacks below.
  High compliance.
- **Harness indexing is CONSISTENT 1-based**: of 200 CHOICE-bearing decisions, the recorded
  `chosen_text` equals the 1-based interpretation of the written `CHOICE:N` in 192/200; the 8
  exceptions are all `CHOICE: 0` -> recorded "pass"/defer (0 = a distinct pass action, not an index
  error). So a plan/choice mismatch below is the MODEL emitting a number inconsistent with its own
  prose, NOT a harness off-by-one.
- **Fallbacks**: 10 `unparsed_reply` (choice -1) + 1 `empty_reply` in the deck140 seat. Every one
  landed on a land drop, an Elixir activation, or an already-lost/won-anyway position — **NONE changed
  a game outcome** (self-healing confirmed). See Finding 3.1 for the ONE truncation with a real
  (non-fatal) cost that EVADED the fallback flag.
- **X-menu offset**: biggest-X picked at every genuine window (vs135 seq24 X=6 of [6..0]; vs44 seq27
  X=4 of [4..0]). vs135 seq28 picked X=6 of [8..0] but that was **exactly lethal** (opp at 6) and won
  the game — correct, not an offset miss. Dead 6 waves.
- **Latency**: still the driver of long games (median ~10 s, spikes to 120 s under `-j` saturation),
  but the grind games are now ADJUDICATED at cap, not timed out (Finding 5). No timeout claims this
  wave.

## TOP FINDINGS

### 1. [VALIDATED — FREEZE] Count-anchored SWEEP TRIGGER holds a 3rd corpus; ZERO held-sweeper misses
Machine-checked across all 6 games: **there is not a single window where a payable sweeper was
offered with `(creatures: N>=1)` and the pilot chose anything other than the sweeper.** The
wave-9 vs135 t13 boundary-misread class (held Cruel Edict at life 16 reading "16 or less" as strict
<) did NOT recur. The disambiguated wording is being READ and CITED correctly:
- vs109 seq14 (life 8, C=2): *"if the opponent has 1+ creatures and my life is 16 or lower (8 counts),
  I must cast a sweeper immediately"* -> Cast Cruel Edict. Boundary + "counts" phrasing quoted verbatim.
- vs44 seq34 (life 20, C=2): *"My life is 20 (not 16 or lower)... They attacked last turn... the
  condition 'attacked you on any recent turn' is met... I should cast a sweeper."* Both the boundary
  and the "recent turn" wording applied correctly.

**Over-fire into C=0** (the wave-7 overshoot mechanism): 2 sweeper casts landed on an empty board
(vs131 seq11 Pyroclasm, vs133 seq5 Cruel Edict) — but NEITHER is the count-trigger misfiring. In BOTH
the PLAN reads C=0 correctly and concludes HOLD; only the trailing `CHOICE:N` contradicts the plan
(Finding 3.3/3.4). The count is read correctly every time. **Sweeper overshoot stays DEAD
(count-anchored, 3rd corpus). FREEZE.**

### 2. [OBSERVED OBEYED — new data] SHROUD belief-kill line finally tested, and obeyed both ways
The multi-wave shroud watch was NO-DATA through waves 7-9 (no shroud creature on a board the pilot
answered). This corpus **vs44 produced one**: a Sleep-Cursed Faerie (4/4, shroud). Two windows, both
obeyed:
- vs44 seq37 t25 (C=2, shroud Faerie present): *"Cruel Edict... forces a sacrifice (which bypasses
  shroud), I should cast it immediately"* -> Cast Cruel Edict. The "Cruel Edict makes the PLAYER
  sacrifice, it does not target" half of the line, obeyed.
- vs44 seq38 t27 (C=3, shroud Faerie + a 5/5 flyer): *"I need a board wipe that hits all creatures
  regardless of shroud. Wrath of God destroys all creatures."* -> Cast Wrath of God. The "SHROUD does
  NOT stop a sweeper" half, obeyed.

The class is now OBSERVED OBEYED (was untested). Per the pause-aware demotion doctrine the line is
technically eligible for consideration, but it stays at **FULL STRENGTH**: it is a compact rules-fact
(not a war-story or self-citation), the sample is thin (1 seat / 2 windows), no shipped representation
replaces it, and the resume/pause context freezes it. Records the class as validated; **no core
promotion, no demotion.** (The old "shroud core candidate at 2nd seat" cannot advance — still 1 seat.)

### 3. [REPLY-PROTOCOL — route to the A/B, NOT the guide] Answer-after-plan killed head-first collapse but exposed a MIRROR: trailing PLAN/CHOICE mismatch that casts-over-holds
The protocol shift did what it was designed to do — the wave-8/9 head-first intent-collapse (answer
committed BEFORE reasoning, head never revised) is **structurally gone**: there is no head number, the
reasoning precedes the answer, and the wave-9 self-correcting head cases do not recur. BUT a MIRROR
residual persists: the PLAN prose reaches the CORRECT decision, then the trailing `CHOICE:N` emits a
number that contradicts it, and it **systematically selects a SPELL when the plan wanted to HOLD /
cast-nothing.** 4 genuine windows / 215 decisions (~1.9%), all NON-FATAL this corpus:

1. **vs44 seq34 (truncation variant)** — the reply was cut at the 2048-token cap mid-deliberation,
   BEFORE any `CHOICE:` line (a huge land/mana-enumeration loop, 70 s latency). The plan had explicitly
   rejected Rakdos's Return (*"firing Rakdos's Return now is a waste of the kill shot"*) and was heading
   to Cruel Edict — yet the recorded choice was **Cast Rakdos's Return** (option 0). Forced into the
   X-menu at seq35, the pilot dumped **X=0** to minimize the damage of a cast it didn't want ->
   **a kill shot wasted for 0**. Won anyway. This truncation was NOT flagged `unparsed_reply`
   (`fallback: None`) despite having no answer line — it evaded the guard (notes.md #2).
2. **vs133 seq13 (stale-plan)** — the plan reasoned about casting **Damnation** (already spent at
   seq12, not in the offered list [Pyroclasm, Rakdos, Cast nothing]) and stated *"I will hold Rakdos's
   Return... X=3 is not lethal against 19 life."* The trailing `CHOICE: 2` picked **Rakdos's Return**;
   seq14 X=3 -> **the 2nd kill shot wasted** into opp-19. The loss was reach-starvation regardless
   (life 6, no board sweep available).
3. **vs131 seq11** — plan: *"Holding Pyroclasm is correct... I will cast Staff of Nin."* Options were
   only [Pyroclasm, Cast nothing]; `CHOICE: 1` cast **Pyroclasm into `(creatures: 0)`** — a wasted
   sweeper. Won (adj 27-26).
4. **vs133 seq5** — plan: *"the opponent has no creatures to target with Cruel Edict. Holding removal
   is correct... play Akoum Refuge."* Options [Cruel Edict, Cast nothing]; `CHOICE: 1` cast **Cruel
   Edict into `(creatures: 0)`** — fizzled. Won.

Cost: 3 of 4 wasted a card (2 Rakdos kill shots, 1 Pyroclasm, 1 Cruel Edict). The tell for the durable
fix: **the model reliably NAMES its choice in prose** (*"I will hold Rakdos"*, *"I will cast Staff of
Nin"*) — the failure is purely translating that to the numeric index. An A/B that reconciles the
trailing index against the plan's NAMED action (or requires echoing the option TEXT, not a bare number)
catches all 4. **Do NOT re-word the guide over this** (the guide is being read correctly; the leak is
below it) and do NOT score it against a guide rule. deck140 is a 2nd witnessing seat (after deck135)
that the class SURVIVES the protocol shift — the A/B is still DUE, now with a sharper target.

### 4. [TOOLING — for the synthesis / reply-protocol-ab] intent-collapse-metric.py is uncalibrated for answer-last; its flag count is NOT comparable across protocols
Ran `tools/intent-collapse-metric.py` per the brief. Results on the deck140 seat (215 records):
`non-numeric heads: 215/215` (the head parser is now meaningless BY DESIGN — every reply is
prose-first), `last-bare-int differs: 0`, `reversal + concluded-option != choice: 21`. Corpus-wide
it reports 135 (vs the 120 control baseline). **The naive "135 > 120 = intent-collapse got worse" read
is an ARTIFACT.** Hand-auditing all 21 deck140 flags: ~all are FALSE POSITIVES — the
`plan_concluded_option` heuristic matches "hold Rakdos" mentions in the plan tail (deck140 says it in
almost every plan) and mis-indexes multi-option lists, so it flags CORRECT sweeps/holds (e.g. seq38
Wrath, seq23 Damnation, seq16 correct cast-nothing). It also MISSED all 4 genuine mismatches from
Finding 3 (none of vs131 s11 / vs133 s5 / vs133 s13 / vs44 s34 appear in its list). The metric needs a
rewrite for the answer-after-plan protocol before any cross-arm comparison. Route to notes.md #1.

### 5. [INFRA — RESOLVED] Wave-9 grind timeouts are now ADJUDICATED WINS
The wave-9 loss column had 3 timeouts, 2 with deck140 AHEAD (vs110, vs131). This corpus those exact
games are **adj0 WINS at cap**, verified against the translogs: vs110 last decision t18 life 18/17 =
results.tsv `adj0 18 17 18`; vs131 last decision t32 life 27/26 = `adj0 27 26 32` (both gameend absent
= capped, adjudicated by life). The **adjudicate-by-life-at-cap fix** (deck140's own wave-9
general-suggestion #1) SHIPPED and works. No timeout to triage this wave — the correct-but-slow control
grinds now bank as the wins they were.

### 6. [STRUCTURAL — whole loss column, unchanged] Reach starvation vs fast aggro
Both losses are the same deck-construction fact. vs109 (mono-R) and vs133 (mono-B) swept faithfully
(Cruel Edict at life 8 "8 counts"; Damnation at life 14), HELD Rakdos correctly every non-lethal window
(opp at 19-20 both games), but ran out of removal with no early lifegain/blocker; the 1-damage Staff
clock is far too slow against a T13-15 kill. vs109 seq15-19 is the clean picture: at life 8 then life 1,
the ONLY offered spell was the held Rakdos (below lethal) — nothing left to sweep with. Teeth, not text
(notes.md #7). **Do NOT spend guide length on reach.**

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. SWEEP TRIGGER + belief-kills: VALIDATED 3rd corpus. Fire clean (0 held-sweeper misses); over-fire
   into C=0 is reply-protocol, not the trigger. Shroud now tested + obeyed. No re-widening.
2. Displacement: the residual did NOT move to a new play seam — it sits at (a) the CLOSER
   (slow/counterable = deck construction) and (b) the REPLY PROTOCOL (plan/choice mismatch = harness).
   Neither is a displaced guide leak.
3. Rakdos hold (#1): obeyed at every genuine non-lethal window it CHOSE (vs109, vs131 X=13-short-at-cap,
   vs135 held to lethal X=6). The 2 wasted Rakdos casts (Finding 3.1/3.2) were the trailing-CHOICE
   mismatch OVERRIDING a plan that said hold — not the #1 rule being disobeyed by the reasoning.
4. BSZ-X / X-menu / Staff face-ping / Lightmine / Mulligan / Lands: obeyed where exercised. Freeze.
5. No attack-floor issue (deck runs ~no creatures; Blightsteel deployed vs44 seq52).

## WATCH (do NOT legislate — single seat / re-widening risk / precondition-absent)
- **SNOWBALL single evasive threat** (carried wave-8/9): vs44's Sleep-Cursed Faerie grows (printed
  3/3 shown 4/4) but the pilot swept it with Wrath (seq38). No snowball-past-sweep-range LOSS. Hold the
  WATCH; do NOT widen (re-opens the wave-7 overshoot).
- **Within-turn double-sweep at C=0** (carried wave-9): no clean re-offer double-sweep this corpus; the
  two C=0 casts (Finding 1) are single casts via plan/choice mismatch, not a within-turn re-offer loop.
  Hold the WATCH.
- **BSZ partial-sweep guardrail** (wave-9 line): no Black Sun's Zenith cast this corpus (every X-menu
  was Rakdos's Return). Precondition ABSENT -> FREEZE, do not demote.
- **Lightmine second-copy line** (wave-9 line): the strict precondition (a Lightmine already RESOLVED
  on the battlefield + a 2nd copy offered) did NOT occur — the UWx opponent Counterspelled every copy
  (vs131 seq18 events: Lightmine hand->stack, Counterspell, Lightmine stack->graveyard), so
  MyLightmineOnField=0 at all 4 re-casts. The re-casts were correct re-attempts, not double-deploys.
  Precondition-absent -> FREEZE, do not demote.
