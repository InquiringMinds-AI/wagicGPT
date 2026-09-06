# Wave-64 deck 162 ("Draw and Die!") — per-deck review

Corpus `matchups-20260906-001533`, binary `dab6a6cba`. Seat files
`1788671*-ai_baka_deck162-0x*-vs-ai_baka_deck<N>.jsonl` (cited below as `vs<N> seq K`).
93 model calls across 6 games, **0 fallbacks, 0 wall misses, 0 re-asks**
(`identical_ask_answers_reserved` 0/0/0/0/0, `wall_miss_events` 0 in every `gameend`);
median latency 30.4 s, median prompt 7.3 KB. Record **3-3**.

## 1. Game by game

| game | result | turn | deciding decisions |
|---|---|---|---|
| vs123 | **WIN** 14 / -4 | 15 | Howling Mine t3 (seq 6), walls down early (seq 5,7); three clean blocker windows (seq 10/14/18) held life at 17 while two Mines + Dictate + 2 Caress raced 123 from 20 to -4. Nothing went wrong. |
| vs125 | **WIN** 20 / 0 | 19 | Mulligan to 6 (seq 2-4, bottomed Teferi's Puzzle Box), then Mine t6 → Fog Bank → Dictate → Underworld Dreams → Ob Nixilis ×2 + Shield Spheres. 125 never attacked profitably. Nothing went wrong. |
| vs130 | **WIN** 20 / 0 | 12 | Two Fate Unravelers (seq 9, 11) under a Howling Mine; attacked correctly into an empty board (seq 12/16 — the render said "They have 0 untapped creatures able to block"). Nothing went wrong. |
| vs152 | **LOSS** -4 / 35 | 11 | **Variance.** Kept a 6-land + Ob Nixilis 7 (seq 2); drew 8 more lands in 9 turns. Underworld Dreams did not land until t8, Ob Nixilis t10. No creature was ever castable, so no blocker window ever arose. Do not read this as a guide effect. One real item at seq 10 (MED-3 below). |
| vs126 | **LOSS** -3 / 41 | 13 | **The deciding decision is seq 13** (t12, 21 life). 126 controlled both halves of the Sanguine Bond + Exquisite Blood life loop. The seat cast Fate Unraveler off a row that priced the body at `life 21 -> 17`; 126 answered with Tribute to Hunger (gain 4 = its toughness) and the loop killed the seat from 21 in one turn. See HIGH-1. Contributing: seq 7/8 (see MED-2). |
| vs146 | **LOSS** -2 / 13 | 17 | **The deciding decision is seq 12** (t16, 9 life). Two Master of the Feast (5/5 flier) `[castable now]` in hand, 0 creatures on board; the seat cast Dictate of Kruphix instead and died to 11 crack-back damage. The header said "up to 7 ... you would be at 2 or lower" while naming two Hives of the Eye Tyrant at 3 power each in the same sentence. See HIGH-2. |

Both losses that were not variance were **PERCEPTION**: in each, the row/verdict the seat obeyed
priced the choice as survivable, and the true price was its life. Its stated plan at vs126 seq 13
even reads "Watch out for Sanguine Bond/Exquisite Blood loop" — it knew, and the row's arithmetic
overrode it. That is the trust doctrine working exactly as designed and the surface owing truth.

## 2. Engine / interface / card items

### HIGH-1 — `{their converter:}` prices a body as survivable inside a live life LOOP, contradicting the LOOP SCOPE paragraph in the same prompt
`vs126 seq 13` (t12, my life 21). The prompt's LOOP SCOPE paragraph is correct and explicit:

> Both halves of a life LOOP are on THEIR battlefield (Sanguine Bond + Exquisite Blood) ... Any life
> they gain, or any life you lose, chains until YOU are at 0 - **so ANY nonzero payment on a tag above
> is fatal, not merely expensive.**

Six lines later the cast row it governs says the opposite:

> `1. Cast Fate Unraveler {3}{b} (3/4) ... {their converter: this body has toughness 4 and they control 1
> life-to-damage converter (Sanguine Bond) - any effect of theirs that gains them life equal to its
> toughness (a sacrifice-for-toughness edict, a drain of that size) takes 4 off YOU: life 21 -> 17}`

Row 2 (Master of the Feast) carries the same tag reading `life 21 -> 16`. Both are false: with
Exquisite Blood also out the payment does not stop at 4 or 5. The tag detects only Sanguine Bond
("they control 1 life-to-damage converter") — the loop detector that wrote the paragraph is not
consulted by the per-row arithmetic. The seat took row 1 and was dead on the opponent's next
Tribute to Hunger (`mtg.txt:124075`, `toughnesslifegain` — verified against the primitive and it
matches the printed Oracle text).

Fix shape: when both loop halves are on the same battlefield, the row's `-> N` must be replaced by
the fatal verdict the paragraph already computed, not a subtraction. A true number in the wrong
scope is a lie (trust doctrine (2)). Row 3 (a second Underworld Dreams — no body, no exposure,
same clock) was the safe line and carried no tag distinguishing it.

### HIGH-2 — the crack-back lethality verdict, and therefore the F11 `{crack-back cover:}` gate, is computed from the FLOOR while the same sentence names the rungs that break it
`vs146 seq 12` (t16, my life 9, hand holds two `Master of the Feast {1}{b}{b} (5/5) [creature] [castable now]`, battlefield holds 0 creatures):

> `CRACK-BACK NEXT TURN: 5 of their creatures will be able to attack (tapped ones untap first), for up
> to 7 from combat as their board stands - you would be at 2 or lower - and that number is a FLOOR, not
> a ceiling: noncreature permanents of theirs that can animate and attack are not in that count - Hive
> of the Eye Tyrant (best rung their mana pays for: 3 power once animated, for {3}{b} ...), Hive of the
> Eye Tyrant (best rung ...)`

7 + 3 + 3 = 13 against 9 life. The line names the two rungs, prices them, and then prints a
NOT-lethal verdict ("at 2 or lower") built from 7 alone. Lane AH gates `crackBackBlockerRowTag`
"on the LETHAL case only", so the two castable 5/5 bodies got **no** `{crack-back cover:}` clause —
the tag rendered 2 times in the whole 21-game corpus and never in this window, which is the exact
window it was written for. The seat cast a symmetric draw enchantment instead of a blocker and took
11 (one Hive animated for 3 + the dungeon's dark pool 1). Rung power verified: `borderline.txt:53394`
`becomes(Creature Beholder^3/3^black^menace)` — the *3 power* figure is correct; only the verdict
and the gate are wrong.

Fix shape: the lethality test that gates both the verdict clause and the cover tag should run on
floor + the rungs the same sentence already priced (they are already computed — they are being
printed), or the verdict must stop asserting a survivable resting life when it has named unpriced
power above the gap.

### MED-1 — `{feeds:}` never says a symmetric engine also draws YOU
Corpus-wide the clause has exactly two forms: `the opponent draws 1 extra card per turn` (41) and
`the opponent draws an amount that is not fixed (read the card)` (12). Dictate of Kruphix
(`mtg.txt:29401-29402`: `@each my draw:draw:1 controller` **and** `@each opponent draw:draw:1 opponent`),
Howling Mine and Teferi's Puzzle Box are all symmetric, and for this deck the seat's own extra draw
is the reason to run them (it digs to the punishers it is waiting for). Repro: `vs126 seq 7`, the
Dictate row. Silent omission, and the omitted half is the deck's own upside — say `each player draws
1 extra card per turn (you and them)` when the script has a controller-side draw line.

### MED-2 — `draw converters in your hand: N` counts cards that cannot be paid for, and it is the release condition the guide keys on
8 of 8 "engine-first" rows the seat took under `draw converters ... on your battlefield: 0` had
**zero** of the named hand converters castable that turn. Every one of them is contradicted by the
hand line ~40 lines above in the same prompt:

`vs126 seq 7` — row: `draw converters in your hand: 3 - Fate Unraveler, Underworld Dreams, Fate Unraveler`;
hand: `Fate Unraveler ... [cannot pay now: needs 4 mana, you have 3 untapped sources]`,
`Underworld Dreams {b}{b}{b} ... [cannot pay now: needs {b}{b}{b}, your 3 untapped sources cannot pay it]`.
Same shape at vs123 seq 6/9/15, vs125 seq 8/11, vs130 seq 5, vs126 seq 8.

This is not automatically a misplay — engine-first went 3-1 across these games — but at vs126 it
handed 126 ten extra cards across turns 7-12 while the seat's own punisher was four turns of black
mana away, and 126 found Sanguine Bond (t9) and Exquisite Blood (t11) inside that window. The
payability fact exists at render time (it is printed on the hand line); adding
`draw converters in your hand: 3, castable this turn: 0` makes the release condition mean what the
guide intends it to mean, at the render rather than in prose.

### MED-3 — Ob Nixilis's -2 row label omits *whose* draw it is; the model read it as its own
`vs152 seq 10` (t10, 3 life). Row: `-2: destroy target creature and draw two cards with Ob Nixilis,
the Hate-Twisted targeting Brutal Cathar`. The label comes from the card script, not the renderer:
`bin/Res/sets/primitives/planeswalkers.txt:2687`
`auto={C(0/0,-2,Loyalty)}:name(-2: Destroy target creature and draw two cards) destroy target(creature) && draw:2 targetcontroller`.
The mechanism (`targetcontroller`) is right and the adjacent `{card text:}` says "Its controller draws
two cards" (matches the printed Oracle text, `text=` line 2688), but the name the row leads with does
not, and the reply says: *"This also draws two cards for us, potentially finding more threats or
answers."* 1 occurrence in 21 games — the choice was correct anyway and the seat was dead regardless
— but the label is wrong for the general case and the fix is a data edit, no recompile:
`name(-2: Destroy target creature, its controller draws two)`.

### LOW-1 — Shield Sphere renders without a `[creature]` tag while being counted as one
`vs123 seq 10`: `Your battlefield (7 permanents listed, of which 2 are creatures ...): ... Shield Sphere
{0} (0/6) [artifact] [defender]`. It has P/T and it is in the creature count and in the blocker list
(B1), but the type bracket says artifact only, unlike `Fog Bank {1}{u} (0/2) [flying, defender]` and
unlike hand rows, which do carry `[creature]`. Cosmetic here; a class to check where artifact
creatures gate a decision.

### Observation — F8/F9 (last wave's deck-162 HIGH-1) look repaired on this deck's evidence
All three blocker windows (`vs123 seq 10/14/18`) carried the truthful form:
`chosen for your blockers' material as well as for the life ... every blocker in it survives. This
assignment is an OPTION, not an instruction: declining every block leaves you at 12 and costs you no
permanent`, on `NOT lethal` headers, with the ceiling stated ("your creatures can legally block at
most 2 of those 4 attackers"). The seat copied the suggestion 3/3 — and 3/3 it was right (free wall
blocks, nothing dies). No self-contradicting header appeared in any of the six games.

## 3. Guide verdict: **KEEP**

`bin/Res/ai/baka/deck162_strategy.txt` (60.7 KB, in band) is kept as installed. Reasons:

- The seat executed the guide's central teach — the "draw converters ... on your battlefield: K"
  brake and its release — **correctly and literally in all 8 windows where it applied**, and did so
  in three wins. The guide was not the loser in any game.
- The two non-variance losses were decided by rows that priced a choice as survivable when it was
  fatal (HIGH-1) and by a verdict computed from a floor it had itself declared incomplete (HIGH-2).
  Guide prose written against either of those surfaces loses to the surface, and the trust doctrine
  forbids teaching the pilot to doubt a render. Both fixes belong at the render; both are cheap.
- The one guide-shaped gap I can defend — the release condition treating an unpayable card in hand
  as "one turn away" (MED-2) — is a *render* fact the row already has and does not print. Fixing it
  in prose would mean teaching the pilot to re-derive payability that the engine computes exactly;
  fixing it in the row makes the existing guide rule correct with no edit at all.
- 0 fallbacks, 0 re-asks, 0 wall misses, 3-3 against the pool. Nothing in the six games shows the
  pilot deciding against the guide.

No `strategy.txt`, no `general-proposals.md`, no `skill-proposals.md`: nothing in this deck's
evidence pays for a general-guide or skill amendment that the existing corpus of amendments 1-331
does not already carry.

## 4. What I did NOT check

- The five opponents' seats beyond the narration and the single final prompt I read for 126v162 and
  146v162 — I did not trace opponent decision quality.
- Lane AG/AH/AI/AJ/AK predictions as such: adjudication is the engine seat's job. Where a lane's
  observable happened to fall inside my six games I reported the observation (F8/F9 above, F11's gate
  in HIGH-2), and nothing here should be read as a lane verdict.
- No window in these six games exercised: F1 (no X spell was ever cast — Peer into the Abyss was
  never castable), F3 (deck 162 runs no pathways), F4/planeswalker attack targets (the seat had no
  attackers in the one game where an opposing walker was on board — vs152 seq 10, 0 creatures),
  F7 gang blocks (never more than 2 blockers, never gang-assigned), F13/F14 (engine-seat scope).
  Last wave's deck-162 MED about `N untapped creatures able to block` being unfiltered by evasion is
  **UNTESTED here**: both attackers windows printed N = 0.
- I did not run wagic, build anything, or touch `bin/Res`, `src/`, or git. Card facts above were read
  from the primitives (`planeswalkers.txt:2684-2688`, `mtg.txt:29399-29404`, `71934-71938`,
  `39182-39185`, `124073-124077`, `borderline.txt:53391-53395`); I did not query Scryfall, and I flag
  that: the primitive `text=` lines I quote agree with the row annotations, but I have not
  independently confirmed them against Scryfall for the four cards where the two would have to
  disagree for my reading to change (Ob Nixilis, Tribute to Hunger, Dictate of Kruphix, Hive of the
  Eye Tyrant).
