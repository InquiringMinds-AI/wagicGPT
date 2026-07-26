# deck136 (Yarok of Pranks) - findings.md
# Wave-28. FIRST-GUIDED (wave-27 guide deployed, IDENTICAL to wave27/strategy.txt).
# Binary ad8930961. Corpus matchups-20260726-050449. All seq refs = deck136 seat.
# Record 2/6 (down from 4/6 Step-0).

## Record: 2/6
WINS:  vs134 (won, 22 life, opp -2, t15 - full-board beatdown), vs137 (won, 26 life,
       opp 0, t11 - Cavalier closes).
LOSSES: vs146 (adj, 8-16, t22 - long grind vs Orzhov Dungeons value deck),
        vs18 (lost -5, opp 19, t12 - Kithkin go-wide run-over),
        vs22 (adj, 5-10, t15 - Giants ramp race, CLOSE),
        vs93 (lost -4, opp 19, t12 - Relentless Rats go-wide run-over).
Fallbacks at deck136 seat: 0. (Corpus-wide 18 = all OTHER seats: 16 deferred_to_heuristic
+ 2 retracted_choice, none at deck136.) Parser/mana/render clean at this seat.

## e2 PHANTOM-CHOOSER VERIFICATION -> SOLVED (the headline this wave)
Wave-27: 4 fallbacks, ALL the Legion's End phantom hand-target chooser
(R-RESOLVED-SPELL-PHANTOM-CHOOSER); the three LARGEST replies in the whole corpus
(15.9k/15.5k/15.4k) were those refusals.
Wave-28: 0 phantom hand-target asks. 0 fallbacks. Verified three ways:
1. CARD-SCRIPT: borderline.txt Legion's End no longer has the offending 4th line
   `auto=donothing notaTarget(*|targetcontrollerhand)`. The script is now
   target -> moveto(exile) -> same-name exile from battlefield/hand/graveyard, with NO
   chooser-spawning reveal line. The engine fix is visible in the data asset.
2. LIVE CAST WINDOWS: Legion's End cast 6 times (vs22 seq6, vs18 seq6+seq10, vs137 seq7,
   vs146 seq19+seq41). Where 2+ same-name legal targets exist, a target ask fires
   ("TARGET CHOICE ... already on the stack ... pick the ONE target") but it now offers
   ONLY the real opponent-battlefield MV<=2 creatures (Mogg Sentry #1/#2; Goldmeadow
   Dodger/Wizened Cenn; Edgewall Innkeeper #1/#2) - NO hand cards, NO lands, NO "any
   hand" degradation. Model answers cleanly, 0 fallback. Where 1 legal target exists the
   engine auto-selects, no ask. This is the CORRECT target seam.
3. PRIMARY EFFECT INTACT (live narration): same-name exile fires - vs22 exiled BOTH Mogg
   Sentries (battlefield->exile x2); vs137 exiled BOTH Edgewall Innkeepers (x2); vs18
   exiled Wizened Cenn. Destroy + exile-same-name works exactly per Oracle.
Reply-length profile: max now ~13-14k, but the top replies are LEGITIMATE combat/cast
decisions (vs22 seq16 attackers 13.9k; vs18 seq15 blockers 13.4k; vs137 seq2 cast 13.9k),
NOT refusals. The 15k-char refusal-tax CLASS is gone. VERDICT: R-RESOLVED-SPELL-PHANTOM-
CHOOSER RESOLVED at this seat; retire it from the active ledger (keep fix record). Only
residual: the target ask is cosmetically redundant when the two legal targets are
same-name identical (either pick exiles both) - harmless, not a defect, no action.

## GUIDE AUDIT - a teach that ACTIVELY MISLED (the harshest finding, per brief)
The guide's Relentless Rats facts are FABRICATED and INVERT the whole Rats matchup.
GROUND TRUTH: engine Relentless Rats = mana={1}{B}{B} = MV 3 (real Oracle {2}{B} = MV 3
too). The guide asserts MV 4 in THREE places. Consequences, both wrong:
  (a) Guide: "Ritual of Soot MISSES the Rats (MV4)." FALSE. Ritual = "Destroy all
      creatures with mana value 3 or less" -> it KILLS MV3 Rats. The guide steered the
      pilot AWAY from its single best answer to the Rats.
  (b) Guide: "Legion's End ... GREAT against ... Relentless Rats" + names Rats as an
      anti-wide tool. FALSE. Legion's End targets MV<=2; Rats are MV3, immune.
LIVE DAMAGE (vs93): the model's PLAN chased "Legion's End the Rats" for FIVE turns
(t5 seq6 "exile the opponent's Relentless Rats (MV 2)", t7 seq10, t9, t11, t12) - a play
the 601.2c cast filter never offers. It self-corrected once at t9 seq18 ("Legion's End
requires MV<=2, Rats are MV 3") then RELAPSED at t11/t12, and never treated Ritual as the
answer because the guide told it Ritual was dead. Straight wagicgpt-verify-oracle-text
violation carried in from the wave-27 draft (MV asserted from memory). FIXED in this
wave's strategy.txt: Rats = MV3; Ritual SWEEPS them (#1 answer); Legion's End does NOT hit
them; go-wide matchup split into a Rats section and a Kithkin/tokens section (where
Legion's End IS correct). Also softened "you almost never lose the long game" (falsified
by vs146) and added a value-mirror matchup line + an Ugin cast nudge.

## 4/6 -> 2/6 DECOMPOSITION (per-matchup + per-decision, NOT from the aggregate)
Stable across both waves: WINS vs134, vs137; LOSSES vs22, vs93. The 2-game drop is TWO
independent swings, NEITHER caused by deck136's guide degrading play:
  SWING 1 (PURE POOL): deck131 rotated OUT (a wave-27 WIN); deck146 (Orzhov Dungeons)
    rotated IN and won (adj 16-8, 22-turn grind). Net -1 = pool composition, not play.
  SWING 2 (OPPONENT GOT STRONGER + VARIANCE): vs18 flipped 35-1 dominant WIN (w27) ->
    -5 LOSS (w28). deck18 went 6/6 THIS wave (first-guided perfecta) - it improved with
    its OWN guide. deck136 didn't draw Ritual, got run over by Kithkin. Net -1 =
    opponent's guide + variance, not deck136's.
The guide's Rats error is REAL but did NOT cause the drop: vs93 was a LOSS in w27 too, and
this game the model never drew Ritual to be misled about holding it - the error cost
planning cycles, not the game. It COULD cost a future game where Ritual is in hand and the
pilot benches it believing it dead. That is why the fix ships regardless of the record.
GUIDE FLOOR HELD in wins/close games: removal cast + targeted well every game; attack
floor active where profitable (vs134 escalating full-board beatdown to opp -2; vs137
Cavalier + Dread Presence close t11); defensive holds correct (vs18 t11 / vs93 t11 "no
attackers" at 3 and 11 life vs lethal go-wide boards = right, not passivity).

## Loyalty / Rankle surfaces (wave-27 carry-forward, exercised this corpus)
- LILIANA LOYALTY: NOW SURFACES. vs22 seq32 + seq39 activated "+1: create a 2/2 Zombie" -
  render clean, decision guide-aligned (default +1; vs22 Giants not go-wide so -4 rightly
  not needed). Carry-forward RESOLVED for +1. The -4 anti-wide still had no live trigger.
- RANKLE: cast vs134 seq38 (t13), attacked t13 + t15 (flying/haste). The "choose any
  number of modes" combat-damage MAY-ASK did NOT surface (game ended t15; Rankle may have
  been blocked, or the trigger did not fire). Still UNVERIFIED - carry forward, light
  notes.md watch.
- UGIN: never cast (offered vs146 seq41, model chose Legion's End). Fully unexercised;
  added a guide cast nudge.

## Land tags (e4) at this seat: CLEAN
Mono-black; land options render fine, all mulligans "Keep this hand", zero land-tag noise
or color confusion. Negative confirmation (no derived-mana/color confound here).

## Layer routing summary
- Engine: R-RESOLVED-SPELL-PHANTOM-CHOOSER RESOLVED (retire; fix record notes.md).
  Rankle-connect may-ask surface = new light WATCH -> notes.md.
- Guide: Rats MV fix, go-wide matchup split, long-game overconfidence softened,
  value-mirror line, Ugin nudge -> strategy.txt.
- Skill: guide-vs-pool attribution + fabricated-MV-caught-by-decision-audit -> skill.md.
- Core: no signal -> PASS #23 (general-suggestions.md).
