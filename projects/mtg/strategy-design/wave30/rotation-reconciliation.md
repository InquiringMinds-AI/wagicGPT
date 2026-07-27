# Wave-30 rotation reconciliation

## OUT this wave (one seat) -- UNSENTIMENTAL

- **deck136 (Yarok-of-Pranks BW walker-midrange)** -- new-work EXHAUSTED at every layer the loop
  owns. Exit conditions met:
  1. **Every guide surface exercised and correct.** Ugin CAST at last (vs137 seq32 t13; vs152
     seq27 t12 -> correct -3 on Intrepid Adversary) -- the sole surface that held the seat open
     across w28-w29 fired correctly. Rankle clean (both modes, no priority bleed); Liliana both
     modes resolved earlier in the arc; the MV3 Relentless-Rats discipline validated (17
     consecutive correct Legion's-End rejections held). Guide is FROZEN byte-identical
     (cmp+md5 f96f6e77..., 10367 b).
  2. **Zero fallbacks this corpus** (0 unparsed, 0 retracted, 0 defers); parser/mana/tapped-tags
     clean; no phantom-refusal tax.
  The only LIVE item is **N-136a** (the g2 revealzone-path PUT filter is STILL unfiltered --
  Mausoleum Secrets offered 42/38/43/39 whole-library options, silent no-op 2/4), an ENGINE SEAM
  bug that routes to the ledger and is better validated by a targeted PROBE deck than by a live
  rotation slot (an engine bug at the seat does NOT reset the exhaustion clock -- wave-30 skill
  HL5). The flagship Rats matchup is not even in the pool (deck93 rotated out wave-29). On
  rotate-out: fix N-136a from the ledger, then re-add deck93 TRANSIENTLY + probe-validate the
  revealzone fix to close the Rats matchup at last. deck136 leaves with its guide frozen.
  -> **converged canary list.**

## Canary list

19 -> **20** (+deck136). (19 held deck18 + deck22 + deck93 from wave-29.) deck134 remains on the
SEPARATE construction-terminal list (not a canary).

## Wave-31 pool (7 seats)

Holdovers (6):
- **137** (GW convoke + Adventures) -- sole convoke observation post; the g1 exit gate was NOT met
  (arrival-tracing found ~55% offered-convoke resolution: 2 defers + 3 UNLOGGED silent no-ops, one
  on an AFFORDABLE Loxodon). The convoke arc CANNOT close until "offered choices producing no
  downstream arrival -> 0" is verified by arrival-tracing. Guide byte-frozen. Longest-serving seat
  (in-pool since wave-22); holds until the residual closes because it is the only instrument that
  can tell whether the next convoke fix worked.
- **146** (WB Orzhov Dungeons) -- NEAR-TERMINAL. g5 landed -> N-146f retired; Lolth loyalty
  validated (the shared walker surface is proven; Kaya = accepted draw-variance on that surface).
  Guide FROZEN. Wave-31 = confirm the freeze holds a 3rd corpus + N-146g does not mislead a real
  decision. EXIT projected wave-32.
- **139** (Temur/Sultai Mutants) -- FIRST-GUIDED, 1/6. g3 render batch VALIDATED (before/afters);
  guide REVISED this wave (stopgap retirement) so it needs one validation corpus before a
  byte-freeze. GATED on **N-139h** (the ManaCost::remove crash on discounted mutate) -- the
  mutate-under-discount surface is NOT fully observed until the crash is fixed + revalidated.
  Exit gate wave-32: N-139h fixed and revalidated, revised guide validated + frozen, combat-latency
  teach proven.
- **148** (mono-W Kor Army equipment) -- FIRST-GUIDED, 2/6, CONSTRUCTION-CEILINGED confirmed (+1
  win + quality upgrade = the ceiling the shell allows; all 4 teaches fired). ONE confirming corpus
  then ROTATE with construction hand-off to the roster owner (trim equipment glut, add AM
  survivability + interaction). Guide freezes.
- **122** (WB Eternal Life) -- FIRST-GUIDED PERFECTA 6/6, NEAR-TERMINAL. All 5 teaches
  live-validated, guide FROZEN, 0 fallbacks, 25th core PASS. The only residual (Felidar wingame
  firing, N-122a) is a probe-deck engine task the random pool cannot exercise -- NOT a seat hold.
  EXIT projected wave-31 (a near-terminal confirm seat: re-confirm the frozen guide + carry N-122a
  to a between-wave probe).
- **152** (Bant Midrange GWU) -- Step-0 debut, initial guide authored this wave (deployed). Step-0
  seats never rotate. Werewolf day/night FLIP remains UNEXERCISED (needs a slow/durdle matchup or a
  scripted spell-less turn); N-152a (MDFC play-land both faces) + N-152b (cosmetic-only-option
  suppression) on the ledger.

Rotate-in (1), picked from the guideless roster to MAXIMIZE new decision-surface + archetype/color
coverage vs the holdovers (which are entirely W/B/G/U -- **RED is ABSENT from all six**, and the
covered mechanics are convoke, dungeons/venture, mutate, equipment, lifegain/alt-win, werewolf/
class/walker):

- **deck158 "Orcs of Mordor" (Rakdos RB, LTR amass aggro-attrition).** One-line rationale: it opens
  the **AMASS surface** (a novel token/counter-growth decision -- create-or-grow an Orc Army with
  +1/+1 counters), reliably exercised EVERY game by a non-singleton playset core (Foray of Orcs x4,
  Swarming of Moria x3, Mordor Muster x2, March from the Black Gate x2), PLUS the marquee reactive
  **Orcish Bowmasters x4** (instant-speed ETB ping + amass on opponent draw) and a likely Saga
  (Assault on Osgiliath), all on a **mono-heavy RED/black manabase that adds the pool's entirely
  missing red** and a Rakdos aggro-attrition profile distinct from every holdover. Broadest
  new-surface + color-diversity hit among guideless candidates; non-singleton avoids the
  absent-engine-card (deck102 Tergrid) 1-of whiff risk.
  - Runner-up considered: **deck52 "Magnivore" (RB land-destruction + graveyard-scaled Magnivore)**
    -- adds red and a land-destruction strategic surface, but Magnivore's dynamic P/T is
    already-covered ground and land-destruction opens no new DECISION-RENDER; deck158 wins on the
    genuinely new amass surface. **deck38 "Bad Dreams" (RB punisher)** rejected: its symmetric
    card-draw-punishment (Underworld Dreams / Howling Mine) re-exercises the existing SYMMETRIC-
    EFFECT idiosyncrasy rung (deck102 Pox, deck59 Wrath), not a new surface.

## Guideless-roster accounting

164 total decks. 27 currently carry a `deckN_strategy.txt` (8 14 17 18 21 22 27 35 44 49 59 62 93
102 109 110 122 131 133 134 135 136 137 139 140 146 148). Deploying deck152's initial guide this
wave -> **28 guided, 136 guideless** (matches the brief's "guideless count after 152's deploy =
136"). deck158 becomes the wave-31 Step-0 rotate-in but stays GUIDELESS until its initial guide is
drafted next wave, so the guideless reserve after this wave is **136**. deck136 rotates OUT of the
active pool but KEEPS its guide (converged canary), so it remains counted among the 28 guided.
