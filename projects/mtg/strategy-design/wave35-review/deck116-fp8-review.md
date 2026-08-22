# deck116 — FP8 quant A/B follow-up (wave-35): Qwen3.6-35B-A3B official FP8 vs the 4.75-bit arm

Rubric (owner, binding): win rate is irrelevant; score guide ADHERENCE and, where the model
diverges, DIVERGENCE QUALITY (justified audible vs comprehension failure).

- Guide: `projects/mtg/bin/Res/ai/baka/deck116_strategy.txt`
- FP8 arm: qwen36-35b-a3b official FP8, run `matchups-20260822-111102` (6 games, same opponents)
- Prior arms: 4.75-bit (`matchups-20260820-192210`) and 122B (`matchups-20260821-033000`),
  reviewed in `deck116-tier-review.md`. Question: do the 4.75-bit arm's documented failures
  (Piper R4 attack, Emrakul deathtouch block, "cast nothing" residue) recur under FP8?

Scoreable decisions (excl. gamestart/gameend and one empty-reply anomaly): 127.
All 6 opening hands engine-counted 3 lands with a {G} source → all 6 Keeps adherent (R6).
Results (for the record only): won vs deck146 (t11) and vs deck36 (t15); lost the other four.

---

## Prior failure classes — recurrence answer

### Class 1 (R4, Piper attacks): DID NOT RECUR — rule engages explicitly.
- vs deck139 seq15 t9 attackers (Piper listed as A2): trace verbatim — "Wait, should I attack
  with Piper? Rule 4 is explicit: 'ELVISH PIPER NEVER ATTACKS AND NEVER BLOCKS. NOT EVEN ONCE.'
  So definitely just A1." Answered ATTACK: A1.
- vs deck139 seq20 t11 (Piper listed as A1): attacked only the two Finks.
- vs deck139 seq21 t12 blockers (Piper offered as B1 on four branches): trace — "Check rule 4
  again... Correct"; blocked with Finks #2 only.
- Piper never attacked or blocked in any FP8 game. The 4.75-bit trace never surfaced the rule;
  the FP8 trace quotes it verbatim at the exact seam. → quant damage, not tier.

### Class 2 (game-losing block vs printed outcome lines): DID NOT RECUR at the same ask.
- vs deck146 seq10 t8 is the SAME matchup shape the 4.75-bit lost the game on: A1 Triumphant
  Adventurer (first strike + deathtouch, printed "your blocker dies, attacker lives"),
  A2 Silverquill Silencer 3/2 (printed "you kill it, your blocker lives"), big blocker Kozilek.
  FP8 answered BLOCKS: B1:A2 — exactly the guide's block row and the survival-correct pick.
  Caveat (see Residual risk): the trace briefly re-derived A1 as survivable ("Kozilek survives
  due to 12 toughness vs 1 power" — ignoring deathtouch) in a hypothetical branch, but the
  final choice matched the printed lines and it explicitly ranked A2 better.
- All other outcome-line blocks used the printed line correctly: vs deck36 seq13/22/30
  (Jin/Kozilek/Pelakka each on "you kill it, your blocker lives"), vs deck139 seq21 (chump with
  the persist Finks to stay above lethal, math in trace correct).

### Class 3 ("cast nothing" R5 residue): RECURRED, same magnitude.
Five R5 violations (prior arm: three), all harmless:
- vs deck158 seq15 t9 main-1: "Cast nothing" with ToN/Farseek castable ("Pass this turn due to
  lack of enablers" — the guide's named wrong argument) — then cast BOTH in main-2 same turn
  (seq16/18), so net zero card loss; still the letter violation.
- vs deck36 seq18 t11 (Farseek up), seq27 t13 main-1 and seq29 t13 main-2 (ToN up): three
  minor step-2 violations, lethal already assembled on board.
- vs deck36 seq34 t15: "Cast nothing" with Elvish Piper castable — a step-1 violation by the
  letter (creature/enabler castable). Harmless: the lethal swing was taken and won that turn
  (seq35), and a fresh Piper affected nothing. → tier-intrinsic residue, not quant.

---

## NEW defect (FP8): illegal multi-block — game-losing, new class at the same combat seam

**vs deck105 seq25 t12 blockers.** Prompt: at 6 poison, "you would be at 12 of 10 poison —
LETHAL if it all connects... block the poison." Three infect attackers (A1 Blight Mamba 1,
A2 Ichorclaw Myr 1, A3 Skithiryx 4); sole blocker B1 Emrakul, printed "you kill it, your
blocker lives" against ALL THREE, and the boilerplate constraint "Assign each blocker to AT
MOST ONE attacker" printed in full. The model answered `BLOCKS: B1:A1, B1:A2, B1:A3` — its
trace QUOTES the constraint and then dismisses it: "It doesn't explicitly forbid one blocker
from blocking multiple, which is standard MTG rules. I'll proceed." The engine applied only
B1:A1 (chosen_text: "Emrakul blocks Blight Mamba"); A2+A3 connected for 5 poison → 11 ≥ 10,
game lost at 28 life. The legal survival line was printed and trivial: B1:A3 (Skithiryx, the
4-poison attacker; "you kill it, your blocker lives") → 8 poison, Emrakul untaps and wins.
Verdict: COMPREHENSION FAILURE, game-losing. Different mechanism from the 4.75-bit block
blunder (constraint compliance, not combat-math misread), but the same seam: FP8's intent was
right (block the poison) and its execution overrode a printed rule.

Wave-36 docket suggestion: on an illegal multi-assignment reply, re-ask (or reject with the
constraint echoed) instead of silently applying the first pair — this failure was recoverable
at the harness level.

## Other findings

- **Minor R7 violation** — vs deck152 seq10 t7: second Time of Need picked Ulamog (#5) while
  Emrakul was in hand and Kozilek (#13) was eligible in the same reveal; R7 rank says Kozilek
  next. No material effect (lost t9 to the aggro draw before any cheat; the deck's known
  failure case, otherwise played by the book — cf. the 4.75-bit vs deck152 game).
- **Empty-reply anomaly** — vs deck152 seq12 (Farseek PUT reveal): reply and reasoning both
  empty (0 chars). Looks like a harness/API drop, not a model decision; the engine re-asked at
  seq13 and play continued. Excluded from scoring; flagging for the runner.
  (seq13's PLAN line said "Pick Sheoldred" during a land search — stale plan text, choice fine.)
- **R2 clean everywhere**: both Fabricate casts gated correctly (vs deck36 seq7 hand held no
  enabler — verified hand line; vs deck105 seq22 hand held no enabler, Piper already on board
  with a Skithiryx-insurance plan in trace). Enabler-first honored: Amulet t5 (deck146 seq7,
  over ToN/Finks), Amulet t7 (deck36 seq10), Piper t7/t9 (deck139 seq10, deck105 seq15).
- **R3 "target the biggest" perfect on all 8 cheats**: Kozilek/Finks/Jin (deck146 seq8/11/14 —
  each the biggest then in hand, verified hand lines), Jin/Kozilek/Pelakka/Sheoldred (deck36
  seq11/14/23/31 — ditto), Emrakul (deck105 seq21).
- **R7 ToN picks otherwise perfect**: Emrakul first every game; deck158's chain
  Emrakul→Kozilek→Ulamog (seq11/13/17) is the exact rank order with in-hand exclusions.
- **R1 never violated**: no legend was ever cast in any game.

## Justified audibles (3, one ×2)

| Game | seq/turn | Decision | Verdict |
|---|---|---|---|
| vs139 | 13,17/t9,t11 | Declined "Put in Play with Piper targeting Elvish Piper" (only creature in hand) | JUSTIFIED ×2 — trace: "This is definitely a trap"; same audible the 122B made |
| vs105 | 20/t11 | "Cast nothing" holding {G} | JUSTIFIED — spent it on the Piper→Emrakul activation same turn (seq21), then still cast Fabricate (seq22) |
| vs158 | 8/t6 | Finks chumps the 8/8 Orc army over trading with the 2/2 | Guide-silent, reasonable — absorbs 8, persist returns it |

## Adherence

FP8: 120/127 ≈ 94.5% adherent-or-justified — 1 comprehension failure (new class, game-losing),
5 minor R5 violations, 1 minor R7 pick. Prior arms: 4.75-bit 121/126 ≈ 96% (2 comprehension
failures, 3 minor R5); 122B 115/116 ≈ 99% (0 comprehension failures, 1 minor R5).

## Residual risk noted in the traces

FP8 still RE-DERIVES combat outcomes alongside the printed lines rather than treating them as
authoritative (deck146 seq10's deathtouch slip in a discarded branch; deck105 seq25's "standard
MTG rules" override). When its derivation agrees with the print it lands right; deck105 shows
what happens when it doesn't. The 122B's margin in the prior review — consulting the printed
rule at the seam — is only partially closed.

## Verdict — quant vs tier for this seat

**Split verdict.** The 4.75-bit arm's two signature comprehension failures were QUANT DAMAGE:
under the official FP8, Rule 4 engages verbatim at the Piper seams and the deathtouch-vs-
"you kill it" block ask is answered per the printed lines — both classes gone. The "cast
nothing" R5 residue is TIER-INTRINSIC: it recurred at slightly higher count (5 vs 3, all
harmless). But FP8 does NOT clear the seat: it produced its own game-losing combat-seam
comprehension failure (the illegal multi-block, a printed-constraint override the 122B never
exhibited). Combat-seam fragility at this tier changed shape rather than disappearing. Keep
the 122B on deck116; if the 35B-FP8 is retried after wave-36, gate it on blocker-assignment
legality (and consider the harness-side re-ask, which would have saved the deck105 game).
