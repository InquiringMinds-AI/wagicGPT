# wave-81 review — deck125 (Revelation Fracture), UW draw-go control

Corpus `matchups-20260917-191141-final`, binary `wagic-6e0c51238-w81step1`. Six games, record **2/6**.
Seat windows 544 (ask 390, priority 121, discard 30, bottom 3, attackers 1 — 0 blockers, 0 order, 0 reveal).
Protocol: 544/544 `compliant`, 0 `plan_line_missing`, 0 `off_protocol_bytes`, 0 fallbacks, 0 `reask_reason`
records, thinking on at every window. Latency p50 by game 61–135 s, max 382 s.

## 1. Game by game

Seat logs are `<epoch>-ai_baka_deck125-<ptr>-vs-ai_baka_deck<M>.jsonl` in the corpus dir; seqs below are
that file's `seq`.

**vs deck123 (Intruders Of Thraben) — WIN 34–0, turn 28.** Textbook execution of the guide. Staff of Nin
cast t11 (seq28) and a second t15 (seq41), a ping at the face in every window offered (seq29/33/40/43/45/
46/52/53/63/64/77/78/86/87/100/101/108), Revelation only in main phase (seq36/37 X=3, seq49/50 X=6), Path
spent on power (seq15, seq44, seq55), Elixir cast and activated at seq102–104. No deciding error found.

**vs deck146 (Orzhov Dungeons) — WIN 63–(−11), turn 46.** Long grind decided by the clock: two Staffs, then
Emrakul at seq239 with their life 6, their battlefield line reading 0 creatures and no converter paragraph —
EMRAKUL step 1 exactly — and the attack at seq282 closed it. Supreme Verdict correctly declined at K=0 in
nine windows (seq19, 31, 113, 136, 150, 162, 199 among them). One P9 miss, seq265/268 (below), on the final
turn with the game already decided.

**vs deck130 (Budde's Ponza) — LOSS −1, turn 23.** Not a guide failure: five land-destruction spells
(Molten Rain ×2, Stone Rain ×3) held the seat to 1–3 lands all game. The seat had **zero priority-seam
windows in 24 turns** — with one land on the battlefield from t5 to t21 nothing was payable, so no counter
window was ever opened. Essence Scatter was spent correctly on Siege-Gang Commander (seq13, t12). At seq16
(t23, life 1) the only castable row was a Revelation at X=0; at seq20 a Siege-Gang activation on the stack
was lethal and no row answered an ability. Mulligan to five (seq2/seq3) followed the guide's ONE LAND rule
on a genuine one-land seven and one-land six — hand variance, not guide effect.

**vs deck152 (Bant Midrange) — LOSS 0–22, turn 16.** *Deciding decision: seq48.* At t14 upkeep, life 13,
six lands, the seat took `Life with Elixir of Immortality` for {2} (guide-mandated: "ACTIVATE it whenever
your life is 15 or less … ahead of any spell"). Their header read four creatures, K of 4. At seq50, the
same turn's main phase, `Final Judgment {4}{w}{w}` printed `[cannot pay now: needs 6 mana, you have 4
untapped sources]` — the two mana the activation spent were exactly the sweeper's shortfall, and the
SWEEPERS rule ("K of 2 or more: cast Supreme Verdict, or Final Judgment") had no row to take. The board
then dealt 13 in one attack (t15) and the seat died at 5 life two windows later. Secondary: seq26 (own
End step) and seq50 took the hold row under the declined-N override, each covering the whole of the
opponent's next turn; in both the seat held only unpayable or creature-only counters, so no proven cost.

**vs deck162 (Draw and Die!) — LOSS −3, turn 33.** Underworld Dreams + Teferi's Puzzle Box (both resolved
t28) turned every draw step into 12–16 damage. *Deciding decision: seq296/297.* At t33 upkeep, life 10,
with `DRAW PUNISHERS on the battlefield: theirs - Underworld Dreams` printed — Revelation stop (1), and the
low-life floor explicitly does not lift stop (1) — the seat cast Sphinx's Revelation for X=2 off the row's
own "X=2 is the largest listed X whose NET (−6) leaves you alive" badge. The two extra cards raised the
hand, the Puzzle Box draw step that followed drew nine, and the seat went from 17 to 1 (seq301). The guide
was right and the pilot reasoned past it; the badge is the competing surface (item M3). At seq211 (own End
step, t27) the hold row under the declined-N override covered t28, where Underworld Dreams and Teferi's
Puzzle Box resolved — the seat held only Essence Scatter ×2 against two noncreature spells, so again no
proven cost.

**vs deck50 (Erosion, mill) — LOSS by decking, turn 42, at 59 life with the opponent on 7 and 1 card left.**
The seat was winning on both clocks and killed its own library. Their board: Memory Erosion ×4 ("Whenever an
opponent casts a spell, that player mills two cards" — eight cards per cast of yours) plus Howling Mine ×3.
*Deciding decision: seq281/282.* At t39, library 25, the seat cast Sphinx's Revelation and announced X=12
per the guide's "ANNOUNCE THE LARGEST X THE CEILING ALLOWS"; the X row promised "13 left" and the library
read **5** at the next window (seq283). Compounding: seq301 (Cancel, library 30 → 22 at seq302) and seq305
(Cancel at library 11, cast to counter a Tome Scour that would have milled five, costing eight) left it on
2 (seq306) and it drew out at t43. The guide has no mill rule and the screen has no cast-price for
mill-on-cast (items H1/H2).

## 2. Engine / interface / card items

**H1 — no deck-out warning on YOUR OWN library, while the opponent's line carries one.** The prompt prints
`Opponent library: 1 cards - DECK-OUT IS IN RANGE: a player who must draw from an empty library LOSES …`
but `Your library: N cards` is bare at every N. Corpus-wide check (all 42 seat logs, 2,122 windows): the
`DECK-OUT IS IN RANGE` string renders on the opponent line only — 15 windows — and never on the seat's own,
and the only annotation the own-library line ever carries is the search/reveal parenthetical (6 windows).
Repro: `1789690337-ai_baka_deck125-0x55f86ec66780-vs-ai_baka_deck50.jsonl` seq305, which prints
`Opponent library: 1 cards - DECK-OUT IS IN RANGE: …` and, four lines later, `Your library: 11 cards`; the
seat lost to its own empty library eight windows later. The asymmetry is what lost the one game deck125
lost from a winning position.

**H2 — the library forecast on a cast/X row omits the mill the cast itself triggers.** With four Memory
Erosions on their battlefield, every cast of the seat's mills it eight. Repro: same file, seq282, option 12
reads `X = 12 … {library: this draws 12 of your 25 library cards - 13 left; 6 draws are already owed that
you cannot decline, and the 13 left still pays them}`; the library at seq283 is **5**, not 13. Same shape
at seq301 (`Cast Cancel` taken at library 30, library 22 at seq302). A cast-price clause of the DRAW PRICE
kind ("every spell you cast mills you N") would price this the way `DRAW PRICE:` prices Underworld Dreams.

**H3 — a hold taken at the seat's OWN End step silences the whole of the opponent's next turn.** The row's
scope is "until your next turn begins", so an End-step hold skips every cast and priority window of the
opponent's turn, and the DK re-open fires only for a new lethal. `1789690324-…-vs-ai_baka_deck162.jsonl`
seq211 (t27 End) → t28 Underworld Dreams and Teferi's Puzzle Box resolved with no window asked (30
`hold_verdict_safer_ignored` events in that game, 46 `hold_events`, 277 `hold_windows_skipped`);
`1789690345-…-vs-ai_baka_deck152.jsonl` seq26 (t12 End) → 19 skipped windows covering t13's Ranger Class and
Brutal Cathar. Neither cost a provable answer (the held counters were unpayable or creature-only), so this
is ranked on mechanism, not on a lost game: at this seam the option that closes one window and the option
that disarms a whole opponent turn are the same row.

**M1 — P5 batch collapse keeps the FIRST line's life total.** `…vs-ai_baka_deck162.jsonl` seq229, GAME LOG:
`Underworld Dreams -> 1 damage to you (now 30) [x12 - 12 lines of this shape in this batch; only the numbers
in them differ]`. The seat was at 19 after that batch. The old register printed twelve lines ending
`(now 19)`; the compacted one names the count and the opening life and never the closing one, so the log
cannot be read for the rate that was killing the seat. Three instances in that game (t29, t31, t33), one
benign instance in the deck146 game (a sacrifice batch). Cheapest fix: carry the batch's LAST life total.

**M2 — a reasoning-budget overrun re-asks with reasoning disabled.** Two windows in six games:
`…vs-ai_baka_deck152.jsonl` seq23 (`reasoning_chars` 20,683) and `…vs-ai_baka_deck162.jsonl` seq300
(26,808), both `reasoning_budget_hit: true`, `reasoning_forced_close: true`, `retry: 1`,
`max_tokens_reasoning: 0`. Both answers happened to be right, but the retry path answers with the native
reasoning channel switched off, which is the one thing the reply protocol's owner ruling forbids. A longer
budget or a re-ask that keeps thinking on would hold the ruling.

**M3 — the X row's "best X" badge argues against the row's own arithmetic.** `…vs-ai_baka_deck162.jsonl`
seq296 option 1: `[<- best X for this cast: X=9 - largest affordable X - X=9 gains 9 life and draws 9 cards;
no listed X does more - but NET -20 life for this cast … this KILLS you. X=2 is the largest listed X whose
NET (-6) leaves you alive, at 2]`. The pilot took the survivable figure the badge named and cast a spell
that every stop on its guide refuses. `x_cast_row_refusal_markers` fired 0 times in all six games; a
refusal-first marker on a row whose own NET is negative at every X is the DL-lane shape this row needed.

**L1 — `counter_intel_rendered: 2022` on a 151-window game** (`…vs-ai_baka_deck50.jsonl` gameend) while
every other counter in that record is a window or event count; the magnitude reads like a byte total in a
count-named field. Cosmetic, but it will mislead a census.

**L2 — `chosen_text_core` is null on folded land rows with no bracket annotation** (`Play Island`,
`Play Plains`) and populated when the row carries one (`Play Seachrome Coast [enters TAPPED …]`). Repro:
`…vs-ai_baka_deck146.jsonl` seq19 (`chosen_text_core: null`) against seq58 (populated). Harness bookkeeping
only; it makes a take-count keyed on `chosen_text_core` undercount land drops.

**L3 — two guide literals render nowhere in the corpus.** `LIFE-TO-DAMAGE CONVERTER` and `LOOP` print in
**0 of 2,122 windows across all 42 seat logs**, and the census reports `own_loop_verdict_lines_rendered` 0 /
`own_loop_windows_asked` 0; deck125's guide keys four rules to them (its converter/loop section, EMRAKUL
steps 1 and 3, PATH step 1) and sibling guides likely do too. I could not establish that the window arose
in these six games (no Sanguine Bond / Exquisite Blood reached a deck125 opponent's battlefield line), so
the rules STAY per the literal-audit rule — but if the render was renamed in the DL/DM lanes, every guide
that quotes them is holding dead text and this is a cross-guide repair.

Not found: no `WAGIC addToGame REFUSED a garbaged element` line in any of the 21 stderr files in the corpus
dir; no fallback, no `reply_truncated`, no `action_before_plan`, no `menu_pass_no_progress` at this seat.

## 3. P9 and P5 counts

**P9 (land drop folded into the casting menu).** Both surfaces are live for this seat: 58 windows carried a
folded `Play <land>` row on a casting menu and 36 were separate `Land drop:` asks (94 windows over 77
turns).

- Turns where a land drop was available and NOT taken while a land row was on the menu: **1 of 77**
  — deck146 game, turn 47, seq265 and seq268, both answered with the folded decline row
  `Cast nothing right now (combat comes next this turn) - and play no land in this window`. The game was
  already decided (Emrakul had been cast t46 and attacked t47), so the cost was zero.
- Turns where a land was played: **76 of 77**. Per game: 11/11, 21/22, 13/13, 8/8, 16/16, 7/7.
- Turns where the land row was taken instead of a spell that should have gone first: **0**. In the 36
  windows where a land row was taken with cast rows also on the menu, every deferred cast was either one
  the guide refuses on that board (a sweeper at K=0 — 145 zero-verdict sweeper rows offered at this seat,
  0 taken; a Lightmine Field at N=0 — 28 offered, 1 taken and that one sanctioned by the guide's
  discard-punisher step) or was cast later the same turn. In seven of them the land taken first is what
  paid for the spell that followed: seq27 and seq48 (deck123 game → Staff of Nin, Revelation), seq99,
  seq111, seq122 (deck162 → Staff, Revelation, Path), seq167 and seq275 (deck50 → Revelation).
- One surface note, no edit: the guide's LAND DROPS rule quotes the separate `"Land drop:"` question, which
  still renders (463 windows), and the pilot applied it to the folded rows without being told to. The
  folded decline row (`… - and play no land in this window`) is not named anywhere in the guide and is the
  row the single miss was taken on.

**P5 (compacted GAME LOG).** One class of defect found, four instances, all the batch-collapse shape in
item M1 — quoted line and the old register's version are there. No other omission or mis-statement found:
the compacted register carried every fact the seat needed in the decks I traced (the deck130 log, read
end to end at seq20, states each land destruction, each burn spell with the resulting life total, and the
cycling that hid Starstorm; the deck50 log narrates every mill by name).

## 4. Guide verdict — **EDIT**

Staged at `wave81/deck125/strategy.txt`, **19,995 bytes** (live guide 19,957; ceiling 20,000).
Verification: A332 grep `corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for
)?by|[0-9]{3}v[0-9]{3}` = 0; `you (declined|took|cast|passed) [0-9]` = 0; frequency-adjective grep = 0;
`check-reply-instructions.py projects/mtg/src <guide>` = OK, 1 guide (the tool matches `*_strategy.txt`
only, so it was run on a copy of the staged file under that name).

Two rules added, two literals repaired, three passages cut to pay for them. The two additions cost 460
bytes; the cuts and repairs returned 422, and 38 came from the live guide's headroom.

**ADD 1 — the Elixir activation yields to a sweeper the board is calling for.** Paid for by seq48/seq50
of the deck152 game.
Before: `ACTIVATE it whenever your life is 15 or less OR "Your library: N cards" reads 16 or less - in THAT
window, on a priority menu as much as on a casting menu, ahead of a Staff ping, ahead of any spell and
ahead of the hold row.`
After: `… reads 16 or less - in THAT window, unless its {2} is what a Supreme Verdict or Final Judgment on
your hand line still needs at a header second number of 2 or more, which puts that sweeper first - on a
priority menu as much as on a casting menu, ahead of a Staff ping, ahead of any spell and ahead of the
hold row.`
Why an inline exception and not a new paragraph: this is a collision between two imperatives that fire in
one turn (ELIXIR "ahead of any spell" against SWEEPERS "K of 2 or more: cast"), and the skill puts the
resolution at the site of the later imperative, inline with the take-it instruction it excepts.
Prediction: on a window at the seat's own upkeep or draw step with an Elixir activation row, a Supreme
Verdict or Final Judgment on the hand line, and the opponent header's second number at 2 or more, the
activation is declined whenever `Mana available:` minus 2 is under that sweeper's printed cost. A hit is a
break only if the activation is taken on such a window AND the sweeper prints `[cannot pay now: …]` in the
same turn's main phase. Denominator: windows matching that shape, all games.
Removal condition (belief-correcting): the render prints the sweeper's shortfall on the activation row
itself AND two corpora pass with zero breaks on the triggering shape.

**ADD 2 — MILL PUNISHERS.** Paid for by seq281/282 (and seq301/302, seq305/306) of the deck50 game.
Added at the end of PUNISHER BOARDS, in the grammar of its Forced Fruition entry:
`- MILL PUNISHERS: while their line prints a card reading that a spell you cast "mills two cards", each
cast of yours costs two cards per printed copy off "Your library: N cards", counted on no row. At 25 or
less take only an Elixir ACTIVATION, the one row that puts cards back, or a Staff ping.`
Why here and not as a sixth Revelation stop: the deck's own guide already refuses casting under a
punisher board from this section (the Forced Fruition entry reaches the Revelation the same way), and a
sixth entry in the "FIVE PRINTED STOPS" list would contradict that list's own closing sentence.
Prediction: with such a card on the opponent's battlefield line and `Your library:` at 25 or less, the
only rows taken are the Elixir activation and Staff pings. A hit is a break only if another row is taken
on such a window. Denominator: windows matching that shape (deck50 is the only pool deck that produces
them today; 33 such windows in this seat's one game against it).
Removal condition (information-supplying): one corpus in which the cast/X rows carry the mill in their
own library count (item H2 fixed).

**REPAIR 1 — stale literal, TAP-OUT GATE.** The guide quoted `"[no cast row now: it must have a target and
there is no legal target on the board]"`, which renders 0 times in 2,122 windows; the live string is
`[no cast row now: no legal target - HELD]` (637 windows across 18 seat logs). Rewritten to the live
string. Returns 41 bytes.

**REPAIR 2 — stale literal, the Revelation CEILING.** The guide quoted `"… X=K is the largest value on this
menu the library pays for."`; the rendered sentence is `X=2 is the largest listed X the library pays for
(…)` (14 windows, deck146 game seq234). Rewritten to `X=K is the largest listed X the library pays for.`
Returns 10 bytes.

**CUT 1 (pays 187 B) — the TAP-OUT GATE's zero-verdict restatement**: `THE GATE ONLY EVER DECIDES WHETHER
YOU MAY AFFORD A ROW, NEVER WHETHER THE ROW DOES ANYTHING: a row whose own verdict reads zero is still not
a play, with an empty hand and an open gate.` It restates the absolutes in SWEEPERS (K is 0) and LIGHTMINE
FIELD (N of 0), and those hold on their own at this seat: 145 zero-verdict sweeper rows offered, 0 taken;
28 zero-verdict Lightmine rows offered, 1 taken (deck162 seq229) and that take was licensed by the guide's
discard-punisher step, not by a failure of this sentence.
Prediction for the trim: zero-verdict sweeper and Lightmine takes stay at 0 (excepting discard-punisher
windows). A restoration is owed if any such row is taken outside that carve-out.

**CUT 2 (pays 70 B) — the LAND DROPS decline restatement**: `"Play no land right now" is never your answer
while a land is listed.` The imperative above it ("TAKE ONE - every turn, at any life, with any hand, at
any land count, TAPPED OR NOT") already forbids it, and the measured behaviour is 76/77 with the one miss
taken on the folded decline row this sentence does not name.

**CUT 3 (pays 114 B) — the CEILING's hand derivation**: `With no such sentence printed, use "library, minus
2, minus one per Staff"` plus the sentence's trailing clause. The render now prints a per-row library count
on every X row (`{library: this draws N of your M library cards - K left}`, 188 X-pricing windows at this
seat), so the hand-computed fallback is a derivation the render performs; the clause that the row's own
count "counts the library ALONE" is kept, and ADD 2 supplies what that count leaves out.
Removal condition already discharged; a restoration is owed if an X window renders with neither the
ceiling sentence nor a per-row `{library: …}` count.

Rules considered and NOT changed: the Revelation stops (stop (1) was broken once, at seq296, with the fact
printed — the skill's rule is that a broken stop does not earn a fourth paraphrase, and the competing
surface is item M3, an engine item); the declined-N hold override (it drove 43 of the seat's 49 own-turn
holds and no window in these six games shows an answer it cost, so narrowing it would be unevidenced —
but see item H3, and it is the first thing to revisit if a later corpus shows a payable counter skipped);
the converter/loop section (item L3 — literals dead corpus-wide, but reachability in the pool not
disproved, so the rules stay).

## 5. Proposals

`general-proposals.md` — one proposal (own-library-as-a-resource). No skill proposal: everything this seat
learned about writing the guide is already covered by A228 (two imperatives in one window state which runs
first), the literal audit, and A332.
