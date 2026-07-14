# deck131 — wave 9 review (UR Guttersnipe spellslinger; "Mind Control / Best Counter Deck Ever")

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-094942/`, one GPT-vs-GPT
round-robin, qwen35 both seats, **wave-8 guide in play** (`bin/Res/ai/baka/deck131_strategy.txt`:
strict two-step mulligan gate, collapsed LANDS block, board-wipe counter clause, vs135 YP-swarm
winning line, NEVER-block-Guttersnipe). Binary `/tmp/wagic-a40671057`. **6 deck131 games.**

**Record: 0 W / 5 L / 1 TIMEOUT (0/6).** Fifth straight bottom-tier wave (5:0/6, 6:1/6, 7:1/6,
8:1/6). Win column = context only (USER EVALUATION DOCTRINE); the unit is the DECISION.

Seat-file -> game map (from the `gamestart` record):

| result | vs (deck) | file (epoch) | recs | end life me/opp | one-line cause |
|---|---|---|---|---|---|
| LOSS    | 109 (mono-red Hellrider)        | 1784041212 | 32 | -6 / 20 | Hellrider/Stromkirk aggro outraced it; opp never touched (20) |
| TIMEOUT | 140 (lifegain "Wipe Them Out!") | 1784042388 | 88 | **29 / 25** | grind; deck131 slightly AHEAD at cutoff — deck140 grind class |
| LOSS    | 110 (Etched Affinity)           | 1784044085 | 36 | -2 / 9  | affinity beatdown; **closer (opp 9)** |
| LOSS    | 44  (Faerie Archmage)           | 1784046898 | 56 | 0 / 18  | slow faerie tempo; **kept a 0-Mountain hand** (see MULLIGAN) |
| LOSS    | 133 (Phyrexian Asphodel)        | 1784047264 | 58 | -1 / 5  | zombies; **closest loss (opp 5)** |
| LOSS    | 135 (Modern Snow)               | 1784047950 | 23 | -4 / 17 | snow tempo outraced it |

Opp life at death in the 5 losses: 20, 9, 18, 5, 17 — two now CLOSE (110 opp 9, 133 opp 5), a
softening of the multi-wave "opp near 20 at death = never ran a clock" signature, but the deck still
finished behind in every game. **OBEYED-BUT-LOSING, fifth wave.** The guide is a clean, obeyed
instrument; the losses are construction/matchup. Transport clean: **0 desyncs, 0 defers, 0
`fails validation`, 0 engine no-ops** across all 6 games + 6 stderr sets (waves 5-6-7-8-9 all clean).

**Bottom line: the wave-8 revision is VALIDATED on every axis it targeted. Zero decision-level guide
defect this wave. The one bad OUTCOME (vs44 keep) is a REPLY-PROTOCOL defect, not a guide defect —
route it there, do NOT touch the guide.** strategy.txt is a freeze copy of the live wave-8 guide with
one freeze-safe de-specification (the corpus-pinned "last win" self-citation → a general pattern).

---

## HEADLINE — the strict-ordered mulligan gate WORKED at the reasoning level; the false-KEEP-by-hijack class is DEAD. The residual bad outcome moved to the reply protocol.

The wave-8 verification ask (top of brief): *did the false-KEEP class die, both tally directions?*
Answer: **yes at the level the guide controls (reasoning), and the leak moved to a layer the guide
cannot control (the head-first reply protocol).**

**The vs44 case (same shape as wave-8's false-KEEP — 0 Mountain, no draw spell, a lone Young
Pyromancer as bait).** Opener: `Island; Island; Downsize; Island; Essence Scatter; Cyclonic Rift;
Young Pyromancer` (0 Mountain, no Artificer's Epiphany/Opportunity → a clear MULLIGAN). The pilot's
PLAN this wave reasoned CORRECTLY and explicitly REJECTED the payoff-hijack that flipped wave-8:

> "1
> PLAN: Keep this hand. It contains a Mountain? No, it has three Islands. Wait, the strategy guide
> says: 'If the word Mountain does NOT appear ... AND the hand has no Artificer's Epiphany and no
> Opportunity, MULLIGAN'. My hand has no Mountain, no Artificer's Epiphany, and no Opportunity.
> Therefore, I must Mulligan. **The hand has Young Pyromancer and counters, but without a red source
> or a way to dig for one ... I cannot cast my win condition. I will shuffle this hand back**"

This is exactly what the wave-8 strict-gate rewrite was for. In wave-8 the reasoning ITSELF concluded
keep (it grabbed "a hand with a Young Pyromancer is ALWAYS a keep," dropped the "Mountain and"). This
wave the reasoning correctly disqualifies on the red-source gate FIRST and un-overridably, and names
Young Pyromancer only to reject it. **The de-hijacking succeeded; the guardrail no longer inverts.**

**BUT `chosen_text` = "Keep this hand" (`choice`=1).** The reply's HEAD TOKEN is `1` (= Keep),
emitted on its own line BEFORE the PLAN. The PLAN then reasons to Mulligan, but the choice stayed
locked to the stale head number. This is **REPLY-PROTOCOL INTENT-COLLAPSE** — the head is committed
before the model reasons, and a model that reasons itself to the OTHER same-window option never
revises the head. It is the SAME mechanism as **deck135 wave-8 s48** (head `4`=Cast nothing, PLAN
concludes cast Icehide Golem). The bad OUTCOME (keep a dead hand) is identical to wave-8's false-KEEP,
but the CAUSE moved from the guide (reasoning hijack — now fixed) to the reply protocol (head-first
commit — a harness property, symmetric across both seats, not deck-specific).

- **Cost:** the kept 0-red hand → first Mountain arrived only mid-game; the pilot limped to a 0/18
  loss vs faerie tempo. Same failure the wave-8 fix aimed at, now via a different door.

**ROUTING (do NOT re-word the guide over this — the skill is explicit):**
- The guide's mulligan reasoning is now CORRECT. Adding guide words to "fix" an intent-collapse is
  the anti-pattern the skill forbids ("route to the reply-protocol layer, never re-word the guide over
  it, never score it against a guide rule"). **FREEZE the strict gate verbatim.**
- **This is the SECOND seat for reply-protocol intent-collapse (deck135 s48 + deck131 vs44 seq1).**
  It strengthens the case for the reply-protocol A/B the wave-8 synthesis scheduled (accept the LAST
  bare number / move CHOICE after the PLAN, judged by decision-quality not win-rate). Two seats, two
  waves, two decks → this is no longer a single-instance watch; it is a reproducible reply-protocol
  defect worth the A/B. Routed in full to notes.md + general-suggestions.md.

**Both-directions tally (the brief's explicit ask):**
- **no-Mountain openers = 1 (vs44).** Correct verdict = MULLIGAN. **Reasoning correct: 1/1.**
  **Outcome correct: 0/1** (intent-collapse committed the head). Wave-8 was 1/2 (false-KEEP by
  reasoning). The reasoning-level metric IMPROVED to 1/1 — the guide fix landed.
- **keepable Mountain-hands kept = 5/5** (vs109, vs140, vs110, vs133, vs135 — every Mountain-hand
  read its red correctly and kept; the wave-6/7 false-MULLIGAN did NOT recur, 0 this wave). The
  keep-side direction holds.

## Mountain-first — STEADY, no regression. FREEZE.

- **Multi-land co-offers (both "Play Mountain" and "Play Island" in one option list): 10** (vs109 1,
  vs140 2, vs110 1, vs133 2, vs135 4, vs44 0). The wave-7 land-drop enumeration fix continues to fire.
- **Mountain-first when a land was played: 9/9.** Every co-offer where the pilot played a land chose
  `Play Mountain` (incl. when Island was listed first — the pilot picks by NAME). The one non-Mountain
  pick (vs135 seq5 T3, chose "Play no land right now") was a CORRECT decline of a SECOND land drop in
  one turn — the pilot had already played its Mountain that turn and held to draw a blue source; not a
  Mountain-first violation. vs44 had 0 co-offers because it drew no Mountain until mid-game (the kept
  0-red hand — a mulligan/draw story, not the interface).
- Land fix has now fired-and-obeyed across TWO corpora (wave-8 12/12, wave-9 9/9). CLOSED. FREEZE the
  pick rule; the interim stopgap + stale menu explanation stay dropped (wave-8 removal condition met).

## Guttersnipe — cast-every-turn OBEYED; NEVER-block HELD; more board presence than wave-8.

- **Board presence UP: Guttersnipe genuinely on the battlefield in 2 games** (vs44 3 decisions,
  vs135 3 decisions), vs wave-8's 1/6. Precondition still rare but better sampled.
- **Cast-every-turn while Guttersnipe out: OBEYED. 0 fuel-declined leaks** across both games (every
  window with a castable instant/sorcery cast it; the only cast-nothings while a Guttersnipe string
  appeared in the prompt were false positives — Guttersnipe was in HAND/cast-list, not on board, or
  the only castable was Prism Ring/Elixir/Aetherize-with-no-target, all correct declines).
- **NEVER-block-with-Guttersnipe: HELD.** Guttersnipe ATTACKED in vs135 (seq21 T9, "Elemental,
  Guttersnipe") and NEVER blocked in any game. No reflexive high-life blocks: every blocker decision
  at 17-20 life answered "no blockers" (vs109 T3, vs135 T4/T8). The lone chump was vs133 seq56 T23
  (Young Pyromancer blocks Geralf's Messenger at 8/5, a lost late game) — marginal, not a guide
  defect, game already gone; noted as a low-confidence watch, not scored.
- Attack participation: 100% of the offered `A#.` list where racing (vs110 grew YP → YP+3 Elementals;
  vs135 grew to Elemental+Guttersnipe). No subset-attack leak.

## Sweeper-recognition counter clause (wave-8) — NO test window this corpus; the counter seam played WELL.

The wave-8 clause ("a board wipe deals 2+ to all creatures = removal aimed at Guttersnipe; counter
it") had **no precondition window this wave**: vs140 (the Pyroclasm deck) did NOT resolve a Pyroclasm
into a live Guttersnipe with a counter up. Instead the pilot used its counters on the RIGHT targets
and kept its engine alive to the timeout (AHEAD 29/25):
- vs140 seq28 T22: chose **Dissipate on Staff of Nin** (over Counterspell — kept the harder counter);
  seq33 T32 **Counterspell on Venser's Journal**; seq41 T44 **Essence Scatter on Blightsteel Colossus**.
- vs110 seq30 T9: **Counterspell on Cranial Plating**; vs44 seq11 **Essence Scatter on Scion of Oona**,
  seq24 **Counterspell on Archmage of Echoes**.
All are sound "beats-you-on-its-own / removal-for-your-payoff" counter picks. The counter seam is
healthy. The clause is FROZEN/untested (precondition rare) — neither validated nor refuted; keep it.

## `damage >= toughness = dead` watch — NO recurrence. Remains single-seat. Do NOT promote.

Grepped all 6 games for toughness-arithmetic reasoning ("survive/kill/destroy/dies" + "toughness/2
damage"): **zero hits.** The wave-8 vs140 seq51 instance (pilot asserted a 2/2 Guttersnipe survives
2 damage) did not recur — because its precondition (a sweeper-on-stack-with-counter-up over a live
Guttersnipe) did not occur. The false belief stays **single-seat (wave-8 only)**. Per single-seat-
can't-cut-core epistemics, carry as a core rules-fact WATCH; do NOT promote off one seat.

## FROZEN-LINE RECHECK (obeyed-but-losing discipline — verify obedience, do not churn)

- **Strict two-step mulligan gate** — reasoning correct 1/1 no-red + 5/5 keepable; false-KEEP-by-
  hijack DEAD. FREEZE verbatim (residual is reply-protocol, not the guide).
- **Guttersnipe cast-every-turn #1** — obeyed, 0 fuel-declined leaks (2 games testable). FREEZE.
- **Two-gear identity (race/dig)** — obeyed. FREEZE.
- **Deploy-payoffs / cast-creature-first / YP-swarm second win path** — obeyed. FREEZE.
- **Mountain-first** — 9/9. FREEZE (stopgap stays dropped).
- **Attack-every-creature / no-block-while-racing / NEVER-block-Guttersnipe** — obeyed, Guttersnipe
  attacked and never blocked, no high-life blocks. FREEZE.
- **Reactive / board-wipe counter clause / draw / Prism-Elixir-lifegain / lethal-check** — obeyed;
  counter seam played well. FREEZE (board-wipe clause untested this wave, kept).
- **Winning-line block** — self-cited "the last win" is now corpus-stale (no win this corpus; the
  cited token-swarm win is from the wave-8 corpus). De-specified to "this deck's fastest wins close"
  — a freeze-safe removal of the corpus-pinned self-citation per the skill's staleness rule. The win
  PATTERN it describes is real and unchanged; no pick changes.

## Routing summary
1. **[GUIDE — FREEZE, no decision-level change]** strategy.txt = the live wave-8 guide, one freeze-safe
   de-specification (stale "last win" → general pattern). Every targeted line validated; churning
   obeyed lines off a 0/6 corpus is exactly the anti-pattern the doctrine forbids.
2. **[REPLY-PROTOCOL — PROMOTE the A/B]** intent-collapse now has TWO seats (deck135 s48 wave-8 +
   deck131 vs44 seq1 wave-9), two decks, two waves. No longer a single-instance watch — a reproducible
   head-first-commit defect. Run the reply-protocol A/B (accept LAST bare number / move CHOICE after
   PLAN), judged by decision-quality on symmetric core. general-suggestions.md.
3. **[DECK construction — unchanged, dominant]** threat density 6/60 (3 Guttersnipe + 3 Young
   Pyromancer) and no reach vs lifegain own the loss column; two losses now closer (opp 9, opp 5)
   suggest a small threat-density bump would flip games. 14I/8M manabase leans slightly wrong; cheap
   11M/11I tweak. Cut a Prism Ring for reach (Spellheart Chimera #SB). general-suggestions.md.
4. **[CORE — WATCH only, single-seat]** `damage >= toughness = dead` (wave-8 vs140 only; no
   recurrence). Do NOT promote.
5. **[DECK/matchup — hard/unwinnable, n=6 noise]** vs109 mono-red aggro, vs110 affinity, vs133
   zombies, vs135 snow tempo, vs140 lifegain-grind (timeout, AHEAD). Route to construction/tiers.
