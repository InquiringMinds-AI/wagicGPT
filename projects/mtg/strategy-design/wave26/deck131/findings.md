# deck131 -- wave 26 review (UR Guttersnipe spellslinger). Two CORRECT-DEVIATION edits' first corpus + Guttersnipe guard round 4.

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-115216/` (binary 77e91ef4e, step-1 batch
79e39511f + 77e91ef4e, -T 3000, REPPENALTY=1.05). deck131 = 6 games. Live guide = the wave-25 REVISED guide
(12161 bytes; deployed == wave25/deck131/strategy.txt, byte-verified via `cmp` exit 0 -- so BOTH wave-25
edits, bottom-step recalibration + lethal-only-blocker exception, are LIVE this corpus for the first time).

**Record: 2 W / 4 L (2/6, up from 1/6).** Seat->game map (results.tsv; deck131 is deck0 vs 133/22/137/18,
deck1 vs 102/27 THIS corpus -- read my_life per record, columns vary):

| result | vs (deck) | end life me/opp | turn | one-line cause |
|---|---|---|---|---|
| **WIN (death)** | 102 (Tergrid/Doomsayer/Animate Dead) | 19 / 0 | 15 | two Guttersnipe stuck; FAVORABLE-blocked the recurring Doomsayer twice (kept Gut + life 19), ground opp 18->0; last wave this was a 1-life LOSS -- flip on draw variance + piloting |
| **WIN (death)** | 22 (Giants, mono-R ramp, Step-0 guideless) | 20 / 0 | 12 | curved YP+Gut, opp near-empty board all game, raced 20->0 taking ZERO damage; free win vs a slow guideless deck |
| LOSS (adj) | 137 (Selesnya Adventures) | 1 / 8 | 12 | FOUR straight no-red mulls -> mull-to-3; crippled hand never stabilized vs go-wide; last wave this was the WIN |
| LOSS (death) | 18 (Kithkin anthem aggro, Step-0, 4/6) | 0 / 15 | 19 | NO Guttersnipe drawn all game; single-YP token race out-paced by wide anthem board + first strike |
| LOSS (death) | 133 (Phyr. Asphodel drain) | -7 / 13 | 25 | 25-turn grind; no reach vs drain wall |
| LOSS (death) | 27 (Inkfathom unblockable + go-wide) | -6 / 14 | 16 | held Guttersnipe correctly all game, no reach; lost the race; lethal-only-block fired correctly at the end (s31) |

**Bottom line: the record is CONSTRUCTION-TERMINAL, unchanged from the standing 9+-wave diagnosis**, now
expressed at 2/6. The two wins are a durdling Tergrid deck (variance flip from last wave's 1-life loss) and
a slow guideless Giants deck -- not a construction change. The four losses are the known limits (no reach
vs drain/go-wide, no early defense, threat-density variance: vs18 drew no Guttersnipe, vs137 mulled to 3).
Zero fallbacks corpus-wide (cleanest deck131 corpus, again). Every game fully piloted.

**Both wave-25 edits VALIDATED this corpus (first exercise):**
1. **Bottom-step recalibration** -- vs102 s3 bottomed **Elixir of Immortality FIRST** (the rank-(1)
   lifegain-first piece that went UNTESTED last wave); vs137 s6 (mull-to-3) explicitly cited the
   recalibrated ladder ("surplus lands... over-costed spells... redundant duplicates") to drive its bottom.
   The guide NO LONGER fights the model's correct play. (A self-coherence wobble on vs137 s6's 4th card --
   below, non-decisive, NOT a parser or guide defect.)
2. **Lethal-only-blocker exception** -- vs27 s31: at 7 life facing 8 lethal power (2 unblockable Inkfathom
   + 2 blockable Zombies), Guttersnipe the ONLY creature, the model blocked the biggest blockable to
   survive to 1. Textbook exercise of the exception. **CORRECT.**

**ONE new guide-fixable finding this corpus (same CORRECT-DEVIATION shape as wave-25):** the never-block
absolute is STILL too tight after the wave-25 lethal edit -- it does not cover a FAVORABLE block where
Guttersnipe SURVIVES. vs102 s15 + s20 both blocked a power<=1 attacker with Guttersnipe (2/2), killed it,
and Guttersnipe LIVED. -> guide edited this wave (favorable-block exception; see strategy.txt / notes I-2).

**Guttersnipe tapped-read guard, ROUND 4: NO dangerous misread recurrence.** The wave-25
"attackers-tapped => all-tapped" false generalization did NOT recur. See MANDATE 2.

---

## MANDATE (1) -- THE TWO CORRECT-DEVIATION EDITS' FIRST CORPUS

### Bottom windows (2): the recalibrated ladder is USED, and the guide no longer fights the play

- **vs102 s3 (kind=bottom, T0, mull-to-6):** hand = Island, Artificer's Epiphany, **Elixir of Immortality**,
  Mountain, Cyclonic Rift, Guttersnipe, Island. Model reply `PUT: 3 (Elixir of Immortality)` with reasoning
  "Prism Ring and Elixir of Immortality are pure lifegain cards that do not advance the clock and should be
  bottomed first." This is the rank-(1) piece of the ladder -- the part the wave-25 note flagged as UNTESTED
  ("neither hand this corpus contained Prism Ring or Elixir"). It is now exercised and **CORRECT**. The
  model kept Mountain (red), Guttersnipe (payoff), the dig + counter. Impeccable.
- **vs137 s6 (kind=bottom, T0, kept after 4 mulligans -> bottom EXACTLY 4):** hand = Counterspell, Downsize,
  Mountain, Downsize, Young Pyromancer, Aetherize, Mountain. The model explicitly walked the recalibrated
  ladder in prose: "(1) Lifegain - none; (2) Surplus lands - two Mountains, one surplus; (3) Over-costed
  spells - Aetherize slow; (4) Redundant duplicates - two Downsize." That is the wave-25 ranking, used as
  intended. The model did NOT fight the guide (last wave it had to IGNORE the "never bottom a draw spell"
  absolute; this wave the guide and play agree).
  - **Self-coherence wobble (non-decisive, NOT a defect):** the first line was `PUT: 1, 2, 4, 6` (bottoms
    Counterspell + 2 Downsize + Aetherize -> keeps 2 Mountains + YP), but the model then reasoned to a
    DIFFERENT conclusion, twice: "So the indices are 2, 3, 4, 6" and "Bottom cards 2, 3, 4, and 6" (bottoms
    1 Mountain + 2 Downsize + Aetherize -> keeps Counterspell + YP + 1 Mountain). The parser recorded the
    first line (chosen='Counterspell, Downsize, Downsize, Aetherize'). **This is NOT a parser misfire:** the
    ONLY line in `PUT:` coded format is line 1; the prose conclusion was never re-emitted as a PUT: line, so
    no parser (answer-first OR natural-stop-on-a-coded-line) could pick it. It is a MODEL self-coherence
    wobble -- it emitted its coded answer before finishing reasoning. Non-decisive: keeping 2 red sources +
    YP is a fully defensible mull-to-3 keep for a deck whose #1 loss mode is red screw (arguably the SAFER
    keep than 1-land + counter). The game was lost on the mull-to-3 itself (construction/variance), not this.
  - The **bottom ladder did not steer either play wrong**; the mull-to-3 disaster was FOUR straight no-red
    hands (see MANDATE 4 mulligan read), not a bottom error.

**Verdict on the bottom edit: VALIDATED (not contradicted; the untested rank-(1) piece now confirmed).**

### Lethal-only-blocker window (1): the exception fired, CORRECT

- **vs27 s31 (kind=blockers, T14, life 7):** prompt board (verbatim): 4 attackers -- `Inkfathom Infiltrator
  (2/1) [unblockable]`, `Zombie Outlander (2/2)`, `Vodalian Zombie (2/2)`, `Inkfathom Infiltrator (2/1)
  [unblockable]`; my only creature = Guttersnipe (2/2). The engine's own line: *"Unblocked, these attackers
  deal up to 8 - you would be at -1 - LETHAL if it all connects: block enough to survive."* Model reply
  `BLOCKS: B1:A2` (Guttersnipe blocks Zombie Outlander) with correct math: "2 Inkfathoms unblockable = 4
  regardless... block one 2/2, trade Guttersnipe, take 6, survive at 1... Guttersnipe is the only blocker I
  have; I must use it to prevent immediate death." **This is EXACTLY the wave-25 exception (incoming 8 >=
  life 7 AND Guttersnipe is the only creature). CORRECT.** The model correctly blocked a BLOCKABLE 2/2 (the
  two 2-power unblockable Inkfathoms cannot be blocked). Non-decisive to the game (already lost on
  construction -- no reach the whole match), but a clean first-corpus validation of the edit.
- Contrast the CORRECT HOLDS: vs27 s16 / s23 / s28 -- Guttersnipe faced a 2/2 attacker while NOT lethal and
  the model answered `BLOCKS: none` (took the hit rather than trade). This is the never-trade discipline
  working; the lethal exception did NOT over-fire.

**Verdict on the lethal-only-blocker edit: VALIDATED (fired correctly, did not over-fire).**

---

## MANDATE (1b) -- NEW FINDING: the never-block absolute is STILL too tight (FAVORABLE-block gap)

Same CORRECT-DEVIATION shape as wave-25 (correct model play, Magic-wrong guide absolute), a THIRD instance
of the never-block-absolute being miscalibrated:

- **vs102 s15 (kind=blockers, T8, life 19):** opponent attacks with Elvish Doomsayer (**1/1**); my only
  creature = Guttersnipe (2/2). Reply `BLOCKS: B1:A1` (Guttersnipe blocks Doomsayer): "favorable trade: the
  Doomsayer dies, and my Guttersnipe survives with 1 damage marked... Since the Doomsayer has shroud, I
  cannot target it with spells, so combat is the only way to remove it." **CORRECT** -- Guttersnipe (2
  toughness) takes 1, LIVES, kills the 1/1, and is untapped for next turn (blocking does not tap).
- **vs102 s20 (kind=blockers, T10, life 19):** Doomsayer now **0/1** (Animate Dead -1/-0); Guttersnipe
  blocks again: "Blocking with Guttersnipe (2/2) guarantees the destruction of the Doomsayer while your
  Guttersnipe survives (taking 0 damage)... overwhelmingly favorable." **CORRECT** -- free removal.

Both VIOLATE the wave-25 guide's "NEVER block with Guttersnipe... it stays alive in EVERY case." But
Guttersnipe DOES stay alive -- it SURVIVES the block (attacker power < its 2 toughness). The wave-25 edit
added only the LETHAL-and-only-blocker chump case; it did NOT cover a FAVORABLE block where Guttersnipe
lives and removes the attacker for free. These two blocks were PART of the vs102 WIN (they kept Guttersnipe
on board and life at 19 against a recurring attacker). Had a future model OBEYED the absolute literally, it
would take 1 face damage per turn from a recurring 1/1 rather than kill it once -- a strictly worse line.

**-> JUSTIFIED guide edit (see strategy.txt / notes I-2):** allow Guttersnipe to block an attacker with
power LESS than 2 (a power-0/1 creature) -- it survives, kills/shrinks the attacker, stays untapped to
attack. Preserve the never-TRADE core (power 2+ and not-lethal = do not block) and the lethal-only-blocker
exception. This is rules-correctness, NOT "shout louder" -- the covered behavior was WRONG (too absolute),
not under-emphasized, and the model played the correct deviation TWICE with explicit correct reasoning.

---

## MANDATE (2) -- GUTTERSNIPE TAPPED-READ GUARD, ROUND 4 + the "attackers-tapped => all-tapped" WATCH

Every Guttersnipe attack window this corpus (Guttersnipe on my battlefield, kind=attackers), diffing the
model's claimed board read / decision vs the prompt's actual opponent [tapped] tags:

| game | seq | T | opp board (relevant) | decision | verdict |
|---|---|---|---|---|---|
| vs102 | s14 | 7 | Elvish Doomsayer [tapped], only creature | ATTACK: none (held Gut) | CONSERVATIVE (safe to swing, held) -- see below |
| vs102 | s19 | 9 | 0 creatures | SWING Gut, Gut | CORRECT |
| vs102 | s25 | 11 | 0 creatures | SWING Gut, Gut | CORRECT |
| vs102 | s32 | 13 | 0 creatures | SWING Gut | CORRECT |
| vs102 | s38 | 15 | 0 creatures | SWING Gut | CORRECT (won 19/0) |
| vs22 | s7 | 4 | 0 creatures | held Gut (summoning-sick), swung YP | CORRECT |
| vs22 | s9/s11/s14 | 6/8/10 | 0 creatures | SWING Gut + others | CORRECT |
| vs22 | **s17** | 12 | Stinkdrinker Daredevil (1/3) UNTAPPED | SWING Gut + YP + Elem | **CORRECT (LETHAL)** |
| vs27 | s9 | 5 | Vodalian Zombie (2/2) UNTAPPED | held Gut, swung YP | CORRECT |
| vs27 | s14/s20/s27/s29 | 7/9/11/13 | Zombie Outlander (2/2) UNTAPPED | held Gut (ATTACK: none) | CORRECT |

**Round-4 result: the guard held; NO dangerous tapped-read misread.** Specifically:
- **vs27 (5 windows):** the model correctly held Guttersnipe home EVERY time an untapped Zombie (2/2) could
  block. This is exactly the discipline the guard teaches -- no "attackers-tapped => all-tapped" slip
  recurred (the wave-25 vs137 s26 sub-shape did NOT return).
- **vs22 s17 is NOT a misread:** the model swung Guttersnipe into an untapped Stinkdrinker Daredevil (1/3)
  -- but this was the LETHAL winning turn (opp at 3, my board YP 2/1 + Gut 2/2 + Elem 1/1 = 5 power vs one
  1/3 blocker). The reply enumerated all 4 block scenarios and confirmed lethal in each ("In all cases, the
  opponent reaches 0 life"). Game ended T12, opp 0. This is a CORRECT lethal alpha strike -- it revealed a
  minor tension between the Guttersnipe-hold rule and the LETHAL CHECK, now reconciled by a one-line guide
  clause (strategy.txt; the model already resolved it correctly on its own).
- **vs102 s14 is a CONSERVATIVE hold + a first-line/conclusion wobble (non-decisive):** Doomsayer was
  [tapped], so swinging Guttersnipe was safe; the model's first line `ATTACK: none` held, but its reasoning
  then concluded "the opponent has zero untapped creatures... Guttersnipe is safe to attack. Why did I
  initially think 'none'?" The parser took the first coded line (ATTACK answer-first, per spec) = the
  CONSERVATIVE answer. Missed 2 face damage; game won 19/0 anyway. Same self-coherence class as vs137 s6
  bottom (below), NOT a parser misfire (ATTACK is answer-first by design; the coded line was `none`).

**The tapped-read WATCH (I-1): NO RECURRENCE this corpus.** It stays a single-SEAT WATCH (deck131 only, 3
prior corpora) -- promote to a cross-seat combat-safety item only if a 2nd SEAT shows it. Guide UNTOUCHED on
the per-creature tapped-read guard prose (still 8/9+ historically; sanctioned-against to shout louder).

---

## MANDATE (3) -- c1-c5 BEHAVIOR SHIFTS AT THIS SEAT

- **(c1) full target enumeration:** UNEXERCISED / IMMATERIAL. deck131 has no chooseTarget seam records this
  corpus (0 target-choice, 0 may-ask -- verified by scan). Its "targeted" spells are counters (target a
  spell on the stack), Essence Scatter, Downsize/Cyclonic Rift (bounce/shrink) -- targets ride the cast
  menu, not a separate single-target preview, and the deck mostly just CHAINS spells for Guttersnipe damage
  rather than casting removal at enumerable creatures. No previously-hidden kill target was taken because
  the deck has no burn/removal that kills a creature via a target-preview. Honest mark: **UNEXERCISED** at
  this seat; the c1 win (opponent-first full enumeration) has no burn-seat surface in this counter/bounce
  build. No prompt-length side effect observed.
- **(c2) ETB pay-or-tap annotations:** UNEXERCISED. deck131 runs only basic Island/Mountain -- no
  shockland/pathway. No `[enters UNTAPPED/TAPPED]` annotation surfaced.
- **(c3) PT-pump render (delta + result):** UNEXERCISED. deck131 has no activated/own-creature PT-pump
  (Downsize is a -4/-0 debuff on an OPPONENT creature, not a pump; no +N/+N). No stat-conflation surface.
- **(c4) parser hardening:** CORPUS GATE PASSED -- zero fallbacks, zero dropped/mis-picked legitimate
  replies. Two model self-coherence contradictions DECOMPOSED verbatim (vs137 s6 bottom PUT: 1,2,4,6 vs
  prose 2,3,4,6; vs102 s14 ATTACK: none vs prose "safe to attack") -- BOTH are the parser correctly taking
  the only coded / answer-first line, NOT hardening misfires (the model's alternate conclusion was never
  re-emitted in coded format, so it is unparseable by construction). See notes I-4. No line-anchored or
  example-echo misfire found.
- **(c5) may-ask render:** UNEXERCISED (0 may-ask records at this seat).

---

## MANDATE (4) -- 1/6 -> 2/6 DECISIONS-READ + CONSTRUCTION-TERMINAL REASSESSMENT

Zero fallbacks at this seat (no empty_reply/unparsed/retracted_choice/stale_echo). Every game fully piloted.

**The 2 WINS:**
- **vs102 (19/0, T15):** the standout. Two Guttersnipe stuck; the model FAVORABLE-blocked the recurring
  Doomsayer twice (s15/s20), keeping Guttersnipe alive at 19 life, then ground opp 18->0 with spell damage
  + attacks into an empty board (s19/s25/s32/s38). Last wave this exact matchup was a 1-LIFE LOSS; this
  wave a dominant win. The flip is DRAW VARIANCE (Tergrid durdled) + clean piloting (the favorable blocks).
- **vs22 (20/0, T12):** free win vs a slow guideless Giants deck (mono-R ramp, Step-0). 131 curved
  YP T2 / Gut, opp had a near-empty board the whole game, and 131 raced 20->0 taking ZERO face damage. Not
  a signal about deck131's ceiling -- a signal about a weak un-guided opponent.

**The 4 LOSSES -- construction-terminal, restated (owner's ledger, not relitigated):**
- **vs137 (adj, 1/8):** FOUR straight no-red mulligans (s1-s4, all correctly shipped) -> mull-to-3; the
  3-card keep (2 Mountain + YP) never assembled an engine vs Selesnya go-wide adventures. Variance +
  construction (threat density too thin to survive a 4-mull).
- **vs18 (0/15, T19):** NO Guttersnipe drawn all game (GUT on battlefield: never). Single-YP token race
  out-paced by Kithkin's wide anthem board + first strike + a lifegain-ish wall. The recurring "one engine,
  no reach" loss.
- **vs133 (-7/13, T25):** 25-turn grind vs Phyrexian Asphodel drain; no reach to push past the drain wall.
- **vs27 (-6/14, T16):** held Guttersnipe correctly every window; no reach vs unblockable Inkfathom + go-
  wide; lost the race. Lethal-only-block fired correctly at the death (s31).

**CONSTRUCTION-TERMINAL flag: CONFIRMED, STANDS.** Threat density 6/60 (vs18 drew zero Guttersnipe;
vs137 mulled to 3), no reach vs lifegain/drain/go-wide (vs133/vs27/vs18), no early defense. The 2/6 is
draw variance over the SAME construction-capped deck, not a construction change. The lever remains DECK
CONSTRUCTION (owner's call; sideboard already carries Spellheart Chimera reach / Aetherling finisher).

**Construction ceiling reassessment (per the brief -- "is the ceiling higher than flagged?"):** NO. The
two wins do not raise the ceiling. One is a variance flip against a deck that beat 131 by one life last
wave (same durdle ceiling, opposite coin); the other is against an un-guided Step-0 deck that will climb as
it gets a guide. Against the GUIDED field (18/27/133/137) 131 went 0/4 this corpus. The ceiling is where it
has been for 9+ waves.

---

## MANDATE (5) -- REPPENALTY=1.05 round 5 + retry: SAME, no degradation

- Reply prose coherent; the longest tails (vs102 win, vs137 mull-to-3 deliberation) are on-task reasoning,
  no decode-collapse, no token-loop. The two self-coherence wobbles (vs137 s6, vs102 s14) are the normal
  N9 self-correction family (the model reasons past its own first answer), non-decisive both times.
- Retry fired 0x -- CORRECT (no garbage/decode-collapse replies to catch). Zero fallbacks confirms the
  parser stack converged at this seat again (cleanest deck131 corpus, 5th running).
- No timeout-draw at this seat; latency within corpus norms under -T 3000.
