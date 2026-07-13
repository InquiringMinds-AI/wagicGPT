# Deck44 wave-4 notes — everything that doesn't live in the other four files

Companion files in this dir: findings.md (game review), strategy.txt (revised guide),
skill.md (method proposals), general-suggestions.md (core-prompt proposals). This file is
organized by audience.

---

## For the ENGINE owner — three work orders

### E1. Surface the stack in the board snapshot
The `--- CURRENT SITUATION ---` block never mentions the stack; a pending opponent spell
is discoverable only as the trailing `hand -> stack` line of the GAME LOG. Add a line,
present whenever the stack is non-empty at a decision point, e.g.:
`On the stack (you may respond): Opponent's Guttersnipe {1}{r}{r} - "<card text>"`
Order stack objects LIFO if more than one. This single line obsoletes the counter-tell
stopgap now carried in deck44's guide and proposed for the core prompt.

### E2. Label reaction options with their pending target
Where a counter/response option is offered because of a specific pending spell, render it
as `Cast Counterspell {u}{u} - counter Guttersnipe` rather than a bare cast line. Cheaper
attention-wise than E1 alone and disambiguates multi-object stacks. E1+E2 together are the
full fix; either alone is a big improvement.

### E3. Priority-timing defect — response window arrives AFTER resolution (repro)
Repro citation: run `matchups-20260712-230636`, game start_epoch 1783922874 (deck44 seat
translog `1783922880-ai_baka_deck44-0x556c6c896360.jsonl`), opponent deck110, turn 6.
Sequence in the seat log: `Opponent's Master of Etherium: hand -> stack` immediately
followed by `stack -> battlefield` with NO deck44 decision record (no ask/priority of any
kind) in between — deck44 held {u}{u}+ and two Counterspells. Its next windows (s28/s30,
turn 9-10) arrived during LATER pending spells. Same pattern turn 4 (Steel Overseer) and
turn 8 (first Cranial Plating) in the same game. By contrast, other games (e.g. vs131
e2536, vs109 e...0058) DID open windows while spells were live — so the seam works
sometimes; determine what gates it (phase? whose priority round? an interest heuristic
skipping the ask?). This is the same class as wave-3's silent-blockers finding (F2 there,
since fixed): a decision seam that sometimes never reaches the GPT player. E1/E2 only help
at windows that actually open pre-resolution.

Small bonus fixes while in the representation code:
- Empty counter bracket: `[counters:]` renders on Sleep-Cursed Faerie at 0 stun counters
  (e2880 s28 battlefield line). Drop the bracket when empty.
- The evaluated-effect annotation `{right now: life -2}` on "Cast Faerie Bladecrafter"
  reads as a COST of casting; it is the dies-trigger drain on the opponent. Prefix with
  the trigger it came from, e.g. `{if it dies: opponent -2}`.

## For the NEXT reviewER — method notes

### The wave-3 mis-attribution case study (what it teaches)
Wave 3 observed deck44 "dying with counters in hand" and attributed it to guide WORDING
(hoard-y "save/premium" language); wave 3's rewrite duly deployed anti-hoard text. Wave 4
shows counters still 0/12 — because the stack is invisible and the pilot cannot know
anything is counterable. The wording diagnosis wasn't wrong, it was SHALLOW: both causes
were real, but the representation cause caps what any wording can achieve. Lesson (now
proposed as skill R5): before accepting a wording/posture diagnosis, pull 2-3 full prompts
for the misplay and verify the deciding fact was VISIBLE somewhere the executor attends
to. One prompt-read in wave 3 would have caught this a wave earlier. Corollary: when a
wave's fix for a leak produces no movement in the next corpus, suspect the layer below
the one you edited.

### Tell-audit recipe (rerun on any deck / any hypothesized tell)
Before teaching "when you see X, Y is true", enumerate every corpus window where X
appears and verify Y. For the counter tell, ~20 lines of python per run dir:
- For each seat jsonl, for each record whose `prompt` contains the offer (e.g.
  `"Cast Counterspell"` / `"Cast Arcane Denial"` as an option line — match
  `^\s*\d+\. Cast <name>` to exclude mere hand listings), extract the GAME LOG section
  and take the trailing `hand -> stack` lines: the tell holds iff the last one has no
  subsequent `stack ->` line for the same card.
- Report per window: seq, turn, pending-spell-or-NONE. Any NONE is a counterexample;
  downgrade the guide wording from an absolute ("always false") to a look-and-check
  procedure.
Deck44 result: 12/12 games, every counter-offer window had a live pending spell — zero
counterexamples. Also useful generally: `chosen_text` is now populated in the translog,
so offered-vs-taken tallies no longer need choice-index regexing; a per-game one-liner
over `kind` counts finds seam silences (that's how wave 3 caught blockers and how E3
shows up as a missing record).

### Small corpus facts that save the next reviewer time
- deck44 is deck0 (first column) in all 12 of its games across both runs; seat files pair
  to results.tsv rows by epoch = start_epoch+2..6.
- gameend records carry final life totals; negative my_life = died that turn.
- The one `choice: -1` empty-reply fallback in 12 games: run1 e...0062 s17. Rate ~0.3%.

## For the DECK BUILDER

- **Reactive slots in an aggro shell:** deck44 runs 6 reactive cards (2 Counterspell,
  2 Arcane Denial, 2 Go for the Throat) in a deck whose whole plan is racing. The GFTT
  pair earns its slots (fired promptly and well in 5 games). The 4 counters were dead
  weight in every loss — partly the engine defect, but even post-fix, consider: all 5
  losses were racing decks where deck44 needed BODIES; both vs109 losses it attacked
  every turn with only two 1/1s while lands+counters stacked up in hand. 2 counters /
  2 more cheap flyers is the direction the corpus points.
- **Color screw game:** e2536 (run2 vs131) — opener Island x3 + Miscreant + 3 counters,
  never found black; cast four Faerie Miscreants all game. 14 blue vs 10 black sources
  with black gating Bladecrafter/Obyra/GFTT is a real (if secondary) fragility.
- **Go for the Throat vs deck110:** "nonartifact creature" makes it blank against the
  affinity deck's best creatures (Master of Etherium, Ornithopter, Vault Skirge) — in
  that matchup the counters are the only interaction that works, which doubles the cost
  of the E1-E3 defect there.

## For the MODEL EXPERIMENTER — qwen35 pilot profile (wave-4 update)

Reliably FAILS (unaided):
- The offered-implies-legal-now inference: cannot conclude from a counterspell being
  OFFERED that a target exists, even with the general prompt stating offered=payable and
  the window-will-arrive line. It requires the fact to be VISIBLE, not derivable. This is
  the cleanest probe question this corpus produced for candidate-model comparison: show a
  candidate the e2540 s8 prompt verbatim and see whether it counters Guttersnipe.
- Repeatable-activation window discipline: obeys "pay in main 1 when attacking" and
  STILL double-pays in upkeep the same turn (needs per-window instruction; skill R4).

Now reliably EXECUTES (don't spend experiment budget here):
- Act-floor: casts creatures every turn, deploys 5-drops (Archmage 10 casts in one game
  via copy engine), no timeout durdle (0/12 vs 13/21 in wave 3).
- Combat: attacks every turn, explicit lethal arithmetic in wins, sensible blocks at the
  now-working blockers seam (chump-blocks to survive, wither-blocks with Gatewarden).
- Removal targeting: GFTT consistently at the correct threat, promptly.
- Protocol: PLAN discipline held; one empty reply in ~330 decisions.

Tendency to note: its PLAN text is carried into the next prompt and it re-affirms its own
prior reasoning — a false belief ("no spell on the stack") self-reinforces across
consecutive windows in the same turn (e2880 s28→s30 nearly verbatim). Fixes that change
the PROMPT (E1/E2) break this loop; fixes that argue with it in guide text fight the
model's own words at equal footing.

## Process notes for the next wave

- Wave-3 platform fixes all verified live in this corpus: phase auto-skips (priority
  records way down), blockers seam firing, chosen_text field, X-announcement menu
  (unexercised by this deck — no X spells).
- Validation for the wave-4 deck44 guide: rerun self-play, then measure exactly two
  things first — Counterspell/Arcane Denial casts per game at offered windows (was 0-for-
  everything except 3 Arcane Denials), and Sleep-Cursed double-untap occurrences (was 3
  games). If E1/E2 land before the rerun, expect the counter-tell section to be deletable
  — check the deletion trigger before re-reviewing wording.
- The 5 losses cluster on aggro; if matchup scheduling allows, over-sample deck44-vs-109
  and vs-110 in the validation run — that's where the guide changes must show up.
