# Wave-42 seat: deck125 (2/6, was 5/6) — harvested 2026-08-24

388 decisions, 0 fallbacks, seating verified. THE DROP IS NOISE-COMPATIBLE (Fisher
two-tailed p~0.24; pool-wide wins conserved; the decks that took wins from 125 did not
improve overall) AND the per-loss audit EXONERATES R1-R5 entirely (zero misfires in four
losses) while INDICTING E7's two unconditional triggers (wave-40's decisive edit):
- vs162 t21: guide-compliant X=7 with a 9-card hand and Liliana's Caress VISIBLE ->
  8 cleanup discards x2 life = net -9 and discarded both sweepers ("Everything aligns").
- vs126 t34: ">=6 mana -> spend" tapped out through BOTH combo halves with Cancel in
  hand; died at 62 life with Emrakul on board (both halves on its own counter list).

Predictions: 1 SPLIT (opp<=10+Staff windows 0 — honest null; ">=15 min library" proxy
was MIS-SPECIFIED, measures game length; floor mechanism unbroken: 15/15 X within cap);
2 HOLDS 2/2 (traces quote R3 then override — the fix working); 3 HOLDS 45/45 (zero
defender-Paths; the misfire did not recur); 4 safety half airtight (0 casts at <3
creatures in 13 windows), gate 1/2 n=2; 5 HONEST NULL for a different reason (R4's
engines never reached a counter WINDOW — coverage 3/12, see engine INFO); 6 E5 RESOLVED:
KEEP (not a null wave — decline half 33/34 exercised and load-bearing; cast half still
null) + W4 life clause; 7 E4 confound STANDS and the guide's names-only sentence is now
a FALSE SURFACE CLAIM -> W3 correctness fix.

## Edits W1-W5 (verbatim in task transcript):
W1 HIGH cap X vs discard/draw punishers (hand+X <= 7 when Caress/Dreams/Unraveler
visible) + add Liliana's Caress to counter-on-sight; W2 HIGH counter-mana reserve
outranks the >=6 trigger (subtract counter cost from available before spend rules);
W3 MED replace the names-only Path sentence (render now carries P/T+tags inline);
W4 MED E5 life<=10 sweeper release; W5 LOW E7 floor race carve-out at life<=10.
NOT recommended: raising the library floor (0 windows; self-corrects above it).

## Engine INFO (wave-43 docket):
1. Counter-window coverage 3/12 on counter-on-sight cards; deck162 game logged 7
   priority records in 21 turns while 5 listed engines resolved — check interrupt-window
   OFFERING when seat holds mana + counter.
2. HIGH-VALUE RENDER: counter option lines carry the SPELL's card text, never the
   TARGET's — R4 asks for a judgment the line cannot support; Path's inline-facts shape
   is the fix.
3. No-op churn scaling confirmed (~23 byte-identical decline windows in the 75-turn game).
4. Latency median 77s p90 148s (harness note).

## Owner lategame specimen candidate: deck162 seq48-49 (the X=7 decision) — cleanest
illustration of decision-value-per-token failing at COMPOSITION level.
