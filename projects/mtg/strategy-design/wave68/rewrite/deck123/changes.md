# deck123 guide rewrite (Amendment 332)

## (a) Rules KEPT (one line each, all restated as condition -> action)
1. Bare `CHOICE:`; "ANSWER: CHOICE:" is discarded. 2. A CHOICE names a numbered row on THIS window; an unpayable
card has no Cast row. 3. Costs read in mana vs "Mana available". 4. First CHOICE line runs; only the four
correction openers plus a second CHOICE line replace it. 5. Carried PLAN loses to a `{right now:}` clause. 6.
Land drop every turn; not spent by a spell or a Flats crack. 7. Untapped-land preference; Arcane Sanctum /
Isolated Chapel tapped-entry. 8. Crack Flats the first window listed. 9. Fetch the row tagged "(you cannot make
{W} right now)", else the doubled pip; dual over basic; colour printed at 0 first ({U} scarce, 10 of 24). 10.
Cast priority P0-P8, with the Alarm ahead of a SECOND maker and of the Greaves. 12. An all-dead menu is "Cast
nothing right now". 13. Pass always available; hold at the FIRST dead window; same answer on a reopened run. 14.
Instants offered on the opponent's turn at every phase - best for Slip and edicts. 15. Stop = L + C + 3, with
M/L/C read off the print; power cannot measure the loop. 16. Repeat row taken ONCE at N = (L+C+3) - M in the
row's own format; N of 0 or less is a pass. 17. Four-number PLAN template, blanks filled, restricted to the
token window. 18. Recompute the stop every window; arithmetic beats the sentence. 19. A RE-ASK past your own
stop is a pass, and the stop governs the single-tap Create row too. 20. Fire on the opponent's turn; own upkeep
worst against a sweeper log. 21. Nothing untaps under the Alarm, so the attack list is what you made on THEIR
turn. 22. Alarm first then tap; a Create row passed twice is retired for the turn. 23. Damnation destroys
creatures only; three-number plan line; the six cast/hold thresholds (M>=4 never; M=0 & K>=2 cast, no hold at
life<=10; N>M & K>=1 cast; K=0 on Captain/Guardian cast; other K=0 or N=1 one-for- one; M>N or N=0 never;
cheaper of two rows). 24. Tragic Slip decided at the cast row; Morbid clause; "kills 0 of" declines; take "-
DIES". 25. Edicts: N exactly 1; Tribute first; Devour only when Tribute is absent; stack creatures do not count;
defender/power-0 declined unless it pumps their board; never above their life; Sanguine Bond converter -> cast
nothing; naming Yourself as a life gain under Fateful hour. 26. "ON THE STACK ... that would KILL YOU" is the
only fact in the window; never a hold. 27. Greaves equipped once per copy; never cast onto an empty board. 28.
Alarm symmetry; a second copy is legal but useless; the Alarm alone does nothing. 29. Idyllic Tutor finds only
the Alarm; three brackets, one live; an Alarm in hand is a dead Tutor. 30. Bait-cast a dead row only against
counterspells, and only with the maker on the same menu. 31. Fateful hour arithmetic and its "lethal or nothing"
limit. 32. Keeper transform rule, with its {B} / five- Vampire / untapped-source condition. 33. Vision Skeins as
dig only; hand-size-6 discard risk; never into a draw-punisher. 34. Attack CHECK 0 crack-back X; CHECK 1
branches A / B2 / B (3B+3 and the block-legality gate) with the lethal override; CHECK 2 gang blocks; CHECK 3
one-blocker-one-attacker count; CHECK 4 send-all, one maker held home, lone-maker hold-back. 35. Blocking:
incoming second number; "AT BEST" N as the verdict; loop-board STEP -1; a B row is a blocker; the header
assignment is a life-maximiser; tokens before cards; lethal biggest-first; menace; makers decline "(both die)"
above 5 life. 36. Mulligan ladder, engine land/spell count, coverage line for one-landers only, zero/all ships,
six-lander keep; bottoming order 1-5; a mana land is never the bottom; Flats is no source. 37. Cleanup:
duplicates first, verdict-carrying rows, never the only maker/Alarm/Damnation.

## (b) DROPPED by category (estimates against the 71,487-byte live guide)
- Game and record narration, "WHAT THIS COST" ledgers, per-opponent loss stories: ~24,000 B.
- Counts of past behaviour and corpus tallies: ~4,000 B.
- Opponent-deck numeric identifiers: ~600 B.
- Triplicated rules (RULE + DECIDING SITUATIONS + KEY CARDS; one copy kept): ~16,000 B.
- Meta-commentary about the guide and its size bands: ~1,500 B; rhetorical elaboration and
  repeated warnings on kept rules: ~5,400 B.

## (c) Rules NOT carried, and why
Lightmine Field attack width; the basic-fetch carve-out against nonbasic-only land destruction;
"[NAMED BY THEIR ...]" surcharge and "[from exile]" pricing; the tutored-Alarm {U}-count check;
edict-vs-planeswalker; annihilator sacrifice order; the blocking GANG-BLOCK bullet. All are real
condition -> action rules, but each fires on a rare board and they were cut last to reach the
20,000-byte ceiling. Nothing was dropped for being unstatable as condition -> action.

## (d) Verification
- `stat -c %s strategy.txt` = **19996** (ceiling 20000).
- Amendment 332 grep = **0**.
