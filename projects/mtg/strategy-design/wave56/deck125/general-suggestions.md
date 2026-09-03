# deck125 -> core-prompt / render proposals (wave 56)

Layer-routed. Engine BUGS go to `wave56/seats/seat-125-126.md`'s ranked list, never here; these are
prompt-text and render-content proposals for the core loop. Every string below is quoted from a
rendered `prompt` in `matchups-20260903-120952`.

## G1 (HIGH) - narration is 63% of every prompt, and the ask kind predicts how much of it can matter

Measured over my 1,288 board-bearing prompts: mean prompt **12,618 chars**, of which **7,971
(63.2%) is the GAME LOG above `--- CURRENT SITUATION ---`**. The corpus's largest prompt is mine
(26,298, `125v130` seq 113) and `wave56/lategame-specimen-logcost.txt` splits the specimen as
**narration 20,953 | situation 2,910 | choices + protocol 436**.

The specimen's decision is a three-row LAND DROP between two lands, at 32 life against an opponent
on 2 with an empty hand and no creature, off 14 untapped sources - all three answers win. **86% of
the prompt is spent on the part that cannot change it, at 28.6 s per decision.** 133 of my 1,314
decisions (10.1%) are land-drop asks, and their narration share is the same 61%.

Ask: make the narration budget a function of the ask kind and of what the ask can consume.
- `Land drop:` asks whose menu is only `Play <land> ... / Play no land right now`: last **2 turns**
  of log. Nothing older can distinguish two lands.
- `discard` (cleanup): last 2 turns. The ask is answered from the hand line and the two battlefield
  headers, both of which are in CURRENT SITUATION.
- `ask`/`priority` with an EMPTY `ON THE STACK`: last 3 turns.
- `ask`/`priority` with a non-empty stack, `blockers`, `attackers`, `reveal`: unchanged.
This is not a cap on legal choices - the menu is untouched. It is a cap on a section whose own
content the ask provably cannot use.

## G2 (HIGH) - price the row, don't describe the card: the Emrakul case

`125v126` seq 254 renders `Cast Emrakul, the Aeons Torn {15} (15/15) {leaves 3 of your 18 untapped
mana sources untapped} {card text: "..."}`. On the SAME menu, row 1 carries `{removes: ...}` and an
enumerated legal-target list, and row 4 carries `{X pricing: max affordable X=15 (18 mana total)}`.
The prompt separately carries `LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs - Sanguine
Bond #1, Sanguine Bond #2` **forty lines above the menu**, and the opponent's line lists four
Tribute-capable bodies.

Proposal: a creature cast row rendered while a converter of THEIRS is on the battlefield should
carry its own price, in the shape the engine already uses twice on that menu:
`{their converter: if this is sacrificed or destroyed they gain up to <toughness> and you lose that
much - you would be at K}`. **The general principle, and it is the one thing that separates every
deck125 rule that went to zero from every one that keeps breaking: a rule enforced by a number ON
THE ROW holds (low-X Revelation 0/86, sweeper-at-0 0/124, Lightmine 0/91); a rule enforced by a
paragraph elsewhere in the prompt does not, however loudly the guide repeats it.**

## G3 (MED) - the ping menu prices the KILL but not the RACE

`125v162` seq 87 renders `1. Deal 1 damage with Staff of Nin #1 targeting Ob Nixilis, the
Hate-Twisted ... {right now: takes 1 damage - SURVIVES (loyalty 5, 4 left)}` and
`4. ... targeting the opponent {right now: takes 1 damage - they would be at 12}`. Both verdicts are
correct and complete for ONE ping. What the decision needs is the comparison over the game, and the
prompt already computes both halves of it elsewhere: `DRAW FORECAST: your next draw step draws 3
cards (1 + Dictate of Kruphix 1 + Howling Mine 1) = 3 x 1 = 3 life to the punishers above`, and
the source count of the pings themselves.

Proposal: when a ping target is named on the `DRAW PUNISHERS` line, append the payback to its row:
`{4 more pings removes it; at 3 life per draw step that is 12 life saved}`. This is the same device
as `[DRAW PRICE: ... - you would be at K]` pointed at a repeated cost instead of a one-shot one.
The seat took the face row 8 times of 8 and lost the game 16 -> 0 while taking them 13 -> 6.

## G4 (MED) - the "ceiling" phrasing in the guide is a prompt-side risk too

Not a render change, an observation the core loop should carry into any generated guidance:
`Casting decision (Main phase 1, YOUR turn): which card do you cast now, if any?` invites a
per-window judgement, and 210 of deck125's 302 opponent-turn windows had no card to cast at any
phase. The `{leaves N sources - no other row on this menu needs more than N}` clause that lane B/D
added is a good example of the opposite: it tells the pilot the menu is finished. Extending that
clause to the whole menu - `{no row on this menu changes a number on the board}` when every cast
row is dead by the engine's own verdicts - would let a pilot decline without needing a guide to
authorise it. deck126's six dead-row spends in one game are the same defect from its side.

## G5 (LOW) - two harness notes for reviewers, not code changes

1. `{spare: you control N lands already}`, `(copies k-m of n in your hand)` and the HOLD row's
   `{taking this row skips ...}` tail render ONLY inside the prompt; the translog `options_text`
   array holds bare names. Counting lane literals from `options` reports shipped features as
   missing (this is the brief's own warning; it still catches people).
2. HOLD takes must be PREFIX-matched. 23 of my 138 takes lack the row's tail in `chosen_text`;
   exact-matching undercounts by 17% and would have read deck125's take rate as 16.8% rather than
   18.5% - which is the difference between FAILING and MEETING lane A's target.
