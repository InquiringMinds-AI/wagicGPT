# Deck-140 review — wave 5 findings

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus. Plan: sweep, deploy a finisher,
grind the opponent's face down. **The deck out-survives everything and under-damages everything —
its whole win rate lives on reach.**

Corpus: one selfplay run `matchups-20260713-145947`, 6 games (single round-robin, `-j`-concurrent).
Record **2 W / 2 L / 2 T = 2/6** — UP from wave-4's 1/12. The turnaround is real and traceable to
one engine fix (the X-menu reversal) landing exactly where wave-4 predicted.

Outcome map (opponent -> result -> last turn / final my/opp life):
- vs131 **W** (t34, my56/opp13) — Blightsteel grind-out
- vs44  **W** (t35, my26/opp-2) — **the designed kill**: sweep + Staff to opp 12, then Rakdos's Return **X=12** to face
- vs135 **L** (t14, my0/opp15) — fast snow-ramp aggro; never drew a real sweeper for a 9/9
- vs109 **L** (t29, my0/opp18) — out-attritioned; swept forever, never damaged
- vs133 **T** (t20, my2/opp7) — latency-starved (infra); game was live and close
- vs110 **T** (t32, my15/opp7) — winning-but-slow; reach-starved, Blightsteel arrived t32

Severity legend: [FIXED] — [GUIDE] — [STRUCTURAL] (deck/infra, not a guide/model bug).

---

## Harness / executability sweep (run FIRST — all clean)
- **Fallbacks: 0** across all 6 games (every decision was the model's own; head-first parser is 0%).
- **Desyncs: 0 / 399** parseable-head records (reply's first integer == recorded `choice` every time).
- **Menu numeric-offset (the wave-4 killer): essentially resolved** — see Finding 1.
- **No game wedged at negative life** — vs44 ended cleanly the instant opp hit -2 (`won:true`,
  gameend record present). The deck140-class negative-life SBA fix holds.

The corpus is clean: every loss below is bad hand / reach (deck) or infra, and the two guide levers
are (1) a now-INVERTED stale rule and (2) one wrong default. There is no piloting collapse this wave.

---

## TOP FINDINGS

### 1. [FIXED by engine — but the GUIDE'S #1 RULE IS NOW INVERTED AND DANGEROUS]
Wave-4's headline defect (X announced as index-not-value, collapsing to X=0) is **gone**. The engine
now presents the X menu **largest-first** and appends "option 1 is the LARGEST X" to the ask text.
Across the 7 X announcements this corpus the realized X was **at the maximum in 6/7** (values
1, 2, 3, 3, 3, 5, 12 — **zero X=0 blanks**, down from 4/17 blanks in wave 4). Proof it delivers wins:
**vs44 t35** cast Rakdos's Return, menu `1. X = 12 ... 13. X = 0`, chose option 1 -> **X=12 to the
opponent's face, opp 12 -> -2, game won.** That is the deck's designed kill firing correctly for the
first time in the project.

The one non-max pick — **vs110 t26 seq75**, Black Sun's Zenith, menu `1. X = 6 ... 7. X = 0`, chose
option 2 -> X=5 — was **menu-adjacency index carryover, not the old value/index trap**: the model had
just been fixated on "option 2 = Wrath of God" on the immediately-preceding cast menu (seq74) and
replied the bare "2" again onto the X menu. Immaterial here (the target was a 13/1 Skirge, toughness
1 — X=1 already kills it). Minor; see notes.md for the engine belt-and-suspenders ask.

**THE PROBLEM: the live guide's THE #1 RULE now points at the WRONG option.** It says (lines 7, 14):
*"THE X MENU: ALWAYS PICK THE LAST OPTION ... The biggest X is always the LAST line ... reply with
the number of the LAST line."* Under the reversed menu the **last line is now X = 0**. Had the pilot
obeyed the guide literally it would have blanked every X spell. It did **not** — it picked option 1
(the max) in 6/7, following the engine's ask text and the general prompt's numeric plan-binding,
**ignoring the guide's stale instruction.** We won *in spite of* the #1 rule, not because of it. This
is the most urgent edit: **flip the rule to "option 1 is the biggest X — pick it," and demote it from
a loud 12-line block to a one-line confirm** (the defect is now engine-fixed; per the skill's stopgap
lifecycle a fixed-defect stopgap is deleted/compressed).

### 2. [GUIDE — wrong default] Rakdos's Return is the deck's ONE reach; spending it small early forfeits the kill
The guide's Rakdos's Return advice (lines 22-25) says: *"Even when not lethal, cast it once you have
6+ lands: 4+ damage plus emptying their hand is your best play."* That default cost us a game:
- **vs110 (TIMEOUT):** cast Rakdos's Return **X=3 at t12** (opp 20 -> 17). That spent the only reach
  the deck drew, for 3 face damage. The rest of the game the deck had only Staff-of-Nin pings (1/turn)
  to grind 17 life, deployed Blightsteel too late (t32), and **timed out AHEAD at opp 7.** Had that
  same card been held, by t26 (opp already 9 from Staff) an X=8+ Rakdos's Return is lethal.
- **vs44 (WIN):** did the opposite — **held** Rakdos's Return, let Staff + sweeps grind opp to 12,
  then fired **X=12 for the kill** at t35. That is the win.

The deck has **only 2 copies** of its only reach; chipping one away early for 3 damage trades the
kill-shot for marginal value. The lethal check already in the guide (X = untapped lands - 2; if
X >= opp life, cast NOW) is correct and WORKED in vs44. The fix is to remove the "cast it even when
not lethal" license and make Rakdos's Return a HOLD-FOR-KILL finisher, with a countable release so
it doesn't hoard (fire for value+discard only when the opponent has a full hand AND is pressuring
you). Evidence-backed by the direct vs44/vs110 contrast; it does NOT risk passivity — opp life falls
every turn from Staff, so the lethal trigger arrives on its own.

### 3. [FIXED] Staff-of-Nin targeting is solved
Wave-4's Guttersnipe-ping waste is gone. Of 36 Staff activations across the corpus, **35 hit the
opponent's face**; the **1** creature ping (vs110 t28, an Ornithopter 0/2) was immaterial. The
wave-4 rule ("target a creature only if 1 damage KILLS it — toughness 1 — else face; Guttersnipe is
2/2, survives") is obeyed. **Demote to one compressed line**, keeping the load-bearing tokens
(face-by-default; creature only if toughness shown as 1).

### 4. [STRUCTURAL — deck construction] Reach starvation is the loss driver, not piloting
Both losses and the winnable timeout share one precondition the guide cannot create: **the deck had
no castable way to deal damage.**
- **vs109 (L):** opp sat at **20 until turn 14** and finished at **18**. The pilot cast Pyroclasm ~8
  times (correct — deck109 floods 1-toughness tokens) and its two X spells (Rakdos's Return X=2 at
  t14 with only 4 lands; Black Sun's Zenith X=3 as a sweep at t22), but the deck had no clock while
  its own life bled 20 -> 0. Faithful execution, no reach.
- **vs110 (T):** correct declines throughout (every "Cast nothing" was offered ONLY sweepers with the
  opponent's board empty — verified). Only proactive plays were Staff (1/turn) and a {12} Blightsteel
  arriving t32. Winning-but-slow.
- **vs135 (L):** at t13 the pilot's PLAN correctly reads "lethal board (9/9 Abominable Treefolk with
  trample)" but the **only sweeper offered was Pyroclasm (2 damage)** — Wrath/Damnation/Black Sun's
  Zenith were not in hand. It cast the only thing it had and died. Pure variance vs a fast snow-ramp
  deck.

Staff of Nin was **cast the very first turn it was OFFERED in every game** (t14/t20/t24/t25) — the
lateness is draw variance, not piloting delay. Blightsteel was cast the first turn offered in both
games it appeared. **The pilot deploys correctly; the deck is just slow. This is a
deck-construction reach flag (notes.md), not a guide bug — do not answer it with louder guide
prose.** The X-fix already converted the one game where reach WAS drawn (vs44) into a kill.

### 5. [STRUCTURAL — infra] Timeout triage
- **vs110 (t32, my15/opp7):** winning-but-slow — the pilot was AHEAD. Reach-starved close, not
  passivity. Finding 2's hold-for-kill change plausibly converts this to a win.
- **vs133 (t20, my2/opp7):** **latency-starved.** Top per-decision latencies 72s / 67s / 63s / 62s /
  58s (baseline ~4-5s); 87 records at only turn 20 (an ultra decision-dense control mirror). The game
  was live and close (both bleeding). This is the `-j` concurrency artifact, not a deck140 defect.
  Report as infra; write nothing.

Both timeouts are the archetype's structural exposure: control mirrors generate 87-98 decisions vs
24-46 for aggro games, so at a fixed 2400s cap they eat the timeout column regardless of play. See
notes.md (stalemate detector + cap-by-decisions ask, still open from wave 3/4).

---

## OVERSHOOT AUDIT (mandatory)
Wave-4 added loud rules. Checking each against this corpus:

1. **THE #1 RULE — "ALWAYS PICK THE LAST OPTION" (guide lines 7, 14).** The wave's overshoot headline,
   in the stale/inverted form: the engine reversed the menu after the rule was written, so "the LAST
   line" now names **X = 0**. Evidence: every X menu this corpus is largest-first
   (`1. X = 3 ... 4. X = 0`); the pilot picked option 1 (max) in 6/7 and **would have blanked every X
   spell if it had obeyed the guide.** A loud rule that now points at the worst option is a live
   hazard — flip it (Finding 1).
2. **"cast Rakdos's Return once you have 6+ lands ... even when not lethal" (lines 22-25).** Overshot
   the deploy-early instinct onto the deck's scarce finisher; caused the vs110 X=3 chip that forfeited
   a winnable game (Finding 2). Fix by removing the not-lethal license.
3. **Staff targeting rule (lines 44-48).** No overshoot — obeyed cleanly, 35/36 face. Demote, don't
   rewrite.
4. **"Cast nothing allowed ONLY when every offered spell is a sweeper AND opp has no creatures"
   (lines 66-68).** No overshoot into paralysis: the high cast-nothing rates (vs131 25/36, vs110
   14/27) were **verified correct declines** — every instance was sweepers-only against an empty
   board, and the deck deployed its finishers the moment they were castable. Working; keep compressed.
5. **No attack-floor suicide** (the deck has ~no creatures; Blightsteel correctly attacked vs131,
   `A1`, infect 11 -> win). **No "one X is enough" undershoot** — X is now maxed.

## Plan-then-unexecute (wave-4 CHANNEL-shaped failure): RESOLVED this corpus
The wave-4 signature (sound PLAN prose, wrong reply-encoding at the menu index step) does **not**
persist as a game-losing pattern. X picked at max 6/7; 0 desyncs / 399; 0 fallbacks. The lone
residue is the one X=5 menu-adjacency carryover (Finding 1), immaterial. The channel is now carried
by the engine's largest-first ordering + ask text. Watch it next wave only as a regression check.

---

## Wave-4 findings status
- **W4 #1 X index/value trap** — **FIXED** (engine largest-first reversal shipped; 6/7 max). Guide
  stopgap now compressible; the stale "pick the LAST line" wording must FLIP.
- **W4 #2 near-zero offense / reach** — **PERSISTS as STRUCTURAL** (Findings 4/5). One guide sliver
  added: conserve Rakdos's Return for the kill (Finding 2). Otherwise deck-construction (notes.md).
- **W4 #2 sub — Staff pings wasted on 2-toughness creatures** — **FIXED** (35/36 face; Finding 3).
- **W4 #3 timeout over-representation** — **PERSISTS** (still no stalemate detector; vs133
  latency-starved, vs110 winning-slow — Finding 5).
- **W4 #4 out-attritioned losses** — **PERSISTS as STRUCTURAL reach** (Finding 4), not a guide-fixable
  posture problem: the pilot acts every affordable turn.
- **Pool-panic / false can't-pay** — **stays FIXED** (13 "cast-nothing with a mana mention" were all
  correct declines or true statements like "Blightsteel needs 12, I have 8"). Do not re-litigate.

## Priority for the guide reviser (next task)
1. **FLIP + COMPRESS the X rule** (Finding 1): "option 1 is the biggest X — pick option 1; verify the
   number is the biggest shown." One line, positive. Delete the 12-line "pick the LAST option" block.
   Flag the X-teaching as a now-fixed stopgap in the handoff.
2. **Make Rakdos's Return a hold-for-kill finisher** (Finding 2): keep the per-turn lethal check;
   remove the "cast even when not lethal at 6+ lands" license; add a countable release.
3. **Demote the Staff-targeting and cast-nothing rules** to their compressed load-bearing cores.
4. Set expectations: the residual is deck reach (notes.md). Judge the deck on whether the X-fix +
   hold-for-kill turn winnable timeouts into wins, not on absolute win rate.
