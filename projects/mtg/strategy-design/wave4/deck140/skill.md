# Deck-140 proposals for revising the strategy-writing skill (wave 4)

Evidence base: 12 deck140 games across `matchups-20260712-192832` + `matchups-20260712-230636`
(1W/5L/6T), findings in `wave4/deck140/findings.md`, revised guide in `wave4/deck140/strategy.txt`.
Format per instruction: QUOTE (wave-3 skill passage) -> REPLACEMENT/ADDITION -> WHY (one line,
with evidence). Ordered by how much each would have changed what I wrote.

---

## P1. NEW first-class pattern: teach a UI surface AS THE MODEL SEES IT

**Quote (Step 4, item 5):** "Include a HOW-IT-OPERATES line for every X-spell and
priority-seam ability the pilot provably fumbles."

**Replacement:** Include a HOW-IT-OPERATES line for every X-spell and priority-seam ability
the pilot provably fumbles. **Where the fumble is on a MENU-SHAPED decision (X announcement,
modal choice, color pick, damage order), a how-it-operates line about the CARD is not enough —
teach the MENU. The pattern, all four parts:
(1) transcribe the literal menu into the guide as the model will see it (`1. X = 0` /
`2. X = 1` / ...);
(2) state the index/value mapping flatly ("the reply is the OPTION number, NOT the X value;
option 1 is always X = 0");
(3) give a one-step pick procedure over the LIST, not over the value ("reply with the number
of the LAST line");
(4) add a verify step phrased over the visible text ("the line you picked must literally read
'X = <the biggest number shown>'"), plus one worked example of the exact observed wrong reply.**

**Why:** deck140's pilot reasoned CORRECT X values in its own PLAN prose ("X=3 ... destroying
them immediately") and then replied the value as if it were the index — realized X was below
its own stated intent in 15/17 X decisions, 4 of them collapsing to X=0. The wave-3 guide's
"pick the LARGEST number offered" was value-space advice for an index-space channel; only
menu-space teaching can fix it.

## P2. Step 2 diagnostic addition: the PLAN-vs-consumed-choice comparison (intent betrayal)

**Quote (Step 2, "Confirm before you accuse"):** "compare the `reply`'s stated pick against
the `choice` the engine consumed (deck44 saw a reply naming option 4 while choice 3 was
consumed — a parse bug, not a decision; also `choice: -1` with an empty reply = timeout)."

**Addition (extend the same bullet):** "...= timeout). **For every NUMERIC menu (X values,
quantities, damage split), also compare the number the PLAN says it wants against the
`chosen_text` the engine consumed, across ALL such decisions in the corpus. A SYSTEMATIC
offset (chosen = intended - 1; or collapse to option 1) is an interface trap, not a Magic
misjudgment — the fix is menu-space teaching (see the UI-surface pattern) or an upstream menu
change, never more strategy prose. One mismatch is noise; the same offset in 15/17 decisions
is the deciding lever.**"

**Why:** this single comparison converted deck140's "the deck can't close" mystery into a
mechanical, fixable defect in one table; nothing in the wave-3 skill directs a reviewer to run
it, and eyeballing chosen_text alone (X=1, X=2 look plausible) hides it completely.

## P3. Step 2/Step 0: a causal-triage test for guide vs platform vs model vs deck

**Quote (Step 0):** "attribute each misplay to a specific line: missing, wrong, or too weak
to override a general-prompt prior."

**Addition (new short block at the end of Step 0):** "**Triage every recurring misplay into
one of four causes before writing a single guide line, using the PLAN text as the
discriminator: (a) the PLAN states the correct intent and the consumed choice betrays it ->
INTERFACE/PLATFORM trap (fix upstream; the guide carries only a defensive procedure while the
defect is live); (b) the PLAN itself reasons wrongly and echoes guide phrasing -> GUIDE defect
(rewrite that line); (c) the PLAN reasons wrongly with no guide echo -> MODEL error (add the
corrective fact or procedure); (d) intent AND choice are both right and the deck still loses
-> DECK weakness (set expectations in the handoff; do not fix it with guide length).**
Guide-text fixes only help (b) and (c); mis-triaged (a) and (d) produce prose that dilutes
the guide without moving the record."

**Why:** deck140's 1/12 decomposed cleanly into (a) the X menu, (d) a reach-starved 14-wipe
deck, and infra timeouts — under wave-3's framing I would have owed the whole record to the
guide and over-written it; the four-way test told me which two lines to write and what NOT to
attempt.

## P4. Timeout passage: split stalls from latency starvation, and normalize by decision density

**Quote (Step 2):** "Treat TIMEOUT as its own outcome class. A 40-minute cap with life totals
frozen for 10-20 turns is mutual durdle — the passivity failure in its purest form ... A guide
that shortens the game (kill by turn 5-6) is also the timeout fix."

**Replacement:** "Treat TIMEOUT as its own outcome class — but TRIAGE it before blaming the
guide, using `latency_ms` and the record count: (i) life totals frozen for 10-20 turns with
normal latencies = genuine durdle-stall, the passivity failure in pure form — a guide that
shortens the game is the fix; (ii) a LOW-turn timeout (game ended mid-progress) with high
per-decision latency (avg 15s+, spikes to the HTTP timeout) = the run's concurrency artifact,
NOT a play defect — report it as infra, write nothing in the guide for it. Also normalize the
deck's timeout count by DECISION DENSITY before comparing across decks: a control deck
generates 3-4x the decisions per game of an aggro deck (98 records at t36 vs 24-36 records in
games ending t9-22), so identical per-decision latency gives control archetypes
disproportionate wall-clock exposure. A raw timeout column penalizes the archetype, not the
piloting."

**Why:** 2 of deck140's 6 "timeouts" (t8 and t14, avg 14.9s/decision, 52s spike) were
latency-starved games progressing normally; wave-3's framing would have had me "fix" them
with more aggression prose that no guide line can execute.

## P5. Step 0 demote-and-compress: keep the forbidden-phrase sentence VERBATIM

**Quote (Step 0):** "A rule that now gets followed has done its job — DEMOTE and compress it
(keep it, shorter; deleting it risks regression)..."

**Addition (same bullet):** "...risks regression). **When compressing a quote-and-forbid rule,
the part to keep verbatim is the NAMED FORBIDDEN SENTENCE itself ('NEVER pick Cast nothing
with the excuse I have no mana') — that exact token pattern is what intercepts the model's
own phrasing; the explanation and war-story around it are what you cut.**"

**Why:** deck140's pool-panic block compressed from ~12 lines to 3 (declines fell 55% -> 11%
under the full block) by keeping the forbidden sentence and cutting the narrative; the skill
says "compress" without saying which tokens carry the effect, and cutting the wrong half
silently deletes the fix.

## P6. Step 5 quote-and-forbid: add the negative-framing-bleed caveat

**Quote (Step 5):** "quote-and-forbid the recurring wrong thought by name ... a named
forbidden sentence intercepts where a paraphrase misses."

**Addition (same bullet):** "...where a paraphrase misses. **But negative framing BLEEDS: a
clause phrased as 'do NOT ...' can suppress the positive instruction adjacent to it (proven
in a controlled A/B this wave). Rules: lead every load-bearing item with the positive
procedure and attach the forbidden phrasing as a subordinate clause ('run THE #1 RULE: pick
the LAST line ... (replying "1" blanked four kill spells)'), never as a standalone DON'T
beside the rule it protects; and never let the DO-NOT section RESTATE a rule the guide
already carries positively — reference the rule and add only the evidence.**"

**Why:** my first draft's DO-NOT section restated the X-menu rule as two standalone negatives
directly adjacent to content the #1 rule carried positively — exactly the proven bleed shape;
the skill currently endorses quote-and-forbid with no bleed warning, so every author will
reproduce this.

## P7. Step 5 arithmetic: a win condition fires only as a per-turn CHECK over visible counts

**Quote (Step 5):** "Give arithmetic it can run. 'Damage = unblocked attacker power + (1 per
attacker if Hellrider out) + burn to face; if >= opponent life, attack with everyone and fire
all burn.'"

**Addition (same bullet):** "**For a deck whose win condition is a sized/announced spell
(X-burn, drain, alt-cost dump), the arithmetic must be written as a RECURRING CHECK bound to
a trigger the pilot meets every turn ('EVERY TURN it is in hand: X = your untapped lands
minus 2; if X >= the opponent's life, cast it NOW — you win this turn'), not as a lethal
formula the pilot must remember to invoke. Posture prose about the same card ('this IS your
kill') does not produce sized casts — in 5 losses the opponent finished at 14-20 life with
the kill spell repeatedly in hand.**"

**Why:** wave-3's guide already named Rakdos's Return as "your kill" and the pilot still
never closed; the delta between a named win condition and a fired one was exactly the
every-turn-check framing, computed from counts on the visible board.

## P8. Step 0 "failure moves downstream": re-audit the fixed chain END-TO-END

**Quote (Step 0):** "Expect the failure to have MOVED downstream. Fixing the blocking action
exposes the next bottleneck in the same chain (fetches now get cracked -> but every fetch
takes the wrong land -> the deck still casts nothing)."

**Addition (same bullet):** "...casts nothing). **Concretely: after a fix lands, follow the
SAME card/action through the logs all the way to its effect RESOLVING on the board — cast
happened is not effect happened. Verify each stage: offered -> taken -> parameters set
correctly (X, mode, targets) -> resolved with the intended magnitude.**"

**Why:** wave-3's {l}->{X} render fix made Rakdos's Return get CAST (0/26 -> ~17 takes) and
the celebration would have been premature — the very next stage (X announcement) blanked it;
one more column in the offered-vs-taken table (realized magnitude) would have caught it in
the same pass.

---

## What the wave-3 skill got RIGHT for deck140 (keep, no edits)

- **Step 0 revision mode as a whole**: reading the deployed guide as evidence and auditing
  rule-by-rule (followed / never-fired / stale) drove every demotion decision I made.
- **The offered-vs-taken tally**: still the fastest first diagnostic; it showed casts were
  now happening, which pointed the investigation downstream (P8).
- **The selection-TABLE pattern**: deck140's sweeper table was followed correctly in
  essentially every observed sweep across 12 games — field-proven twice now; keep the
  "table pattern is field-proven" note and cite wave 4.
- **Mine the reply/PLAN text**: without it the X trap is invisible (P2 is an extension,
  not a correction).
- **Closed-exception "Cast nothing"**: the tightened exception held; declines are now
  mostly legitimate sweeper-holds.
