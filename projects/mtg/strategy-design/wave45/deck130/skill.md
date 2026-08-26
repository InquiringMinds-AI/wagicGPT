# Strategy-writing skill proposals from seat deck130 (wave 45)

Only what this seat's evidence supports.

## S6 - A "SPEND THE CARD" ROW AND AN "USE THE CARD" ROW ON ONE MENU NEED A NAMED VERDICT EACH.
deck130's guide has a whole opening RULE about not cycling its spells, and a correct release
condition for the one board where cycling IS right. At exactly that board the pilot reached the
release, opened "Choose an option for Starstorm:" - `1. Cast Card Normally` / `2. cycling` /
`3. Decline` - and took row 1, then announced X=0, believing that "spends the card" the same
way. The guide had a verdict for WHETHER to spend the card and none for WHICH ROW spends it.
PROPOSED AMENDMENT (extends #37): when a card has two or more menu paths that dispose of it,
the section that governs the card enumerates the paths BY THEIR RENDERED ROW NAMES and gives
each one a verdict, including the degenerate path (a value of 0, a mode with no legal target).
"Do X" is not a decision the executor can carry out if two rows both look like X.
DECISIVE FALSIFICATION: `1787768330-ai_baka_deck130-0x559cc3d27570-vs-ai_baka_deck123.jsonl`
seq 19/20/21 - cast at X=0 against an empty board, drew nothing, stayed on four lands with two
Rorix and two Siege-Gang stranded, lost 19 to -23.

## S7 - #25 HAS AN ATTACK-SIDE TWIN: A FLOOR EVALUATES, IT NEVER ALLOCATES THE LAST BODY.
#25 put a survival gate above the BLOCK ladder because a ladder ranks options and cannot answer
"which attacker do I spend my one blocker on". The mirror failure cost this seat a game: its
ATTACK FLOOR ranks bodies ("send everything except Blastminer"; "if their power totals your
life, send only Rorix") and cannot answer "should my ONLY creature attack, when it is also my
only blocker". At 4 life against a 4/4 with vigilance, with the opponent on 15, the floor's own
exception fired and told it to send. Rorix held home kills the 4/4 and lives - the blockers
screen prints that trade.
PROPOSED AMENDMENT: every ATTACK section for a deck that can be reduced to one creature carries
a race arithmetic gate ABOVE the floor, written as two divisions over rendered numbers (their
turns to kill you / your turns to kill them), with vigilance named as the keyword that keeps an
attacking creature on the defensive side of the sum. Label attack rules EVALUATION or SELECTION
exactly as #25 requires for blocks.

## S8 - GRADE BY ODDS MEANS THE GUIDE TEXT IS THE DEFENDANT - TWO CLEAN CASES.
Under #47 this seat produced two deviations that were RIGHT and two guide clauses that were
WRONG, in one corpus: "chump the biggest attacker" named an attacker tagged "[NONE of your
available blockers can block this attacker ...]", and "take any block where both die" named
Siege-Gang Commander against a token. Both rules were written from a real incident and both
generalised one word too far - "biggest" without "blockable", "creature" without "card or token"
(#39).
PROPOSED AMENDMENT, cheap and mechanical: when an edit is written FROM one incident, before
shipping it, walk the render vocabulary for the tags that could make the new sentence false -
the legality tags on the same screen ("[NONE of your available blockers can block ...]", the
"may block" list), the body-class distinction, the keyword list. This is #30's "record a
falsifier" turned into an authoring step rather than a documentation step: find the falsifier by
reading the render's own strings, not by waiting for the next corpus.

## S9 - A QUICK-REFERENCE LINE INHERITS ITS RULE'S CARVE-OUTS, ALWAYS.
#24 established that the closing list is a contract in both directions after deck130's own face
burn was authorised by a list line its rule forbade. The same guide has now shipped a SECOND
instance in a different lane: rule #2 step 0 gained two named DO-NOT conditions in wave 44 and
the list line "Their board shows two or more creatures and Starstorm is in hand -> cast
Starstorm" did not gain them. This one has not cost a game yet only because the pilot happened
to obey the rule instead of the list (five windows, all correct, one win).
PROPOSED AMENDMENT (a Definition-of-Done check, not new doctrine): when a RULE gains an
exception in a wave, grep the closing list for every line that summarises it and edit them in
the same edit. Track it as part of the same numbered edit so the reviewer cannot ship half.
