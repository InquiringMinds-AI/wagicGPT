# deck140 wave-22 — development notes (engine/harness; layer-routed, with repros) + ROTATION VERDICT

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260723-173843`, 6 deck140 seat files
`*ai_baka_deck140-0x*-vs-*.jsonl`. Binary `/tmp/wagic-1d8ed8194`. `choice` 1-based; `-1` = heuristic
fallback; `CHOICE: 0` = pass. Opp creature count = `Opponent battlefield (creatures: N)`.

## HARNESS (parser/echo; route to harness batch, never guide prose) — I OWN the stale_echo cluster

1. **[HIGH — echo family NOT closed; a THIRD member] ANNOUNCE_X spell-name-prefix echo.** On the
   Black Sun's Zenith X-announce menu (a child menu spawned from an already-consumed parent cast), the
   model answers `CHOICE: N (Cast Black Sun's Zenith with X=N)` instead of the bare option label
   `X = N`. The echo detector matches "Black Sun's Zenith" against the CONSUMED parent cast option and
   fires stale_echo, discarding a numerically valid in-range `CHOICE: 1` -> heuristic. Neither shipped
   (c) fix covers it: the "<spell> targeting X" source-prefix strip only knows the token `targeting`
   (target menus), so `Cast <spell> with` survives; and even stripped, `X=N` (no spaces, model) !=
   `X = N` (spaces, option) fails the name match. consumePlan label-filtering only filters CoT labels
   (`Attack:`), not a full CHOICE line. **4/4 BSZ X-menus this corpus.** HARM = 0 (heuristic's biggest-X
   argmax matched the model's intent every time), but HARNESS-OWNED per the wave-21 headline-1 ruling
   (a fallback against a reply CONTAINING a correct, in-range, uncontested first-line coded answer is a
   harness FP). Latent-consequential the day the wanted X is NOT the heuristic argmax.
   FIX: on child menus whose option labels are bare parameter values (`X = N`), generalize the
   source-prefix strip to also peel `Cast <pending-source> with ` / `<pending-source> with `, and
   whitespace-normalize around `=`; then trust the in-range numeric `CHOICE: N`.
   REPROS: vs62 s27 (`X = 3/2/1/0`, `CHOICE: 1 (Cast Black Sun's Zenith with X=3)`), vs27 s26
   (`X = 2/1/0`, `...with X=2`), vs102 s12 (`X = 1/0`, `...with X=1` — SHORT clean reply, cleanest proof).

2. **[MED — fourth-shape residual] absent-card CHOICE pre-empts last-well-formed-wins.** vs102 s9
   (priority, only option = activate Elixir). Reply emits `CHOICE: 0 (pass)` ... a mid-body
   `CHOICE: 1 (Cast Black Sun's Zenith...)` naming a card ABSENT from the option set (sorcery in
   upkeep) ... then a final `CHOICE: 0 (pass)`. The correct answer (pass) bookends a hallucinated
   middle CHOICE, but stale_echo fired on the whole reply instead of last-well-formed-wins taking the
   final `CHOICE: 0`. HARM = 0 (heuristic passed, correct). FIX: when a coded CHOICE names a card
   absent from the current option set, DROP that line and keep parsing for a well-formed in-range
   CHOICE, rather than firing stale_echo on the reply. REPRO: vs102 s9.

## ENGINE (C++/representation; route to engine batch, never guide prose)

3. **[HIGH — LOSS-CAUSING] forced-self-discard rendered with the generic detrimental-inverting target
   template.** The Archon-of-Cruelty forced-discard seam renders as `TARGET CHOICE for discard a card
   ... Pick the ONE target it will affect ... answer with the chosen TARGET's name`. That template is
   calibrated for choosing what a spell AFFECTS (an opponent's permanent to remove -> pick the MOST
   valuable), and inverts catastrophically for a self-discard where the "target" is your own card you
   LOSE (-> pick the LEAST valuable). deck140 discarded Damnation (vs102 s13) then Pyroclasm (vs102
   s14, over a land its own PLAN had named) — its two live sweepers — into a 6/6 Archon + Zombie-army
   board and died T12 (23->0 in ~2 turns). 8 such prompts in the vs102 game; only game in the pool with
   the mechanic. FIX: a detrimental-self-choice framing header (see general-suggestions.md) —
   "you are choosing which of your own things to GIVE UP; pick the least useful." REPROS: vs102 s13,
   s14 (both `TARGET CHOICE for discard a card`, options are the seat's own hand).

4. **[MED — latency + echo twofer] X-announce follow-up mana display is self-contradictory.** On the
   BSZ X-menu the line reads `Mana available: (no untapped sources) | Already in pool: {r}{b}{b}{w}{w}`
   — the parent cast already tapped the lands and floated the mana, so "no untapped sources" reads as
   "no mana" and contradicts the full pool. The model spent 40+ lines and 58-199 s re-deriving the
   mana from scratch (vs62 s27, vs27 s26), which is exactly when it re-anchors to "Cast <spell> with
   X=N" (the parent action) and trips echo item #1. Fixing the display friction cuts BOTH the worst
   latency spikes AND the echo. FIX: on a child/announce menu, either suppress the "no untapped
   sources" phrase when a pool is floated, or state the floated pool as the available mana ("Mana in
   pool: {..} — already committed to this cast"). REPRO: vs62 s27 / vs27 s26 prompt tails.

## KNOWN / CONFIRMED, no new action (account for, do not re-diagnose)
5. **"Mana available" hides dual second colors** (the corpus-wide engine item from prior waves): still
   present; deck140 cast red spells (Pyroclasm) many times despite it, and reasons off the offered
   list — 0 impact this corpus again. Unchanged priority. REPRO: any Pyroclasm cast window.
6. **Adjudication regime.** 4 of 6 games life-adjudicated at cap (vs135 +1, vs27 +11, vs44 +6 = WON;
   vs62 -18 = LOST); vs109 (T20) + vs102 (T12) were REAL board deaths. Cap outcomes are LEGITIMATE and
   robbed deck140 of no win. deck140 is the corpus grinder (highest decision counts) with high latency
   (p90 up to 178 s) — that pushes its games to the wall, but latency caused NO loss (deaths preceded
   any cap; the adjudicated loss was correctly behind). Worst spikes trace to item #4.

## DECK-CONSTRUCTION (user owns; flag only)
7. **No early answer to hasty aggro / go-wide reloads = the loss column.** vs109 (Mono-R Aggro): at 9
   life by T5, sorcery-speed sweeps + Pyroclasm's toughness-2 cap can't race haste + burn; died T20.
   vs102 (Tergrid): go-wide Zombie reloads outrun one-sweeper-per-turn, and deck140 has ZERO blockers,
   so every unblocked hit lands while it clears one board. Same threat-mismatch as the wave-15 vs135
   snow-trampler loss. Priorities unchanged: (a) a cheap early-interaction / fog-style survival tool,
   (b) a faster stabilizer that survives to the Staff-of-Nin grind the deck WINS with (all 3 wins
   deployed a clock; all 3 losses never did). 14 sweepers is right; the deck needs to LIVE to turn 6-8.

## PER-DECK ROTATION VERDICT (brief-mandated)
**ROTATE OUT AGAIN — but the rotation-in EARNED one real guide addition.** The re-validation confirmed
the guide's core (SWEEP TRIGGER, sweeper-selection, #1-Rakdos-hold, shroud, finisher order, Staff
face-ping) is OBEYED and load-bearing under the modern engine — no guide-refinement work signal there.
The one NEW guide rule (forced discard/sacrifice) came from the CHANGED POOL (Tergrid), is now written
into strategy.txt as the sanctioned interim crutch, and its durable fix is routed to the engine/core
ledger (items #3). Two belief-kill crutches were DEMOTED (tapped/artifact — the `(creatures: N)` count
carries them), shroud KEPT. After this diff deploys and validates, the seat is a clean rotation
candidate again: the remaining residual is HARNESS (echo items #1/#2, my owned cluster) + ENGINE
(#3/#4) + DECK-CONSTRUCTION (#7), none of it guide-refinement. CAVEAT (unchanged from wave 15): the
guide is demonstrably LOAD-BEARING (the model cites SWEEP TRIGGER / #1-Rakdos-hold / finisher order
verbatim and wins 3/6 with all 3 losses structural) — rotate means "stop re-reviewing a proven seat,"
NOT "remove the guide." Recommend the synthesis agent DEPLOY the wave-22 strategy.txt (it fixes a
real, loss-causing, previously-uncovered seam) before the seat rotates back out.
