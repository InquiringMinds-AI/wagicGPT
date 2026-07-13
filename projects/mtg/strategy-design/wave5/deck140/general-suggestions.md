# Deck-140 wave-5 — general-strategy / core-prompt proposals (layer-routed)

Baseline: `strategy-design/wave4/general-strategy.txt` (the wave-4 core revision) + the live
`bin/Res/ai/gpt/system_prompt.txt`. The burden of proof is ON core: every deck pays for each line.
From deck140's single seat I can PROPOSE but not unilaterally justify a core change — single-seat
evidence cannot cut or add a core line; these are for the synthesis agent to weigh against
cross-deck convergence.

## Short version
**No new CORE PROMPT doctrine proposed.** The core prompt performed well for this deck this corpus:
the numeric plan-binding line ("the option's position is not the number it stands for") plus the
engine's largest-first X reversal together produced 6/7 max-X picks and 0 desyncs. The two deck140
levers this wave are (1) an ENGINE/REPRESENTATION item (already shipped — the X reversal) with one
small follow-up, and (2) a PER-DECK finisher-discipline rule (stays in the guide). Nothing generalizes
to core from one seat.

## Routed items

### ENGINE / REPRESENTATION (not core prompt)
1. **X-menu reversal — SHIPPED, confirmed working.** The largest-first ordering + the ask-text line
   "option 1 is the LARGEST X" is the wave-4 fix, and it is the direct cause of deck140's 1/12 -> 2/6
   turnaround (vs44's X=12 kill). Keep it. No further prompt-body work needed for the value/index
   trap; it is an engine/representation win, not a doctrine win.
2. **Belt-and-suspenders on the X ask (small, engine-side):** a single menu-adjacency carryover was
   observed (vs110 seq74->seq75: a bare "2" carried from the prior cast menu onto the X menu -> X=5
   not 6, immaterial). If cheap, reset/relabel so the second back-to-back menu can't inherit the
   prior index. Low priority; see notes.md. This is engine-layer, NOT prompt text.

### PER-DECK (stays in the guide, do NOT promote)
3. **Finisher discipline (Rakdos's Return = hold-for-lethal).** This is role- and named-card-specific
   (a 2-copy scarce-reach finisher in a control shell). Promoting a "hold your burst for lethal" line
   to core would misplay every aggro/burn deck whose reach IS its clock and should go face on curve.
   The deck-agnostic FLOOR already in core is right ("once the opponent is at 8 or less always fire
   damage at their face"; "check for LETHAL twice a turn"); deck140's guide instantiates it as
   "X = lands - 2; if >= opp life, cast NOW" and adds the deck-specific HOLD because the finisher is
   scarce. The floor is core, the hold is per-deck. Correct split; nothing to route up.

### Note on a core line that is LOSING-at-distance for this deck (candidate for the ledger, not a rewrite)
4. Core's "your own turn is for ACTING ... deploy a threat every turn" is correct and obeyed, but for
   deck140 the deck literally has no castable threat on most turns (14 sweepers, ~2 finishers, {12}
   Blightsteel). The pilot handled this correctly (correct sweeper-only declines), so this is NOT a
   core defect — it is the deck-construction reach residual (notes.md). Flagging it only so the
   synthesis agent does not mistake deck140's high cast-nothing rate for a core-prompt passivity
   problem: it is not. No core edit.

## Epistemics
- Reuse-of-thresholds: deck140's finisher lethal check reuses the opponent-life quantity already in
  core (the 8-life burn-at-face threshold is the nearest core number; the finisher check keys on
  "opponent's life" directly, no new constant minted). Good — no second number introduced.
- Removal of the now-fixed X stopgap from deck140's guide is a per-guide edit (done in strategy.txt),
  not a core removal experiment. Other X-decks' reviewers should independently confirm the reversal on
  their corpora before their guides drop the X-teaching (cross-deck agreement is the removal signal).
