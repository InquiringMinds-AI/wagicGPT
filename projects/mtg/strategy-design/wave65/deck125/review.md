# Wave-65 per-deck review — deck125 "Revelation Fracture" (Modern UW Control)

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-033121/`
Seat files: `*-ai_baka_deck125-*-vs-*.jsonl` (6 games). Record **4-2**.
Zero fallbacks, zero re-asks, zero transport errors, zero wall misses across all 6 seats.
Counts below are from the rendered `prompt`, prefix-matched, never the `options` array.

## 1. Game by game

| Opp | Result | Turn | Final life | Decisions | Deciding line |
|---|---|---|---|---|---|
| 130 | **WIN** | 46 | 38 / -12 | 110 | Sweeper + Staff grind; wraths held while their board mattered. |
| 123 | **WIN** | 32 | 45 / -14 | 77 | Counter-and-Revelation; 46 hold windows collapsed a long stall. |
| 152 | **LOSS** | 17 | -2 / 20 | 41 | Keep was guide-legal (3 lands, coverage named Elixir/Revelation/Path). Wrath cast from exile at seq 32 answered one wave; no second answer arrived. Aggro tempo, not guide effect. |
| 146 | **WIN** | 39 | 52 / 0 | 140 | Staff-of-Nin clock; seq 136-138 double-Staff ping finished it. |
| 126 | **WIN** | 36 | 20 / -14 | 145 | Won *despite* pitching a live Supreme Verdict twice (seq 55, 126) — Lightmine Field carried it. |
| 162 | **LOSS** | 19 | 0 / 18 | 42 | Underworld Dreams x2 + Howling Mine + Teferi's Puzzle Box. Died to a compulsory draw step forecast at 10 cards x 2 life. Guide's DRAW PUNISHERS lines were followed; the deck has no answer to a creatureless prison. |

Both losses are matchup/variance. Mulligan keeps in both were exactly the guide's rule
(2-5 lands + a coverage line naming a card). Card facts verified against
`bin/Res/sets/primitives/mtg.txt` (Supreme Verdict :115943, Lightmine Field :67058,
Staff of Nin :112425) — all render text matches `text=`.

## 2. Engine / interface / card items

### HIGH-1 — X-pricing's "leaves you alive" verdict ignores damage already ON THE STACK
`1788683518-ai_baka_deck125-0x56503f007390-vs-ai_baka_deck162.jsonl`, **seq 36** (turn 19, life 5).
The SAME prompt carries both of these:

```
ON THE STACK: 13 damage to you - you would be at -8; that would KILL you
1. Cast Sphinx's Revelation ... [<- best X for this cast: X=5 ... but NET -5 life for this cast
   puts you at 0; this KILLS you. X=4 is the largest listed X whose NET (-4) leaves you alive, at 1]
```
X=4 does not leave you alive: 13 unresolved damage sits under it. The X pricer nets against
`my_life` and never subtracts the stack total the header two screens up already computed.
Recurs seq 36,37,38,39 (5 of the 8 windows in this seat that printed a stack-damage header).
This is a false verdict of the class the trust doctrine forbids — the model is instructed to
believe the surface, and here the surface offers a survival line that does not exist.
**Fix shape:** the "leaves you alive" arithmetic must start from `life - stackDamageToMe`,
the same number the ON THE STACK header prints; the two verdicts are computed twice and disagree.

### HIGH-2 — the cleanup-discard menu prints `{spare:}` and then does not order by it
`1788683510-...deck125-...-vs-ai_baka_deck126.jsonl`, **seq 55** (turn 19, life 20/15).
Row 1 is the live sweeper, three explicitly-spare lands are rows 5, 6, 8:
```
1. Supreme Verdict {1}{u}{w}{w} ... {right now: destroys 7 of their creatures ...}
5. Seachrome Coast (land) ... {spare: you control 10 lands already; the most expensive card in
   your hand you could still reach costs 6}
--REPLY-- PUT: 1
```
Census over the seat's 31 discard asks: **12 asks offered at least one `{spare:}` row; only 6 of
them discarded exclusively spare rows.** The engine has already computed which card is free and
prints it, then lists it *below* the card it is trying to protect. The cast menu's convention
(most impactful first, declines last) is inherited here, but the discard seam INVERTS value
polarity — at a discard, the row the model should reach for first is the *least* valuable one.
Repeats at seq 117 (turn 31: four redundant Path to Exile on the list, model pitched
Supreme Verdict + Sphinx's Revelation) and seq 126.
**Fix shape:** sort the cleanup list `{dead right now:}` / `{spare:}` rows first, live
`{right now:}` rows last — the same ordering the deck guide already teaches in prose and the
model already ignores half the time. This is a render fix, not a guide fix (see §3).

### MED-1 — the hold key is row-identity only; life change does not re-open it
`1788683496-...deck125-...-vs-ai_baka_deck152.jsonl`, **seq 30** (turn 11, life 22).
The prompt's own bracket states the key:
```
[hold check: every row above was also on the menu at the last window at this seam (4 windows in
 a row now) - a hold taken here holds until one of them changes]
INCOMING THIS COMBAT: not declared yet - 4 of their creatures can attack, for up to 14 - you would be at 8
```
The model held with 5 untapped sources and two instant-speed Sphinx's Revelations in hand.
Next logged window is seq 31 at **life 8** — the 14 damage landed with every combat window
suppressed, because the option rows (Revelation / Cast nothing / Hold) were textually unchanged.
A row's `{X pricing: ...}` text is life-independent, so a hold taken at 22 survives a drop to 8.
**Fix shape:** fold `my_life` (or the INCOMING/stack damage figure) into the hold key, or expire a
hold at the combat-damage step; the model gave up a real instant-speed window it was told it kept
("you give up no cast").

### MED-2 — two decline rows compete and the model takes the expensive one 7:1
Across the 6 seats, **299 windows offered both `Cast nothing right now` and the `Hold priority`
row. The model chose the plain decline 213 times and the hold 29 times**; not once did it choose
"Cast nothing" on a window where no hold row existed. 43% of this seat's 501 ask/priority windows
are plain declines. The hold row is a strict improvement on its face ("any change re-opens this
window; you give up no cast"), so the pool is paying ~200 round trips per deck for a row that
collapses them. Contributing causes visible in the render: the hold row is ~3x longer than
"Cast nothing right now", sits *after* it, and is hedged with a caveat paragraph; MED-1 above is a
real reason to distrust it. Repro anywhere — e.g. `...-vs-ai_baka_deck162` seq 35-40, six
consecutive plain declines in one Draw step as life fell 10 -> 1.

### LOW-1 — multi-answer at a repeatable seam: first-wins is safe here (AO lane evidence, PASS)
`...-vs-ai_baka_deck146.jsonl` **seq 136**, `parse_note: multi_answer_first_taken`,
`coded_answers: 2`. Opponent at 2 life; the model wrote:
```
CHOICE: 1 (Deal 1 damage with Staff of Nin #1 targeting the opponent)
CHOICE: 3 (Deal 1 damage with Staff of Nin #2 targeting the opponent)
```
First-wins took Staff #1 (opp -> 1); seq 138 the very next priority window fired Staff #2 and won
the game. No `later_answer_ignored`, no loss. Recording it as evidence the AO rule costs nothing
at a seam that re-asks — not as a defect. It does show the model reaching for a *two-action*
answer the protocol has no syntax for; if a batched "fire both" ever becomes cheap, this is the
seam that wants it.

### LOW-2 — stale PLAN carried verbatim past its own premises
`...-vs-ai_baka_deck152.jsonl` seq 30: the carried plan reads "Keep the counter in my hand
payable; Staff of Nin only from a row whose leaves N covers that counter's cost; ping their face
every turn ...; Activate Elixir when library is low" while the hand holds no counter, no Staff is
on either battlefield line, and the Elixir was already shuffled away at seq 11. The protocol says
to restate only when "part of yours is now done or false"; the model re-emits it unchanged. Low
harm (declines dominated that game) but it is ~60 wasted tokens per window and it is a stale
surface the model then reasons from.

## 3. Guide verdict — **KEEP as is**

`bin/Res/ai/baka/deck125_strategy.txt`, 70,901 bytes (top of the 41-71 KB band; ~99 bytes of
headroom). No edit; no `strategy.txt` written.

Why KEEP is the success verdict here:

- **The guide already says the right thing at every place the model went wrong.** The two
  misplays worth naming this corpus (pitching a live sweeper at seq 55 / 117 / 126) are covered
  three separate times in the existing CLEANUP DISCARD section — step 2's "EVERY LAND ON THE
  NUMBERED LIST ... whenever your own battlefield line reads 'and N are lands' with N of 6 or
  more" (the seq-55 board read 10 lands), the tier note, and the explicit
  "SEND IN THAT ORDER: every '{dead right now:}' row and every '{spare:}' row first ... While a
  dead or spare row is still on the numbered list, a live one is never part of your answer."
  The model read the surface correctly and decided against a rule already written in the live
  guide. Restating a rule the pilot is already ignoring is the busywork pattern the loop flagged
  after wave 58. The lever is the render (HIGH-2), not more prose.
- **The sections that ran did their job.** The DRAW PUNISHERS / cleanup-price teaches were
  exercised end-to-end in the 162 game (the seat correctly declined every Revelation under two
  Underworld Dreams and re-aimed the Staff at the punisher, seq 28/34); the mulligan rule
  produced two guide-legal keeps in the two losses; 4 wins, three of them 30+ turn grinds, on a
  deck with one creature.
- **13 of the 20 sweeper discards this corpus happened with the opponent battlefield line reading
  0 creatures** — those were correct, not the bug. Overstating them as a guide failure would be
  reading variance as guide effect. The genuine failures are the four in the 126 game (opponent
  at 7 and 10 creatures), and that game was still won.
- **The band leaves no room.** At 70,901 of 71,000 bytes any addition requires a deletion, and
  nothing in the current text is dead weight I can pay for with a seq.

No `general-proposals.md` and no `skill-proposals.md`: my evidence routes to the render layer
(HIGH-1, HIGH-2, MED-1, MED-2), which is the core loop's, and none of it generalizes into a
strategy-writing rule that amendments 1-331 do not already carry.

## 4. What I did NOT check

- The opponent seats' own decision quality (read only for board context and life reconstruction).
- Whether MED-1's suppressed windows contained a *better* line than the Revelation X=2 I named —
  the held windows are not logged, so the claim is "a window it was told it kept was suppressed",
  not "it lost a specific play".
- `hold_windows_skipped` per-window detail (only the gameend totals: 9/46/36/11/168/0).
- Scryfall cross-check: I verified against the primitives only; no disagreement was suspected,
  so no `api.scryfall.com` call was made.
- Anything under `bin/Res` or `src/` was read-only; no build, no git, no wagic run.
