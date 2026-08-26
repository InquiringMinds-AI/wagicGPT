# deck123 - wave-46 guide edits, before -> after, each with its finding

Base: the live guide `projects/mtg/bin/Res/ai/baka/deck123_strategy.txt` as of the wave-45
reviewer revision + the reviewer boundary pass (3ca0b2622). Full revised file: `strategy.txt`
beside this one. Every card cost, type and P/T asserted in a new sentence was read off
`projects/mtg/bin/Res/sets/primitives/mtg.txt` with `/usr/bin/grep`; the "no enchantment removal"
claim was read off `projects/mtg/bin/Res/ai/baka/deck123.txt`. File is pure ASCII, no line over
130 chars (the one 130-char line is pre-existing).

---

## EDIT A46-1 - a new RULE -1 at the top of the file: the mulligan chain stop, hoisted and
## made plan-proof

FINDING. Two of six games were lost at the pre-game mulligan ask. vs126: seven mulligans, kept
ZERO cards, lost 42-0 on turn 20. vs162: six mulligans, kept ONE card, made one decision all
game, dead on turn 13. Nine windows at "(keeping 5)" or lower were answered Mulligan with at
least one land in hand - the exact case the boundary pass's STEP 1 was written to stop. The
mechanism is not wording: the model's own PLAN ("Mulligan to find a hand with 4+ lands ...") was
re-shown to it as "YOUR PLAN (as you last stated it)" at every subsequent look, and its reply at
looks 3-7 is that sentence verbatim. A rule that lives at line 340 of 424 does not reach an ask
that fires before turn one against a plan sitting at the top of the prompt.

BEFORE: no rule at the top of the file; the chain stop was STEP 1 inside `=== MULLIGAN ===`.
AFTER: a new `=== RULE -1 - THE FIRST ASK OF THE GAME IS THE ONE YOU LOSE MOST OFTEN ===`
immediately under the deck thesis, keyed to the ask's literal "(keeping M)" price line with three
branches (M<=5 keep on one land and stop reading; M=6 keep unless zero-lands or all-lands; M=7
use the comparison), plus an explicit `THE CARRIED PLAN IS NOT EVIDENCE ABOUT THIS HAND`
paragraph naming the re-shown plan block by its rendered string, plus both losses in numbers.
The MULLIGAN section's STEP 1 is left in place unchanged - a reader who reaches it gets the same
answer, and skill 66 forbids moving a floor in either direction on one corpus.
WHY PLACEMENT AND NOT WORDING: amendment 60. The text was already correct and was already
obeyed by neither game.

## EDIT A46-2 - CHECK 1 (the lifegain wall board): the release clause named two cards this deck
## cannot remove

FINDING. vs126 seq 39: CHECK 1 fired correctly for the first time in three corpora - 14 attackers
held on a Sanguine Bond + Perimeter Captain + Pride Guardian board, and the hold is right on the
arithmetic. vs126 seq 40, two windows later: `Cast Damnation`, plan "to wipe the board, removing
the Sanguine Bond, Perimeter Captain, Pride Guardian". Sanguine Bond `type=Enchantment`,
Exquisite Blood `type=Enchantment`; deck123's sixty contain no enchantment removal. So the
clause "until a Tragic Slip, a Damnation or an edict has taken the LAST of them off that line"
is UNREACHABLE for two of its own four names, and CHECK 1 as written was an instruction never to
attack again - which is what happened: 14 Humans stayed home from turn 15 until the game ended
42-0. The guide taught the false belief that produced the losing Damnation.

BEFORE: "If ANY of them is on that line, answer "ATTACK: none" - this window and every attack
window after it - until a Tragic Slip, a Damnation or an edict has taken the LAST of them off
that line."
AFTER: the check splits. BRANCH A (Perimeter Captain / Pride Guardian only - both creatures,
both removable) keeps the old absolute verbatim. BRANCH B (Sanguine Bond or Exquisite Blood on
the line) states outright that they are never coming off and replaces the freeze with a
computable release: attack when your attacker count A is at least 3B+3, where B is their untapped
creature count. The 3B+3 is read off the render's own numbers - each of their blocking bodies
gains at most 3 (Pride Guardian), 2 (Perimeter Captain per blocking defender) or 1 (a lifelink
blocker), and A-B unblocked damage has to beat that. At vs126 seq 39, A=14 and B=6, so 3B+3=21
and the rule says hold - the rule reproduces the pilot's correct decision at the one window
where it was tested. Lethal overrides both branches on the printed life number.
B14 RE-CHECK (the wave-45 recorded falsifier): still UNREACHABLE-IN-POOL. No lethal alpha strike
on a wall board arose (best was 14 power into 22 life). A different falsifier landed instead and
is what this edit answers; the lethal-check exception is now written in anyway, inside BRANCH B,
because the arithmetic branch needs it to be complete.

## EDIT A46-3 - RULE 3: Damnation destroys creatures and nothing else, plus a hard stop

FINDING. Same window, vs126 seq 40. Own creature count 14, theirs 6 - a flat violation of the
rule's own "cast when THEIRS is bigger". 37 Damnation offers this corpus, 1 cast, and the 1 was
the game. The plan's stated reason was to remove two enchantments.

BEFORE: "Damnation destroys ALL creatures, yours included." (opening line of RULE 3.)
AFTER: a new opening paragraph - `DAMNATION DESTROYS CREATURES AND NOTHING ELSE`, quoting the
option row's own printed text, listing what survives (enchantment, artifact, planeswalker, land),
naming the 14-Humans loss, and stating that nothing in the deck removes an enchantment. Plus, at
the end of the rule, a `THE HARD STOP`: if your own "of which N are creatures" is 4 or more, do
not cast Damnation at all, whatever theirs says. Four bodies is a board this deck wins with and
there is no opposing board worth trading four of them for.
WHY A SECOND ABSOLUTE RATHER THAN RE-STATING THE COMPARISON: the comparison was already stated
three ways and was read as a comparison of THREATS, not of counts. An absolute keyed to the
pilot's own printed number cannot be re-interpreted.

## EDIT A46-4 - RULE 5: an edict cannot touch a planeswalker

FINDING. vs126 seq 38. Opponent battlefield "of which 7 are creatures"; it cast Tribute to Hunger
with the plan "to remove Sorin, Lord of Innistrad". Primitive: `target=opponent ...
notaTarget(creature|mybattlefield) sacrifice ... targetedplayer`. Three errors in one window -
N was 7, not 1; the named removal target was a planeswalker; the card text stating both was on
the option row.

BEFORE: the N-check ran straight from "Cast Devour Flesh ..." into "Before any edict, run this
in order:".
AFTER: a paragraph inserted above the N-check - `AN EDICT CANNOT TOUCH A PLANESWALKER` - keyed to
the render's `[planeswalker] [counters: Nx loyalty]` tag, naming the Sorin sentence, and closing
the loop honestly: if a planeswalker is what is beating you, no card in this deck answers it, so
build the board and attack it. The N-check itself is NOT restated a fifth time (amendment 28).

## EDIT A46-5 - RULE 1: the creature-count cap is deleted and replaced by a power stop

FINDING, and this is the sunset test coming due. The boundary pass raised the cap 12 -> 20 with
the sunset "if wave 46 shows 0 violations at 20 ... wave 47 deletes it". Wave 46 shows SIX takes
at or above 20 (vs130 seq 51/52 at 20 and 21; vs152 seq 39-42 at 20, 21, 22, 23), none under the
lethal exception, in two games that were both WON - and SIX declines BELOW the cap where the rule
says take the line (vs130 seq 13 at 3 and seq 22 at 12; vs126 seq 31/33 at 15; vs146 seq 33 at 13,
seq 36 at 15), none punished. A threshold crossed freely in one direction and obeyed wrongly in
the other is not a threshold. The size half of the WHY re-measures TRUE and therefore removes the
last argument FOR a headcount: the largest deck123 prompt at 23 or more creatures this corpus is
19,082 chars, identical to its largest at 12 or more; the two biggest prompts at the seat (23,953
and 21,704) are a library reveal and a 0-creature board.

BEFORE: "M is 19 or less: take the line ... M is 20 or more: answer 0 (pass)", the
`WHY THE NUMBER IS 20` paragraph, and the lethal-blocker exception.
AFTER: the test becomes POWER against their life - add the printed power of your creatures that
do not read "summoning sick" (call it P); P below their printed life, take the line; P at or above
it on YOUR turn, stop and go to ATTACK; P at or above it on THEIR turn, take it anyway (a token
made then blocks now and attacks next turn, which the section already teaches). The paragraph
that replaces the WHY records why both numbers went, and the measured prompt-size flatness so no
later wave re-derives it. The two DECIDING SITUATIONS token lines inherit the new test in the
same edit (skill 51), and the `[repeat:]`-note reading instructions collapse into the battlefield
line, which is what the new test needs anyway.
NOT A DELETION OF THE ONLY STOP (skill 35): the section still stops, on a condition that has
evidence.

## EDIT A46-6 - DECIDING SITUATIONS additions

Three new entries and two rewrites, so the quick-reference agrees with the rules above it: the
"(keeping 5)" line gains the carried-plan clause; the wall-board line splits into the two CHECK 1
branches; new lines for "Damnation does not remove an enchantment", "your own count is 4 or more
- do not cast Damnation", and "an edict cannot touch a planeswalker"; the two token-line entries
lose the 19/20 numbers and gain the power test.

---

# THRESHOLD WHYS (skill amendment 61) - every numeric threshold this guide now ships
- T = "(keeping 5)" (the mulligan chain stop, now in RULE -1) because the London mulligan already
  shaves a seven to six for one card, so a third look buys a fresh seven at the price of two.
  Wave 46 added the price in full: the two games that ignored it kept 0 and 1 cards and lost
  42-0 and on turn 13.
- T = 3B+3 attackers (the BRANCH B wall-board release) because each of their blocking bodies
  gains at most 3 (Pride Guardian), 2 (Perimeter Captain, per blocking defender) or 1 (a lifelink
  blocker), and Sanguine Bond turns every point of that into life off you; A-B unblocked 1/1s
  have to beat that sum, which they do from 3B+3 up. Measured against the one live window
  (A=14, B=6): the rule says hold, and holding was correct.
- T = 4 of YOUR creatures (the Damnation hard stop) because four bodies is a board this deck
  closes games with, and the one Damnation cast this corpus traded fourteen for six and lost.
- T = P >= opponent life (the token stop) because that is the point at which another token adds
  nothing to your own turn; it replaces T = 20 and T = 12, both of which were crossed unpunished.
- T = 3 lands (the bottoming land floor) and T = 4 mana (the bottoming keep-order target) are
  unchanged from wave 45 and untested this corpus - neither `bottom` ask this wave presented a
  real ordering choice (one bottomed the entire hand, one kept a single card).
- T = 2 (the collapsed tag's N) unchanged: at N of 2 the defender may gang the attacker and the
  parenthesis prices only the biggest single blocker; at N = 1 the parenthesis is exact.

# WHAT THIS FILE DOES NOT CHANGE
- The Marsh Flats order (Rule 2): 0 offers this corpus, so no evidence either way.
- The Lightning Greaves gate: 1 cast in 4 offers, correct, but too thin to call fixed.
- The blocking STEP 1 token-first letter: the one divergence (vs130 seq 55, Lord of Lineage spent
  on a "(both die)" row where a token was available) was the winning play, and skill 66 forbids
  moving a floor on one favourable window. Recorded, not patched.
