# Wave-81 synthesis

Seven seats reported: `deck{123,125,130,146,152,162,50}/review.md`. Guide verdicts — **KEEP**:
deck123, deck162 (both success verdicts, reasons in their §4). **EDIT**: deck125 (19,995 B),
deck130 (19,988 B), deck146 (19,999 B), deck152 (19,951 B). **AUTHORED (first guide)**: deck50
(14,671 B). Proposals filed: `deck125/general-proposals.md`, `deck146/general-proposals.md`. No
`skill-proposals.md` from any seat.

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260917-191141-final`, binary
`archives/wagic-6e0c51238-w81step1`. File names below are the corpus-dir basenames; `seq` is the
record `seq` in that file.

---

## 1. Proposal decisions

| proposal | seat | decision | reason |
|---|---|---|---|
| **P-G1** — your own library is a resource that can lose you the game, and nothing on the screen says so | deck125 | **ADOPT** — as **R332** (RENDER, HIGH) in `wave81/general-strategy.md` | A won game was lost to it: deck125 at 59 life against an opponent on 7 drew from its own empty library (`1789690337-ai_baka_deck125-…-vs-ai_baka_deck50.jsonl` seq305). The asymmetry is measured, not argued — `DECK-OUT IS IN RANGE` renders 15 times on the opponent line and 0 of 2,122 on the seat's own. Adopted as the seat itself ranked it: the render fix carries the paragraph, with H2's cast-price clause folded in and the text's retirement condition recorded. Deck50's arrival makes the shape reachable against every seat, which is what makes it general. |
| **P-146-1** — a counter test that is not scoped to one card | deck146 | **ADOPT** — as **R333** (CORE-PROMPT) in `wave81/general-strategy.md` | Generalises a rule the corpus shows WORKING (seq23 deferred Lolth by name) rather than inventing one; keys to two facts the render already prints (`counter_intel_rendered` 1319 in that game) and fires only on the row that spends the whole turn on one object — the shape all 12 countered spells had. Bounded against the opposite failure by the same game's own evidence (both Lolths discarded at cleanup, seq48/seq60). Filed CORE-PROMPT because entry-2 scoping is an accident of where the rule was written, not a fact about Lolth. |
| **strategy-writing skill amendment** | — (none filed) | **REJECT — no `wave81/strategy-writing-skill-v2.md` written** | Two seats hit the same wall and neither is a skill defect. deck123 §4 (the DAMNATION maker-veto with no life boundary) and deck162 §4 (BLOCKING clause (b), an attacker's lifelink added to the block line's cost) both describe a rule correct in general and wrong in its hard case, and both name A334 plus the 20,000-byte ceiling as what blocks the repair. A334 forbids the *concessive* form, not the repair: deck146's Edit 1 made exactly this repair within the current edition — a flat, screen-observable, scope-limited exception (`never Kaya or Lolth if you control neither`). What remains is a byte-ceiling problem, not an edition problem, and deck162's own ordering is right (land the render item first, re-cut the rule against the new surface). No seat's evidence shows the wave-68 edition producing a bad rule. |

**Contradiction check (adopted general change vs a seat's staged guide edit): none found.** The one
adjacency worth naming: deck152's **E2** ("ANY row beginning `Play ` -> TAKE IT … no second land
question follows it") is the guide half of merged item **H1** below and does not conflict with R332
or R333 — but it asserts more than the corpus supports. Both land regimes are live in the same game
(item **H2**), so a *later* standalone `Land drop:` ask can follow; E2 survives only because it keys
to "ANY row beginning `Play `", which covers both shapes. deck50's guide keys the same rule the same
way, deliberately (its LOW item measures 32 standalone against 35 folded windows). Neither needs
changing; it is recorded so step one does not read E2 as a claim about the render.

---

## 2. Merged engine / interface / card list — HIGH first, de-duplicated across seats

This list is the next step one's bug-list input. "Seats" names every seat that independently
elevated the item.

### HIGH

**H1. The `Land drop:` header paragraph asserts a separate land question that P9 removed, on the
same menu that carries the folded `Play <land>` row.** *Seats: deck123 (HIGH-2), deck130 (MED-1),
deck146 (HIGH-1), deck152 (HIGH).* Every such menu carries `Land drop: a land play IS available to
you right now. The land drop is its OWN decision (a "Land drop:" question with its own Play
options), so the absence of a land from the choices below does not mean the drop is gone.` — while
a `Play <land>` row sits in the choices below it. deck130 measures it at **44 of 44** windows,
deck146 at 17, deck123 at 27. Ranked HIGH on one seat's traced cost, not on volume: **repro
`1789690312-ai_baka_deck152-0x565158e5a870-vs-ai_baka_deck130.jsonl` seq26** — reply
`PLAN: Play Plains. … / CHOICE: 5 (Cast nothing right now)`; the plan names the land, the action
declines it, and the drop was lost that turn. Corrected text already exists in the same binary (the
folded decline row reads `… - and play no land in this window`).

**H2. Both land regimes run at once — the standalone `Land drop:` ask is still reachable in every
game.** *Seats: deck123 (HIGH-3), deck50 (LOW), deck125, deck152, deck162 (all in their §3).*
Per-seat splits (standalone / folded): deck123 20/27, deck125 36/58, deck152 20/22, deck50 32/35;
deck162 reports the fold is conditional on a casting menu existing. **Repro
`1789690346-ai_baka_deck123-0x561624893220-vs-ai_baka_deck152.jsonl` seq34 and seq37** — a
standalone `ask` whose whole option list is `['Play Marsh Flats', 'Play no land right now']`.
Consequence for the owner's P9 call: until this is one regime the revert criterion is being
evaluated against a half-shipped change.

**H3. The attacker row's `their untapped blockers` verdict ignores damage already marked on the
attacker, and no surface outside the GAME LOG shows that damage.** *Seat: deck130 (HIGH-1).*
**Repro `1789690335-ai_baka_deck130-0x5586b2e3ab70-vs-ai_baka_deck123.jsonl` seq72** — the same
prompt's log carries `Starstorm -> 4 damage to Rorix Bladewing`, the battlefield line prints
`Rorix Bladewing … (6/5) … [untapped]` with no damage tag, and the A-line reads
`[their untapped blockers: … Vampire #1 (4/4) (you kill it, your attacker lives)]` over a prompt
that says *"Do not re-derive these outcomes; use them."* The seat used them, attacked, and lost its
only win condition. Two fixes, both needed: print the marked damage on the battlefield/attacker
line, and subtract it inside the 1-on-1 result.

**H4. The blockers seam prices an attacker's lifelink as a cost of BLOCKING and never as what they
gain unblocked.** *Seat: deck162 (HIGH-1).* **Repro
`1789690306-ai_baka_deck162-0x565093601250-vs-ai_baka_deck152.jsonl` seq11** — `A2. Intrepid
Adversary (6/4) deals 6 [lifelink]` (bare tag, no owner, no figure) against
`B1. Shield Sphere … {their attacker's lifelink, this block: they gain 6 from this block only, and
this attacker deals nothing to your life}`. The gain figure exists only on the block line, so the
menu reads as though declining avoids it. The seat's reasoning inverted both halves ("I gain 6 life.
So net change is 0"), answered `BLOCKS: none`, and took 11 instead of 5 at 15 life. Direction: put
both branches and the owner on the A-line, drop `from this block only` from the B-row brace. Same
emitter family as **R331**; that item's `from this block only` clause is the string named here.

**H5. No deck-out clause on the seat's OWN library line; and no cast/X row prices the mill the cast
itself triggers.** *Seat: deck125 (H1, H2).* **Repro
`1789690337-ai_baka_deck125-0x55f86ec66780-vs-ai_baka_deck50.jsonl` seq305** (opponent line carries
`DECK-OUT IS IN RANGE`, own line is a bare `Your library: 11 cards`) and **seq282** (option 12
promises `13 left`; the library at seq283 is 5, with four Memory Erosions on their battlefield).
**Adopted as R332** in `wave81/general-strategy.md`.

**H6. A reasoning-budget overrun re-asks with the native reasoning channel switched OFF.** *Seat:
deck125 (M2).* **Repro `1789690345-ai_baka_deck125-0x55c9788152e0-vs-ai_baka_deck152.jsonl` seq23**
(`reasoning_chars` 20,683) and `1789690324-ai_baka_deck125-0x564f3c4d6640-vs-ai_baka_deck162.jsonl`
**seq300** (26,808) — both `reasoning_budget_hit: true`, `reasoning_forced_close: true`, `retry: 1`,
`max_tokens_reasoning: 0`. Promoted from the seat's MED to HIGH here because it is the one path in
the harness that violates the standing owner ruling directly: reasoning happens ONLY in the native
channel. Both answers happened to be right. Fix: raise the budget, or keep thinking on across the
retry.

**H7. The single heuristic fallback of the whole corpus is a parser-robustness miss, and it fell
through with no re-ask.** *Seat: deck50 (HIGH).* **Repro
`1789690337-ai_baka_deck50-0x55f86ff817d0-vs-ai_baka_deck125.jsonl` seq41** (window_seq 39, T28,
4 options, `reask_reason` null, one attempt); recovery record **seq42** shows
`answer_label_absent: false`, `executed_by: heuristic`, `executed_text: "cast nothing"`. The reply,
verbatim and complete, is the single line `PLAN: 4 (Cast nothing right now)`. There is no plan for
the answer to precede; `4` and the parenthesised short name match row 4 and no other row; the
label-less-answer path did not engage because a label WAS present, just the wrong one. Under the
standing ruling ("make the parser robust"; only an answer PRECEDING the plan is rejected) this
should have been read. Fix direction from the seat: when no action-label line exists and the sole
`PLAN:` line's content is exactly `<row number> (<that row's short name>)` matching one row, read it
as the answer; failing that re-ask (891 s of deadline remained, `deadline_pct` 8.9) rather than fall
back to Baka. Here the heuristic happened to land on the row the model asked for; on a live menu it
would not.

**H8. The "Hold priority" row's SHORT NAME is read in its Magic sense, and it decided a game.**
*Seat: deck123 (HIGH-1).* **Repro
`1789690335-ai_baka_deck123-0x5586b1b1b620-vs-ai_baka_deck130.jsonl` seq78** — row text
`Hold priority - pass now, and do not ask me again - YOU CANNOT COME BACK AND TAKE ONE OF THE ROWS
ABOVE LATER THIS TURN …`; reply `PLAN: Hold priority to block Rorix with Vampire #1 and Vampire #2
in combat. | CHOICE: 3 (Hold priority)`. The seat took a hold row 25 times across six games and in
substantially all of them the PLAN on the same line states an intention to act LATER THIS TURN —
the exact thing the row forfeits (deck125 seq108/seq121, deck162 seq33/seq62, deck50 seq78/seq19,
deck146 seq19, deck130 seq105). The body text is already correct; the two-word name is what the
attention lands on. Recommendation: rename the row for what it does (a *stop asking me this turn*
row), no extra body words.

**H9. A hold taken at the seat's OWN End step silences every window of the opponent's entire next
turn.** *Seat: deck125 (H3).* The row's scope is "until your next turn begins", and the DK re-open
fires only for a new lethal. **Repro
`1789690324-ai_baka_deck125-0x564f3c4d6640-vs-ai_baka_deck162.jsonl` seq211** (T27 End) → T28
Underworld Dreams and Teferi's Puzzle Box both resolved with no window asked (that game:
`hold_windows_skipped` 277, `hold_events` 46, `hold_verdict_safer_ignored` 30); same at
`1789690345-ai_baka_deck125-…-vs-ai_baka_deck152.jsonl` seq26 (19 skipped windows covering T13's
Ranger Class and Brutal Cathar). Ranked on mechanism — at this seam the option that closes one
window and the option that disarms a whole opponent turn are the same row. Neither instance cost a
provable answer (the held counters were unpayable or creature-only).

**H10. A damage spell's target ask has no declining row, so a wrong cast is irreversible.** *Seat:
deck130 (HIGH-2).* **Repro
`1789690327-ai_baka_deck130-0x55bc1e82dda0-vs-ai_baka_deck162.jsonl` seq11** — `Write your PLAN: …
CHOICE: … (this ask has no pass row)` over `1. The opponent … 2. Master of the Feast … - SURVIVES
(toughness 5) 3. Yourself`. Once the cast row is taken every guide rule that says "decline" is
unreachable and the face is the only sane row left. Same shape at the same seat's
`1789690312-ai_baka_deck130-0x56515a193370-vs-ai_baka_deck152.jsonl` seq24 (which at least offers
`3. Decline - do nothing`, with the mana already spent). Either give the target ask a decline that
unwinds the cast, or make the cast row carry the refusal.

### MED

**M1. P5: a collapsed damage batch prints the batch's FIRST life total and never its last.** *Seats:
deck125 (M1), deck152 (LOW), deck162 (HIGH-2), deck50 (MED) — four seats, the most-reported item of
the wave.* **Repro
`1789690324-ai_baka_deck125-0x564f3c4d6640-vs-ai_baka_deck162.jsonl` seq229** —
`Underworld Dreams -> 1 damage to you (now 30) [x12 - 12 lines of this shape in this batch; only the
numbers in them differ]`, seat actually at 19 after the batch. Further instances:
`1789690323-ai_baka_deck162-0x55a0aea1f560-vs-ai_baka_deck50.jsonl` seq15 (`(now 17) [x8]`, true
total 10), `1789690322-ai_baka_deck50-0x55a0ad704700-vs-ai_baka_deck162.jsonl` seq18/19 (same batch
from the other seat), `1789690306-ai_baka_deck162-…-vs-ai_baka_deck152.jsonl` seq21 (`(now 17)
[x9]`, true 9). The annotation *"only the numbers in them differ"* asserts the collapse is lossless
while dropping the one number that matters; deck50 adds that the per-line amount is also unknown, so
an 8×1 batch cannot be told from any other split. Fix: end the collapse with the batch's final total
and state the per-line value when it is constant (`8 x 1 damage, you 17 -> 10`). No decision is
provably traceable to it — the `CURRENT SITUATION` header carries the true life on the same prompt.

**M2. P5: opponent spell casts render with no actor prefix, inside the seat's own turn block.**
*Seats: deck123 (MED-1), deck146 (MED-4).* **Repro
`1789690316-ai_baka_deck146-0x555bfc3f8960-vs-ai_baka_deck125.jsonl` seq230**, T35:
`… cast Emeria's Call; cast Cancel; your Emeria's Call was COUNTERED by Cancel …` — their Cancel is
a bare `cast Cancel`, identical in shape to the seat's own casts (12 such lines in that game). Same
at `1789690308-ai_baka_deck123-0x5646f41325c0-vs-ai_baka_deck125.jsonl` seq62, where the opponent's
Path to Exile sits unattributed between two of the seat's own casts; the OLD register attributed it
(the `events` field of seq56 in the same file reads `- Opponent cast Staff of Nin`). 30+ lines in
deck123's six logs. Costs 4 bytes per line to fix and removes the only ambiguity either seat found
in the new register.

**M3. `async_drops` rose 9 → 52 corpus-wide and has no per-window provenance.** *Seats: deck130
(MED-3), deck146 (LOW-6), deck162 (MED-3); deck123 and deck152 answered the brief's question from
stderr instead.* The count lives only on `gameend`; no `window`, `hold_event` or `recovery` record
in any seat log carries a drop field. **Repro
`1789690335-ai_baka_deck130-0x5586b2e3ab70-vs-ai_baka_deck123.jsonl` `gameend` `async_drops: 3`,
with no other record in the file mentioning it.** The brief asked every seat what dropped and
whether the re-ask was right; three seats could not answer from the translog. Where stderr was read
the answer is good — deck123: 12 drops, all `[outcome: re-asked]`, every re-ask correct; deck152: 10
drops, all re-asked, the following window was the correct current question each time (repro line
`game-152v50-1789690301.stderr:1034`). deck162 notes the drop count matches the blockers-window
count in two of its games but not corpus-wide, so blockers alone is not the cause. Ask: an
`async_drop` side record carrying seam, `window_seq` and what was discarded.

**M4. Discard / bottom / reveal seams print bare card names with no verdicts, costs, types or copy
counts.** *Seat: deck123 (MED-2).* **Repro
`1789690333-ai_baka_deck123-0x56535056ce90-vs-ai_baka_deck162.jsonl` seq41** (Cleanup, discard),
whose entire option list is `['Bloodline Keeper', 'Bloodline Keeper', 'Intruder Alarm', 'Thraben
Doomsayer', 'Marsh Flats', 'Damnation', 'Tribute to Hunger', 'Devour Flesh']` — no cost, no
`copy N of N`, no `dead right now`, no `{spare:}`, and the two identical Keepers not marked as
copies of one another. Every discard/bottom/reveal window in that seat's six games is the same bare
list (also seq54, seq83; deck125 seq6/seq9; deck130 seq45; deck146 seq4; deck152 seq33). Guide
sections written to read verdicts this seam never prints.

**M5. A tutor/search window lists the whole library, unfiltered and uncollapsed.** *Seat: deck123
(MED-3).* **Repro
`1789690335-ai_baka_deck123-0x5586b1b1b620-vs-ai_baka_deck130.jsonl` seq45** (seam `reveal`): 45
rows, the entire library, for a card whose own casting row said `finds only an enchantment card`;
nothing marks which rows are legal and identical entries are not collapsed the way P3 collapses hand
entries. Same at deck125's seq9 (48 rows). Both picks were right; the surface offers illegal
answers.

**M6. P5: an activated ability's damage renders as the attacker's combat damage on the `Attack:`
line.** *Seat: deck130 (MED-2).* **Repro
`1789690335-ai_baka_deck130-0x5586b2e3ab70-vs-ai_baka_deck123.jsonl` seq67**, T18 block:
`Attack: Siege-Gang Commander, Goblin, Goblin -> Siege-Gang Commander: 2 damage, opp 4; … used Deal
2 damage with Siege-Gang Commander -> the opponent; …`. Siege-Gang was BLOCKED; those 2 came from
the sacrifice activation, and the line that names the activation carries no result at all. A seat
reading the log concludes a blocked attacker hit the player.

**M7. P5: the same draw step is narrated twice, with different counts.** *Seat: deck162 (MED-4).*
**Repro `1789690323-ai_baka_deck162-0x55a0aea1f560-vs-ai_baka_deck50.jsonl` seq15** — turn line
`T13 (opp): drew.` followed by `Draw: opp put a card from their hand into their library (x6); drew
6; …`. 7 turns across that seat. A deck whose whole clock is cards-drawn × punishers can read 1
where the answer is 6, or add the two. Ask: when a turn gets a `Draw:` block, the turn line should
not also carry the draw.

**M8. P5: damage that fails to kill a creature prints no outcome line.** *Seat: deck123 (MED-4).*
**Repro `1789690335-ai_baka_deck123-0x5586b1b1b620-vs-ai_baka_deck130.jsonl` seq75**, T20:
`Starstorm -> 4 damage to Lord of Lineage; Starstorm -> 4 damage to Rorix Bladewing; your Vampire
died …`. Both survived and neither says so, while every body that died carries a `died` clause — a
reader counting the board from the log undercounts survivors. Sibling of **H3**: the register's rule
is "damage carries the new life total"; creatures have no equivalent.

**M9. The X row's "best X" badge argues against the row's own arithmetic, and the refusal marker
never fires.** *Seat: deck125 (M3).* **Repro
`1789690324-ai_baka_deck125-0x564f3c4d6640-vs-ai_baka_deck162.jsonl` seq296** option 1:
`[<- best X for this cast: X=9 … but NET -20 life for this cast … this KILLS you. X=2 is the largest
listed X whose NET (-6) leaves you alive, at 2]`. The pilot took the survivable figure the badge
named and cast a spell every stop on its guide refuses; `x_cast_row_refusal_markers` fired 0 times
in that seat's six games. The DL lane's refusal-first marker is the shape this row needed.

**M10. The hold row is offered where every other row is a free loyalty ability, while the same
screen prints a LETHAL crack-back verdict.** *Seat: deck146 (MED-2).* **Repro
`1789690314-ai_baka_deck146-0x56152ebf2920-vs-ai_baka_deck152.jsonl` seq36** — rows were Lolth +0,
Lolth -3, Hold, Pass; nothing on the menu costs mana, so the hold can only give value away. The DK
re-open rescued it at seq40/seq41, but in Main 2, so the two Spiders were summoning-sick and could
only block. Suggest suppressing the hold row (or printing the X-style refusal marker on it) when
every non-pass row is a zero-mana activated ability.

**M11. `verdict_clamped` storm — 44 of the corpus's 108 `hold_verdict_safer_ignored` are one game,
from two verdict faces that say the same thing.** *Seat: deck146 (MED-3).* **Repro
`1789690316-ai_baka_deck146-0x555bfc3f8960-vs-ai_baka_deck125.jsonl`, hold_event seq140 onward**,
`reason: held [stack death verdict: you survive the stack] over live [stack death verdict: nothing
lethal on the stack]`, in bursts of 9 / 10 / 10 / 15 on turns 38 / 40 / 42 / 46 — one per Staff of
Nin ping. Folding the two faces into one removes the burst without changing a decision.

**M12. Engine-wide guide literals that render 0 times — a cross-guide audit, not one deck's
problem.** *Seats: deck152 (two MED items + E1/E5), deck125 (L3 + REPAIR 1/2).* Dead strings found
this wave: `...blocking can leave them as high as N` (0 of 23 `ATTACK TOTAL:` renders, against 15 /
5 / 3 / 5 renders of four live forms — repro
`1789690346-ai_baka_deck152-0x56162355a630-vs-ai_baka_deck123.jsonl` seq14); `unless the board
changes` as the hold row's own words (0 renders against 174 of the live text — repro
`1789690306-ai_baka_deck152-0x5650922c8630-vs-ai_baka_deck162.jsonl` seq33); `you SACRIFICE this` (0
renders; the live header is `FORCED sacrifice OF YOUR OWN CARD` — repro
`1789690314-ai_baka_deck152-0x56152ff2b2a0-vs-ai_baka_deck146.jsonl` seq12); `[no cast row now: it
must have a target …]` (0, live string `[no cast row now: no legal target - HELD]`, 637 windows);
`LIFE-TO-DAMAGE CONVERTER` and `LOOP` (**0 of 2,122 windows across all 42 seat logs**, with
`own_loop_verdict_lines_rendered` 0 — deck125 keys four rules to them and left them standing under
the literal-audit rule because reachability in the pool is not disproved). deck152 and deck125
re-keyed their own guides; the same strings should be swept across every guide in the pool.

**M13. `check-reply-instructions.py` gives a staged `strategy.txt` ZERO coverage — the briefed
verification passes vacuously.** *Seat: deck50 (MED).* `GUIDE_SUFFIX` is `("_strategy.txt",)`, so
`python3 projects/mtg/tools/check-reply-instructions.py projects/mtg/src
strategy-design/wave81/deck<N>` reports `0 guide(s)` and exits 0 whatever the guide contains.
Repro: run it on `wave81/deck50`, then on a copy renamed `deck50_strategy.txt` and watch the guide
count go 0 → 1. Fix: match `strategy.txt` as well, or have the brief name the staged file
`deck<N>_strategy.txt`. (deck50, deck125 and deck152 each re-ran the check against a renamed copy;
all clean. Other seats' "OK" lines may be vacuous.)

**M14. The Upkeep animation row does not say the animation cannot block on the opponent's turn.**
*Seat: deck146 (MED-5).* **Repro
`1789690314-ai_baka_deck146-0x56152ebf2920-vs-ai_baka_deck152.jsonl` seq45** — the row's clause is
`[Upkeep offer: this animation lasts only until end of turn, and the same row is offered again in
your main phase]`, which is true but is about the SAME turn; the reply planned to "Animate Hive to
block Brute" on the opponent's turn. Extending the clause to name the block case closes a plan the
current wording does not refute.

### LOW

**L1. `plan_line_missing` counts the LABEL, not the plan.** *Seats: deck130 (LOW-1), deck146
(LOW-9), deck152 (LOW), deck162 (MED-5).* 61 corpus-wide (2.9%); per-seat rates vary widely —
deck162 6.6% (9/137), deck152 4.6% (13/285), deck146 5.6% (19/342), deck130 13 records, deck50 4.3%,
deck123 0. In every case any seat checked, the plan was present, was a plan, and PRECEDED the action
line, and the parser read all of them (0 `action_before_plan` corpus-wide). Repro:
`1789690320-ai_baka_deck130-0x55be47cbf420-vs-ai_baka_deck50.jsonl` seq14, reply
`"\n\nCast Stone Rain at Island #1.\nCHOICE: 3 (Cast Stone Rain)"`. Under the standing ruling these
are substantively compliant; the counter's NAME reads as a protocol failure rate. Rename it, keep
the rate visible.

**L2. Reserved ask replays consume a `seq` and write no record.** *Seat: deck162 (LOW-8).* **Repro
`1789690333-ai_baka_deck162-0x56534f251900-vs-ai_baka_deck123.jsonl`** is missing seq 8, 9, 14 and
15 entirely, against `identical_ask_answers_reserved: 4` on its `gameend`. A reviewer cannot see
which window was re-served or confirm the latch key was right. One side record closes it.

**L3. P5: the discard punisher's life-loss line prints before the discard that causes it.** *Seat:
deck162 (LOW-6).* **Repro
`1789690325-ai_baka_deck162-0x564f3d7f1710-vs-ai_baka_deck125.jsonl` seq35**, T21:
`Cleanup: opp lost 2 life (now 28); opp discarded Plains.` Ordering only; both facts present, total
right.

**L4. Ten consecutive identical windows in one turn, ~19 minutes of wall clock.** *Seat: deck123
(LOW-1).* **Repro
`1789690333-ai_baka_deck123-0x56535056ce90-vs-ai_baka_deck162.jsonl` seq16–25**, all T5,
Upkeep→Cleanup, all 3 rows, all answered `Cast nothing right now`, every one a live round trip
(63–160 s). `crossphase_identical_reputs` 9 in that game. Worth asking whether a cross-phase
identical re-put on an unchanged board can latch at the FIRST window rather than the second.

**L5. `counter_intel_rendered: 2022` on a 151-window game.** *Seat: deck125 (L1).* Every other
counter in that `gameend` record is a window or event count; the magnitude reads like a byte total
in a count-named field. Repro: `1789690337-ai_baka_deck125-0x55f86ec66780-vs-ai_baka_deck50.jsonl`
`gameend`. Cosmetic, but it will mislead a census.

**L6. `chosen_text_core` is null on folded land rows with no bracket annotation.** *Seat: deck125
(L2).* Repro: `1789690316-ai_baka_deck125-0x555bfd731400-vs-ai_baka_deck146.jsonl` seq19
(`Play Island`, `chosen_text_core: null`) against seq58 (`Play Seachrome Coast [enters TAPPED …]`,
populated). Harness bookkeeping only — it makes any land-drop take-count keyed on
`chosen_text_core` undercount.

**L7. `deck162.txt` still carries `#HINT:dontattackwith(Fate Unraveler)` and
`#HINT:dontblockwith(Fate Unraveler)`.** *Seat: deck162 (LOW-7).* Those hints steer `AIPlayerBaka`,
never reach the GPT seat, and the guide directs the opposite in both cases — both guide behaviours
fired correctly this corpus. Dead-hint hygiene; no behaviour change asked.

### Checked and CLEAN — recorded so step one does not re-open them

- **bug-list #11/#12, `garbageReaddRefused`:** the dev stderr line `WAGIC addToGame REFUSED a
  garbaged element` is **absent from all 21 stderr files** in the corpus dir. Independently verified
  by all seven seats (deck123, deck125, deck130, deck146, deck152, deck162, deck50). The guard is
  silent.
- **P10 whole-order answer — first live firing, 3 of 3 parsed.** deck130's single order window
  (`1789690335-ai_baka_deck130-0x5586b2e3ab70-vs-ai_baka_deck123.jsonl` seq73, `ORDER: B1, B2`,
  `chosen_text` `'Vampire, Vampire'`) and deck146's two
  (`1789690314-ai_baka_deck146-0x56152ebf2920-vs-ai_baka_deck152.jsonl` seq38 and seq39,
  `ORDER: B2, B1`). No fallback to per-pick asks; seq39 was a bare `ORDER: B2, B1` with no plan at
  all and still parsed. deck123, deck125, deck162 and deck50 drew 0 order windows.
- **Parser robustness on two more shapes** (deck146 LOW-9): attackers answered by card NAME instead
  of A-number parsed correctly —
  `1789690342-ai_baka_deck146-0x5645001b4aa0-vs-ai_baka_deck130.jsonl` seq21
  (`ATTACK: Triumphant Adventurer, Silverquill Silencer, Goblin`), seq32, and
  `1789690340-ai_baka_deck146-0x55fd33662980-vs-ai_baka_deck123.jsonl` seq40 (`ATTACK: Spider`).
  The one shape that did NOT parse is **H7**.
- **No card-script / Oracle item was raised by any seat.** Six of the seven explicitly say they did
  not open a `primitives/*.txt` or retrieve Oracle text; deck152 read two primitives (Katilda,
  Luminarch Aspirant) for its own guide edit and found the engine matching the script. Nothing in
  the corpus behaved mechanically wrong, so the card lane is empty this wave — an absence of
  checking as much as an absence of bugs.
- `reply_truncated` 0, `action_before_plan` 0, `multi_answer_first_taken` 0, `menu_pass_no_progress`
  0 corpus-wide; `reask_reason` fired at no seat.

---

## 3. P9 — the land drop as a casting-menu row: the seven seats' counts, collected

The owner's revert criterion is "degrades behavior". **No verdict is offered here.** Units differ by
seat and are named in each row; every seat's own §3 carries its per-game split.

| seat | turns / windows with a land row | drops available and NOT taken | land row taken ahead of a spell that should have gone first | the seat's own note on the misses |
|---|---|---|---|---|
| **deck123** | 47 windows (27 folded + 20 standalone) | **1 turn** — `…-vs-ai_baka_deck50` T16, seq118 and seq119 | **0** | Both declines were guide-REQUIRED: lethal was already on the board and the seat won that turn (seq133, 20 attackers, opponent to −24). Also 3 turns took a spell before an available land row; none cost a castable spell. |
| **deck125** | 94 windows over 77 turns (58 folded + 36 standalone) | **1 of 77 turns** — `…-vs-ai_baka_deck146` T47, seq265 and seq268 | **0** | Game already decided (Emrakul cast T46 and attacked T47); cost zero. Land played on 76 of 77 turns. In 7 windows the land taken first is what paid for the spell that followed. |
| **deck130** | 44 windows over 38 turns | **0 of 38** | **0** | Casting window re-opened after the land entered in every case; both orders occur and neither lost a spell or a drop. |
| **deck146** | 49 turns where `Land drop:` read "a land play IS available" | **2 turns** — `…-vs-ai_baka_deck125` T35 seq128; `…-vs-ai_baka_deck123` T17 seq39 | **0** | seq128 declined an MDFC land row to hard-cast into 15 open sources and was countered — card AND drop lost in one window. seq39 declined to pass to combat; that combat won the game. 17 windows carried both a land row and a live cast row; the land was taken in 9, and in every one the spell was cast later the same turn or was priced dead. |
| **deck152** | 42 windows (22 folded + 20 standalone) | **3 windows / 2 distinct (game, turn)** — `…-vs-ai_baka_deck50` seq73 and seq74 (T15); `…-vs-ai_baka_deck130` seq26 (T17) | **0** | All 3 are folded menus; 0 of the 20 standalone asks were declined. Both lost drops fell on a turn the seat won — 0 life, 0 games. seq26 is the plan/choice contradiction filed as **H1**. Of the 22 folded menus: land taken 14, cast taken 5, declined 3; no cast displaced. |
| **deck162** | 45 turns | **0 of 45** | **0** | Land row taken in the turn's first casting window every time; spell rows re-offered the same turn. Three turns ended land-played-no-spell, each because the spell was declined on brake grounds, not displaced. |
| **deck50** | 67 windows over 47 turns (35 folded + 32 standalone) | **0 of 47 turns** (one WINDOW declined the drop, `…-vs-ai_baka_deck146` seq10 T6, recovered at seq11 in main 2) | **0** | The 16 windows where a spell was taken over the land row were all re-put in the same turn and the drop was made. Seat-level benefit recorded: on a folded menu the drop and the cast were priced against each other in one window (`…-vs-ai_baka_deck162` seq18 played the Swamp that made Damnation `[castable now]` at seq19). |
| **TOTAL** | ~388 windows carrying a land row across the seven seats | **7 windows, ~6 distinct turns** | **0 — at every seat, in all 42 seat logs** | — |

Two facts that condition any reading of the table, both filed above as bug items: the folded row
coexists with the standalone `Land drop:` ask in every game (**H2**), so the seats measured a
half-shipped regime; and every folded menu still carries the header paragraph telling the pilot the
drop is a separate question that will be asked later (**H1**), which is the mechanism one seat
traced to a lost drop (deck152 seq26) and which the owner may wish to see fixed before reading the
revert criterion.

---

## 4. Skill

No `wave81/strategy-writing-skill-v2.md` is written. Reason in §1, third row.
