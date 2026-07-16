# deck35 (Mighty Slivers) — GUIDELESS FIRST PASS — findings

Seat: deck35, lord-stack tribal (GUR Slivers), **guideless** (core prompt only). Record **2/6**.
Binary /tmp/wagic-7cdcf9c73. Corpus matchups-20260716-151559.

Deck: 35 Slivers + 27 lands (12 Forest / 5 Island / 10 Mountain). Every non-basic is a Sliver
lord: each one grants an anthem or keyword to the WHOLE sliver team (engine `auto=lord(sliver) …`).
Base bodies are tiny (mostly 1/1–2/2); the deck's power is CUMULATIVE — N lords make N buffs apply
to every sliver at once. Win path = go wide, stack lords, race with evasion (flying/unblockable/
trample) + pump. This is an **aggro swarm race deck**, not a value/midrange deck.

## Record split (the whole story is in the split)

| opp | result | my/opp final | attackers decisions | note |
|-----|--------|--------------|---------------------|------|
| 21  | WIN t11 | 2 / -2      | 5 | raced from t3, Heart Sliver haste early |
| 135 | WIN t9  | 20 / -6     | 4 | dominant race, Heart early, opp no blockers |
| 17  | LOSS t10| -1 / 2      | 3 | genuine close race, lost by 1 — good play |
| 110 | LOSS t10| -2 / 12     | 2 | raced but too slow / defensive lords |
| 49  | LOSS t12| -3 / 20     | **0** | **durdled to death, dealt ZERO damage** |
| 62  | LOSS t10| -6 / 20     | **0** | **durdled to death, dealt ZERO damage** |

The two clean losses (49, 62) left the opponent at **20 life** — deck35 never attacked once. The
wins are games where it curved into an early haste-enabled board and simply raced an empty side.
The deck's win/loss axis is almost entirely **"did it race or did it turtle,"** not card quality.

## Failure classes

### FC1 — THE DEFENSIVE SPIRAL (dominant, game-losing). The model SAYS "be the aggressor," then never attacks and chump-blocks its whole swarm away.
Across both zero-damage losses the model narrates the correct plan and does the opposite. It
repeatedly writes "I must be the aggressor to win the race" / "I need to apply pressure
immediately" (deck62 s6, deck49 s3/s9) — then every concrete combat decision is a BLOCK, and it
declares **zero attackers all game**.

- **deck49**: cast Muscle(t3), Striking+Gemhide(t5), Horned+Striking(t7) — five slivers — and
  traded/chumped every one away as a blocker on t6 (s8), t8 (s12), t10 (s15). By **t9 its
  battlefield shows `creatures: 0`** (prompt confirmed) despite deploying five bodies. It never
  once reached an attack step with a live untapped creature → **0 `attackers` records**, opp
  ends at 20.
- **deck62**: same shape. Deploys Gemhide/Muscle/Spinneret/Fury, uses them as blockers (s8, s14)
  or holds them home "to block next turn" (s9/s11), **0 `attackers` records**, opp ends at 20.

Mechanism: the model treats each sliver as a one-shot chump blocker to shave 2–3 life, instead of
as a permanent that must survive to swing. For a swarm-race deck this is a death spiral — the
board never accumulates, the clock never starts, and a marginally faster opponent wins uncontested.
The core prompt's generic "preserve life / trade favorably" instinct is actively LOSING this deck;
the guide must **countermand** it (Step 0-bis rule d).

### FC2 — Wrong lord priority: deploys DEFENSIVE lords over RACE-CLOSERS; never fetches Heart Sliver (haste) early.
The two WINS both had **Heart Sliver (haste) down early**, so freshly-cast slivers attacked the
turn they landed and the clock never stalled (deck21 s6 t3, deck135 s6 t3 — attacking by turn 3).
The losses lead on defensive/situational lords — Spinneret (reach) into a race (deck110 s4,
deck62 s9), Striking (first strike) redundantly (deck49 s5/s7/s11 cast Striking THREE times),
Horned (trample) with nothing to trample — while the raw **race-closers sit in hand**: Winged
(flying = evasion vs any ground deck), **Shifting (can't be blocked except by Slivers = flat
unblockable vs every non-sliver opponent in this pool)**, Muscle/Might/Bonesplitter (raw pump),
Fury (double strike). The model never once deployed Shifting Sliver despite it being an
auto-include kill button here.

### FC3 — Enormous reasoning tax on first-strike / trample / reach BLOCKING math (representation/comprehension signal).
Blocking decisions balloon into multi-thousand-character rule re-derivations, several with the
model literally re-reading combat rules mid-answer:
- deck110 s5 — **8717 chars** re-deriving reach/flying vs Signal Pest.
- deck62 s14 — **5720 chars**; s5 — **2531 chars** (mis-computed a 1/3 Canopy Spider as dealing
  trample overflow it does not have: "the remaining 2 damage goes to me").
- deck49 s12 — **2271 chars**; s16 — **2254 chars** ("Wait, let me re-read the combat rules:
  'a creature it kills deals NO damage back'").
This tax lands almost entirely at the BLOCK seam, which is exactly the seam the deck should mostly
be DECLINING (FC1). Cutting the blocking down (guide: "you are the beatdown, rarely block")
removes both the misplays and the tax at once. The residual first-strike-math confusion is a
general-comprehension item → notes/general.

### FC4 — Verbal misconception "lords buff FUTURE slivers only" (minor, never bit).
deck49 s4: "grants +1/+1 to all **future** Slivers"; similar phrasing recurs. Wrong — a lord buffs
every sliver already in play too. It never caused a misplay because the engine renders the buffed
[X/Y] and the model reads that (see buff-math verdict), but the guide should state it plainly to
kill the tic and because it matters for sequencing intuition (drop the pump lord and your EXISTING
attackers grow this turn).

## CUMULATIVE-BUFF-MATH VERDICT (the brief's specific watch): PASS — the model understands it.

The model does **not** evaluate slivers at printed base stats. It correctly aggregates cumulative
buffs and applies lord-granted keywords team-wide, because it trusts the engine-rendered [X/Y] and
keyword tags on the battlefield line:
- deck21 s16: with Muscle + Winged + Striking out, it read all four 1/1 slivers as **"2/2 flying
  first strike"** and computed **8 damage** — correct on all three counts (Muscle's +1/+1 stacked
  onto existing bodies, Winged's flying and Striking's first strike applied team-wide).
- deck135 s13: five slivers (Striking/Heart/Heart/Horned/Muscle) → summed **"11 power"** with
  "first strike, trample, haste" — exactly right (2+2+2+3+2 with Muscle's anthem).
- deck17 s14: summed the pumped team to 12 correctly.

**Caveat (do not over-claim):** every observed case was a SINGLE +1/+1 lord (Muscle). No corpus
game ever had two or three pump lords out at once (Muscle+Might+Bonesplitter = +5/+2 team), so the
STACKING of multiple pump anthems is untested, not proven. And the model's verbal model of the
rule is wrong (FC4) even though its arithmetic off the rendered line is right. Guidance: **reinforce
"trust the rendered [X/Y], do not recompute the lord stack"** (converges with deck62 S4) rather
than re-teaching cumulative buffs from scratch — the model already gets the arithmetic; teach it
to keep trusting the number and to remember the anthem hits creatures already in play.

## What went RIGHT (do not re-teach — Step 0-bis)
- Haste recognition: correctly attacked summoning-sick bodies once Heart Sliver was out, and
  correctly HELD a sick Horned Sliver when no haste was available (deck17 s7).
- Trusting rendered buffed stats and team keywords (buff-math verdict).
- Mana/curve basics, mulligan keeps (all six keeps were reasonable), "I have no instants, don't
  hold mana" reasoning — all handled by the core. No mana agonizing worth taxing.
