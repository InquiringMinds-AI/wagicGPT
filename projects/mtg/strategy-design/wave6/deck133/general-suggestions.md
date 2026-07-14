# Wave 6 — deck133 handoff to the core prompt / representation / engine

## CORE PROMPT — no new edits proposed. One present-but-losing-locally note.
- The core already carries the anti-premature-payoff line (wave-5 core, "cast a payoff whose
  value scales when it does the most, not the first turn you can afford it"). deck133's vs131
  premature Gray Merchant shows this core line **present but losing locally** — the pilot cast
  the payoff at low devotion anyway. Per the routing discipline, a core line present-but-bypassed
  routes to **per-deck reinforcement** (done: the guide's #1 impact entry now states the ELSE)
  and/or **representation**, NOT a second core paragraph. No core change.
- The wave-5 core Edit 2 (act-floor satisfied by the HIGHEST-impact play; threat before a
  same-turn value/dig/discard) is CONFIRMED working for this deck — vs109/vs110 deployed removal
  and creatures over the cheapest discard. Keep as-is.
- Single-seat: nothing here is deck-agnostic enough to cut or add a core line.

## REPRESENTATION / ENGINE (route upstream; see notes.md for repro)
1. **Gray Merchant drain display still under-counts by 2** (wave-5 E2, STILL LIVE, re-confirmed
   vs44 shown 10 → actual 12). The `{right now: drains N}` value omits Gray Merchant's own
   {B}{B}=2 devotion because he is still in hand when the option is rendered. Fix: compute the
   displayed drain as if he were already on the battlefield. Until then the guide teaches
   "real drain = shown N + 2" (stopgap, removal condition = this fix ships).
2. **Single-option priority re-ask multiplier** inflates decision count and wall clock
   (vs140: 81 priority records in a 105-record game; vs131: 20). A within-turn "declined this
   exact single-option offer" memo would cut grind/timeout games at zero decision-quality cost.
3. **results.tsv still lacks final life on `timeout` rows** (carried from wave-4/5): vs131 and
   vs140 both scored `timeout - - -`. A final-life snapshot (or a `gameend` on cap) would stop
   every reviewer re-opening the translog to learn whether a timeout was won/lost/drawn.

## STOPGAP LEDGER (guide lines that die when a platform fix ships)
- "real drain = shown N + 2" — patches the GM display under-count (E2 / repro notes.md);
  REMOVE when the displayed drain includes Gray Merchant's own devotion.
