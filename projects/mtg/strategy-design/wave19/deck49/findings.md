# deck49 (Dragons) — REVISED-GUIDE ROUND-2 RE-VALIDATION findings (wave 19)

Mono-red Dragons aggro-midrange, playing its SECOND guided corpus — the first RE-validation of the
wave-18 revision (compressed Rule #1, trimmed Rule #2 mechanics, added flood off-case). Corpus: 6
games, **5/6** (W vs deck35, deck14, deck62, deck27; W-adj vs deck135; L vs deck110). 142 decisions
(108 ask, 24 attackers, 10 blockers), 0 timeouts, 0 segfaults. Binary /tmp/wagic-72b05535d, which
carries the wave-19 engine batch (TRADE OUTCOME block annotations; combat name tolerance;
salvageLoopedChoice) AND the E-49c suppression->ANNOTATION replacement.

Step-0-ter posture: report per taught class DIED/PERSISTED/MUTATED before the record; the record is a
coin-flip surface (rule 1). The record jumped 2/6->5/6 but that is pool/matchup (deck49 flipped vs14,
vs62, vs35 from L last wave to W; deck21/17 gone, deck27 in). Judge by taught-class survival + tax.

## Headline: the mask LIFTED and C4 proved SOLVED — the last open item is closed

The wave-18 carry-forward WATCH was: "a suppressed class cannot be validated as DEAD — only as 'had
no chance to fire.' E-49c hid every enemy-only War Paint; re-test C4 when the mask lifts." **This
corpus is that re-test, and C4 PASSED.** The owner's suppression->annotation replacement is LIVE: at
deck135 s4/s5/s7 Goblin War Paint was offered with ONLY opponent-controlled targets, now carrying the
engine warning annotation:

> "Cast Goblin War Paint {1}{r} - legal targets right now: Icehide Golem, Boreal Druid (warning:
> every legal target is the OPPONENT's - this would help their side unless you are deliberately
> triggering something)"

The model DECLINED all three (Cast nothing s4/s5; Cast Bloodmark Mentor s7), zero loops, 0 fallbacks.
The fatal wave-17 C4 loop (deck17 s18, deck135 s22 — 12-14k-token unparses) had a live opportunity to
fire and did NOT. C4 (never-aura enemy-only) is now proven-solved, not merely masked. The shipped
annotation wording is near-identical to my wave-18 G-49b proposal -> E-49c CLOSES.

## Zero fallbacks again — second consecutive clean seat (142 / 142 parsed)

| metric | wave-17 (guideless) | wave-18 (guided) | wave-19 (round 2) |
|---|---|---|---|
| fallbacks at this seat | 9 | 0 | **0 / 142** |
| stale_echoes | 7 | 0 | **0** (trimmed Rule #2 held — no regression) |
| fabricated mana-locks (C1) | multiple | 0 | **0** (2 mana mentions, both LEGIT affordability) |
| burn/player target menus -> correct | leaky | 17/17 | **16/16** (15 face + 1 sanctioned wall-kill) |
| War Paint on own creature | looped | 5/5 | **1/1 own-cast + 3/3 correct declines** |
| Crucible into empty board | 2 misfires | 0 | **0** (guide-cited decline) |

## WHICH GUIDE RULES FIRED (the round-2 ask) — every taught class DIED or PERSISTED-correct

- **Rule #1 mana-is-trivial (COMPRESSED wave-18): held DEAD.** 0 fabricated locks in 142 decisions.
  The 2 mana-language hits are correct accounting: deck110 s16 "I have 3 mana... cannot cast Shivan
  Dragon (needs 6)" (true — 3 lands); deck62 s5 "cannot cast Bloodmark Mentor yet, need {1}{R} have
  only {R}" (true — 1 untapped after a Bolt). The compression did not reopen C1.
- **Rule #2 target-menu (TRIMMED wave-18): held.** 0 stale_echoes; every target sub-menu resolved by
  index (Lava Spike/Bolt -> the player target; War Paint -> own creature). The wave-18 prediction —
  trim the mechanics prose, the engine TARGET CHOICE framing carries the representation — is
  CONFIRMED at round 2 (no fallback regression from the trim).
- **BURN-GOES-FACE: 16/16 correct.** 15 burn/player menus resolved to FACE across all six games; the
  ONE creature-target (deck27 s14, Lightning Bolt -> Oona's Gatewarden 2/1 flying defender+wither) is
  the guide's SANCTIONED exception — the model reasoned "must remove the opponent's only blocker...if
  it blocks my Firespitter Whelp it deals 2 and places two -1/-1 counters, killing my Whelp," then
  sent the SECOND Bolt at the face. Both the rule AND its exception fired correctly.
- **never-aura-opponent: fired 4x, all correct.** (a) deck62 s7 DECLINE-with-citation: "I will not
  cast Goblin War Paint as all legal targets are the opponent's creatures, which would buff them."
  (b) deck135 s4/s5/s7 declined enemy-only War Paint under the NEW annotation (above). (c) deck62
  s10->s11 the HARD case the annotation does NOT cover — a MIXED menu (own Bloodmark Mentor + enemy
  Birds/Saprolings, no warning) — the model cast War Paint and at the target sub-menu chose its OWN
  Bloodmark Mentor over the opponent's bodies. The mixed-menu case is carried by the GUIDE rule, not
  the annotation (see notes: keeps the rule load-bearing -> NOT a trim candidate).
- **Crucible-needs-dragons: fired.** deck62 s21/s23 declined Crucible citing "only buffs Dragons (I
  have none)"; every actual Crucible-context cast waited for a Dragon or a lethal-attack line.
- **deploy-and-attack: obeyed.** deck49 attacked in every eligible attacker-decision across all six
  games (Dragonmaster Outcast / Bloodmark Mentor / Firespitter Whelp / Rorix as they came online).
  The ONE "no attackers" (deck135 s10) was FORCED — its only creature, Bloodmark Mentor, was tapped
  by Gelid Shackles (can't-untap lock). No durdle signature at any seat, including the adj game.
- **flood/no-curve off-case (ADDED wave-18): NOT ENGAGED this corpus.** No game reached the
  flood/lone-1-1-plus-burn state with a wrongly-held face burn — the two wave-18 grind losses (vs62,
  vs14) both flipped to WINS. The 3 "cast nothing while a face-burn was on the menu" (deck110 s5,
  deck14 s9, deck62 s3) are all EARLY-GAME DEVELOPMENT holds (turn 1-4, has a curve, developing toward
  the Dragonmaster engine) — the correct read, not the mild burn-hold the off-case targets. The
  off-case remains UNTESTED (a winning corpus never floods; Step-0-ter rule 6 anticipates this).

## Validation focus — wave-19 engine batch at this seat

- **Combat name tolerance: no block bled.** 0 discarded block replies (0 fallbacks). deck135 s11 is
  the near-miss: the model opened "BLOCKS: B1:A1, B1:A2", caught its own double-assignment, and
  emitted a valid single block (Thunderbreak Regent blocks Icehide Golem) — parsed cleanly. Nothing in
  the old collapse shape survived.
- **TRADE OUTCOME annotations: LIVE, ACCURATE, used correctly — no misleading instance.** First corpus
  with per-pairing outcome tags on block options. Verified accurate: deck14 s23 "Ashenmoor Liege (4/1)
  — may block A1 (both die), A2 (both die)" (4/1 vs 2/1 Coral Merfolk -> both die, correct); deck110
  s19 "Firespitter Whelp (2/2) — may block A1 (both die), A2 (your blocker dies, attacker lives)";
  deck35 s16 "(you kill it, your blocker lives)...(your blocker dies, attacker lives, 1 tramples to
  your face)". The model READ these to make correct mechanical calls (declined chumps when ahead:
  deck14 s29 no-block a 5/5 into (both die) while racing; correct triage when behind: deck110 s19
  blocked the lifelink flyer, correctly noting it could not block the 10/5 Master). BONUS: at deck14
  s23 the annotation "(both die)" corrected the GUIDE/card-script Ashenmoor Liege mismatch (guide says
  4/4, engine renders 4/1) — the model trusted the rendered outcome over the guide's toughness. The
  annotation is doing real cross-checking work.
- **salvageLoopedChoice / unparsed:** deck49 contributes 0 to the corpus's 21 unparsed (nothing to
  salvage here).
- **Pitch surfacing:** N/A (deck135 seat).

## The single loss — top-heavy-curve pre-flag CONFIRMED, still latent (not guide-fixable)

vs110 Etched Affinity (L, died at -2, turn 8): deck49's clock was a lone **1/1 Dragonmaster Outcast
that never reached 6 lands / never made a Dragon token** all game (max 4-5 Mountains before it died);
Firespitter Whelp arrived late. It was OUT-RACED by explosive Affinity — a **10/5 Master of Etherium +
Vault Skirge** on turn 7-8. The model played correctly throughout (attacked every turn, burned face,
made the only-legal block on the lifelink flyer while correctly conceding it could not block the 10/5).
This is the EXACT wave-18/wave-17 pre-flagged construction shape: early clock is a 1/1 that needs 6
lands, real threats at 4-6 mana, no way to race an explosive draw. Verdict on the pre-flag: **still
LATENT** (it produced the sole loss), but only 1 loss this corpus and it is a DECKLIST property, not a
guide or decision defect (routes to construction, G-49d unchanged). Not "dead" (a curve cannot die
from strategy text); simply not more prevalent.

## Residual reasoning-tax — relocated tax persists, all parsed (Step 0-ter rule 4)

13 replies ran >=600 words (peak **2,435**, deck14 s16 — a card-choice spiral, Ashenmoor Liege vs
Dragonmaster Outcast, re-deriving land counts, landing correctly on Outcast). Down from wave-18's
20/125 (peak 2,800) but that is variance, not a proven improvement. Every spike stayed PARSED (0
fallbacks). Same successor class as wave-18 G-49a: combat/card-value deliberation + stale-plan
re-derivation. CORE/general-prompt concern, unchanged — routed to general-suggestions.md, not the guide.
