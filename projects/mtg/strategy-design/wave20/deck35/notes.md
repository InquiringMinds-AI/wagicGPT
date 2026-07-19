# deck35 -- development notes (engine / harness / model) + rotation verdict

## Engine / representation ledger

1. **[ENGINE BUG -- CONFIRMED, now 3-WAVE convergent -- HIGHEST PRIORITY, ready for targeted fix]
   Declare-attackers decision is NOT issued to the GPT seat on some own-turns despite eligible
   untapped, unsick attackers.** This is the seat's #1 result-affecting item and the corpus's most
   important engine finding. Reproduced a THIRD wave, and this wave it also REFUTES the wave-20 brief's
   payment attribution.
   - **vs49 (LOSS, my -2 / opp 20) -- COMPLETE 0-attacker, 0-combat-damage game.** ZERO `attackers`
     decisions all game (jq `.kind=="attackers"` count = 0); opponent life never left 20. On **T8**
     (seq10/11) the board was three unsick untapped flyers (Gemhide cast T4, Winged cast T6, Striking
     cast T6, all rendered `[flying, first strike]`, none `[tapped]`) with an explicit plan "attack
     with my full flying swarm"; on **T6** (seq6-8) Gemhide was unsick + untapped. No attack step
     either turn. File: `1784502245-ai_baka_deck35-0x563bbaee8920-vs-ai_baka_deck49.jsonl`.
   - **vs14 (WIN):** Striking (cast T1) eligible on T3/T5/T7 -> no attack step; first attack T9 (seq17).
     File: `1784492337-ai_baka_deck35-...-vs-ai_baka_deck14.jsonl`.
   - **vs62 (WIN):** Gemhide (cast T4) eligible on T6/T8 -> no attack step; first attack T10 (seq16).
     File: `1784502179-ai_baka_deck35-...-vs-ai_baka_deck62.jsonl`.
   - **CRITICAL cross-check with the wave-20 engine batch:** item (b) "attacker-sparing mana payment"
     attributed the wave-19 durdle to PAYMENT (auto-tapping Gemhide). That fix WORKS (vs49 seq8: Winged
     paid with two lands, Gemhide left untapped) but did NOT remove the 0-attacker games. The would-be
     attackers were UNTAPPED, so payment was never the cause here. **The root cause is combat-gate
     non-issuance (this ledger item), which the payment fix does not touch.** Do not close this on the
     strength of the payment fix.
   - **Sharpened (partial) hypothesis for engine to test first:** the attack step is reliably OFFERED
     when (a) a HASTE creature is present (Heart Sliver -> vs102/vs135/vs27 attacked every turn), or
     (b) the model explicitly chooses "Cast nothing right now" (vs102 seq22 -> seq23 attack offered);
     and is SKIPPED when the model casts its LAST affordable spell (taps out / auto-ends the main
     phase) with all attackers non-haste (vs49 all turns, vs14 T3/T5/T7, vs62 T6/T8). Anomalies remain
     (vs27 T5, vs14 T9 attacked after a cast with no haste), so it is a correlate, not a proof.
     **Where to look:** the phase transition from main-phase-1 to combat for the AIPlayerGPT seat --
     specifically whether reaching end-of-castable auto-advances past the declare-attackers offer when
     no just-cast creature is itself a legal attacker (i.e., the combat step is only entered when the
     *last resolved* action leaves an attack-legal creature, missing pre-existing unsick bodies).
     Confound (unchanged): the translog cannot show the engine's eligibility/phase computation.

2. **[REPRESENTATION -- attacker-sparing mana payment (wave-20 item b) -- VALIDATED WORKING, CLOSE OUT
   as a payment fix]** The engine now prefers swing-neutral sources: vs49 seq8 cast Winged {1}{u} by
   tapping Forest+Island and SPARING the Gemhide (left untapped/attack-eligible). Corpus-wide, no
   Sliver was ever rendered tapped-for-mana with lands available; every tapped Sliver was post-attack
   or an edict target. The auto-tap-the-attacker hazard (my wave-19 ledger #3 watch) is CLOSED. Note
   for the ledger: this fix is correct and confirmed, but it must NOT be recorded as resolving the
   0-attacker durdle (that is ledger #1 and remains open).

3. **[REPRESENTATION -- block-seam trade annotations -- remains SHIPPED + VALIDATED, stays closed]**
   Per-blocker "you kill it, your blocker lives" tags again drove correct selective blocking (vs14
   seq22 free profitable block; 6x correct no-block). No misleading annotation at my seat. Already
   retired in wave-19; reconfirmed, no action.

## Model / decode
- **Decode-repetition tax (route to sampler, not guide):** vs14 seq19 (14,078 chars) and vs62 seq7
  (12,809 chars) are repetition loops that still parsed. 0 fallbacks this wave is real but FRAGILE.
  See general-suggestions.md G1 -- recommend a decode-time repetition penalty / max-token guard.

## Deck-construction observations (recorded; not my call to change)
- **Blue availability + a HASTE enabler are the two swing variables.** Heart-present games attacked
  every turn AND dodged the non-issuance bug; no-Heart games skipped attack windows. Running more haste
  (only 2 Heart Slivers in a 33-card nonland set) would both improve tempo and reduce the deck's
  exposure to ledger #1. Construction signal, not guide-fixable.
- **Shifting Sliver (the premier win-con) has now gone undeployed FOUR full corpora** -- {3}{u}, blue
  scarcity. Never provably a misplay (never castable when relevant) but the deck's best card never hits.

## Rotation verdict
**FREEZE the guide; do NOT rotate the seat.** Two verdicts, stated separately (skill.md S3):
- **Guide = FREEZE, byte-identical.** No decision-evidence warrants a change: FC1 durdle dead 3rd wave;
  land-drop-discipline revision VALIDATED (vs135 land drops now short/committed, 0 fallbacks); the
  three losses decompose to ENGINE (vs49) + RACE (vs27, vs135), none guide-fixable; the residual tax is
  a decode-repetition issue routed to the sampler, not the guide. strategy.txt this wave == deployed.
- **Seat = KEEP (not a rotation candidate).** Strong live new-work signal: the attackers-non-issuance
  bug reproduced a 3rd wave AND this seat produced the refutation of the wave-20 payment attribution --
  the corpus's most important engine finding. The next corpus's exit questions: (1) does an actual
  non-issuance fix land and do the vs49-class 0-attacker games disappear? (2) does the decode-repetition
  tax convert any 14k near-miss into a real fallback? (3) does the payment fix stay clean (no
  regressions to tapped attackers)?

## Engine items count: 3 (item 1 open/HIGHEST -- 3-wave convergent; item 2 validated-working/close as
## payment fix but keeps #1 open; item 3 reconfirmed-closed).
