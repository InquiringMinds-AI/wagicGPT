# Deck-140 review — wave 22 (ROTATED-IN re-validation under the modern engine)

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus.

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260723-173843`, 6 deck140 seat games,
binary `/tmp/wagic-1d8ed8194` (wave-22 engine batch). Guide DEPLOYED = the wave-11 vintage,
byte-identical to what wave-15 froze (7364 bytes). deck140 was ROTATED IN this cycle after
converging in wave 15 — mandate: re-validate the OLD guide under the modern representation and
retire stale crutches per the current (wave-21) strategy-writing skill. Pool CHANGED (135/62/27/102
continuing + 109 Mono-R Aggro + 44 Faerie Tempo new), so cross-wave record comparison is WEAK.
`choice` 1-based; `choice -1` = heuristic fallback; `CHOICE: 0` = pass.

**Record: 3 W / 3 L** (140:3/6). From `results.tsv`:

| game | opp | outcome | end life | class |
|---|---|---|---|---|
| vs135 | Modern Snow | **WIN adj** 18-17 | razor-thin life-adjudication; swept correctly, no finisher, coasted 1 ahead |
| vs62 | Enchantresses | **LOSS adj** 2-20 | ground to 2 by the Yavimaya-Enchantress engine + trample; never deployed a clock |
| vs27 | Zombies | **WIN adj** 28-17 T24 | textbook grind: sweeps + Staff clock + Venser's + Lightmine |
| vs109 | **Mono-R Aggro** | **LOSS (death) 3->-1 T20** | STRUCTURAL: hasty/burn raced sorcery-speed control; at 9 life by T5 |
| vs102 | **Tergrid (mono-B)** | **LOSS (death) 23->0 T12** | Archon-of-Cruelty forced-discard made it PITCH ITS SWEEPERS; go-wide then killed it |
| vs44  | Faerie Tempo | **WIN adj** 24-18 T35 | dominated; ground a tempo deck out with edicts/Pyroclasm, Staff + Lightmine, life never dropped below 24 |

Two of the three losses are the two NEW-pool decks (109 aggro, 102 Tergrid); vs62 is a
continuing-opponent grind loss. **All three losses are structural OR representation-driven — none is
a SWEEP-TRIGGER / sweeper-selection / Rakdos-hold failure** (those lines were obeyed everywhere they
fired). The guide's core machinery is intact under the modern engine; the new leak is a seam the
old guide never covered (forced discard).

Tags: [ROTATED-IN RE-VALIDATE] · [ECHO-CLUSTER OWNER] · [NEW LEAK: forced discard] · [ENGINE] discard-target
framing + X-menu mana-pool display -> notes ledger · [STRUCTURAL] aggro/go-wide vs sorcery-speed control.

## Headline
1. **The stale_echo cluster is a THIRD, un-closed echo-family member — NOT covered by either shipped
   (c) fix.** All 4 of my stale_echos are the SAME root: on a **child menu spawned from an
   already-consumed parent cast** (the ANNOUNCE_X menu for Black Sun's Zenith), the model corroborates
   its `CHOICE: N` by re-naming the PARENT action ("Cast Black Sun's Zenith with X=N") instead of the
   bare option label ("X = N"). The echo detector matches "Black Sun's Zenith" against the consumed
   parent cast option -> fires stale_echo -> discards a numerically valid, in-range `CHOICE: 1`. The
   "<spell> targeting X" prefix-strip only knows `targeting` (target menus); consumePlan
   label-filtering only filters CoT labels. Neither touches `Cast <spell> with X=N`, and neither
   normalizes the whitespace gap `X=N` (model) vs `X = N` (option). **4/4 BSZ X-menus this corpus hit
   it. Family is OPEN — third member documented below.** Harm this corpus = ZERO (the heuristic's
   biggest-X argmax matched what the model wanted every time), but it is a HARNESS-OWNED false
   positive (per the wave-21 headline-1 ruling: a fallback logged against a reply that CONTAINS a
   correct, in-range, uncontested first-line coded answer is a HARNESS bug, not a guide/model one).
   vs102 s12 is the clean proof: a SHORT, correct, uncontradicted answer-first `CHOICE: 1` discarded.
2. **The vs102 loss is a GENUINE new play-quality leak the old guide never covered: forced
   self-discard.** Archon of Cruelty triggered 8 forced-discard "TARGET CHOICE" prompts. deck140
   discarded **Damnation** (s13) then **Pyroclasm** (s14) — its two live sweepers — into a go-wide
   Tergrid board, then died at T12 to the creatures those sweepers would have cleared. Root is a
   REPRESENTATION inversion (below) BUT it is also guide-fixable interim: a control deck must pitch a
   LAND/redundant card, never a sweeper or finisher. **This is the new leak -> the guide's one real
   addition this wave.**
3. **Re-validation verdict: the guide's core is OBEYED and load-bearing under the modern engine, but
   two belief-kill crutches are now DOUBLE COVERAGE and one seam is uncovered.** SWEEP TRIGGER,
   sweeper-selection, #1-Rakdos-hold, shroud-kill, Staff face-ping, finisher order — all obeyed where
   exercised. The `(creatures: N)` count now ships as printed representation, so the guide's full
   **tapped-still-counts** and **artifacts-don't-lower-C** walkthroughs are demotable to one-line
   marker nods (the count carries "how many"); the **shroud** targeting-fact STAYS (the count does not
   carry "how your spell interacts") — exactly the wave-8 deck140 per-belief demotion ruling. Net: a
   constrained diff (demote 2 crutches, add 1 rule, trim), NOT a faithful rewrite (deck110's 0/6
   warning applies — the record is OBEYED-BUT-LOSING with structural losses).

## THE STALE_ECHO CLUSTER — classified against the two shipped (c) fixes (I OWN THIS)

Shipped fixes I'm classifying against: (1) **"<spell> targeting X" source-prefix strip** (anchored to
the pending target-spell source); (2) **consumePlan label-filtering** (a combat-math "Attack:" CoT
line can't hijack a valid CHOICE).

| # | rec | menu | option labels | model's CHOICE line | why (c) did NOT fire | harm |
|---|---|---|---|---|---|---|
| 1 | vs62 s27 | ANNOUNCE_X (BSZ) | `X = 3/2/1/0` | `CHOICE: 1 (Cast Black Sun's Zenith with X=3)` | prefix is `Cast <spell> with`, not `targeting`; `X=3`!=`X = 3` | 0 (heur X=3) |
| 2 | vs27 s26 | ANNOUNCE_X (BSZ) | `X = 2/1/0` | `CHOICE: 1 (Cast Black Sun's Zenith with X=2)` | same | 0 (heur X=2) |
| 3 | vs102 s12 | ANNOUNCE_X (BSZ) | `X = 1/0` | `CHOICE: 1 (Cast Black Sun's Zenith with X=1)` | same — SHORT clean reply, cleanest proof | 0 (heur X=1) |
| 4 | vs102 s9 | priority (Elixir only) | `Life with Elixir...` | multi-line: `CHOICE: 0`->...`CHOICE: 1 (Cast Black Sun's Zenith...)`->`CHOICE: 0` | a mid-reply CHOICE names BSZ, a card ABSENT from the option set (sorcery in upkeep) -> absent-card echo; last-well-formed `CHOICE: 0` not taken | 0 (heur pass, correct) |

**Classification: #1-#3 are ONE genuinely-NEW shape** the shipped fixes should be extended to cover
(NOT a regression of them — they were scoped to `targeting`/label CoT, and this shape is neither).
Precise mechanism for the ledger:

> **ANNOUNCE_X (and any child menu spawned from a consumed parent cast) — spell-name-prefix echo.**
> The option labels are bare parameter values (`X = N`). The model answers `CHOICE: N (Cast <spell>
> with X=N)`, re-stating the parent cast (already consumed as a separate decision). The echo detector
> finds `<spell>` in the already-consumed parent cast option and classifies the answer as a stale
> echo of that consumed decision -> heuristic. Two compounding breaks: (a) the source-prefix strip only
> recognizes the literal token `targeting`, so `Cast <spell> with` survives; (b) even after any strip,
> `X=N` (no spaces, model) != `X = N` (spaces, option label) fails the name match. **Fix:** on child
> menus whose option labels are bare parameter values, generalize the source-prefix strip to also peel
> `Cast <pending-source> with ` / `<pending-source> with `, and whitespace-normalize around `=`; then
> trust the in-range numeric `CHOICE: N`. 4/4 BSZ X-menus this corpus.

**#4 is a related but distinct absent-card + multi-CHOICE shape** — a reply with `CHOICE: 0 ...
CHOICE: 1(absent-card) ... CHOICE: 0` where the correct answer (`CHOICE: 0` pass) bookends a
hallucinated middle CHOICE naming a card not in the option set. "last-well-formed-CHOICE-wins" should
have taken the final `CHOICE: 0`; the absent-card echo pre-empted it. Route: same ledger family — when
a coded CHOICE names a card absent from the option set, DROP that line and keep parsing for a
well-formed in-range CHOICE rather than firing stale_echo on the whole reply.

**Family verdict: NOT CLOSED. A third member exists** (the X-announce spell-name-prefix shape) plus a
fourth-shape residual (#4). The (c) work reduced the family but did not terminate it. Both are
HARNESS-owned per the headline-1 doctrine — do NOT paper over in guide prose (a guide cannot reliably
control how the model phrases a CHOICE parenthetical, and the correct answer WAS emitted).

**Why it recurs specifically on the BSZ X-menu (linked to an engine display bug):** every X-menu reply
that echoed was ALSO fighting the mana display. The follow-up X-menu shows `Mana available: (no
untapped sources) | Already in pool: {r}{b}{b}{w}{w}` — the parent cast already tapped the lands and
floated the mana, so the "no untapped sources" phrase reads as "I have no mana" and contradicts the
full pool. vs62 s27 and vs27 s26 spent 40+ lines and 58-199 s re-deriving the mana from scratch —
which is exactly when the model re-anchors to "Cast Black Sun's Zenith with X=N" (the parent action)
and trips the echo. Fixing the display friction would cut BOTH the latency AND the echo. -> notes.md.

## THE NEW LEAK — forced self-discard (vs102, cost the game)

s13/s14 prompts (verbatim framing): `TARGET CHOICE for discard a card (this spell/ability is already
on the stack and needs a target - it is NOT a cast or phase step). Pick the ONE target it will affect
from the list below, and answer with the chosen TARGET's name`.

- s13 (T10, L20): options [Clifftop Retreat (land), Pyroclasm, **Damnation**]. deck140 chose **Damnation
  (option 3)** — discarded its single best board wipe into a 6/6 Archon + Zombie-army board.
- s14 (T10, Blockers, L17): options [Clifftop Retreat (land), **Pyroclasm**]. Its own PLAN said "Discard
  Clifftop Retreat"; it then chose **Pyroclasm (option 2)** — a PLAN/CHOICE mismatch, pitching the
  second sweeper over the land it had just named.
- Result: by T11 deck140 had no sweeper for the reloaded board and died T12 (23->0 in ~2 turns).

**Root = representation inversion.** The forced-discard seam reuses the generic "TARGET CHOICE ... the
ONE target it will affect" template, calibrated for choosing what a spell AFFECTS (normally an
opponent's permanent you want DEAD -> pick the most relevant). For a self-discard the "target" is YOUR
OWN card that you LOSE, so the value logic inverts: pick the LEAST relevant. The template gives the
model no cue to invert, and it pitched its highest-impact cards. -> engine/core ledger (a
discard/sacrifice detrimental-framing header, analogous to the wave-21 reveal-framing header).

**Guide-fixable interim (ADD):** deck140 has 14 sweepers and floods lands — its correct discard is
almost always a LAND, else its most redundant sweeper; NEVER a finisher or its last board wipe for a
live board. n=1 game but 8 instances and a loss — strong enough to legislate a short rule. This is the
guide's one real addition this wave (the rotated-in mandate's payoff: a new leak the old guide never
saw because the mechanic wasn't in the old pool).

## GUIDE RE-VALIDATION — line by line under the modern engine

- **SWEEP TRIGGER (C = printed `(creatures: N)`):** OBEYED, load-bearing, teaches what representation
  shows. KEEP the core. Cruel Edict on C=1 + recent-attack fired correctly ~10x (vs135 s6, vs62
  s10/s20, vs27 s20, vs102 s5/s7, vs109 s12, vs44 s6/s26/s34/s41). Pyroclasm on 2+ small, Wrath/
  Damnation on bigger, BSZ on single grown creature — all correct selection.
- **tapped-still-counts walkthrough / artifacts-don't-lower-C:** the `[tapped - untaps and can attack
  next turn]` tag and the `(creatures: N)` count both ship now; the model never wrongly held on a
  tapped/artifact board this corpus. Per the wave-8 deck140 per-belief demotion (the count carries
  "how many") -> **DEMOTE both to one-line marker-anchored nods.**
- **shroud does NOT stop a non-targeted sweep:** the count does NOT carry "how your spell interacts"
  -> **KEEP** (per the same wave-8 ruling). Not exercised this corpus, but a real, count-immune
  misconception — stays compressed.
- **#1 RULE — HOLD Rakdos's Return:** OBEYED. Only one Rakdos fire all corpus (vs109 s22): at L3, empty
  board, facing lethal mono-R burn with a full opp grip — a desperation hand-strip in an already-lost
  game, covered by the guide's below-lethal discard exception. Not a violation, not a missed lethal.
- **THE X MENU (pick option 1 = biggest):** OBEYED where it reached the parser; the biggest-X pick was
  correct every time (BSZ shuffles back, so max-X is free). The menu now self-labels "option 1 is the
  LARGEST X" — the guide line is lightly redundant but cheap; KEEP compressed.
- **DEPLOY WHEN SAFE (finisher order):** OBEYED. Staff of Nin deployed on safe boards (vs27 s40, vs44
  s58); Lightmine vs attackers (vs62 s35, vs27 s51, vs44 s17); Venser's as survival (vs27 s32 at L4).
  Blightsteel never reached (needs 12 mana; games capped first). KEEP.
- **STAFF OF NIN face-ping:** OBEYED (vs27 s41/49/54, vs44 s59/60 all "a player"). KEEP.
- **LANDS / MULLIGAN / no-instants OVERRIDE:** OBEYED (a land every turn; kept all openers). KEEP.

## Timeout economics / latency (who was ahead at each cap)
4 of 6 games life-adjudicated at cap (vs135/62/27/44); vs109 + vs102 were REAL board deaths (T20, T12).
Of the 4 adjudications deck140 was AHEAD in 3 (vs135 +1, vs27 +11, vs44 +6 -> WON) and BEHIND in 1
(vs62 -18 -> LOST). The cap outcomes are LEGITIMATE — they reflect board/life reality, and the cap
robbed deck140 of no win. deck140 IS the corpus grinder (highest decision counts: vs44 66, vs27 54)
and its latency is high (medians 14-26 s, p90 up to 178 s), which is what pushes its games to the
2400 s wall — but **latency did not cause any loss**: the two deaths were board losses well before any
cap, and the one adjudicated loss (vs62) was correctly behind. The worst latency spikes (150-200 s)
are the BSZ X-menu mana-pool-confusion rambles (linked to the echo above) — a latency + echo twofer
that the display fix would cut.

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. **SWEEP TRIGGER:** obeyed, no overshoot (no case of casting a sweeper into C=0; the many "Cast
   nothing" on empty boards are correct holds). No displacement to a new guide seam.
2. **#1 Rakdos-hold:** obeyed; the single fire was a covered desperation. No over-fire, no missed lethal.
3. **The residual did NOT move to a guide seam — it sits at (a) the HARNESS (X-menu echo, third family
   member), (b) the ENGINE/CORE (forced-discard framing inversion + X-menu mana-pool display), and
   (c) DECK CONSTRUCTION (no early answer to hasty aggro / go-wide reloads — the vs109 + vs102 deaths).**
4. The one NEW guide rule (forced-discard) is an ADDITION for an uncovered seam, not an overshoot fix.
