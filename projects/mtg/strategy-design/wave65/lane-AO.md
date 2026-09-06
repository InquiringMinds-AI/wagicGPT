# Lane AO — G8 (the reply rule) + the MED items it carries

Base `537070ac2` (wave-64 close). Branch `w65-lane-AO`, worktree `worktrees/lanes/w65-AO`.
One file: `projects/mtg/src/AIPlayerGPT.cpp`. All comments tagged `#W65-AO (G8)` / `#W65-AO (MED)`.
`git diff | /usr/bin/grep -c U+FFFD` = **0**.

## G8 — the rule, written once

The wave-64 corpus records were read first. All three replacements have the SAME shape and the
shipped code was doing exactly what the shipped protocol promised — **last-wins**:

| record | reply led with | engine executed | offsets |
|---|---|---|---|
| 146v130 s93 | `CHOICE: 2 (Cast nothing right now)` | `CHOICE: 1 (Cast Acererak the Archlich)` | latched 6544, `plan_block_end` 735, `latched_line_in_plan` false |
| 146v130 s101 | `CHOICE: 1 (becomes beholder …)` | `CHOICE: 3 (Hold priority …)` | 2653 vs 2467 |
| 130v146 s47 | `CHOICE: 1 (Deal 2 damage …)` | `CHOICE: 3 (Draw 1 …)` | 2467 vs 2042 |
| 130v162 s57 | `CHOICE: 0 (pass)` | row **4** (`hold_row_named;echo_index_conflict`) | 1981 vs 857 |

`#W63-AD`'s plan-block exclusion cannot reach any of them: the block ENDS at the first paragraph
(735 / 2467 / 2042 / 857) while the deliberation runs on for kilobytes, so every one of those lines
is "outside the plan" by the letter and pure chain-of-thought in fact.

**Shipped rule** (`findAnswerLabelLine`, the seam that actually selects the answer, so every
consumer of it obeys — priority, ask, menu, combat, salvage):

1. The **FIRST** coded answer line is the answer. The scan now collects every head and selects
   among them; the first CLEAN head wins, and with no clean head the first head stands
   (`#W50-Y` D7's clean/unclean grammar and the W36 adjacent-run head rule are unchanged — they
   now choose among heads instead of tracking "the last one seen").
2. A LATER head supersedes only when it **EXPLICITLY CORRECTS**: it is a coded answer line by the
   `#W64-AK` R5 skip set (an INDENTED top-level correction still counts) AND carries a retraction
   marker from a closed set (`gptAnswerCorrectionCue`: correction / corrected / revised answer /
   revising my / final answer / changing my answer / scratch that / disregard / ignore my previous /
   supersede / instead of choice / not choice …) on its own line or on the nearest preceding
   non-blank line. Deliberation vocabulary ("wait", "actually", "on reflection", "let me re-read")
   is deliberately NOT a marker — that is the exact prose all four corpus rows were lost to.
   A marker written ON the correcting line is allowed to be "unclean" by D7's tail test, because
   the marker IS the tail.
3. The rule is **label-wide**: CHOICE / ATTACK / BLOCKS / PUT all obey it.
4. The **PLAN comes from the answer's own block**. `findPlanMarker` is now explicit about it (last
   marker before the answer line, else the FIRST marker after it). NOTE — this is a *documentation*
   change, not a behaviour change: the shipped selector already computed that. What actually fixes
   s47 is (1): with the answer back on block 1, the plan that was already being harvested IS block
   1's. Pinned by `findPlanMarker(s47, answerLine) == p47`.
5. Nothing is silent. A refused later line sets the new `kPlanAnswerLaterIgnored` note →
   `later_answer_ignored` on the record, beside the existing `answer_replaced` / both offsets;
   `plan_answer_line_ignored` / `plan_answer_line_only` are unchanged.
6. **Number/name conflict is a RE-ASK.** `parseChoice`'s reserved-name branches (`hold_row_named`,
   `pass_row_named`, `pass_hold_ambiguous` — D9 / D2a / D11a) now also stamp `index_name_conflict`
   when the coded number and the named row disagree, and BOTH seams (priority ~29500, ask ~30140)
   route that to the ONE re-ask `#W52-J` D6 already built, quoting both halves, before anything
   executes. The named row stays the answer of last resort on the exhausted second pass, so D9/D2a
   are not reverted — only the *silent* pick is.
7. **The protocol now states the rule the seams run** (`kReplyProtocol`): "Your FIRST answer line is
   the one that runs … write the word CORRECTION on that line or on the line just above it: a later
   answer line WITHOUT that word is read as thinking-out-loud and is ignored", and the closing
   parenthetical flipped from LAST to FIRST. Under the trust doctrine the surface must be true, and
   the old text was the half that was true.

## MED shipped

- **PLAN carry cap stated** (deck126 s41's 5,522 dropped bytes; deck146 s24's inverted sentence;
  routed here by wave64/synthesis-notes.md). `kPlanCarryMaxChars` (400) has bounded the carried plan
  since `#W60-M` and the protocol never said so. It now does, with the shape of the cut. The
  "sentence boundary" half was ALREADY true — `planCarryBound` already cuts at the last `.!?` past
  the halfway mark — so nothing was changed there; the case pins that the claim matches the
  function, and pins the protocol's number to the constant.
- **`{feeds:}` states the SYMMETRIC half** (deck162, 41/41). Dictate of Kruphix is two script lines
  (`mtg.txt:29401-29402`): `@each my draw:draw:1 controller` and `@each opponent draw:draw:1
  opponent`. The scan read only the second, so a card that feeds BOTH players rendered as "the
  opponent draws 1 extra card per turn" — true, and false in scope. `extraDrawPerTurnUncached` is
  now parameterised by recipient, `ownExtraDrawPerTurn` reads the controller half, and the tag adds
  "; and so do YOU: N extra card(s) per turn (this engine is SYMMETRIC …)". A one-sided feeder's row
  is byte-identical to the shipped tag (pinned).

## RED evidence

The same tree was rebuilt with the shipped semantics restored (last-clean-wins selection, no
`index_name_conflict` stamp, the wave-64 protocol strings, `selfPerTurn` forced to 0) and the new
cases run against it:

- **base semantics: `4543 passed, 20 failed`** — including all three corpus replies verbatim
  (s93 ×3, s47 ×2, s57 ×2), the unmarked-correction family (×5), both protocol cases, and the
  symmetric-feeds cases.
- **this tree: `4563 passed, 0 failed`.**

No suite fixture is shipped: nothing here is an engine-behaviour change (the GPT seams have no
endpoint under the suite), so the seam is verified by the corpus lines themselves as PARSETEST
cases, which is what the brief asks for.

## Gate

Detached unit `w65-AO-gate`, `MemoryMax=4G MemorySwapMax=0`, binary rebuilt from clean
(`rm -f bin/wagic && make -f Makefile.sdl -j4`), suite at `WAGIC_TESTSUITE_THREADS=1`.

| leg | result |
|---|---|
| build | 0 errors |
| PARSETEST | **4563 passed, 0 failed** (base 4536; +27) |
| suite | **1269 tests (0 failed), 67 AI tests (0 failed)** |
| `==Test Failed !==` | 0 |
| `==Test timed out` | 0 |

Nothing was killed under the memory cap. Ten shipped cases were RE-PINNED, each with a `#W65-AO`
comment saying which expectation reversed and why, and each paired with a new POSITIVE showing the
MARKED form still wins (so `#W64-AK` R5's finding — indentation is not plan membership — keeps its
outcome available):
`W36-B3` prose-separated correction, `W36-B3` adjacent BLOCKS, `#W50-Y` D7 ×2 (later-line-affirmed),
`#W62-Z` D9 + `#W63-AD` E6c (protocol strings), `#W64-AK` R5 ×2 (r41 and the F13 indented pair),
`#W63-AD` E6b (the wave-47 post-plan recode), and the wave-22 `B3` any-label bug repro — whose bug
was itself a last-wins artefact and no longer reproduces at either scan.

## Predictions for the wave-65 corpus

1. **G8/1** — no record carries `answer_replaced: true` for a later line that has no correction
   marker. Falsifier: an `answer_replaced` record whose `latched_line` offset exceeds the first
   coded line's and whose reply contains none of the marker phrases.
2. **G8/2** — `later_answer_ignored` appears on records that previously carried `answer_replaced`,
   and every one of them executed the reply's first coded line. Falsifier: a `later_answer_ignored`
   record whose `chosen_text` is not the first coded line's row.
3. **G8/3** — no record executes a row whose number and name disagree without an
   `index_name_conflict` / `index_name_conflict_exhausted` note first. Falsifier: a record carrying
   `hold_row_named` or `pass_row_named` and no conflict note.
4. **G8/4** — no window is served a `YOUR PLAN` line harvested from a block whose answer did not
   run. Falsifier: a served plan whose text contradicts the same seat's own executed choice two
   records earlier in a two-block reply.
5. **MED plan cap** — the fraction of windows carrying the truncation notice falls (deck126's 62/271
   is the baseline). Falsifier: the notice rate holds or rises while replies still exceed 400 chars.
6. **MED feeds** — every `{feeds:` tag on a card whose script has a `draw:N controller @each` line
   carries "and so do YOU"; no one-sided feeder does. Falsifier: either half missing.

## What I did NOT verify / did NOT do

- **No live model probe and no corpus run.** Every change here is verified as a STRING and as a
  selector over recorded bytes, never as a decision a model made differently.
- **146v130 s93's tail bytes do not exist.** The record trims the 5,578 bytes written past its PLAN
  line, so the case is recorded-prefix + the record's own `latched_line`. What it pins is the SHAPE
  the record proves (a coded line deep in post-plan deliberation), not the lost bytes.
- **s47's replacement was arguably GOOD PLAY.** Its second block reasons its way to cycling and the
  reviewer flagged it for the PLAN mismatch, not the answer. Under first-wins the seat now takes
  the burn. That is a real, accepted cost of the rule the brief specified: an unmarked late
  improvement no longer runs. The correction route is kept, marked, and stated in the protocol, and
  whether the model USES it is exactly what the wave-65 corpus will show — prediction 1's falsifier
  doubles as the measurement.
- **The correction marker set is a closed list, not a grammar.** A model that writes a genuine
  retraction in words outside the list loses it. I did not enumerate the phrasings the pool actually
  produces; the corpus will.
- **Extending first-wins to BLOCKS/ATTACK/PUT is a rule extension, not a corpus finding.** The
  brief's "make every seam obey it" is the whole basis. `W32-N122d`'s line-precedence case is
  re-pinned; I did not re-derive whether any live blockers reply relied on a second BLOCKS line.
- **The re-ask widening is untested live.** `index_name_conflict` now fires on a class that used to
  execute silently (deck130's `hold_row_named` records), so re-ask VOLUME will rise; I did not
  measure by how much, and the exhausted path still executes the named row.
- **NOT DONE from the MED list:** `draw converters in your hand: N` counting uncastable cards
  (8/8) — the castability verdict is computed in the SITUATION-block emitter from
  `LegalActionsOracle::legalCasts`, and it is not available at `feedsRowTag`'s caller without a
  per-row oracle call; the only cheap alternative was a hedging parenthetical, which the trust
  doctrine forbids. Also not done: the valor-counter affordability clamp, Elite Spellbinder's bare
  target window, Kaya `-3` self-targeting, the `Choose a card NAME` token-first ranking, `{text:}`
  dropped from reveal rows, and the plain `Cast nothing` row's withdrawal under the
  declined-this-list tag. All six remain docketed, untouched by this diff.
- Nothing was checked against the two known concurrency-only failures; the gate ran single-threaded,
  which is ground truth.
