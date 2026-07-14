# Dev notes — deck109 wave-9 (self-contained; project pause after this wave)

## Record: 6/6 (context only per doctrine) — MATCHUP + VARIANCE, spine frozen
Beat 140, 44, 131, 110, 133, 135. Last wave this seat LOST to 133 and 110; this wave it BEAT both — the
exact inverse — confirming the win column is draw/opponent variance at n=6, not a guide signal. No guide
edit. strategy.txt = byte-identical frozen copy of the LIVE wave-8 guide (cmp verified).

## Corpus / provenance
- Run: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-094942` (21 games round-robin, decks 44/109/110/
  131/133/135/140). Binary `/tmp/wagic-a40671057`. Post-A/B core (hand->stack anchor removed).
- deck109 seat files (opp): 1784040587 (140), 1784040589 (44), 1784041212 (131), 1784044788 (110),
  1784045652 (133), 1784045654 (135).

## Blocker-seam verification (THE headline) — 3 windows, all correct
| file | seq | T | my/opp | surfaced line | choice | verdict |
|---|---|---|---|---|---|---|
| 1784044788 (110) | 4 | 2 | 20/20 | up to 5, at 15, NOT lethal | `no blockers` | CORRECT — high-life gang-block class DIED (was wave-8's deck110 s13 misplay) |
| 1784044788 (110) | 14 | 6 | 5/13 | at -2, LETHAL | `Goblin x2 blocks Memnite` | CORRECT — chump to survive per rule (b) |
| 1784041212 (131) | 8 | 4 | 20/20 | up to 2, at 18, NOT lethal | `Goblin x2 blocks Young Pyromancer` | CORRECT — exception (a) token-maker; PLAN cites "removes the token-maker", YP died 2v2 (events seq9) |

- **Obliterator-block-at-10-life class: ZERO test windows.** deck133 (Obliterator deck) was raced out T10;
  grep of `1784045652-...deck109....jsonl` for "Obliterator" = 0 hits (never cast into a combat the pilot
  answered). The NEVER-block-Obliterator absolute neither fired nor was violated. KEEP it at full
  prominence — untested guardrail against a known game-loser, into a pause with no wave-10 safety net.

## Punisher-rider USE — no test data at this seat
No opponent damage-punisher entered any deck109 attack/block window (Obliterator absent; no opposing Boros
Reckoner — all "Boros Reckoner" strings are the pilot's OWN, in plans). Verification of the broadened rider
(render + USE) belongs at the deck133 seat and its opponents' logs. Engine ledger #3 still owed. See
general-suggestions.md #2.

## Pillar `{R}` corrective — VALIDATED, false-cost belief gone
- 0 replies cite the false `{1}{R}` cost. 8 Pillar-at-face casts; 15 "declines" all correct: 13 cast a
  creature instead (#1 rule), 2 held Pillar to MP2 and fired it face same turn.
  - deck131 seq13 (T5, {r} avail, Pillar-only) -> Cast nothing MP1, then seq15 fired Pillar face (opp 20->17).
  - deck135 seq7 (T3, {r} avail, Pillar-only) -> Cast nothing MP1 ("hold for combat tricks" — false, no
    instants), then seq9 fired Pillar face (opp 20->17). SOFT residual, covered by existing guide clause,
    non-outcome-changing. NOT an edit.

## Phantom-lethal watch — NO 2nd instance (keep at 1 seat)
5 "Cast nothing" picks, all correct: deck110 seq33 (lethal on board, swing for 6 vs opp 3), deck131 seq13 +
seq23, deck135 seq7 (all held burn to MP2 / attacked first), deck133 seq3 (T0 develop). No creature-body
declined while fixated on an unaffordable bomb. Do NOT promote.

## Harness sweep
- Desyncs among numeric heads: 0. Cast no-ops / same-pick loops: 0. Phantom "missed cast" framing: 0.
- Attacker commitment: 27/28 (kind=attackers windows). The 1 miss = **deck44 seq7 REPLY-PROTOCOL
  INTENT-COLLAPSE**: head token literal word `none` -> parsed to choice 0 = `no attackers`; PLAN wanted the
  attack ("Attack with Stromkirk Noble and Gore-House Chainwalker this turn"). Non-outcome-changing (won
  T11). Route to reply-protocol layer (general-suggestions.md #1), NOT the guide.

## strategy.txt freeze-check (FROZEN — no diff this wave)
Constrained-diff mode with ZERO diff: the wave-8 revision is byte-identical to the live guide and every
line held against this corpus. Confirmed present + validated/load-bearing:
- Identity block (RDW beatdown, no defensive plan) — obeyed (raced 6/6).
- #1 DEPLOY-AND-ATTACK rule + "declare EVERY creature that can attack" — 27/28 (1 harness miss).
- MP2 checklist (LETHAL -> otherwise deploy -> spend mana) — lethal taken every close-out.
- BURN face-default + Pillar `{R}` clause + exception (a) — validated (0 false-cost; YP gang-block fired).
- BLOCKING re-anchor (block ONLY on surfaced LETHAL) + exceptions (a)/(b) + NEVER-block-damage-punisher —
  3/3 correct where tested; punisher absolute untested but KEPT.
- PHYREXIAN OBLITERATOR note (incl. "NEVER block it") — untested this corpus, KEPT.
- EVASIVE/UNTOUCHABLE BOARDS note — matchups vs44/vs110 won on the race; note load-bearing.
- KEY CARDS, MULLIGAN, SITUATIONS — unchanged, consistent with observed play.

## Stopgap ledger
| stopgap line | defect it patched | status this wave | action |
|---|---|---|---|
| deck109 FALLBACK RULE (Hellrider) | combo-hold hint veto | DELETED wave 6; 0 regression 3 waves | stays DELETED |
| core hand->stack anchor | cast no-op class | REMOVED in wave-8 A/B; 0 no-ops this corpus | gone, confirmed clean |
| BLOCKING chump re-anchored to surfaced LETHAL | too-eager chump-at-8 fighting the math | 3/3 correct (high-life class DIED) | KEEP (validated; thin sample) |
| deck109 Obliterator "NEVER block" absolute | punisher rider missing/unconfirmed on Obliterator | 0 test windows this corpus | KEEP until engine ledger #3 confirms rider renders+works (deck133 seat) |
| Pillar `{R}` (not `{1}{R}`) corrective | false-cost belief + hold-mana | 0 false-cost instances | KEEP (validated; still load-bearing, do not demote into pause) |

## Deck-construction observations (upstream, unchanged — do NOT relitigate)
- No answer to evasion (flyers/shroud/pro-red) — won vs44 + vs110 on the race this wave; structural
  weakness stands. No card advantage / mana sink — inherent to the archetype. Devotion+Obliterator+Gray
  Merchant (133) is a sibling hard matchup, beaten this wave by racing it out before it came online. Do not
  change at 6/6.

## Pause handoff (no wave-10 assumed)
1. Guide is shippable standalone — FROZEN, validated. Nothing pending on a next wave.
2. Two items carried OUT of the guide to durable layers: (a) non-numeric-head parse defect ->
   reply-protocol A/B; (b) Obliterator punisher-rider render+use -> engine ledger #3, verified at deck133
   seat. Both self-contained in general-suggestions.md.
3. Two WATCHES held (both at 1 seat, do NOT promote without a 2nd instance): phantom-lethal (creature-body
   shape); the Obliterator-block class (guardrail kept, awaiting a real test window).
