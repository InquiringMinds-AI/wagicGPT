# deck131 wave-27 -- dev notes (engine / harness / model / deck-construction; self-contained).

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-193608/`, 6 deck131 games, **3W/3L (3/6)**.
Binary ffec79fd8 (step-1 batch ffec79fd8, -T 3000). Pilot: rep_penalty=1.05. Live guide = wave-26 REVISED
(13264 bytes, deployed == wave26 AND == wave27 byte-verified via cmp exit 0). **This wave: NO guide edit --
strategy.txt FROZEN (byte-copy).** deck131 is deck0 vs 137/93/134/18/22, deck1 vs 136 (see results.tsv).
Two fallbacks (vs134 s39/s40), both benign.

Seat->game map (results.tsv): vs137 WIN(adj) 15/11 T19 | vs93 WIN 20/-2 T14 | vs134 WIN(adj) 32/9 T18 |
vs18 LOSS -6/20 T11 | vs22 LOSS -15/12 T13 | vs136 LOSS -1/4 T17.

Layer-routing: engine/harness/model items live HERE, never in the guide. Deck-construction flags are
RESTATED for the user, not relitigated.

## Reproducible engine/harness/model ledger

### I-1 -- [MODEL-COMPREHENSION, WATCH, guide-unfixable] tapped-read misread: NO RECURRENCE round 5
- ROUND 5 of the Guttersnipe tapped-read guard. Every Guttersnipe attack window this corpus was correct or
  correctly-conservative; NO "attackers-tapped => all-tapped" slip. Cleanest signals: vs137 s43 correctly
  distinguished the tapped Faerie Guidemother / Giant Killer from the UNTAPPED Human (1/1) and held; vs22
  s24 correctly read TWO untapped Mogg Sentries and swung anyway ONLY as a life-2 desperation (dead next
  turn regardless) -- a correct read, not a misread.
- **Guide UNTOUCHED on the per-creature tapped-read guard prose** (sanctioned-against to shout louder at an
  already-explicit line that held). WATCH stays SINGLE-SEAT (deck131 only, 4 prior corpora; 0 this corpus).
  Promote to a cross-seat combat-safety item ONLY if a 2nd SEAT shows the misread. Repro: for
  kind=='attackers' with Guttersnipe among options, diff the reply's claimed tapped-set vs the prompt's
  opponent-battlefield [tapped] tags; flag a reply that claims "all/no untapped" while a creature lacks
  [tapped] AND the swing is not lethal/desperation-justified.

### I-2 -- [GUIDE-DEFECT-FREE; MODEL INCONSISTENCY, WATCH] favorable-block exception applied inconsistently
- The wave-26 favorable-block edit (exception (a): block a power<2 attacker; Guttersnipe survives = free
  removal, not a trade) is LIVE and rules-correct. First-corpus scorecard: FIRED CORRECTLY 3x (vs22 s10
  textbook 1/1-Automaton free kill; vs22 s22 correctly identified the sole favorable block under a 4-creature
  alpha strike; vs22 s18 within-envelope, harmless, minor PT-misread of the 1/3 as 1/1). DECLINED 2x where a
  free kill was on offer: vs137 s41 (Giant Killer 1/2, engine-labeled "you kill it, your blocker lives") and
  vs137 s45 (Giant Killer 1/2 + Human 1/1). In both declines the model reverted to the OLD "never trade
  Guttersnipe" frame ("trading it for a 1/1 or 1/2 is unnecessary"), treating a favorable block as a trade --
  the exact confusion the edit corrects.
- **NOT a guide defect, NOT a fourth corner.** The guide line is correct and was followed when the model
  engaged; the declines are the SAME favorable-block corner, not a new one. Per skill discipline: a WRONG
  model deviation from a RIGHT line = model slipped = WATCH, never shout louder. No guide edit. Likely trigger
  for the decline: an unblockable flyer also attacking ("I take damage anyway, why bother") -> the model
  undervalues PERMANENT removal of a ground attacker (Guttersnipe does not tap when blocking). Both non-
  decisive (vs137 won on adj). Repro: for kind=='blockers' where an attacker power<2 is available AND the
  engine option says "you kill it, your blocker lives", a `BLOCKS: none` reply that calls the block a "trade"
  is this class. Promote to a guide concern ONLY on cross-seat convergence OR if a decline flips a decisive
  game.

### I-3 -- [HARNESS/MODEL, benign, latency WATCH] the two turn-18 priority unparsed fallbacks (vs134 s39/s40)
- Single-option degenerate priority: deck131 stuck (1 blue source, all spells uncastable/target-less, land
  drop used), engine offers ONLY "Life with Elixir of Immortality". Both replies emitted `CHOICE: 2 (Cast
  Opportunity)` -- a NONEXISTENT option (only option 1 exists) restating the stale PLAN -> unparsed_reply ->
  choice=-1. The model IGNORED the harness's explicit stale-plan note ("the actions your plan names are no
  longer among the options available right now - re-derive"). Correct answer was CHOICE: 0 (pass) or 1.
- **s40 = 209,618 ms decode outlier (209.6s).** The model engaged, correctly found Opportunity uncastable,
  then spiralled on a FALSE contradiction (I-4), and STILL emitted the nonexistent option 2. On-task prose
  the whole 209s (no decode-collapse), just a wrong answer-first line.
- **BENIGN + non-decisive:** fallback resolved, Elixir harmlessly activated (pointless lifegain at 27-32
  life; deck-cycled graveyard), s41 attacked cleanly, game won 32/9 (adj). Routing: plan-fixation despite an
  existing correct re-derive note = CORE-prompt behavior, single-seat, below-bar for a core change (shout-
  louder). Latency = WATCH. Repro: kind=='priority' single-option where the model's PLAN names an unavailable
  action and it emits a CHOICE index/name absent from the option list. stderr showed "AIPlayerGPT: dropping
  stale async answer" (4x in vs137) -- the harness async-staleness mechanism, working as intended, related to
  the same plan-stale surface; benign.

### I-4 -- [d1-adjacent + parser-precedence] cross-zone duplicate-name conflation; BLOCKS is NOT answer-first
- **Cross-zone duplicate-name gap (d1-adjacent, model-comprehension).** In the s40 spiral the model declared
  a log-vs-board "contradiction" (Elixir shown both shuffled-to-library in the log AND on the battlefield in
  the current state; Opportunity in the graveyard/library in the log AND in hand). VERIFIED FALSE: turn 15
  deck131 played TWO Elixirs; at turn 19 Elixir #1 shuffled itself + graveyard into library while Elixir #2
  remained on the battlefield (board correctly shows one Elixir + offers it at s40). The hand's Opportunity/
  Downsize/Essence Scatter are distinct copies from the graveyard copies (deck runs 2-3 of each). Board
  render is INTERNALLY CONSISTENT -- the model could not reason that same-named cards in different zones are
  distinct copies. The d1 #N handles fixed BATTLEFIELD collisions (rendered + used correctly this corpus:
  Elixir #1/#2, Prism #1/#2, Urza's Mine #1/#2/#3, Mogg Sentry #1/#2, etc.); they do NOT disambiguate
  same-name copies ACROSS hand/library/graveyard. NOT a d1 regression; a cross-zone limit. Below-bar (single
  incident, non-decisive); surface to synthesis only on cross-seat convergence of a cross-zone same-name
  confusion that flips a decision.
- **BLOCKS parser is NOT answer-first (positive precedence datapoint, vs22 s22).** The model emitted TWO
  contradictory `BLOCKS:` lines: first `BLOCKS: Guttersnipe blocks Sunrise Sovereign` (WRONG -- kills
  Guttersnipe), then reasoned to and re-emitted `BLOCKS: Guttersnipe blocks Stinkdrinker Daredevil` (RIGHT --
  favorable, Guttersnipe survives). The parser resolved to the CORRECT final line (chosen_text = "Guttersnipe
  blocks Stinkdrinker Daredevil"). This is the OPPOSITE resolution to wave-26 I-4, where ATTACK / PUT
  (answer-first) took the model's PRE-reasoning WRONG line. So the parser precedence differs by kind: BLOCKS
  recovers the corrected line; ATTACK/PUT entrench the first line. Recorded so a future reviewer does not
  report the vs22 s22 double-BLOCKS as a parser defect -- it is the parser working WELL. Repro: for
  kind=='blockers', a reply with multiple BLOCKS: lines resolves to the LAST valid one.

### I-5 -- [BELOW-BAR, model obeyed the guide -> no edit] favorable-ATTACK asymmetry
- The guide has a favorable-BLOCK exception (Guttersnipe survives a power<2 attacker) but the attack-hold
  rule has no parallel "swing into a harmless/favorable blocker" nuance. vs137 s43: the model correctly held
  Guttersnipe home vs a lone untapped Human (1/1) that Guttersnipe would kill favorably on the swing -- an
  over-conservative but guide-OBEYING hold. Because the model OBEYED the rule (no correct-deviation signal),
  and adding a favorable-attack clause would be speculative scope-creep for a non-decisive spot, NO edit.
  WATCH only; promote only if a conservative-hold-into-a-killable-blocker costs a decisive game.

## Interface validations (clean this corpus)
- **Mulligan all correct/defensible.** Only 1 bottom record (vs22 s3, kept after 1 mull): bottomed
  Opportunity (rank-3 over-costed {4}{u}{u} with a cheaper dig also in hand), kept red source + both payoffs
  + cheaper Cyclonic Rift + 2 lands. The recalibrated bottom ladder VALIDATED again (rank-3 over-costed).
- **Guttersnipe attack discipline clean** (I-1): held every dangerous untapped-blocker board, swung every
  empty/tapped board, one correct life-2 desperation swing (vs22 s24), one conservative-but-obeying hold
  (vs137 s43, vs136 s21). No dangerous misread.
- **Guttersnipe BLOCK discipline correct + expanded** (I-2): favorable-block edit fired correctly 3x,
  declined 2x (model inconsistency, WATCH), correct never-block holds vs deathtouch (vs136 s19/s23). No
  Guttersnipe traded away when not forced.
- **Attacker/blocker declarations pure-index/name, all parsed** (no name-leak fallback).
- **Two fallbacks** (vs134 s39/s40, both model-side unparsed, benign, I-3). c4 corpus gate: no dropped/
  mis-picked LEGITIMATE reply; the vs22 s22 double-BLOCKS RECOVERED the correct answer (I-4).
- **d1/d2 VALIDATED at this seat** (battlefield #N handles + changeling annotation rendered + used
  correctly); d3/d4/d5 steady/unexercised; no crash/assert/segfault in any stderr.

## DECK-CONSTRUCTION ledger (RESTATED for the user; user owns; not relitigated)
The standing 9+-wave diagnosis holds; the 3/6 is matchup/draw variance over the same construction-capped deck:
- **Threat density 6/60** (3 Guttersnipe + 3 YP). Variance the deck cannot absorb.
- **No reach to CLOSE** -- now visible INSIDE a win: vs134 (control) ground to opp 9 but could NOT finish,
  won only 32-vs-9 at the cap; vs136 lost with opp at 4 (1 short). vs133-style drain/go-wide walls unchanged.
- **No early defense vs aggro** -- vs18 (Kithkin) out-raced 131 by T11; vs22 (Giants, now guided) by T13.
- **The 3 WINS do NOT raise the ceiling:** two adjudications (vs137, vs134 -- ahead-on-life-at-cap grind-outs
  vs slow/controlling decks it could out-durdle) + one free race vs a guideless Rats deck (vs93). Against the
  GUIDED field (137/18/22) 131 went 1W/2L, the win an adjudication.
- **Matchup note:** vs22 flipped from a wave-26 FREE WIN (guideless Giants) to a wave-27 LOSS now that 22 is
  guided (5/6) -- confirms 131's wins lean on weak/un-guided opponents; the guided field is where it caps.
- Sideboard candidates unchanged (Spellheart Chimera reach, Aetherling finisher) for the no-reach axis.
- These are the user's to decide; recorded, not argued.

## For a future deck131 reviewer -- gotchas
- n=6, single run: win-rate is noise; 3/6 here vs 2/6 wave-26 vs 1/6 wave-25 are all the SAME construction-
  terminal deck under variance. Do NOT read the climb as a trend. vs22 FLIPPED win->loss purely because the
  opponent got a guide.
- Positions vary per row: deck131 is deck0 in vs137/93/134/18/22 and deck1 in vs136 THIS corpus. Read
  winner/life columns per record.
- **Guttersnipe BLOCK is CORRECT if** (attacker power < 2 => survives, FAVORABLE) OR (incoming >= my_life AND
  Guttersnipe is the only creature => lethal chump). A power 2+ trade while NOT lethal IS a slip. The model
  applies the FAVORABLE case inconsistently (I-2) -- score a DECLINE of a "you kill it, your blocker lives"
  option as a model WATCH, not a guide defect.
- **BLOCKS parser is NOT answer-first** (I-4): a reply with two BLOCKS: lines resolves to the LAST valid one;
  a first-line-wrong/last-line-right wobble is RECOVERED, not entrenched (contrast ATTACK/PUT answer-first).
- **Single-option priority + stale plan** (I-3): if the model's PLAN names an unavailable action and it emits
  a CHOICE index absent from the list -> unparsed fallback (benign if non-lethal). Correct answer is CHOICE:
  0 or the sole listed option.
- **Cross-zone duplicate names** (I-4): same-name copies in hand vs library vs graveyard are NOT #N-tagged
  (#N is battlefield-only); the model can conflate them into a false "contradiction."

## ROTATION VERDICT

**ROTATE OUT.** Both wave-26 rotation conditions are met with finality.
- **Gate 1 (guide-mod? = candidate):** PASSES -- the guide was NOT modified this wave (frozen, byte-copy
  verified against deployed AND wave26). A revised guide can never be a rotation candidate; a FROZEN,
  re-validated guide can.
- **Gate 2 (new guide-addressable work-signal?):** ABSENT.
  - Favorable-block edit VALIDATED (fired correctly when engaged; never-trade core + lethal exception did not
    mis-fire). The 2 declines = MODEL inconsistency on an already-correct rule = WATCH, NOT a new corner and
    NOT a guide edit (no FOURTH corner-softening -- the whole-corner recalibration is complete).
  - The two priority fallbacks (s39/s40) decompose BENIGN: model plan-fixation + a cross-zone duplicate-name
    false-contradiction spiral; engine render VERIFIED CORRECT; non-decisive (won 32/9 adj). Model/harness
    WATCH + latency flag -> notes, not guide.
  - No new too-absolute corner surfaced; tapped-read guard 0-recurrence (round 5); racer-block deviations
    below-bar; favorable-ATTACK asymmetry is model-OBEYED (no correct-deviation signal). All model/engine
    WATCHes, none guide-fixable.
- **Needed guide edit? NONE.** Every finding routes to notes.md (model/harness/engine WATCHes) or is a
  validation. strategy.txt frozen.

**UNSENTIMENTAL read for the roster owner:** deck131 is CONSTRUCTION-TERMINAL (owner's call, 9+ waves) and the
guide-fixable corner-supply is now EXHAUSTED -- the never-block rule is fully recalibrated (survives-block +
lethal-chump + never-trade), the bottom ladder is complete, and this wave produced ZERO new guide-fixable
decision classes. The wave-26 skill's prediction held exactly: once the corner's full rules-space is stated,
no new corner appears, and the seat's guide-work is done. The remaining residuals (favorable-block
inconsistency, tapped-read stochastic misread, cross-zone duplicate conflation, single-option-priority plan-
fixation) are all MODEL/HARNESS behaviors off the guide track, none decisive, none guide-fixable. The
construction lever (threat density / reach / early interaction) remains the owner's, off the loop. **deck131
ROTATES OUT this wave. Guideless roster candidate to backfill: 141 (per brief).**
