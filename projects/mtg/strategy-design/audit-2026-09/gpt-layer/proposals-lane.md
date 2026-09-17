# GPT-layer proposals lane — P3, P4, P6, P8, P5, P10, P9, P11, P12 (LEDGER-v2 §4/§6 as corrected by §7)

Branch `gpt-proposals` from master `b2723ba49` (the brief's `9aa2c59ef` plus the
ledger-correction commit), worktree
`/home/magi/Projects/wagicGPT/worktrees/lanes/gpt-proposals`. One commit per item,
in the corrected order. Bytes are measured by rebuilding the recorded boards /
events of the wave-80 corpus (`matchups-20260912-074153-final`, 2,083-2,135
prompts, 28,033,442 prompt bytes) through the new emitters, in PARSETEST pins
and in Python replays of the same fold rules; the per-item figures below are on
that corpus, independently of each other (the hand-line items overlap a little,
so they do not add exactly).

| item | commit | what | measured delta on the corpus |
|---|---|---|---|
| P3 | `61dc578c6` | identical hand entries fold to `Name xN` + tail | hand lines 1,591,224 → 1,247,587 B: **−343,637 B = 1.23 %** of all prompt bytes |
| P4 | `1ccead82f` | the two dominant hand tags shortened; one legend per hand line | hand lines 1,575,089 → 1,195,495 B: **−379,594 B = 1.35 %** |
| P6 | `d71bef665` | one clause per number: mana bill, fit clause, ATTACK TOTAL, blockers header, per-row blocker cost | **−85,270 B = 0.30 %** (fit 66,279; mana 12,363; attack 5,035; per-row 1,104; blockers header 489) |
| P8 | `670c4e8a8` | only the range grammar the menu used | 99 prompts, note 165,924 → ~37,800 B: **−128,142 B = 0.46 %** |
| P5 | `90964fa26` | narration compaction (the owner's register) | recorded event streams 454,822 → 309,787 B: **fraction 0.681** (narration was 48.5 % of all prompt bytes → ≈ −15 % of all prompt bytes) |
| P10 | `74aa3d269` | whole-order blocker ORDERING answer | corpus: 0 ordering windows (no multi-blocker gang on a GPT attacker); probe: see §P10 |
| P9 | `b404326c0` | land drop folded into the casting menu, behind a switch | corpus: 332 land asks, 241 of them followed by a cast ask in the same turn; probe round trips: see §P9 |
| P11 | `5f8ffe643` | telemetry: one `window` kind, `seam` a field; side records only where there is no window | shape change; every counter kept; every tool reads both shapes |
| P12 | `8c06bce46` | AIPlayerGPT.cpp split by responsibility, a move | PARSETEST byte-identical (cmp of 9,148-line logs); 154 narrative paragraphs (1,565 lines) to comment-archaeology.md |

## P3 — collapse identical hand entries (`61dc578c6`)

`joinZoneEntries` (`src/AIPlayerGPT.cpp`, the hand branch `collapse == false`)
folds every hand entry whose NAME and byte-identical FACT TAIL match into one
`Name xN` entry at the first copy's position. Equality is the whole tail (cost,
type, castability tag, MDFC back-face tag), so a copy whose live facts differ
keeps its `(copy R of N in your hand)` entry and so do its namesakes; the fold is
gated on non-`#N` handles so the battlefield grammar and its W42-4 pins are
untouched. Seams that pick ONE instance (discard, bottom, reveal, hand-removal
rows) build their own numbered rows through `listCopyRank` and are unchanged —
the instances stay distinguishable where a seam asks for one.

Pins (`#W82-P3`, +11): the recorded `123v126` seq-4 hand renders 649 → 342 B
with the three doubled names folded and singletons untouched; a differing copy
keeps its ordinal; a partial agreement folds nothing; the battlefield collapse
is byte-identical.

Corpus: 2,135 hand lines, 1,257 with copies, 3,661 copy-tagged entries.

## P4 — the two dominant hand tags (`1ccead82f`)

`handCastabilityTag`: `[no cast row now: sorcery speed - only in your own main
phase with an empty stack; right now <why>]` → `[no cast row now: sorcery
speed]`; the HELD instant tag (258 B) → `[no cast row now: no legal target -
HELD]`. The rule text and #W67-AW's failing half, and T8's HELD scope lesson,
print ONCE per hand line in a new `Hand tags:` legend (`handTagLegend`, pure),
only when a card on the line carries the tag. Per-card exceptions (cannot-pay,
restricted, castable now / answered, back face) are untouched. Correction: the
sorcery-speed no-target tag says "no legal target on the board or the stack" — a
counterspell's legal target is a stack object. The per-card tags carry no board
number, so the ask key (whose board half the hand line is) moves on nothing new.

Pins: `#W67-AW M2` and `#W79-DB T8` inverted to the new shape; +10 `#W82-P4`.
Five sorceries in an upkeep: 810 → 333 B.

Note for the deck reviewer (not edited by this lane — guides are reviewer-owned):
`deck125_strategy.txt` line 33 quotes the old HELD tag head verbatim
(`"[no cast row now: it must have a target and there is no legal target on the
board]"`); the new tag keeps the head `[no cast row now:` and the words `no legal
target`, so the teach still binds, but the quotation is no longer byte-exact.

## P6 — one clause per number on a row (`d71bef665`)

* Mana bill: `foldManaBillClauses` now absorbs the adjacent `{spends K of your M
  ... this turn; NAME COST in your hand needs N}` strand group as well as
  `{paying this taps: ...}` — the strand group sat BETWEEN the two the wave-74
  fold joined, so on the rows with all three the fold never fired and M printed
  twice. One bracket: `{leaves L of your M untapped mana sources untapped; spends K
  this turn - NAME COST in your hand needs N; paying this taps: ...}` — spent (K)
  and left (L) both kept, M once. Brace-balanced group scan (a strand carries a
  mana cost inside it).
* Fit clause: printed ONLY when a row is lost (owner ruling), and INSIDE the row's
  bill (`; row 2 needs more mana sources than that`) so the sources-left number
  is not restated; rows without a bill keep the standalone tag.
* ATTACK TOTAL: `... puts them at 13 - NOT lethal. All of it lands whatever they
  block.` — total, resulting life and verdict in one sentence; a floor equal to
  the total names no second life figure; a floor below the total keeps its own.
* Blockers header once: `combatDamageForecast(..., figureOnFrame)` — when the
  frame's latched INCOMING THIS COMBAT figures equal the seam's, the tail prints
  the verdict without numbers (nothing when lethal — the frame already says
  KILLS/SURVIVE); the poison, life-loop and compulsory-draw branches print in
  full because those facts live only there.
* `{after this combat: you control 1 fewer blocker ...}` once per B row, not once
  per pairing.

Pins: 16 inverted (`#W54-C D18`, `#W60-L`, `#W61-R`, `#W67-AW M1`, `#W74-CC O5`,
`#W76-CO Q4b`), +21 `#W82-P6`.

## P8 — only the range grammar the menu used (`670c4e8a8`)

`joinNumberedRows` records the shorthand forms it printed (bitmask: `#N` range,
plain no-ordinal run, copy-tag range, source block, X climbing, X descending,
repeat-pay band) and `optionRangeNote(forms)` composes only the matching
sentences of `kOptionRangeNote`, in its own order and words — all bits set is
byte-identical to the constant (pinned). All four call sites use it. Corpus: 99
prompts carried the 1,676-B paragraph; their forms were `#N` 53, copies 25,
source block 8, X-up 7 (Astra's "0 use the X forms" was wrong by 7), plain 3,
band 3. The `#N`-only note is 403 B.

Pins: +14 `#W82-P8`.

## P5 — narration compaction (`90964fa26`)

`compactNarration()` is a pure render over the append-only event log into the
register the owner approved on the `152v125` seq-15 sample: `T7 (you): drew X;
played Y.` (the draw-step draw and the first land drop on the turn line), then
one line per phase in which something happened (`  Main 1: ...`), the combat
phases merged into `  Attack:`; pay/cast/resolve fold to one event with the cost
in parentheses (`cast Elite Spellbinder ({2}{w}, 3 sources) -> resolved` — the
source count is kept, and named sources are kept when the line named them);
target/use/result fold to one (`its ETB exiled Supreme Verdict from their
hand`); damage carries the new life total (`-> 1 damage, opp 18`). Order within
a phase is preserved (a land after a cast stays after the cast); every line
family without a fold rule renders in its phase in the log's own words with the
seat voice shortened (`your`/`their`/`you`/`opp`) — nothing is dropped. The
recorded log (`mNarration`) and the translog `events` delta are untouched: this
is a render at prompt assembly. `WAGIC_GPT_NARRATION_V1=1` restores the previous
render byte for byte (the A/B switch).

Pins (`#W82-P5`, +12): the owner's sample renders to the approved text BYTE FOR
BYTE (1,021 → 365 B); order preserved; an upkeep draw stays in its phase; every
one of the 33 corpus line families keeps its names and numbers through the
compactor; unknown families pass verbatim; trim marker and pregame lines
verbatim in place; an unmatched payment is not lost; an empty turn says so.

Measurement: `WAGIC_GPT_COMPACT_FILE=<log>` under `WAGIC_GPT_PARSETEST=1`
renders a recorded event log (desktop PARSETEST builds). The 42 seat streams of
the corpus (every record's `events` delta concatenated) replayed through the
real compactor: **454,822 → 309,787 B, fraction 0.681**. Narration was 48.5 % of
all prompt bytes, so this is ≈ 15 % of the prompt.

## P10 — whole-order blocker ORDERING answer (`74aa3d269`)

`orderBlockers` asks ONCE for the whole permutation of one attacker's blockers.
`gptOrderLineFromReply` reads the first clean `ORDER:` line that is a full
permutation of 1..n (`B2, B1, B3`; lower case, bare numbers and spaces read the
same), or — the robust-parser ruling — a label-less line that is nothing but the
permutation AFTER the plan (never before it). Any partial or malformed answer is
recorded (`fallback: order_incomplete_per_pick`) and the per-pick asks take over
unchanged: no blocker and no ordering is ever lost. Turn-stamped per-attacker
done / per-pick sets keep the tick re-entry from re-asking.

The ONE approved reply-shape extension, verbatim, in both editions of the golden
text (`bin/Res/ai/gpt/reply-protocol.txt` and `kReplyProtocol`, still pinned
byte-identical):

> `Use exactly the label the decision asks for (CHOICE: for numbered choices, ATTACK: for attack declarations, BLOCKS: for block assignments, PUT: for the card-number seams, ORDER: for the combat damage order).`

> `ORDER: takes the B# labels of that one attacker's blockers, all of them, in the order damage is dealt ("ORDER: B2, B1, B3").`

(the first sentence gained `, ORDER: for the combat damage order`; the second is
new and follows the ATTACK/BLOCKS sentence). `check-reply-instructions.py` now
scans the `ORDER:` label; `corpus-stats.py` counts `order` as a round-trip kind.

Round trips: the corpus has 0 ordering windows (no GPT attacker was blocked by
two or more creatures in it), so the saving cannot be counted there; on the probe
see §Probe.

Pins: +12 `#W82-P10`.

## P9 — land drop folded into the casting menu (`b404326c0`)

On the seat's own main phase the land branch of `FindCardToPlay` stages the
playable lands and delegates to the cast branch, which appends the `Play <land>`
rows after the cast rows (before the hold/decline rows) and says on the question
that a land row plays that land instead of casting and that the casting
decision is asked again after it enters. One action per answer: a land pick is
committed through the same validation the separate ask used
(`commitLandDropPick`: MDFC face pre-answer, forced candidate) and the cast
question re-puts on the changed board; a spell pick is the cast; the decline row
reads `... - and play no land in this window`. If the cast branch put NO menu to
the model (no castable spell, a hold standing, a no-progress marker), the
separate land ask runs exactly as before — the land drop is never lost. Keys are
board state + question, so the combined menu is key-stable.

**Revert switch**: `kLandDropInCastMenu` (compile-time constant, default on) and
`WAGIC_GPT_LAND_SEPARATE=1` (runtime env) restore the separate land ask.

No pure surface to pin (seam choreography); the probe is the instrument
(§Probe: combined windows, land rows taken, separate asks that still ran, round
trips per own main phase).

## P11 — telemetry schema (`5f8ffe643`)

Every decision the model was asked writes `kind: window` with `seam` ∈ {ask,
priority, attackers, blockers, discard, reveal, bottom, order} and `window_seq`
(the window's ordinal), so side events join by number. Side records exist only
for events with no window: `hold_event`, `recovery`, `gamestart`/`gameend`/
`system`, `defer` (an answer the engine could not execute, no prompt),
`ask_replay` (the sidecar), and a `forced_close` or `wall_miss` whose window never
wrote a record. A forced close whose window does write a record rides that
record as a field (`forced_close: [events]`; `writeForceCloseRecord` stashes,
`flushForceCloseFold` joins by `window_seq` or emits the orphan as a side record
in file order and at gameend); a wall miss that joins its window's record carries
`wall_miss: {class, latency_ms}` instead of a bare `1`. Every counter is kept
(gameend rows unchanged); this is a shape change, not a deletion.

Tools, each reading BOTH shapes: `corpus-stats.py` (normalises `window`+`seam`
to the seam for its census and reports the folded events; selftest OK),
`selfplay-harness.sh` (the stall / loop readers skip `hold_event` and
`forced_close` side records, the summary names the seam; `--selftest`: harvest
16/0, pilot-stall 21/0, window-loop 14/0), `regime-gate.py` (gates window
records as before; case added; 35 checks / 0 failed), `runmanifest.py` (reads no
kinds; selftest OK), `prompt-byte-census.py` (kind-agnostic; selftest OK),
`w81-dk-window-probe.sh` (reads stderr strings only; nothing to change).

Pins: +4 `#W82-P11` (`translogRecordShape`).

## P12 — the split (`8c06bce46`)

`AIPlayerGPT.cpp` (63,151 lines) → five translation units, every unit moved
verbatim (a tool walked the file into top-level units, assigned each by region,
and re-wrapped the anonymous namespaces around the units that stay file-local):

| file | lines | responsibility |
|---|---:|---|
| `src/AIPlayerGPT.cpp` | 41,174 | prompt facts + presentation: renders, narration, `serializeGameState`, holds/verdicts, prompt build |
| `src/AIPlayerGPTTransport.cpp` | 2,922 | the async request lifecycle and transport: worker, slot keys, `pollCompletion`, retry/force-close legs |
| `src/AIPlayerGPTTranslog.cpp` | 2,324 | telemetry records, gameend counters, notices |
| `src/AIPlayerGPTParse.cpp` | 2,862 | answer selection and parsing: PLAN split, coded lines, latches, `parseChoice` |
| `src/AIPlayerGPTSeams.cpp` | 16,536 | the decision seams and `askModel` |
| `include/AIPlayerGPTInternal.h` | 644 | the 458 file-static / anonymous-namespace helpers, constants and 7 types a region now compiled apart references — definitions unchanged (`static` dropped; default arguments stated once, taken from the prototype that carried them; overloads declared individually) |

The PARSETEST forwarders follow their targets. Build lists: `wagic.pri`
(sources + the header), `Makefile` (PSP objs), `CMakeLists.txt` (Vita),
`Android/jni/Android.mk`, `Makefile.win` (objects + compile rules) — exactly as
split-parsetest.md did; `Makefile.sdl` regenerated with `qmake6 wagic-SDL.pro
CONFIG+=debug -o Makefile.sdl`. `check-reply-instructions.py` scans every
`AIPlayerGPT*.cpp` except the corpus file (5 source files).

Pins: PARSETEST output **byte-identical** before and after the split (`cmp
~/.gatelogs/p12-parsetest-before.log ~/.gatelogs/p12-parsetest-after.log`, both
9,148 lines, `7314 passed, 0 failed`), and byte-identical again after the
comment pass; `check-ctor-init` OK (123 files). The full-prompt pin is the
PARSETEST corpus's own prompt-assembly fixtures (the `#W70-BL E1` tail gates and
the stub-board sections render whole asks) plus the P5 instrument replaying the
42 recorded event streams to the same bytes; a fresh stub game is not a fixed
record (the engine has no seed env), so no game-level prompt diff was taken —
stated plainly.

Comment archaeology: `strategy-design/audit-2026-09/gpt-layer/comment-archaeology.md`
(2,339 lines) holds 154 narrative paragraphs (1,565 lines — seq/game citations,
wave-by-wave measurements, corpus counts) moved verbatim from BEHIND their lead
paragraphs; each is replaced in the code by a one-line pointer to its anchor.
The lead paragraph of every block (the WHY) stays, and any paragraph carrying a
rules citation, an owner ruling, an invariant or a doctrine rail was left in
place. Comment lines 21,858 → 20,447. This is deliberately conservative: a
heuristic cannot tell every WHY from every measurement, so it moved only
paragraphs that are plainly measurement and never the first.

## Gate

All on the `make -B` binary of `8c06bce46`; the hermetic legs were run once more by the
queued unit `gp-gate-after-probe` after the probe (`~/.gatelogs/gp-gate.log`) and its
numbers are the ones here.

| gate | result |
|---|---|
| (a) suite, `WAGIC_TESTSUITE_THREADS=1`, detached unit, foreground until-loop | **1312 tests (0 failed), 81 AI tests (0 failed)** — baseline exactly; `==Test Failed !==` 0, `==Test timed out` 0; no flake fired, no solo rerun needed (my own run `~/.gatelogs/gpt-proposals-suite.log` and the queued gate agree) |
| (b) PARSETEST | **7314 passed, 0 failed** (baseline 7231; +83 over the nine items, −0 lost: every inverted pin was rewritten, none deleted) |
| (c) check-ctor-init | OK (123 files — the four new TUs; `mOrderDoneTurn`, `mCastMenuAsked` in the ctor list) |
| (c) check-reply-instructions | OK (5 source files, 37 guides — it now scans every AIPlayerGPT TU) |
| (c) tool selftests | corpus-stats OK; regime-gate 35 checks / 0 failed; runmanifest OK; prompt-byte-census OK; harness `--selftest` harvest 16/0, pilot-stall 21/0, window-loop 14/0 |
| (c) `git diff \| grep -c $'\357\277\275'` | 0 |
| (d) heuristic A/B vs `archives/baka-ab/62a5733ec`, three independent 210-game samples | **0 unfinished in all three (630 games), PASS (hard) ×3.** Sample 1 (`~/.gatelogs/gpt-proposals-baka`): 123 +0.97, 125 +0.00, 126 +1.61, **130 −2.39 FLAG**, 146 −0.52, 152 +1.05, 162 −1.02. Sample 2 (`-baka2`): 123 +1.87, 125 +0.23, 126 +0.23, **130 −1.03**, 146 −0.75, 152 −0.63, 162 −0.08. Sample 3 (the queued gate): 123 +0.30, 125 +0.46, 126 +0.00, **130 +0.25**, 146 −0.75, 152 −1.56, 162 +1.38 — no flag. |

The deck-130 flag is sampling noise: `git diff --stat b2723ba49..HEAD -- projects/mtg/src
projects/mtg/include ':!*AIPlayerGPT*'` is EMPTY — this branch touches no engine, Baka,
AllAbilities, TargetChooser, ActionLayer, LegalActions or DecisionContract file, so the
heuristic path is byte-identical to the accepted binary; the same deck reads −2.39, −1.03
and +0.25 across three samples of that identical path, which is what noise does and a
behaviour shift does not.

## Probe

Two full games on the pilot (`qwen36-35b-a3b` at `http://100.116.136.74:8084`,
`--thinking on -r 1 -T 0 -j 1`), each its own detached unit (a two-deck pool at `-r 1` is
ONE game per run, hence two runs: `-p 152,125` → `~/.gatelogs/gpt-proposals-probe`,
`-p 125,152` → `~/.gatelogs/gpt-proposals-probe2`); key fetched into a scratch file,
never printed, deleted afterwards. Regime gate PASS on both.

| | game 1 (probe) | game 2 (probe2) |
|---|---|---|
| result | **deck125 beats deck152** | **deck152 beats deck125** |
| life at end | 114 vs 3 | 20 vs −2 |
| turn | 55 | 13 |
| natural end | yes | yes |

Census, `python3 projects/mtg/tools/corpus-stats.py ~/.gatelogs/gpt-proposals-probe ~/.gatelogs/gpt-proposals-probe2` (the P11-updated tool):

```
dirs 2 clean seat logs 4 = games 2
ALL KINDS {'gamestart': 4, 'system': 4, 'ask': 433, 'hold_event': 136, 'priority': 134,
           'discard': 9, 'attackers': 11, 'gameend': 4, 'reveal': 7, 'defer': 3}
RECORD KINDS SUM 733  ...of which defer 3, hold_event 136, engine_answered 0
FOLDED EVENTS on window records (joined by window_seq): {'forced_close': 6}
window-shaped records (kind=window + seam): 594
fallbacks ALL kinds {('defer', 'menu_not_askable'): 3}
fallbacks (ask seam) {} total 0
ask records 433 model decisions 433
with reasoning 433 thinking values {'on'}
CORPUS-WIDE: model decisions 594 off_protocol_bytes>0 16 plan_line_missing 17 latency p50 36.1 s
ASK-SEAM ONLY: reply_truncated 0 off_protocol_bytes>0 13 plan_line_missing 14 action_before_plan 0
latency p50 34.5 s p95 64.2 s n=433
reasoning_chars median 6035 max 26538
protocol_deviation classes {'compliant': 577, 'unlabelled_plan': 16, 'plan_absent': 1}
decisions/turn 3.18
CENSUS CHECK: gameend protocol_replies 594 vs record-kind sum 733
              (difference 139 = defer 3, hold_event 136)
CENSUS CHECK RECONCILES.
WINS per deck: deck152 1/2 deck125 1/2
```

Against the gate: **0 fallbacks of any kind** (the three `defer` records are
`menu_not_askable` — an engine menu with no answerable shape, not a model failure, and
not on the ask seam); **reasoning on all 594 records**, `thinking: on`;
**`plan_line_missing` 17/594 = 2.9 %** (wave-80: 2.9 %); **0 unparsed** — the only
deviation classes are PLAN-line shapes, every answer was read. P11 is visible: 594
`window` records with their seam, 6 forced closes folded onto the window records they
joined (`forced_close_events: 6`, `forced_close_unrecorded: 0`, and `phase2_answer_recovered: 6`
— every close recovered its answer), 136 `hold_event` side records reconciling through
`hold_events: 136`.

**P9 (land drop from the combined menu) — EXERCISED.** 48 casting windows carried
`Play <land>` rows beside the cast rows and **33 of them were answered with a land row**;
23 land drops still went through the separate ask (the fall-through when the cast branch
had no menu to put — no castable spell, a hold standing), 265 windows were cast-only. Round
trips per own-main-phase (seat, turn) — cast + land windows, own turn: **corpus 2.93 (n=523)
→ probe 2.12 (n=8)**; the sample is small (only main phases where the pilot took a land AND
a cast are counted), so read it as direction, not a rate. The early diagnosis (asks per seat
/ turn / phase from the live logs, the 2026-09-09 rule) found the deck152 seat of game 1
at 11-14 asks on five of its main phases (corpus max 12, p99 6); I read the 13-ask turn-11
phase record by record: Teferi +1 → mode choice → target → the cast menu re-put after each
resolution → a hold — thirteen DIFFERENT questions on a moving board (`declined this exact
list N times` never passed 6), the shape of a walker-plus-modal deck on a long turn, not a
window loop; `menu_pass_no_progress: 0`, `ask_replays_refused: 0`, both games ended.

**P10 (blocker ordering) — NOT EXERCISED, plainly.** `ORDER windows 0`: in neither game
was a GPT attacker blocked by two or more creatures (11 attackers windows, 0 blockers
windows with a gang), so the whole-order answer was never asked live. Its evidence is the
12 `#W82-P10` parser pins and the fallback path by construction; the corpus had 0
ordering windows too, so the round-trip saving is unmeasured on real play.

## Weakest evidence

The weakest claim is **P9's**: that a combined land+cast menu does not degrade play. The
owner's ruling is "try it. Revert if it degrades behavior", and two games cannot say
whether it degrades — game 1 went 55 turns and the seat that took 14 of the 33 combined
land rows lost 3-114, game 2 went 13 turns and the seat that took the rest won; a 21-game
corpus with the guides' seat reviewers is the instrument, and the revert is one env var
(`WAGIC_GPT_LAND_SEPARATE=1`) or one constant (`kLandDropInCastMenu`). Second: **P10 never
fired live** — the parser is pinned twelve ways but no real reply was ever read by it; if a
model writes something the pins did not anticipate (a permutation with names, say), the
fallback is the old per-pick asks, so the failure mode is a lost saving, never a lost
option. Third: **P5's compactor is measured on recorded events but the model has only
played two games against it**; the family-coverage pin proves every corpus line family
keeps its names and numbers, not that a family the corpus never produced does — an
unknown family renders verbatim, so the risk is a verbose line, not a dropped fact.
Fourth: **P12's zero-behaviour-change claim rests on PARSETEST byte-identity and the two
probe games**, not on a fixed-record prompt diff — the engine has no seed env, so a live
game is not a fixed record; the split moved every unit verbatim and the linker would have
refused a lost or doubled definition, but a default argument mis-stated in
`AIPlayerGPTInternal.h` would compile and change a render silently. I took defaults from
the prototypes that carried them and PARSETEST renders through most of those helpers, but
"most" is not "all". Fifth, smaller: the **comment archaeology is a heuristic** (a paragraph
that reads as measurement, never a block's first) — some measurements stayed in the code
and no WHY was moved, which is the conservative side to err on.
