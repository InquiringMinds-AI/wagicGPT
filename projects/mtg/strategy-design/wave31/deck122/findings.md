# deck122 (Eternal Life -- WB lifegain / Felidar alt-win / Sorin) -- wave 31 findings

SECOND guided corpus, NEAR-TERMINAL EXIT REVIEW. Record **3/6** (down from the wave-30
6/6 first-guided perfecta). Binary cf7cb2e53, corpus `matchups-20260727-025026`, -T 3000,
REPPENALTY=1.05. 176 model decisions at this seat, **2 fallbacks (both
`truncated_abandoned`, both at the blockers seam)**. Guide FROZEN and byte-copied
unchanged (`cmp`-verified).

**VERDICT: EXIT.** No teach is misleading. The 3/6 is pool-shift + variance + THREE
non-seat failures (two harness, one draw/engine option-gap). Details below.

## Record decomposition (3/6) -- from results.tsv

| game | result | life | turn | class |
|---|---|---|---|---|
| 122 v 152 | **WIN (kill)** | 63 / -1 | 18 | clean close, peak 63 life |
| 139 v 122 | **WIN (kill)** | -7 / 37 | 13 | lifelink alpha, Divinity 8/8 |
| 137 v 122 | **WIN (adj1)** | 12 / 16 | 13 | adj-ahead |
| 146 v 122 | LOSS | 16 / -5 | 12 | draw + power mismatch |
| 148 v 122 | LOSS | 20 / -8 | 14 | mull-to-5 + missing cast asks |
| 122 v 158 | LOSS | -16 / 7 | 13 | render contradiction + truncation |

2 real kills + 1 adj-ahead (wave-30 was 2 kills + 4 adj in a 12-adjudication pool; this
pool had only 3 adjudications total). Applying the wave-30 headline-6 adjudication caveat
in reverse: the 6/6 was inflated by the grindy pool, and **the 3/6 is the same deck
without that structural tailwind** -- the kill count is UNCHANGED at 2, the adj-ahead
count collapsed with the pool's adjudication rate (12 -> 3 corpus-wide). Read the drop as
pool composition first, deck158's aggro entry second, and guide quality not at all.

## LOSS DECOMPOSITION (bad draw / bad play / never-had-the-option)

### Loss 1 -- 146 v 122 (t12, -5): DRAW + POWER MISMATCH. No decision fault.
- Mulliganed to 6 (seq1 keep-after-1: 0-lander mulligan is correct; seq2 keep is a
  4-land + Serra + Nighthawk + Felidar six).
- Drew and cast exactly FOUR spells in 12 turns (Serra t3, Blood Artist t5, Suture Priest
  t7, Nighthawk t9). Opponent had 7 creatures + Lolth by t11 with a dungeon-boosted
  Nadaar at (5/5) (printed 3/3).
- **4 of the 6 combat decisions were single-option** (seq7, seq12, seq16 blockers n=1;
  seq19 attackers n=1) -- the seat had no choice to get wrong.
- The two real choices are defensible: seq16 t8 no-block at 21 life vs 11 incoming with a
  lone Suture Priest (the render itself said "NOT lethal: block only where the trade
  favors you"); seq20 t10 Nighthawk-blocks-Nadaar + Priest-blocks-Adventurer is the
  correct must-survive block.
- **Teach deviation (no cost):** seq3 bottomed **Felidar Sovereign** on the mulligan,
  against guide #2's "do NOT bottom Felidar unless the rest of your hand truly cannot
  function". The hand (4 lands + Serra + Nighthawk) could function, so the clause says
  keep. The model's reasoning was sound MTG ("6-mana win condition ... too slow ... does
  nothing to help me reach the necessary thresholds") and the cost was zero: the game
  ended at t12 with the seat at 5 life and a stripped board. Recalibration candidate,
  NOT a misleading teach (see teach table).

### Loss 2 -- 148 v 122 (t14, -8): DOUBLE MULLIGAN + AN ENGINE OPTION-GAP.
- **Mulled to 5** (seq1/seq2 Mulligan, seq3 keep, seq4 bottom 2 Swamps). Never had more
  than 4 lands; Divinity of Pride and two Vampire Nighthawks rotted in hand.
- Opponent's Kor Duelist went (3/1) -> **(10/1) double strike** on stacked equipment;
  deck122 has zero removal. Raw-power loss on a 5-card hand.
- **NEW ENGINE/HARNESS ITEM (N-122b, notes.md):** on deck122's turns **11 and 13** the
  engine offered **no casting decision at all** while `Blood Artist {1}{b}` and
  `Vampire Nighthawk {1}{b}{b}` sat in hand with THREE untapped W/B sources
  (seq16 t11 prompt: `Mana available: {b}{w} from 3 untapped sources`; battlefield
  Isolated Chapel + Scrubland + Plains). stderr `game-148v122-1785147558.stderr`: the only
  deck122 event at Turn 11 is `AIPlayerGPT: declared attack (0 of 1) in one reply`
  (line 729); at Turn 13 only `AIPlayerGPT: Land drop: play Plains now? -> chose 1 of 2`
  (~line 801), then Turn 14 and the loss. A 2-mana Blood Artist that the guide calls the
  drain engine was never offered on the two turns that decided the game.
  **This is never-had-the-option in the strict engine sense, not a seat miss.**

### Loss 3 -- 122 v 158 (t13, -16): the ONLY loss with real decision content -- and its
### two decisive decisions were BOTH taken away from the model by the harness.
Life trajectory: climbed cleanly 20 -> **35 by t9** (Serra deployed as `(6/6) (printed
1/1)` at 31 life, seq21-22) against an RB amass deck. Then:

1. **seq26 t9 blockers, `fallback=truncated_abandoned`, latency 141s, reply 11,266
   chars.** The reply's FIRST LINE is a complete, legal answer:
   `BLOCKS: B3:A2, B1:A1, B2:A3`, and the truncated tail re-converges on the SAME
   assignment ("the plan is to block all three attackers with Soul Warden, Kitchen Finks,
   and Suture Priest respectively"), with the model's own math: net -4 (35 -> 31) plus 3
   Blood Artist drains. The harness discarded it -- stderr line 790:
   `AIPlayerGPT: truncated-abandoned block commit -> safe no-blocks default`.
   Realized instead: **no blocks, 13 damage, 35 -> 22**, opponent left at 14 not 11.
2. **seq30 t10 attackers -- the one genuine model-side gap in the whole seat.** At 24 life
   it attacked with Soul Warden + Suture Priest + Kitchen Finks into a fully tapped
   opposing board, reasoning "Risk: None. I am not exposing myself to combat damage this
   turn because the opponent has no untapped creatures to block." True for THIS combat,
   but it tapped every blocker before the crackback: at seq35 t11 the only untapped bodies
   were two 0/1 Blood Artists and the 15/15 Orc army swing took **28 -> 7**. Had Soul
   Warden / Suture Priest / Finks been home, one chump on the Army absorbs 15 of the 21.
   The guide DOES carry the vs-aggro control posture ("Do not race. Deploy blockers");
   what it does not say -- and what the model failed to re-derive locally, though it
   states the principle correctly three seqs later at seq37 -- is that **attacking taps
   your blockers even when the attack itself is free**. Routed to skill.md as a
   generalizable teach-category rung, not a guide edit (guide frozen, seat exiting).
3. **seq39 t13 -- THE LOSING DECISION, caused by a render contradiction (N-122c, the
   top-priority item in notes.md).** The board line for every declared attacker reads
   `Orc army (19/19) (printed 0/0) [counters: 19x +1/+1] [tapped - cannot attack or block
   this turn] [attacking]`. The model spent ~5,000 characters on the paradox and named it
   verbatim:
   > "Wait, the log says 'tapped - cannot attack or block this turn' but also
   > '[attacking]'. In Magic, a tapped creature cannot attack. If it is attacking, it must
   > be untapped. ... This is a contradiction in the log description."

   and then resolved it the WRONG way:
   > "Given the explicit warning 'tapped - cannot attack', I will assume it does NOT
   > attack. ... So, BLOCKS: B1:A2, B2:A3. This leaves A1 unblocked, but since it's
   > tapped, it deals 0 damage."

   Its answer-first line had been the correct `BLOCKS: B1:A2, B2:A3, B3:A1, B4:A1` (two
   chump blockers on the Army); the paradox-driven self-correction dropped them.
   Arithmetic confirms the contradiction is exactly what killed the seat: 19 (unblocked
   Army) + 2 (Mauhur -- the lone Soul Warden block is illegal vs menace and was correctly
   refused at damage) + 3 (Olog-hai trample over the 1/1) = 24; 8 - 24 = **-16**, the
   recorded life. Chumping the Army with either 0/1 absorbs all 19 and the seat lives at
   4+ with the opponent at 7 and a deathtouch Nighthawk in hand -- the line the model
   itself wrote out before talking itself off it.

**Loss ledger: 0 of 3 losses is attributable to a guide teach.** vs146 = draw/power;
vs148 = mull-to-5 + N-122b option-gap; vs158 = N-122c render contradiction + a truncation
fallback + one guide-gap misplay that cost tempo, not the game.

## TEACH-VALIDATION TABLE (frozen wave-29 guide, re-checked live)

| # | Teach | Verdict | Evidence |
|---|---|---|---|
| 1 | THRESHOLD DISCIPLINE (Serra 30 / Divinity 25; do not race own life down) | **VALIDATED** | Zero self-inflicted drops below a line in 6 games. vs158 seq21-23 t8 holds the line explicitly: "My priority is to maintain the 30+ life threshold to keep Serra Ascendant as a 6/6 flyer", and Serra renders `(6/6) (printed 1/1)` at 31. vs152 monotone 20 -> 63. vs137 dips to 9 (t12) purely on opponent damage, recovers to 21 by t13 on Blood Artist chains. Every life drop in the corpus is opponent-inflicted. |
| 2 | LIFE-AS-ENGINE / the 40-life win axis | **VALIDATED** | Felidar/40 named in the model's own PLAN in 5 of 6 games (per-game "Felidar Sovereign" string counts 17/24/49/59/66/79). vs152 seq19 t9 plans a Suture Priest line specifically "to gain 1 life (reaching 40)". Pursuit present; CONVERSION still nil (see N-122a). |
| 3 | LIFELINK CLOCK is the primary win route | **VALIDATED** | vs139 seq17 t13 alpha with `Divinity of Pride (8/8) [flying, lifelink]` rendered at 29 life (primitives: `this(controllerlife > 24) 4/4`) -> opponent -7. vs152 Serra (6/6) attacks t14/t16/t18 -> 63 life / opp -1. |
| 4 | SORIN chains (+1 every turn) | **UNEXERCISED** (report the null, headline-4 discipline) | Sorin never drawn or cast in any of the 6 games. Validated live in wave 30; per method headline 5 the loyalty-menu surface is a proven shared surface, so this does NOT hold the seat. |
| 5 | BLOOD ARTIST -- never attack with it | **VALIDATED, with 2 sound deviations** | Held everywhere it mattered, including vs158 seq30 where Blood Artist #1 was OFFERED as A4 and left home. Both deviations are vs139 (seq14 t11, seq17 t13) into an EMPTY opposing board on a lethal/near-lethal alpha, with explicit reasoning ("The opponent has 0 creatures ... There is no risk"). A 0-power attacker into no blockers risks nothing; the teach's rationale (keep the drain engine alive) does not bind there. Recalibration candidate only. |
| -- | Felidar mulligan-keep clause | 1 deviation, no cost | vs146 seq3 (above). |
| -- | "Voice-of-the-Blessed sizing" (per the brief) | **NOT APPLICABLE -- card is not in this deck** | `deck122.txt` contains no Voice of the Blessed (the card lives in `primitives/borderline.txt` and is not on the list), and the guide never mentions it. The seat's sizing teaches are Serra Ascendant (primitives `this(controllerlife >29) 5/5` + flying) and Divinity of Pride (`this(controllerlife > 24) 4/4`); both render correctly and both were read correctly. Reported as a brief-side mix-up, not a finding. |

**No teach is misleading. Two absolutes ("never attack with Blood Artist", "do not bottom
Felidar") were relaxed by the model with correct local reasoning and zero realized cost --
the deck131 never-block-absolute pattern.** Under the frozen-guide rule that is an
observation, not a revision trigger.

## N-122a -- Felidar Sovereign win-check: DID NOT FIRE (expected NO, confirmed)

- Felidar Sovereign **never reached the battlefield in any of the 6 games**. It appears in
  hand exactly once (vs146 opening seven) and was bottomed at the mulligan (seq3).
- The 40-life state was reached WITHOUT it for the second consecutive corpus: vs152 hit 40
  at seq19 t9 and peaked at **63** (gameend), passing through five upkeeps at 40+. Had a
  stray Felidar been on board, that one game gave the check five chances to fire.
- The ability exists and is well-formed in primitives (`mtg.txt`):
  `auto=@each my upkeep:this(controllerlife > 39) wingame`.
- **Disposition: N-122a moves to the between-wave probe queue (stacked deck199-style
  probe), exactly as wave 30 projected. It is a probe-deck ledger item and does NOT hold
  the seat** -- two full corpora now show the random pool cannot be relied on to
  co-present the card and the life total.

## Fallbacks at this seat -- 2 truncated_abandoned, SAME shape, BOTH discarded a valid answer

| seq | game | turn | opts | latency | reply | first line emitted | harness took |
|---|---|---|---|---|---|---|---|
| 22 | vs137 | t10 | 3 | 210,738 ms | 11,611 ch | `BLOCKS: B1:A3, B2:A4` | `no blockers` |
| 26 | vs158 | t9 | 4 | 141,086 ms | 11,266 ch | `BLOCKS: B3:A2, B1:A1, B2:A3` | `no blockers` |

Both are multi-attacker BLOCKER decisions; both replies OBEYED the answer-first protocol
(a complete, legal `BLOCKS:` line on line 1); in both, the truncated deliberation
re-converged on the SAME assignment it had already written; and in both the harness threw
the whole reply away for `truncated-abandoned block commit -> safe no-blocks default`
(stderr `game-137v122-1785147987.stderr:1339`, `game-122v158-1785152435.stderr:790`).

**Did the heuristic answer visibly hurt? Yes, measurably, in both:**
- vs137 seq22: the model's own line (Finks -> Emmara, Pridemate -> Flaxen) was worth 3
  Blood Artist triggers and net life 20; the no-blocks default took the damage instead,
  19 -> 14 (seq23). deck122 still won 16-12 by adjudication, so no result change -- but in
  an adjudicated pool, 5 life IS the win metric.
- vs158 seq26: cost 9 life against the model's own computed line (-13 actual vs -4
  planned) and 3 points of opponent life, three turns before dying at -16.

"Safe no-blocks default" is the WORST available default for a lifegain/control deck under
an alpha strike, and here it was applied on top of an already-valid parseable answer.
Routing in notes.md (N-122d).

## Parser / render quality at this seat

- **One decisive contradiction (N-122c)**, quoted verbatim above -- corpus-wide, not
  seat-local: the string `[tapped - cannot attack or block this turn] [attacking]` occurs
  **331 times across the corpus** (49 in deck122-vs-137 alone) and FOUR different seats'
  models verbalized the paradox in-reply (deck122-vs-158, deck152-vs-122, deck152-vs-158,
  deck137-vs-158). This is the wave-30 g4 tapped-tag reword -- validated then on
  non-combat tapped creatures -- failing on the declared-attacker path. Textbook method
  headline 2: a fix is PATH-SCOPED.
- Minor: Clue token annotation renders empty text with a wrong source attribution --
  `Draw 1 with Clue [cost: {2}, Sacrifice] {card text: "() source: Serra Ascendant"}`
  (vs152 seq27/seq28). Cosmetic; the model used the option correctly at seq28.
- Minor: model fabricated a card in its PLAN under pressure (vs146 seq21/seq22 plan "cast
  Ajani's Pridemate" with a hand of three lands). No illegal ACTION resulted -- choices
  are index-selected -- so it is a reasoning artifact, not a seam failure.
- 0 crashes / 0 aborts in both this seat's stderr files -> **N-139h corpus gate PASSED at
  this seat**.
- g1/g2/g3/g5/g6, h4/h5 are all null at this seat (no convoke, tutor/reveal, mutate, loot,
  dungeon, DFC/MDFC, and no planeswalker was cast). Reported as null observations.

## Latency / verbosity profile

| game | n | latency p50 / max | reply p50 / max |
|---|---|---|---|
| vs152 | 39 | 19s / 123s | 1,197 / 8,012 |
| vs146 | 22 | 18s / 203s | 1,123 / 13,283 |
| vs148 | 18 | 17s / 206s | 1,070 / 13,689 |
| vs137 | 41 | 20s / 211s | 1,264 / 11,611 (truncated) |
| vs139 | 17 | 18s / 172s | 1,120 / 11,709 |
| vs158 | 39 | 17s / 151s | 1,355 / 11,266 (truncated) |

Median healthy and stable vs wave 30; the tail is the story. Note that 13,283- and
13,689-char replies parsed FINE, so the two truncations are not a simple character cap but
the token budget landing mid-deliberation on the two longest combat enumerations. The
wave-30 note ("vs137 is the verbosity/latency outlier under aggro pressure") has now
CONVERTED into actual fallbacks in exactly the predicted seam.

## EXIT VERDICT

**EXIT -- rotate deck122 out. Guide frozen and unchanged.**

Gate check:
1. *Teaches still validated where exercised?* Yes -- 4 of 5 exercised and validated; the
   5th (Sorin) unexercised but already validated in wave 30 on a shared surface (method
   headline 5). Two absolutes relaxed correctly by the model at zero cost.
2. *Is the 3/6 a teach failing that the perfecta hid?* **No.** No loss traces to a teach.
   The record delta is (a) the pool's adjudication rate collapsing 12 -> 3, removing the
   structural tailwind wave-30's own taxes audit warned about, (b) deck158's aggro
   replacing deck136's grind, (c) three non-seat failures: N-122b's missing cast asks,
   N-122c's render contradiction, one truncation fallback. Kill count is flat at 2 across
   both corpora.
3. *New guide work available?* One item only -- the "attacking taps your blockers"
   corollary from vs158 seq30. It is a GENERAL teach-category gap, not a deck122 fact, and
   is routed to skill.md so the next control-role guide inherits it. It does not justify a
   rotation slot.
4. *Residuals?* All engine/probe-lane: N-122a (Felidar probe), N-122b (missing cast asks),
   N-122c (attacker tapped-tag contradiction, cross-seat), N-122d (truncation discards a
   valid answer-first block line).

Unsentimental rotation applies: new guide work is exhausted and a variance/pool loss does
not reset the clock. deck122 joins the canary list; the guide is byte-frozen at
`bin/Res/ai/baka/deck122_strategy.txt`.
