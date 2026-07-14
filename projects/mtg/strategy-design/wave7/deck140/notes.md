# deck140 wave-7 — development notes (engine / harness / model / deck-construction)

## INFRA / HARNESS
1. **`-j` latency saturation is corrupting control-mirror corpora.** deck140 games hit latency
   spikes to **111.9s** (vs131), ~47s (vs110/vs44), and vs135 produced **19 empty-reply fallbacks
   out of 34 decisions** at 32.5s median latency. A control deck makes 3-4x the decisions of an
   aggro deck, so it is disproportionately exposed. For any run whose DECISION QUALITY matters (all
   the wave-loop review runs), use `-j 3` or raise `WAGIC_GPT_TIMEOUT` well above the fallback
   floor — otherwise a control deck's "collapse" is partly manufactured by empty replies. Repro:
   the fallback-count and latency-median columns in scratchpad `digest.py` over this run.
2. **Control-mirror timeout adjudication is still open.** vs131 timed out with deck140 AHEAD 22-14
   vs a creatureless burn deck at the 2400s cap (123 records). A raw timeout reads as a deck140
   loss/failure but the deck was winning. The cap-by-decisions / stalemate-detector ask (carried
   from wave-6 notes) remains the right fix: adjudicate control mirrors by life/board at the cap,
   or cap by decision count instead of wall-clock. Until then, timeout rows for control decks need
   manual adjudication (done in findings.md).
3. **Malformed reply artifact.** vs133 t6 emitted `#1. #  #` (parsed as a broken/degenerate choice)
   — a generation/parse artifact, not a decision. One instance; note for the reply-protocol/parser
   watch, no action unless it recurs.

## ENGINE / REPRESENTATION
4. **Surface an opponent creature COUNT on the battlefield line** (representation ledger). deck140's
   entire wave-7 collapse is the pilot mis-reading how many creatures are on a cluttered opponent
   battlefield line (artifacts + [tapped] flags obscure the 1-3 real creatures), then holding its
   sweeper. A first-class "creatures: N" integer on the opponent-battlefield line — and/or tagging a
   tapped attacker as "(untaps and attacks next turn)" — would make the sweep trigger unmissable and
   would let the per-deck false-belief kills (strategy.txt) eventually be deleted. This is the wave-6
   "representation beats instruction" pattern applied to the sweep decision. Repro: vs110 t6/t8/t10
   PLANs assert "0 (untapped) creatures" while the board line shows Memnite/Steel Overseer/Etched
   Champion.

## MODEL (route to model-experiments residuals, not a guide line)
5. **Runtime board-parse under clutter.** Even with the strategy.txt count instruction, the pilot
   must correctly count (X/Y) tokens on a 6-8 permanent line each decision — a runtime-parse task
   the guide cannot pre-compute. If the surfaced count (#4) does not ship, this stays a
   model-capability litmus case (does the pilot count creatures correctly off a mixed board?).

## DECK-CONSTRUCTION (the user owns deck edits — flags only)
6. **Reach starvation vs aggro is the persistent structural loss** (vs109, vs133; also the vs131
   slow-clock timeout). deck140 executes its sweep-and-grind plan faithfully and still loses to
   aggro because it has: no early lifegain/blocker to survive the first 4-5 turns, and a very slow
   clock (Staff = 1/turn; Blightsteel = 12 mana; Rakdos = held for lethal). 14 sweepers is the
   right density — the deck needs TEETH, options in rough priority:
   - a cheaper or second recurring clock so it closes before the control-mirror cap;
   - a small amount of early defense/lifegain vs aggro (the deck already runs Lightmine Field x4 and
     Venser's Journal x2 — they arrive too late; a 1-2 mana speed bump would help);
   - a couple more UNTAPPED red sources (currently Plateau/Badlands/Akoum(tapped)/Clifftop = 14/26,
     but Akoum enters tapped) so Pyroclasm {1}{R} is the reliable turn-2/3 cheap sweeper it is meant
     to be — repeatedly the pilot had W/B mana and no red for Pyroclasm and passed the turn.
   Repro: vs109 bled 21->0 while sweeping 4x correctly; vs133 dead t11 in 9 decisions.

## STATUS SUMMARY for the synthesis / apply step
- Guide fix (strategy.txt) targets the ONE model-fixable lever: the hold-everything overshoot,
  re-anchored to a countable sweep trigger with the three false beliefs killed inline. Validate by
  re-running deck140 at `-j 3` and checking: held-sweeper-while-opp-has-creatures drops toward 0,
  and the fast losses (vs110/vs44) extend past t11-12.
- No core edit proposed (general-suggestions.md PASS; two mechanics-fact watches logged).
- Structural residual (reach) is deck construction, flagged above; do not spend guide length on it.
