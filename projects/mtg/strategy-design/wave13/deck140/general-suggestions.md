# deck140 wave-13b — general (cross-deck / synthesis) suggestions

## 1 [REPLY-PROTOCOL A/B — ledger #3, RE-CONFIRMED at higher impact] The name-echo protocol left the "plan's named action is not in the options" branch unhandled; that branch dumped a Rakdos kill shot this wave
Wave-11 specified the reconcile-named-action arm and named the absent-named-action branch as the promotable
part. Wave-13 shipped name-echo (100% "(name)" compliance, index-wins-on-conflict) and the branch is STILL
open: name-echo can only reconcile when the echoed name MATCHES a listed option, but the failure mode is
precisely when the model fixates on a card that is NOT offered. deck140 vs44 s40-42 is the sharpest witness
yet — the model, having already played its land, kept echoing `CHOICE: 1 (Play Badlands)` against option
lists whose index 1 was a *Cast*, and index-wins:
- s40 -> Cast Rakdos's Return (plan said hold it, non-lethal);
- s41 X-menu -> X=8 (the held kill shot fires for 8);
- s42 -> Cast Wrath of God into an empty board (C=0).
Plus the under-cast direction recurs at vs109 s9 (declined an OFFERED payable Cruel Edict at a mandated
life-16 sweep window because the plan wanted an unaffordable Damnation). **Proposed arm (unchanged, now
promotable with a 2nd mechanism-independent + higher-impact witness):** parse the plan's NAMED action; if
it maps to a listed option and the index disagrees, prefer the named option; **if it maps to NO listed
option, do NOT accept the raw index and do NOT map to an arbitrary cast — route to the intent-consistent
best-available option (cheapest payable removal if the plan wanted to sweep, else Cast-nothing/pass if it
wanted to hold or deploy an unaffordable card), i.e. fall through to the engine heuristic as the
out-of-range guard already does.** Not a guide edit and not a core prompt-body edit.

## 2 [ENGINE ledger — HIGH] Rakdos's Return X is sometimes auto-resolved to a minimal value with no X-menu surfaced; and the "Mana available" line hides dual-producible colors
Two engine items with corpus impact this wave, both worth prioritizing:
- **X auto-underpayment (vs109 s43):** an INTENDED lethal Rakdos (X=9 affordable, opp at 9) fired for X=3
  with 6 mana left in pool and NO X-menu decision logged — contrast vs44 s41 where the X-menu appeared as
  its own prompt. Whatever code path skipped/defaulted the X selection converted a T30 game-winning lethal
  into a T32 adjudication. Verify the Rakdos/BSZ X-choice prompt fires on EVERY X-cast and that auto-tap
  maximizes X for a lethal-range spell.
- **Mana-annotation color-hiding (brief #5, CONFIRMED):** the `Mana available: {..}` line renders one
  defaulted tap-plan (duals shown as a single color) rather than the achievable color space. At vs109 s9
  the line read `{r}{w}{w}{w}{w}` (no black) while the SAME prompt OFFERED a payable Cruel Edict {1}{b} —
  the model trusted the line, declined, and missed a mandated sweep (black was in fact available, proven by
  s11 casting Cruel Edict next window). The line and the offered-legal set disagree; the model reasons from
  the line. Fix direction: annotate the option-line affordability from the SAME engine that generated the
  legal-move set (or show per-source color flexibility), so "offered" and "Mana available" cannot
  contradict. Corroborates the deck44 reviewer's auto-tap color-stranding item (candidate 2nd site: deck140
  vs133 s11, where Pyroclasm {1}{r} appears wrongly excluded from the legal set under the same defaulting).

## 3 [REPRESENTATION ledger — WATCH, flag for 2nd witness] Forced-discard menus render like cast menus
deck140 vs133 s12: Liliana of the Veil's +1 forced a discard; the selection prompt listed deck140's hand
sweepers as `Card [sorcery] [your hand] - "text"` with no "you are being forced to DISCARD" framing, so the
model read it as a stack response and emitted CHOICE: 0 (Pass) -> unparsed -> heuristic discarded. Low
impact (forced either way) but the label ambiguity is a clean representation micro-lever. Do not act on one
seat — flag for a 2nd witness across the corpus.
