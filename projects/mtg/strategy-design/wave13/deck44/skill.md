# Skill contribution — deck44, wave 13b (name-echo era; single-seat; no core/guide edits)

The CURRENT skill is `strategy-design/wave11/strategy-writing-skill.md`. The deck44 guide is FROZEN
byte-identical (10th wave running, md5 `bd097b0c4cf8524fb6cc30b75312cabe`). Per the convergence gate a
single-seat wave cannot add or cut a method/core line. The items below are (1) a re-witness that
PROMOTES an existing rung with a name-echo-era refinement, and (2) diagnostic WATCHes for synthesis.

## Method refinement (re-witness of wave-11 headline 1, extended to the name-echo layer)

Wave-11 headline 1 established: the reconcile-named-action arm must route a plan's NAMED action that is
ABSENT from the options to the best-available LISTED option (Cast-nothing if the plan wanted to hold),
never an arbitrary listed cast, and reject-and-repair an out-of-range index. Wave 13b adds the missing
piece for the SHIPPED name-echo protocol: **a 100%-compliant `(name)` echo does NOT rescue the
absent-echo case — the "index wins on conflict" default overrides it into an unwanted cast.** The
refinement to score/route:

- When the CHOICE echo matches **no current option**, treat that as a **STALENESS SIGNAL**, not a tie to
  be broken by the raw index. Two mechanisms produce it and both point the same way: (a) the echo names
  the PARENT action while the menu is a target sub-menu (deck44 vs135 s24: echo "Cast Go for the Throat"
  vs a 3-target creature list → index 1 = own creature → SELF-DESTROY); (b) the option list SHIFTED after
  a same-turn cast and the echo names the just-cast card while the index now points at a different spell
  (deck44 vs133 s14: echo "Cast Surveilling Sprite" (already cast) → index 1 is now GFTT → unwanted removal).
  In BOTH, the safe route is the Cast-nothing option (or a re-prompt), NOT the index. The current
  index-wins default is the wrong tie-break for absent-echo.
- **Verification signature for a self-destroy-via-own-removal class:** grep target sub-menus
  (`events` contains "Choose the target") for a `chosen_text` carrying `[your battlefield]`. deck44 had
  **1 true self-GFTT** (vs135 s24) — a REGRESSION in outcome from wave-11's "0 self-casts / 29-29 obeyed,"
  even though OWN-ONLY parent windows were still obeyed 20+/20+. The lesson: score self-casts at the
  TARGET SUB-MENU, not only at the parent OWN-ONLY option — wave-11's clean count missed the sub-menu layer
  because that layer's failure mode only appears when a NAMED (mixed own+opponent) target set exists.
- **Separate a FORCED sacrifice from a self-cast** when tallying "own creature to graveyard": deck44
  vs133 s19 was Liliana of the Veil's forced sacrifice (opponent effect), not a self-GFTT — do not count
  it as a self-destroy. It belongs to the same REPRESENTATION shape (unframed sub-menu read as a "glitch")
  but a different owner than F1's reconcile bug.

## Watch (single-seat; promote only with a 2nd seat)

1. **Forced-choice sub-menu representation** (target menus, sacrifice menus): bare creature lists with no
   committing-spell/effect header and no cancel drive the model to call them "glitches" and re-litigate
   the parent decision (deck44 vs135 s24, vs133 s15, vs133 s19). Deck-agnostic — any deck running targeted
   removal or facing edicts/sacrifice will hit it. Synthesis: grep other seats' `Choose the target` /
   sacrifice records for `[your battlefield]` picks and "glitch"/"pass" reasoning at a forced sub-menu.

2. **Mixed index+name declaration partial-parse** (attackers AND choices): the parser takes A-indices and
   silently drops name-form entries. At deck44 every dropped name was an INELIGIBLE (summoning-sick)
   attacker, so 0 cost — but a name→index reconcile arm for attackers must respect eligibility, and the
   same "index wins, name ignored" shape is what makes the CHOICE-side absent-echo bug (item above) fire.
   Promote if a 2nd seat shows a dropped ELIGIBLE attacker or a name-form choice costing a decision.

## Convergence-gate note
All items are single-seat (deck44) and DIAGNOSTIC/WATCH, not core or guide edits. The core prompt and the
deck44 guide are both FROZEN. The one item that CROSSES the adopt-a-revision bar (a genuinely new
decision-level failure class — self-destroy via own removal) sits at the reply-protocol reconcile arm and
the forced-sub-menu representation, NOT in guide prose, so it routes to the engine/reply-protocol ledger,
not to a guide line. A DEFENSIVE guide-line candidate is offered in general-suggestions.md as a WATCH only.
