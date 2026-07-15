# deck131 — wave 11 review (UR Guttersnipe spellslinger; "Mind Control / Best Counter Deck Ever")

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-072254/`, one GPT-vs-GPT
round-robin (decks 44,109,110,131,133,135,140), qwen35 both seats, wave-10 (FROZEN) guide in play,
binary `/tmp/wagic-54a5a9d01` (wave-10 engine batch + **max_tokens 4096** + parse ramble guards +
stack-target annotation fix + fetch pre-targeting keying). **ANSWER-LAST reply protocol**
(scratch reasoning -> PLAN: -> CHOICE:/ATTACK:/BLOCKS: final line). **6 deck131 games.**

**Record: 1 W / 5 L (1/6).** First win since wave-8. The win is `135 131 adj1 1 19 15` in
results.tsv — deck131 beat deck135 (Modern Snow) by adjudication at T15, deck131 at **19** life,
deck135 at **1** (deck131 raced it to the brink). Win column = context only (USER EVALUATION
DOCTRINE); the unit is the DECISION.

Seat-file -> game map (results.tsv, deck131 as deck0 or deck1):

| result | vs (deck) | file epoch | end life me/opp | cause |
|---|---|---|---|---|
| **WIN (adj1, T15)** | 135 (Modern Snow)   | 1784124329 | 19 / 1  | Young-Pyromancer token swarm; NO Guttersnipe all game |
| LOSS (death -2, T19)| 133 (Phyr. Asphodel)| 1784118179 | -2 / 11 | mull-kept, Guttersnipe died on cast; ground out, drew behind |
| LOSS (adj0, T30)    | 140 (lifegain)      | 1784119853 | 23 / 26 | grind to cap, lost adj by 3; Guttersnipe died on cast |
| LOSS (death -4, T13)| 109 (mono-red Hellrider)| 1784123504 | -4 / 18 | aggro raced it; Guttersnipe blocked into a trade (see H1) |
| LOSS (death 0, T14) | 44 (Faerie Archmage)| 1784126729 | 0 / 20  | faerie tempo + mana screw; opp untouched (20) |
| LOSS (death -6, T11)| 110 (Etched Affinity)| 1784127211| -6 / 18 | affinity outraced; unlisted-card CHOICE misfire at 9 life |

**Bottom line: the wave-10 frozen guide is VALIDATED on every axis it targets. The mulligan gate
(6/6 + one six-card dig-keep), Mountain-first (12/12), the two-gear identity, NEVER-block-
Guttersnipe (obeyed in ALL reasoning), and the Young-Pyromancer second-win-path are all clean —
the WIN is a textbook execution of the guide's YP-swarm line. Zero *guide-addressable* decision
defect this wave. strategy.txt is a byte-identical freeze of the live guide (9036 bytes, `diff`
clean).**

**The one game-relevant decision-cost this wave is a HARNESS/PARSE defect, not a guide gap
(H1 below): bare `BLOCKS: none` — the natural all-decline reply — is systematically UNPARSED,
handing the block to the baka heuristic, which at vs109 blocked Guttersnipe (the only reach) into a
fatal trade — the exact play the guide forbids and the pilot correctly REASONED against.** Route:
notes.md (harness). Layer-routing forbids re-wording the guide over a parse defect.

---

## HEADLINE — the win line: a Young-Pyromancer token swarm, NO Guttersnipe all game. The guide's "second win path" VINDICATED end-to-end.

The vs135 win is the guide's own predicted fastest close, verbatim: *"A swarm of Elemental tokens
is a real second win path on its own — if Guttersnipe never shows up, Young Pyromancer plus its
tokens can still race the game out (that is how this deck's fastest wins close: opp 20 -> dead on a
token swarm, no Guttersnipe on board all game)."* Guttersnipe was **never cast** in this game
(`Guttersnipe: stack -> battlefield` events = 0). The line, turn by turn:

- **T1** Keep (Mountain+YP hand); Play Mountain.
- **T3** Play Mountain; **Cast Young Pyromancer** — payoff down on curve.
- **T5** Play Mountain; attack YP.
- **T7** Cast Downsize (instant -> YP makes a 1/1 Elemental token); attack YP -> opp 16.
- **T9** Cyclonic Rift on Ohran Viper (tempo/removal); **cast a 2nd Young Pyromancer**.
- **T11** Attack with YP + Elemental + YP (seq24, 3 named attackers) -> opp 14 -> 9.
- **T13** Aetherize cast as fuel (instant -> another token); attack **5 creatures** (seq29:
  YP, Elemental, YP, Elemental, Elemental) -> opp 1.
- **T15** Opp at 1; game adjudicated, deck131 at 19. WIN.

Execution highlights: **attack-every-creature OBEYED** (seq24 named 3, seq29 named 5, no creature
held home while ahead); **cast-instants-as-token-fuel OBEYED** (Downsize/Aetherize each spawned a
token via YP); Mountain-first 3/3 in this game. This is the guide operating exactly as authored on
a game where the primary payoff never arrived — the strongest single-game validation of the
YP-swarm line the project has produced. FREEZE the line; it works.

## Mulligan gate — 6/6 correct both directions + one six-card dig-keep. STEADY. FREEZE.

Every opening-hand ask, both directions:

| vs | hand has Mountain? | dig spell? | should | chose | ok |
|---|---|---|---|---|---|
| 133 | yes | no | KEEP | Keep | ok |
| 140 | yes | no | KEEP | Keep | ok |
| 109 | yes | no | KEEP | Keep | ok |
| 135 | yes (x2) | no | KEEP | Keep | ok |
| 44 | yes | no | KEEP | Keep | ok |
| 110 seq1 | **no** | **no** | **MULL** | **Mulligan** | ok |
| 110 six-card (post-mull) | no | **yes** (Artificer's Epiphany + 2 Island) | KEEP | kept | ok |

The one no-red opener (vs110: `Island; Cyclonic Rift; Island; Island; Downsize; Prism Ring; Young
Pyromancer`) reasoned the red-source gate explicitly ("relying on drawing a red source later is a
trap") and landed on Mulligan. The mull-to-6 hand had a dig spell (Artificer's Epiphany) + blue to
cast it, so keeping is the STEP-1 exception — correct. **No false keep, no false mulligan.** The
answer-last protocol continues to make the concluded decision land (wave-9 residual stays closed).
The gate is compact, load-bearing, and just produced 7/7 — demoting it is the churn the pause-aware
doctrine forbids. FREEZE verbatim.

## Mountain-first land execution — 12/12. FREEZE.

12 land co-offers (option list containing BOTH "Play Mountain" and "Play Island"): vs133 x2,
vs140 x3, vs109 x1, vs135 x3, vs44 x3. **Chose a Mountain option 12/12** — including 10 windows
where "Play Island" was listed FIRST (the pilot picks by NAME, not index). Now fired-and-obeyed
across FOUR corpora (wave-8 12/12, wave-9 9/9, wave-10 10/10, wave-11 12/12). CLOSED. FREEZE.

## Guttersnipe engine — cast-every-turn OBEYED; NEVER-block obeyed in ALL reasoning.

- **Guttersnipe reached the board in 3 games** this corpus (vs133, vs140, vs109) — more than
  wave-10 — but died FAST each time (vs133/vs140 on the cast turn; vs109 traded in combat, see H1).
  Not a decision defect: it was answered/blocked, a construction fragility (a 2/2 body).
- **Cast-instants-as-fuel OBEYED** where a live payoff existed: the win game cast Downsize +
  Aetherize as token fuel (YP trigger); no fuel-declined leak in any completed reply.
- **NEVER-block-with-Guttersnipe: obeyed in EVERY captured reasoning.** vs109 seq13 (T10, 10 life,
  4 attackers) reasoned it precisely ("Blocking with Guttersnipe risks it being killed... I should
  preserve my Guttersnipe... I will declare no blockers") and emitted `BLOCKS: none`. vs135 seq17/30
  (the win game) both declared `BLOCKS: none`. **The guide rule fired perfectly at the decision
  layer in 3/3 windows.** The vs109 Guttersnipe trade happened DESPITE the correct decision — a
  parse-layer leak (H1), not a guide failure.

## Engine-change firing (brief's confirmed-fired doctrine)

- **Ramble guards** — the one formless/out-of-range cast this seat (vs110 seq17, CHOICE:5 into a
  4-option list) correctly recorded `fallback=unparsed_reply, choice=None` and deferred to the
  heuristic; NO prose-scanned integer cast. Ramble guard behaving as designed.
- **Fetch pre-targeting keying** — N/A: deck131 runs 14 Island + 8 Mountain, ZERO fetchlands. No
  fetch-crack windows at this seat.
- **Legal-target names on targeted casts** — RENDERED and used cleanly. vs135 seq14 `Cast Downsize
  {u} - legal targets right now: Ice-Fang Coatl, Ohran Viper` -> picked Ohran Viper; vs110 seq5
  `Cast Cyclonic Rift {1}{u} - legal targets right now: Mox Opal, Signal Pest`. **No fabricated-
  target class at this seat.**
- **max_tokens 4096** — see the spiral analysis (notes H2): most confusion spirals now CONCLUDE
  correctly; the deepest burn to the 120s HTTP timeout (empty_reply).
- **adj rows** — sanity-checked: vs135 (tsv life1=19, seat last=19) EXACT; vs140 (tsv life0=26
  vs seat last 23; timeout snapshot lag, no gameend for adj) consistent with the known no-gameend-
  for-timeouts behavior.

## Fallbacks — profile FLIPPED to the HTTP-timeout class; none changed the win

14 fallbacks across the corpus this wave: **13 `empty_reply` + 1 `unparsed_reply`** (wave-10 was
6 unparsed + 1 empty). The 4096 cap gave the spirals room; the deepest ones now run PAST 120s and
the HTTP client aborts with an empty body. **All 13 empty_reply have `latency_ms` = 120003-120004**
(the exact 120s HTTP-timeout wall). This is the brief's KNOWN CORPUS-WIDE class (72 corpus-wide,
harness fix WAGIC_GPT_TIMEOUT 240s shipped next corpus, commit 95cf9f5f9). Detail:

- Most empty_reply are trivial low-stakes windows (Elixir/Prism/nothing; Play/Hold Island;
  Aetherize/nothing) where the heuristic pick is fine.
- A few were Aetherize-as-fuel windows in the win game (vs135 seq26 @ opp 9, seq28 @ opp 8) —
  timing out to a heuristic MAY have cost a token each, but the swarm closed anyway (opp -> 1). No
  game impact.
- **NONE changed the win**; none flipped a loss (the losses are construction-bound).

The 1 `unparsed_reply` (vs110 seq17) is a distinct model/reply-protocol defect (notes H3).

## Obeyed-but-losing terminus — NO new guide-addressable class. Construction-bound, 6th wave.

The five losses are the standing construction diagnosis (RESTATED, not relitigated — notes.md):
threat density 6/60, no reach vs lifegain (vs140 lost the cap by 3 with a single burn spell it
does not run), Guttersnipe's 2/2 body dies to anything (3/3 games it hit the board). The guide
plays correctly and loses; wins come from CONSTRUCTION (as the vs135 win — the deck happened to
draw and hold two Young Pyromancers + never needed Guttersnipe). No decision-level failure class
the guide can address emerged this wave.

## Routing summary
1. **[GUIDE — FREEZE, byte-identical]** strategy.txt = the live wave-10 frozen guide, 9036 bytes,
   `diff` clean. Every targeted line validated; the WIN vindicates the YP-swarm line; no
   guide-addressable defect. Churning obeyed lines off this corpus is the anti-pattern the doctrine
   forbids.
2. **[HARNESS/PARSE — new, game-relevant]** H1: bare `BLOCKS: none` is systematically unparsed
   (4 instances corpus-wide), only `B<n>:none` parses (3 instances) — the natural all-decline
   reply falls to the baka blocker heuristic, which at vs109 blocked Guttersnipe into a fatal trade.
   Root fix: accept bare `BLOCKS: none` / "no blockers". notes.md, NOT the guide.
3. **[HARNESS/MODEL — known class]** H2: the mana/confusion spiral under 4096 — most now conclude
   correctly; the deepest burn to the 120s HTTP timeout (empty_reply). Known corpus-wide; 240s fix
   shipped next. notes.md.
4. **[REPLY-PROTOCOL — 2nd witness, WATCH]** H3: vs110 seq17 plan fixates on an UNLISTED card
   (Young Pyromancer, not in the option list) and emits an OUT-OF-RANGE `CHOICE: 5` (4 options).
   2nd witness of wave-10 deck110 P3. Reply-protocol layer. notes.md.
5. **[REPRESENTATION/rules-gap — single-seat, WATCH]** H4: vs109 seq16 blocked when NOT lethal
   (7 life, survives at 1 either way) via a FIRST-STRIKE combat-math error (claimed YP kills Ash
   Zealot; first strike kills YP first). Instance of the standing reflexive-blocking convergence
   class (REPRESENTATION-routed, core rejected 5x) + a first-strike knowledge gap. notes.md.
6. **[DECK construction — unchanged, dominant, RESTATED not relitigated]** notes.md.
7. **[CORE — PASS]** no 2+-seat core candidate at this seat. general-suggestions.md = PASS.
8. **[WATCH — single-seat]** Guttersnipe attack-trade (vs109, 2nd instance after wave-10 vs135);
   still no attributable pilot attack-decision record. notes.md; below the bar to touch the guide.
