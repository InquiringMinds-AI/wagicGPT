# deck131 — wave 6 review (UR Guttersnipe spellslinger; deck name "Mind Control / Best Counter Deck Ever")

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260713-190011/`, one GPT-vs-GPT round-robin,
qwen35 both seats. **6 deck131 games.** Live guide in play = the wave-5 revision
(`bin/Res/ai/baka/deck131_strategy.txt`: two-gear identity, Guttersnipe cast-every-turn as #1,
by-name override of "reactions need a target", Mountain-first, attack/no-block, draw-on-curve).

**Record: 1 W / 4 L / 1 TIMEOUT.** (Wave-5 was 0 W / 5 L / 1 TO; wave-4 2 W / 8 L / 2 TO.)
FIRST WIN in two waves — beat deck135 cleanly (turn 11, opp -4).

Seat-file -> game map (seat epoch = results start_epoch + 3..4; confirmed by opponent card names):

| result | vs | file (epoch) | turns | end life me/opp | one-line cause |
|---|---|---|---|---|---|
| TIMEOUT | 133 | 1783987216 | 31 | 17 / 17 | dead-even grind stall; Guttersnipe removed, deck has no closer |
| LOSS | 140 | 1783989086 | 34 | 6 / 143 | unwinnable lifegain matchup; opp gained to 143 |
| LOSS | 44  | 1783994322 | 12 | -29 / 20 | curved out, Guttersnipe + YP removed on sight; opp untouched at 20 |
| LOSS | 109 | 1783994820 | 9  | 0 / 20 | red screw (first Mountain T8); mono-red raced it; opp untouched at 20 |
| WIN  | 135 | 1783995005 | 11 | 12 / -4 | textbook: YP T3, Guttersnipe T5, chain spells + attack every turn |
| LOSS | 110 | 1783996823 | 13 | -13 / 18 | ZERO Mountains all game (kept a 3-Island opener); YP uncastable |

**Opponent life at death: 17 (TO, dead even), 143, 20, 20, (WIN opp -4), 18.** In the four losses,
three opponents finished at ~20 — the wave-4/5 signature (opp near 20 = deck131 never had a plan
running) recurs almost unchanged. This is again an **OBEYED-BUT-LOSING** corpus: obedience metrics
are high, the win is the guide working as written, and the losing record traces to deck construction
(manabase + threat density) and variance against a hard slate — NOT to a guide regression.

---

## MANDATORY AUDIT 1 — the Guttersnipe rule #1 (obeyed / overshot / non-firing, with counts)

`Guttersnipe auto=@movedTo(instant,sorcery|mystack):damage:2 opponent` (verified in mtg.txt) — 2 to
the opponent whenever you cast an instant or sorcery. Wave-5 promoted "with Guttersnipe out, cast a
spell every turn — the cast IS the payoff" to rule #1 and added a by-name override of the core
"reactions need a target" prior, because wave-5 logged >=3 "cast-nothing while Guttersnipe on board"
instances (vs44 seq10/24, vs140 seq22).

**Count this wave — Guttersnipe reached the battlefield with a castable instant/sorcery window in
only 3 windows, ALL in the vs135 WIN:**
- vs135 seq11 (T7): Cast Cyclonic Rift — **FIRED** (2 to face, opp 18->16 via the trigger).
- vs135 seq16 (T9): a 2nd Guttersnipe creature was castable and taken instead — deploying a second
  payoff, correct.
- vs135 seq20 (T11): Cast Cyclonic Rift — **FIRED** (2 to face, part of the lethal turn).

**Cast-nothing-while-Guttersnipe-out instances this wave: ZERO.** The wave-5 lever is RESOLVED — the
by-name override held every window it was tested. **Overshoot: none** — the rule did not push the
pilot into casting junk when Guttersnipe was NOT out (see Audit 2).

**BUT the resolution is under-powered, and I am flagging it as such (do not over-read it).**
Guttersnipe only survived on the battlefield in 1 of 6 games. In the five non-win games the rule's
precondition (Guttersnipe on board) barely occurred, so "0 cast-nothing-while-out" is CONSISTENT with
the override working but is not strong proof of it — n=3 test windows, all in one game. Non-firing in
the losses is deck/variance, not suppression:
- vs44: Guttersnipe cast T5 (seq7); battlefield lines T7 onward show it GONE — removed/countered on
  sight (opp = fliers/tempo: Faerie Miscreant, Archmage of Echoes). Young Pyromancer likewise never
  appears on the board after its cast. The engine physically never had a cast window with it out.
- vs133: Guttersnipe cast T10 (seq15); `gut=0` on the battlefield for all 30+ later windows — removed
  by the mono-black opponent, never online again.
- vs140 / vs109 / vs110: Guttersnipe never drawn/cast (red-screwed or unwinnable).

**Verdict: rule #1 is OBEYED where the precondition existed and won the one game where the engine came
online. FREEZE it verbatim (turnaround/obeyed-but-losing discipline). The recheck metric for wave 7 is
unchanged — `(spells cast while Guttersnipe on board)/(castable-spell windows while Guttersnipe on
board)` — this wave 2 fired / 3 windows / 0 declined, all in vs135.**

## MANDATORY AUDIT 2 — threat-before-value core line (the Prism Ring / Elixir durdle)

Wave-5 flagged Prism Ring/Elixir lifegain as a durdle sink and the core prompt shipped a
threat-before-value completion (act-floor satisfied by the highest-impact play; deploy a threat before
a same-turn value/ramp/dig spell). **Question: did that line move the durdle, and is the durdle
driving unwinnable-length games?**

Prism Ring / Elixir casts this corpus (8 total): vs140 4 Prism + 2 Elixir; vs133 1 Prism; vs44 1
Prism; vs109 1 Prism + 1 Elixir; vs110 1 Prism. **In EVERY one of these casts, no threat (Young
Pyromancer or Guttersnipe) was in the offered options that window** (checked `options_text` at each
cast: `threats_avail=[]` in all six vs140 casts; vs133 seq5 offered only `Cast Prism Ring / Cast
nothing`). The pilot **never chose Prism/Elixir OVER a castable threat** — every Prism/Elixir cast was
a "nothing better available" window, which the guide explicitly permits. **Threat-before-value did NOT
misfire; it is holding.**

**Is the lifegain durdle driving the long games? NO.** The 31-turn vs133 timeout — the corpus's one
grind-out — cast Prism **once** and Elixir **zero** times. Its length is not a lifegain sink; it is a
deck-construction stall (below). The heavy Prism/Elixir game (vs140, 6 casts) is the unwinnable
lifegain matchup where opp gained to 143 regardless of piloting — the 2-damage clock cannot beat +10
life/turn at any pilot quality. **The Prism-Ring/Elixir durdle remains a minor mana-sink, NOT a
piloting lever and NOT the length driver. It stays a deck-construction flag (cut the 4 Prism Rings for
threats/red sources), not a guide fix.**

## MANDATORY AUDIT 3 — convergence watch: reflexive high-life blocking

Only vs135 surfaced `blockers` decisions (T4, T6, T10) — all **"no blockers"** at 18 / 16 / 14 life,
correct racing per the guide and the core survival-first-at-8 rule. **No other game surfaced a
blockers decision** (in the losses, creatures were removed before an attack/block step arrived). **Zero
reflexive high-life blocks observed** — the convergence watch is CLEAN this wave (matches wave-5). No
evidence to promote the reflexive-blocking concern to a core line; it stays a per-deck watch item.

## MANDATORY AUDIT 4 — manabase evidence refresh

**Red screw directly caused or shaped 3 of 6 games this wave** (14 Island / 8 Mountain over all-red
payoffs Guttersnipe {2}{R} and Young Pyromancer {1}{R}):
- **vs110:** ZERO Mountains across all 13 turns. Kept a 3-Island opener (Counterspell, Essence
  Scatter, Young Pyromancer, 3 Islands, Prism Ring). Young Pyromancer uncastable the entire game; dead
  at -13, opp untouched at 18. (Also a MULLIGAN violation — see below.)
- **vs109:** first Mountain on T8. Mono-red aggro (Ash Zealot, Goblins) raced it dead by T9. (This
  hand was correctly mulliganed to 6, and STILL had no red until T8 — the manabase, not the mulligan,
  is the root cause.)
- **vs133:** first Mountain on T10 → contributed to the 31-turn stall (Young Pyromancer stranded
  early; see the impossible kept-hand plan below).

vs140 and vs44 had a Mountain T1 (fine) and lost to matchup/removal. **The manabase flag is REFRESHED
and CONFIRMED for the third straight wave, now with 3 more losing data points.** Cheapest fix: move
3-4 Islands to Mountains (11M/11I or 12M/10I). The blue spells are forgiving — only Counterspell
{U}{U}, Dissipate {1}{U}{U}, Opportunity {4}{U}{U} are blue-hungry, and all three are support; the
workhorses are single-{U} (Downsize, Cyclonic Rift, Essence Scatter, Artificer's Epiphany, Aetherize).

---

## THE ONE MODEL-FIXABLE LEVER THIS WAVE: mulligan discipline (no-red keeps)

Wave-5's cast-seam lever (Guttersnipe suppression) is resolved. The new residual lever is at the
**MULLIGAN seam**: **the guide's own mulligan rule ("a hand with no Mountain... mulligan it") is being
violated, and the general-prompt keep-default is winning over it.**

Three no-Mountain openers this corpus:
- **vs109 — CORRECTLY mulliganed.** Reply verbatim: *"Mulligan this hand because it lacks a Mountain,
  which is required to cast Young Pyromancer or Guttersnipe."* Proves the pilot CAN apply the rule —
  this is inconsistency, not a capability gap.
- **vs110 — KEPT (violation).** Hand: Counterspell, Essence Scatter, Young Pyromancer, 3 Islands, Prism
  Ring. No Mountain, no draw spell. Reply: *"Keep this hand. Play an Island, then cast Young Pyromancer
  if I draw a Mountain next turn."* — a keep that BETS on drawing red. It never drew one (0 Mountains,
  13 turns). Dead.
- **vs133 — KEPT (violation).** Hand: 3 Islands, Counterspell, Prism Ring, Elixir, Young Pyromancer. No
  Mountain, no draw spell. Reply: *"Keep this hand. Play an Island, then cast Young Pyromancer
  immediately"* — a plan that is **mechanically impossible** (YP needs {1}{R}; there is no red). Red
  did not arrive until T10; the game stalled to a turn-31 timeout.

Both kept hands have 3 lands + a castable non-red spell, so the **general-prompt mulligan default**
("keep any 7 with 2-5 lands and a castable spell") says KEEP, while the **deck guide** says MULLIGAN
(no red source). Guide precedence should win, but the pilot followed the general default 2 of 3 times.
The current guide mulligan line is buried at the very bottom, is hedged, and never overrides the
general default BY NAME. **Fix: promote + sharpen the mulligan line, override the general default by
name, phrase the trigger as a countable ("count the Mountains in your hand; zero = Mulligan"), and
intercept the exact false thought ("I will cast my creature once I draw a Mountain").**

**Honest ceiling on this lever (do not over-sell it):** vs109 mulliganed correctly and STILL lost
red-screwed (a no-red 7 shuffled into a 6 off a 14I/8M base often still misses red). So the mulligan
fix is a real but MARGINAL-ceiling improvement — a no-red keep is strictly worse than the mull, but
the deck's low red density is the dominating cause. This is why the mulligan sharpen is a promoted
seam block, NOT the #1 top slot: spending the loudest slot on a low-ceiling lever would churn the
frozen, obeyed Guttersnipe rule for little gain. The manabase flag (Audit 4) is the actual #1 fix.

---

## Harness-defect sweep (required)
- **DESYNCS: 0** across all 6 games (head-first parser clean; matches wave-5). Transport is clean —
  reply-format experiments can assume it.
- **ENGINE NO-OPS: none observed.** No repeated-pick-never-happened loops.
- **NUMERIC-OFFSET / menu traps: n/a** — this deck has no X-spells; no numeric menus surfaced.
- **PLAN/CHOICE MISMATCH: none** — the mulligan keeps above are genuine choices matching their PLAN
  text (the pilot planned to keep and kept); the error is the keep DECISION, not an intent collapse.
- **Representation note (persists):** vs110-class mana-ability mislabel ("Cast Card Normally with
  Mountain") — carried forward in notes.md.

## What the pilot did well (FREEZE these — do not "fix")
- **100% attacker participation** (vs135: declared every legal attacker every combat, verified against
  the `A#.` offered list).
- **Correct racing blocks** (vs135: "no blockers" at 18/16/14, above the 8-life floor).
- **Guttersnipe engine fired** every window it was live (2/3 casts as fuel in the win; 0 declines).
- **Mountain-first** obeyed whenever a Mountain was in hand (vs135, vs140, vs44).
- **Draw-on-curve, cast-payoff-first, reactions-held-with-no-target** all held.
- **The vs135 win is the guide's own proven line** — transcribed into strategy.txt as the winning
  script.

## What the losing record traces to (one line each)
- vs110: **kept a no-red hand** + zero Mountains drawn → MULLIGAN (model, marginal) + DECK (manabase).
- vs109: **red screw** (Mountain T8) vs mono-red → DECK (manabase); mulligan was correct.
- vs133: **kept a no-red hand** → red T10 → grind stall + win engine removed → MULLIGAN (model) + DECK
  (threat density / no closer).
- vs140: **unwinnable lifegain matchup** (opp to 143) → DECK/matchup.
- vs44: **threats removed on sight**, opp at 20 → DECK/matchup (removal-heavy fliers).
- vs135: **WIN** — guide executed as written.
