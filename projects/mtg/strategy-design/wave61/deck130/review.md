# wave61 / deck130 (Budde's Ponza, mono-red land destruction) — per-deck review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-092408/`, 6 games, my seat =
`*-ai_baka_deck130-0x...-vs-ai_baka_deck<N>.jsonl`. Record 2-4. 0 crashes, 0 hangs,
1 fallback (`wall_miss_unrecorded`, recovered), 7 `async_drops`, 335 decisions in my seat.
Guide read: `projects/mtg/bin/Res/ai/baka/deck130_strategy.txt` (70,927 B).
All card facts below checked against `projects/mtg/bin/Res/sets/primitives/mtg.txt`.

## 1. Game by game, and the decisions that decided them

**vs deck123 — WIN, turn 20, 17 to -1.** Textbook. Land kill early (s11-13 Lay Waste at Tundra),
Siege-Gang at 5 lands (s16), then the Hammer of Bogardan recursion loop run to completion:
returns at s55/59/64/70 and casts at s57/61/66/72, four straight turns of 3 to the face, closing on
the lethal row at s73 (`{right now: takes 3 damage - they would be at -1; THIS WINS THE GAME}`).
Rule #2 step 0 and the EMPTY-HAND rule both did exactly what they were written for.
Correct forced-sacrifice pick at s36 (Devour Flesh; it took the tapped Goblin).

**vs deck146 — WIN, turn 18, 19 to -3.** Land kill at s10/17/24, Blastminer, Siege-Gang at s21,
alpha strikes at s26/30/36, Rorix for the kill. `ATTACK TOTAL` was exact 3/3 (s26 "at 10" -> 10;
s30 "at 6" -> 6; s36 "at -3; that KILLS them" -> -3).

**vs deck152 — LOSS, turn 11, 0 to 17. DECIDED at s18.** Turn 10, my life 9 (after the Talisman),
board empty, `CRACK-BACK NEXT TURN: ... up to 10 ... you would be at -1 or lower; that would KILL
you`. Hammer of Bogardan is on the stack and the target menu prices every row: Katilda (1/1) DIES,
Luminarch Aspirant (1/1) DIES, **Wolf (2/2) DIES**, Briarbridge Tracker SURVIVES (toughness 5).
It chose the Aspirant. Killing the Wolf removes 2 from the crack-back (10 -> 8) and survives at 1;
killing the Aspirant removes 1 (10 -> 9) and dies at 0. Nothing on the screen connects a `DIES`
row to the crack-back total, and the model's stated reason was "remove the biggest threat".
Contributing but NOT decisive: s16, at life 10 with that same crack-back printed, it cast a second
Talisman of Impulse (ramp) ahead of the removal; the 1 life it later charged took 0 to -1, but the
seat was on exactly 0 either way. Classified: s18 STRATEGY + interface (see HIGH-2); s16 STRATEGY.

**vs deck162 — LOSS, turn 14, 0 to 20.** Not a guide failure. Opponent assembled Underworld Dreams
+ 2x Howling Mine + Dictate of Kruphix: `DRAW FORECAST: your next draw step draws 4 cards ... = 4
life to the punishers` (s55). The #0 RULE cannot fire against FORCED draws. The seat played this
well — s19-21 Starstorm at X=4 killed Fate Unraveler and left 4 marked damage on Master of the
Feast (5/5, primitive mtg.txt:71934-71942), which s23-25 Spark Spray then finished; the
`{right now: takes 1 damage - DIES}` verdict on the 5/5 is CORRECT (marked damage), not a false
verdict. It died from 4 life to draw triggers with 10 cards in hand and no answer in the deck.

**vs deck126 — LOSS, turn 17, 0 to 28.** Two threads. (a) s20-22, the X=0 null cast — see HIGH-1;
one Starstorm burned for nothing. (b) The end was Sanguine Bond + Exquisite Blood on their side.
The converter/loop narration at s36 is excellent and the model read it correctly ("any life gain or
loss triggers a chain that will kill me ... my only win condition is Rorix dealing lethal"). It
raced to 8 and lost. Rorix died to Tribute to Hunger on their turn 17 and the loop finished it.
No answer existed in this deck; not a guide item.

**vs deck125 — LOSS, turn 35, 0 to 41. Best single misplay of the corpus: s67.** Turn 22 upkeep,
my life 13, **opponent life 10, zero creatures on their board**, 8 mana untapped, hand = Starstorm
(blank, 0 creatures), Stone Rain (dead, 9 of their lands), Mountain. Row 1 was
`Put a card into hand with Hammer of Bogardan [cost: {2}{r}{r}{r}]` — 5 to return + 3 to recast = 8,
exactly what was available, 3 to the face a turn against 10 with only 2 Staff of Nin pings coming
back. Four turns of that is the game. It answered `CHOICE: 0 (pass)` with the plan
"cycle Starstorm for card advantage" — the exact phrase the guide's #1 rule lists as a forbidden
thought. Opponent then went 10 -> 17 -> 21 -> 32 -> 41 on Sphinx's Revelation and the game was over.
It DID take the return at s77 and s86, one and two turns too late. STRATEGY, against a rule the
guide already carries.

## 2. Engine / interface / card items

**HIGH-1 — the X=0 cast is a forced dead-end: an offered row that can only destroy the card, with
no way back.** `130-...-vs-ai_baka_deck126.jsonl` s21/s22 (turn 10).
s21 cast row: `1. Cast Starstorm {r}{r}{x} {X pricing: your mana affords only X=0 right now, which
deals 0 damage and kills nothing}`. Taking it leads to s22, whose ENTIRE menu is:
`1. X = 0 {X pricing: X=0 - this cast does NOTHING: it deals 0 damage and the spell is spent}` and
the header `this ask has no pass row`. There is no decline, no "cast nothing", nothing. Once the
cast row is taken the card is unavoidably spent for zero effect. Census over deck130's 6 games: the
`affords only X=0` cast row was offered in **7 windows** and taken in **1** — one card lost to a
menu that could not be backed out of. Two independent fixes, either sufficient: (a) suppress the
cast row entirely when max affordable X = 0 (an X-spell whose only legal X does nothing is not a
play), or (b) give the X menu the decline row the "Choose an option for" menu already has
(`Decline - do nothing`). Note the guide already spends ~20 lines telling this deck not to do this
(`NEVER announce X=0`) and the model did it anyway with the annotation on screen — the render-side
fix is the reliable one. This is not in `known-bugs.md` and no lane R/S/T/U/V item covers it.

**HIGH-2 — target rows price `DIES` but never price it against the CRACK-BACK total printed six
lines above.** `130-...-vs-ai_baka_deck152.jsonl` s18. Same screen carries
`CRACK-BACK NEXT TURN: ... for up to 10 from combat ... you would be at -1 or lower; that would KILL
you` and a target list where three rows read `DIES` for creatures of power 1, 1 and 2. The seat has
to do the subtraction itself, and did not. Each removal row already knows its target's power; the
row should carry it: `{removes 2 from the CRACK-BACK total above: 10 -> 8}`, and mark the row(s)
that take the total below your life. Adjacent to known-bugs #3 (crack-back census accuracy) but a
different defect: the number here was RIGHT, it was just not connected to the menu that could
change it. Same shape applies to Pyrite Spellbomb, Spark Spray and Siege-Gang's sacrifice.

**MED-1 — a 900-second wall miss carrying a decisive `transport` field is still classed
`wall_miss_unrecorded`.** `130-...-vs-ai_baka_deck126.jsonl` s12:
`"transport": "curl=28,http=0,empty=1,connect_ms=20000,phase=wall"`, `"deadline_pct": 100.0`,
`"latency_ms": 900024`, `"fallback": "wall_miss_unrecorded"`. `connect_ms=20000` with `http=0` says
the connection itself never completed — that is a transport failure with evidence, not an unknown
wall miss. This is known-bugs #13 recurring in the new `transport` channel; `deadline_pct` IS back
(emitted on every decision in all 6 games), so half of #13 is fixed. Recovered at s13, no game
impact, but it cost 15 minutes of the run.

**MED-2 — a HOLD taken in main phase 2 was never re-opened by the opponent taking me from 20 to 0.**
`130-...-vs-ai_baka_deck126.jsonl` s36 (turn 16, hold) -> s37 `gameend` (turn 17, my_life 0),
`hold_windows_skipped: 30`. Here the hold was correct on the merits (8 mana, hand of Lay Waste +
Talisman, both halves of the Sanguine Bond loop on their side, no answer), so this is a
report-not-repro: I could not distinguish "hold suppressed the window" from "no window arose".
Flagging it for the engine seat because lane U C14 (HOLD across the turn boundary) predicts exactly
this seam and my seat has one instance of a lethal turn with zero windows after a hold.

**LOW-1 — cast rows for a second copy of a legendary/nonlegendary permanent read well.** Positive:
`[second copy: you already control Talisman of Impulse; both stay on the battlefield - no legend
rule]` (152 s16, 162 s15, 146 s13) and the Rorix legend bracket both rendered correctly; the seat
never mis-cast a second legend this corpus. No action; recorded so it is not re-litigated.

### Wave-61 lane items my seat can speak to
- **Lane U C10a (sweeper CAST row ranked) — improved.** Wave 60: Starstorm offered 51x, cast 0x.
  This corpus: offered **64x, cast 4x** (126 s25 X=4 kills Perimeter Captain + Wall of Omens;
  162 s19 X=4 kills Fate Unraveler; 125 s73 correctly took `cycling` when the row said no creature
  on the board; 126 s21 the X=0 mistake). 3 of 4 casts were correct. The marker is being read.
- **Lane T C11 (`{spare: ...}` conditioned) — holds.** **0** `{spare:` renders in all 6 of deck130's
  games, including the `130v123` s9 window lane-T names. Wave-60 deck130 HIGH-1 does not recur.
- **Lane R C1 (ATTACK TOTAL resulting-life honesty) — holds in my seat.** 3 of 3 exact (146 s26/30/36),
  including the `that KILLS them whatever they block` row.
- **Lane R C3 (CRACK-BACK floor) — floor held.** 152 s16/s18 predicted "up to 10 ... would KILL you"
  against a board of 6+2+1+1; the seat died. No under-count observed in my 6 games.

## 3. Guide verdict: KEEP

`deck130_strategy.txt` stays as it is. Reasons, and they are success reasons, not a shrug:

1. **The wins were the guide being followed.** vs123 is the EMPTY-HAND / Hammer-loop section
   executed exactly as written, four returns in a row to a `THIS WINS THE GAME` row; vs146 is the
   land-count cast order (`3 lands: STONE RAIN`, `5 lands: SIEGE-GANG COMMANDER`, `6+: RORIX`)
   run top to bottom. Neither needed a rule that is not already there.
2. **Every loss traces to something the guide already says, or to something no guide could fix.**
   s67 (deck125) is the guide's own named forbidden thought — "card advantage" — chosen over a rule
   already written two paragraphs above it. Adding a third statement of a rule the model read and
   overrode is the busywork pattern, not a fix. vs162 (forced draws under Underworld Dreams +
   Howling Mine x2 + Dictate of Kruphix) and vs126 (Sanguine Bond + Exquisite Blood with no
   enchantment removal in a 60-card mono-red deck) are unwinnable board states, not guide gaps.
3. **The one genuinely new failure is an interface dead-end, not a strategy gap.** 126 s21/s22
   (X=0) already has ~20 lines of guide text against it, at the top of the file, in capitals, with
   the exact annotation string quoted. It fired anyway. That is evidence the fix belongs in the
   render (HIGH-1), and spending more of a 70,927-byte guide on it would buy nothing.
4. **Byte budget.** The guide is at 70,927 of the 71,000-byte pool ceiling. Any edit here has to be
   byte-neutral; none of the candidates above is worth displacing text that is demonstrably working.

Closest call I rejected: extending the EMPTY-HAND RULE's hand test from "(0 cards)" / "only a land
spell the two numbers have already killed" to "only cards the guide's own gates have already
declared dead" (125 s67's hand was Starstorm-with-no-creatures + a dead Stone Rain + a Mountain,
which is dead-equivalent but not literally empty). I rejected it because the model's stated reason
at s67 was not a reading of the empty-hand test at all — it was "cycle for card advantage", which
the guide forbids by name; a wider gate would not have changed the sentence that produced the pass.

## 4. Proposals
None. No general-guide proposal and no strategy-writing-skill proposal: everything above is either
an engine/render item (sections 2) or a rule the guide already carries.

## What I did NOT check
- The other five decks' seats except where cited for context (126, 152, 123 opponent logs skimmed
  only around the deciding turns; 125/146/162 opponent logs not read).
- I did not adjudicate lane R/S/T/U/V predictions systematically — only the four my seat touched.
  UNTESTED in my seat and not reported on: lane S C5/C6/C12, lane T C7/C8/C9, lane U C13/C14
  (beyond MED-2), all of lane V except by absence.
- I did not verify the opponents' card facts against primitives except Master of the Feast,
  Starstorm and Dwarven Blastminer; I did not consult Scryfall (no primitive looked wrong).
- I did not compute a byte-level diff of the live guide against wave-60's; I took the 70,927-byte
  file on disk as the live edition.
- No engine source was read, no build or wagic run, no git.
