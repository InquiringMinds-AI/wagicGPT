# Findings from deck133 — beyond the guide/skill/general-suggestions
(Persisted by the orchestrator from the agent's returned text — its Write hit a report-file guard.)

Concrete observations from deck133's 6 self-play translogs (`pool7-20260705-232254`). Ordered by likely usefulness.

## 1. (Most useful) Target/card option lines show a NAME but not what the card DOES
For discard and removal, the legal-action list gives only the card's name (plus P/T for creatures), e.g.:
```
1. Ice-Fang Coatl (1/1) [opponent's hand]
2. Arcum's Astrolabe [opponent's hand]
4. Search for Azcanta [opponent's hand]
5. Boreal Druid (1/1) [opponent's hand]
```
To rank these by impact the model must ALREADY KNOW Astrolabe is a cantrip rock, Search for Azcanta a card-advantage engine, Boreal Druid a mana dork — recall each function from the `{OPPONENT_DECK}` blob. Weak qwen can't: in game `1783311794` it stripped Astrolabe then Boreal Druid — the two LEAST impactful cards — over Search for Azcanta and the creature. Picks look near-arbitrary = "no basis to evaluate." Representation ROOT of the systemic "doesn't value by impact" failure. FIX in PROMPT ASSEMBLY: append a short descriptor to each target option — the card's one-line `text=`/`auto=` effect, or a coarse role tag (threat / removal / counter / ramp / draw / land). Helps every deck's discard and removal decisions.

## 2. Devotion / actual drain is invisible at the Gray Merchant cast option
Option is literally `Cast Gray Merchant of Asphodel {3}{B}{B} (2/4)` — nothing tells the model how much it drains now. In game `1783314186` it cast Gary with five Swamps and no other black permanent = devotion 2, a 2-life drain, throwing away the payoff. FIX: for cards whose effect is computed from board/game state (devotion drains, X-spells, "for each" effects, overrun pumps), surface the CURRENT magnitude on the option line — `Cast Gray Merchant of Asphodel (drains 2 now)`. Engine complement to the "cast it when it does the most" prompt rule.

## 3. Prompt hygiene: duplicate options and a leaked internal score
- Identical options not de-duplicated: cracking Verdant Catacombs offered NINE byte-identical lines (fetch one of nine identical Swamps). Collapse byte-identical option lines.
- The Baka heuristic score is exposed to the LLM (`(heuristic score 5)` / `(heuristic score 0)`) — leaks the fallback AI's internal number into the reasoning model's prompt (a potential anchor). It didn't slavishly follow (picked a score-0 Yawgmoth activation over score-5 fetches), but check whether the annotation is intentional in the GPT path.

## 4. Live board state omits static abilities the model must recall
Board line shows P/T and counters but not keyword/static abilities. Bloodghast shows `Bloodghast (2/1)` with no hint it CAN'T BLOCK or gains haste at opp<=10. Same recall burden as #1; lower priority (caused no observed blunder here).

## 5. Event log is phase-spam heavy
"Events since your last decision" often runs 20+ lines of bare phase transitions incl. many `Phase: --- (opponent's turn)` placeholders, for one meaningful event. Filter empty/`---` phase lines and collapse untap->draw runs.

## Note on what was NOT a misplay
Initially suspected Yawgmoth cast with no sacrifice fodder in game `1783314186`, but logs show it DID have fodder — its `-1/-1 [cost: Life, Sacrifice]` activations were legal and used (a reasonable value line). Flagging so the deck guide's "don't cast Yawgmoth without fodder" reads as a general guard, not something observed failing.

## Cross-cutting takeaway
Findings #1 and #2 are the same shape: the weak model can only pick well when the option line makes the deciding fact VISIBLE. Card names without function, and payoffs without their current magnitude, force a recall/computation the weak executor can't do — so it guesses. Surfacing "what does this do / how big is it right now" on the action line is probably a bigger cross-deck play-quality lever than any single strategy-guide edit, because it fixes the input to every target/payoff decision at once.
