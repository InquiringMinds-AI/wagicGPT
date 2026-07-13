# Deck-140 review — wave 4 findings

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Staff of Nin / Rakdos's Return / Blightsteel. Game plan: sweep, then deploy a finisher
and kill. **The deck has no fast clock — its whole win rate lives or dies on reach.**

Corpus: two selfplay runs (`matchups-20260712-192832`, `matchups-20260712-230636`), 6 games each.
Combined record **1 W / 5 L / 6 timeouts = 1/12**, worst in the pool; 6 of the corpus's 7 timeouts
are deck140 games.

Outcome map (file-epoch -> opponent -> result -> last turn / final opp life):
- R1: v131 TIMEOUT(t38) - v110 **L**(t27, opp 14) - v44 **L**(t32, opp 18) - v133 TIMEOUT(t8) - v135 TIMEOUT(t28) - v109 **W**(t50, opp 11)
- R2: v109 **L**(t19, opp 18) - v135 TIMEOUT(t31) - v110 **L**(t9, opp 20) - v131 TIMEOUT(t36) - v44 **L**(t22, opp 14) - v133 TIMEOUT(t14)

Severity legend: [GAME-LOSING] - [DEGRADING] - [STRUCTURAL] (infra/deck, not a guide/model bug).

---

## TOP FINDINGS (read these first)

### 1. [GAME-LOSING] The X-announcement menu is an index/value trap — the deck's burst is silently gutted
**This is the wave-4 headline and the single most consequential defect.** Wave-3's `{l}`->`{X}`
render fix WORKED (the pilot now casts Rakdos's Return and Black Sun's Zenith — it took them ~17
times across the corpus, up from 0/26 in wave 3). But that fix unmasked the next layer: **the pilot
cannot correctly set X.**

The X menu lists options ascending, `1. X = 0`, `2. X = 1`, `3. X = 2`, ... so `option_index = X + 1`.
The model reasons about the X **value** it wants in its PLAN prose, then emits a bare menu number —
and systematically types the *value* as if it were the *index*, or collapses to option 1.

Across all 17 X decisions in the corpus, the realized X was **below the model's own stated intent in
15 of 17**, and hit **X = 0 (a total blank) in 4**:

| Game | turn | chose | intended (its own PLAN) |
|---|---|---|---|
| R2v131 | t6 | **X=0** | X=2 |
| R2v44 | t13 | **X=0** (Black Sun's Zenith) | X=3 |
| R2v135 | t5 | **X=0** | X=1 |
| R2v133 | t6 | **X=0** | X=2 |
| R1v131 | t24 | X=2 | X=3 |
| R1v110 | t6/t12 | X=1 | X=2 |
| R2v135 t17 / R2v131 t14 |  | X=5 | X=6 |
|  |  | X = intended - 1 (typical) |  |

Two verbatim reply/PLAN mismatches proving it is menu-confusion, not bad Magic judgment:
- **R2v44 t13** (Black Sun's Zenith vs two 3/3 fliers): reply = `1` (-> X=0), PLAN = *"Black Sun's
  Zenith with **X=3** will place three -1/-1 counters on each, reducing their toughness to 0 and
  destroying them immediately."* It wanted the kill; it announced X=0 and killed nothing.
- **R2v131 t6**: reply = `1` (-> X=0), PLAN = *"Cast Black Sun's Zenith with **X=2** to place two
  -1/-1 counters on the opponent's Guttersnipe."*

Why it is game-losing for THIS deck specifically: Rakdos's Return is the deck's designated burn
finisher (guide win-condition #3) and Black Sun's Zenith is its indestructible/big-creature sweeper.
Undershooting X by 1-3, or zeroing it, turns the deck's only reach and its premium removal into
blanks. In the 5 losses the opponent ended at **14-20 life** — deck140 dealt them roughly 0-6 total
damage across entire games. A correctly-sized Rakdos's Return (X~5 with 8 lands, per the guide) is
often the difference between the perpetual stall and a kill.

**Fixes (both warranted; the platform fix is stronger):**
- **PLATFORM/UI:** the X-announce menu VIOLATES the project's own "usually-correct option first"
  ordering — it lists the *worst* value (X=0) as option 1 and the largest last. Reverse it: list the
  **largest affordable X as option 1**. That aligns the menu with both the model's strong
  first-option bias AND the guide's "pick the largest" instruction, and it makes the catastrophic
  collapse-to-1 land on max-X instead of X=0. (Alternatively/additionally, label each option so the
  X value is unmistakable and never let a bare "1" mean "the spell does nothing.")
- **GUIDE:** current text says *"pick the LARGEST number offered"* — true but it doesn't warn about
  the index/value gap. Add an explicit, positively-framed rule: *"The X menu lists X = 0 FIRST and
  the biggest X LAST. Always choose the LAST option in the list. Never choose option 1 — option 1 is
  X = 0 and your spell does nothing."* (If the platform reverses the menu, flip this to "choose
  option 1 / the first option.")

### 2. [GAME-LOSING/DEGRADING] Near-zero offense — the deck cannot close, and Staff is its only real clock
The deck stabilizes completely (it sweeps everything and gains huge life) but has almost no way to
end a game. Evidence:
- In all 5 losses the opponent finished at **14-20 life** (R2v109 opp went 20->18 over 19 turns while
  deck140 went 20->dead). The deck out-*survives* but never *damages*.
- The lone WIN (R1v109) required **turn 50 and 181 life** — it ground the opponent out via Blightsteel
  (cast t48) plus a long tail of Staff-of-Nin face pings. It CAN win, but only by out-durdling an
  opponent who also can't close.
- Staff of Nin (1 damage/turn) is the only *repeatable* clock, and Blightsteel ({12}, 2 copies) is
  drawn+deployed in only 2/12 games.

Sub-defect — **Staff pings wasted on un-killable creatures:** of 50 Staff activations, 40 hit face
(correct) but **10 hit creatures, 7 of those the 2/2 Guttersnipe** (verified `toughness=2` in
`mtg.txt`; Staff deals 1 — it can NEVER kill Guttersnipe). R2v131 alone throws away the clock on
Guttersnipe at t20/24/26/30/32. Each wasted ping is a turn of zero progress in a deck whose clock is
already 1/turn. The guide's rule *"kill a 1-toughness creature if they have one, otherwise hit face"*
is being misapplied to 2-toughness targets. Tighten to: *"only target a creature if 1 damage KILLS
it (its toughness shown as 1); Guttersnipe (2/2) and anything toughness 2+ survives — hit the
opponent's face instead."*

This is largely a **deck-weakness/reach** problem, but Finding #1 is the biggest *fixable* lever on
it: Rakdos's Return is meant to be the burst that ends these grinds and it is being neutered.

### 3. [STRUCTURAL] deck140's timeout over-representation is partly an artifact of control being decision-dense
6 of the corpus's 7 timeouts are deck140 games, but they split into two distinct causes:
- **Genuine grind-stalls** (R1v131 t38, R2v131 t36, R2v135 t31, R1v135 t28): the deck stabilizes and
  gains life but can't close (Findings #1/#2). These reach high turn counts with life totals frozen.
- **Latency-starved pseudo-timeouts** (R1v133 t8, R2v133 t14): the game was progressing normally
  (both players trading, life moving) but ran out of the 2400s wall-clock because per-decision
  inference was slow — R2v133 averaged **14.9 s/decision with a 52 s spike**; R1v133 a 27 s spike.
  These are the `-j` concurrency artifact (harness note: -j6 -> median 17 s, p90 at the HTTP timeout),
  NOT a deck140 defect. (This nuances the task framing that "timeout games progress to turns 28-38" —
  two of them are low-turn and purely infra-bound.)

The *reason* deck140 dominates the timeout column is structural: a control mirror generates
**3-4x the decisions** of an aggro game (every sweep, every Staff activation, every life-gain loop —
R2v131 = 98 records at t36, R2v135 = 91, vs aggro games ending t9-22 with 24-36 records). Decision
count x high per-decision latency = disproportionate wall-clock exposure. Two levers, both outside
the guide: (a) run decision-quality corpora at `-j 3` or raise `WAGIC_GPT_TIMEOUT`; (b) implement
wave-3's still-open **stalemate detector** (end as `draw/stall` after N turns of frozen life+board)
so genuine can't-close stalls are distinguished from slow-inference in results.tsv.

### 4. [DEGRADING] Loss mode = out-attritioned: reactive every turn, proactive never
The losses aren't misclicks — they're the deck spending every main phase reacting while its own life
erodes and the opponent's barely moves. R2v109 (L, t19): five Pyroclasms + Black Sun + Wrath cast,
Staff deployed only t14, opponent never dropped below 18; deck140 slid 20->15->11->2->dead. The pilot
follows the guide's "sweep the board" instinct faithfully but the deck has no proactive damage plan
early, and its one burst (Rakdos's Return) is either not cast in the reactive spiral or X-undershot
to near-0 (Finding #1). This is the same root as #2 seen from the losing side. Fast-aggro losses
(R2v110, dead t9) are partly variance/matchup — run over before stabilizing.

---

## Wave-3 findings status

- **W3 #1 [GAME-LOSING] `{X}` renders as `{l}`** — **FIXED.** Options now show `{r}{b}{x}` and the
  pilot casts Rakdos's Return / Black Sun's Zenith freely. *This fix directly unmasked wave-4
  Finding #1 (X-value selection).*
- **W3 #2 [GAME-LOSING] "Mana in your pool (none)" false can't-pay** — **LARGELY FIXED.** "Cast
  nothing" declines citing no-mana/tapped-lands fell from ~55% (47/86 in wave 3) to **~11% (12/110)**,
  and most of those 12 are actually correct sweeper-declines (regex-caught substrings), not real
  pool-panic. The heavy guide rules + representation change did their job.
- **W3 #3 [DEGRADING] stale-PLAN poisoning** — **MOSTLY FIXED / not observed as a loss driver.** The
  guide's "trust the CURRENT battlefield over your PLAN" clause plus higher act-rate mean the
  board-contradiction death-spiral from wave 3 didn't recur in these 12 games. (No age-stamp/expiry
  was needed to unblock this deck; leave as monitored.)
- **W3 #4 [DEGRADING] priority-seam "cast inside a priority window"** — **not seen as harmful here;**
  priority passes are clean. Treat as fixed-or-latent.
- **W3 #5 [DEGRADING] timeout economics / stalls indistinguishable from slow games** — **PERSISTING.**
  Still no stalemate detector; grind-stalls and latency-starved games both log as bare `timeout`
  (see wave-4 Finding #3). The append-only-narration prompt-growth cost also persists.
- **W3 #6 [ANNOYANCE] translog quality** — **FIXED.** `turn`, `phase`, `latency_ms`, `chosen_text`,
  and closing `gameend` records are all present now and made this review far cheaper.

---

## Priority for the guide reviser (next task)
1. Rewrite the Rakdos's Return / Black Sun's Zenith X guidance to defeat the index/value trap
   (Finding #1) — positively framed, explicit about which option number to pick. Flag the
   platform-side menu-reversal as the stronger fix if engine changes are on the table.
2. Tighten the Staff-of-Nin targeting rule to "1 damage must KILL it (toughness 1), else face"
   (Finding #2) — call out Guttersnipe by name as the trap.
3. Nothing else in the guide is badly broken. The residual 0-1/12 is dominated by deck reach
   (Finding #2/#4, a deck-tier issue) and infra timeout artifacts (Finding #3), neither of which the
   guide can fully solve. Set expectations: even a perfect guide leaves this deck reach-starved
   against a pool that keeps rebuilding boards.
