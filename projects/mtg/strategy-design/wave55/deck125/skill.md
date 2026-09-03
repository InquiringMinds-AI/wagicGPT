# wave-55 deck125 - proposals for the strategy-writing skill

Only what this seat's corpus supports. Each proposal names the amendment it would sit beside, the
evidence, and what it would cost. Six proposals; two of them are new devices, four are refinements
of amendments that already exist.

## S1 - A rule that ends with a WIN story reads as permission, however it is labelled.

Both my decks carry a mulligan branch of the shape *"<imperative>. You did the opposite once and
won; the win does not move this line."* Both branches were broken this corpus, in the same
direction, at 0 ships of 2 - deck125 kept a one-land seven (`125v123` seq 1) and deck126 kept a
one-land seven (`126v162` seq 1). The disclaimer is doing no work: what the pilot reads last is a
game it won by doing the other thing.

Proposal: **when a rule must carry a case that went against it and still won, the win goes FIRST
and the imperative goes LAST, and the paragraph names WHY the losing case looked right.** Both
guides are edited that way this wave (deck125 `MULL`, deck126 `D-F`) and W108/D110 is the test. The
general form - "order the paragraph so the last sentence is the one you want executed" - is a
cheap, checkable rule for every narrated counter-example in the corpus, not just mulligans.

## S2 - Key a threshold to a printed COMPARISON, never to a bare life number.

deck125's Revelation carve-out read "your life is 5 or less and the opponent battlefield header's
second number is 1 or more". At `125v146` seq 32/36 the seat was at **10** against three attackers
whose printed powers summed to **11**, with a life-gain row on the menu, and it declined twice and
died at -1. The number that mattered (life vs incoming) was never on either side of the guide's
test. This is the same failure shape as wave-54's W89 (a flat floor of 15 forbidding the cheapest
life the deck could spend), of opposite sign: **a constant compared against one printed number
cannot express a danger that lives in the RELATION between two.**

Proposal: extend the #192 threshold-shape amendment with a preferred FORM. When a rule's danger is
a relation, write it as *"if <printed A> is at or above <printed B>"*, and only fall back to a
constant when no second number is printed. Corollary for the render layer (routed to
general-suggestions, not here): a relation the guide has to compute from three P/T strings is a
relation the engine should print finished.

## S3 - A gate whose every observed violation was CORRECT is mis-scoped, not disobeyed.

deck125's tap-out gate has produced exactly three live breaks in two corpora and **all three are a
FIRST Staff of Nin at "casting this taps you out"**: one in wave 54 (unpunished) and two here
(`125v123` seq 49, `125v126` seq 49 - both resolved, both started the clock on the next upkeep,
both games won 32-0 and 44-0). The wave-54 seat file's response was to restate the gate harder. The
right response is a carve-out.

Proposal: an amendment saying **when a rule's violation set is homogeneous AND the outcomes went
the pilot's way, the next edition carves the shape out of the rule rather than restating the rule**
- and the edit must keep the rule's remaining scope explicit (here: second Staff, Elixir
activation, Lightmine Field), so the carve-out is a narrowing and not a repeal. The falsifier is
W105: first-Staff takes N/N, second-Staff takes 0.

## S4 - An absolute of the form "X is NEVER your answer to a menu that prints any Y row" must
## enumerate the Y rows other rules already forbid.

deck126's Rule #2 says *"while both names are on your battlefield line, 'Cast nothing right now' is
NEVER your answer to a menu that prints any Cast row at all"* while Rule #4 forbids a Tribute at
"at 0 this does nothing" and entry 7 forbids a `[second copy:` row. At `126v125` seq 169/185 the
only Cast rows WERE those two; the pilot obeyed the specific rules and broke the absolute, then at
seq 200 obeyed the absolute and broke the specific one (the corpus's only second-copy take, 1 of
51).

Proposal: **every "any row" absolute carries its exclusion list inline.** A guide that contains
both a universal and its exceptions in different sections is a guide with a live contradiction, and
the pilot will resolve it differently in adjacent windows - which is exactly what happened here,
three windows apart.

## S5 - Count literals from the rendered `prompt`; the translog `options` array is a different
## object and it silently under-reports.

Lane D's discard verdicts (`{spare: you control N lands already}`, `{dead right now: ...}`) and its
`(copies k-m of n in your hand)` collapse render **inside the prompt** and are absent from
`options_text`, which still holds bare card names. A first pass over `options_text` reported 0
renders for a feature that had shipped on 46 rows. The wave-55 brief already carries this warning;
this seat is a second, independent instance of it, and the failure mode is a REVIEWER one (a
shipped lane reported as missing), so it belongs in the skill's verification section rather than in
a guide.

Proposal: add to the audit procedure - **for any literal that lives inside an option row, the
denominator is a count of rows parsed out of the rendered prompt text, and the seat file must say
which it used.** Every row count in this wave's seat file is prompt-derived and says so.

## S6 - An ordered send-list needs a DEAD tier, or the pilot must break the list to make the right
## send.

deck125's cleanup list is seven numbered steps. Four times this corpus the seat sent a LONE sweeper
ahead of its counterspells with the opponent header at 0 creatures (`125v152` seq 216, `125v162`
seq 64, `125v123` seq 45, `125v126` seq 166) - a send the list had no step for, and the right one
every time. Meanwhile step 1 (Lightmine first) is 4 of 12 by the letter and every miss cost nothing,
because the alternative sent was also dead.

Proposal: **write the head of a send-list as a TIER (everything the board has made dead, in any
order) and the tail as a sequence.** A strict sequence over items whose relative value is set by the
board, not by the card, generates fake breaks at the top of the list and real gaps in the middle.
Steps 1-3 of deck125's list are relabelled that way this wave (edit W-F2); W106 is the test.
