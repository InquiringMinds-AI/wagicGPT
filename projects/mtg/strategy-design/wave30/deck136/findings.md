# deck136 (Yarok of Pranks, 4/6) - wave30 findings.md
# Binary 6e168aa05. Corpus matchups-20260726-181642 (21 games). g2 seat.
# All seq refs = deck136 seat. Record 4/6 (DOWN from 5/6 w29 - decomposed Job 4).
# Guide FROZEN (byte-identical to wave29; cmp-verified vs wave29 AND vs deployed).

## RECORD: 4/6
WIN  vs146  clean lethal   (0, 22 / -2,  t12)  - the ONLY clean kill this corpus.
WIN  vs139  adj, thin      (adj0, 20 / 18, t4) - opp stalled/adjudicated turn 4;
                                                  8 seat records total. A near-forfeit
                                                  adj, not a conversion.
WIN  vs137  adj grind      (adj1, 137=10 / 136=12, t12)
WIN  vs152  adj grind      (adj0, 28 / 5, t15)  - Ugin cast + -3 fired here (see Job 3).
LOSS vs148  close race     (1, 0 / 5, t17)      - N-136a fired t8 + lost aggro race.
LOSS vs122  ground out     (adj1, 136=14 / 122=39, t26) - out-valued by the 6/6 perfecta.
Fallbacks at deck136 seat: 0 (ZERO). Parser/mana/tapped-tags clean throughout.

## JOB 1 - g2 VERIFICATION (PUT-predicate filter). VERDICT: **FALSIFIED at this seat.**
## N-136a is STILL LIVE on binary 6e168aa05. The phantom-tutor class is NOT dead.
The brief predicted "tutor windows should be clean; the phantom-tutor class dead." They
are NOT. The reveal seam STILL offers the ENTIRE library, unfiltered by the option's
target predicate. Four Mausoleum Secrets reveals this corpus, option counts 42/38/43/39 -
i.e. near-whole-library (vs152's list literally numbered 1-43 incl. ~20 Swamps + Ugin +
Liliana). If the g2 filter had landed, an MV<=(creatures-in-gy) predicate would have cut
these to a handful (and to ZERO / no-ask in the vs152 empty-graveyard case). It did not.

The four reveals, and whether the tutored card actually reached hand (the real N-136a harm
is a SILENT no-op when the model picks an over-MV card the engine's target filter rejects):

  seq/turn        chose                   opts  reached hand?   legal?
  vs148 s22 t8    Rankle (MV4)            42    NO -> no-op     illegal (gy short)  <- HARM
  vs137 s27 t11   Yarok's Fenlurker (MV2) 38    YES (seq28)     legal
  vs152 s12 t6    Ritual of Soot (MV4)    43    NO -> no-op     illegal (0 creat gy)<- HARM
  vs122 s35 t16   Ritual of Soot (MV4)    39    YES (seq36)     legal (gy stocked by t16)

So the bug FIRES 2 of 4 reveals (vs148, vs152) - exactly when the model reaches for its
best on-color card and the graveyard is not yet stocked, which is the class the constraint
forbids early. It "works" only by luck when the model happens to pick a card whose MV is
already within the graveyard gate (vs137 Fenlurker MV2; vs122 Ritual MV4 at turn 16 with
4+ dead creatures). The empty-eligible NO-ASK path was NOT hit: vs152 had 0 creatures in
graveyard (0 legal cards) yet the whole 43-card library was offered and the model picked an
illegal Ritual (its own PLAN even read "will find no targets with zero creatures in the
graveyard" - it knew, and was still handed the illegal menu).

  Note contrast: game-137v136 stderr shows FILTERED reveals ("reveal put 1 of 5") - but
  those are deck137's OWN seat / a DIFFERENT reveal card, NOT deck136's Mausoleum Secrets.
  So SOME reveal path may be filtered in 6e168aa05; Mausoleum Secrets' reveal-zone path is
  NOT. -> notes.md N-136a (re-confirmed still-live), engine-lane.

RATS MATCHUP: **cannot be evaluated in-corpus - deck93 (Relentless Rats) rotated OUT this
corpus (canaries 19).** The flagship guide-target matchup was not played. It remains UNWON
by absence, and the bug that blocked it last wave (N-136a) is confirmed still live. To
close it, re-add deck93 transiently AFTER the N-136a seam fix lands (or validate the fix
with a probe deck) - it does not require deck136 in the live rotation.

## JOB 2 - RANKLE MODE-BLEED WATCH (wave-29 light watch). VERDICT: CLEAN at this seat.
The brief noted the 2 priority fallbacks this corpus are at OTHER seats; confirmed - deck136
seat has ZERO fallbacks (0 unparsed, 0 retracted, 0 defers). Rankle connected and its
"choose any number" trigger surfaced as sequential per-mode opt-in asks in vs137 (seq14 t7
-> "sacrifice") and vs152 (seq17 t8 -> "sacrifice"); both navigated correctly, and NEITHER
bled its mode frame into a following priority window (the w29 seq39-style bleed did not
recur). The modal->priority context-bleed watch stays a watch (no second witness at this
seat); carry.

## JOB 3 - UGIN. VERDICT: **CAST AT LAST (4th wave) - carry-forward RESOLVED.**
After 3 waves fully unexercised, Ugin FIRED in 2 of 6 games:
  vs137 seq32 t13: "Cast Ugin, the Ineffable {6}".
  vs152 seq27 t12: "Cast Ugin {6}", THEN seq28 t12 activated
        "-3: destroy target colored permanent ... targeting Intrepid Adversary" - a CORRECT
        pick (destroys the opponent's colored threat; Intrepid Adversary is a colored
        permanent, legal -3 target). vs152 was won (28/5, t15).
Both games it was cast on a stalled/grindy board at 6 mana - exactly the guide's nudge
("If you reach 6 mana with the board stalled, CAST it"). The guide's Ugin line VALIDATED.
The 3-wave "Ugin never cast" carry-forward is retired. This removes the one surface that
was holding the seat open (see Job 6).

## JOB 4 - 5/6 -> 4/6 READ + STEADY-STATE AUDITS.
The drop is a POOL-HARDENING artifact, not a regression:
  - vs122 flipped WIN(w29 adj)->LOSS(w30 adj). In w29 deck122 was a Step-0 guideless DEBUT
    (2/6, one of the field's weakest); this wave it is the FIRST-GUIDED 6/6 PERFECTA
    (strongest deck in the pool). deck136 was out-VALUED and out-LIFEGAINED (opp climbed to
    39 over 26 turns while 136 ground to 14) - EXACTLY the guide's own warning ("vs ANOTHER
    value/attrition deck you can be OUT-GROUND"). Correct-shape loss to a superior engine,
    not a misplay. The w29 "win" over 122 was always soft (adj vs a weak debut deck).
  - vs148 flipped WIN(w29 adj)->LOSS(w30). Close aggro race (Kor allies): N-136a wasted the
    t8 Mausoleum Secrets (phantom Rankle, seq22, never reached hand) - a real tempo tax -
    but the pilot cast Ritual of Soot from hand anyway (seq26 t10) and simply lost a narrow
    attrition race (life 20->12->9->5->0, opp 17->10->5). N-136a contributed; not clearly
    decisive.
  So 5/6->4/6 is a WASH: two soft w29 adj-wins over then-weak decks turned into two
  correct-shape losses (one to a now-tuned superior deck, one a close race with an engine
  tax). Net honest strength unchanged.
  STEADY STATE audits, all clean:
  - Liliana: not activated this corpus (no game reached a spot where a walker mode was the
    play; both modes already confirmed live in w28/w29 - not a new gap).
  - Removal priority: correct - Legion's End on MV<=2 targets (Armament Master, Stone Haven
    Pilgrim vs148); Ugin -3 on the biggest colored threat (Intrepid Adversary vs152); no
    misfires observed.
  - Attack floor: fired appropriately - Knight/Fenlurker/Rankle attacked when profitable;
    vs146 closed lethal t12; no observed "sit on a full grip" stalls.

## JOB 5 - g3/g4/PARSER.
  - g3 verb-labeled loot asks: NOT EXERCISED at this seat - mono-black attrition has no
    self-loot/discard. Yarok's Fenlurker's ETB is an OPPONENT-exile (not a self-discard);
    Dread Presence's draw mode is a may-choice, not a discard. No loot decision misread as
    targeting (none fired). The Rankle discard/sacrifice modes (Job 2) DID render verb-first
    ("sacrifice" / "discard" as the option text) with CR-725 role context and resolved
    cleanly.
  - g4 tapped-tag: the reworded `[tapped - cannot attack or block this turn]` renders across
    all games (14/30/7/38 occurrences), and composes coherently with combat state
    (e.g. vs137 seq14: "Rankle ... [tapped - cannot attack or block this turn] [attacking]"
    - tapped-from-attacking, correctly co-tagged). No w29 tapped-misread class recurrence.
  - PARSER CONTRADICTIONS: none found. No mana-miscount wobble this corpus (the w29 vs93
    transient did not recur - Rats not in pool). Reply lengths within norm; no phantom-
    refusal tax.

## JOB 6 - ROTATION. VERDICT: **ROTATE OUT** (unsentimental; new-work exhausted). See notes.md.
The brief's gate: "if the ONLY unexercised surface left is Ugin, apply the new-work-
exhaustion doctrine - a single uncast card does not hold a seat." Ugin is now CAST and its
guide line VALIDATED (Job 3), so even the one held surface is resolved. Every guide-layer
surface is now exercised and executed correctly: MV3/Legion's-End valuation (w29), Mausoleum
Secrets constraint (articulated correctly, e.g. vs152 plan), Liliana both modes (w28/w29),
Rankle modes (w29/w30), Ugin cast + -3 (w30). Guide FROZEN, byte-identical, cmp-verified.
Zero fallbacks, clean parser. The ONLY remaining item is N-136a - an ENGINE/SEAM bug
(routes to the ledger; probe-deck validatable) - and the Rats matchup, which is NOT IN THE
POOL. Keeping deck136 in the live rotation to re-observe an engine bug that a targeted probe
deck validates better is exactly the sentimentality the doctrine forbids. Rotate out; work
N-136a from the ledger; re-add deck93 transiently to close the flagship matchup once the
seam fix lands.

## Layer routing
- Engine: N-136a re-confirmed STILL LIVE on 6e168aa05 (g2 did not land for Mausoleum
  Secrets' reveal-zone path); the "1 of 5" filtered reveal at deck137's seat suggests the
  filter exists on SOME path -> notes.md, with the path distinction + repro.
- Guide: FROZEN. No edits (byte-copy cmp-verified). Nothing left to teach.
- Skill: g2-validation-negative + Ugin-resolution + "soft adj-win over a weak debut deck
  reverses when that deck is later tuned" record-read lesson -> skill.md.
- Core: no core-prompt defect at this seat -> PASS #25 (general-suggestions.md).
