# deck18 notes (engine/harness/model items + ROTATION verdict) - wave-28 6/6

Layer-routing: engine/harness/model items with seq repros, NOT guide prose. Guide is
BYTE-FROZEN (cmp-identical wave26 == wave27 == deployed, re-verified). Rotation verdict at end.
Corpus: matchups-20260726-050449, binary ad8930961, -T 3000.

## Status of prior notes items (all still RESOLVED-at-guide / unchanged)

- N-18a (lord/anthem P/T re-derivation tax): RESOLVED at guide lane, HOLDS. p50/mean flat;
  no re-derivation spiral at the top of the wave-28 distribution. Close - stays closed.
- N-18b (flying not applied when blocking): RESOLVED at guide lane, HOLDS. Only flyer/reach
  attacker faced (vs22 seq24 Skyraker Giant [reach]) taken correctly to "no blockers", 0
  illegal ground-blocks. The optional ENGINE angle (per-blocker legality annotation) remains
  a broad multi-seat change - NOT opened on this seat.
- N-18c (Mobilization discoverability): EXERCISED fluently again (vs93/136/146 priority
  windows), no "no Activate button" confusion. Guide interface note still pre-empts it.
- N-18d (card-script oracle-verify divergences: Zealous Guardian {W/U} 1/1 vs real {W} 2/2
  Defender; Thistledown Liege 1/3 vs real 4/4): UNCHANGED, flagged for a later oracle pass,
  NOT guide changes. No new divergence surfaced this corpus.

## L-18-1 (MODEL, latency-not-correctness) - UPDATE: ESCALATED + BROADENED

Wave-27: whole-turn planning tax on trivial single-option LAND DROPS (~5-7k chars).
Wave-28: the same tax now fires on low-stakes CAST and BLOCK decisions WHILE FAR AHEAD, at
11-14.6k chars, BUMPING the -T 3000 token ceiling, and in one case running to truncation.
Six wave-28 decisions ran 174,687-218,599 ms (2.9-3.6 min) each; the top four low-stakes
ones (opp at 3-6 life, game already decided) write a full multi-turn lethal plan on a
2-option prompt.
Repros: matchups-20260726-050449 ai_baka_deck18-*
  vs146 seq13 (14,600 / 203s, Ballynock-vs-Mobilization at opp-6),
  vs93  seq17 (11,879 / 175s, Mobilization-vs-nothing at opp-6),
  vs22  seq20 (7,411 / 140s), vs22 seq24 blockers (5,557 / 85s, declined block at opp-2).
Still single-family MODEL item; if it recurs across seats it is a SKILL/CORE rung candidate
("answer the decision in front of you; when far ahead on a low-stakes/forced choice, commit
briefly - do not compute multi-turn lethal"). NOT a deck18 guide-mod. Convergence-tracking.

## N-18e (NEW, HARNESS) - unterminated reply -> stale-coded-line fallback

Repro: matchups-20260726-050449 ai_baka_deck18-*-vs-ai_baka_deck93 seq20 [blockers],
12,363 chars / 200,903 ms, my_life 17 vs opp 3.
Mechanism: the reply emitted ONE early coded line `BLOCKS: B1:A1`, then entered a
non-terminating "So I should NOT block ... Wait ..." deliberation loop and hit the -T 3000
token ceiling WITHOUT emitting a final CHOICE:/BLOCKS: terminator (reply ends mid-sentence:
"...If I don't block, I have 13 life.\n"). The parser fell back to the stale early
`BLOCKS: B1:A1`, so the executed move (Soldier blocks Rats, blocker dies for nothing)
CONTRADICTS the model's own repeated concluding prose ("do not block"). It parsed to a LEGAL
move and the game was already won, so it is SILENT - the 6/6 record hides it.
Class: this is the truncation edge of the L-18-1 loop meeting the parser's
last-coded-line precedence. Two failure modes compound: (a) MODEL non-termination near the
token ceiling; (b) HARNESS taking a stale mid-reply coded line as authoritative when the
reply is truncated.
Candidate fixes (route to synthesis / roster owner, NOT this seat's guide):
  - Detect a truncated reply (finish_reason=length AND no terminator after the last coded
    line) -> prefer the SAFE default for the phase (blockers: "no blockers") or re-ask once,
    rather than honoring a stale coded line.
  - Or raise -T for combat-block prompts (they can legitimately need more tokens than a cast).
Severity: LOW (silent, harmless here) but it is a real correctness-masking defect that only a
tighter game would expose. Ledger it as a cross-seat HARNESS watch.

## d1 (R-DUPLICATE-NAME-INSTANCE) corroboration - engine render CORRECT at this seat
#N handles rendered and bound correctly again (Cenn's Heir #1/#2, Soldier #1/#2,
Mobilization #1/#2). No mis-binding this corpus. deck18 remains corroborating evidence for
d1. No engine action.

## ROTATION verdict: ROTATE OUT at wave-28 (applied with finality)

deck18 was Step-0 (wave-26) -> first-guided (wave-27) -> 6/6 validation on a FROZEN guide
(wave-28). This corpus IS the deck59 exit shape exactly. Applying the rotation test
unsentimentally:
- Guide-mod this wave? NO. strategy.txt byte-frozen (cmp-verified == deployed).
- All three targeted leaks still validated? YES (Dodger evasion 9/9, flyer-race off-case
  clean, trust-line tail holding). e4 land tags clean. 0 fallback markers at this seat.
- New work signal that demands a GUIDE-mod? NONE. The audit surfaced two new items -
  L-18-1 escalation and N-18e (truncation->stale-line) - but BOTH route OFF the deck guide
  (MODEL + HARNESS layers, cross-seat). Neither is a deck18 guide edit.
- The record went 4/6 -> 5/6 -> 6/6 with the guide frozen since wave-26. New-work at the
  GUIDE lane is exhausted.

Per the rotation rule (no-guide-mod AND no-new-GUIDE-work-signal = rotate; termination =
new-work exhaustion, UNSENTIMENTAL): deck18 ROTATES OUT with finality at wave-28. Hand
L-18-1 (escalated) and N-18e to the cross-seat ledger for convergence tracking - they are
NOT reasons to hold deck18 (they are not guide-fixable and appear at other seats). Replace
from the guideless roster (~140).
