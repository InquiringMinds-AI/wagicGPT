# deck130 -> strategy-writing skill: proposals from the wave-57 corpus

Counted from the rendered `prompt` of the six `deck130` seat logs in
`matchups-20260903-174505/`. Only proposals my own evidence supports.

## S57-6. A RUNG BELONGS AT THE WINDOW WHERE THE ALTERNATIVE EXISTS - THIS IS THE SEAT'S BIGGEST FINDING
**Evidence, three independent instances in one corpus.**
1. `130v125` s92/s94. The face-damage floor ("only when that number is 6 or less") was applied at
   a TARGET menu whose only two rows are `The opponent (player, life 26)` and `Yourself (player,
   life 20)`. There is no declining row. Scored by the letter these are two breaks; they are not
   decisions at all. The decision was s91/s93, the CAST rows, which printed `{no creature target
   - and 3 to the opponent at life 26 leaves them at 23}` and a `Cast nothing right now` row.
2. `130v162` s7/s8. The "NEVER announce X=0" rung sits on the X menu. At s8 the X menu had ONE
   row. The spell was spent at s7, where the cast row read `{X pricing: your mana affords only
   X=0 right now, which deals 0 damage and kills nothing}` and `Cast nothing right now` was on
   the screen.
3. `130v152` s31/s33. The "YOURS names Rorix or Siege-Gang -> do not cast" clause is written
   inside the X-menu apparatus; the cast row at s31 printed the same two lists and the same
   marker, and that is where the card was spent.
**Proposal.** Add a hard check to the skill: *for every prohibition, name the menu on which the
alternative row exists, and quote that row's text. If the seam you wrote the rung at has no
declining row, the rung is at the wrong seam and moves one screen earlier.* A reviewer can
falsify this mechanically: for each rung, census the menus that carry its trigger literal and
count how many carry a decline/hold row. A rung whose trigger renders mostly on decline-free
menus is mis-seated.
**Falsifiable next corpus:** at this seat, takes scored as breaks on a menu with no declining
row: 4 -> 0 (because they are re-scored at the cast, where the rule now lives).

## S57-7. NAMING THE RATIONALIZATION DOES NOT SUPPRESS IT; ONLY A TEST ON A PRINTED STRING DOES
**Evidence.** deck130's guide contains, verbatim, *"'Start chipping away at their life total' and
'it is the only active play I have' are the two sentences that spend it early"*. On `130v125`
s91 the model replied `PLAN: ... 3 damage is not lethal, but it is the only active play. Pass
turn after casting.` and on s93, one window later, the same sentence again. The prohibition
named its own failure sentence and the model wrote that sentence while performing the forbidden
act, twice.
Contrast the rungs that HELD this corpus, which are all tests on a printed string: `[second
copy:` (deck130 9/9 correct - Talisman and Spellbomb both stack), the `DRAW PUNISHERS` line
(0 draws), `{paying this costs you N life}` (6 takes, none at 3 life or below), the land-drop
ask (55/55 at deck123, 24/24 here).
**Proposal.** Add to the skill's prohibition-writing section: *a prohibition is written as a
TEST the pilot runs on a string the render prints - "read clause X, compare number A to number
B, and if the comparison holds the answer is row R". Quoting the pilot's own past rationalization
is decoration and, on the evidence, is ignored at this tier. Where a guide already carries such a
quotation, it must ALSO carry the string test, and the reviewer counts the test's denominator,
not the quotation's.*

## S57-8. WHEN A RENDER MARKER MOVES TO A NEW SEAM, EVERY GUIDE RULE ABOUT THAT MARKER IS STALE
**Evidence.** deck130's guide says, correctly and in capitals, *"A MARKER IS NOT A REASON TO
CAST. It answers WHICH X, never WHETHER: ... WHETHER is decided one screen earlier, at the cast
row."* This corpus the marker MOVED: `[<- best X for this cast: X=4 - best trade: the most of
THEIRS at the smallest cost to YOURS]` now prints on the CAST row itself - **7 renders, 4
takes**, across `130v152` s31, `130v146` s64, `130v126` s59 and `130v123` s50. The
sentence "one screen earlier" no longer separates the two, and the pilot cast a sweeper that
killed five of its own creatures to kill one of theirs, in a reply whose PLAN named the
creatures the sweep destroyed.
**Proposal.** *Reviewers census each render marker BY SEAM (cast row / announce menu / target
menu), not by literal, and any guide sentence that locates a marker at a seam is re-checked
against that census every corpus.* Add "which seams did this literal render on" as a required
column of the three-number literal audit.

## S57-9. A "BEST X"-STYLE MARKER NEEDS ITS OWN GUIDE ARITHMETIC, NOT DEFERENCE
**Evidence.** The marker's own definition is "the most of THEIRS at the smallest cost to YOURS".
On `130v152` s31 the only X that killed anything of theirs killed five of the pilot's own
creatures, so that X is the marker's answer by construction - and the marker is the loudest
thing on the row. Wave 56 recorded "marked row taken 6 of 6" as this seat's best result; this
corpus the same deference produced the seat's worst cast.
**Proposal.** *When the guide defers to a render marker, it states the ONE comparison under which
deference is withdrawn, in the marker's own printed vocabulary.* Applied here as D57-5: take the
marked row when the THEIRS list is no shorter than the YOURS list. This is not doubt of the
render - both lists are printed on the row and both are true; it is a rule over the true numbers
rather than over the recommendation built from them. The render-side half of this belongs to the
core loop and is filed as E3 in my seat file.
