# deck136 (Yarok of Pranks) - notes.md (engine/harness ledger items)
# Wave-29. Binary 50eeed8be. Corpus matchups-20260726-115018.
# Layer-routing: engine/card-script/harness items. NONE become guide prose.

## NEW - N-136a: the REVEAL/PUT seam offers ILLEGAL tutor options (whole library, not the
## card-script's constrained set). RECURS in 3 of 6 games. Directly cost the Rats game.
SEVERITY: MEDIUM-HIGH (silently loses a tutor's value; the pilot believes it succeeded
and mis-plans around a card it never got). Violates the architecture invariant "illegal
choices are structurally impossible (options come from the engine's legal set)" - at the
reveal seam that invariant does NOT hold.

MECHANISM. Mausoleum Secrets (bin/Res/sets/primitives/borderline.txt):
  auto=name(search card) Reveal:type:*:mylibrary revealzone(mylibrary) optionone
    name(choose card) target(<1>*[black;manacost<=type:creature:mygraveyard]|reveal)
    transforms((...moveto(hand))) optiononeend optiontwo name(shuffle) ... revealend
The `Reveal:type:*:mylibrary` correctly reveals the WHOLE library. optionone's TARGET
predicate `[black; manacost<=type:creature:mygraveyard]` is the legal filter (black card,
MV <= number of creature CARDS in your graveyard). The AIPlayerGPT reveal seam builds its
PUT option list from the revealZone (all revealed cards) and does NOT apply the active
option's target predicate. So the model is offered every card in the library as a legal
PUT, including cards the constraint forbids. It picks one; the engine's target filter
rejects it; the transform's moveto(hand) never fires; the card returns to library. No
error surfaces to the model - the tutor silently no-ops.

REPRO (from corpus, no new run needed):
- vs93 seat 1785090615-...-vs-ai_baka_deck93.jsonl seq10 (cast Mausoleum Secrets),
  seq11 kind=reveal, options=43 (int; the whole library), chose "Ritual of Soot" (MV4)
  with 0 creatures in graveyard. stderr game-93v136-1785090611.stderr:
  "AIPlayerGPT: reveal put 1 of 43 to option one in one reply" then NO move-to-hand.
  Ritual never in hand thereafter (seq16/seq23 hand = 2x Legion's End, Swamp, Liliana).
- vs139 seat seq16: tutored Rankle (MV4); Rankle never cast (count 0) - same failure.
- vs148 seat seq22: tutored Rankle (MV4); Rankle never cast (count 0) - same failure.
Pattern: the model always reaches for its best on-color card (an over-MV bomb), which is
exactly the class the constraint forbids early, so the seam's missing filter bites nearly
every time Mausoleum Secrets fires before the graveyard is stocked.

FIX (engine/seam layer): when building PUT options for a reveal decision that carries an
option-level target predicate, filter the offered cards by that predicate (the same
`WParsedInt`/target-legality machinery the cast/target seams already use for
"structurally impossible illegal choices"). If the constrained set is EMPTY, do not offer
the reveal choice at all (or auto-take optiontwo/shuffle) so the model is never asked to
pick from an impossible set. Reproduce on the current binary first (archived
/tmp/wagic-50eeed8be) before touching the seam. GUIDE half is mitigated separately in
strategy.txt (the pilot is told not to over-tutor for Ritual early) but that does NOT
remove the engine defect - the seam must not offer illegal options.

## NEW LIGHT WATCH: priority window mis-read as a continuation of a preceding sequential
## modal (Rankle mode) sequence -> unparsed_reply.
vs122 seat seq39 (priority T12, Combat damage): pilot answered a PRIORITY window
(pass / activate Castle Locthwain) with a Rankle-mode answer ("CHOICE: 2 (Choose
\"discard\")"), reasoning it was still resolving "a sequence of Rankle's triggered
abilities." Rankle's "choose any number" is decomposed into N sequential per-mode opt-in
asks (see findings Job 4); after the last mode ask, the following priority window inherits
the mode-selection frame in the model's context. No legal option matched -> unparsed_reply
-> Baka answered (benign; game won). Not a confirmed defect. Cheap partial mitigations if
it recurs: (a) a terminal marker on the last Rankle mode ask ("this is the last choice for
Rankle's trigger; the next question is a new decision"); or (b) narrate the trigger's
completion as an event so the next window's context is clearly post-Rankle. Carry as a
watch; do not build until a second seat shows the same modal->priority bleed.

## Carry-forward decision classes (status update)
- LILIANA -4 (anti-wide): RESOLVED this wave - vs93 seq25 activated "-4: each player
  sacrifices two creatures", clean render, correct pick vs 4 Rats. Both +1 (w28) and -4
  (w29) now confirmed live. Retire from carry-forward.
- RANKLE combat-damage MAY-ASK: RESOLVED this wave - surfaced vs122 as sequential per-mode
  opt-in asks; reaches the interactive seat; pilot navigates it correctly. Retire the
  wave-28 WATCH (superseded by the new priority-bleed light watch above).
- UGIN, the Ineffable: STILL never cast (all 6 games). Fully unexercised for a 3rd wave.
  Guide nudge present. Re-check next wave; candidate to rule "seat-structural
  under-casting" if it fails to fire once N-136a and the Rats matchup are sorted.

## Corpus integrity (deck136 seat)
6 games. 2 fallbacks (both priority, both benign - stale-drop + modal-bleed, decomposed
in findings Job 5). Parser/mana otherwise clean; the one mana-miscount (seq16 vs93) was a
transient in-reply wobble that self-corrected, not a parser fault. f3 sick-tag renders.
Reply lengths within norm; no phantom-refusal tax (the wave-27 class stays gone).

## Oracle-verification note (routed per wagicgpt-verify-oracle-text)
Mausoleum Secrets constraint read directly off borderline.txt auto=:
`target(<1>*[black;manacost<=type:creature:mygraveyard]|reveal)` = black card, MV <= number
of CREATURE CARDS in graveyard (matches Oracle "Undergrowth"). Ritual of Soot mana={2}{B}{B}
= MV4; Rankle mana={2}{B}{B} = MV4; Dread Presence {3}{B} = MV4; Cavalier {2}{B}{B}{B} = MV5;
Command/Liliana {4}{B}{B} = MV6; Ugin {6} = MV6. Guide MV claims verified against these.
No engine defect in the CONSTRAINT itself - the defect is the SEAM not enforcing it (N-136a).

## ROTATION VERDICT: KEEP (not a wave-30 rotation candidate). See findings Job 6.
Real guide modification this wave (Mausoleum Secrets constraint) + a new engine ledger
item (N-136a) that actively costs the flagship matchup + Ugin still unexercised = live
new-work signal. The MV3 fix validated at the decision layer but the Rats matchup remains
UNWON pending N-136a. One more validation wave minimum before the frozen-guide exit shape.
