# Wave-43 engine+narration validation seat (harvested 2026-08-24)

Instrument validated first (delta reconstruction byte-identical 467/467).

## OWNER NARRATION FIXES: ALL PASS
1a rules-text tails 0 (controls intact: text still on option/board surfaces; note the
"2036" baseline was a cumulative-prompt artifact — true delta baseline 67); 1b damage
double-entries 0/64, (now N) coverage 577/577, gained/lost survivors correct, toxic
null; 1c mana-hint tails 0/555.

## WAVE-43 ENGINE LANES
2 MENACE: attack side PASS (3/3 tagged w/ honest counts + forecast suppression; 0/24
untagged; ASSERT absent 21/21); block side VACUOUS (0 menace attackers reached a
blockers window) -> discharge via PROBE DECK, stop carrying as corpus prediction.
Dropped-assignment accounting: corpus total 1, non-menace, correctly accounted.
3 reveal attribution 38/38 PASS. 4 transforms 6 lines = 3 flips x2 seats PASS; Day/Night
transform 0, battlefield entries 0/4380; silent swaps 0/4296. 5 DFC sweep: 47/47 Flip
Side = labelled modal-DFC toggles; Cathar/Huntmaster shape 0 PASS. 6 silent cast:
ANNOUNCE_X ratio 1.00 (18/18 verifiable; 19th unverifiable-not-falsified — seat died
pre-flush); re-offers 0 PASS. 7 X pricing 109/109 incl. alt branches; history leaks 0
PASS. 8 counter target text 57/57 PASS. 9 owner tags 946/946 PASS. 10 observer parity
PASS (0.420 vs 0.421 targeting; handles ~0.20 both; was 64/309 vs 0/302); run collapse
0 runs PASS; Day/Night filter PASS. 11 lexicon: put-a-put 0 PASS, token-text 0 PASS,
teferieffect FAIL (29 — stack path never consults legibleCounterName), dungeon
[your zone] FAIL (84 — dungeons live in SIDEBOARD not command zone; root-caused).
12 fallbacks 4/2317 = 0.17%, all ONE mechanism: plan-name leak into narrow windows
(now 4/4 of all fallbacks — the LOW-batch item is promoted).

## NEW -> wave-44 docket
1 (MED) teferieffect stack-label path (one-line fix at stackAbilityName).
2 (MED) dungeon rows: sideboard zone name / suppress zone tag on dungeon rows.
3 (MED) Day/Night INVISIBLE in CURRENT SITUATION (0/2190; 33 records render daybound
  creatures with no day/night state anywhere but the log) — trust-doctrine omission.
4 (LOW) loyalty 'lost' lines carry no (now N) (15/15; got-lines do).
5 (LOW) narration targeting clauses unowned 231/365 (option rows 946/946) — needs an
  explicit ruling (log-shape parity vs owner tags).
6 (LOW) {room effect:} rules text survives in history (19 lines) — OWNER CALL (rooms
  are opaque by name alone; may be a deliberate keep).
7 (LOW) plan-mismatch note predicate missed both fallback sites — audit trigger.
