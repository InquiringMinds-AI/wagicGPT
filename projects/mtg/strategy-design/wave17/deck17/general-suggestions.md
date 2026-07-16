# general-suggestions.md — deck17 (wave-17)

Cross-cutting observations. Non-binding; single-seat items cannot add/cut core lines.

1. **MULLIGAN heuristic — redundant/land-light hands vs unknown aggro (from the vs21 loss).**
   The one loss traces to keeping 3 Islands + 3x Scion of Oona + 1 Gatewarden: a hand whose only
   sub-3-mana play is a wall and which offers three copies of the same 1/1 lord (first Scion not
   castable until turn 3). The current mulligan rule ("keep 2-4 lands + at least one castable
   spell") technically greenlights it. Consider a future refinement (needs multi-seat corroboration
   before it touches the core): a hand whose only ≤2-mana play is a Defender, OR that is 3+ copies
   of one 3-mana creature, is a MARGINAL keep — ship it on the draw / into an unknown field. Do
   NOT act on this from one seat/one game; flag for the mulligan-heuristic tracker across decks.

2. **Long-ramble at ALREADY-LETHAL windows (vs135 s13) is a recurring token-waste pattern.**
   When the board is already lethal, the model sometimes over-recalculates damage and overflows
   into an unparsed reply instead of just casting/attacking. Corpus-wide (this is not deck17-
   specific): a representation cue in the prompt — e.g. surfacing "your board already deals ≥ opp
   life in the air" — could short-circuit the recomputation. Routes to representation, not guide.

3. **Cloud Sprite / flyer block-restriction re-derivation burns tokens (vs21 s10).** The model
   correctly knows the rule but re-proves it from card text every time a block decision involves a
   Cloud-Sprite-adjacent flyer, sometimes overflowing. A per-creature annotation of the effective
   block restriction (e.g. `[can block: flyers only]` vs `[can block: any]`) on the blockers menu
   would remove the derivation entirely. Representation item; benefits any flyer deck.
