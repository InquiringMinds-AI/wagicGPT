# Deck44 — proposed revisions to the strategy-writing skill (wave 4)

Format per revision: QUOTE (wave-3 skill passage) → CHANGE (replacement or addition) →
WHY (one line, with deck44 evidence). Written to merge: each proposal is independent and
names its target section.

What the skill got RIGHT for me this wave (keep, no edits proposed): Step 0's
demote-don't-delete rule (compressing the now-obeyed Archmage/hybrid lines freed space for
the counter section without regression risk); the offered-vs-taken tally (found
Counterspell 0/12 instantly); "confirm before you accuse" (the vs110 windows looked like
model error until the prompt showed the spell had already resolved); "opponent decklists
are known — name the actual cards".

---

## R1 (Step 5, new bullet) — The representation-defect workaround: TEACH THE TELL

QUOTE (Output and handoff): "representation defects found in log mining — misleading pool
text, mangled cost rendering — get reported upstream too; the guide carries a defensive
line only while the defect is live."

CHANGE — promote from a handoff aside to a Step 5 pattern bullet:

> **When the prompt HIDES a deciding fact, teach the TELL that proves it.** If the board
> representation omits a fact the pilot needs (the stack, a pending trigger), find a
> reliable proxy in what the pilot DOES see — often the option list itself, since the
> engine only offers legal actions — and write a perception procedure that derives the
> fact from the proxy: "the option appearing IS the proof; the pending spell is the last
> 'hand -> stack' log line with no 'stack ->' line after it." A tell converts an
> interface hole into an executable rule. Mark every tell section as a STOPGAP in the
> handoff with its deletion trigger ("delete when the stack is surfaced in CURRENT
> SITUATION") so it is removed from every guide at once when the platform fix lands.

WHY: deck44's counterspells fired 0/12 games because the stack is invisible in CURRENT
SITUATION; the model's PLAN said "no spell on the stack to counter" while Guttersnipe sat
on it (e2540 s8). The tell was the only guide-layer fix available, and the skill had no
pattern for authoring one.

## R2 (Step 2, new required substep) — Verify a tell empirically before teaching it

QUOTE (Step 2 opening): "**FIRST: the EXECUTABILITY AUDIT.** Before judging any play,
establish which decision surfaces the harness actually gives this deck..."

CHANGE — add a parallel required audit immediately after it:

> **THE TELL AUDIT (required for any inference rule).** Before teaching any rule of the
> form "when you see X, Y is true," scan the corpus for EVERY window where X appeared and
> confirm Y held in all of them — script it, don't sample. One counterexample means the
> tell trains a false-positive the executor will obey off a cliff (it cannot judge
> exceptions). Record the check in your findings ("all N counter-offer windows had a live
> pending spell") so the synthesis agent can trust the rule.

WHY: I nearly wrote "offered = counterable" from two games' evidence; checking all 12
games' counter-offer windows (every one had a live `hand -> stack` spell) is what makes
the rule safe to state as an absolute — and absolutes are the only register the executor
reliably executes.

## R3 (Step 5, amend the "Concrete, named, numeric" bullet) — Thresholds must match the observed pool

QUOTE (Step 5): "**Concrete, named, numeric.** Name cards and costs; state thresholds
numerically and say what they turn on."

CHANGE — append:

> A numeric threshold is only real if it MATCHES the games: before shipping any
> "X+ mana / power Y+" rule, check it against the threats that actually appeared in the
> corpus and decided games. Derive selection rules as a NAMED-CARD table from the observed
> pool first ("counter Master of Etherium, Cranial Plating, Guttersnipe on sight"), then
> add ONE numeric catch-all keyed on visible state for the unnamed remainder ("any
> creature or equipment while your life is 12 or less"). Generic-Magic thresholds
> inherited from prior waves are stale-rule candidates like any other line.

WHY: deck44's inherited "counter the first 4+ mana spell or power-4 creature" matched ZERO
of the cards that actually killed it — Master of Etherium is {2}{u} printed 0/0 (became
11/6), Cranial Plating {2}, Guttersnipe {2}{r}. The rule was well-formed, numeric, and
useless against the real pool.

## R4 (Step 5, amend the repeatable-activation bullet) — Scope by WINDOW, and say the option comes back

QUOTE (Step 5): "**Repeatable-cost activations need a when-NOT-to keyed to observable
state.** 'Only when it helps' requires reasoning qwen doesn't do; write 'NEVER the turn it
entered; ONLY in main phase 1 when it will attack this combat.'"

CHANGE — replace the bullet body with:

> Write the rule per PROMPT WINDOW, not per condition: name each window where the engine
> will offer the activation and give the action for each ("Upkeep prompt: pass.
> Opponent's turn: pass. Your Main Phase 1 on a turn it attacks: pay it, once."), and
> state explicitly that a passed option RETURNS ("it will still be offered in Main
> Phase 1"). A condition-scoped rule ("only in main phase 1 when attacking") tells the
> executor when paying is right but not what to do at the OTHER windows — it pays early
> anyway, fearing the window won't come back, and can satisfy the rule AND waste mana in
> the same turn.

WHY: deck44's pilot obeyed the wave-3 condition rule's positive half and STILL paid the
Sleep-Cursed untap in Upkeep and again in Main 1 of the same turn — 4 mana burned in a
race lost with the opponent at 7 life (e2880 turn 7). The leak was the unaddressed
windows, not the condition.

## R5 (Step 0, amend first bullet) — Check the fact was VISIBLE before blaming wording

QUOTE (Step 0): "**Read the deployed guide as evidence, and attribute each misplay to a
specific line:** missing, wrong, or too weak to override a general-prompt prior."

CHANGE — extend the attribution ladder:

> ...or too weak to override a general-prompt prior — and BEFORE settling on any wording
> diagnosis, check the REPRESENTATION rung: could the executor even SEE the fact the
> correct play depends on? Pull the full prompt for 2-3 instances of the misplay and
> verify the deciding fact appears somewhere the executor attends to (CURRENT SITUATION or
> the option line — the log tail barely counts). A misplay whose deciding fact is
> invisible is a platform defect wearing a guide costume: rewording the guide will not fix
> it, and the wave will re-diagnose it forever.

WHY: wave 3 diagnosed deck44's "died with counters in hand" as hoard-y guide wording; wave
4 shows the stack was never surfaced, so the model literally could not know there was
something to counter — one prompt-read in wave 3 would have found it a wave earlier.

## R6 (Step 4 item 9, replace) — The closing list recaps positively; quote-and-forbid stays scoped to false beliefs

QUOTE (Step 4): "**DO NOT** — a tight list of the 3-5 specific misplays you actually
observed, each as a flat imperative phrased as the exact situation qwen was in."

CHANGE — replace with:

> **DECIDING SITUATIONS** — a tight closing list of the 3-5 situations that actually
> decided games, each as "situation the executor will recognize → the correct play,"
> recapping rules already stated positively above (the recap re-fires the rule at the
> tail of the guide; a free-standing prohibition there suppresses its neighbors).
> Reserve quote-and-forbid (Step 5) for refuting a VERBATIM false belief mined from the
> replies — that is a corrective fact, not a behavior prohibition — and never place a
> DON'T clause adjacent to the positive instruction that carries the plan.

WHY: the negative-framing A/B (a "do NOT save them..." clause suppressed the adjacent
positive instruction; deck131's positive rewrite went 1→8 draw casts) makes the DO-NOT
section the skill's own residual bleed risk; every deck44 DO-NOT item was already
restatable as situation→play with zero content loss.

---

## Not proposing (considered, rejected)

- A "counterspell doctrine" section: deck-specific content; R1+R2+R3 carry the
  generalizable parts (tell authoring, tell verification, pool-derived tables).
- Relaxing the ~40-70-line budget for tell sections: the budget pressure is what forced
  the Step-0 compression that paid for mine; keep the constraint.
