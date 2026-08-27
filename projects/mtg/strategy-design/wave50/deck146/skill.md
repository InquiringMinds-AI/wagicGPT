# deck146 - proposals for the strategy-writing skill (wave 50)

S-1. **A LIST-ANSWER ASK (PUT: n, m) GETS ITS OWN EMIT SHAPE, AND THE GUIDE NAMES THE CARD CLASS
THAT NEVER APPEARS IN IT AT THAT N.** `146 vs126` s4: the bottom ask at (keeping 5) on a 2-land
keep; the reply's PLAN said "play Shineshadow Snarl and Brightclimb Pathway" and its PUT line sent
Pathway under. The guide's rule ("bottom a land only when the lands left would still cover the
spells") was a board predicate the executor had to evaluate; the failing reply evaluated the plan
correctly and emitted the wrong numbers. #105 (shape with blanks) and #120 (gate first in the
dictated sentence) were written for CHOICE/PLAN lines; the PUT line is a third emit surface with
the same failure mode. Proposed rule: where an ask's answer is a list of card numbers, the guide
states the list's CONTENT rule in emit form ("the numbers of the highest-cost spells, most
expensive first") plus the one class that never enters it at the current N ("no land from a keep
of two or fewer"), and tells the pilot to read the list against its own plan sentence. Metric:
PUT lines containing a card the same reply's PLAN names as a play.

S-2. **CROSS-GUIDE CONVERGENCE IS A CHECKLIST ITEM, NOT A HOPE.** deck152's RULE -1 M=6 line
("one land under 'would not cover any spell in it' ships") was written in wave 48 after a
33-turn no-cast loss; deck146's STEP 2 still said "one land when the spells cost 3 or more ...
judgment call" and kept exactly that six (`146 vs125` s2, no land two until T19). #81's
coverage-diff obligation fired in wave 49's boundary pass for the mulligan FLOOR text but not for
the coverage-line KEY. Proposed rule: when one guide re-keys a rule to a rendered literal, the
boundary pass greps every other guide's same section for the pre-key form and lists the holdouts
by name - the diff is between KEYS, not only between verdicts.

S-3. **A CAST-ORDER CONDITION THAT NAMES A PERMANENT CLASS IS VERIFIED AGAINST THE PRIMITIVE'S
FILTER, NOT THE ORACLE.** Entry 1 said "creature or planeswalker you can hit" for Soul Shatter;
the primitive (`borderline.txt:106093`) filters `creature[...]` only, and the seat planned Soul
Shatter "for Ob Nixilis" twice (`146 vs162` s21, s25). The brief's card-fact rule already says
read the primitive; this extends it: every class word in a guide condition (creature, planeswalker,
nonland, permanent) is grepped in the primitive's target/filter clause, and where Oracle and
primitive disagree the guide teaches the primitive and the seat file reports both.

S-4. **A MATCHUP RULE'S "BLOCK" HALF IS CHECKED AGAINST THE MECHANISM, NOT THE VOCABULARY.**
The both-halves bullet said "block every [lifelink] attacker" - a true-sounding sentence that
described a losing action (lifelink pays on damage to a blocker; the chain runs either way). The
render said so on the A-row (`... chains without limit - fatal to you, not a trade`) and the pilot
denied it (`146 vs126` s49: "there is no loop"). #114 says write the mechanism; this is #114's
audit form for existing text: for each imperative in a matchup rule, ask what the printed row
says the action DOES on that board - a rule whose action the row prices as fatal is a false
sentence regardless of how safe it sounds.

Supporting instance, not a proposal: #109's "read the alternative" cut the other way at the Hive
(`146 vs126` s31/s35 - the alternative was `pass`, the take burned 8 mana): the violation is
real, but the offer's PHASE (11/11 Upkeep, 0 main-phase offers on this seat) is the lever, and it
is an engine item (E-4), not a fourth restatement.
