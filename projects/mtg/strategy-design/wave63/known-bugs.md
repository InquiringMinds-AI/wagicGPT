# Wave-63 step-one input: KNOWN BUGS from the wave-62 evaluation (orchestrator list; details in the cited files)

Corpus: `matchups-20260905-133058` (20 natural + 1 HUNG 152v125, fixed 9230a94c3, `wave62/corpus-hang.md`) + rerun
`matchups-20260905-161801`. Sources: `wave62/deck{146,152,125,126,162,123,130}/review.md`, `wave62/engine-seat.md`.
Adjudication: 18 PASS, 5 FAIL (D1-p3, D19, D5, R1, D13), 8 UNTESTED (D2, D6a, D9, D11, R2, R5, R6, R7/R8).
Numbers below are the seats' own; cite their files. Owner-held: D42 design (rulings in wave58/owner-decisions.md).

## HIGH — decisions lost to a false or missing claim
- **E1 Life loop on the BLOCK side (deck123 HIGH-1; D2 was attack-side only).** 123v126 seq 76: blockers header
  `best case with every blocker assigned: you would be at 16`, B rows `they gain 1 from this block only` beside
  `(you kill it, your blocker lives)` — with Sanguine Bond + Exquisite Blood on their side. Seat blocked; seq 78 =
  eleven lose-1/gain-1 pairs, 16 -> 0 from a won board (83 blockers vs one 1/1); `BLOCKS: none` ends at 15. Apply
  D2's fail-closed rule (no survival claim, name the loop) to the blockers header and every B-row lifelink price.
- **E2 CLEANUP PRICE undercounts (deck125 HIGH-1).** 125v162 seq 89: Revelation row priced "up to 7 discards ...
  at worst you would be at 4" while the same prompt's ON THE STACK listed four pending draw triggers it did not
  count, floor computed off PRE-cast life; seq 94 asked 10 discards / 20 life (25 -> 5). Fold stacked draw triggers
  and the cast's own life delta into the price.
- **E3 Survivable-block suggestion is life-maximising only and the model copies it (deck152 HIGH-1/2).** 146v152 seq
  25: header named `Wolf blocks Barrowin` (a chump) while the same Wolf line offered two `(you kill it, your blocker
  lives)` results; seat did the chump, lost the Wolf, seq 28 had no survivable line. On a lethal screen the header
  withdraws entirely and the unguided seat picked the damage-MAXIMISING block. Rank the suggestion: survive AND
  keep material > survive; on a lethal screen still name the least-damage line.
- **E4 GANG BLOCK verdict buried behind friendly per-blocker results (deck162 HIGH; deck146 HIGH-2).** 162v146 seq
  16: `- GANG BLOCK:` sat after three "your attacker lives" parentheses on an uncollapsed tag; the reply reasoned
  the gang block out loud and attacked anyway (opp at 1). Attack A-lines price `(blocking trigger:` per lone block
  and never say prices SUM (146v126 seq 32: worst tag 5, three walls gang-blocked, opp gained 9). Hoist the verdict
  to the front; sum blocking-trigger prices.
- **E5 Manland listed once per affordable rung (engine HIGH-1; deck130 second HIGH).** ONE Lair of the Hydra reads
  five times in CRACK-BACK (130v152 seq 17-19; `AIPlayerGPT.cpp:18317-18330` appends per script line) — lane Y's
  D4 per-ability keys leaked into lane X's walk; the animate clause prices nothing (MED-10) and misses the comma
  power form; Lair's own rung list stops one short (LOW-11). Collapse per permanent; price the best rung.
- **E6 Last-coded-line latch is unauditable and sometimes wrong (deck146 HIGH-1; deck126 HIGH-1; deck152 note).**
  `answer_replaced` overrode the model's first `CHOICE:` from a coded line inside trailing CoT (146v162 seq 41,
  `latched_coded_line: 2`, 2,803 B of CoT) and `reply_trimmed_bytes` cut the deciding line OUT of the record.
  It won games (152v162 seq 34) and lost windows (126: 7 latches, 2 dropped Staff pings). Record the latched
  line verbatim regardless of trim; decide whether a line inside the plan/CoT paragraph may override at all (D9/D10
  bound — check whether these fell inside `planParagraphBound`); reply protocol absent from every logged prompt
  (engine HIGH-7) — the system message must be in the record or D9 can never be adjudicated.
- **E7 ANNOUNCE_X Decline row is state-dependent (engine HIGH-5; D5/R1 FAIL).** 17/18 asks carry it; one 4-row menu
  after mana was committed does not. Make the decline unconditional on the ask, or state why it is absent.
- **E8 Resolved land-drop verdict names a wrong witness (engine HIGH-3; D1-p3 FAIL).** `enters UNTAPPED: you control
  X` where X lacks the named subtype. Fix the witness search (subtype vs name), PARSETEST the shape.
- **E9 D19 relief clause never fires (engine HIGH-4; D19 FAIL)** — 0 occurrences corpus-wide including its own two
  repro windows; the gate `crackBackNextTurnDue` is wrong or the rows never carry the verdict emitter.
- **E10 Hold promise broken 15x at one seam (engine HIGH-2; D13 FAIL; deck130 MED; deck125 MED).** A hold in main 1
  is void in main 2 (only `(combat comes next this turn)` differs on the pass row); hold row offered 459x taken 6,
  declined-N tag 204x held 4 — the latch is nearly inert. Scope the promise to what the latch actually honours.
- **E11 `reveal` records carry a constant `choice: 1` and a false parse stamp (engine HIGH-6).**
- **E12 Card script: Nadaar, Selfless Paladin pumps itself (deck130 HIGH, card).** borderline.txt:75387
  `lord(creature|myBattlefield)` vs Oracle "Other creatures" — verify-Oracle rule; fix the script, not the engine.

## MED
- E13 DRAW PUNISHERS board paragraph still carries D3's direction error (engine MED-8); "N life to you from your
  punishers" reads as gain (MED-9); `{feeds:}` converters vs discard punishers invite conflation (deck162 MED).
- E14 Sorin +1 every turn for 18 turns while `-2: emblem` sat on the menu — guide fixed (deck126); the plan carry
  asserted exiled permanents for eight turns (deck126 HIGH-1 second half; PLAN as scratchpad, D16(a) bound).
- E15 `mana of ANY colour pays a generic cost` clarifier renders 1922/1922 on nonzero lines and 0/177 on zero-mana
  lines (deck123) — preceded a 7,680 B reply concluding the deck uncastable.
- E16 `{spare:}` never tags a land while Emrakul is in hand (deck125 MED).
- E17 `intrepid_adversary_repeated_payment.txt` is a genuine intermittent (fails 1-2 of 3 solo at THREADS=1 on
  66afd72bb AND caa5d6ea3) — find the nondeterminism; lane T/AA left it undiagnosed.
- E18 D7's `{paying this taps:}` half never had a window; repeat-row grind is a real cost (engine LOW-12).

## Carried, untested
D2 (attack-side loop; E1 is its block-side twin), D6a, D9, D11, R2, R5, R6, R7/R8; C6, R3, R4, R7 from wave 61.
