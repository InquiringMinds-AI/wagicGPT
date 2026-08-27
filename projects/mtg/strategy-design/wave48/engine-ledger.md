# Wave-48 engine ledger — THE WAVE-49 DOCKET (engine / UX only)

Corpus `matchups-20260827-072004` (fifth fair-hand corpus), binary master **eccf8284c** (E1
5ce8f8a5a + lanes P 9b19fbfe6, O 96a07e864, N 7a858904d, Q 7246c4a95; PARSETEST 1655/0).
42 seat logs, **1,813 records**, **1,729 decisions**, **21/21 games natural** (every seat has a
`gameend`; longest seat 133 records). Wave 47 was 5,726 decisions with one dead-looped game.
Corpus root: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260827-072004/`. Every repro is a file
plus a `seq`; seat names `deckA-vs-deckB` = the deckA seat's JSONL. NOTE (D17): the JSONL
`turn` field is the narration's turn minus one; seat files quote the field.

**Health.** Fallbacks **5 / 1,729 = 0.289%** (baseline 0.10-0.31%: in band, upper edge).
`retracted_choice` **0** (was 2). `answer_replaced` **21** (E1 working). `commit_retracted` 0.
`parse_note`: `repeat_count_missing` x4, `echo_index_conflict` x1. Explicit `CHOICE: 0` passes:
**214** (was 0 in the loop game). Max prompt **61,240** chars (one prompt; next 27,987).
Max priority windows in one turn **9**. `mana_only_windows_skipped` max **835** (was 8,610).

**Ownership (owner doctrine 2026-08-26).** Engine/UX only below. Guide content is the
reviewers'; the guide-side halves are named only where a docket item retires one. Hard caps on
the model's legal choices are rejected; every item adds a true token, a true row or one re-ask.

---

# THE DOCKET, RANKED

## D1 — HIGH — the B-row `may block` list is unranged and unfactored: the last uncollapsed enumeration on the combat screens
(= seat-engine **D-1** / seat-125-126 O FAIL / `general-strategy.md` **R28**)
**Repro.** `1787833243-ai_baka_deck126-0x560971e0cf50-vs-ai_baka_deck123.jsonl` seq **15**,
blockers, **61,240 chars**, latency 28.7 s, reply `BLOCKS: B1:A1, B2:A3, B3:A4`. A side: 3 rows
(`A3-A257. Human #1-#255 (1/1) deals 1 [...] x255`). B side: `B1. Pride Guardian #1 ... - may
block A1 (neither dies (blocking trigger: you gain 3, and your converter takes 3 off them)),
A3 (neither dies (...)), ... A257 (...)` — **256 handles, ONE distinct parenthetical, 22,926
chars for B1**, repeated on B2 and B3. Every other prompt in the corpus <= 27,987.
**Ask.** (a) ranges over consecutive handles with an identical parenthetical (`may block A1
(...), A3-A257 (neither dies (...))`) — ~5 KB alone; and/or (b) factor an all-identical
parenthetical (`may block A1, A3-A257 (all: neither dies (...))`). The ranged handle already
exists on the A side. Secondary (D13): `B1. Pride Guardian #1` / `B3. Pride Guardian #2` split by
`B2. Overgrown Battlement`.
**Locus.** R8's B-row builder (`AIPlayerGPT.cpp` ~17472 / ~17591 as of wave 47) — the `may
block` clause emitter.
**Wave-49 prediction.** No prompt of any kind > 30,000 chars; blockers max < 15,000.

## D2 — HIGH — the E1 replacement executes a re-answer whose parenthetical NAME does not match the option at that index, and does not cover `ATTACK:`/`BLOCKS:` lines
(= seat-125-126 **HIGH #1** + deck126 G1 / seat-146-152-162 **E-1** + deck146 G-1 / **R29**)
**Repro (name/index).** `1787833231-ai_baka_deck126-0x55ee9173e000-vs-ai_baka_deck130.jsonl`
seq **25** (main 2, 2 options `1. Cast Tribute to Hunger ... 2. Cast nothing right now`). Line 1
`CHOICE: 2 (Cast nothing right now)` ("Rule #4 Zero Stop"); 2,563 chars; last coded line
`CHOICE: 1 (Cast Sanguine Bond)` — not on the menu. `answer_replaced: true`, `choice: 1`,
`chosen_text: Cast Tribute to Hunger ... casting this taps you out`; cast at `0 are creatures`,
nothing sacrificed, tapped out.
**Repro (ATTACK line).** `1787833230-ai_baka_deck146-0x5622aed801a0-vs-ai_baka_deck126.jsonl`
attackers seq **48**: reply opens `ATTACK: A1, A2`, ends `ATTACK: A1` after "Do not sacrifice
Spider"; engine sent both (`chosen_text: "Silverquill Silencer, Spider"`); Spider died, 16 -> 12;
record reads `answer_replaced: true` — the field states the opposite of what executed.
**Ask.** (a) validate the replacement's parenthetical against the option at that index; on
mismatch keep the earlier matching coded answer, else fallback; count it as
`named_row_not_offered` — which also re-classifies this corpus's four `unparsed_reply`
fallbacks (all `CHOICE: n (<card not on the menu>)`, n out of range). (b) apply the
last-coded-line rule to `ATTACK:` / `BLOCKS:` (or state in those tails which line is taken), and
never set `answer_replaced` when the executed answer is the FIRST coded line.
**Locus.** the E1 replacement path added at 5ce8f8a5a (`choiceRetractedNoReplacement` region,
`AIPlayerGPT.cpp` ~11490 as of wave 47); the attackers/blockers reply scanners.
**Wave-49 prediction.** Replacements executing an index whose name is not on the menu: 0;
`answer_replaced` records whose executed ATTACK/BLOCKS matches the first line: 0 (was 1);
`named_row_not_offered` reported separately from `unparsed_reply`.

## D3 — HIGH — the repeat-row count parser accepts one spelling; a missing count runs once with no receipt
(= seat-engine **D-2** / seat-123-130 **H1** / **R30**)
**Repro.** `1787833239-ai_baka_deck123-0x55ca87a19a00-vs-ai_baka_deck162.jsonl` seq **28**
(`CHOICE: 2 (Create human with Thraben Doomsayer #1, repeated N times, then stop, N=45)`, last
line `N=17`) -> `repeat_count_missing` -> ONE activation, no receipt; seq **29**, **31** (label
echoed, no number) -> once each; `1787833243-ai_baka_deck123-0x56097312ac50-vs-ai_baka_deck126.jsonl`
seq **22** same. Against seq 30 `x15` -> `ran 15 of the 15 times you named`. 4 of 12 takes;
~262 s of latency in one turn; the pilot had computed 17 correctly three times.
**Ask.** `scanRepeatCountInLine` (`AIPlayerGPT.cpp` ~10700) accepts any integer after the short
name (`x17`, `N=17`, `N = 17`, `17 times`, `, 17)`); on no integer, ONE re-ask ("you chose the
repeat row but named no N") or run once with the receipt `ran 1 time (you named no count)`.
The row's example remains the canonical spelling. The deck123 guide states that spelling
positively (A48-1) and must not assert what is NOT read (boundary B1).
**Wave-49 prediction.** `repeat_count_missing` 0/N; every repeat take carries a receipt with
N >= 2 or a narrated "no count named"; the three deck123 token games stay < 20 priority records.

## D4 — HIGH — the automatic mana payment spends the scarce colour on a generic cost
(= seat-123-130 **H2** / deck123 R4 / **R32**)
**Repro.** `1787833239-ai_baka_deck123-0x55ca87a19a00-vs-ai_baka_deck162.jsonl` seq **17** ->
**19**: `Idyllic Tutor {2}{w}` paid from `Underground Sea #1, Underground Sea #2, Scrubland #3`
(the only two blue sources) when `Swamp, Scrubland #1, Scrubland #2` paid it exactly; `{U} 0`
after; `Cast Intruder Alarm {2}{u}` (just tutored) absent for a full turn cycle; the loop
started turn 13 instead of 11; the game was lost by one turn (seq 35 -> gameend at 0).
**Ask.** Pay generic pips from sources whose colour set is a subset of the others' (mono-colour
first, most-flexible last). This is engine correctness in the payment code, the LLM-seat
sibling of the three human-seat mana oracle fixes shipped 2026-08-27 (below). Companion
render (LOW): `{leaves N ... - {w}{b} only}` on the cast row.
**Locus.** the auto-tap payment order (not traced by the seat — inferred from the two `Those
sources` lines; trace before scoping).
**Wave-49 prediction.** 0 casts whose payment taps a colour-C source while a non-C source could
have paid the same pip; report `{U} 0`-after-cast windows with a blue spell in hand.

## D5 — HIGH — the converter PAIR loop clause (R12, carried from wave 47, now a lost game)
(= seat-146-152-162 **E-5** / deck152 G-1 / deck146 G-4 / **R12**)
**Repro.** `1787833233-ai_baka_deck152-0x56029bcff000-vs-ai_baka_deck126.jsonl` seq **25**:
battlefield line carries both `Exquisite Blood` and `Sanguine Bond` with `{effect:}` texts; the
block names Sanguine Bond alone; `ATTACK: A1` (Elite Spellbinder 7/5 flying, unblockable) ->
**20 -> 0 in one combat**, opponent 14 -> 32. Primitives verified (`@lifelostfoeof` /
`@lifeof ... -thatmuch opponent`).
**Ask.** Wave-47 text: when both halves are on one battlefield the block says `both halves of a
life LOOP are on their battlefield (Sanguine Bond + Exquisite Blood): any life they gain
repeats without limit, so ANY nonzero payment on a tag is fatal, not merely expensive`; A-rows
on that board carry `(... and their loop makes any damage you deal fatal)`. Plus one clause of
DIRECTION on the single-converter block (`this is not a trade: their total goes UP by N and
yours goes DOWN by N in the same event`) — deck146 read the fold as "net 0" in 3 of 4 windows
and died on its own attack (`146 vs126` s72).
**Wave-49 prediction.** Loop clause on every both-halves prompt; 0 `ATTACK:` other than `none`
on such a board; 0 "net 0" replies under the direction clause.

## D6 — MED — `[DRAW PRICE:]` misses three row classes, and nothing forecasts the next draw step
(= seat-engine **D-6** / seat-146-152-162 **E-4** / seat-125-126 **MED #4** / deck123 R1, deck130 R1, deck125 G1, deck152 G-3 / **R31**)
**Repros.** (a) `draw:X`: `1787833227-ai_baka_deck125-0x55d9ed7b1350-vs-ai_baka_deck162.jsonl`
seq **38**, **41** — `Cast Sphinx's Revelation ... {X pricing: ... draws you 1 card}` under Fate
Unraveler, untagged (`scriptSelfDrawCount` returns 0 for a non-literal). (b) activated draws:
`1787833242-ai_baka_deck152-0x55a92bd5d200-vs-ai_baka_deck162.jsonl` seq **28**, **29** — `1. Draw
1 with Clue [cost: {2}, Sacrifice]` under `... 2 life`, cracked at 8 life. (c) opposing
cast-trigger: the deck125 file seq **44** (Elixir at 15 life -> drew 7 under Forced Fruition +
Dreams) and seq **51** (Cancel at 11 -> drew 14 -> dead); `1787833213-ai_baka_deck130-0x55ce8937fa00-vs-ai_baka_deck162.jsonl`
seq **23** read "that player" as THEM and cast Rorix at 1 life. (d) forecast:
`1787833239-ai_baka_deck123-0x55ca87a19a00-vs-ai_baka_deck162.jsonl` seq **35** — 9 life, a
9-card draw step at 2 each, K stated nowhere.
**Ask.** (a)/(b) two more sources for the same tag (the `{X pricing:}` clause carries it for
`draw:X`; `Draw N with <token>` is cycling's class); (c) when an opposing permanent's `auto=`
matches `@movedTo(...|opponentstack):draw:N`, every `Cast ...` row gets `[DRAW PRICE: casting
this draws you N (their Forced Fruition), and their Underworld Dreams deals you N]`; (d) `DRAW
FORECAST: your next draw step draws K cards (1 + Howling Mine 1 + Dictate 1 + Teferi's Puzzle
Box: your hand size 7) = K x N life` beside the punisher line. Verb agreement (`punish` ->
`punishes` with one name) rides along.
**Wave-49 prediction.** Tag on every such row (report N); draws/casts at or below the summed
cost 0/N; 0 draw-step deaths where the printed K x N >= life and a life-preserving row was on
the previous menu.

## D7 — MED — the carried plan is immortal when it names no action
(= seat-123-130 **M3** / deck130 R2, deck123 R3 / **R34**)
**Repro.** `1787833215-ai_baka_deck130-0x5589601133c0-vs-ai_baka_deck125.jsonl` seq **84-131**:
`YOUR PLAN (as you last stated it): The game is lost ... Passing is the only legal action with
no impact.` for 48 windows; under it seq **97** Hammer of Bogardan targeted `2 (Yourself)` and
seq **111-112** Starstorm at X=12 with `there is no creature on the battlefield for it to damage`.
**Ask.** Drop the plan block and re-ask for a PLAN when the carried plan names no card and no
option verb from the current menu, or after 5 identical verbatim echoes. R11's principle
(pregame + land drop) generalised to the in-game plan.
**Wave-49 prediction.** No plan echoed verbatim > 5 consecutive windows; `Yourself` damage
targets 0; "the game is lost" PLAN lines 0 (was 48).

## D8 — MED — an index past the menu with an unmatched name goes straight to Baka; the exit row does not say it leads to combat
(= seat-engine **D-3** / seat-146-152-162 **E-6** / **R35**, supersedes R16/R17)
**Repros.** `1787833211-ai_baka_deck146-0x5561d5e2f180-vs-ai_baka_deck125.jsonl` seq **80**
(`CHOICE: 3 (Cast Acererak the Archlich)`, 2-row priority window right after a cast ask);
`deck130-vs-deck126` seq **30** (`CHOICE: 6 (Cast Rorix Bladewing)`, 5 rows);
`1787833239-ai_baka_deck162-0x55ca866fd210-vs-ai_baka_deck123.jsonl` seq **10** (`CHOICE: 3 (Cast
Fate Unraveler)`, 2 rows, 4-drop with 3 mana); `1787833243-ai_baka_deck123-0x56097312ac50-vs-ai_baka_deck126.jsonl`
seq **29** (`CHOICE: 5 (Attack with all creatures)`, 4-row Main-1 cast menu, 257 creatures).
4 of 5 fallbacks.
**Ask.** One re-ask on index > N with no name match (`"<name>" is not on this list; answer with a
number from 1 to N, or 0`); `Cast nothing right now (moves on to combat)` when the attack step
is ahead; the R16 sentence on a priority ask that directly follows a cast ask.
**Wave-49 prediction.** Index-past-menu fallbacks 0/N (was 4/5); rate <= 0.31%.

## D9 — MED — lane P's blockers header double-counts a `[vigilance] [attacking]` body
(= seat-123-130 **M1** / deck130 R3 / **R37**)
**Repro.** `1787833207-ai_baka_deck123-0x5615ef06def0-vs-ai_baka_deck152.jsonl` seq **26**:
`4 are creatures, 2 of them are attacking right now, 3 more able to attack right now` with
Briarbridge Tracker `[vigilance] [attacking]` (untapped), Brutal Cathar `[tapped - attacking]`,
Katilda, Luminarch Aspirant — "more able" should be 2. seq 10 (2 / 0) correct; 26/26 on the
first bucket corpus-wide.
**Ask.** The second bucket excludes `isAttacker()` bodies.
**Locus.** `battlefieldHeaderText` (lane P, 9b19fbfe6).
**Wave-49 prediction.** M counting an `[attacking]` body: 0; N + M <= creature count always.

## D10 — MED — the edict row and the sweeper row need the `{right now:}` fact they turn on
(= seat-123-130 **M2** / deck123 R2, deck126 G3, deck125 G2 / seat-125-126 **MED #5** / **R33**)
**Repros.** `1787833217-ai_baka_deck123-0x5615456bb280-vs-ai_baka_deck130.jsonl` seq **22** ->
**23** (Tribute at `0 are creatures` with Siege-Gang on the stack: resolved, nothing sacrificed,
no life); `...deck123-...-vs-ai_baka_deck152.jsonl` seq **18** (N=4); deck126 `vs130` seq 25 (via
D2). Sweepers: `1787833215-ai_baka_deck125-0x55895edf7530-vs-ai_baka_deck130.jsonl` seq **45**
(`0 permanents ... 0 are creatures`, `Cast Supreme Verdict ... leaves 1` taken), **72**;
`...vs-ai_baka_deck126.jsonl` seq **46** (five walls); `...vs-ai_baka_deck162.jsonl` seq **21**;
Lightmine Field `...vs-ai_baka_deck123.jsonl` seq **40**. Fourth and fifth corpus respectively;
the guide lane is exhausted at both (skill #102).
**Ask.** `Cast Tribute to Hunger ... {right now: they control N creatures - they choose which;
at 0 this does nothing}` (1-branch names the creature and the life); `Cast Supreme Verdict ...
{right now: destroys 0 of their creatures (0 able to attack), 0 of yours}`; Lightmine `{right
now: 0 creatures able to attack - deals 0}`. Same emitter as `{X pricing:}`.
**Wave-49 prediction.** Edict casts at N = 0 corpus-wide 0/N; sweeper casts at a row reading
`destroys 0` 0/N.

## D11 — MED — the cost of an action includes what paying it TAPS, and no row says so
(= seat-146-152-162 **E-2**, **E-3** / deck146 G-2 / deck152 G-2, G-4 / seat-engine (R19) / **R38**)
**Repros.** `1787833211-ai_baka_deck146-0x5561d5e2f180-vs-ai_baka_deck125.jsonl` seq **63-65**
(`becomes beholder with Hive of the Eye Tyrant #2 [cost: {3}{b}]` bare; battlefield `Hive #2
[tapped]`; only `{b}` source = Hive #1; both Hives `[tapped - cannot attack or block this turn]`,
8 mana, no attackers record). `1787833242-ai_baka_deck152-0x55a92bd5d200-vs-ai_baka_deck162.jsonl`
seq **24-25** (`Cast Briarbridge Tracker ... casting this taps you out` when `Those sources` =
`Intrepid Adversary {w}; Brutal Cathar {w}`; 11 lethal power stayed home); same file seq **22**
(`put 1/1 counters with Katilda [cost: {4}{g}{w}, Tap]` in Upkeep, 8 of 9). R19's Blastminer
A-row (`deck130 vs152` seq 12/17/25/38) is the fourth shape.
**Ask.** `{paying this taps: Intrepid Adversary, Brutal Cathar - they cannot attack this turn}`
on any row whose payment must include a creature; `[this land is TAPPED: animated, it still
cannot attack this turn]` on a `becomes` row for a tapped source; `{tapping Katilda now: she
cannot attack this turn}` on a creature `Tap`-cost activation offered before combat;
`[attacking taps it: you lose {2}{R},{T}: ... this turn]` on the A-row. The mana line already
holds every fact.
**Wave-49 prediction.** Creature-tapping casts in a lethal-on-board window 0; `becomes` taken
for a `[tapped]` source 0; report N per clause.

## D12 — MED — the `//` face budget drops the whole back face with no marker
(= seat-engine **D-4** / **R36**)
118 `{card text:}` blocks end on a bare `// <face name>` (`// Agadeem, the Undercrypt` 44,
`// Pelakka Caverns` 30, `// Lord of Lineage` 12, `// Moonrage Brute` 11, pathways 16). Not a
mid-word cut (lane Q's prediction held 0/2,077) but an unmarked truncation.
**Ask.** `// <face> (text omitted)` or `...`.
**Wave-49 prediction.** Bare `// <name>` endings 0/N (was 118).

## D13 — LOW — the option/A-row/B-row lists sort on `name #n`, so `#1`/`#2` straddle another card and defeat identical-text grouping
(= seat-engine D-1 secondary + lane-O watch (2/71) / **R41**)
`1787833230-ai_baka_deck146-...-vs-ai_baka_deck162.jsonl` seq **15**, **20**: `A1. Triumphant
Adventurer #1 ... A3. Triumphant Adventurer #2` split by `A2. Silverquill Silencer`; D1's
`B1. Pride Guardian #1` / `B3. Pride Guardian #2`. Fix together with D1 if gap-grouping is the
route.

## D14 — LOW — the X free-kill marker sits on the row largest-first puts LAST; the option-1 bias took row 1
(= seat-engine **D-5** / seat-123-130 (D48-5) / **R45**)
`1787833220-ai_baka_deck130-0x556f484876d0-vs-ai_baka_deck146.jsonl` seq **47**: `1. X = 6
{kills THEIRS: Silverquill Silencer; YOURS: none}` ... `5. X = 2 {same kills as X=6, for 4 less
mana} [<- most kills ...]`; `CHOICE: 1 (X = 6)`; 4 mana overpaid. 1 of 4 marker windows (the
other 3 had the mark on row 1). Owner ruling largest-first stands; echo the mark on the top row
(`[X=2 kills the same for 4 less mana - see row 5]`).

## D15 — LOW — a menace single-block goes to the heuristic with no re-ask (closes the wave-47 red pin)
(= seat-engine **D-7** / seat-125-126 F9 / **R43**)
`1787833230-ai_baka_deck126-0x5622b00b9650-vs-ai_baka_deck146.jsonl` seq **24**: `BLOCKS: B1:A2,
B2:A3` single-blocking two menace Spiders under the explicit A-line -> `all_assignments_illegal`
-> Baka. Reuse `multiblock_reask` for the menace conflict, then Baka. D19/R27's canonical
substring stands beside it.

## D16 — LOW (measurement) — post-answer overrun is the reasoning channel and is unmeasured
(= deck126 G2 / seat-125-126 **MED #3** / **R40**)
9/174 deck126 replies carry 2+ coded lines with 2.5-4.9 KB between (`post_answer_overrun` 4,648 /
4,889 / 4,779 / 2,563; 235 s, 515 s); deck125 0/406. Ask: a `multi_choice_replies` counter with
the rescued / executed-not-on-menu split (skill #116) BEFORE any prompt-side change. The
bounded pre-CHOICE reasoning block is the candidate only if the count says drift.

## D17 — LOW (record hygiene) — the JSONL `turn` field is the narration's turn minus one
(= seat-engine **D-8** / **R42**)
`deck123-vs-deck126` seq 3: `turn: 1, phase: Main phase 1` under `=== Turn 2 - YOUR turn ===`.
Emit the same 1-based number in both.

## D18 — LOW — carried parity gaps and residuals, re-measured not re-derived
`{leaves N ...}` absent on activated-ability rows (R15; `Life with Elixir of Immortality [cost:
{2}, Tap]` took deck125 to 0 open before an uncontested Underworld Dreams, `vs162` seq 29; 4
activations to 0-2 open) · `Cast nothing right now` bare (R17, folded into D8's `(moves on to
combat)`) · the cycling `[cost:]` bracket (R23, 12/12 correct, untested) · `manaAvailableLine`
number word above ten (R24, **85 emissions**, third wave) · the DFC Pathway PLAN-line tail (R18;
the header shipped, the tail untested) · `dropped_assignments` pads (R22, 0 this corpus) ·
`mana_only_windows_skipped` post-filter fuzz (R21, max 835) · degenerate-decode counter (R20,
0 events; the ~800 s ceiling recurred twice with COMPLETE replies) · `ATTACK:` ranges naming
non-existent labels trimmed silently (R44, `deck130 vs152` seq 24) · `Keep this hand (keeping N
cards)` (R46) · the draw-engine row's `{feeds: ...}` count (R39, filed early, not asked) · Ob
Nixilis `-2` rows lacking the target's text (R47) · the LETHAL-window castability fact (R26) ·
Clue token textless (`Draw 1 with Clue [cost: {2}, Sacrifice]`, 3 windows, read correctly) ·
damage OBJECT bare, `ability$` pay-or-lose punishers undetected, R7 name-substring skip: carried
without new evidence.

## D19 — LOW — one keyword, three rendered wordings (wave-47 D19/R27, carried)
Now with a fallback behind it (D15). Ask unchanged: one canonical substring per keyword plus the
surface clause. Report distinct-form counts for `menace`, `defender`, `flying`, `lifelink`.

## D20 — MED (frontend / human seat) — frontend token overload: Arena-style grouping + kill the O(n^2) RenderSpell scan (owner item, added 2026-08-27, carried by number)
No hard cap on battlefield size, but `GuiPlay::Render`'s `RenderSpell` does a linear scan of all
CardViews per card per frame (O(n^2)) and `HorzStack` spreads >= 16 cards across
(SCREEN_WIDTH-50)/total px — a 1,539-creature loop board (w47) would be a smeared pile at
seconds-per-frame, unusable on PSP/Vita, CardSelector unnavigable. Owner's pointer: Arena-style
grouping of identical permanents (same name/tapped/counters/sickness/attachments) into one
CardView with an xN badge, expand-on-click — the same equivalence key lane O already uses for
the LLM's battlefield rows. Measure the frame-time knee with a windowed Doomsayer+Alarm probe
before designing. Kill the O(n^2) scan regardless. (Lane N makes the 1,539 board rarer — max
this corpus 257 — not impossible: the repeat row's ceiling is 200 per take.)

---

# SHIPPED OUT-OF-WAVE — owner live play on the Vita, 2026-08-27 — recorded as DONE, not docketed

Three human-seat ORACLE fixes (the human player's castable/usable border, not the LLM render):

| commit | what was wrong | fixed |
|---|---|---|
| **19e2b04f7** | the human cast gate counted only a dual land's FIRST colour, so a spell payable from the dual's second colour showed uncastable | dual lands contribute every colour they produce to the gate |
| (same session, after 19e2b04f7) | a Delve alternative cost was priced as `{0}`, drawing a FALSE castable border on Gurmag Angler | Delve priced from the real generic cost minus exilable graveyard cards |
| (same session, after 19e2b04f7) | an ability-tapping source counted its OWN mana toward its activation cost, drawing a FALSE usable border on Westvale Abbey | a `{T}`-cost ability excludes its own source from the payable pool |

These are human-seat correctness in the same payment family as D4 (the LLM seat's auto-tap
colour choice); D4 is the open sibling. No LLM-seat rate is claimed to move.

---

# DISCHARGED — SET B: wave-48 engine lanes, by the emitter's actual string

| lane / commit | prediction | verdict | counts |
|---|---|---|---|
| **E1 5ce8f8a5a** | retraction-to-Baka on replies whose last coded `CHOICE:` is a real re-answer: 0/N | **PASS**, with two holes -> **D2** | `retracted_choice` 0/1,729; `answer_replaced` 21, chosen index == last coded line 21/21; the loop exited at `deck123-vs-deck126` seq 25 by exactly this shape. Hole (a) name/index mismatch executed (1); hole (b) `ATTACK:` lines take the FIRST line (1) |
| **N 7a858904d** F1 repeat row | offered when counters >= 2 this turn, self-tap only; does qwen take it? | **PASS; TAKEN 12/14** | `x50` x5 + `x15` executed exactly (receipt `ran N of the N times you named`), 4 taken with no parseable count -> **D3**; ceiling 200 never reached; pass row chosen on 2/14 repeat windows |
| **N** F3 `0. Pass priority (take no action this window)` | real last row on single-option priority windows | **PASS 139/139** (+14 repeat windows) | chosen 0 in 75, 1 in 64 — a decline that was 0/1,871 is now 54% |
| **N** F2 tag names both scopes | `activated this turn N times already ... taken it N times in a row with no other action in between` | **PASS** | max 203 at `deck123-vs-deck126` seq 25 = 2 singles + 50 + 50 + 1 + 50 + 50 (reconcilable with the receipts); `deck123-vs-deck162` max 20 = 1+1+1+1+15+1 |
| **N** (D1 headline) | no game reaches 500 priority windows in one turn; three deck123 token games < 60 priority records; 21/21 natural | **PASS x3, by 5x** | max **9** windows in a turn; **17 / 12 / 8** (was 1,550 / 1,871 / 223); 42/42 gameends; longest seat 133 records; priority mean 23,546 -> 11,331 chars, count 4,128 -> 364 |
| **N** (D13 tag scope) | tag maximum reconcilable with the seat's activation count | **PASS** | above; the `in a row` count crossing a turn boundary with only passes between (seq 14: `this turn 2` vs `in a row 3`) is plausibly intended — not raised |
| **O 96a07e864** A-row collapse + range grammar | no prompt > 60 KB; blockers/attackers p90 <= 60 s | **FAIL by 1.2 KB on one prompt -> D1; PASS on latency** | max 61,240 (B-rows) / next 27,987; attackers max 24,153 (was 233,662); `ATTACK: A1, A2, A3-A257` and `ATTACK: A1, A2-A4, A5-A7` both accepted; `kAttackerRangeNote` 9 renders; numbered-option ranges 50 prompts, 2 in-range choices accepted; blockers p90 56.1 s, attackers p90 50.6 s (max 347 s = a 4,118-char reply on an 8.9 KB prompt) |
| **O** narration cycle collapse (D11) | `- ... (xN)` | **PASS** | `created 50 1/1 Human tokens`; `Your Goblin dealt 1 damage to Fate Unraveler (x3)` 11 prompts; no 180-deep pair remains to collapse |
| **O** battlefield grouping by identical text (D12) | no > 5 uncollapsed identical rows | **PASS** | `Human #1-#255 (1/1) ... x255`, `Perimeter Captain #1-#3 ... x3`; watch: same-name `#n` straddle on option lists 2/71 -> **D13** |
| **P 9b19fbfe6** attacking header (D3) | header below the `[tapped - attacking]` row count 0/N | **PASS 26/26**, one new wrong number -> **D9** | `257 of them are attacking right now` over 257 tapped-attacking rows; vigilance double-count at `deck123-vs-deck152` seq 26 |
| **P** priced representative (D4) | pricing by the representative alone while a larger price sits on the line 0/N | **PASS (thin)** | 2 collapsed tags; `most expensive to attack into: Pride Guardian (0/3)` rendered once (`deck146-vs-deck126` seq 72), reply named it and sent anyway (guide) |
| **P** stack/target keyword tags (D7) | counters on a stack object of printed power 0 / "Defender" text 0 | **PASS 0/33** | `[defender]` on 5 stack rows at deck125; `[vigilance]`, `[flying]`, `[lifelink]` elsewhere; deck125's negative-existence sentence retired (B3 closed) |
| **Q 7246c4a95** `{card text:}` budget (D5) | mid-word cuts 0/N | **PASS 0/2,077; unmarked back-face drop 118 -> D12** | 13 `...` tails, all after a full stop |
| **Q** `auto=choice` header (D6) | `A choice is required` 0; `target controller` 0 | **PASS 0 / 0** | `Choose an option for Barkchannel Pathway:` 57; `Exquisite Blood [enchantment] [your reveal]`; `{right now: if you choose "the opponent": life -6, draws ...}` |
| **Q** `[DRAW PRICE:]` rows (D8) | tag on every controller-draw row under a punisher | **PARTIAL -> D6** | 11 tagged prompts (cycling, Wall of Omens, Dream Fracture); 0 draws at a tagged row; `draw:X`, Clue and opposing cast-trigger rows untagged; verb agreement nit |
| **Q** ANNOUNCE_X (D9) | no plan block; stale-plan X restated while a larger free-kill row is on screen 0 | **PASS 0/28 plan blocks; 0/4 stale X** | marker on 4; 3/4 chose the marked row; 1/4 took row 1 with the mark on row 5 -> **D14** |
| **mana-only gate** (carried) | 0 leaks | **PASS** | max 835 skips (`deck126-vs-deck123`, the 203-token turn), 103, 0 x40; 0 all-mana menus leaked |
| **fair hands** (carried) | | **PASS** | 15 mulligans on 8 seats; kept lands 1-5; two chains (146 vs152 to 3 cards, 125 vs126 to 4) — reviewer-owned |

# DISCHARGED — SET A: wave-47 guide edits + boundary pass, by deck (reviewer verdicts, summarised)

| deck | verdict summary | routed |
|---|---|---|
| **123** | A47-1 loop stop **FAIL** (tag 203; 4 takes past L+C+3) — harmless at lane-O widths, rewritten for the repeat row (A48-1); B4 `M >= 50` ceiling FAIL and its reason is gone (deleted; skill #103); A47-2 RULE 5 **FAIL 4th corpus** -> D10, no prose; A47-3 costs-in-combat PASS 0/8; RULE -1 sunset PASS 2nd corpus; B5 cast-list entry 8 **violated 12x** in one game (A48-5); RULE 3 Damnation declined 2/2 at 0-vs-4 (A48-3, skill #108); blockers "no blockers" under a `B1.` row 2/4 (A48-4, skill #106) | D3, D4, D6, D10 |
| **125** | W30 X PASS 8/8; W31 sweeper-at-0 **FAIL 4/8, fifth wave** -> D10; W32 Lightmine FAIL 1/44; W33 tap-out **FAIL 10** (Staff 2/6; skill #109, #115 -> W10); W34 Path UNTESTED / textless-token FAIL 2/7; W35 Emrakul PASS 2/2 (skill #97 held); W36 counters PASS 0/27 and the tag SHIPPED (W11 retires B3); W37 land 84/84; mulligan chain to 4 cards -> W13 odds rewrite (skill #119); Forced Fruition uncovered -> W8 + D6 | D6, D10 |
| **126** | D29 Tribute at `1 is a creature` **FAIL 0/2** (skill #107 -> F5); D30 zero stop held 40/40 in the model's text, 1 cast via the E1 hole -> D2; D31 menace one-each FAIL 1/8 -> F9 + D15; D33 Lantern rule **reversed on evidence** (F6, skill #109); D34 unpaid `Cast nothing` PASS 4/21; D35 gate PASS; win-button rule "only if their side is empty" wrong (F7, skill #114) | D2, D15 |
| **130** | D47-1/2 X kill list PASS 4/4, STEP 2 FAIL 1/4 (D48-5, skill #110 + D14); D47-3 punishers PASS 0/38 one-sided; Blastminer gate FAIL 1/1; B6 re-key FAIL 1 (triple chump at "at 10", D48-2); planeswalker-first FAIL 1/1 (D48-1); Starstorm absolute obeyed where obeying lost (D48-3, skill #112); collapsed-vs-enumerated tag FAIL (D48-4, skill #111); Talisman declined 2 (D48-6); 48-window dead plan (D48-7, skill #115 + D7) | D7, D9, D14 |
| **146** | P1 converter **FAIL 7/7 rows, third corpus** -> 146-A (#102 + #106); P2 `Cast nothing` split; P3 Upkeep animations PASS 0/58 (was 10); P4 repeat rows PASS 0/2; cast-list first-match 60/75 (was 39/67); accountability example parroted (146-E, skill #105); Command "always castable" false (146-D, skill #108); tapped Hive / other-Hive payment (146-C, skill #113 + D11); STEP 1 mulligan floor **FAIL 1/1** on "no white source", chain to 3 cards (146-F, skill #119); ATTACK line first-line executed -> D2 | D2, D5, D11 |
| **152** | deploy floor 35/35 (4th corpus); Fateful Absence 14/14; land 131/131; converter posture correct 2/3 then the loop PAIR **20 -> 0** (152-A/B, skill #114 + D5); lethal spent in Upkeep/Main 1 with creature mana sources (152-C, skill #112 + D11); Clue cracked under punishers (152-D + D6); counter spreading 2/13 third corpus (carried, #45/#64); no DECIDING list — added (skill #24 DoD) | D5, D6, D11 |
| **162** | Rule 1 17/19; 162-A/B1 tiebreak PASS 1/1; GANG BLOCK first live hold 1/1; send rung 4/4; land 93/93; Rule 2 6/15 breaks, all wins, third corpus -> 162-A (#102 via skill #118); B2 Master-of-the-Feast sentence cited while broken (skill #107 -> count key); Ob Nixilis -2 on a token beside the maker (162-B, #17); one-land-six collision (#97) -> 162-C by look (skill #119); 0 blockers records in six games (skill #117) | D6 (R39 filed early) |
| **pool** | wave-47 boundary B1 PASS 1/1 · B2 read-and-argued-around (re-keyed) · B3 retired by lane P · B4 deleted (reason gone) · B5 roll-call rejected then entry 8 violated 12x · B6 re-keys: deck130 FAIL 1, deck123 no window · B7 examples fixed | |

---

# CARRIED — open, re-measured, not re-derived
See **D18** and **D19**. Additionally: the wave-47 "does qwen take the repeat row" OPEN question
is CLOSED (12/14); the Baka menace gang-block red pin is CLOSED into **D15** with a record; the
floating-mana disappearance (wave 46) was again not reproduced; the ~800 s latency ceiling
recurred twice with complete replies (801.9 s `152 vs123` s6; 590.7 s `123 vs126` s15) — not a
truncation, not investigated (whether it is a client timeout remains unknown).

---

# FALLBACK CLASSES — 5 / 1,729 = 0.289% (in band)

| n | class | records | disposition |
|---|---|---|---|
| 4 | **index past the menu naming a card NOT on it** | `deck146-vs-deck125` seq 80 (Acererak, 2-row priority after a cast ask); `deck130-vs-deck126` seq 30 (Rorix, 5 rows); `deck162-vs-deck123` seq 10 (Fate Unraveler, 2 rows, unaffordable); `deck123-vs-deck126` seq 29 (`Attack with all creatures`, 4-row Main-1 cast menu) | parser right to refuse; one re-ask + range tail -> **D8**; counted as `named_row_not_offered` under **D2** |
| 1 | **menace single-block** `all_assignments_illegal` | `deck126-vs-deck146` seq 24 | one re-ask -> **D15** |
| 0 | retraction with a mid-line replacement (was 2) | | **E1 shipped and paying** |
| 0 | decode degeneration (was 3) | | R20 stays LOW |

---

# PROMPT CHARS PER DECISION KIND — wave 47 -> wave 48 (ALL games both sides; wave 47 includes its loop games)

| kind | n47 | mean47 | max47 | n48 | mean48 | max48 |
|---|---|---|---|---|---|---|
| ask | 1,342 | 10,496 | 164,397 | 1,240 | **8,162** | 27,987 |
| priority | 4,128 | 23,546 | 116,148 | 364 | **11,331** | 27,721 |
| attackers | 109 | 12,866 | 233,662 | 71 | **9,588** | 24,153 |
| blockers | 38 | 12,380 | 74,063 | 26 | 12,733 | **61,240** |
| reveal | 19 | 13,623 | 23,365 | 20 | 11,405 | 15,555 |
| bottom | 8 | 2,196 | 2,403 | 8 | 2,107 | 2,370 |

Priority mean halved and its count fell 11x; every max fell below 30 KB except blockers — **D1**.
Latency: corpus p50 ~20 s; attackers p90 50.6 s; blockers p90 56.1 s; the tail is now reply
length, not prompt size.

---

# LATEGAME SPECIMEN — invariant 0 (for the owner's per-wave prompt review)

**Primary (one combat, both seats):** `1787833243-ai_baka_deck123-0x56097312ac50-vs-ai_baka_deck126.jsonl`
seq **31** (attackers, 11,274 chars for 257 attackers, `A3-A257. Human #1-#255 (1/1) ... x255`,
reply `ATTACK: A1, A2, A3-A257` in 3 s — lane O at its widest) paired with
`1787833243-ai_baka_deck126-0x560971e0cf50-vs-ai_baka_deck123.jsonl` seq **15** (the 61,240-char
blockers screen for the same combat — **D1**). Both new collapses and the one that is missing.
**Decision-quality specimen:** `1787833242-ai_baka_deck152-0x55a92bd5d200-vs-ai_baka_deck162.jsonl`
seq **22-24** — lethal on board, mana sources are creatures, the row says `taps you out` (**D11**).
**Natural late grind:** `1787833211-ai_baka_deck146-0x5561d5e2f180-vs-ai_baka_deck125.jsonl` seq
**93** (turn 34, 13 sources, Command / Acererak / Agadeem's X=10 / Soul Shatter, an E1
replacement in the reply).
**Draw-forecast specimen:** `1787833239-ai_baka_deck123-0x55ca87a19a00-vs-ai_baka_deck162.jsonl`
seq **35** (9 life, five draw engines on their line with `{effect:}` text, a 9-card draw step
coming and K stated nowhere — **D6**(d)).
