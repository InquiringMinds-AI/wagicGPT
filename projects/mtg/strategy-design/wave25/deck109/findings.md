# Wave-25 review — deck109 (Hellrider, mono-red aggro / RDW) — Pillar 2-corpora clock CLOSES

**Corpus:** PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-035605`. Binary
`/tmp/wagic-62d8783a5` (rules-fix batch f0500ad66 + step-1 batch 62d8783a5), -T 3000,
REPPENALTY=1.05. deck109 seat = 6 games. Pool: 27/131/59/133/102/137 + deck109. Guide FROZEN
from wave-23/24 (md5 `a8f163f7f2bf1ecce5c9f4f13997ccf1`, deployed == wave24/deck109/strategy.txt,
cmp-verified). **This corpus is corpus 2 of 2 on the wave-23 Pillar-restoration validation clock.**

**Record: 5/6** — beat 131, 27, 137, 133, 102; lost to 59. Context only; decomposed by decision.
**Fallbacks: 0 at seat** (matches brief's 0 true model-fallbacks corpus-wide). Answer-first held
**133/133** ask/priority + **25/25** attacker windows.

| opp | result | end (my/opp, turn) | class |
|---|---|---|---|
| 131 | WIN  | 10 / 0, T8   | clean race; opp resolved Young Pyromancer + Guttersnipe; won 20->0 T8. |
| 27  | WIN  | 14 / -7, T9  | undead/merfolk; UNLEASH bodies + burn race. |
| 137 | WIN  | 15 / 0, T10  | GW Adventures; wide goblin swarm + Hellrider pings. |
| 59  | LOSS | -5 / 13, T13 | WB persist/wither grind; raced opp 20->13 by T6 then walled + out of burn (decomposed below). |
| 133 | WIN  | 18 / 0, T8   | fastest kill of the set; T0 Legion Loyalist -> haste beats. |
| 102 | WIN  | 16 / 0, T11  | mono-B devotion; out-raced, Ash Zealot closed at T11. |

**W/L flips vs wave-24:** vs133 flipped L->W (was the drain ceiling loss); vs59 flipped W->L (the
persist/wither grind — same RDW-vs-grind race class, opposite variance draw). Neither is a
regression; both are the documented "close race decided by burn density" ceiling (see loss decomp).

---

## (1) PILLAR 2-CORPORA CLOCK — CORPUS 2 CLOSES CLEAN. 0 mis-costings with trigger present.

10 windows offered Pillar of Flame as a cast option. **4 are the exact `{R}`-at-1-red-mana
trigger shape** (Pillar castable with exactly one red mana available — the sharpest test of the
wave-23 `{R}`-vs-`{1}{R}` conflation, because a cost-belief error there forces a decline):

| trigger window | mana | choice | verdict |
|---|---|---|---|
| vs131 s3 (T0) | `{r}` | Cast Stromkirk Noble (over Pillar) | **correct** — reasoned Pillar affordable, chose the body per DEPLOY-AND-ATTACK. |
| vs137 s3 (T0) | `{r}` | Cast Pillar of Flame | **correct** — quoted the guide's cost line verbatim: *"Pillar of Flame costs {R} = ONE red mana... you CAN afford it."* Only Pillar castable. |
| vs133 s3 (T0) | `{r}` | Cast Legion Loyalist (over Pillar) | **correct** — *"Legion Loyalist costs {r}, so I can cast it"*; deployed the haste body. |
| vs102 s3 (T1) | `{r}` | Cast Pillar of Flame | **correct** — *"Cost of Pillar is {R}. I can cast it"* (long deliberation, right cost throughout). |

**Every trigger window costed Pillar correctly as `{R}` = 1 red mana. Zero conflations. Zero
affordable-Pillar declines attributable to mis-costing.** The 2 deploy-body-over-Pillar windows
(vs131 s3, vs133 s3) explicitly recognized Pillar as affordable and chose the body under the
deploy rule — not a cost-driven decline.

Also-correct positive casts under the higher-mana Pillar windows: vs131 s18 (Pillar at `{r}{r}{r}`
as reach in a lethal turn, opp 5->3, correct). No decline anywhere was cost-belief-driven.

**Corpus-wide conflation grep = 0 real conflations.** 6 regex hits (`pillar` on a line also
containing `{1}{r}`/`2 mana`) all verified benign: they name the CORRECT cost of a *different*
card (Krenko's Command `{1}{R}`, Searing Spear `{1}{R}`) or quote the guide's own Pillar=`{R}`
line. No line ever costs Pillar itself as two mana.

**Verdict: corpus 2 of 2 passes cleanly WITH the trigger present (4 windows, incl. the same vs137
matchup that produced the wave-23 regression). The restored cost line is now VALIDATED across both
required corpora — the Pillar crutch is settled.** See ROTATION VERDICT (notes.md).

---

## (2) pw-REDIRECT REMOVAL (a4) — CLEAN. 0 redirect records at seat AND corpus-wide.

`/usr/bin/grep -il redirect *.jsonl` = **0 files** across all 42 corpus logs. At the deck109 seat,
all 11 burn-target-selection windows offered **"The opponent (player, life N)" directly** and the
model hit face in every one — no redirect menu, no planeswalker-damage-redirect prompt. **No
behavioral shift in burn lines vs prior corpora**: face-by-default operated identically. (Expected:
deck109 never faces a planeswalker, so the redirect menu never surfaced for it even pre-a4; a4 is
confirmed harmless-and-absent at this seat, and the corpus-wide grep confirms it corpus-wide.)

---

## (3) BURN DISCIPLINE ROUND 5 — CLEAN (0 self-face; face-by-default 11/11).

11 true burn TARGET-SELECTION windows (options include "Yourself"/"The opponent (player, life N)").
**0 self-face (no "Yourself" ever chosen). Face 11/11.** Two judgment calls both correct:

- **vs27 s14/s18 — faced over Oona's Gatewarden (2/1 flying DEFENDER, wither).** A defender only
  blocks; it is NOT a token-maker/lifegain engine. Facing to advance the race (opp 12->9->kill) is
  guide-correct; a 2/1 defender lets at most ~2 through if killed vs 3 face.
- **vs102 s20 — faced over Witch of the Moors (4/4 deathtouch), opp at 5.** Searing Spear (3) can't
  even kill a 4-toughness creature, and opp was at 5 with a lethal race live. Facing is
  unambiguously correct (game ended T11 opp 0).

No exception-class engine (token-maker / recurring drain) was ever a legal burn target that got
faced-over. The one on-sight-kill target that WAS mandated (Young Pyromancer, vs131) the model
*tried* to kill but was blocked by a harness issue — see (6), which is NOT a discipline failure.

---

## (4) ATTACKERS DECLARED-VS-RECORDED ROUND 4 — CLEAN (25/25).

25 attacker windows. **First-parseable coded `ATTACK:` line honored in ALL 25; recorded set ==
the first line's in-eligible tokens in ALL 25. ZERO eligible attacker dropped. ATTACK-first 25/25.**
7 over-declaration windows (first line named more tokens than eligible) — every extra token was
OUT of the eligible set and correctly dropped (summoning-sick or nonexistent): vs27 s19 (+2
Goblins sick), vs137 s10 (+A2/A3 nonexistent), vs137 s18 (+Stromkirk sick), vs59 s18 (+Stromkirk
sick), vs133 s16 (+Boros sick), vs102 s10 (+Boros sick), vs102 s17 (+A2 nonexistent). No prose
"Attack: Deal N..." hijack line appeared in any window. Aggression intact: every eligible attacker
declared, every window (all-in racing per guide). Attack-hijack class STAYS DEAD.

---

## (5) RULES-SHIFT WATCH (a1/a2 turn-1 upkeep) — CLEAN. No turn-1 upkeep action attempts.

The new turn-1 structure (a1: turn 1 begins at untap with a real upkeep; a2: untap/cleanup grant
no priority) produced **no confusion at this aggro seat**:
- The 6 "Untap"-phase records are all the **pregame mulligan ask** (cosmetic phase label; answered
  "Keep this hand" correctly). No spurious untap/cleanup priority ask appeared (a2 confirmed).
- Only **2 real mid-game Upkeep priority windows** (vs59 s26, vs102 s11 — Boros Reckoner
  first-strike activation offered at instant speed during upkeep). Both correctly **passed**.
- Every game opened at Main phase 1 and deployed a body; the model never tried to *act* at a
  turn-1 upkeep. **a1 does not tempt the aggro curve into a nonsensical upkeep play** — it
  proceeds sensibly to main phase.

---

## (6) NEW FINDINGS — vs131, OUTCOME-NEUTRAL (won 8/0 T8), route to HARNESS/core NOT deck guide.

Two genuine decision-quality taxes surfaced in vs131, both **preventing the model from executing
the guide's exact "kill Young Pyromancer / Guttersnipe ON SIGHT" exception** — and in both cases
the model *wanted* to comply and was blocked by mechanism, not judgment. The guide already mandates
the correct behavior, so neither is deck-guide-fixable. Both are cross-cutting (would recur at any
seat facing engine creatures behind a truncated target list). Full repros in notes.md.

**(6a) Truncated burn-target PREVIEW hides on-sight-kill engines behind "(+N more)".** The cast
option line's inline `legal targets right now: Stromkirk Noble, Goblin, Goblin, you (+2 more)`
lists the player's OWN creatures first (never valid burn targets for RDW) and truncates the
opponent's Young Pyromancer into "(+2 more)". At **vs131 s11** the model reasoned verbatim: *"the
option list explicitly excludes Pyromancer from the target list... I cannot target Pyromancer with
the current option... I will pass"* — DECLINING the mandated on-sight kill because it read the
truncated preview as the complete legal set. Also vs131 s8 (didn't consider Pyromancer a target).
Young Pyromancer + Guttersnipe then pinged the model for 2 each (events confirm 4 damage the model
"wanted" to prevent). Won anyway 8/0. HARNESS ledger item: prioritize opponent permanents in the
truncated preview, or don't truncate the target preview, or mark that "(+N more)" holds additional
legal targets.

**(6b) Prose-only answer reversal -> recorded the wrong action.** At **vs131 s10** the reply opens
answer-first `CHOICE: 3 (Cast nothing right now)`, then reasons extensively and concludes *"So I
will cast Searing Spear at Young Pyromancer... So Choice 1"* with a PLAN confirming Searing Spear —
but **never re-emits a coded `CHOICE: 1` line**. The parser (correctly, per b2's "last coded line")
took the only coded line, `CHOICE: 3`, recording Cast nothing — the OPPOSITE of the model's actual
final decision. Contrast **vs59 s13**, where the model DID re-emit coded lines (`CHOICE:` = [2,1,1])
and b2 correctly recorded 1. **So b2 is not buggy** — the gap is a prose-only reversal that never
gets re-coded. Routes to core-prompt/protocol (commit-discipline) as a convergence candidate — see
general-suggestions.md. Single instance at this seat.

---

## (7) MULLIGAN SURFACE — UNTESTED for a second corpus (draw variance).

All 6 opening hands were textbook 3-land RDW keeps (3 Mountains + 1-2 drops and/or burn); 0
mulligans, 0 bottom events. **No marginal hand (1-land, all-land, spell-heavy) was dealt again this
corpus**, so the aggressive-one-land keep/mull judgment remains observationally unconfirmed — same
as wave-24. Not a failure; a surface draw variance never exercised. (Flagged to synthesis: this
surface has now gone two full corpora unexercised at this seat.)

---

## Loss decomposition — vs59 (T13, -5/13): persist/wither grind ceiling, no misplay.

Front half correct and FAST: deployed Gore-House (unleashed 1/1-counter), Krenko's Command, and
fired **both** Searing Spears at the face (opp 20 -> 17 -> 14 via burn), plus goblin attacks — opp
to 13 by T6, faster than the average grind matchup. Then the opponent's **persist wall** (Safehold
Elite, Rendclaw Trow with **wither**) walled the ground: persist creatures don't die to chump-trades
(they return), and Rendclaw's wither permanently shrinks the model's 1/1 attackers. **The model
drew NO further burn after s13** (verified: 0 Pillar/Searing Spear cast options after s13) — so RDW
had no way to break the wall. It kept deploying (Stromkirk s17, Boros Reckoner s23) and racing per
doctrine, but the ground was closed and the opponent ground it 19 -> -5 over T8-T13. No held-burn
misplay; no available winning line. This is the **RDW-vs-grind race ceiling** (same class as
wave-24's vs133 drain loss) — the guide already documents "a long game is a lost game" and the
racing doctrine; RDW simply loses the close race when burn dries up against persist+wither. The
W->L flip vs wave-24's vs59 win is draw variance on a close race, not a decision regression.
