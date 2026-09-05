# Wave-62 step-one input: KNOWN BUGS from the wave-61 evaluation (orchestrator list; details in the cited files)

Corpus: `matchups-20260905-092408` (the RERUN on 8097c12cc; the first run livelocked — `wave61/corpus-livelock.md`).
Sources: `wave61/deck{146,152,125,126,162,123,130}/review.md`, `wave61/engine-seat.md`. Adjudication: 20 PASS, 3 FAIL
(C7a-outcome, C7b, R6), 4 UNTESTED (C6, R3, R4, R7), 3 SPLIT (C3, C9, C14b). Numbers below are the seats' own; cite their files.

## HIGH — render claims that are false against engine state
- **D1 enters-tapped classifier (C7b FAIL; engine HIGH-1, deck125 HIGH-1, deck152 HIGH-1).** `Play <land>` prints an
  unconditional `enters TAPPED - it makes no mana this turn` on 131 of 172 rows while quoting the card's own "unless you
  control..." sentence. Two idioms missed: the condition on the line AFTER `tap(noevent)` (`tap(noevent)` + `aslongas(...) untap`
  next line — Glacial Fortress mtg.txt:46617-18, Drowned Catacomb 32505-06, Sunpetal Grove; 70 rows) and `if <expr> then
  tap(noevent)` (Deserted Beach, Overgrown Farmland, Lair of the Hydra, Hive; 56 rows). Lane V's fix (finding 3) read only the
  first idiom. Cost: 125v162 seat steered to Plains x3, colour-screwed loss (seq 8, 26, 27). Where decidable from the printed
  battlefield (deck123 MED-1: Isolated Chapel with Underground Sea/Tundra/Scrubland out — subtypes at 125732/124691/100809),
  RESOLVE the condition; otherwise print the hedged form.
- **D2 ATTACK TOTAL kill claim vs an opposing life loop (deck123 HIGH-1; C1 repro).** `126` seq 52: `KILLS them whatever they
  block, gain included` (98 power) next to `any life they gain ... chains until you are at 0 ... fatal`. blockGain was summed as a
  bounded ceiling; with Sanguine Bond + Exquisite Blood on their side a BLOCKING lifegain trigger resolves in declare-blockers
  and loops the attacker to 0 before damage (seq 73 -> gameend: 19 -> 0, 20 -> 39). Fail closed: withhold the kill claim
  whenever a life loop is on their side and blockGain > 0 (C1's own unpriced-punisher rule).
- **D3 LOOP CAUTION is false for the pilot's own punishers (deck162 HIGH-1; closes the open lifelostfoeof question).**
  `Exquisite Blood` is `@lifelostfoeof(player)` (mtg.txt:37966); `damage:1 opponent` punishers never raise it, so the seat's
  own pings do NOT feed the opponent's loop (162v126 seq 52/54 warned; seq 55 opp 20 -> 0, my_life untouched). Sanguine
  Bond's half does fire. Either fix the trigger (Oracle: any life loss) — engine change, suite fixture owed — or make the
  caution match the engine's actual binding. Decide which; do not leave the narration lying either way.
- **D4 Lair of the Hydra X animate rendered as a fixed 1/1 (deck152 HIGH-2).** 387 rows corpus-wide all read `becomes a 1/1
  hydra {1}{g}` while the row's card text says `{X}{G}: X/X`; the primitive defines 20 rungs (borderline.txt:64293-64312).
  Repro G3 seq 115, 13 mana, opp at 19. Render the affordable X rungs (or the best affordable), not rung 1.
- **D5 X=0 cast is a forced dead end (deck130 HIGH-1, new).** `Cast Starstorm {X pricing: ... only X=0 ... kills nothing}` ->
  the X menu is ONE row `X = 0 ... this cast does NOTHING` with `no pass row`; card burned. Offered 7, taken 1. Suppress the
  cast row when max affordable X = 0, or give the X menu a Decline row.
- **D6 CRACK-BACK names Lolth's emblem as 8 per activation at 1 loyalty (engine HIGH-2, 17 windows)** — blocking DOES stop it;
  and R6's gate prices a next-turn forecast against THIS turn's open mana (53 true clauses silently dropped). Split the two.
- **D7 Intrepid Adversary "add N counters" modal ask prints no mana and no `{paying this taps: ...}` clause (deck152 HIGH-3).**
  G2 seq 23/24: engine paid 3 counters with Katilda + Elite Spellbinder, Attackers step vanished. (Related: the intermittent
  `intrepid_adversary_repeated_payment.txt` from lane T.)
- **D8 DRAW FORECAST tense + no KILLS-you verdict (deck126 engine HIGH).** Written "your next draw step" while the triggers are
  already on the stack; forecast 26 vs printed life 20 carries no fatal verdict. Also: no row is ever tagged as winning while the
  loop banner prints.

## HIGH — protocol / parser
- **D9 Reply commits the answer line before reasoning (deck146 HIGH-1).** 146v152 seq 54: `BLOCKS: B2:A2, B1:none`, then the PLAN
  derives `B2:A1` — ignored, died at -2 with the header's own "best case: you would be at 6". 8/207 replies deliberate past the
  answer line. Options: honour a LATER coded line in the same reply (deck126 says the engine now latches the LAST coded CHOICE —
  `answer_replaced` at 125 seq 64/79/87 — make BLOCKS/ATTACKS consistent with that), or teach the protocol to put PLAN first.
- **D10 `latched_row_mismatch` discarded an unambiguous answer (deck162 HIGH-2).** 162v126 seq 49: `CHOICE: 4 (Cast Teferi's
  Puzzle Box)` — index and name both match row 4, coded_answers 1 — resolved to row 5 "Cast nothing". Parser bug; PARSETEST.
- **D11 Out-of-range CHOICE naming a card not on the battlefield (deck126, seq 44/65/82)** — three windows lost; the re-ask
  should quote the offending name.
- **D12 Survivable block matching computed but never printed (deck146 HIGH-2).** `AIPlayerGPT.cpp:17351` finds the assignment
  and prints only the number; print the assignment.

## MED
- D13 Hold row's "this turn or later" promise is seam-scoped: 9 leaks; `[you declined this exact list N times]` and `[hold check: 1
  row above is new]` contradict in one prompt (deck125, 126 seq 50-52). C14b SPLIT.
- D14 `Cast Sphinx's Revelation` row hides its tap-out cost (9/154 carry a leaves tail vs 323/634 elsewhere) while carrying the
  best-X badge; all three X-floor breaks are on badged rows (deck125).
- D15 MDFC land faces contradict the "never listed in a casting menu" header and carry no land marker in-game (deck146 HIGH-3).
- D16 ~3 KB PLAN overflow truncated then quoted back for six windows (deck152 G3 seq 115-121); "Night" offered in Emrakul's
  forced-sacrifice list absorbs one of six (G3 seq 135).
- D17 One-land land-drop ask wording differs from the many-land one (`Hold X - do not play it now`) (deck123 MED-2);
  `plan_missing` refused seq 21 but accepted the identical shape at seq 18 (MED-3).
- D18 A 900 s `curl=28, connect_ms=20000` is still classed `wall_miss_unrecorded` (deck130); transport phase says wall.
- D19 Target rows print `DIES` but never price it against the same screen's CRACK-BACK total (deck130 HIGH-2).

## Carried, untested (no window this corpus)
C6, R3, R4 (infect), R7; F2 decline path; B1 fold. Owner-held: D42 design, D13 log window.
