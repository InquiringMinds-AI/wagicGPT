# Deck-110 (Etched Affinity) — Wave-7 game review

**Evidence base.** 6 seat translogs from `matchups-20260713-222831/` (one round-robin run),
188 logged decisions for this seat (ask 98, priority 56, attackers 22, blockers 6, gameend 6).
Facts verified against `bin/Res/sets/primitives/mtg.txt` and `src/AIPlayerGPT.cpp`.
Citations are `<epoch prefix>` + seq (+ turn).

## Record: 4 WIN / 2 LOSS (beat 140, 135, 131, 109; lost 44, 133) — REBOUND from wave-6 raw 1/6.

| file (epoch) | matchup | result | end T | end life | one-line cause |
|---|---|---|---|---|---|
| 1784000115 | 135v110 | **WIN**  | 9  | my20 / op-1  | went wide, closed fast |
| 1784000498 | 140v110 | **WIN**  | 11 | my19 / op-8  | double-Lightmine avoided, closed with bodies+Blast |
| 1784006250 | 131v110 | **WIN**  | 9  | my31 / op-4  | two Platings, blowout |
| 1784009179 | 110v109 | **WIN**  | 8  | my5  / op-5  | aggro race WON (was a LOSS in wave-6) |
| 1784005681 | 44v110  | **LOSS** | 14 | my-3 / op20  | Mox/land FLOOD; Master of Etherium countered x2 |
| 1784007189 | 110v133 | **LOSS** | 17 | my-2 / op12  | weakest-body flier draw, ground out |

**Harness sweep (required, CLEAN):** `fallback` absent in all 188 records (0 desyncs, 0 engine
no-ops, 0 validation-drops). Every play below is a GENUINE decision.

This wave CONFIRMS wave-6's attribution ("a falling record on n<=6 is variance, not regression").
Same guide, same OBEYED-BUT-LOSING posture, one variable flipped per game (a real board vs. a
flood). vs109 flipped LOSS->WIN with no matchup-specific change — pure draw variance, exactly as
wave-6 predicted. Freeze the obeyed lines; the two losses route to the standing deck-construction
flag, not the guide.

---

## VALIDATION TARGET 1 — THE RE-ATTACH MARKER: SHIPPED, FIRED, AND OBEYED. (headline)

Wave-6's method headline was that the marker was **shipped-but-inert** (it gated on `auraParent`,
which equipment never sets — Cranial Plating stores its host in `source->target`). Wave-6 notes
#E1 asked to also test `moved->target == action.target`. **That fix LANDED**
(`src/AIPlayerGPT.cpp:1163-1173`: equipment is excluded from `auraParent`, so `host = moved->target`;
the code carries the TYPE_EQUIPMENT carve-out comment). Verified end-to-end from deck110's logs:

- **The marker string now APPEARS.** `(ALREADY attached to it - this would change NOTHING)` renders
  on the equip option line in 4 of 6 games (absent only in vs140/vs44, which had 0 marked windows).
  Wave-6: it appeared **0 times**.
- **deck110: 29 marked-offer decisions, marker OBEYED in 28.**
- **No-op re-equip rate: 1 of 16 equip picks = 6%**, DOWN from wave-6's **9/18 (50%)**. Clean
  end-to-end proof of the wave-5/6 chain: shipped -> FIRED -> OBEYED. It closes the "read-then-
  disobey ceiling" — the marker on the OPTION line (not a board cue the pilot had to read) was the
  real lever, exactly as wave-6 concluded.

**The single corpus-wide disobedience is deck110's — and it is NOT a marker failure.**
`1784006250` (vs131) **s33 T9**: chosen_text carries the marker; the reply is `3\nPLAN: ...
re-equipping [Vault Skirge] (Option 3) is a wasted action that does nothing. However, I have a
second Cranial Plating ... NOT yet attached to Master of Etherium [Option 2] ...`. The pilot **READ
the marker, understood it, stated it wanted Option 2** (attach the SECOND Plating to Master), then
typed **3**. This is a **PLAN/CHOICE MISMATCH (intent collapse)**, not a re-equip-waste: the
representation worked; the model fat-fingered the index. Routes to model-experiments /
reply-protocol, **NOT** to guide wording. Cost nothing (vs131 was a blowout, opp -4 T9).

**Secondary — multi-Plating equip-churn (watch, not a lever).** vs131 T9 had SIX equip decisions
(s31-s36) shuffling two Platings between Master of Etherium and Vault Skirge. Only s33 was marked;
the others are unmarked because moving Plating-A off a carrier that already holds Plating-B IS a
state change. The marker cannot catch same-turn multi-Plating shuffle churn. Low-value (total team
power unchanged) and inside a crushing win — a watch item. If it recurs across seats it is an
engine-representation ask (suppress re-offer once every Plating is attached to SOME carrier), never
guide text.

**Guide consequence — Rule #1 COMPRESSED (the count-brake is now simplifiable).** Per the skill's
stopgap lifecycle + demotion rule: the marker is the representation fix that was Rule #1's whole
reason to exist; obedience is total-or-near (28/29); the defense now lives at a stronger layer (the
option line). So Rule #1's multi-line two-power PERCEPTION PROCEDURE and the once-per-plating COUNT
BRAKE scaffolding demote to load-bearing tokens: keep the corrective FACT (an already-attached equip
is wasted {1}), the marker string it now reads, the named RELEASE (equip again only when the carrier
DIED or a better carrier arrived), the positive DESTINATION (mana -> creatures/Blasts), and ONE
demoted two-power backup line (marker is new-this-wave; the pilot's own board-reading was
historically unreliable). Delete the rest.

---

## VALIDATION TARGET 2 — COUNTER-AWARE BAIT LINES vs deck44: the flood removed their applicability.

**Was Champion/Plating countered again? YES — Master of Etherium, twice.** `1784005681` grep:
`Your Master of Etherium: stack -> graveyard` x2; `Opponent's Counterspell: stack -> graveyard` x2.
deck44 answered deck110's lone bomb with hard counters. (Etched Champion T13 resolved.)

**But this was a THREAT-STARVATION FLOOD, same as wave-6 vs44 — the line's precondition was unmet.**
Cast sequence: T1 Ornithopter (0/2), T5 Signal Pest (0/1), then Island/Mountain/Mox for six turns,
until T11 the hand was **Galvanic Blast + Etched Champion + Master of Etherium** — all counter-bait,
no cheap bodies left. At T11 s10 the pilot jammed **Master {2}{u} into open blue** (a real
disobedience of "don't jam your lone Master into open blue mana") and it was countered. By then the
"lead with cheap bodies, go WIDE" advice was MOOT: the cheap bodies were already deployed at
0 power. opp NEVER dropped below 20 all game — deck110 had no clock to protect. The counter loss is
**deck-construction (flood) + META (a now-working counter deck)**, not a guide-wording failure —
verdict identical to wave-6, route upstream.

The counter-aware block is correct DOCTRINE (keep verbatim) — it simply had no cheap body to apply
to in this flooded draw. No wording fixes "I drew only lands and bombs against blue."

---

## DISPLACEMENT / OVERSHOOT AUDIT on wave-6 additions (mandatory)

- **RACE `<=8` no-attack-with-a-needed-blocker clause (wave-6 P3) — now TESTABLE and WORKING.**
  Wave-6 had ZERO blocker windows. This wave the block seam ARRIVES: 6 windows corpus-wide (vs133 2,
  vs109 2, vs131 1, vs135 1). vs133 shows the survival behavior firing: at my3 then my2 the pilot
  blocked Geralf's Messenger with Vault Skirge (T15) then Etched Champion (T17) instead of racing.
  vs109 — the wave-6 LOSS where the pilot attacked a needed blocker at 8 life and died — flipped to
  a WIN. Small n, but the clause landed on the right side with no over-blocking (no racing creature
  chumped a 1/1 at high life). No overshoot, no displacement.
- **Counter-aware go-wide clause — no harm.** No body-hoarding; cast-nothing stayed low where bodies
  existed.

## Corpus health
- **0 fallbacks / desyncs / no-ops** across 188 decisions.
- **cast-nothing 14/56 cast-decisions (25%)**, concentrated in the two stall/Lightmine wins (vs135
  5/12, vs140 6/16) where holding is often correct (vs140 is the double-Lightmine game whose guide
  ORDER is "cast nothing / don't attack"); close/lost games ran 0-1. No durdle regression.
- **attackers 22, blockers 6** — both seams now exercised.

## DECK-CONSTRUCTION / META FLAG (reaffirmed — the record's real owner)
1. **Flood / dead-draw.** 8 of ~25 "creatures" are ~0-power (4 Signal Pest 0/1, Ornithopter 0/2)
   atop 20 lands + 3 Mox Opal + 4 Cranial Plating. Threat-light draws (vs44) produce no clock.
2. **Counter-vulnerability (meta shift).** Wins route through a few discrete high-value threats
   (Master of Etherium, Etched Champion, Cranial Plating) — exactly what a now-effective counter
   deck answers 1-for-1. Master countered twice this wave.
3. **Only 4 Galvanic Blast** = sole reach through Lightmine / stall / counter walls.

This wave's one model-fixable lever (the no-op re-equip) is now FIXED at the engine layer, so the
guide edit is a pure COMPRESSION, not a new rule.

## Handoff for the guide revision (OBEYED-BUT-LOSING / frozen-line diff)
1. **COMPRESS Rule #1** to load-bearing tokens (marker now does the perception + brake work).
2. **Everything else VERBATIM.**
3. The vs131 intent-collapse and the multi-Plating churn route to notes (model-experiments /
   engine-representation), NOT guide text.

## FREEZE-CHECK (obeyed-but-losing mode)
Every wave-6 line survives verbatim in wave-7 strategy.txt EXCEPT the Rule #1 compression:
- Identity / kill-by-5-6 / cast-nothing=lost-turn / beatdown-by-default — VERBATIM.
- CASTING mana falsity-fact + Memnite/Ornithopter {0} — VERBATIM.
- THE ENGINE (cast-Plating-T1 / equip-first-{1} / carrier order / power-only) — VERBATIM.
- THE RACE (go-wider / chump-at-8 / `<=8` no-attack-with-needed-blocker) — VERBATIM.
- ATTACK EVERY TURN / Signal Pest / Etched Champion / Lightmine rewrite — VERBATIM.
- LETHAL CHECK / Blast-to-face — VERBATIM.
- PLAYING AROUND COUNTERS — VERBATIM (validated doctrine; flood, not wording, lost vs44).
- OTHER KEY CARDS / MULLIGAN / DECIDING SITUATIONS — VERBATIM.
- Rule #1 — COMPRESSED per demotion rule; load-bearing tokens kept verbatim.
