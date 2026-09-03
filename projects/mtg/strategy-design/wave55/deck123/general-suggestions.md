# deck123 -> core-prompt / render proposals (wave 55)

Layer-routed. Nothing here is an engine BUG - those are in
`wave55/seats/seat-123-130.md` under ENGINE / RENDER ITEMS (E1 the HOLD re-open predicate, E2 the
false ability-row opponent life, E3 the empty `chosen_text` on a refused re-ask, E4 the narrow
discard verdict predicate, E5 duplicate cast rows, E6 the unpriced Greaves row, E7 `reveal_stall`
untested). What follows is prompt/render text I would ask the core loop for, each with the seat
evidence and the one-line reason it improves decision quality rather than just adding bytes.

## G1. GIVE THE MENU ITSELF A DEAD-VERDICT LINE WHEN EVERY CAST ROW ON IT IS PRICED DEAD.
**Evidence.** 108 windows at deck123 where every `Cast ...` row carried a dead verdict; 2 casts off
them (`123v125` s83, s87). Wave 54: 5 of a comparable population. The per-ROW verdicts are doing
almost all the work already - the rung has gone 5 -> 2 with no engine change - and the residue is
a pilot that reads six rows, agrees in prose that each is dead, and then answers one of them
anyway ("dead, but only way to dig", "Board is clear. Cast Damnation to clear board (0 vs 0)").
**Ask.** One line under the option list, emitted only when the predicate holds:
`[every cast row on this menu is priced dead right now; the rows that do something are <the
non-cast rows>]`. It costs one line on ~12 % of casting windows and it converts six separate
per-row judgements into one.
**Why it is a render ask and not a guide ask.** Both guides already say it in words (deck123's
"WHEN EVERY CAST ROW ON THE MENU IS PRICED DEAD, ENTRY 8 IS THE PLAY"), and that paragraph is what
took the count from 5 to 2. The remaining 2 are windows where the pilot had the rule and still
aggregated the rows wrongly - an aggregation the engine has already computed.

## G2. FIRE `plan_choice_conflict` ONLY WHEN THE PROSE NAMES A DIFFERENT ROW ON THIS MENU.
**Evidence.** 6 firings at my seats (deck123 4, deck130 2), and they are the largest single
fallback class in the corpus at my seats. Carried unchanged from wave-54's E4 because the shape
has not moved: `123v126` s42 and s69, `123v146` s16, `123v152` s56 all pair a CHOICE line with a
PLAN line whose *wording* about passing or stopping is the pool's own mandated template
("this window: pass", "stop reached"), not a competing row name.
**Ask.** Restrict the predicate to: the reply's prose names a card or row label that (a) appears on
THIS menu and (b) is not the row the CHOICE line took. A plan that merely contains the word "pass"
or "stop" while the CHOICE takes a row is the guides' own PLAN template and should not fire.
**Paired guide half.** A55-TRIM re-aims deck123's cast-list warning away from CL numbers (fixed:
0 CL-number replies this corpus, down from 5) and onto the two shapes that remain - naming a card
that is not on the menu, and naming an ABILITY on a CASTING menu.

## G3. STAMP `chosen_text` WITH THE ROW ACTUALLY EXECUTED, ALWAYS.
**Evidence.** `123v126` s25 and s75, s125, s207; `123v152` s56; `130v123` s109, s122 - records with
a live `choice` and an empty `chosen_text`, and one (`123v130` s9) with `choice: -1` and empty
text. Adjudicating these required reading the following record. This is an instrument-quality ask,
not a gameplay one: a harvest predicate that keys on `chosen_text` silently under-counts every
class that passes through a re-ask.
**Ask.** On every decision record, `chosen_text` is either the executed row's text or an explicit
reason string (`"<refused: named_row_not_offered>"`). Never empty while `choice >= 0`.

## G4. ON A REPEATED WINDOW, PRINT WHAT CHANGED SINCE THE LAST ASK - AND PRINT IT AS THE ONLY THING THAT CHANGED.
**Evidence.** The declined-note now reaches **27** in one Upkeep step (`123v126` s87) and 15 in one
Draw step (`123v162` s50). The note tells the pilot HOW MANY times it has declined; it does not
tell it that the reason the window re-opened is a life total the pilot cannot affect. The pilot's
prose in those runs reads like a model looking for a reason it is being asked again
("Board unchanged", "The loop is currently resolving; I will pass priority and wait").
**Ask.** Extend the existing note by one clause when the option SET is unchanged:
`[you declined this exact list N times already this turn; the only change since your last answer
is: Your life 6 -> 5]` - or, when nothing at all changed, `... nothing on this menu has changed`.
**Why it is worth a line.** It is the difference between a pilot that treats the re-ask as evidence
it misplayed and one that treats it as noise. It also makes E1's defect legible from the transcript
alone, which is how I found it. If E1 ships (the hold honouring the option-set key), most of these
windows vanish and this line costs nothing on the ones that remain.

## G5. PRICE THE EQUIPMENT CAST ROW.
Filed as E6 in the seat file because the ask is a verdict clause, but it belongs on any core-prompt
list too: `Cast Lightning Greaves {2}` is the only recurring cast row at this seat with no
`{right now: ...}` clause, and it is the seat's oldest unclosed break (fifth corpus, four separate
guide paragraphs against it). Asked string:
`{right now: you control 0 creatures - this equips nothing}` on the cast row at zero own creatures,
and `{right now: <name> already wears Lightning Greaves #1}` on a re-offered equip line.
Every sibling rung at this seat closed when its row got a verdict; this one has never had one.

## G6. NOTHING I WOULD CHANGE ABOUT THE REPLY PROTOCOL.
Recording a negative, because the wave-54 file asked for two things here and both landed. The
`CHOICE: N (short name)` contract held at 873 decisions with 2 name-over-index refusals and 0 wrong
casts from them; **0 of 814 coded replies named an index outside the menu's range**, which is the
shape wave 54's five CL-number replies took, so the worked-example line plus A54-7's relabel has
closed that class outright; and the age stamp on the carried plan (`as you last stated it, N
windows ago on turn T`) rendered on every window I read. (I did NOT measure first-option echo as
such - 211 of 814 replies took row 1, which on these menus is usually the best play and is not by
itself evidence of anything.)
The one reply-side residue is G2's over-firing, which is a predicate, not a protocol.
