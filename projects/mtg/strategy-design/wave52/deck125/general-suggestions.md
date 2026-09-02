# wave-52 core-prompt / render proposals - deck125 seat

Layer-routed. Engine BUGS are in `seats/seat-125-126.md`; this file is prompt text and render
wording only.

## G3 - a `[DRAW PRICE:]` total at or above the pilot's life should carry lane K's lethal clause

**Where.** The `[DRAW PRICE: casting this draws YOU N cards (their Forced Fruition), and their
Underworld Dreams deals you N]` tag, printed on EVERY cast row while a Forced Fruition is on the
opponent's line.

**Repro.** `1788320083-ai_baka_deck125-0x557cd81d6290-vs-ai_baka_deck162.jsonl` seq 91 (t19, 3 life,
all seven rows carry `deals you 7`) and seq 97 (t20, 3 life; the reply took row 1 and the seq-98
events show seven draws and the seat at 2, then 1, then 0).

**Proposal.** Lane K already prints `{paying this costs you N life (...) - you would be at K}` and
`- this KILLS you` for a MANA cost that drains the caster. The Fruition draw price is the same
arithmetic reaching the same conclusion through a different clause, and it is the clause my seat
actually died to; lane K's life-cost form had zero windows on either of my decks this corpus
(UNTESTED-AT-THIS-SEAT) while this one had 7 rows on one menu. Extend the same tail:
`[DRAW PRICE: ... deals you 7 - you would be at -4; this KILLS you]`. One string, and it turns a
rule the guide has to teach into a fact the row states.

## G4 - opponent-turn priority windows whose only cast row cannot legally do anything

**Where.** Lane I's new opponent-turn stops. Across my 12 seat logs, 381 opponent-turn windows had
exactly two rows (one cast + `Cast nothing right now`) and 358 of them were passed (94%). Of those,
206 were `Cast Tribute to Hunger {right now: they control 0 creatures - at 0 this does nothing}` on
deck126, and 185 were `Cast Path to Exile` whose entire `legal targets right now:` clause was
[defender] / printed-power-0-or-1 bodies on deck125.

**Repro.** `1788320097-ai_baka_deck126-0x55b12e55f100-vs-ai_baka_deck125.jsonl`: 178 of the seat's
218 decisions (82%) are that one Tribute-at-0 window; the seat log runs 220 records for a 39-turn
game. `deck125 vs162` seq 70 is the Path-at-a-Shield-Sphere shape.

**Proposal (prompt/UX, not legality).** Lane I is a large win - it gave deck125 forty-odd
opponent-turn counters and turned a 2-4 seat into 5-1 - and nothing here asks to take a choice away.
The ask is that a window be *skipped the way `mana_only_windows_skipped` already skips all-mana
windows* when EVERY cast row on it prints its own no-effect clause: `at 0 this does nothing` is the
engine stating that the only action is a null action. If skipping is judged to constrain choice, the
cheaper half is to keep the window but stop paying for a full prompt on it (the seq-215 prompt for
one of these is 18,086 chars, of which the menu is 5 lines).

**Falsifiable.** wave 53: opponent-turn windows whose only cast row carries `at 0 this does nothing`
reaching the model: 0 (was 206 on my seats). Windows where such a row is taken: 0 (was 0 - the
guide's zero-stop is at its ceiling, which is why the window buys nothing).

## G5 - the `[RE-ASK]`/parse path is healthy on this seat; the two failures were decode, not format

`deck125` produced 2 `unparsed_reply` fallbacks in 869 decisions, both degenerate decodes at ~3.2 s
latency (`vs146` seq 50, reply `跟 \.8`; `vs152` seq 136, reply a single space) on ordinary 2-3 row
menus late in long games (t32, t52). No prompt-format cause is visible - the same menus were
answered normally before and after. Recorded so the fallback count is not read as a format problem.
