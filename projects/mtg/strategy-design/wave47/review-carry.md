# Wave-47 review carry-list (assembled 2026-08-26 at step-1 merge)

Corpus: fourth fair-hand corpus. Binary = master 5b1bf9668 (wave-46 engine-seat fixes
a7310f01c + hand count 1c71d5301 + lanes J/K/L/M). PARSETEST 1519/0. Guides = wave-46
reviewer revisions (19e00cf66) + boundary pass (af0376a37). Same pool/recipe.

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides; synthesis owns skill + general strategy; the core loop (engine
correctness, LLM UX, bugs) is the purpose. Deck-content questions are the reviewer's call.
Adjudicate render predictions by the emitter's actual string. Baselines: fallbacks 0.10-0.28%;
21/21 natural x7. Expect prompt chars to MOVE both ways (R7/R6 add, R8 removes) — measure per
decision kind vs wave 46 (engine seat).

## Set B — engine changes since the wave-46 corpus (each with its lane's prediction)
- a7310f01c: menu header names ability carriers (`A choice is required` 0/N — was 4);
  exemplar from option 1 (`Cast ...` replies into cast-free menus 0/N; stale_echo on non-card
  menus 0); repeat tag from the first repeat (single-option loop windows > 100 s: 0).
- 1c71d5301: `Your hand (N cards):` — replies stating a wrong own-hand size 0/N (was 3/4).
- J 2860f78c2: DRAW PUNISHERS line + `[DRAW PRICE:]` row tags (draws at/below summed cost
  0/N — was 2/2 lethal); annotated `Cast Card Normally / cycling` menu (Cast-Normally chosen
  with a cycle plan 0/N); X=0 null-cast callout + life/draw X preview (X=0 answered where
  X>=1 affordable and X=0 does nothing: 0/N).
- K d30871747: converter doubling folded into blocking-trigger/lifelink tags (sends into
  such rows with their converter printed 0/N; life lost between attackers records 0);
  affirmative `[no creature they control can block this attacker]` (ATTACK: none on it
  0/N; attack rate on formerly untagged rows >= 90%); zero-power STOPS scoped `THIS COMBAT`
  (wall on a dies-row while a lives-row is offered 0/N).
- L 2a4ff28be: `{if you pass here, this option is not offered again this turn}` on rows at
  their decline cap (root cause of R4 = the two-decline allowance; lethal-in-upkeep
  activations taken 100%, was 0/2); priced `Mulligan (a keep after this one would keep N
  cards)` row + NO carried plan on pregame asks (chains reaching a third look with a land
  in hand 0/N — was 9; no seat reaches keeping-4); land-drop asks: no PLAN request +
  consequence sentence (max 2-option latency < 200 s — was 453; land-drop declines 0/N).
- M 8b6545b48: per-clause `{effect:}` budget keeping the ultimate (truncated loyalty
  abilities 0/N); own-side `{effect:}` minus permanents on an option row (own bare
  permanents 0/N); blockers ranged collapse `B2-B22. Vampire #1-#21 ... x21` (blockers
  p90 <= 60 s, tail mean <= 4,600 chars); `{c}` in colour sets (`{1}`/`{x}` in a colour set
  0/N — flip traced to ManaPool::add re-slotting); `N of them able to attack right now`
  header + `{leaves N of your M untapped mana sources untapped}` on cast rows (sweeper
  casts into all-defender boards 0/N; deck125 Staff tap-out re-measured n>=10).

## Set A — wave-46 guide edits: per wave46/deck*/edit-texts.md + wave46/reviewer-boundary-pass.md.

## Carried (wave46/engine-ledger.md LOW + residuals)
damage OBJECT bare (27/273); Clue token textless; mana-only leak via menuObject/targetChooser
(3); ability$ pay-or-lose punishers undetected (lane J residual); R7 name-substring skip; R4
one-window-turn case (no clause when declines=0); Baka menace gang-block probe (red pin).
