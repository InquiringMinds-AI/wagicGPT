# deck122 -> strategy-writing skill proposals (wave 29)

Context: Step-0 guideless debut of the campaign's FIRST alt-win-condition deck (Felidar
Sovereign 40-life) plus a life-threshold-anthem payoff class (Serra/Divinity) and a
build-loyalty value walker (Sorin). Proposals below are for the synthesis agent to merge;
convergence with the 139/148 Step-0 seats is the merge signal.

## PROPOSAL 1 (NEW teach category) -- ALT-WIN-CONDITION decks: teach the win threshold as a
## FIRST-CLASS PLAN; the model will not infer it from option/board text alone.

deck122 has Felidar Sovereign: the engine faithfully implements "at your upkeep, if you have
40+ life, you win the game" (AAWinGame, code-confirmed). Across 6 games the model NEVER once
planned toward 40 or named the win line -- it reasoned about life purely as "stay alive."
The Felidar option text states the trigger, yet the model treats the number 40 as
unreachable/irrelevant.

Rung: when a deck's engine implements an alternate win condition (life total, poison,
mill-yourself, permanent count, etc.), the guide MUST elevate that win line to a named,
first-class plan with (a) the exact numeric threshold, (b) an explicit "this WINS, it is not
a bonus" framing, and (c) a bridge from the deck's normal game to the threshold ("once
stabilized, actively climb to X"). Do NOT assume the option text carries it -- it does not.
This is distinct from a normal payoff teach because the payoff IS the game ending.

## PROPOSAL 2 (extends wave-28 perception-vs-strategy rung, POSITIVE control) --
## conditional-static P/T boosts ARE rendered live; life/threshold play is a GUIDE teach.

The wave-28 rung says: classify a leak as perception vs strategy before routing. deck122
supplies a clean positive data point. Serra Ascendant renders `(6/6) (printed 1/1) [flying,
lifelink]` at 31 life and `(1/1)` below 30; Divinity renders `(8/8) (printed 4/4)` at 26.
The render surface FULLY reports the live effective P/T and conferred keywords for the
conditional-static-boost / life-threshold-anthem class. Therefore the vs93 threshold-race
loss (raced life below 30, turned its own 6/6 flyers into 1/1s) is a STRATEGY leak, not a
perception one -> route to GUIDE prose, not to a notes.md annotation. Add to the skill's
"perception-surfaces-that-ALREADY-work" list (alongside sick-tag, land-identity, magnitude,
printed-vs-current P/T): conditional-static P/T + conferred keywords render live with the
`(printed X/Y)` annotation. When a life/count threshold governs a payoff, the model CAN see
the current stats -- so teach the THRESHOLD DECISION in prose.

## PROPOSAL 3 (generalizable reframe) -- LIFE-AS-ENGINE (resource-reframe teach).

For lifegain-matters decks (and any deck where a resource the model defaults to treating as
a cushion is actually the win engine): lead the guide with an explicit reframe -- "your life
total is not a cushion to spend, it is the ENGINE that powers your best cards; gaining life
IS progress toward winning." The observed failure is the model spending the very resource
that fuels its threshold payoffs. A one-line identity reframe at the top plus per-card
threshold callouts (stay above 30 / above 25 / climb to 40) is the fix. Generalize the
pattern: name the resource, name that spending it is a mistake here, name the thresholds.

## PROPOSAL 4 (extends 146-Lolth loyalty partial) -- build-loyalty VALUE walkers.

Sorin, Lord of Innistrad renders all three loyalty abilities with full text, `[cost:
Counters]`, and targets, and the model piloted it WELL: +1 (make a 1/1 lifelink Vampire)
every turn, correctly declined a -6 aimed at a worthless 0/3. Confirms the loyalty-render
work generalizes past Lolth. Guide teach that worked implicitly and should be codified: for
a build-loyalty value walker, name +1 as the DEFAULT every-turn play (ticks up + adds
value) and gate the minus abilities on target/board quality ("do not -X for a small target;
keep +1ing until the minus is a blowout"). This matches how the model already behaves and
reinforces it.
