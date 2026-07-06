# deck135 work — additional findings (catch-all)
(Persisted by the orchestrator from the agent's returned text — its Write hit a report-file guard.)

Observations from the deck135 self-play logs outside the strategy guide, skill, and general-prompt suggestions. Grounded in `~/.Wagic/ai/gpt/selfplay-runs/pool7-20260705-232254/*deck135*.jsonl`.

## 1. The fetchland "crack" action shows only COST, never the BENEFIT (highest-value fix)
The action line qwen sees for cracking a fetch reads verbatim:
`search basic land with Prismatic Vista targeting Snow-Covered Forest [cost: Life, Tap, Sacrifice] (heuristic score 5)`
The only emphasized info is the COST. The benefit — you put a real land onto the battlefield (mana/fixing/ramp) — is only implied by "targeting." Against the general prompt's "weigh cost against gain" + "a valuable permanent is rarely worth trading," a weak model sees a pure-downside action and passes it. Mechanism behind fetchlands-never-cracked (16 crack-actions offered and passed in one game). FIX: make the gain explicit and symmetric with the cost, e.g. `... [pay: 1 life, tap, sacrifice this] -> puts a Snow-Covered Forest onto the battlefield (mana/fixing)`. Helps every fetch/ramp/sacrifice-for-value action across all decks.

## 2. Playing a land and cracking it are two disconnected decisions
Playing the fetch is an `ask` ("Play Flooded Strand"); cracking it is a separate later `priority` decision, nothing links them or signals the land makes no mana until cracked. One intent split across two decisions the model doesn't connect (same class as the attackers+blockers bundling). OPTIONS: auto-offer/auto-resolve the crack immediately after a fetch is played, or bundle "play and crack" into one action.

## 3. Duplicate, indistinguishable action lines clutter the list
The legal-action list repeatedly contains multiple identical entries (four copies of the same fetch->same land). qwen picks among indistinguishable numbers — wasted attention, inflated prompt. FIX: de-duplicate identical actions or collapse ("targeting Snow-Covered Forest (x3 available)").

## 4. `(heuristic score 5)` leaks the fallback AI's internal score into the LLM prompt
Every action carries a `(heuristic score N)` suffix; uniformly `5` in everything seen. Uniform = pure noise (tokens, no signal); non-uniform would silently anchor the LLM to the weak heuristic. FIX: remove from the prompt, or surface only when discriminating and intended.

## 5. Inconsistent verb for the same conceptual action
Cracking Prismatic Vista = `search basic land with ...`; cracking Flooded Strand = `Put in Play with ...`. Same concept, two verbs. FIX: normalize wording across fetch-type cards.

## 6. Phase/turn labeling is internally contradictory
One prompt's events block read `Phase: Upkeep (your turn)` while the header below read `Phase: Upkeep | It is the opponent's turn.` — disagree on whose turn. Also some phases render `Phase: --- (opponent's turn)` (blank label). Whose-turn/what-phase are load-bearing for legality. FIX: audit turn attribution in the events serializer vs the header; name or omit `---` phases.

## 7. Possible wasted inference on forced-pass decisions (hedged)
Translog is dominated by `priority / choice 0 (PASS)` at phases with nothing to do (Cleanup, Draw, Combat damage, empty Blockers). If only "pass" is legal there, calling the model to say "0" is wasted latency/log volume (games are inference-bound). Architecture claims no model call when one outcome — these records suggest the model IS polled to pass in some no-real-choice spots. FIX (audit first): confirm >1 legal action; if only pass, skip the call.

## 8. Positive confirmations (no change needed)
- Attacker/blocker bundling parsed cleanly (`A1`, `B1:A1`).
- Mulligan seam sensible (kept reasonable 7s every game).
- Force of Negation fired at genuine noncreature targets (Cruel Edict, Searing Spear), only with a real spell on the stack.

Items 1–3 are the most actionable and would raise play quality for many decks at the engine/representation layer, independent of any per-deck guide.
