# deck136 (Yarok of Pranks) - wave30 notes.md (engine/harness ledger)
# Binary 6e168aa05. Corpus matchups-20260726-181642. Layer: engine/seam items.
# NONE of this becomes guide prose (the guide half is already mitigated + frozen).

## N-136a RE-CONFIRMED STILL LIVE on 6e168aa05 (g2 did NOT land for this path).
## SEVERITY: MEDIUM-HIGH. The reveal/PUT seam offers the WHOLE library, unfiltered by the
## active option's target predicate; illegal picks silently no-op.
The wave-29 fix batch (g2 "PUT-predicate filter - tutor reveals offer only predicate-legal
cards; empty -> no ask") was expected to close N-136a. At the deck136 seat it did NOT.
Four Mausoleum Secrets reveals this corpus, option counts 42/38/43/39 = near-whole-library.
Two of four (vs148 Rankle MV4 turn 8; vs152 Ritual MV4 turn 6 with 0 creatures in
graveyard) picked an over-MV card the option's target predicate forbids; the engine target
filter rejected it, the transform's moveto(hand) never fired, the card returned to library,
and NO error surfaced to the model. In vs152 the model's OWN plan read "will find no targets
with zero creatures in the graveyard" - it understood the constraint and was still offered
the full illegal menu (including Ugin, Liliana, ~20 Swamps). The empty-eligible NO-ASK path
was NOT reached (vs152 had zero legal cards yet a 43-option ask was posed).

MECHANISM (unchanged from w29). Mausoleum Secrets (bin/Res/sets/primitives/borderline.txt):
  auto=name(search card) Reveal:type:*:mylibrary revealzone(mylibrary) optionone
    name(choose card) target(<1>*[black;manacost<=type:creature:mygraveyard]|reveal)
    transforms((...moveto(hand))) optiononeend optiontwo name(shuffle) ... revealend
The AIPlayerGPT reveal seam builds its PUT option list from the revealZone (ALL revealed
cards) and does not apply optionone's target predicate `[black; manacost<=type:creature:
mygraveyard]`. Violates the architecture invariant "illegal choices are structurally
impossible (options come from the engine's legal set)".

NEW DIAGNOSTIC THIS WAVE - a filtered reveal path DOES exist in 6e168aa05:
game-137v136-1785112555.stderr contains "AIPlayerGPT: reveal put 1 of 5" and "1 of 46"
alongside deck136's "1 of 38". The "1 of 5" is a SMALL (filtered-looking) option set at
the OTHER seat (deck137's own reveal card), whereas deck136's Mausoleum Secrets reveals log
42/38/43/39 (whole library). READ: the g2 filter may be implemented for the "search"-style
reveal path but NOT for Mausoleum Secrets' `Reveal:type:*:mylibrary` revealzone path. The
fix must cover the revealzone-based reveal seam, not only the search path.

REPRO (from corpus, no new run needed; archived binary /tmp/wagic-6e168aa05 if present):
- vs148 seat 1785107806-...-vs-ai_baka_deck148.jsonl seq22 (reveal, opts=42), chose Rankle
  (MV4); stderr game-136v148-1785107803.stderr "reveal put 1 of 42 to option one in one
  reply"; Rankle never appears in a later "Your hand:" line -> silent no-op.
- vs152 seat 1785113021-...-vs-ai_baka_deck152.jsonl seq12 (reveal, opts=43), 0 creatures
  in graveyard, chose Ritual (MV4); stderr "reveal put 1 of 43"; Ritual never reaches hand.
- CONTRAST (works when legal): vs137 seq27 chose Fenlurker (MV2) -> reached hand seq28;
  vs122 seq35 chose Ritual (MV4) at turn 16 with 4+ dead creatures -> reached hand seq36.

FIX (engine/seam layer, unchanged recommendation, now with path scope): when building PUT
options for a reveal decision that carries an option-level target predicate, filter the
offered cards by that predicate (reuse the WParsedInt / target-legality machinery the
cast/target seams already use). Cover the `revealzone(mylibrary)` path, not just the search
path (the "1 of 5" evidence shows one path is already filtered; Mausoleum Secrets' is not).
If the constrained set is EMPTY, do not offer the reveal choice - auto-take optiontwo
(shuffle) so the model is never asked to pick from an impossible set. Reproduce on the
current binary first. GUIDE half already mitigated + frozen; the seam must stop offering
illegal options.

## LIGHT WATCH (carry, no new witness this wave): modal->priority context bleed.
The w29 vs122 seq39 unparsed_reply (priority window answered with a Rankle-mode reply) did
NOT recur at this seat (0 fallbacks). Rankle modes surfaced clean in vs137/vs152 with no
following-priority bleed. Keep as a watch; do not build until a second seat reproduces it.

## Carry-forward decision classes (status)
- UGIN: RESOLVED this wave - cast vs137 (seq32 t13) and vs152 (seq27 t12, then -3 on
  Intrepid Adversary seq28). Retire from carry-forward; the guide's Ugin nudge validated.
- LILIANA both modes: already RESOLVED w28(+1)/w29(-4); not activated this corpus but not a
  new gap. Stays retired.
- RANKLE combat-damage may-ask: RESOLVED w29; re-confirmed clean w30 (vs137/vs152). Retired.

## Corpus integrity (deck136 seat)
6 games, ZERO fallbacks (0 unparsed, 0 retracted, 0 defers). Parser/mana/tapped-tags clean.
No phantom-refusal tax. g4 tapped-tag renders coherently with combat state. Nothing new for
the harness.

## Oracle-verification note (routed per wagicgpt-verify-oracle-text)
No new guide MV/text claims this wave (guide frozen). N-136a constraint re-verified off
borderline.txt auto= unchanged from w29: `target(<1>*[black;manacost<=type:creature:
mygraveyard]|reveal)` = black card, MV <= number of CREATURE CARDS in graveyard. Ritual MV4,
Rankle MV4, Fenlurker MV2. The defect is the SEAM not enforcing the constraint, not the
constraint itself.

## ROTATION VERDICT: ROTATE OUT (unsentimental). See findings Job 6.
Guide validated across every surface + FROZEN (byte-identical, cmp-verified) + Ugin now
exercised + zero fallbacks = guide/skill/core new-work EXHAUSTED at this seat. The only live
item is N-136a, an engine/seam bug that routes to the ledger and is better validated by a
targeted probe deck than by a live rotation slot; the flagship Rats matchup is not even in
the pool (deck93 rotated out). Rotate deck136 out; fix N-136a from the ledger; re-add deck93
transiently + probe-validate the seam fix to close the Rats matchup at last.
