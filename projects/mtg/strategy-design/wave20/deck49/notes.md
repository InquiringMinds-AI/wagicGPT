# notes.md — deck49 (Dragons) engine/representation ledger + rotation verdict (wave 20)

## Engine / representation ledger items (status after this corpus)

### E-49d (NEW, HIGH) — reasoning-tax truncation at an unadjudicated fork -> unparsed_reply x3
The two-wave zero-fallback streak broke: 3 unparsed replies, ALL ask seq 8, same develop seam (t4-5,
~20/17, lone-1/1 board), vs deck27/62/35. Two components with different owners:
  - DECODE/HARNESS (this ledger): deck27's reply is a decode-REPETITION loop (one paragraph repeated
    ~10x verbatim, truncated mid-word). A decode-time repetition penalty / n-gram early-stop would end
    it before the token cap. deck62 is a PROGRESSING spiral (advancing, not repeating) — a decode
    repetition guard will NOT catch it; only the guide tiebreak + a general commit-brake reach it.
    deck35 is a PLAN/CHOICE mismatch: `CHOICE: 5 (Cast Dragonmaster Outcast)` emitted to a 2-option
    LAND-DROP menu (out-of-range index, name matches no option) — the parser CORRECTLY rejected it
    (nothing valid to salvage). Note deck35's prompt `YOUR PLAN` field was contaminated with the
    model's prior casting-menu deliberation echoed back — a possible prompt-assembly hygiene item
    (a stale sub-menu leaked into the plan field).
  - GUIDE (handled in strategy.txt, not here): the fork itself — competing "burn goes face" vs
    "deploy every turn" vs an over-broad "key blocker" exception. Dissolved by the two constrained-diff
    edits this wave.

### E-49e (NEW, MEDIUM) — loop-salvage must NOT be extended to unresolved ask/target deliberations
The wave-20 batch extended salvageLoopedChoice to BLOCKS/ATTACK/PUT. It did NOT (and should NOT) fire
on deck27 seq8: the loop's repeated intent was "bolt the Zombie Outlander" (a creature), which was the
WRONG resolution — the fallback + follow-on sent the burn to the FACE. Salvaging the loop would have
committed the play the model talked itself OUT of. RECOMMENDATION: keep loop-salvage scoped to
format-mangled SINGLE intents; do NOT teach it to pick a "winner" from an oscillating two-aim
deliberation. The right levers are the decode repetition guard (E-49d) + the guide fork-dissolution.

### E-49f (NEW, LOW/observation) — a stolen threat is not a durdle (Persuasion at deck14)
deck14: deck49's Thunderbreak Regent was under OPPONENT control (Persuasion aura) t10-12, so its
"missing attacks" are correct, not a durdle or attackers-non-issuance. No engine defect — logged so a
future reviewer running the 0-attacker instrument adds "is the creature still yours?" to the
disqualifier check. Control auras (Persuasion) transferring a Dragon also do NOT trigger Thunderbreak's
"opponent targets a Dragon -> they lose 3" punisher on the STEAL in this corpus — worth a spot-check
against Oracle if it recurs (Persuasion targets on cast; the punisher should have fired). Queue, low.

### E-batch20 confirmations at this seat (confirm, keep)
- Own-only War Paint annotation ("the only legal targets are YOUR OWN right now") LIVE and used
  correctly (deck62 seq5 cast safely). The enemy-only + own-only warning pair now both validated.
- Attacker-sparing payment (deck35 owns the check): at THIS seat, no game showed a would-be attacker
  tapped for mana with lands available — deck49 pays off Mountains, so the seam does not stress here
  (deck49 has no creature-mana). Nothing to add.
- Reveal partition / #N ordinals / discard-pick surfaces: N/A at deck49 (no reveals, no duplicate-name
  combat boards this corpus).

### Card-script divergences (Oracle-verification queue) — UNCHANGED, still queued
Standing list holds (Bloodmark Mentor engine 1/1 vs real 2/1; War Paint engine {1}{R}/+2+2+haste vs
real {R}/+1+1; Crucible engine {3}{R} vs real {2}{R}{R}; Ashenmoor Liege engine 4/1 vs real 4/4 — the
guide describes ENGINE values by design). No card mis-executed in a game-losing way this corpus. Queue,
do not rush. NEW low item: the Persuasion/Thunderbreak-punisher interaction (E-49f).

**Engine items this corpus: 3 NEW (E-49d decode/harness, HIGH; E-49e loop-salvage scope, MEDIUM; E-49f
stolen-threat observation, LOW) + batch20 confirmations + standing Oracle queue (unchanged).**

## Rotation verdict — HOLD EXIT ANSWERED; hold ENDS; deck49 CONTINUES (does NOT rotate out)

**HOLD EXIT QUESTION (binding, answered explicitly): YES — the flood/no-curve off-case WAS exercised
this corpus.** deck27 was the pure off-case game (opening 3 Mountain + Bloodmark Mentor 1/1 + Lava
Spike + 2 Lightning Bolt, no dragon; the condition held all game). The off-case's prescription was
FOLLOWED — every burn went at the face, none held (Lava Spike t1 20->17, both Bolts t5 17->11), chipping
opp to ~10 with burn + the lone 1/1 exactly as the off-case directs. The loss traces to CONSTRUCTION
(ran out of burn / no reach, G-49d), NOT the off-case. So the off-case is now VALIDATED on behavior and
the wave-19 open thread is DISCHARGED. (One wart: at deck27 seq8 the model agonized face-vs-blocker
before the burn reached the face — that is the burn-exception over-breadth leak, fixed in strategy.txt,
not the off-case failing.)

**Rotation decision: END the hold, but deck49 does NOT rotate out — it re-enters as a normal
revised-guide seat for one validation cycle.**
- The hold's SOLE purpose (exercise the off-case) is discharged, so the hold ITSELF ends.
- But the corpus produced a GUIDE-MOD (two decision-evidenced constrained-diff edits: tighten the
  over-broad key-blocker exception + add the develop-seam commit/either-is-fine tiebreak) AND a
  new-work signal (E-49d/E-49e). By the brief's own rotation test (no-guide-mod AND no-new-work-signal
  = rotation candidate), deck49 FAILS BOTH conditions this wave — it has a guide-mod and open engine
  work. It is therefore NOT a rotation-out candidate; it needs one corpus to validate the new edits.
- strategy.txt is REVISED (md5 479f907edbbc57e76c02bfaab8ba12d9; was a7d4813a51e89069222a2299b3fbafde).
  The diff is exactly two hunks; every other line is byte-frozen. FREEZE-CHECK passed: Rule #1
  mana-trivial, burn-DEFAULT-face, NEVER-0-power-wall, War-Paint-own, DEPLOY-AND-ATTACK, the off-case,
  and MULLIGAN all survive verbatim; only the key-blocker exception clause changed and one develop-seam
  paragraph was added.

**Recommendation to synthesis:** deck49 is a REVISED seat this cycle (not converged, not rotation-out).
Next corpus, validate: (1) did the develop-seam fallbacks at seq-8-class DIE (target metric: 0 unparsed
at the early Main-1 develop seam)? (2) did burn-face rate hold and the key-blocker tightening avoid
over-generalizing into "never kill any blocker"? (3) does the "either is fine, commit" line avoid the
opposite failure (a blind creature-cast when burn was lethal)? If those hold with 0 fallbacks, deck49
returns to converged/rotation-candidate. Judge by taught-class survival + tax, not the record (rule 1).
