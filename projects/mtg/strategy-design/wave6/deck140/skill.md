# Deck-140 wave-6 skill proposals (for the synthesis agent)

Two method contributions from this seat, each multi-instance in this corpus and generalizable
beyond deck140. Plus confirmations of existing method. Format matches the wave-5 changelog.

---

## PROPOSAL 1 (primary) — INTRA-GUIDE CONTRADICTION: a #1 rule can fail because ANOTHER rule in the SAME guide forbids executing it

**The pattern.** Wave 5 correctly diagnosed deck140's leak (chipping away the scarce Rakdos's Return)
and installed a loud #1 rule: "HELD FOR LETHAL." Wave 6 shows the rule was disobeyed 6/7 — but NOT
because it was too weak or ignored. A *different* rule lower in the same guide (the cast-nothing scope:
*"'Cast nothing' is correct ONLY when every offered spell is a sweeper AND the opponent has no
creatures - otherwise cast something"*) **forbade the very action the #1 rule required.** Rakdos's
Return is not a sweeper, so on an empty-board turn the cast-nothing rule said "cast something," and the
model fired the burst. The proof is a two-option window (vs110 t8 seq11: `1. Cast Rakdos / 2. Cast
nothing`) where the PLAN said "hold both Rakdos's Returns" and it still took option 1 — because "hold"
was not a permitted output.

**Why the lower rule wins.** The executor resolves a contradiction toward the rule that **names the
actual option in front of it.** The #1 rule spoke about a card ("hold Rakdos's Return"); the
cast-nothing rule spoke about the literal choice ("otherwise cast something" → the option list has a
cast, take it). Concreteness beats prominence. This is the mirror of the skill's existing "representation
beats instruction": here it is *intra-guide*, one guide rule out-representing another.

**The method addition (Step 0 / Step 2).** When a #1 rule UNDER-fires (disobeyed, not overshot), before
concluding it's too weak, **scan the rest of the guide for a rule whose scope EXCLUDES the #1 rule's
action** — especially any cast-floor / cast-nothing / "otherwise cast something" clause, since those
enumerate the permitted holds and a hold the #1 rule needs may not be on the list. The fix is to
**widen the lower rule's exception to name the #1 rule's action**, not to shout the #1 rule louder. This
is a sibling of DISPLACEMENT (the leak moved to an adjacent SEAM); here the leak moved to an adjacent
RULE in the same guide. Add to the Step-0 audit: "does any OTHER guide line forbid the action my #1
rule commands?" — a self-inflicted contradiction is invisible if you only audit the #1 rule against the
logs.

**Adapted, not new doctrine:** extends the existing "cast-nothing / act-floor exception list must be
COMPLETE" idea — the act-floor's closed-exception list is load-bearing precisely because an action it
omits becomes forbidden. New emphasis: the omission can silently veto YOUR OWN top rule.

---

## PROPOSAL 2 — REMOVING AN ENGINE GUARD RESURFACES THE BEHAVIOR IT MASKED: re-audit for it

**The pattern.** This is the first corpus with combo-hold hint-vetoes removed, so the model's sweeper
picks now resolve. The engine hint (`restriction{type(creature)~morethan~1}`) had silently enforced
"2+ creatures" on Pyroclasm/Wrath/Damnation. With it gone, a **latent false belief the guard was
masking** surfaced immediately: the model cast sweepers into artifact-only boards, its PLANs stating
*"Cast Pyroclasm to destroy all opponent artifacts."* The false belief (board wipes hit artifacts) was
always present; the guard had been absorbing its cost. ~7/22 sweeps were wasted the moment the guard
lifted.

**The method addition (Step 0, when an engine guard was removed before this corpus).** When the wave's
engine change REMOVES a guard/veto that was gating a class of actions, **do not just verify the newly-
allowed action succeeds** (it does — that's the fix working). **Re-audit for the misplays the guard was
silently preventing** — they resurface as new guide work. Concretely: for each guard removed, ask "what
bad version of this action did the guard block?" and grep the new PLANs for it. The guard's removal is
an A/B that exposes the model's unmediated behavior; the guide must now carry what the engine used to.

**The specific fix it produced:** a corrective FACT anchored to the named cards + the engine `auto=`
(Pyroclasm `damage:2 all(creature)`, Wrath/Damnation `bury all(creature)`, BSZ `counter(-1/-1,X)
all(creature)` — creatures only). Standard corrective-fact form; the contribution is the *trigger* to
go looking (guard removed → latent behavior resurfaces).

---

## CONFIRMATIONS (existing method fired correctly — no change)
- **Stopgap lifecycle / X-menu value-anchored flip (wave-5 deck140 P1):** the flipped, value-anchored
  X rule ("pick option 1 = biggest X, verify the number") held 7/7 max-X. The reversal is fully
  absorbed; keep the compressed one-line confirm. The wave-4 index/value trap is dead two waves.
- **Harness-defect sweep first:** 0 desyncs / 295, 0 fallbacks — clean, as wave 5. Ran before scoring.
- **Timeout triage before blaming the guide:** correctly separated vs133 (latency-starved infra) from
  vs110 (piloting loss mislabeled "winning-but-slow" in wave 5). The "read latency + who's ahead first"
  discipline caught a wave-5 mis-attribution.
- **Residual → deck construction:** vs44/vs109 reach starvation routed upstream, not fixed with prose.
- **Win-as-mirror transcription:** vs131's Blightsteel-infect-vs-lifegain line lifted from the actual
  winning game into one keyed guide line, per "transcribe the pilot's own proven line."

## NOT proposed for CORE (single-seat / per-deck)
- The Rakdos hold-vs-burn tension is per-deck (a scarce X-reach finisher vs the core's burn-is-clock
  prior). Handled by an in-guide by-name override, not a core edit. Convergence-watch only if another
  deck runs a scarce X-burst finisher that the burn-prior mis-fires.
