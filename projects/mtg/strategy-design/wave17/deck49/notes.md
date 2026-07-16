# notes.md — deck49 (Dragons) engine/representation items + rotation verdict (wave 17)

## Engine / representation ledger items

### E-49a — stale_echo matcher: player-target labels defeat the echo match (WRONGFUL downgrade).
deck21 s4: target sub-menu option "The opponent (player, life 20)". The model's reply
`CHOICE: 1 (Cast Lightning Bolt targeting The opponent)` carries the correct index AND the token "The
opponent", yet routed to stale_echo→heuristic. The "(player, life 20)" suffix on the canonical label
means the echoed bare token "The opponent" is neither an exact match nor a clean substring of the full
label, so the matcher misses it. Fix candidate: when matching an echo against a PLAYER-target option,
strip/ignore the "(player, life N)" suffix (life is volatile and not identifying), or match on a stable
target token. Non-fatal here (index was also correct; heuristic bolted face; game won) but it is a real
wrongful downgrade of a correct answer. Reproduce on /tmp/wagic-7cdcf9c73 before changing the matcher.

### E-49b — target sub-menu: the model does not recognize it is on a target menu (drives ALL 7 echoes).
Across deck35 s11, deck17 s13, deck110 s9 the model echoes the SPELL name ("Goblin War Paint") as its
parenthetical on a target sub-menu instead of the target, and in deck135 s4 it invents "Cast nothing"
on a mandatory target menu. Representation candidate: label the target-sub-menu prompt/options so the
menu ROLE is explicit (e.g. a header "Choose a target for Goblin War Paint:" and/or option lines that
carry the pending spell). This is the engine-side companion to core proposal G1 (which teaches the flow
in prose). Two options are complementary; the label is the more robust of the two.

### E-49c — beneficial aura offered on OPPONENT-only targets should be suppressed (fatal loop source).
deck17 s18 (unparsed, 12,406 tax) and deck135 s22 (unparsed, 13,831 tax) both burned a whole decision
because Goblin War Paint ({1}{R}, +2/+2 + haste) was offered while its ONLY legal targets were opponent
creatures — the model looped hunting for a use for a buff it would never want to give the enemy. The
existing own-target-only BAD-effect suppression (this binary's wave-16 batch) has a MIRROR gap: a
BENEFICIAL aura/pump/haste cast whose legal targets are ALL opponent-controlled is one the pilot should
never take, and offering it only produces loops. Candidate: extend the cast filter to suppress a
beneficial-aura cast when every legal target is opponent-controlled (symmetric to the bad-effect rule).
Verify against Oracle intent first — War Paint CAN legally target an enemy creature; the suppression is
a "never-useful, so don't offer" heuristic, same justification as the bad-effect side.

## Card-script divergences (Oracle-verification queue, per wagicgpt-verify-oracle-text)
The GUIDE uses the ENGINE values above; these are flagged only for a future card-data pass. None are
behavior-breaking for the guide, but a couple mis-state costs/stats vs. real MTG:
- Bloodmark Mentor: engine power=1 (mtg.txt), real card is 2/1. (Guide says 1/1 — matches engine.)
- Goblin War Paint: engine {1}{R}, +2/+2 + haste; real card is {R}, +1/+1 + haste. Cost AND buff differ.
- Crucible of Fire: engine {3}{R}; real card is {2}{R}{R}. (Same CMC 4, different pips.)
- Rorix Bladewing: engine power=6, toughness unconfirmed in scan (real 6/4). Verify toughness.
- Ashenmoor Liege: engine mana {1}{BR}{BR}{BR} and "opp targets it → opp loses 4"; real card is
  {B/R}{B/R}{B/R}{B/R} and "when it is DEALT damage, deals that much to you" (a different, self-harm
  trigger). Engine turned the drawback into a punisher-on-target. Verify against Gatherer before any
  card-script edit; the guide describes the ENGINE behavior (all-red anthem).
None of these were mis-executed in the corpus in a way that lost a game; queue, do not rush.

## Rotation verdict
NEWCOMER — no rotation. deck49 entered this cycle GUIDELESS; writing its initial guide (strategy.txt)
IS the new-work signal by construction. Per the brief, newcomers effectively never rotate on entry.
The guide is a HYPOTHESIS to be validated next corpus (Step 0-ter): watch whether C1 (mana belief),
C2 (target sub-menu), C4 (War-Paint loops) and C5 (dead Crucible) die, and whether the reasoning-tax
at the cast/target seams collapses. Flag for that review: deck49's losses were substantially
deck-CONSTRUCTION (top-heavy Dragon curve — only two {6} finishers, thin 3-cost body slot, mana-screwed
holding uncastable dragons in 2 of 3 losses), so judge the guide by taught-class death and tax collapse,
not by the win-rate, which the pool swap and draw variance will move more than the guide will.
