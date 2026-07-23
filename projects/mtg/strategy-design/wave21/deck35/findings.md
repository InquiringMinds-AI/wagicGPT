# deck35 (Mighty Slivers) — wave-21 findings — FROZEN guide, 4th guided corpus

Seat: deck35, GUR Sliver lord-stack swarm-race. AGGRO. Guide **FROZEN, byte-identical** to the
deployed file (md5 `2cc75f921ab40a6e2b51fd3a666ebbd1`, wave20/deck35/strategy.txt ==
bin/Res/ai/baka/deck35_strategy.txt == this wave's strategy.txt). Binary 3be7f5a67. Corpus
matchups-20260723-084938. Record **3/6** (flat vs wave-20).

WON: 135, 14, 27.  LOST: 49, 62, 102.  (Opponent pool repeated 7-of-7; the *which*-games flipped:
vs27 L->W, vs135 L->W, vs62 W->L, vs102 W->L, vs49 L->L, vs14 W->W. Slate-variance, decomposed below.)

**I OWN the partial attacker-sparing payment fix.** Verdict up front: **VALIDATED CLEAN. The fix
works, the 0-attacker games are GONE, and the wave-21 payment REFRAME is confirmed — it supersedes my
own wave-20 "combat-gate non-issuance" root-cause hypothesis.** One fallback at this seat (vs62 seq18,
`retracted_choice`) — classified below, was CORRECT and did not hurt. Answer-first: 87/87 replies lead
with the coded line; the wave-20 fragile 14k near-misses now PARSE CLEAN.

---

## HEADLINE 1 — PARTIAL ATTACKER-SPARING: VALIDATED CLEAN across all 6 games. Zero counter-examples.

Three sub-checks from the brief, each PASSED:

**(a) On turns where the seat cast its last affordable spell with non-haste attackers up, was the
attack window offered and taken?** YES, every time an attacker ended the turn untapped. The cleanest
proof is a NO-HASTE deck (no Heart on board), where pre-fix the last cast tapped the would-be
attackers:

- **vs14 (WIN, no-haste, flyers) — textbook partial sparing over four turns.** Each own-turn the
  just-cast body is summoning-sick and correctly excluded; the PRIOR unsick bodies attack; NONE was
  tapped for mana:
  - T4 (seq6->7): cast Muscle {1}{g}, paid two Forests, **Winged left untapped** -> attacked Winged (seq7).
  - T6 (seq10->11): cast Gemhide {1}{g}, paid lands, **Winged+Muscle untapped** -> attacked both (seq11).
  - T8 (seq13->14): cast **Might {4}{g} = 5 mana with all 5 lands** (Forest/Island/Forest/Mountain/
    Mountain all `[tapped]`), leaving **all three attackers (Winged/Muscle/Gemhide) UNTAPPED** -> attacked
    all three (seq14). This is the partial-sparing guarantee under maximum mana pressure: a 5-drop paid
    entirely from lands, three Slivers spared.
  - T12 (seq18->19): cast Shifting {3}{u}, paid 4 lands, **Bonesplitter untapped** -> attacked (seq19).
- **vs62 (WIN->LOSS but attacked every eligible turn, no-haste):** T5 cast Horned, Two-Headed spared
  untapped -> attacked (seq7); T7/T9/T11 same pattern, attacked every own-turn (seqs 11/15/19).
- **vs135 (WIN, haste):** attacked every own-turn (seqs 5/9/12/15), the swarm growing 1->5 wide.
- **vs102 (LOSS, screwed):** lone Striking (first strike, NO haste, cast T0) attacked T2/T4/T6
  (seqs 5/6/7) with no cast those turns — confirms the step fires for a lone unsick non-haste body.

**(b) Any Sliver tapped for mana when a LAND could have paid?** **NONE found in 6 games.** The engine
prefers lands: vs14 T8 paid a 5-drop from 5 lands and spared 3 attackers (above). The ONLY Slivers ever
tapped for mana were in vs27 (see Headline 3), where lands were genuinely SHORT (3 lands vs a 4-drop),
forcing the lone mana-dork Gemhide into the bill — a forced payment, NOT a land-could-have-paid
violation.

**(c) New pathology (a castable spell declined because too much was spared; partial-sparing splitting
payments weirdly)?** **None observed.** No cast was refused/failed for over-sparing; no split-payment
artifacts. Every offered cast completed.

**(d) The vs49 rematch (wave-20's COMPLETE 0-attacker loss): does it now attack?** **YES.** vs49 this
wave issued an `attackers` decision on **T5** (seq5, Heart Sliver, haste). The wave-20 signature — ZERO
`attackers` decisions all game with an untapped eligible board — **does not recur.** (Detail: I lost the
rematch anyway to a fast Dragons clock — mulliganed to a slow hand, first creature T5, Heart traded into
a Firespitter Whelp chump-block [seq6 events: Heart dealt 1, took 2, died], opp life stayed 20, I died
-1 on T8. RACE/matchup loss, not engine. See Record decompose.)

---

## HEADLINE 2 — the wave-21 REFRAME is confirmed; it supersedes my wave-20 hypothesis (honest update).

My wave-20 notes escalated the 0-attacker games as a "combat-gate non-issuance" engine bug (3-wave
convergent) and explicitly REFUTED the wave-20 brief's payment attribution, on the evidence that the
would-be attackers rendered UNTAPPED. The wave-21 reframe — **the gate was always correct; wave-20's
sparing was all-or-nothing (spared every would-be attacker iff swing-neutral sources covered the WHOLE
cost, ELSE tapped them all), so the turn's last cast tapped the attackers** — is now VALIDATED by
behavior:

- The fix changed the outcome exactly where the reframe predicts: vs49 0->1 attacks; vs14 clean
  multi-turn sparing that pre-fix would have tapped Winged/Muscle on the last cast.
- No non-issuance recurs at any turn where an attacker ends untapped.

Reconciliation of my wave-20 "untapped" read: the ask-time battlefield render (the only snapshot the
translog exposes) showed the attackers untapped BEFORE the turn's last cast resolved; under all-or-nothing
sparing that last cast then tapped them at/for the final payment, which the pre-combat render did not
reflect — the translog cannot show the engine's payment computation (the standing confound, unchanged).
The empirical fix behavior is the arbiter, and it matches the payment reframe. **My wave-20 combat-gate
root-cause hypothesis is SUPERSEDED, not merely additive** — I state that plainly. This is the payoff of
the wave-19/20 model-vs-engine instrument working in reverse: it flagged a real symptom, mis-attributed
the mechanism, and the post-fix corpus corrected the attribution cleanly.

---

## HEADLINE 3 — the only no-attack own-turns are correct: all-sick boards or a forced mana-dork tap.

Two turns in the corpus had a cast but no following attack step. Both are CORRECT (not non-issuance):

- **vs49 T7:** board `creatures: 0` at turn start (Heart died in T5 combat). Cast Muscle + Striking —
  BOTH summoning-sick with no haste enabler on board -> no eligible attacker -> correctly no step.
- **vs27 T4 (seq6/7) and T6 (seq8/9):** Gemhide (the lone mana dork, unsick) rendered untapped at the
  cast-decision point, but the casts FORCED tapping it: T4 cast two Muscles ({1}{g}+{1}{g} = 4 mana) with
  only 3 lands available; T6 cast Bonesplitter {3}{r} = 4 with only 3 lands. In both, lands could not
  cover, so the sparing logic correctly drew the weakest/only mana source (Gemhide) into the bill,
  tapping it out of combat. Corroborated by seq9 rendering the first Gemhide `[tapped - untaps and can
  attack next turn]` on my own turn. **Forced payment, per spec ("drawn into the bill weakest-power-first
  only while still short"), NOT a sparing violation.**

Mild residual (construction-adjacent, NOT engine, NOT guide): when a Gemhide is the only mana source and
lands are short, developing another spell taps the Gemhide out of an attack. On vs27 T4 the model chose
to cast a 2nd Muscle (grow the team) over swinging a lone 2/2 Gemhide — a defensible develop-vs-attack
tradeoff that a lone-small-body board makes marginal. It won the game (T10 six-wide swing, opp -45).
Flagged as a lens, not a defect. Routed to notes.md (roster owner) as construction signal, unchanged
from wave-20's "more haste enablers" note.

---

## HEADLINE 4 — ANSWER-FIRST + parser: 87/87 lead with the coded line; the fragile 14k near-misses now PARSE CLEAN.

- **Ordering:** all 87 model replies at this seat begin with a coded line (CHOICE/ATTACK/BLOCKS). Zero
  not-first. Answer-first is working 100%.
- **The wave-20 exit question — did the fragile 14k near-misses convert to fallbacks or parse clean?
  They PARSE CLEAN.** The longest replies this corpus (vs27 seq9 13,108 chars "Cast Gemhide"; vs14 seq16
  12,855 "Cast Bonesplitter"; vs62 seq17 12,559 "Play Mountain"; vs27 seq8 12,116 "Cast Bonesplitter")
  all parsed to a correct choice. Answer-first DEFUSED the truncation risk: the coded line is line 1, so
  a long/repetitive reasoning tail — even if it truncated — no longer costs the parse. vs62 seq17 is the
  vivid case: a **12.5k-char reply for a LAND DROP** that committed `CHOICE: 1 (Play Mountain)` on line 1
  (correct, instant) then spiraled the rest of the turn's combat math with verbatim repetition
  ("I die." x4, "I have {g}{u}{r}{r}{r}." x4). The guide's "never spend a long reply on a land drop" is
  disobeyed in LETTER but the OUTCOME is correct — this is now a token/latency cost, not a correctness
  risk. Route the residual to a decode-time repetition/max-token guard (general-suggestions G1), NOT the
  guide.
- **Answer-quality regression from committing-first?** One latent instance, caught by the safety net —
  see Headline 5. No decision degraded in outcome.

---

## HEADLINE 5 — the ONE fallback (vs62 seq18, `retracted_choice`): CORRECT, and the heuristic landed right.

Classified: `fallback=retracted_choice`, `choice=-1` (routed to heuristic). Context: T11, at 5 life vs
opp 14 with 8 opposing creatures (dead next turn regardless). The reply emitted **`CHOICE: 3 (Cast Heart
Sliver)` FIRST**, then spiraled ~10 times between Heart and Fury, and concluded in its PLAN line **"Cast
Fury Sliver"** (the max-damage line: Shifting-unblockable board + Fury double-strike = 12, opp 14->2).
The parser detected that the first-line CHOICE was disavowed with no clean re-emitted `CHOICE:` line and
routed to heuristic. Outcome: **Fury Sliver was cast** (seq19 board shows it with double strike) — which
MATCHES the model's actual final intent AND is the correct max-damage play in a lost position.

- **Was the retraction-fallback correct?** YES — the model genuinely disavowed Heart (its reasoning and
  PLAN both settle on Fury).
- **Did the heuristic's answer hurt?** NO — it happened to land on Fury, the model's final intent and the
  best of bad options. The game was already lost (5 life, 8 attackers incoming); Fury's alpha put opp to
  2, one turn short.
- **The pattern to watch (noted, not a defect):** answer-first assumes commit-then-justify, but here the
  model treated its line-1 CHOICE as a first draft and reasoned to a DIFFERENT card, leaving line 1 STALE
  vs the final PLAN. The retraction net caught it; the heuristic recovered the right card by luck of the
  argmax. A prose-intent salvage that read the PLAN line ("Cast Fury") would have recovered the intent
  deterministically rather than relying on the heuristic matching. Single-seat, low-stakes (lost game);
  logged in notes.md as a HARNESS watch, not a change request.

---

## Record 3/6 flat — decompose: 0 guide-fixable, 0 engine-fixable.

- **vs49 LOSS = RACE/matchup.** Mulliganed to a slow hand; first creature T5; Heart (the haste engine)
  traded into a Firespitter chump-block; a Dragons clock (Dragonmaster Outcast / Firespitter Whelp /
  Thunderbreak Regent) beat me 20->-1 while opp stayed 20. The engine attacked when it could (T5); the
  loss is draw + matchup, not the old 0-attacker bug (which is GONE).
- **vs62 LOSS = RACE, by ONE.** Attacked every eligible turn (seqs 7/11/15/19); the Fury double-strike
  alpha would have been lethal one turn later, but I died first at -1 vs opp 2. Coin-flip race against a
  faster board (Enchantress -> 8 creatures). Variance.
- **vs102 LOSS = DRAW/MANA SCREW.** Kept (after mull) a hand that produced only a lone Striking + lands;
  Striking attacked T2/T4/T6 then died, and T8-T12 I drew only lands (Mountain/Mountain/Forest/Mountain)
  with no castable Sliver — zero development. Pure variance; no decision was wrong.

## What went RIGHT (do not re-teach)
- Attack-every-turn is the default wherever a step is offered (validated 4th wave); the partial-sparing
  fix now makes that step FIRE reliably for non-haste boards too.
- Multi-anthem [X/Y] math off rendered numbers stays clean (vs135 5-wide 4/2 read; vs14 Might 5/5 flyer
  team; vs27 six-wide trample overkill for 45).
- Block quality remains selective and correct: "no blockers" in healthy spots (vs62 seq16, vs135), and
  it took the two profitable blocks offered (vs27 seq10 Bonesplitter/Gemhide double-block killing two
  Zombies and surviving; vs62 seq20 Fury blocks Canopy Spider). No chump-durdle.
