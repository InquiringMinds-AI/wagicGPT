# deck131 — wave 10 review (UR Guttersnipe spellslinger; "Mind Control / Best Counter Deck Ever")

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-031825/`, one GPT-vs-GPT
round-robin (decks 44,109,110,131,133,135,140), qwen35 both seats, wave-9 (FROZEN) guide in play,
binary `/tmp/wagic-c8c054be8`, **ANSWER-LAST reply protocol** (scratch reasoning -> PLAN: ->
CHOICE:/ATTACK:/BLOCKS: final line), max_tokens 2048. **6 deck131 games.**

**Record: 0 W / 6 L (0/6).** Sixth straight bottom-tier wave (5:0/6, 7:0/6, 8:1/6, 9:0/6). Three
losses were LIFE-ADJUDICATED cap games (adj rows), three were actual deaths. Win column = context
only (USER EVALUATION DOCTRINE); the unit is the DECISION.

Seat-file -> game map (from `gamestart` + results.tsv):

| result | vs (deck) | file epoch | end life me/opp | cause |
|---|---|---|---|---|
| LOSS (death -2) | 135 (Modern Snow)      | 1784103512 | -2 / 18 | snow tempo; Guttersnipe traded T7, no rebuild |
| LOSS (adj1, T12) | 133 (Phyr. Asphodel)  | 1784105445 | 15 / 20 | mull-to-6, drew no payoff; DIG-gear grind, behind at cap |
| LOSS (adj1, T8)  | 110 (Etched Affinity) | 1784106735 | 6 / 25  | affinity outraced a lone Young Pyromancer |
| LOSS (death -6)  | 109 (mono-red Hellrider) | 1784110077 | -6 / 20 | aggro raced it; opp untouched (20) |
| LOSS (adj0, T32) | 140 (lifegain "Wipe Them Out!") | 1784110759 | 26 / 27 | grind; deck131 lost adj by ONE life |
| LOSS (death -3)  | 44 (Faerie Archmage)  | 1784112264 | -3 / 20 | faerie tempo + mana screw; opp untouched (20) |

Opp life at death/cap: 18, 20, 25, 20, 27, 20 -- the two wave-9 "close" softenings (opp 9, opp 5)
did NOT recur; this draw the deck finished well behind in every game. **OBEYED-BUT-LOSING, sixth
wave.** Transport clean apart from the known truncation class (see Fallbacks): **0 desyncs, 0
defers (`deferred_to_heuristic`), 0 `fails validation`, 0 engine no-ops** across all 6 games.

**Bottom line: the wave-9 frozen guide is VALIDATED on every axis it targets, and the answer-last
protocol KILLED the reply-protocol intent-collapse at this seat's mulligan seam -- the wave-9
headline residual is GONE. Zero decision-level guide defect this wave. strategy.txt is a
byte-identical freeze of the live guide (9036 bytes, `diff` clean).**

---

## HEADLINE -- the answer-last protocol closed the wave-9 residual: the mulligan intent-collapse class is DEAD at this seat. Both no-Mountain mulligans now LAND on Mulligan.

The wave-9 witness (its headline) was a false KEEP at exactly this seat: vs44 seq1, the PLAN
reasoned correctly to Mulligan a 0-Mountain hand but the HEAD token `1`=Keep was committed BEFORE
the reasoning and never revised -- an outcome/reasoning split (reasoning 1/1, outcome 0/1). The
brief's deck131 ask: *the new answer-last protocol should make the concluded mulligan decision
land; audit EVERY mulligan ask both directions.*

**Result -- 6/6 mulligan asks correct in BOTH reasoning AND outcome; the CHOICE now comes LAST and
matches the plan.** The two genuine mulligan windows (0-Mountain, no draw spell) both concluded
Mulligan AND recorded Mulligan:

- **vs135 seq1** (repro): hand `Elixir; Island; Island; Prism Ring; Island; Counterspell; Young
  Pyromancer` -- 0 Mountain, no Artificer's Epiphany/Opportunity -> correct MULLIGAN. Reply reasons
  the red-source gate, ends `CHOICE: 2`; `chosen_text` = **Mulligan**. LANDED.
- **vs133 seq1** (repro): hand `Counterspell; Island; Island; Essence Scatter; Island; Young
  Pyromancer; Prism Ring` -- 0 Mountain, no draw spell -> correct MULLIGAN. Reply reasons the gate,
  ends `CHOICE: 2`; `chosen_text` = **Mulligan**. LANDED.
- The four keepable hands (vs110/vs109/vs140/vs44 seq1) each had a Mountain + Young Pyromancer ->
  all correctly `CHOICE: 1` = **Keep this hand**. No false mulligan.

Both directions: **no-Mountain openers reasoned-correct = 2/2, outcome-correct = 2/2** (wave-9 was
1/1 reasoning, 0/1 outcome). **Keepable-hands-kept = 4/4.** The `CHOICE:`-last placement did exactly
what the wave-9 synthesis (headline 2) and the reply-protocol A/B predicted: the number is emitted
AFTER the reasoning, so a plan that concludes the other option now controls the choice. **The
wave-9 intent-collapse residual is closed at this seat -- do NOT re-open it, and do NOT touch the
mulligan gate (its 6/6 is what the answer-last protocol + the strict gate produce together).**

Label compliance corroborates: on 124 non-fallback asks, **124/124 produced a valid parsed choice**
and **121/124 carried an explicit CHOICE/ATTACK/BLOCKS label** (the 3 unlabeled -- vs140 seq35/43/46 --
still parsed to a correct choice from a trailing token; no anomaly).

## Mountain-first land execution -- 10/10. FREEZE.

Land co-offers (an option list containing BOTH "Play Mountain" and "Play Island"): **10** (vs135 1,
vs133 3, vs110 1, vs109 2, vs140 1, vs44 2). **Chose a Mountain option 10/10** -- including every
window where "Play Island" was listed first (the pilot picks by NAME, not index). No co-offer decline,
no non-Mountain pick. The wave-7 land-drop enumeration fix has now fired-and-obeyed across THREE
corpora (wave-8 12/12, wave-9 9/9, wave-10 10/10). CLOSED. FREEZE the pick rule; the interim
stopgap + stale menu explanation stay dropped.

## Guttersnipe engine -- cast-every-turn OBEYED; NEVER-block HELD.

- **NEVER-block-with-Guttersnipe: HELD.** No blocker window blocked with Guttersnipe. vs135 seq7
  (T6, Icehide Golem attacking, 18 life) -> `BLOCKS: none` (take the hit, keep Guttersnipe) --
  correct. All blocker decisions at 17-20 life answered no-block. No reflexive high-life block.
- **Cast-every-turn while Guttersnipe on board: OBEYED.** vs140 (the only game with a durable
  Guttersnipe) cast Aetherize twice AS FUEL with no attackers to bounce (seq43 T29, seq46 T31 --
  the reply explicitly weighs "Aetherize (waste, no targets)" and casts it anyway for the 2 face
  damage). This is the #1 rule ("the cast itself is the payoff") obeyed. Guttersnipe also ATTACKED
  (vs140 seq30/36/42, T21-27). 0 fuel-declined leaks.
- **`damage >= toughness = dead` watch -- NO recurrence, no misjudgment.** vs135: a 2/2 Guttersnipe
  took 2 combat damage from a 2/2 Icehide Golem and died correctly; the pilot never asserted it
  survives. Grep of all 6 replies for toughness-survival reasoning = 0 hits. Stays single-seat
  (wave-8 vs140 only). Do NOT promote.

## Engine-change firing (brief's confirmed-fired doctrine)

- **#1 answer-after-plan protocol** -- VALIDATED at this seat (see HEADLINE): label 124/124 valid,
  intent-collapse class gone, mulligan lands.
- **#2 fetch consume-on-choose** -- N/A: deck131 runs 14 Island + 8 Mountain, **zero fetchlands**.
  No fetch-crack windows at this seat.
- **#3 legal-target names on targeted casts** -- RENDERED and used cleanly. vs110 seq6: `Cast
  Cyclonic Rift {1}{u} - legal targets right now: Memnite, Ornithopter, Cranial Plating, Mox Opal`
  -> picked a real target (Mox Opal). vs109 seq19: `Cast Essence Scatter {1}{u} - NO legal target
  right now - can target on the stack: Ash Zealot` -> cast it on the on-stack creature spell.
  **No fabricated-target class at this seat** (the deck44 wave-9 s54 defect family did not appear here).
- **#7 adj0/adj1 rows** -- sanity-checked against seat final life: vs110 (tsv deck131=6, seat
  last=6) EXACT; vs133 (tsv=15, seat last=15) EXACT; vs140 (tsv=26, seat last=27) off-by-1 (the
  timeout cap snapshot at T32 is 1 lower than the last logged ask at T27 -- the jsonl writes no
  `gameend` for timeouts, expected; not a discrepancy).
- **#8 truncation guard** -- 7 fallbacks (6 `unparsed_reply` + 1 `empty_reply`), all the KNOWN
  2048-cap/confusion-spiral class. **None changed a game** (see Fallbacks).

## Fallbacks -- all the known truncation class; NONE changed a game

7 fallbacks across the corpus (2 vs135, 1 vs110, 1 vs140, 3 vs44):

- **vs135 seq7** (Blockers): reply reasons "could not block, take the damage, keep Guttersnipe" and
  ends `BLOCKS: none` -- correct intent; the heuristic no-block agrees. No harm.
- **vs135 seq11** (Main, 17 life): a confusion spiral over Opportunity targeting (7.6k chars, hit
  cap). BUT Guttersnipe was **already dead** (`Your battlefield (creatures: 0)`, all lands tapped)
  -- no fuel lost; the pilot was tapped out anyway. No game impact.
- **vs135 seq17** (`empty_reply`): Elixir vs Cast-nothing at 17 life -- trivial.
- **vs110 seq5**: Play/Hold Island (a single land drop) confusion spiral; heuristic plays the land.
  Trivial tempo.
- **vs140 seq16**: Elixir vs Cast-nothing at 22 life in a grind -- trivial.
- **vs44 seq9/11/14**: three mana-screw spirals ("Mana available: (no untapped sources)", "no blue
  mana", "I am stuck") -- the pilot had **no mana to act**; a heuristic fallback could not have done
  better. The game was lost to mana screw + faerie tempo, not to these fallbacks.

The trigger across the non-blocker spirals is **mana-availability confusion** (the model distrusts
"no untapped sources" or miscomputes generic-mana payment and loops to the token cap). Known class,
self-healing, routed to notes.md -- NOT a guide item.

## FROZEN-LINE RECHECK (obeyed-but-losing discipline -- verify obedience, do not churn)

- **Strict two-step mulligan gate** -- 6/6 correct both directions; intent-collapse residual now
  closed by the protocol. FREEZE verbatim. NO demotion: the gate is compact, load-bearing, not
  double-covered by any stronger layer -- demoting the guardrail that just produced the 6/6 is
  exactly the churn the pause-aware doctrine forbids.
- **Mountain-first** -- 10/10, closed across 3 corpora. FREEZE (stopgap stays dropped).
- **Guttersnipe cast-every-turn #1 / two-gear identity / deploy-payoffs / cast-creature-first /
  YP-swarm second win path** -- obeyed. FREEZE.
- **Attack-every-creature / no-block-while-racing / NEVER-block-Guttersnipe** -- obeyed; no
  high-life block, Guttersnipe never blocked. FREEZE.
- **Reactive spells / board-wipe counter clause / draw / Prism-Elixir-lifegain / lethal-check** --
  obeyed. Board-wipe counter clause had NO precondition window again (vs140 never resolved a
  Pyroclasm into a live Guttersnipe with a counter up); untested, kept.
- **Winning-line block** -- the de-specified "how this deck's fastest wins close" (wave-9 edit)
  reads clean and corpus-stale-free. No change.

## Routing summary
1. **[GUIDE -- FREEZE, byte-identical]** strategy.txt = the live wave-9 frozen guide, `diff` clean.
   Every targeted line validated; no decision-level defect; churning obeyed lines off a 0/6 corpus
   is the anti-pattern the doctrine forbids.
2. **[REPLY-PROTOCOL -- CLOSED at this seat]** the wave-9 intent-collapse residual (vs44 mulligan
   false-Keep) is GONE under answer-last: both no-Mountain mulligans land Mulligan. The A/B that was
   DUE has effectively shipped and is validated here. skill.md records the closure; not a guide item.
3. **[DECK construction -- unchanged, dominant, RESTATED not relitigated]** threat density 6/60
   (3 Guttersnipe + 3 Young Pyromancer), no non-incremental finisher, no reach vs lifegain -- the
   user owns this. notes.md restates it.
4. **[CORE -- PASS]** no 2+-seat core candidate at this seat. `damage>=toughness=dead` stays a
   single-seat WATCH. general-suggestions.md = PASS.
5. **[WATCH -- single-seat, unscored]** attack-seam partner to NEVER-block-Guttersnipe (vs135
   Guttersnipe traded while attacking, no captured attack-decision record); Prism-Ring-in-a-race
   (vs109). Both notes.md, both below the bar to touch the guide.
