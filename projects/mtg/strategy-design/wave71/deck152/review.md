# Wave-71 deck 152 review — BANT HUMAN MIDRANGE

Corpus `matchups-20260907-085638`, seat files `...ai_baka_deck152-<ptr>-vs-...` (6 games, all natural ends).
**216 decisions, 0 fallbacks, 0 `reply_truncated`, 0 `action_before_plan`, 0 records with `reasoning_chars` 0**
(min 1,328 / p50 5,533 / p95 10,479 / max 18,700). `plan_line_missing` 12 (5.6%), `off_protocol_bytes>0` 10
(4.6%). Latency p50 102.4 s / p95 219.4 s (this seat runs above the corpus p50, as in wave 70).
Live guide read = the wave-70 edit (19,980 B). Seat record **3-3**.

## 1 — Game by game

| # | Opp | Result | Turn | Life | Deciding decisions |
|---|-----|--------|------|------|--------------------|
| 1 | 130 | **win** | 9 | 20 / -1 | seq 13/18/22, clean curve then the "KILLS them whatever they block" alpha |
| 2 | 162 | **win** | 13 | 5 / -6 | seq 34 — 8-attacker lethal at **1 life** on the turn the draw forecast printed "-7; that KILLS you" |
| 3 | 146 | **win** | 16 | 19 / 0 | seq 36 block kills Nadaar, seq 42 alpha |
| 4 | 123 | loss | 14 | -16 / 7 | opponent assembled Thraben Doomsayer + Intruder Alarm → 28 tokens in one upkeep; **not a guide loss** |
| 5 | 126 | **loss** | 16 | 0 / 41 | **seq 35 — see HIGH-1**; the Sanguine Bond + Exquisite Blood loop closed two turns later |
| 6 | 125 | **loss** | 34 | 0 / 7 | **seq 49 — see MED-1**; UW control, 4x Supreme Verdict + 2x Staff of Nin |

**Game 4 is not a guide loss.** At seq 19 (turn 11) the seat was at 22 vs 13 with 11 power on board against 29
untapped 1/1s and a printed `CRACK-BACK NEXT TURN: ... 30 - you would be at -8; that would KILL you`. The
`reasoning` walks every line, finds none that survives, and takes the one that keeps two blockers. Its lethal
block at seq 23 (5 blockers into a 24-damage lethal swing, ending at 7) is exactly the guide's lethal-attack
rule. Nothing in the guide reaches an opponent combo that makes 28 bodies in one trigger.

**Game 6 is mostly matchup, with one paid-for misplay.** The seat had Sigarda, Luminarch Aspirant x2, Elite
Spellbinder x2, Katilda, Tovolar's Huntmaster and Teferi all answered by Supreme Verdict / Path to Exile /
Essence Scatter / Cancel / Fall of the Gavel. It still got the opponent to 2 and held them there from turn 23.
The one decision that cost the win is MED-1.

## 2 — Engine / interface / card items

### HIGH-1 — an opponent permanent's `{effect: "..."}` is printed in the CARD's voice, and the model read it from the wrong seat; it cost the game
`1788789417-ai_baka_deck152-0x560a15373f80-vs-ai_baka_deck126.jsonl` seq 35 (turn 13, 30 vs 8).

The opponent battlefield line renders:
```
Exquisite Blood {4}{b} [enchantment] {effect: "Whenever an opponent loses life, you gain that much life."}
```
"you" is the card's controller and "an opponent" is the reading seat, but nothing in the line says so. The
`reasoning` inverts it and then plans off the inversion:

> "But wait, Exquisite Blood says "Whenever an opponent loses life, you gain that much life." So if I deal
> damage to them, they gain life from Exquisite Blood!" ... "If I deal 15 damage, opponent loses 15. Exquisite
> Blood triggers 15 times, opponent gains 15. Net life change: 0. So they stay at 8." ... "Wait, if I attack
> with Elite Spellbinder only, they can't block it. They lose 4, gain 4 from Exquisite Blood. Net 0."

Believing its own damage was refunded, the seat concluded that no attack made progress and sent everything
(`ATTACK: A1, A2, A3, A4`) against a rendered `so blocking can leave them as high as 12`. They blocked; the
opponent went **8 → 12** instead of 8 → 4. Two turns later (seq 40, opponent at 12) the seat's two unblockable
fliers dealt exactly 8 — lethal against 4, not against 12 — and the loop closed on the following combat.

Verification: the primitive is right (`bin/Res/sets/primitives/mtg.txt:37966-37968`,
`auto=@lifelostfoeof(player):life:thatmuch controller`, text matching Scryfall exactly) and so is the engine —
at seq 40 the seat dealt 8 and the opponent went 12 → 4 with **no** Exquisite Blood gain, and the seat stayed
at 38. **The render is the only wrong surface.** Classification: PERCEPTION. The same prompt's `LOOP HALF PENDING`
paragraph (Sanguine Bond in exile, "nothing chains while it stays there") was read and dismissed — "This is
just flavor/context" — because it explains the pair, not the direction of the surviving half.

Fix: normalise the perspective of every `{effect: ...}` printed on a line that is not the reader's own, e.g.
`{effect, from your seat: whenever YOU lose life, THEY gain that much}`. The prompt already does exactly this
for the converter block ("A converter of THEIRS turns every life THEY gain ... into that much life off YOUR
total"), so the machinery exists — it is applied to the summary paragraph and not to the battlefield row that
the model actually quoted. Every opponent card with "you"/"your"/"an opponent" in its text is exposed:
Underworld Dreams, Howling Mine, Intruder Alarm and Staff of Nin all appear in this seat's six games with the
same unnormalised voice.

### MED-1 — the Lair of the Hydra upkeep window is offered first and taken every time, and its own bracket says it should not be
`...vs-ai_baka_deck125.jsonl` seq 40 (T21), seq 42 (T23), seq 49 (T25). Every rung carries:
```
[Upkeep offer: this animation lasts only until end of turn, and the same row is offered again in your
 main phase - declining here counts toward this turn's two declines]
```
The seat took the upkeep offer **3 times out of 3** despite the live guide's "PASS that row on sight when it
prints `[Upkeep offer: ...]`". At **seq 49** (9 vs **2** life, holding Sigarda and 11 mana) it animated a 2/2
in upkeep; the opponent answered with Path to Exile in that same upkeep and the seat lost both the land and
its only clock, then never dealt another point in nine turns. The `reasoning` never mentions the bracket — it
reads the window as simply the earliest chance to set up the attack:

> "Wait, can I animate Lair of the Hydra in Upkeep? Yes, the options are there."

Classification: STRATEGY on a true surface (the bracket is correct and complete), fixed by the guide edit
below. The engine-side half worth considering: the bracket tells the seat the row will return in main phase,
so it is describing a window with no upside — offering the animation only at the main-phase seam would remove
a decision that is never right, and would also stop it consuming two of the turn's declines.

### MED-2 — the reveal window still does not name the destination zone (wave-70 MED-2, unfixed)
3 windows this wave (`...vs-deck123` seq 20; `...vs-deck162` seq 26, seq 35). The instruction is still
`Choose ONE card that goes to "get a human"; every other card goes to "put on bottom"` — nothing says the
chosen card goes to your HAND (verified `bin/Res/sets/primitives/borderline.txt` Sigarda `moveto(hand)`;
Oracle agrees, no card bug). The guide now compensates, and it worked: all three picks are the cheapest
castable Human on the list and none is a dead legendary (see §3). Naming the zone in the option label would
let the guide give those bytes back.

### MED-3 — the carried-plan header label collides with the required reply label
`...vs-ai_baka_deck162.jsonl` seq 36. The prompt carries `YOUR PLAN (as you last stated it, 1 window ago on
turn 13) reads "..."` and asks for a line beginning `PLAN:`. The reply was:
```
YOUR PLAN: Pass to let combat damage resolve and win the game.
CHOICE: 6 (Hold priority)
```
counted as `plan_line_missing` with 62 off-protocol bytes. This is a sharper instance of the wave-70 MED-4 /
L11 question than "the label was just dropped": here the model used a label the prompt itself supplies four
lines above the instruction. Renaming the carry header (e.g. `PLAN CARRIED FORWARD (from turn 13):`) removes
the collision at no cost to the seat.

### LOW-1 — this seat's whole off-protocol charge is again one shape
10 records with `off_protocol_bytes>0`, every one an unlabelled first line (`Cast Tovolar's Huntmaster, attack
with all creatures.\nCHOICE: 1 (...)`, `Attack with Sigarda.\nPUT: 5`), none prose after the action line, none
reversing the action, none exceeding one line. 5 of the 12 `plan_line_missing` records are at `ask` seams whose
instruction line ends `Write your PLAN: line first, then on a line of its own CHOICE: ...` — the action label
is in caps, the plan label is not. Unchanged from wave 70 in shape, down from 5.1% to 4.6%.

### LOW-2 — a declined land drop is re-offered in the same main phase (observation, not a defect)
`...vs-ai_baka_deck125.jsonl` seq 51 answers `Play no land right now`; seq 54/55 offer and take
`Play Barkchannel Pathway` in the same turn 25 main phase, after Sigarda was countered. `declined_face_latches`
is 0 for this seat and no land drop appeared in a main 2 after a main-1 decline, so lane BS's latch looks
correct here — recorded because the decline itself is a guide violation (edit 3 below), not an engine one.

### Carried items this seat reproduces without new derivation
- **L11 / wave-70 MED-4** — LOW-1 above.
- **wave-70 MED-1** (reveal rows carry no legendary/duplicate annotation) — still true (`...vs-deck162` seq 35
  prints `Brutal Cathar (copy 1 of 2 in this list)` / `(copy 2 of 2)` with no "already in hand/on battlefield"
  note), but it cost nothing this wave: the guide's compensating sentence held.
- **wave-70 MED-3** (reveal window inherits the combat plan) — 1 of 3 windows (`deck162` seq 26 answered
  `Attack with Sigarda.\nPUT: 5`), down from 6 of 10.

## 3 — Did wave 70's edits pay off?

- **Edit 1, the `ATTACK TOTAL:` rule — 2 PASS / 2 FAIL of 4 windows carrying "blocking can leave them as high
  as N".** PASS: `deck126` seq 16 (as high as 19 → `ATTACK: A2`, the only unblockable line) and seq 23 (as high
  as 15 → `ATTACK: A3`). FAIL: seq 21 (as high as 20 → all three; survived on the opponent declining to block)
  and seq 35 (as high as 12 → all four; **lost the game**, HIGH-1). Both bullet-1 windows ("that KILLS them
  whatever they block") were honoured: `deck162` seq 34 and `deck130` seq 22, both immediate wins. The rule
  works where it is read; the two failures share one shape, addressed by edit 1 below.
- **Edit 2, blocking-trigger timing — PASS, untested by adversity.** No `reasoning` in the 6 games re-derives
  trigger timing against the render; the wave-70 loss mode did not recur.
- **Edit 3, the Sigarda Coven reveal — PASS, and it paid.** 3 of 3 picks correct: `deck162` seq 26 took
  Luminarch Aspirant with Katilda and Sigarda both eligible on the same list (the exact wave-70 failure), seq
  35 took Elite Spellbinder over two Brutal Cathars, `deck123` seq 20 took Brutal Cathar. The `reasoning` at
  seq 35 says "take Elite Spellbinder **to hand** for next turn's cast" — the guide's zone sentence is being
  used, which is why MED-2 cost nothing.

## 4 — Guide verdict: **EDIT**

Revised guide at `wave71/deck152/strategy.txt`, **19,992 B** (live 19,980 B, ceiling 20,000). Amendment-332
grep clean (no wave/seq/corpus/game citation, no counts, no history).

**Edit 1 — the `ATTACK TOTAL:` rule now names the lure that beat it.** Paid for by `deck126` seq 35 (game) and
seq 21.
before: `... bringing one more attacker turns a kill into their survival.`
after: adds `"That IS lethal - but only if none of them is blocked" is the OPENING of that same clause and is
not a kill claim: it prices an attack they get to refuse, so read past it. Adding a blockable body to an
unblockable-only attack never adds damage they cannot prevent.`
Why this shape: at seq 35 the model quoted the whole ATTACK TOTAL line and then wrote "This is a strong hint
that attacking with all is the play, hoping they don't block". The existing rule says to read the LAST clause;
it did not say that the FIRST clause is a priced offer rather than a verdict. Both failures are that one gap.

**Edit 2 — Lair of the Hydra leads with the pass and carries its why.** Paid for by `deck125` seq 49 (game),
seq 40, seq 42.
before: `- LAIR OF THE HYDRA taps for {G}, and {1}{G} makes it a 1/1 Hydra until end of turn - PASS that row on
sight when it prints "[Upkeep offer: ...]" ...; otherwise animate in MAIN 1 only ...`
after: `- LAIR OF THE HYDRA taps for {G}, and {X}{G} makes it an X/X Hydra until end of turn. ANY row printing
"[Upkeep offer: ...]", "[this land is TAPPED: ...]" or "[repeat: activated this turn ...]" is a PASS, with no
exception for a turn you mean to win: the row says the animation lasts only until end of turn and returns in
your main phase, so an early take buys nothing and exposes your land to removal a phase before it could
attack. Animate in MAIN 1 only, with mana left after your creature, and only when its A-line would read "[no
creature they control can block this attacker]".`
Why this shape: the rule already existed and lost 3 times out of 3. It was a subordinate clause after the card
description; it now leads, it names the exact turn the seat overrode it on ("a turn you mean to win"), and it
carries the consequence, which the row's own bracket supplies. Also corrects `{1}{G}` / `1/1` to `{X}{G}` /
`X/X` — the menu offers rungs 1 through 9 and the seat used the 9/9 rung twice.

**Edit 3 — the land drop is never declined for having enough mana.** Paid for by `deck125` seq 51.
before: `... so never "Play no land right now" and never hold a land for a colour you want later;`
after: `... so never "Play no land right now" - not even on a turn your untapped sources already cover every
card you mean to cast - and never hold a land for a colour you want later;`
Why: the `reasoning` accepted the rule's premise and still declined — "I have 7 mana available ... Is there
any reason to play the land? ... I'll play no land and cast Sigarda." The stated reason ("costs no mana") did
not cover the case where mana is already sufficient, so the rule read as inapplicable.

**Edit 4 — the both-halves converter bullet was factually WRONG and is corrected.** Paid for by `deck126`
seq 40 + seq 41.
before: `* BOTH "Sanguine Bond" and "Exquisite Blood" -> a LOOP without limit: one point of damage you deal, or
one wall gaining them 3, ends the game. ATTACK: none every combat, FLIERS INCLUDED ... Block every "[lifelink]"
attacker you are offered a block for, and cast the rest of your hand as blockers.`
after: `* BOTH "Sanguine Bond" and "Exquisite Blood" -> a LOOP without limit, and it runs on life THEY gain and
life YOU lose - NOT on damage you deal them, which they never get back. So there is no defensive line: any
block that gains them life, any block you decline, and any ping at you all end the game. RACE, starting the
turn you see the second name: send every A-line tagged "[no creature they control can block this attacker]"
every combat, and no other attacker, since a blockable one hands them a blocking trigger or a lifelink gain
and that is the loop.`
Why: the deleted claim is false in both the rules and this engine. At seq 40, with both halves on their
battlefield, the seat dealt 8 unblockable damage; the opponent went 12 → 4 and the seat stayed at 38 — no
chain. The old rule would have had the seat pass every combat and die to the Vampire ping instead; the model
correctly ignored it. The other half of the old rule is worse: "block every [lifelink] attacker" is what the
seq-41 render calls fatal (`their attacker's lifelink, this block: they gain 1 ... chains without limit until
you are at 0`), and the seat correctly ignored that too. What is actually true is that both halves in play is
already lost unless you can kill them, so the rule is now a race instruction.

**Bytes.** The four edits add ~860 B, paid by:
(a) **deleting the `INFECT / POISON` bullet (478 B)** — `[infect]` and `Poison counters` appear **zero** times
in the entire 21-game corpus, across all seven decks, not just this seat's six games. It is the only bullet in
the guide keyed to a mechanic no deck in the pool plays. If a later pool surfaces infect, restore it verbatim.
(b) compressing the TEFERI bullet from 929 B to ~560 B with every rule kept (second white source, no mana
contortion, stage 1 is the only decline window, stages 2/3 have no pass, `[tapped]` target wasted, tap on
THEIR turn to stop an attacker, -2 when "0 are creatures"). The cascade was navigated correctly this wave
(`deck126` seq 25-30), so the long walkthrough is buying less than it costs.

## 5 — Proposals

**None.** The one general lesson here — a directional claim about an opponent's card ("who gains, who loses")
must come from the primitive — is already Amendment-covered by `wave68/strategy-writing-skill-v2.md` §4.3
("Every card claim (cost, MV, P/T, loyalty, colours a land makes, targeting, zone, return path, **mechanism**)
is written from the primitive at draft time and re-checked every wave"). Edit 4 exists because that rule was
not followed, not because it is missing. `wave62/general-strategy.md` is not loaded at runtime, so a proposal
there would change no behaviour.

## 6 — What I did NOT check

- The other six decks' seat files beyond the opponent-seat context reads for games 4, 5 and 6; those seats'
  reviews own them. I did not open any `.stderr`.
- The 1 `async_drops` and the 63 `hold_windows_skipped_priority` in the deck125 game, and
  `ask_replays_reserved` (1/0/1/0/1/2 across the six) — the `askreplay/` sidecar directory does not exist in
  this corpus at all, which is the engine seat's item, not mine.
- `blocker_forecast_collapsed` 8 / `blocker_forecast_gang` 2 in the deck123 game: I read the two block windows
  and the collapsed A-line at seq 19, but did not audit every collapsed forecast against the board.
- Card primitives: I verified only Exquisite Blood (`mtg.txt:37966`), Lair of the Hydra
  (`borderline.txt:64290`) and Sigarda's `moveto(hand)`. I did not re-verify the rest of the deck this wave.
- Anything about the deck LIST itself; I read only what the prompts rendered.
- The mulligan and bottoming rules are untested this wave — all six games kept the opening seven at seq 2 and
  no bottoming or cleanup-discard window fired, so §MULLIGAN and §BOTTOMING carry no evidence either way and I
  left them unchanged.
