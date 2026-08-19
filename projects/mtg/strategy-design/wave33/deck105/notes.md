# deck105 — development notes (engine / render / harness), wave 33

Corpus `matchups-20260819-070739`, binary `bdb30d8e7`. All repros are seq+turn cited from the
seat translogs; source line numbers are from the tree at that binary.

Seat health: 123 decisions at deck105's seat / 240 across both seats of the 6 games,
**0 fallbacks of any class, 0 crash / FATAL / assert / sanitizer signatures in all 6 stderr
files.** One `all_assignments_illegal` exists corpus-wide (`152 v 36`) — not at this seat.

---

## CLOSED THIS WAVE (do not re-open)

- **N-105a — CLOSED, VALIDATED.** Poison status line, distinct infect narration for both the
  player half and the creature half, and `alterpoison` narration all ship and reach
  **120/120 (100%)** of post-first-poison-event prompts across both seats. All three of the
  note's own validation predicates pass (0 hand-count re-derivations; no land drop in the top-5
  tax rows; 5 replies quote the rendered number). Details and fractions: findings.md §1.
- **N-105b — CLOSED, VALIDATED, both directions.** Infect blocker windows: **10/10** carry the
  poison-partitioned forecast, **0/10** carry a life forecast, and the misteach clause
  *"taking damage while ahead is often correct"* has zero occurrences on any infect window. The
  non-infect negative control is unchanged (**17/17** at deck105's own seat). The wave-32
  game-losing shape is reversed: infect windows ending in a declared block went **8/12 -> 10/10**.
- **N-105c / G1 — DISCHARGED.** See general-suggestions.md. The win/threshold conflation does NOT
  persist; recommendation is to DROP the held core clause, not adopt it.
- **N-105d** — the `(creatures: N)` battlefield-header misread did not recur in 123 decisions.
  Steady state.

## CARRIED, UNEXERCISED

- **N-105e** — mulligan-bottoming card text truncating mid-word. 0 mulligans and 0 bottoming asks
  in 6 games, so unexercised. Carried, not observed.
- **Relic Putrescence probe (wave-32 spec) — STILL OWED.** Cast once and correctly attached to an
  opponent's `Mox Jet` (`105v36 s16/s17`, 11 targets offered, right one picked), then sat attached
  for three turns with deck36's poison total frozen at `1 of 10`. Whether the enchanted artifact
  ever became tapped is not rendered, so `@tapped(mytgt):damage:1 targetcontroller` on an
  `abilities=infect` Aura is **neither validated nor falsified**. The probe (does it yield a POISON
  counter or 1 life loss?) still stands. **Hand of the Praetors' half of the same probe spec is
  DISCHARGED** — exercised naturally and correct end-to-end in `139 v 105` (lord bonus visible in
  the rendered P/T, cast-trigger fires per infect creature spell, 3/3 correct target picks).

---

## N-105f — BATCHED COUNTER LINES ALL PRINT THE POST-BATCH P/T. (RENDER, MEDIUM)

**Repro quality: PERFECT (every multi-counter infect combat in the corpus).**

`AIPlayerGPT.cpp:3051-3059` renders `WEventCounters` and prints the target's CURRENT power and
toughness (`out << " (now " << e->targetCard->power << "/" << e->targetCard->toughness << ")"`).
The in-source comment justifies this correctly for a SINGLE counter ("the engine sends this event
AFTER applying the counter ... so the permanent's CURRENT P/T is the settled result"). It does not
hold when N counters are applied in one damage event: the N events are narrated after the whole
batch has been applied, so **all N lines print the same final P/T**.

Verbatim repros:
- `139 v 105`, Cystbearer (3/4) infect damage to a 0/3 Arboreal Grazer:
  ```
  - Counter added to Arboreal Grazer #1: -1/-1 (now -3/0) [from Cystbearer]
  - Counter added to Arboreal Grazer #1: -1/-1 (now -3/0) [from Cystbearer]
  - Counter added to Arboreal Grazer #1: -1/-1 (now -3/0) [from Cystbearer]
  ```
  Correct sequence is `(now -1/2)`, `(now -2/1)`, `(now -3/0)`.
- `105 v 116`, Ichorclaw Myr (3/3) into Kitchen Finks (3/2): `-1/-1 (now 0/-1)` x3.
- `139 v 105`, Hand of the Praetors (3/2) into Pollywog Symbiote: `-1/-1 (now -2/0)` x3.

Cost: the pilot reads the run as three separate applications of the same end state, which is one
of the two ingredients of the `139v105 s18` 11,089-char / 115 s board-state spiral (the other is
N-105g). It also makes the append-only log un-replayable: a reader cannot reconstruct intermediate
toughness, which matters for first-strike / multi-blocker orderings.

**Proposed fix (smallest sufficient):** capture `power`/`toughness` INTO the event at fire time
(`Counters::addCounter`) and print the captured pair; or, if the batch size is known at flush,
print `final + (remaining * delta)`. Note the companion line
`- Infect damage: 3 from Cystbearer to Arboreal Grazer - dealt as 3 -1/-1 counters ...` already
states the aggregate correctly, so the per-counter run is redundant AND wrong — the cheapest fix
may be to SUPPRESS the per-counter lines whenever the aggregate `Infect damage:` line covers the
same event.

**Validation predicate:** after the fix, in any window where a creature receives N>1 counters from
one source, the N `(now X/Y)` values must be strictly monotone; grep for two consecutive identical
`Counter added to <same name>: <same delta> (now <same X/Y>)` lines and expect zero.

---

## N-105g — INSTANCE SUFFIXES ARE POSITIONAL AND RE-INDEX ON DEATH, SO THE APPEND-ONLY LOG
CONTRADICTS THE BOARD SNAPSHOT. (RENDER, MEDIUM-HIGH — a new same-zone face of ledger item L2)

**Repro quality: PERFECT, with a verbatim self-diagnosis by the pilot.**

`instanceHandle()` (`AIPlayerGPT.cpp:810-834`) computes the `#N` suffix by walking the
CONTROLLER'S BATTLEFIELD at render time and taking the card's positional rank among same-named
cards, returning `""` when `total < 2` or when the card is not on the battlefield. Three
consequences, all observed:

1. **A dying permanent loses its suffix entirely.** The zone-move narration renders
   `- Opponent's Arboreal Grazer: battlefield -> graveyard` (no `#N`) while the counter lines two
   rows above render `- Counter added to Arboreal Grazer #1: ...`. The log therefore never says
   WHICH copy died.
2. **Survivors are RENUMBERED.** After one of two Arboreal Grazers dies the other becomes `#1`;
   after one of three Pollywog Symbiotes dies the remaining two shift down. Earlier log lines are
   never rewritten, so the SAME permanent is called `#2` in a turn-7 line and `#1` in the turn-9
   board snapshot.
3. **A permanent gains and loses its suffix as copies come and go** (`total < 2` -> `""`), so the
   same card appears as `Pollywog Symbiote`, then `Pollywog Symbiote #2`, then
   `Pollywog Symbiote #1` in one append-only transcript.

**Repro — `139 v 105 s18 t9` (kind=ask, 11,089 chars / 115.5 s, the seat's third-largest reply).**
Log says `- Counter added to Pollywog Symbiote #2: -1/-1 (now 0/2) [from Blight Mamba]` and later
`- Opponent's Pollywog Symbiote: battlefield -> graveyard`; the board snapshot in the same prompt
reads `Pollywog Symbiote #1 {1}{u} (0/2) (printed 1/3) [counters: 1x -1/-1]` — the counter is on
the copy the log called `#2`. The pilot's reply, verbatim:

> *"'Opponent's Arboreal Grazer: battlefield -> graveyard'. It died. ... But the 'CURRENT
> SITUATION' block explicitly lists them as 'Opponent battlefield (creatures: 3): ... Arboreal
> Grazer ... Pollywog Symbiote #1 ... Pollywog Symbiote #2'. This contradicts the game log where
> they died. ... Ah, the 'CURRENT SITUATION' block is the source of truth for the current state.
> ... Maybe the log description of 'graveyard' was a simulation error or I misread?"*

It resolved by distrusting the LOG, which is the wrong half to distrust and is exactly the failure
mode wave-32 recorded at deck105 for the poison count (*"then my count is wrong"*) — the same
shape, relocated to permanent identity.

**Proposed fix:** the handle must be a stable IDENTITY, not a live rank. Assign a per-controller,
per-name monotonic ordinal when the instance enters the battlefield, store it on the instance, and
render that everywhere (zone moves included). Never re-derive from current battlefield position,
and never suppress it once assigned — a name that was ever duplicated keeps its handle for the
rest of the game, which is what makes an append-only log replayable. Emitting the handle on the
zone-move line is the single highest-value half of the fix.

**Validation predicate:** in any game where a duplicate name dies, grep the prompts for a
`#N` that appears on a board line and a *different* `#N` for the same physical permanent earlier
in the same log; expect zero. And every `battlefield -> graveyard` line for a name that was ever
duplicated must carry a handle.

---

## Guide-lane items routed OUT of the engine lane (recorded here so they are not re-diagnosed)

- The vs146 loss traces to an AMBIGUOUS GUIDE SENTENCE (`N` bound to damage instead of resulting
  life), not to a render defect — the render line was correct and the pilot quoted it correctly.
  findings.md §L1.
- The vs158 loss traces to a GUIDE GATE that ignores a growing board. The engine renders the
  growth honestly and completely (`Counter added to Orc army: +1/+1 (now 9/9)`,
  `[counters: 9x +1/+1]`, `(printed 0/0)`). No engine item.
- The vs158 keep traces to a GUIDE MULLIGAN RULE with no colour clause. i9's header printed the
  answer verbatim (`Playing every land in this hand would not cover any spell in it.`) and the
  pilot contradicted it. No engine item — this is i9 working.

---

## Cost observations (not defects)

- **The damage-assignment ORDER seam (`d083a0629`) is EXERCISED and correct but expensive.**
  `139v105 s14/s15/s16`: one blocked attacker produced THREE sequential position asks costing
  **140 s and 11,915 chars** in total, for an outcome worth one creature. If the cap/latency lane
  is being weighed this corpus, note that a 4-blocker combat can add three full model round-trips
  to a single combat step. A single "give the whole order in one reply" ask would collapse it,
  mirroring the existing one-reply attacker and blocker declarations.
- **Latency at this seat:** median 15.8 s (wave-32: 17.1 s), p90 75.6 s (61.8 s), max 186.7 s
  (219.2 s). The p90 rise is the tax relocating from poison-counting to the combat seam; see
  findings.md §4.
- **`ABILITYFACTORY ERROR: Parser returned NULL` noise** appears in two of the six stderr files
  (1 line in `game-146v105`, 6 lines in `game-152v105`), all on OPPONENT card scripts
  (`{t}:add{w}`, `{t}:add{g}`, deck152's day/night `if type(*[day;night])...` line). Pre-existing
  and not deck105's; recorded only so the crash-gate count (0) is not confused with these.
