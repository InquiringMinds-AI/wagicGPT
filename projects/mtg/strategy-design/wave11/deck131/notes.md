# deck131 wave-11 — dev notes (engine / harness / model / deck-construction; self-contained)

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-072254/`, 6 deck131 games,
**1W/5L**. Live guide = wave-10 FROZEN revision. Binary `/tmp/wagic-54a5a9d01` (wave-10 batch +
max_tokens 4096 + ramble guards + stack-target-annotation fix + fetch pre-targeting keying).
Answer-last reply protocol. Seat files (from `gamestart`):
vs133 1784118179 (L -2,T19) | vs140 1784119853 (adjL 23,T30) | vs109 1784123504 (L -4,T13) |
**vs135 1784124329 (WIN adj 19/1,T15)** | vs44 1784126729 (L 0,T14) | vs110 1784127211 (L -6,T11).

Layer-routing note: engine/harness/model items live HERE, never in prompt text. Deck-construction
flags are RESTATED for the user, not relitigated. Score against `chosen_text` for ASK records; for
BLOCKERS records see H1 (chosen_text is UNRELIABLE under the unparsed-none bug — cross-check combat
events).

## Reproducible evidence — validations (guide axes)

### V1 — the WIN line (vs135): Young-Pyromancer swarm, no Guttersnipe. Guide's second-win-path proven.
- File `...deck131...vs...deck135.jsonl`. `Guttersnipe: stack -> battlefield` events = 0 all game.
- Payoffs: YP cast seq6 (T3) + a 2nd YP seq21 (T9). Attackers seq24 (3 named) + seq29 (5 named:
  YP, Elemental, YP, Elemental, Elemental). Instants cast as token fuel: Downsize seq14, Aetherize
  seq29. Opp 20 -> 1 by T13; adjudicated T15 at deck131 19 / deck135 1. WIN.
- Repro: `python3 -c "import json;R=[json.loads(l) for l in open(F) if l.strip()];print([ (r['seq'],r.get('chosen_text')) for r in R if r.get('kind')=='attackers'])"`.

### V2 — mulligan gate 7/7 (both directions)
- 5 Mountain+YP keeps (vs133/140/109/135/44 seq1) + 1 no-red MULL (vs110 seq1: no Mountain, no
  Artificer's Epiphany/Opportunity) + 1 six-card dig-KEEP (vs110 post-mull: Artificer's Epiphany +
  2 Island). No false keep, no false mulligan.
- Repro: filter asks with 'opening hand' in events; test `'Mountain' in hand` or
  `'Artificer' in hand or 'Opportunity' in hand` vs chosen_text.

### V3 — Mountain-first 12/12
- Co-offers (options list has BOTH 'Play Mountain' and 'Play Island'): 12 (vs133 2, vs140 3,
  vs109 1, vs135 3, vs44 3). Chose a Mountain option 12/12, incl. 10 Island-listed-first windows.

### V4 — legal-target names rendered/used; no fabricated-target class
- vs135 seq14 `Cast Downsize {u} - legal targets right now: Ice-Fang Coatl, Ohran Viper` -> Ohran
  Viper. vs110 seq5 `Cast Cyclonic Rift {1}{u} - legal targets right now: Mox Opal, Signal Pest`.

## HARNESS / MODEL ledger (with repros)

### H1 — [NEW, GAME-RELEVANT] bare `BLOCKS: none` is UNPARSED -> baka blocker heuristic runs -> blocked Guttersnipe into a fatal trade (vs109)
- **Repro (corpus-wide parse asymmetry, all 6+ decks' seats):**
  - `BLOCKS: none` (bare) -> `fallback=unparsed_reply` EVERY time: deck131 x3 (vs109 seq13,
    vs135 seq17, vs135 seq30), deck44 x1 (`Scion of Oona:none` name-form). Count = 4 unparsed.
  - `BLOCKS: B<n>:none` (explicit per-blocker none) -> parses CLEAN every time: deck44 x3. Count = 3.
  - So the parser accepts `B<n>:none` and REJECTS bare `BLOCKS: none` / a name-only-`:none`.
  - Repro script: for each `*ai_baka_deck*.jsonl`, for `kind=='blockers'` records, extract the
    `BLOCKS:` line arg via regex and bucket by `fallback` truthiness. `none`(bare) -> all unparsed;
    `B1:none` -> all clean.
- **Game cost (vs109, a LOSS):** seq13 (T10, 10 life, opp attacking with Rakdos Cackler + 2 Goblins
  + Stromkirk Noble). The reply reasoned NEVER-block-Guttersnipe correctly ("I should preserve my
  Guttersnipe... I will declare no blockers") and emitted `BLOCKS: none`. jsonl recorded
  `fallback=unparsed_reply, choice=0, chosen_text='None'` — BUT the next record's combat events
  (seq14) show `Damage: 2 dealt by Guttersnipe to Rakdos Cackler` + `2 dealt by Rakdos Cackler to
  Guttersnipe` + both to graveyard: **Guttersnipe was blocked into a trade.** Confirmed in
  `game-131v109-1784123501.stderr`: `Defenser Toggle: Guttersnipe` then `AIPlayerGPT: declared
  blocks from 1 assignment(s) in one reply`. So the unparsed reply fell to the baka blocker
  heuristic, which assigned Guttersnipe (the deck's ONLY reach) to block — the EXACT play the guide
  forbids and the pilot reasoned against. Guttersnipe never returned; deck131 lost -4.
- **Why it looked benign in wave-10:** wave-10 predecessor saw vs135 seq7 `BLOCKS: none` as an
  unparsed fallback where "the heuristic no-block agrees, no harm." It got LUCKY that baka also
  declined. The systematic bug is that baka is consulted at ALL for a cleanly-reasoned decline, and
  it can DISAGREE and block. This wave it disagreed on the one creature the guide most protects.
- **Root fix (harness/parse, AIPlayerGPT reply parser):** accept bare `BLOCKS: none` and "no
  blockers" / a terminal `none` as "all listed blockers decline" (the prompt already tells the model
  "Blockers you do not mention stay out of combat", so a bare decline is a legitimate, expected
  reply shape). Until fixed, an unparsed blockers reply must NOT default to the baka blocker
  heuristic when the reply's intent is a clean decline — a safer fallback is all-decline.
- **Routing: HARNESS/PARSE. NOT a guide change.** Layer-routing doctrine forbids re-wording the
  guide to emit `B1:none` — that papers a parser bug in prose (the sanctioned-against escalation).
- **Reviewer method note (goes to skill.md too):** for `kind=='blockers'`, `chosen_text` can
  DIVERGE from executed combat under this bug — score block-side obedience by the reply's reasoning
  + the NEXT record's combat events / opponent-seat log, never by `chosen_text` alone.

### H2 — [KNOWN CORPUS-WIDE class] confusion-spiral under 4096: most conclude, deepest time out (empty_reply)
- 14 fallbacks this seat: **13 `empty_reply` + 1 `unparsed_reply`** (wave-10 was 6 unparsed + 1
  empty). All 13 empty_reply have `latency_ms` in 120003-120004 = the 120s HTTP-client timeout wall.
- The brief's ask (do spirals conclude under 4096, or burn to the timeout?): **SPLIT.** The majority
  of long mana/dead-spell deliberations now REACH a correct terminal CHOICE (vs140 seq37 5588-char
  "cast nothing" correct; seq52 5135-char "I must pass" correct; seq56 4887-char "cast Hydrolash"
  correct; vs133 seq16 4422-char "Play Island" correct). Only the DEEPEST spirals run past 120s and
  the HTTP client aborts with an empty body -> empty_reply -> heuristic. So raising the cap converted
  a TRUNCATION class into a LATENCY class.
- Impact: mostly trivial windows (Elixir/Prism/nothing, Play/Hold Island). A couple were Aetherize-
  as-fuel windows in the WIN game (vs135 seq26 @ opp 9, seq28 @ opp 8) — may have cost a token each,
  but the swarm closed anyway (opp -> 1). **None changed a game.**
- MODEL trigger (harness eye, not a guide item, same as wave-10): the spirals are driven by
  MANA-AVAILABILITY distrust ("no untapped sources") / generic-mana-payment looping. Same family as
  the core's "(none) read as I cannot cast" correction. Fix is the 240s timeout (shipped next,
  commit 95cf9f5f9); the deeper root is model comprehension of the mana line.
- Repro: filter asks where `r['fallback']` is truthy; check `latency_ms`.

### H3 — [REPLY-PROTOCOL, 2nd witness] plan fixates on an UNLISTED card + emits an OUT-OF-RANGE index (vs110 seq17)
- vs110 seq17 (T?, 9 life, being outraced by affinity). Options were 4: Cast Downsize {u}, Cast
  Downsize overload {2}{u}, Cast Hydrolash {2}{u}, Cast nothing. The PLAN reasoned "I will cast
  Young Pyromancer immediately to start building a board presence... then cast Hydrolash" — but
  **Young Pyromancer is NOT in the option list** (all options are instants). It emitted
  `CHOICE: 5` — out of range for 4 options -> `unparsed_reply` -> heuristic.
- This is the 2nd witness of wave-10 deck110 P3 (single-seat sub-variant: "plan fixates on an
  UNLISTED card and emits a poorly-mapped fallback label that contradicts its own sub-conclusion
  about the listed options"). First witness deck110 seat; now deck131 seat. Routes to the code-
  appended REPLY-PROTOCOL layer (the answer-last trailing-index defect family: the trailing index
  is wrong/out-of-range while the reasoning is a mis-target of an unlisted card). Ramble guard
  handled it correctly (recorded unparsed, deferred to heuristic — no prose-scanned cast). In a
  likely-lost game (affinity outraced a lone threat); marginal. WATCH; the durable target is the
  reply-protocol A/B (reconcile the trailing index against listed options / reject an out-of-range
  or unlisted-target index). NOT a guide item.

### H4 — [REPRESENTATION + rules-gap, single-seat WATCH] block-when-not-lethal via a FIRST-STRIKE combat-math error (vs109 seq16)
- vs109 seq16 (T12, **7 life**, opp attacking Ash Zealot 2/2 first-strike-haste + 2 Goblins +
  Stromkirk Noble; total 6, deck131 survives at 1 EITHER way — NOT lethal). The pilot blocked Ash
  Zealot with Young Pyromancer, rationalizing "Zealot deals 2 first (I go to 5), then Pyromancer
  deals 2 (Zealot dies). This is a good trade." **The math is WRONG: Ash Zealot has FIRST STRIKE,
  so it deals 2 to the 2/1 YP before YP deals any damage — YP dies, Ash Zealot SURVIVES.** So YP was
  sacrificed for nothing while NOT facing lethal (the guide says block only when going to 0).
- Two overlapping items: (a) an instance of the standing reflexive/mis-framed-blocking convergence
  class (block-when-not-lethal, framed as a "good trade") — already routed to REPRESENTATION (the
  blocker-seam lethal / `[not lethal]` / trade-outcome rider) and rejected as a core sentence 5x;
  the guide already forbids the behavior, so a louder guide line is the sanctioned-against
  escalation. (b) A distinct FIRST-STRIKE rules-knowledge gap in the combat math (the model treated
  a first-strike attacker as a normal simultaneous trade). This is the first-strike flavor of the
  `damage>=toughness` gap; it strengthens the un-shipped blocker-option representation rider
  (surface the actual trade outcome, e.g. `[you deal 0 - first strike]`). Single-seat this wave, in
  a likely-lost aggro race. WATCH; do NOT promote off one instance, do NOT touch the guide.

## DECK-CONSTRUCTION ledger (RESTATED for the user; user owns; not relitigated)
The standing 6-wave diagnosis holds; the losses are construction-bound (opp untouched or ahead in
every loss):
- **Threat density 6/60** (3 Guttersnipe + 3 Young Pyromancer). The WIN happened precisely because
  the deck drew and HELD two Young Pyromancers and did not need Guttersnipe — variance in the
  redundant-payoff direction, not a play change. In the losses Guttersnipe hit the board 3x and
  died on the cast turn twice (a 2/2 body answered by anything).
- **No non-incremental finisher / no reach vs lifegain.** vs140 (lifegain) went to the T30 cap and
  deck131 lost adjudication by THREE life (23 vs 26) — with a single burn/reach finisher that game
  likely flips. Sideboard already carries Spellheart Chimera (reach body) + Aetherling (finisher)
  as candidate swaps.
- **Manabase 14 Island / 8 Mountain leans slightly wrong** for two red win conditions; the vs110
  opener mulligan and much of the vs44 mana screw were red-source shortfalls. Interface is clean
  (Mountain-first 12/12) — this is a draw/count problem, not an unofferable-menu artifact.
- These are the user's to decide; recorded, not argued.

## WATCH (single-seat, unscored — below the bar to touch the frozen guide)
- **Guttersnipe attack-trade (2nd instance).** vs109 seq14 combat: Guttersnipe traded with Rakdos
  Cackler (both 2/2, both died), removing the deck's only reach — same shape as wave-10's vs135
  Guttersnipe-traded-while-attacking WATCH. STILL no attributable pilot attack-decision record: the
  vs109 seat file has NO `attackers` kind (attack declaration was not routed to the GPT layer that
  turn), so it cannot be attributed to a pilot choice. The 2nd instance strengthens the case for the
  ENGINE/HARNESS to route Guttersnipe's attack declaration to the pilot (so the guide's implicit
  "keep Guttersnipe alive" can govern the attack step) — a harness item, not a guide line. Do NOT
  add an attack-seam guide clause off two decision-record-less instances.
- **Prism-Ring-in-a-race (carryover).** Not re-observed as a clear leak this wave; carried as prior
  WATCH only.

## For a future deck131 reviewer — gotchas
- n=6, single run: win-rate is noise (0/6 wave-10 -> 1/6 wave-11 is variance, the WIN was a lucky
  redundant-payoff draw). The guide is at its terminus (6 waves obeyed-but-losing); wins come from
  CONSTRUCTION (threat density / reach), not guide prose. Resist churning frozen lines.
- **BLOCKERS chosen_text is UNRELIABLE under the H1 bug** — for a `kind=='blockers'` record with
  `fallback=unparsed_reply`, `chosen_text='None'` records the PARSE, not the executed combat; the
  baka heuristic may have blocked. Cross-check the NEXT record's combat `events` or the opponent-
  seat log.
- Reply-protocol intent-collapse (wave-9 mulligan) stays CLOSED under answer-last; do not re-open.
- `choice` is the 1-based emitted index; for ASK records `chosen_text` is authoritative for what the
  engine did. Isolate `Your battlefield (creatures: N):` for on-board checks (a loose "Guttersnipe"
  substring matches hand/cast-list/guide text). Timeouts write no per-seat `gameend`; use
  results.tsv adj rows for final life (deck131 appears as deck0 OR deck1 — read the winner column
  by position: `winner==adj1`/`==1` means deck1 won).
