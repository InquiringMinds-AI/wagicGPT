# skill.md — deck49 (Dragons) wave-20 contributions (REVISED-GUIDE, ROTATION-HOLD)

deck49's third guided corpus is the worked example of a two-wave zero-fallback streak breaking at a
SINGLE decision seam, and of the diagnostic that names the cause. Proposed additions/confirmations
(single-seat unless noted). Single-seat cannot add/cut skill CORE lines — these are method rungs and
a deck-local guide diff.

## NEW rung (proposed) — a COMPETING-IMPERATIVE FORK the model CITES is a guide bug, and it fails as a TRUNCATION, not a wrong choice

The sharpest instrument this wave. deck49's streak broke via 3 unparsed replies ALL at the same seam
(early Main-1 develop turn, seq 8, ~20/17, lone-1/1 board), vs three opponents. The mechanism was NOT
a wrong pick — it was the executor UNABLE TO COMMIT because the guide handed it two co-firing
imperatives with no tiebreak, and it deliberated past the token cap. Method points:

1. **When a reply LOOPS or SPIRALS to truncation and QUOTES two guide sentences that pull opposite
   ways, the guide caused it (Step-0 echo test).** deck27 seq8: the model quotes "DEFAULT: aim ... at
   THE OPPONENT" and "aim at a CREATURE ... a key blocker walling your whole board" and oscillates ~10x
   verbatim between them until it truncates mid-word. The fix is to REMOVE THE FORK (tighten one clause
   so the two stop competing), not to add a third rule.
2. **A truncation-unparse is a REASONING-TAX failure at a fork, distinct from a wrong-choice failure.**
   Bucket by mechanism before proposing a fix (wave-19 synthesis): deck27 = decode-REPETITION loop
   (route the truncation to a decode repetition-penalty / max-token guard — NOT guide); deck62 =
   PROGRESSING spiral (a guide tiebreak CAN reach it); deck35 = PLAN/CHOICE mismatch (out-of-range
   index on the wrong menu — reply-protocol/parse). Three surfaces, ONE trigger. The guide can only
   lower the TRIGGER frequency; the loop tendency is CORE/decode.
3. **Resolve the fork by KILLING the false branch, not by legislating a universal winner.** For
   deploy-vs-burn (deck62/35) BOTH answers won on the follow-on (deck62 burned face and won; deck35
   deployed and won), so mandating one risks harm + weak-executor over-generalization (the brake-
   absolutism trap). The right move is "either is fine, pick one, COMMIT" + a soft board-state lean —
   which RETROACTIVELY MATCHES the winning follow-on plays in all three games (a good soundness check
   on a tiebreak: does it agree with what the recovered turns actually did?).
4. **Tighten an over-broad EXCEPTION against the degenerate board it fires on.** "A key blocker walling
   your whole board" reads as ANY blocker when your board is a lone attacker. Scope it to the board
   state where it is actually wrong: a small creature that merely TRADES with one attacker is not a
   "key blocker." Same family as the wave-9 own-targets annotation and the brake-scoping lessons —
   name the state, not the class.

## Loop-salvage caution (proposed rung) — do NOT salvage a genuinely-unresolved deliberation loop

deck27's looped intent ("bolt the Zombie Outlander" — a creature) was the WRONG resolution; the
fallback + follow-on decisions sent the burn to the FACE (the right one). A loop-salvage that committed
the repeated intent would have LOCKED IN the play the model talked itself OUT of. Lesson: loop-salvage
is safe for a FORMAT-mangled single intent (the BLOCKS/ATTACK/PUT shapes it was built for), but NOT for
an ask/target loop that is still oscillating between two AIMS — the repeated line is the losing side of
an unresolved argument, not a mis-serialized decision. Prefer decode-side dampening + the guide tiebreak
over teaching salvage to guess the loop's "winner."

## CONFIRMED rungs (existing doctrine, re-proven)

- **(rule 1) read the tax collapse, not the record.** Record 5/6->4/6 is POOL (deck27/deck14 flipped
  L; both construction/matchup losses). Judge by taught-class survival: burn-face ~16/16 (0
  creature-targeted, cleaner than wave-19's 15+1), War-Paint-own 3/3, Crucible-needs-dragon fired as a
  correct desperate decline (deck14 seq25/26), deploy-and-attack obeyed. The guide's JOB held; the
  streak broke on the develop-fork tax, not on a taught class regressing.
- **before scoring a 0-attacker window as a durdle, run the eligibility-and-control check (wave-19
  headline 2, extended).** deck14 had a 4/4 Thunderbreak "idle" t8-12 while opp sat at 8 — looks like a
  durdle. It was neither durdle nor engine non-issuance: the opponent STOLE it with Persuasion (control
  aura). The disqualifier list grows a THIRD item: (a) eligible untapped/unsick attacker? (b) attackers
  decision issued? (c) is the creature still YOURS? A stolen/controlled threat is not an attack you
  failed to make.
- **an ANNOTATION carries only the case it renders (re-confirmed, own-only side).** The E-49c family
  now has BOTH warnings live: the enemy-only "this would help their side" AND the own-only "the only
  legal targets are YOUR OWN right now" (deck62 seq5, the model cast safely under it). The mixed-menu
  case still has no annotation and is still carried by the guide's never-aura rule (deck14 seq9, deck35
  seq11 both chose own on a mixed menu) — still load-bearing, still NOT a trim candidate.

## WATCH discharged — the off-case UNEXERCISED thread from wave-19 is now EXERCISED

Wave-19 filed the flood/no-curve off-case as an OPEN validation thread ("never fired; a winning corpus
never floods"). Wave-20 exercised it (deck27: sustained lone-1/1 + burn + no dragon) and the off-case
behavior PASSED (all burn to face, none held; the loss was construction/no-reach). The doctrine holds:
an off-case authored proactively DOES eventually get a live test — track it as open until the condition
recurs, then discharge on behavior. The wart it surfaced (the model can still agonize face-vs-blocker
inside the off-case) is the competing-fork rung above, not an off-case failure.
