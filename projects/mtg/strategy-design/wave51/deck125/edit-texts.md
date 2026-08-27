# deck125 - wave-51 edits (each: finding -> before -> after)

Start = live `bin/Res/ai/baka/deck125_strategy.txt` (wave-50 W20-W25 + boundary pass). Full revised
guide: `wave51/deck125/strategy.txt` (44,727 -> 47,836 chars). Every edit below is keyed to a
corpus seq in `seats/seat-125-126.md`.

## W26 - Revelation at `max affordable X=1/2`: the DECIDING-SITUATIONS spend line contradicted the section (W48/W57 FAIL: `vs152` seq 15-16, `vs162` seq 10-11)
Finding: both X=1/2 casts came from menus whose only other rows were "Cast nothing" and a sweeper
at 0; the guide's own bullet "spend it on Staff of Nin, on Elixir, or on Sphinx's Revelation rather
than passing the turn with it unused" licensed exactly that. #131-style pair (two sentences, one
hand, two verdicts). The `vs162` cast tapped out with two Scatters and cost the Master of the Feast
window (seq 13 events).
BEFORE (Deciding situations): "spend it on Staff of Nin, on Elixir, or on Sphinx's Revelation
rather than passing the turn with it unused - the tap-out gate still binds, and so does the
draw-is-damage stop."
AFTER: "spend it on Staff of Nin or on Elixir rather than passing the turn with it unused - the
tap-out gate still binds, and so does the draw-is-damage stop. Sphinx's Revelation is NOT that
spend: its row must read "max affordable X=3" or more, whatever else is on the menu. Twice this
corpus you cast it at "max affordable X=1" and "X=2" at 20 life because nothing else was castable;
the X=1 cast tapped you out on turn 7 with two Essence Scatters in hand, and the Master of the Feast
that killed you resolved on turn 8 with no window offered."
BEFORE (Revelation section): "Sixteen windows at "max affordable X=1" or "X=2" this corpus, sixteen
holds: read the N off the row, and 3 is the floor."
AFTER: "Sixteen windows ... one corpus, sixteen holds; the next corpus two casts at 20 life, both
from menus whose only other rows were "Cast nothing" and a sweeper at 0. "I have spare mana and
nothing else to cast" is never a reason: read the N off the row, and 3 is the floor. IF YOUR PLAN
SAYS "draw a card or two while the mana is unused", THE PLAN IS WRONG." (#102's plan-text form.)

## W27 - The tap-out gate: Essence Scatter counts against EVERY opponent (W54: `vs162` seq 10-13 - a W22-legal tap-out that lost the game; seq 24; `vs152` seq 24)
Finding: W22's exception ("a Scatter counts only when the opponent's line or log shows creatures")
was derived from a wave-50 break that cost nothing; this corpus the seat tapped out on turn 7
against a line showing only Howling Mine + Liliana's Caress, and deck162's Master of the Feast
(5/5 flying, `abilities=flying`) resolved on turn 8 with no ask record between seq 12 and 13. Every
deck in the pool casts creatures. The two `leaves 1` Staff casts with a 3-cost counter in hand
(`vs152` seq 24, `vs162` seq 24) each cost the next opponent turn's window (three creatures; Ob
Nixilis + Howling Mine #2). Price table unchanged (2/3/5); the exception is deleted and the
section shortened by the paragraph that carried it (#102: shrink).
BEFORE: "- Essence Scatter counters CREATURE SPELLS ONLY ... It counts - N must cover its 2 - when
the opponent's battlefield line or the game log shows they cast creatures. Against an opponent
whose line and log show only enchantments, artifacts and sorceries (...) a Scatter in hand is not a
reason to hold mana: it stops nothing they cast. You held two open for a Staff at "leaves 0"
against exactly that deck - the Staff was right, and the sorcery that killed you next turn was not
a creature. - No counter in your hand that counts: the gate is open at any N."
AFTER: "- Essence Scatter counters CREATURE SPELLS ONLY - a spell whose stack line prints "(creature
P/T)". It still counts, against EVERY opponent, and its price is 2. Every deck in this pool casts
creatures, including the draw-punisher deck whose line shows only enchantments on turn 7: its
Master of the Feast (5/5 flying) is the creature that kills you, and the corpus in which you tapped
out on turn 7 "because they cast no creatures" is the corpus in which that Master resolved on turn 8
with no window offered and took you from 21 to 1. - No counter in your hand at all: the gate is
open at any N."
Plus the same key in five places: the gate's opening sentence ("cheapest counter in your hand:"),
the Staff bullet ("A STAFF ROW WHOSE N IS BELOW THE PRICE OF A COUNTER IN YOUR HAND"), the Elixir
activation sentence, and two DECIDING-SITUATIONS bullets (the Staff bullet now reads: "take the
Staff only from a row whose "leaves N" covers the cheapest one (Scatter 2, Cancel or Fracture 3, a
lone Gavel 5), or with no counter in hand ... "Leaves 1" with a Dream Fracture in hand is below the
price - you took that row twice this corpus, and Ob Nixilis and three creatures resolved on the
next opponent turns with no window offered."; the counter bullet now names the Revelation row
explicitly). Render half: general-suggestions G1.

## W28 - Path TARGET ask: Sigarda over Brutal Cathar (`vs152` seq 18-19; the loss)
Finding: the target ask offered `Sigarda, Champion of Light (4/4) [trample, flying] "... Whenever
Sigarda attacks ..."` and `Brutal Cathar (3/3) [daybound] "When this creature enters or transforms
..."`; the reply took the Cathar. Sigarda matched the guide's engine-text list ("Whenever ...
attacks" - primitive `auto=@combat(attacking)`), was the flier and the higher power; the Cathar's
text is a one-shot ETB. Sigarda dealt 18 -> 1. STRATEGY. The rule gains the negative example and
the target-ask order.
BEFORE (Path section, after the Aspirant/Unraveler line): "Only when NO candidate has such a line
do you use the power rule:"
AFTER (inserted before it): ""When this creature enters" is NOT that line - it already happened, and
it is over. Brutal Cathar (3/3, "When this creature enters or transforms ... exile target creature")
is a body, nothing more; Sigarda, Champion of Light (4/4 [trample, flying], "Whenever Sigarda
attacks ...") is the engine AND the flier AND the bigger power. You Pathed the Cathar and left
Sigarda at 18 life; she grew to 6/6 and took you to 1 in three combats. A candidate tagged [flying]
outranks a ground candidate of the same or lower power on its own: a flier is the creature a later
Lightmine Field never stops."
NEW bullet (Deciding situations): "- The TARGET ask lists two or more candidates: the one whose
text says "Whenever ... attacks", "{T}:", "At the beginning of" or "venture" first; then the one
tagged [flying]; then the highest printed power. "When this creature enters" is none of those."

## W29 - Emrakul against an edict WITHOUT a converter: their life <= 15 (W52: `vs123` seq 48 a wash at their 20, seq 117 a win at their 8 that the old clause forbade)
Finding: the log-shown-Tribute clause forbade the winning cast (seq 117) and did not prevent the
wash (seq 48 - no Tribute was in the log yet). Without Sanguine Bond/Exquisite Blood the edict
only refunds the 15 the attack dealt; the extra-turn attack at their <= 15 ends the game first.
BEFORE (#1): "... or the log prints "Opponent used: Gain life equal to its toughness with Tribute
to Hunger" or a Devour Flesh, Emrakul is not a finisher, it is a fifteen-life gift: ..."
AFTER: "... Emrakul is not a finisher, it is a fifteen-life gift: ... THE EDICT WITHOUT A CONVERTER
is a different case: when the log prints "Gain life equal to its toughness with Tribute to Hunger"
or a Devour Flesh and their line shows NEITHER enchantment, the edict only refunds them the 15
Emrakul hit them for - so take the Emrakul row when their printed life is 15 or less (the
extra-turn attack ends the game before the refund matters) and answer "Cast nothing right now"
above 15. You cast it at 20: it attacked them to 5, a Tribute ate it in response to your next
spell, they were back at 20, and the Staffs still won the game 25 turns later; the second cast, at
their 8, won on the extra turn."
Deciding-situations Emrakul bullet re-split the same way (converter -> never; edict-only -> at
their 15 or less).

## W30 - Revelation as a LIFE spell at 5 or less (`vs152` seq 39: 1 life, X=7 offered, Staff #2 cast, died to a 3/1)
Finding: the "X is always 3" section had no branch for a life total the next attack ends; the
X=1/2 exception ("life 10 or less and no other option worth casting") did not fire because the
row read X=7. The header's second number (1) and the 3/1 lifelink were on the screen.
NEW (inserted before "YOUR X IS 3."): "THE ONE TIME THE ROW'S SIZE RULE IS OFF: your life is 5 or
less and the opponent battlefield header's second number is 1 or more. Then Sphinx's Revelation is
a LIFE spell before it is a draw spell: take its row at whatever "max affordable X" it prints, and
on the X menu announce the LARGEST X offered - the cards you discard at end of turn cost nothing
next to the damage you survive. At 1 life with a 3/1 lifelink attacker on their line and "max
affordable X=7" on the menu, you cast a second Staff of Nin instead and died to the 3/1 next turn;
X=7 was 8 life."

## W31 - Cleanup under Liliana's Caress (`vs162` seq 12/16/21/29: 4 asks = 8 life; seq 21 sent a Scatter over a Gavel with Master of the Feast on their line)
Finding: the cleanup section priced nothing; Liliana's Caress (`auto=@discarded(*|opponenthand):
life:-2 opponent`) makes each send 2 life, and a Revelation onto a full hand is a paid discard.
NEW (end of the cleanup section): "And read the opponent battlefield line for "Liliana's Caress"
({effect:} "Whenever an opponent discards a card, that player loses 2 life"): under it every card
this ask sends costs you 2 life on top - four cleanup asks cost you 8 of your 20 in one game.
While it is on their line, a card you can cast on your own turn (an Elixir, a Staff from a row
that passes the gate) is worth casting to get under seven, and a Sphinx's Revelation onto a hand
of seven is a discard you paid mana for."

## Recorded, NOT edited
- `vs126` seq 17: Supreme Verdict declined at `2 without a restriction against attacking` (two 1/1
  lifelink Vampires) at 18 life - a 2+ branch break; the loop, not the Vampires, ended the game.
  Violated-unpunished, first instance of this shape: no restatement (#64/#118).
- `vs162` seq 21 cleanup: an Essence Scatter sent over a Gavel with a creature on their line - the
  order already says "Gavel before Cancel before Scatter"; one miss, no restatement.
- `vs130`: land destruction on four lands; no rule reaches it. `vs126`: a Scatter-only hand.
- Mulligan section: 6 keeps at 2/2/4/4/3/3 lands, all inside the odds text; #131 twelve-hand
  check re-run, no disagreement; untouched.
