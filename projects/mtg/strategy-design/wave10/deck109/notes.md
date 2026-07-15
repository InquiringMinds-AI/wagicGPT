# Dev notes — deck109 wave-10 (RESUME wave; engine/harness ledger with repros)

## Record: 5/6 (context only per doctrine) — matchup + variance
Beat 133, 110, 131, 135, 140; LOST to 44 (untouchable flyers+shroud, raced back — deck construction, not
play; see findings.md). Guide REVISED minimally (one card-cost line); spine otherwise frozen +
freeze-checked.

## Corpus / provenance
- Run: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-031825` (21 games round-robin, decks
  44/109/110/131/133/135/140). Binary `/tmp/wagic-c8c054be8`. Answer-after-plan reply protocol,
  max_tokens 2048.
- deck109 seat files (opp): 1784105766 (44), 1784105908 (133), 1784109135 (110), 1784110077 (131),
  1784110926 (135), 1784112074 (140).

## LEDGER ITEM A (engine / representation, cross-deck) — annotate HYBRID mana costs on the option line
**Repro:** deck131 seq18/19/21. The pilot read `Cast Rakdos Cackler {b/r}` as needing `{b}{r}` (black
AND red), believed it could not pay with `{R}`, and (s19) declined the offered creature / (s21) spiraled
the full 2048-token budget on the offered=payable contradiction -> `unparsed_reply` -> fallback `-1`.
`{B/R}` is HYBRID — one red mana pays it; the engine's offer was CORRECT (offered = payable).
**Proposed engine change:** append a hybrid hint to any option whose cost carries a `{X/Y}` symbol, e.g.
`Cast Rakdos Cackler {b/r} - hybrid: pay with {R} or {B}` (and `Boros Reckoner {r/w}... - hybrid: {R}
or {W}`). This is the durable cross-deck backstop; the deck-guide card note (shipped this wave in
strategy.txt) is the interim. Owner: the option-text builder in AIPlayerGPT.cpp (same site that renders
`{right now: ...}` and `- legal targets right now: ...`). NEVER put this in prompt/guide text as the
primary fix — it is a representation item.

## LEDGER ITEM B (harness / reply-protocol) — spiral truncation vs length-cap truncation
**Repro:** deck131 seq21 (LOOP: same self-contradiction re-stated ~12×, "Maybe the option is a trick and
the game is broken? ... Perhaps the Mana available line is misleading") vs deck140 seq16 / deck135 seq7
(clean LENGTH cuts that self-healed). The known corpus-wide 2048-cap note (4096 already shipped) covers
length cuts; it does NOT cover a reasoning loop, which fills any budget. A reply-protocol clause of the
form "if you reach a contradiction between the listed options and your mana read, TAKE the listed action
and stop reasoning" would break the loop at the seam. Single-seat; note as WATCH. Fixing Ledger Item A
removes the specific contradiction that triggered THIS loop.

## Engine-change verification (this seat)
- **Legal-target names** (`- legal targets right now: ...`): RENDERED in 39 windows; 0 self-target burn
  picks (0/39). Working; no fabricated-no-target confusion at this seat.
- **Fetch consume-on-choose:** N/A — deck109 runs 20 basic Mountains, 0 fetches.
- **Thin-hand discard annotation / `[deals 0]` blocker marker / artifact-tag completeness:** N/A —
  deck109 has no discard, 0-power blockers, or artifacts.
- **adj0/adj1 rows:** none for deck109 (all 6 games decisive; sanity-checked results.tsv life/turn
  against seat gameends — consistent).

## Blocker-seam verification (2 windows, both correct)
| file | seq | T | my/opp | surfaced line | choice | verdict |
|---|---|---|---|---|---|---|
| 1784105908 (133) | 16 | 7 | 20/8 | up to 2, at 18, NOT lethal | `no blockers` | CORRECT (high-life gang-block class OBSERVED OBEYED) |
| 1784110926 (135) | 7 | 4 | 18/19 | up to 2, at 16, NOT lethal | `no blockers` (choice 0) | CORRECT (fallback; heuristic matched the intended no-block) |
- **Obliterator-block class: ZERO test windows AGAIN** (deck133 raced out T8; 0 "Obliterator" hits in the
  deck133 seat). Guardrail neither fired nor violated — KEEP at full prominence. Punisher-rider USE
  verification remains at the deck133 seat (engine ledger #3, still owed).

## strategy.txt freeze-check (REVISED — 1 line added, rest byte-identical)
`diff` vs live guide = ONLY the Rakdos Cackler `{B/R}` hybrid cost note added to its KEY CARDS entry
(mirrors the obeyed Boros Reckoner `{R/W}` treatment; attacks the root false belief in Ledger Item A).
Every other line held against this corpus:
- Identity + #1 DEPLOY-AND-ATTACK + "declare EVERY creature that can attack" — 30/30 attacker windows.
- MP2 checklist / LETHAL recognition — every win closed out on lethal; vs44 loss had no lethal to miss.
- BURN face-default + Pillar `{R}` corrective — 0 self-target, 0 false `{1}{R}` cost belief.
- BLOCKING re-anchor + exceptions + NEVER-block-damage-punisher — NOT-lethal branch OBSERVED OBEYED 2/2;
  LETHAL branch + exception (a) + Obliterator absolute untested this corpus, KEPT.
- PHYREXIAN OBLITERATOR note — untested (Obliterator absent), KEPT.
- EVASIVE/UNTOUCHABLE BOARDS note — load-bearing; vs44 loss + vs110 win both against this class.
- KEY CARDS / MULLIGAN / SITUATIONS — consistent with observed play.

## Stopgap ledger
| stopgap / rule | defect it patched | status this wave | action |
|---|---|---|---|
| BLOCKING chump re-anchored to surfaced LETHAL | too-eager chump fighting the math | NOT-lethal branch OBSERVED OBEYED 2/2 | KEEP (thin sample; other branches untested) |
| deck109 Obliterator "NEVER block" absolute | punisher rider unconfirmed on Obliterator | 0 test windows (2nd wave running) | KEEP until engine ledger #3 confirms rider (deck133 seat) |
| Pillar `{R}` (not `{1}{R}`) corrective | false-cost belief + hold-mana | 0 false-cost instances | KEEP (validated, load-bearing) |
| NEW: Rakdos Cackler `{B/R}` hybrid note | hybrid misread -> offered creature not cast / spiral | ADDED this wave | KEEP; retire once option-line hybrid annotation ships (Ledger Item A) |

## Deck-construction observations (upstream, unchanged — do NOT relitigate)
No answer to evasion (flyers/shroud/pro-red) — lost vs44 on the race this wave, won vs110 same class; the
structural weakness stands. No card advantage / mana sink — inherent to RDW. Devotion+Obliterator+Gray
Merchant (133) beaten by racing it out before it came online (T8). Do not change at 5/6.

## Resume handoff
1. Guide REVISED (one card-cost line) + freeze-checked; shippable standalone.
2. Two durable items routed OUT of the guide: (A) engine option-line hybrid-cost annotation
   (representation); (B) reply-protocol loop-breaker (spiral vs length-cap). Both self-contained above.
3. One convergence FLAGGED to synthesis: offered=payable false belief now 2-seat (deck135 + deck109) —
   core PASS, durable fix = representation, not core prose (general-suggestions.md #1-2).
4. Watches held (do NOT promote without a 2nd instance): the reasoning-loop truncation (1 seat);
   Obliterator-block class (guardrail kept, still awaiting a real test window across waves).
