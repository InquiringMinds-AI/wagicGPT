# deck35 -- development notes (engine / harness / model) + ROTATION VERDICT

## Engine / representation ledger

1. **[ENGINE -- partial attacker-sparing payment fix -- VALIDATED CLEAN, item CLOSED.]** I own this
   validation. Wave-20's sparing was all-or-nothing (spared all would-be attackers iff swing-neutral
   sources covered the WHOLE cost, else tapped them all), so the turn's last cast tapped the attackers
   -- the true mechanism behind the "attackers non-issuance" symptom I escalated for 3 waves. The gate
   was always correct. This wave's partial sparing (attackers drawn into the bill weakest-power-first
   only while still short, X-spells excluded) is confirmed working:
   - **The 0-attacker games are GONE.** vs49 (wave-20's complete 0-attacker loss) now issues an attack
     (T5, seq5, Heart). No game this corpus is attacker-starved for a payment reason.
   - **No Sliver tapped for mana with a land available**, across 6 games. Positive stress-test:
     **vs14 T8 (seq13->14)** cast Might {4}{g}=5 mana with all 5 lands tapped, leaving all three
     attackers (Winged/Muscle/Gemhide) UNTAPPED, and attacked with all three. That is the sparing
     guarantee under a full-mana 5-drop -- the exact case pre-fix would have tapped an attacker.
   - **Every no-attack own-turn is correct:** all-summoning-sick board (vs49 T7, creatures:0 after Heart
     died) or a FORCED tap of the lone mana-dork Gemhide when lands were short (vs27 T4: two Muscles =
     4 mana, 3 lands; vs27 T6: Bonesplitter {3}{r}=4, 3 lands). Forced payment, per spec, not a
     violation. The translog cannot show the engine's payment computation (standing confound, unchanged)
     -- corroboration is the `[tapped - untaps and can attack next turn]` render on Gemhide (vs27 seq9).
   **My wave-20 "combat-gate non-issuance" root-cause hypothesis is SUPERSEDED by the payment reframe.**
   I state this as a correction, not an additive note: the symptom was real and 3-wave convergent, the
   mechanism I named was wrong, and the post-fix corpus settles it. Close item 1.

2. **[HARNESS -- answer-first + parser: VALIDATED, one watch item.]** 87/87 replies at this seat lead
   with the coded line. The wave-20 fragile 14k near-misses now PARSE CLEAN (coded line first survives a
   truncated tail): the four longest replies (13,108 / 12,855 / 12,559 / 12,116 chars) all parsed to a
   correct choice, including a 12.5k-char reply on a LAND DROP (vs62 seq17) that committed
   `CHOICE: 1 (Play Mountain)` on line 1. No conversion to fallback from length.
   - **WATCH (single-seat, low-stakes):** the ONE fallback (vs62 seq18, `retracted_choice`) is a
     commit-then-reason-to-a-different-answer case: line 1 `CHOICE: 3 (Cast Heart Sliver)`, then a
     Heart-vs-Fury spiral, PLAN concludes "Cast Fury". Parser correctly detected disavowal and routed to
     heuristic (choice=-1); the heuristic cast Fury -- the model's actual final intent and the
     max-damage play in a lost position. Fallback CORRECT, no harm. Repro: seq18 in
     `1784814583-ai_baka_deck35-...-vs-ai_baka_deck62.jsonl`. The residual gap: when answer-first's
     line-1 goes stale vs a PLAN that states a clear single card, a **prose-intent salvage reading the
     PLAN line** would recover the intent deterministically instead of relying on the heuristic argmax
     matching. Combat prose-salvage already exists (item d in the batch); this is the priority/casting
     analogue. NOT a change request -- one instance, correct outcome; flagged for the synthesis owner.

3. **[MODEL/DECODE -- reasoning-tax repetition spirals persist as a TOKEN/LATENCY cost, no longer a
   correctness cost -- route to a decode-time guard, see general-suggestions G1.]** The tax did not
   leave; it is defused. 12-13k-char replies still occur (incl. on land drops the guide says to answer
   in one line), with verbatim repetition tails ("I die." x4). Answer-first removed the parse risk;
   the durable fix for the token/latency cost is a decode-time repetition-penalty / max-token guard
   (repetition_penalty shipped as a config key but is OFF this corpus). Cross-deck, not seat-specific,
   not guide-fixable.

## Deck-construction observations (recorded; routed to the roster owner; not my call to change)
- **A haste enabler still does double duty**, but the reason changed: with the payment fix, Heart no
  longer "dodges a bug" -- non-haste boards now attack reliably too. Heart's value is back to pure tempo
  (attack the turn a body lands). Unchanged construction note: only 2 Heart Slivers in the nonland set;
  more haste = more turn-3 starts. Lower priority now that the engine no longer punishes no-Heart draws.
- **Shifting Sliver (the premier win-con) deployed this wave** (vs14 seq18, vs62 board) -- first
  appearances in several corpora. No longer "never hits." No action.
- **The Gemhide-as-attacker tension:** a lone mana-dork Gemhide gets tapped out of an attack whenever
  lands are short and the model develops another spell (vs27 T4/T6). Inherent to a mana-dork body;
  not guide-fixable and not worth a line. Lens only.

## ROTATION VERDICT -- deck35 is now a ROTATION CANDIDATE (stated honestly, per the standing test).

Two verdicts, separately:
- **Guide = FREEZE, byte-identical** (md5 `2cc75f921ab40a6e2b51fd3a666ebbd1`). No decision-evidence
  warrants a change: attack-every-turn validated 4th wave; land-drop discipline holds (correct instant
  land choices even inside the long replies); FC1 chump-durdle dead; the three losses decompose to
  RACE (vs49, vs62) + DRAW-SCREW (vs102), none guide-fixable; the residual is a cross-deck decode-tax
  routed to the sampler.
- **Seat = ROTATION CANDIDATE.** My wave-20 keep-reason was explicit: "KEEP -- the open non-issuance
  work; this seat is the primary witness to the corpus's most important engine finding." **That work is
  now DONE.** The payment fix validated clean, the 0-attacker games are gone, the reframe is confirmed,
  and my prior root-cause hypothesis is settled/superseded. The remaining residuals are all NON-seat:
  the answer-first stale-line watch (item 2) is a general harness observation, and the decode-tax
  (item 3) is cross-deck. There is **no live engine work-signal that requires this deck's shape to
  surface.** Per the standing rule (payment fix validates clean + guide frozen + no new work signal ->
  rotation candidate), I call it honestly: deck35 has converged and can rotate out, its frozen guide
  staying deployed as a canary. If synthesis wants a second-corpus confirmation of the payment fix
  under a heavier no-haste board than vs14 provided, one more freeze cycle is the only reason to hold;
  otherwise, rotate.

## Engine items count: 3 (item 1 payment fix VALIDATED-CLEAN/closed; item 2 answer-first
## VALIDATED with one harness watch; item 3 decode-tax open/cross-deck at the sampler layer).
