# deck146 -> core-prompt / render proposals (wave 46)

Layer routing: engine DEFECTS are in the seat file's ENGINE / RENDER ITEMS list (E-1..E-5) and
are not repeated here. This file holds proposals about the CORE PROMPT — the reply contract, the
option-row conventions, and what the shared narration says — that my deck's records support.

---

## G1 (HIGH) — the reply contract's exemplar teaches a shape that several menus do not have

**Evidence.** The instruction line every ask ends with is:
```
On the FIRST line write CHOICE: followed by the number of your choice and its SHORT NAME in
parentheses (the name only - copy nothing from the {...} annotations), e.g. "CHOICE: 3 (Cast
Example Card)" (a placeholder - copy a real number and short name from the list)
```
The exemplar is a **cast** menu. Four of this corpus's six fallbacks are on menus whose options
are not cards at all — Silverquill Command's mode pairs (`5. you draw and sacrifice creature`) and
combat damage-order lists (`1. Vampire (1/1) [lifelink]`, three identical entries). In every one
the model wrote a legal number and put a CARD NAME in the parentheses, and the reply was
discarded as `stale_echo`. On my seat: 146 vs152 seq 13 and 146 vs130 seq 23, the second of which
was the deciding play of a lost game.

**Proposal.** Make the exemplar match the menu it is attached to. A mode menu should show
`CHOICE: 2 (creature gains 3/3 and you draw)`; a damage-order menu should show
`CHOICE: 1 (Vampire)`. The current single exemplar is doing active harm on exactly the menus
where the option labels are least card-like — it names the one thing the model must NOT copy.

## G2 (HIGH) — a modal card's sub-ask should carry the parent's identity, the way the parent now does

**Evidence.** Wave 45's fix gave modal roots `Choose an option for <card>:` and it worked —
63 firings this corpus (Teferi 10, the Pathways 31, Starstorm 6, Lay Waste 4, Intrepid Adversary
3, Perimeter Captain, Pelakka Predation, Emeria's Call) and the wave-45 residual (Teferi's side
chooser) is at zero. The whole remaining residual is **4 records, all deck146, all one emitter**:
Silverquill Command's mode-PAIR chooser, still headed `A choice is required - choose an option:`
with six unnamed pairs and no statement of what any of them does. It is the same class the
earlier fix closed, one level down.

**Proposal.** `Choose two modes for Silverquill Command:` (or the general form for any
choose-N-of-M spell), and give each pair the effect wording the root menu already knows how to
print. A guide can teach a card's modes; it cannot teach the executor to recognise a menu that
does not say which card it belongs to.

## G3 (MED) — activated-ability rows should say when this is the last offer of the turn

**Evidence.** deck146's manland activation was offered 31x in Upkeep, 11x in Blockers, 4x in Main
phase 1 and 0x in Main phase 2, while the seat had 30 main-phase priority windows. Every good
guide tells the model to spend a mana sink in its main phase; that advice was actionable in 4 of
46 offers, and obeying it at `priority` seq 50/51 (opponent at 1 life) declined the win.

**Proposal.** Either surface the ability at a main-phase priority window when one will occur this
turn, or annotate the row with what the model cannot otherwise know — `{this is the only priority
window you will get this turn}` / `{you will be offered this again in your main phase}`. The
option row already carries state annotations of exactly this shape (`{right now: returns NOTHING
- your graveyard has no creature cards}`, `[repeat: activated this turn 2 times already]`), so
the convention exists.

## G4 (LOW) — the PLAN field is the 2-option latency tail

**Evidence.** The corpus's two slowest 2-option asks are land-drop binaries — 453 s on
`['Play Underground Sea', 'Hold Underground Sea - do not play it now']` and 367 s on the
Brightclimb Pathway equivalent (146 vs130 seq 27, and `answer_replaced` fired on it) — and both
replies are full-turn strategic monologues in the PLAN field over a decision with one right
answer. p99 across all 755 two-option asks is 117.8 s, so the bar is met, but the tail is one
identifiable menu shape.

**Proposal.** On a land-drop ask, suppress the PLAN request (the reply rules already make PLAN
conditional — "only if the reply rules call for one"; a land drop cannot falsify a plan). This is
a token/latency proposal only; it removes no information from the model.
