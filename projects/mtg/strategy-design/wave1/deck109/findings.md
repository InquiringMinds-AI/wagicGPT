# Additional findings from deck109 work
(Persisted by the orchestrator from the agent's returned text — its Write hit a report-file guard.)

Observations from the six deck109 self-play games outside the strategy guide, skill, and general-prompt suggestions. Cited to game id + record. Pool: `~/.Wagic/ai/gpt/selfplay-runs/pool7-20260705-232254/`.

## Representation issues (highest value — cheap, help every deck)

**1. A target prompt rendered with a BLANK effect name (looks like an outright BUG).** Game `1783314190`, rec 17: prompt reads `Choose the target for ` with no spell/ability name, then lists only qwen's OWN creatures as targets (Ash Zealot, two Goblins). The model is asked to target something but isn't told what is targeting. Looks like a triggered/required target whose source name didn't substitute into the `Choose the target for {name}` template. Trace which seam emits it and why `{name}` came through empty. A nameless choice is a blind choice.

**2. Unleash labels don't convey the tradeoff; qwen's answers are inconsistent.** Prompt: `Choose an option for Rakdos Cackler: 1.  1/1 Counter  2. Decline - do nothing`. "1/1 Counter" never says the creature enters BIGGER but UNABLE TO BLOCK. qwen took the counter in `1783314184` (rec 4,21) and `1783315840` (rec 9) but DECLINED it in `1783311791` (rec 4), leaving a 1/1 — a leak for aggro. FIX: relabel to the consequence, `1. Enter with a +1/+1 counter (bigger, but can't block)` vs `2. Enter as printed (can block)`. General principle: modal option labels should state the game CONSEQUENCE, not the raw mechanic.

**3. "Yourself" offered as a burn target, prominently (option 2).** `1783314184` rec 31: `1. The opponent (life 1)  2. Yourself (player, life 18)  3. Legion Loyalist...`. Legal but almost never correct; second-position is a trap for a weak model. qwen didn't fall for it, but list the caster's own face LAST (keep legal). Nearly free.

**4. Face-FIRST ordering in damage-target lists is PROTECTIVE — keep and standardize.** Across every burn-target decision "The opponent (player, life N)" was option 1 and qwen reliably chose it to close (`1783311791` rec 14/16/19/21; `1783314184` rec 31/33; `1783314188` rec 23). Given the pilot's affinity for a correct first option, ensure ALL direct-damage spells list the opponent's face first. Intentional invariant. (Constructive flip of the positional-anchoring finding: put the usually-right option first, traps last.)

**5. Lethal math not surfaced at the attackers seam.** The attackers prompt shows `Opponent life: N` and each attacker's power but sums nothing and doesn't remind of attack-triggered damage (Hellrider ping). A "total attacking power: X" line or echoing relevant attack triggers would make the lethal check executable without recall.

## qwen behavior patterns

**6. Combat aggression is genuinely GOOD; leaks are card-KNOWLEDGE, not reasoning.** qwen consistently attacked with everything, made maximal alpha strikes (`1783311791` rec 17/22; `1783314184` rec 29 = all 7 incl. Hellrider), burned face to close. No lazy low-index bias — it picked the maximal attacker set, not "A1". Residual mistakes were not knowing a card's role (burn-as-reach; Unleash tradeoff). Implication: for aggro, leverage is in the representation layer + guides, not in out-reasoning card-specific lines.

**7. No blockers decisions for deck109 at all** (aggro rarely got attacked into). These logs carry no evidence about defensive play for this archetype. Scoping note.

**8. Decisions are re-polled, inflating logs + inference cost.** Identical decisions recur back-to-back (mulligan twice `1783314184` rec 0-1; identical "Cast nothing/Cast Pillar" asks rec 7-8, 10-11). Consistent with async re-ask + one priority stop per phase step — expected, not a bug — but collapsing unchanged consecutive priority passes would cut log volume + inference. Not urgent.

## Non-issues checked (don't re-investigate)
- Opponent life above 20 renders fine (`1783314188`: "Opponent life: 21").
- Casting lists are pre-filtered to affordable spells — pilot never computes mana affordability. Good; leave it.
- Opponent tapped/untapped lands + hand size are shown, so "what can they do" is derivable — adequate.

**Most useful:** modal choices should label the game CONSEQUENCE not the raw mechanic ("1/1 Counter" → inconsistent Unleash play, repeats on every modal card across every deck); and the blank effect-name in `1783314190` rec 17 is the one thing that looks like an actual bug.
