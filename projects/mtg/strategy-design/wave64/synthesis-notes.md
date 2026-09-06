# Wave-64 synthesis notes (step 3)

Inputs read in full: `deck126/general-proposals.md` (the only proposals file this wave — no deck
filed `skill-proposals.md`), all seven `deck{123,125,126,130,146,152,162}/review.md`, and
`engine-seat.md`.

**Result: 0 of 1 proposals adopted.** No `wave64/general-strategy.md` was written (the wave-62
R331 edition stands unchanged), and no `wave64/strategy-writing-skill.md` was written (the
wave-59 edition, amendments 1-331, stands unchanged). Amendment numbering therefore still
continues from R331 for the next wave that adopts.

## Runtime-reachability check (re-verified this wave, not inherited)

The wave-63 synthesis recorded that `general-strategy.md` is not loaded at runtime. Verified
independently before ruling:

- `/usr/bin/grep -rn "general-strategy\|general_strategy\|generalstrategy"` over
  `projects/mtg/src`, `projects/mtg/include`, and `projects/mtg/bin/Res/ai` returns exactly one
  hit, and it is a comment: `src/AIPlayerGPT.cpp:29426`
  (`//narrowing stands (general-strategy.md, the R118 reconciliation).`). No load, no path
  construction, no file open.
- The only strategy file the engine opens is the per-deck one:
  `src/AIPlayerGPT.cpp:15332-15336` rewrites `ai/baka/deck<N>.txt` to
  `ai/baka/deck<N>_strategy.txt`.

So an adoption written into `general-strategy.md` cannot reach a model. The two surfaces that
can are (a) the per-deck guides `bin/Res/ai/baka/deck<N>_strategy.txt`, and (b) engine-rendered
prompt text in `src/AIPlayerGPT.cpp`. Neither is editable from this seat, and general adoptions
are therefore routed as recommendations rather than written into a dead file.

## Verdicts

### P1 (deck126) — "State the PLAN carry budget as a NUMBER in the reply protocol" — **REJECT as a general adoption; ROUTE to the engine lane**

One line of reasons: the text it changes is not general-guide prose at all — the proposal names
its own target, the code-appended reply protocol at `src/AIPlayerGPT.cpp:233` (verified: the
`LINE 2 is a PLAN: line ... CONCISE, a few sentences of intent, not an analysis.` string is
there, and `kPlanCarryMaxChars = 400` is at `src/AIPlayerGPT.cpp:395`, with neither the protocol
nor any ask footer stating the number) — so there is nothing for a general-guide edition to
carry, and writing it into `general-strategy.md` would put a live-surface change into a file no
model reads.

The evidence is not in dispute and the proposal is not noise. It is routed, not discarded:

**Recommendation to the engine seat / a wave-65 engine lane** (owner's call, not adopted here):
append a clause to the `LINE 2 is a PLAN:` block naming the 400-character carry bound and
pricing the overspend, roughly as deck126 drafted it. Supporting evidence, from three
independent seats:

- deck126 review MED-2 + the proposal: 191 PLAN lines over 271 windows, median 312 chars, max
  5,618; **62 of 271 prompts (23%)** carried the
  `[...the rest of your plan was not carried: N further characters, of M you wrote]` notice, the
  largest discarding 3,315 of 3,542 chars
  (`1788671775-ai_baka_deck126-0x55c433487cc0-vs-ai_baka_deck125.jsonl` seq 149). The seat's four
  slowest decisions are all long replies (505 s / 5,794 chars, `...-vs-...deck123` seq 21)
  against a 13.3 s median.
- Corroborated corpus-wide by `engine-seat.md` line 26: `post_plan_overrun>0` **63**.

Two qualifications the routing should carry forward, because they change what the fix has to be:

1. **deck126's own framing ("inference-economy, not play-quality") is contradicted by two other
   seats.** deck123 MED-1: `...vs-ai_baka_deck146.jsonl` seq 24->25, 676 chars written, 274
   carried, and the cut landed immediately before the clause in which the model corrected itself
   — what seqs 25/26 then read back was a flat false statement of board fact. deck130
   (`...vs-deck125`/`...vs-deck162` block-1/block-2 shapes, and the seq 47->48 case): 1,585 of
   1,964 chars discarded, leaving block 1's raw reasoning arguing for the row the engine did
   *not* take, persisting into seq 49. Truncation can invert a self-correcting plan, so this is a
   play-quality item as well as a token-cost one.
2. Consequently the number-in-the-protocol clause should be considered **alongside**, not
   instead of, deck123's suggested engine repairs (cut at a sentence boundary, or provide a
   one-window scratch channel so working prose stops being routed into PLAN). Stating the budget
   asks the model to stop overspending; it does not make the existing cuts safe.

Risk noted and left with the engine seat: a hard number can read as a hard rule. deck126's
wording prices the overspend rather than forbidding length and leaves the three-case
(a)/(b)/(c) mechanical test for *whether* to write a plan untouched; its falsifiable prediction
(overrun notices materially below 62/271, max PLAN under ~1,500 chars, `plan_choice_conflict_*`
and `decision_reversed_in_prose` unchanged, PLAN-line presence unchanged) is the right gate if
it is ever applied.

## Deck-guide contradictions

None. Three decks filed revised guides this wave (`deck123/strategy.txt`,
`deck125/strategy.txt`, `deck130/strategy.txt`); none of them teaches a PLAN length budget or any
rule the routed recommendation would collide with (checked by grep for length/character/concision
language across all three). No deck guides were edited by this seat.

## What I did NOT check

- I did not open the JSONL translogs. Every count in this file is quoted from the deck reviews
  and `engine-seat.md` as those seats measured it; I re-derived none of it, and the two proposal
  counts I would most want independently confirmed are the 62/271 overrun rate and the 5,618-char
  maximum.
- I did not verify any card fact against the primitives or Scryfall — no verdict here turned on
  one.
- I did not read the lane files (`lane-AG..AK.md`), `codex-review.md`, or `known-bugs.md`; the
  single proposal did not depend on a lane prediction, and `engine-seat.md` carried the
  adjudications I needed.
- I did not build, run wagic, or run git; I did not edit `bin/Res`, `src/`, or any deck guide.
