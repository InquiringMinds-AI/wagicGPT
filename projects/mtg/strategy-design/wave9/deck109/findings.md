# Wave-9 review — deck109 (Hellrider, mono-red aggro / RDW)

**Corpus:** PRIMARY `matchups-20260714-094942`, one round-robin, 6 games (deck109 seat). Binary
`/tmp/wagic-a40671057` (cast-restriction gating + cast-seam livelock breaker + broadened punisher rider +
own-targets annotation), post-A/B core prompt (hand->stack anchor GONE). Guide in play = the LIVE
`deck109_strategy.txt`, **byte-identical to wave-8's revision** (the re-anchored chump rule + NEVER-block-
damage-punisher absolute + Pillar `{R}` corrective all shipped). This is the headline seat for the
re-anchored chump rule.

**Record: 6/6** — beat 140, 44, 131, 110, 133, 135. Per the USER DOCTRINE this is context only (a
reshuffled draw/opponent slate; deck109 lost to 133 and 110 last wave and beat both this wave — pure
variance at n=6). The unit of analysis below is the DECISION.

| seat file (epoch) | opp | end (my / opp, turn) |
|---|---|---|
| 1784040587 | 140 | 20 / -1, T15 (grind, ahead throughout) |
| 1784040589 | 44  | 17 / 0, T11 |
| 1784041212 | 131 | 20 / -6, T13 |
| 1784044788 | 110 | 5 / -3, T11 |
| 1784045652 | 133 | 18 / 0, T10 (raced devotion out) |
| 1784045654 | 135 | 20 / 0, T9 |

---

## HEADLINE — the re-anchored chump rule (block ONLY on surfaced LETHAL): VALIDATED, clean 3/3

This corpus was **very quiet at the blocker seam — only THREE blocker windows across all 6 games** (the
deck raced so hard it rarely defended). All three were played CORRECTLY, and the two wave-8 misplay
classes did not recur where a window existed:

1. **deck110 seq4, T2, me 20 / opp 20** — surfaced `deal up to 5 - you would be at 15 - NOT lethal`.
   Pilot chose **`no blockers`**. This is the exact wave-8 HIGH-LIFE GANG-BLOCK class that misplayed
   (wave-8 deck110 seq13: gang-blocked a 6/6 at 19 life reading "at 12 - NOT lethal"). The re-anchored
   rule ("declare NO blockers when the line says NOT lethal") was OBEYED at the same seat/matchup.
   **The high-life gang-block class DIED.**

2. **deck110 seq14, T6, me 5 / opp 13** — surfaced `you would be at -2 - LETHAL if it all connects: block
   enough to survive`. Pilot chose **`Goblin blocks Memnite; Goblin blocks Memnite`** (chump to survive).
   Correct per rule (b): the ONLY corpus window that said LETHAL, and the pilot chumped exactly as
   instructed. Confirms the re-anchor distinguishes LETHAL from NOT-lethal in both directions.

3. **deck131 seq8, T4, me 20 / opp 20** — surfaced `deal up to 2 - you would be at 18 - NOT lethal`.
   Pilot chose **`Goblin blocks Young Pyromancer; Goblin blocks Young Pyromancer`**. This is a block at
   high life against the NOT-lethal line, but it is **exception (a) firing correctly** — the reply PLAN
   explicitly reasons "removes the token-maker and spell-trigger threat immediately," NOT a phantom-lethal
   fear, and Young Pyromancer died (2 dmg vs 2 toughness; events confirmed at seq9). The re-anchored rule's
   exception (a) coexists cleanly with the NOT-lethal default — the pilot did not misread NOT-lethal as
   lethal here.

**Verdict:** the re-anchored chump rule is validated on the class it could test (high-life NOT-lethal ->
no blockers), correct on the LETHAL branch, and its exception (a) fired for the right reason. NO blocker
misplay this corpus.

## The Obliterator-block-at-10-life class had NO test window — carry the guardrail, do NOT demote

deck133 (mono-black devotion, the Phyrexian Obliterator deck) was **raced out and beaten T10**. Obliterator
**never appeared in a single deck109 prompt this corpus** (grep: 0 hits for "Obliterator" in the deck133
seat file board/option text) — it was never cast into a combat the pilot had to answer. So the
NEVER-block-Obliterator absolute (shipped wave-8) got **zero test opportunities**: neither fired nor
violated. This is the rare-event/thin-sample case. Because this is the FINAL wave before a pause and the
class it guards is a KNOWN game-loser (wave-8 vs133 seq22: block-at-10-life -> sacrifice 2 Mountains ->
manaless -> lost), the correct call is to **KEEP the guardrail at full prominence and carry the class as a
residual watch** — demoting an untested guardrail into a pause with no wave-10 to catch a regression is
exactly the wrong risk. See notes.md for the self-contained watch.

## Punisher-rider USE at this seat — no test data (route to the deck133 reviewer)

The brief's "did the pilot USE the rider on damage-trigger attackers" cannot be answered from deck109's
seat: **no opponent damage-punisher entered any attack or block window this corpus** (Obliterator absent
per above; no Boros Reckoner on any opponent's board — every "Boros Reckoner" mention is the pilot's OWN,
in its plans). The rider-render + rider-USE verification for Obliterator lives at the **deck133 seat** (the
seat that PILOTS Obliterator) and in that seat's opponents' blocker prompts, not here. Carry to engine
ledger #3 (Obliterator punisher-rider) as still-owed representation. Note the shipped blocker-seam lethal
line (#5) is now REPRESENTATION-PRESENT-AND-WINNING at this seat (3/3 obeyed) — the wave-8 "present-and-
losing" wall did not reappear on the windows that occurred.

## Pillar of Flame `{R}` corrective — VALIDATED, false-cost belief GONE

The wave-8 corrective ("Pillar costs `{R}`, not `{1}{R}`; fire it at the face when it's your only castable
option") targeted a recurring false `{1}{R}` cost belief + hold-mana metastasis. This corpus:
- **Zero instances of the false `{1}{R}` cost belief** across all Pillar-decline replies. The false-cost
  axis is dead.
- **8 Pillar-at-face casts; 15 "declines."** Every decline is correct play: 13 declined Pillar to cast a
  CREATURE instead (the #1 deploy rule), and the 2 genuine "Pillar was the only castable option -> Cast
  nothing" windows (deck131 seq13, deck135 seq7) each **fired Pillar at the face in MP2 the SAME turn**
  (deck131 seq15 opp 20->17; deck135 seq9 opp 20->17). The pilot held Pillar through MP1 and burned face
  after combat — no cross-turn hoarding, no outcome loss.
- **One soft residual:** deck135 seq7's stated reason was "save mana for combat tricks / hold up mana"
  (false — RDW has no instants/tricks). But it fired Pillar the same turn regardless, so it is
  non-outcome-changing and already covered by the guide's existing "holding it 'for interaction' does
  nothing (you have no instants)" clause. NOT worth an edit (editing over a covered, single, benign
  instance is gold-plating and risks displacement into the deploy rule).

## Phantom-lethal watch (wave-8 1-seat watch) — NO 2nd instance, keep at WATCH

No turn declined a listed CREATURE while fixated on an unaffordable bomb computing a false lethal. The 5
"Cast nothing" picks this corpus: deck110 seq33 (correct — LETHAL already on board, swing for 6 vs opp 3);
deck131 seq23 + deck131 seq13 + deck135 seq7 (all Pillar/Spear held to MP2, fired same turn or attacked
first — correct sequencing); deck133 seq3 (T0 develop, fine). **Keep the phantom-lethal watch at 1 seat;
do NOT promote.**

## Harness sweep — one reply-protocol intent-collapse (NOT a guide bug)

- **DESYNC among numeric heads: 0.** No cast no-ops, no same-pick/mana-unchanged loops.
- **Attacker commitment: 27/28** offered attacker windows declared attackers. The ONE miss is
  **deck44 seq7 (T3): a REPLY-PROTOCOL INTENT-COLLAPSE**, not a play regression. Head token was the
  literal word **`"none"`** (not a number); the engine parsed it to option 0 = `no attackers`. The reply
  PLAN unambiguously wanted the attack: "Attack with Stromkirk Noble and Gore-House Chainwalker this turn."
  The guide's #1 combat rule ("declare EVERY creature that can attack") was OBEYED in the reasoning; the
  failure was the non-numeric head defaulting to the passive option. **Route to the reply-protocol layer
  (the harness-appended reply contract), never the guide.** Non-outcome-changing (won T11). This is the
  wave-8 intent-collapse class (deck135), now witnessed at deck109's ATTACKER seam with a new tell (a WORD
  head, not a stale number). See general-suggestions.md.

## What the pilot did WELL — freeze verbatim (spine intact)
- 27/28 attacker declarations (the 1 miss is a harness parse defect, not passivity); Unleash "1/1 Counter"
  always taken; a body deployed / land played every main phase.
- Burn to face by default; 0 burn at own permanents; correct exception (a) fired (deck131 seq8 YP
  gang-block, plus token-maker awareness in plans).
- Lethal recognized and taken (deck110 seq33/seq35 swing for the win; deck131/135/44 all closed with
  burn + attacks).
- 0 desyncs / 0 no-ops / 0 phantom framing / 0 false-cost beliefs.

## Deck-construction / meta flags (upstream — restate, do NOT relitigate; NOT guide-fixable at 6/6)
- Evasion / untouchable-board class (vs44 flyers+shroud, vs110 affinity pro-red) stands — no reach, nothing
  to block flyers, no targets on shroud/pro-red. Won both this wave on the race; structural weakness
  unchanged. Race-the-face is already the plan.
- Devotion + Obliterator + Gray Merchant (deck133) is a sibling hard matchup — won this wave by racing
  before Obliterator/Gray Merchant came online (T10). No card advantage / mana sink is inherent to the
  archetype. Do not change at 6/6.

## Guide verdict: FREEZE (no edit)

No new decision-level failure class this corpus. All three wave-8 shipped changes are validated
(re-anchored chump rule 3/3 correct; Pillar `{R}` corrective — false-cost belief gone) or had no test
window (Obliterator absolute — carry, do not demote). The one spine crack is a reply-protocol parse defect,
not guide-fixable. Per the doctrine (a revision is justified by a failure class; validated by that class
disappearing) and the pause discipline (leave the guide standalone, don't demote guardrails into a pause),
**strategy.txt is a byte-identical frozen copy of the live wave-8 guide.**
