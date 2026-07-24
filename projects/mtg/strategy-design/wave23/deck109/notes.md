# Dev notes — deck109 wave-23 (engine/harness ledger with repros) + ROTATION VERDICT

## Record: 4/6 (context only) — beat 131/137/102/140, lost 27/133. Both losses = evasive (merfolk)
/ drain (Gray Merchant devotion) matchup ceilings. 2 fallbacks at seat (1 empty, 1 unparsed).

## Corpus / provenance
- Run `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-013710` (21 games round-robin), binary
  `/tmp/wagic-0e6361732` (wave-23 batch, live-unvalidated -> THIS corpus validates it).
- deck109 acting-seat files (opp): 1784877433 (27), 1784878326 (133), 1784880409 (131),
  1784881679 (137), 1784883026 (102), 1784887078 (140).

## LEDGER ITEM A2 (engine / harness — attacker-salvage precedence) — CLOSED this wave by fix (d)
Wave-22 A2: a clean answer-first `ATTACK: A1,A2,A3,A4` (vs62 s21) under-recorded to a wrong 2-subset
when the reply spiraled to max_tokens and a combat prose-intent salvage arm overrode the coded line.
Proposed fix: the answer-first coded ATTACK line must be authoritative; salvage is a fallback only.
- **Wave-23 delta (d) shipped exactly this**: chooseAttackers now takes the FIRST parseable ATTACK:
  line (a combat-math "Attack: Deal 1.." prose line can no longer hijack via last-wins).
- **Validated at seat (see findings for the full 26-window table): 26/26 attacker windows, first
  parseable coded line honored, recorded set == first-line's in-eligible tokens, ZERO eligible
  attacker dropped. No prose-salvage override reproduced. Ledger A2 = CLOSED.**
- Method note: 7 windows over-declared (over-range indices or named summoning-sick creatures); all
  extra tokens were INELIGIBLE and correctly ignored (the wave-22 A1 outcome-neutral pattern, now
  the steady state). 3 spiral windows with multiple/long ATTACK replies (vs133 s23 x3-lines @103s;
  vs102 s19 @56s; vs137 s7 @157s) all took the first line correctly — the specific case the fix
  targets.
- Repro (if the synthesis agent wants a fixture): vs133 s23 in the deck133 seat file is a clean
  3x-repeated `ATTACK: A1, A2` spiral that records both eligible attackers via first-wins.

## NOT an engine item: the vs137 s12 Pillar mis-costing is MODEL-SIDE -> GUIDE, not a ledger entry
The engine renders `Cast Pillar of Flame {r}` correctly on the option line. The model IGNORED it and
declined an affordable Pillar believing it costs {1}{R} (Searing Spear's cost). This is not an engine
representation bug (the display is right) — it is a wrong prior belief overriding a correct render.
Routes to strategy.txt (lean cost line restored) and skill.md (crutch-lifecycle rung refinement), NOT
here. Recording only to pre-empt a mis-route: do NOT open an engine ticket for Pillar cost rendering.

## Fallbacks classified (2)
- **vs140 s19 empty_reply**: latency 2501ms uniform fast-fail = the transient endpoint/HTTP outage
  cluster the brief names (deck109's later-cluster member). Opponent's-turn forced own-creature pick;
  heuristic answered. Board impact NIL (my 20 / opp 4 / opp board empty, won at T8). No pilot
  correlation inferable.
- **vs27 s13 unparsed_reply**: 2-option Unleash sub-prompt; reply's first+last coded line both
  `CHOICE: 3` (out of range, only 2 options) with label "Cast Gore-House Chainwalker" matching no
  option — the model answered against the imagined PARENT casting-menu after a 7453-ch mana-spiral.
  INDEX-WINS correctly refused (out-of-range index + out-of-context label). Parser behaved correctly;
  fault is model sub-prompt/parent-menu conflation. Heuristic took the 1/1 counter = the guide's
  choice; board impact benign. This is a NEW unparsed shape but NOT an INDEX-WINS-echo regression —
  it is a genuinely unrecoverable garbage answer (no in-range signal existed to recover).

## Transform (b) / echo (c) / forced-loss (a): N/A at this seat
Mono-red, 20 Mountains: no DFC/modal cards (b N/A), no reveal windows, no forced self-discard/
sacrifice of own cards (a N/A — the vs140 forced own-creature target picks are edict-style, handled).
Echo (c) INDEX-WINS: the only relevant event is vs27 s13 above, where INDEX-WINS correctly did NOT
trust an out-of-range index — working as designed on the refusal side.

## ROTATION VERDICT — DO NOT ROTATE OUT YET (answering the predecessor's rotation test directly)
My wave-22 verdict recommended rotating deck109 OUT **after the trimmed guide got one confirming
corpus** (to verify the retired crutches were truly redundant, not load-bearing under a different
draw). **This is that corpus, and the answer is: the confirmation FAILED for one crutch.** The Pillar
cost-correction retirement REGRESSED (vs137 s12 mis-costed an affordable Pillar, a real decision
error) — the exact "load-bearing under a different draw" outcome the confirming corpus was meant to
rule out. The hybrid-mana retirement held cleanly, but a guide REVISION is warranted this wave
(restore the lean Pillar cost line).

Under the rotation test (no-guide-mod-of-substance AND no-new-work-signal = candidate), deck109 does
NOT qualify this cycle: there IS a guide mod (Pillar cost restore) AND a new-work signal at the guide
layer (a stochastic cost-conflation regression). **Recommend: KEEP deck109 in the pool one more
cycle** as a NON-canary (active revised seat), to confirm the restored cost line closes the vs137-s12
mis-costing shape. If the next corpus shows 0 Pillar mis-costings WITH the cost-conflation shape
present (Pillar offered at exactly 1 red mana alongside a Searing Spear in hand), THEN rotate out as a
converged canary. The engine side (Ledger A2 / fix d) is CLOSED and needs no further deck109 attention.
