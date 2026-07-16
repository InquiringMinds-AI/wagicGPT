# deck131 wave-13 — dev notes (engine / harness / model / deck-construction; self-contained)

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-183228/` (wave-13b, first CLEAN
post-crash validation), 6 deck131 games, **2W/4L**. Live guide = wave-11 FROZEN revision (9036 bytes;
byte-identical freeze copied to strategy.txt, `cmp` clean). Binary `/tmp/wagic-a75e2625e`
(Force r2 eb5145082 + Force r3 a75e2625e + auto-tap two-pass d5d7b1f2e + **name-echo c3e6d95b2** +
**BLOCKS:none 42e8bc180** + 240s timeout). Spark GPU_MEM_UTIL=0.75 → high latency; 13/21 corpus games
life-adjudicated (do NOT read adjudication as play-quality). Answer-last + name-echo reply protocol.

Seat files (deck131 as the -0x seat):
vs110 1784160752 (L -2,T9) | vs109 1784160756 (L -6,T15) | **vs135 1784162578 (WIN adj 25/8,T16)** |
vs133 1784163693 (adjL 18/22,T6) | vs44 1784167378 (L -2,T14) | **vs140 1784168494 (WIN adj 31/29,T34)**.

Layer-routing: engine/harness/model items live HERE, never in prompt text. Deck-construction flags
are RESTATED for the user, not relitigated. Scoring gotchas: for `kind=='blockers'`, cross-check the
NEXT record's combat events / the OPTION LEGEND vs the battlefield line — `chosen_text` and even the
offered-blocker set can diverge from intent (see L2). For CHOICE records, score the ECHO content too,
not just `choice`/`chosen_text` — index-wins-on-conflict can hide a model mis-map (see L3).

## Reproducible evidence — validations (guide axes + engine fixes)

### V1 — engine-fix win #1: bare `BLOCKS: none` now PARSES CLEAN (wave-11 H1 CLOSED)
- deck109 s7 `BLOCKS: none` → `chosen_text='no blockers'`, `fallback=None`. deck109 s12 `BLOCKS: None`
  → same. The wave-11 fatal-Guttersnipe-trade bug (bare none → unparsed → baka blocks) is fixed
  (commit 42e8bc180). Repro: for `kind=='blockers'`, extract the `BLOCKS:` line, confirm `fallback`
  is None for a bare `none`/`None`.

### V2 — engine-fix win #2: name-echo 163/163, ZERO wrong-remaps
- Repro: for every `ask` record with `fallback` None and options, take the LAST `CHOICE: N (echo)`
  line; check `norm(echo) in norm(options[N-1])`. Result: 122 exact index-matches + target/descriptive
  echoes that match on inspection; **0 cases where echo matched a DIFFERENT LISTED option** (the HIGH
  wrong-remap case). Index-wins-on-conflict fired only against UNLISTED-card mis-echoes (L3) → benign.

### V3 — Mountain-first 15/15 (11 Island-listed-first)
- Co-offers (options contain BOTH 'Play Mountain' and 'Play Island'): 15 — vs110 s2, vs109 s2,
  vs135 s5/s10/s12, vs133 s2/s5/s8, vs44 s2/s5/s12, vs140 s2/s6/s8/s27. Chose Mountain 15/15.

### V4 — win #1 shape (vs135): YP token swarm, NO Guttersnipe (guide 2nd-win-path)
- `Guttersnipe: stack -> battlefield` = 0 all game; `Young Pyromancer: stack -> battlefield` = 2;
  `Elemental (1/1): created` = 4. YP cast s6 (T3); 2nd YP s19 (T13); instants cast as token fuel
  (Downsize s?/Cyclonic Rift s22/Hydrolash s28). Opp raced 20→8; adjudicated 25/8 at T16. NOTE: the
  win rested on a hand the guide says to MULLIGAN (L1) and on lifegain padding (Prism Ring s3,
  Opportunity-draw, Elixir s24) that helped the LIFE adjudication — doubly variance/artifact-driven.

### V5 — win #2 shape (vs140): Guttersnipe grind-stall, board-wipe counter fired
- `Guttersnipe: stack->battlefield` = 2 (s34 T23, s37 T29); YP = 1; Elemental = 0. s38 T30 cast
  Dissipate targeting **Wrath of God** on the stack — the guide's board-wipe-counter rule (Wrath
  kills the 2/2 Guttersnipe) firing correctly. 34-turn stall vs a lifegain deck; out-lifegained to a
  31/29 cap edge. A legitimate grind-win for the matchup, but a razor life-adjudication, not a kill.

## HARNESS / ENGINE / MODEL ledger (with repros)

### L1 — [MODEL-ADHERENCE, NEW, prominent WATCH] mulligan STEP-1 gate MISFIRE (vs135 s1): kept a no-red/no-dig hand, miscited the guide, won on variance
- **First mulligan deviation in 7 waves** (wave-11 was 7/7). Opening hand:
  `Prism Ring; Island; Island; Counterspell; Essence Scatter; Island; Young Pyromancer` — no Mountain,
  no Artificer's Epiphany, no Opportunity → a clean STEP-1 MULLIGAN by the guide. Model KEPT it.
- s1 reply (verbatim errors): "the presence of Young Pyromancer... is a strong keep... **hope to draw
  a Mountain**... The strategy guide **allows keeping hands with draw spells or payoffs** even without
  red." FALSE: STEP-1's only exception is Artificer's Epiphany / Opportunity (DRAW spells), NOT
  payoffs/counters; and "hope to draw a Mountain" is the exact thought the guide flags as a TRAP.
- Drew a Mountain T3, cast YP, swarmed → won (V4). Pure variance; the guide warns it's unreliable.
- **Attribution: MODEL-ADHERENCE, not a guide gap.** Guide already maximally explicit — louder text
  is the sanctioned-against escalation. Route WATCH; re-check next corpus for a 2nd instance (possible
  name-echo-era behavior drift). Do NOT touch the frozen guide. Repro: read s1 hand from the first
  ask's game-log `opening hand` line; test `'Mountain' not in hand and 'Artificer' not in hand and
  'Opportunity' not in hand` vs `chosen_text=='Keep this hand'`.

### L2 — [ENGINE/HARNESS, GAME-RELEVANT] attacker name→index parse gap (the brief's `ATTACK: Hellrider` shape) — 5/18 declarations leak names
- Attacker protocol wants A-indices. 18 declarations: 13 pure-A (clean) + 3 MIXED + 2 PURE-NAME.
  - **MIXED `ATTACK: A1, <name>` → name token silently DROPPED (only A1 attacks):** deck110 s13
    (`A1, Elemental`), deck109 s11 (`A1, Elemental (1/1)`), deck135 s9 (`A1, Elemental`). In each, a
    live Elemental token existed that turn and was omitted from the swing — a silent MISSED ATTACKER
    (~1 power each; contradicts the guide's "attack with EVERY creature").
  - **PURE-NAME (no A-index) → whole reply `unparsed_reply` → baka heuristic:** deck109 s28
    (`ATTACK: Elemental`), and **deck135 s29** (`ATTACK: Elemental (1/1), Young Pyromancer, Elemental
    (1/1), Elemental (1/1), ...`) — the WIN game's final 5-attacker alpha strike, unparsed → heuristic;
    won by adjudication anyway but the authored lethal swing wasn't executed.
- **Root fix (ENGINE/HARNESS): attackers name→index reconcile arm** — map each attacker name token to
  its unique battlefield A-index (drop only on genuine ambiguity), mirroring parseChoice / the
  name-echo reconcile arm. Handles BOTH the mixed partial-drop and the pure-name unparse. HIGH ledger
  priority — the only game-relevant engine gap at this seat, and it degrades the guide's core line.
- Repro: for `kind=='attackers'`, take the last `ATTACK:` line; tokenize on commas; a token not
  matching `^A\d+` is a name-form leak; check `fallback` (pure-name → unparsed) and `chosen_text`
  (mixed → only the A-index creature).

### L3 — [REPLY-PROTOCOL / MODEL-COMPREHENSION, WATCH] mis-echo of an UNLISTED card at an in-range index (wave-11 H3 continuation) — contained benign by index-wins
- 6 instances of `CHOICE: N (<card-not-in-options>)`: deck110 s3 / deck44 s3 / deck44 s4 / deck140 s3
  all `(Cast Young Pyromancer)` at T1 (YP uncastable, not listed) → index landed on "Cast nothing";
  deck135 s24 `(Cast Hydrolash)` → index 1 = Elixir (benign, padded life); deck140 s41
  `(Cast Artificer's Epiphany)` → index 2 = "nothing".
- Same class as wave-11 H3 ("plan fixates on an unlisted card"), but the failure MODE changed: wave-11
  emitted an OUT-OF-RANGE index → unparsed; name-echo era → in-range index + index-wins-on-conflict
  → benign (echoed card is unlisted, no unique remap, index wins). **0 game impact this wave** (every
  mis-echoed card was uncastable; index landed on nothing/lifegain). WATCH; root is model
  comprehension (believing an unlisted/uncastable card is available at index N). NOT a guide item.
  Reviewer note: "0 wrong-remaps" does NOT mean the model stopped hallucinating options — score the
  echo content to keep this class visible.

### L4 — [ENGINE/REPRESENTATION, single-seat, VERIFY] blocker legend omitted a legal untapped blocker (vs109 s29)
- vs109 s29 (T16, 4 life, lethal 7-attacker swing — dead either way, **0 game impact**). Prompt
  blocker legend listed ONLY `B1. Guttersnipe (2/2)`, though the battlefield line showed an untapped
  `Elemental (1/1)` (`creatures: 2`, neither tapped). The model reasoned NEVER-block-Guttersnipe
  correctly and wanted to block with the Elemental, but with it absent from the menu forced
  `BLOCKS: B1:A4` (believing B1 = Elemental); engine executed B1 = Guttersnipe
  (`chosen_text='Guttersnipe blocks Goblin'`). Decision reasoning correct; MENU wrong.
- VERIFY caveat: confirm whether the engine legitimately excluded the Elemental (it attacked T13,
  should have untapped by T16 — no visible reason). Route: ENGINE blocker-enumeration ledger. Do NOT
  touch the guide. Reviewer-method: cross-check the offered-blocker legend against the battlefield
  line before attributing a block-side guardrail "violation" to the model.

## DECK-CONSTRUCTION ledger (RESTATED for the user; user owns; not relitigated)
The standing 7-wave diagnosis holds; the 4 losses are construction-bound (opp untouched or ahead in
every loss), and the 2 wins are variance/latency-adjudication artifacts:
- **Threat density 6/60** (3 Guttersnipe + 3 Young Pyromancer). vs109/vs110 fielded a lone payoff
  that was removed/outraced; Guttersnipe arrived T15 (vs109) into a dead board or never (vs110). The
  wins came from redundant-payoff variance (vs135 held two YP) or from out-grinding a slow deck
  (vs140), not from a play change.
- **No non-incremental finisher / no reach vs lifegain / no evasion answer.** vs44 lost to a recurring
  evasive Faerie Bladecrafter (bounced 3x, kept returning bigger) — the deck has no permanent answer
  to an evasive recurring threat. Sideboard already carries Spellheart Chimera (reach body) +
  Aetherling (evasive finisher) as candidate swaps.
- **Manabase 14 Island / 8 Mountain leans slightly wrong** for two red win conditions. Interface is
  clean (Mountain-first 15/15) — a draw/count problem, not an unofferable-menu artifact. NOTE:
  latency-adjudication now PENALIZES the slow-dig openers this manabase produces (vs133 adjudicated at
  T6 with no payoff down, behind on life) — a corpus-artifact interaction, not a construction verdict.
- These are the user's to decide; recorded, not argued.

## WATCH (single-seat, unscored — below the bar to touch the frozen guide)
- **Guttersnipe attack-trade** — not re-observed as a pilot decision this wave (vs140's Guttersnipe
  attack s39 was a lone safe swing). Carried as prior WATCH only.
- **Lifegain-in-a-race** — the model cast Prism Ring/Elixir freely (vs135 s3, vs110 repeated Elixir
  activations, vs140). Under LATENCY-ADJUDICATION this HELPED (padded the life the adjudicator reads),
  but that is a corpus artifact; the guide's de-prioritization of lifegain remains correct for actual
  kills. Do NOT encode life-padding — it optimizes for the latency artifact, not the win condition.

## For a future deck131 reviewer — gotchas
- n=6, single run, high-latency corpus: win-rate is noise (0→1→2/6 across waves is variance). BOTH
  wins here were LIFE-ADJUDICATION (ahead at the 2400s cap), not kills — read them as "ahead on life
  at the cap," and check WHETHER for the right reasons (vs135 yes via a real race; vs140 a razor
  lifegain edge). Guide is at its terminus (7 waves obeyed-but-losing); resist churning frozen lines.
- **name-echo era scoring:** score the CHOICE ECHO content, not just `choice`/`chosen_text` —
  index-wins-on-conflict can mask a model mis-map to an unlisted card (L3). Take the LAST `CHOICE:`
  line as the answer-last final.
- **blockers scoring:** cross-check the offered blocker LEGEND against the battlefield line AND the
  next record's combat events — both `chosen_text` and the offered-blocker set can diverge from intent
  (L4). Bare `BLOCKS: none` now parses clean (V1) — the wave-11 H1 unreliability is CLOSED.
- **attackers scoring:** name-form tokens leak (L2) — a MIXED `A1, name` silently drops the name; a
  pure-name reply unparses. Verify intended vs executed attacker count.
- Timeouts/adjudications write no per-seat `gameend`; use results.tsv adj rows for final life (deck131
  appears as deck0 OR deck1 — read the winner column by position: `winner==adj1`/`==1` = deck1 won).
