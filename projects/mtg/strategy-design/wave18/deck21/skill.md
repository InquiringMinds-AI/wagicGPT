# deck21 seat — feedback to the strategy-writing skill (wave-18)

Layer: skill/method observations only. The deck21 GUIDE is frozen this wave; these are notes on
the WRITING PROCESS that this seat's outcome corroborates or refines.

## 1. A first-revision TIGHTENING validates on the SAME matchup that motivated it — design the corpus to re-run it.
The wave-16 race-math exception MISFIRED on a 0-power Birds of Paradise vs62 (lost by 1). The
wave-17 revision added a POWER FLOOR. This corpus put deck21 back in front of **the identical
opponent with the identical card as a legal burn target four times** — and the model sent every
burn face, quoting the new clause verbatim, and won by 12. The clean signal existed *because* the
round-robin re-paired the motivating matchup. Method note (reinforces w16 method-headline-1(d)):
when a revision targets a specific misfire, the strongest possible validation is the SAME card in
the SAME matchup making the OPPOSITE decision. A validation corpus that preserves the motivating
pairing turns "did the class die?" from an inference into a direct observation. Prefer round-robin
schedules that hold motivating matchups fixed across the revision boundary.

## 2. "Quotes the guide verbatim" is the strongest death-certificate for a taught class.
Both closed classes this wave show the model *citing the exact new guide text* at the decision
point (power floor: "Birds of Paradise is a 0-power flyer… not a valid target for race-math
removal"; engine kill: "My strategy guide explicitly states to burn engines worth more dead than 3
face damage"). When the reply narrates the rule and then obeys it, the teaching landed at the
level of the model's stated reasoning, not just its action — the highest-confidence Step-0-ter
"DIED" evidence. Instrument for it: grep replies for guide-phrase echoes at the target decisions.

## 3. A taught fix can fire CORRECTLY inside a game the seat still LOSES — don't let the loss mask the validation.
vs49 LOST, yet it is the cleanest kill-on-sight validation in the corpus: the model killed the
Dragonmaster Outcast on sight at T2 (the exact card it failed to kill in the loss last wave), and
the game was then lost to an unrelated flood (0 creatures from T8). Method: attribute the DECISION,
not the win/loss. A win-indexed reading would have scored vs49 as "still losing = fix didn't work";
the decision-indexed reading correctly reads it as "fix fired, loss is structural." The brief's
"unit of analysis is the DECISION with seq repros" is load-bearing precisely here.

## 4. Converged-guide TRIM: do it as a DEDICATED pass AFTER the freeze, never bundled with a validation.
The brief flags deck21's guide for a trim (grew two waves running). Recommendation to the skill:
a length trim is itself a behavior-risking edit — it must be re-validated. Bundling it with the
freeze that closes the last open class forfeits attribution (any next-wave regression can't be
pinned to the trim vs. noise). The redundancy here is also LOAD-BEARING: the race-math rule is
stated in three sections (BURN / MATCHUPS / SITUATIONS) and the model quoted it from more than one,
which is plausibly *why* the power floor fired so reliably. Skill rule: **trim on a converged,
frozen guide as its own pass with its own validation corpus — treat "freeze" and "trim" as two
separate method operations, never one commit.** (Concrete trim map for a future pass is in
general-suggestions.md.)

## 5. The "giant-reply / computed-P/T-distrust" pocket is now a cross-seat pattern — worth a skill-level note.
deck21's 4 fallbacks (all benign, all in won games) and deck62's known 5-unparsed tax share one
shape: the model over-narrates when analyzing a high-toughness opposing body it cannot kill, and
runs past the CHOICE line. This is not a guide-prose failure (the reasoning is correct); it is a
reply-length/format failure. Skill implication: a guide can't fully fix an emission-format pocket —
route it to the engine/parser layer (see notes.md). Guides that already say "ignore the wall, go
face" don't stop the model from *explaining* the wall at length first.
