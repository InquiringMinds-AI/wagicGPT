# Wave-82 step one — Lane EC (combat verdicts, the P5 narration register, library/mill facts)

Branch `w82-lane-EC` in `worktrees/lanes/w82-EC`, base master `35e1e82dc`. Nine commits, one per item,
in brief order: `f428f6869` H3, `ff0b8350a` H4, `34d7c3605` H5, `3c798eac2` M1, `567319d1e` M2,
`6cade2221` M7, `7c61b012c` M8, `ca66ecb44` M6, `7d5a89381` M11. Files touched: `src/AIPlayerGPT.cpp`,
`src/AIPlayerGPTSeams.cpp`, `src/AIPlayerGPTSelfTest.cpp`, `include/AIPlayerGPTInternal.h`,
`include/AIPlayerGPTSelfTestAccess.h` (5 files, +1201/-101). No engine file, no primitive, no guide,
no pilot config; `git diff | grep -c U+FFFD` = 0; working tree clean.

## Gate (worktree binary, after the ninth commit)

| gate | result | baseline |
|---|---|---|
| PARSETEST (`WAGIC_GPT_PARSETEST=1 ./wagic`, systemd-run scope, `~/.gatelogs/w82-EC-parsetest-final.log`) | **7385 passed / 0 failed** (+71 `#W82-EC` cases) | 7314 / 0 |
| Suite THREADS=1 (unit `w82-EC-suite`, `~/.gatelogs/w82-EC-suite.log`) | **1314 tests (0 failed), 82 AI tests (0 failed)**; `==Test Failed !==` 0, `==Test timed out` 0 — the known flake `intrepid_adversary_repeated_payment` passed this run | 1314 / 82, 0 failed |
| link-time guards (ctor-init, reply-instructions) | OK on every build | — |

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260917-191141-final`. "Renders now" below is the pinned
PARSETEST expectation for the repro's board (the live composition was not re-run — no corpus from a lane).

## Per item

### H3 — marked damage in every 1-on-1 verdict, and on the body (`f428f6869`)
- **False** (`…deck130-…-vs-…deck123.jsonl` seq72): log `Starstorm -> 4 damage to Rorix Bladewing`; battlefield
  `Rorix Bladewing … (6/5) … [untapped]`; A-line `[their untapped blockers: … Vampire #1 (4/4) (you kill it, your
  attacker lives)]`. Rorix had 1 toughness left; the 4/4 kills it.
- **Now**: `CombatTradeStat.remaining` = `Damageable::life` when `0 < life < toughness` (0 = undamaged);
  `combatTradePreviewStats` decides lethality against remaining on BOTH sides (wither/counter arithmetic still
  over printed toughness). Same pairing with 4 marked → `both die`. Tag on the battlefield line and on every
  attacker/blocker row: ` [4 damage marked this turn - 1 more damage kills it]`. Undamaged / dying / over-full
  bodies print no tag (undamaged 6/5 vs 4/4 stays byte-identical to wave 81).
- **Pins**: `#W82-EC H3 KEY hold-latch / option-set / ask tail` — two rows differing only in the tag yield identical
  hold-action, option-set and ask/async keys (`CHECK(rowA != base)` guards the pin is real).

### H4 — lifelink priced on the A-line with its owner and both branches (`ff0b8350a`)
- **False** (`…deck162-…-vs-…deck152.jsonl` seq11): `A2. Intrepid Adversary (6/4) deals 6 [lifelink]` bare; the
  gain only in `B1 … {… they gain 6 from this block only, …}` → read as "declining avoids the gain", `BLOCKS: none`.
- **Now**: A-line tag ` [lifelink: THEY gain 6 if it connects, blocked or not - a block only changes who takes the 6;
  only prevented damage stops the gain]` (double strike: per damage step it deals in; 0-power: says it gains
  nothing). B-row brace: `their attacker's lifelink, this block: they gain 6, and this attacker deals nothing to
  your life` — `from this block only` is gone (five older expectations re-keyed to the new brace).
- **Pin**: `#W82-EC H4 KEY` — the tag is outside hold, option-set and ask keys.

### H5 — own-library deck-out countdown; mill-on-cast priced on the row (`34d7c3605`)
- **Missing** (`…deck125-…-vs-…deck50.jsonl` seq305): own line bare `Your library: 11 cards` while the opponent line
  carried `DECK-OUT IS IN RANGE`; seq282 option 12 promised `13 left` with four Memory Erosions on their board
  (library 5 at seq283).
- **Now**: one pure `deckOutCountdownClause` shared by both lines; draw-step size = 1 + fixed extras (Howling Mine
  etc.), range = three draw steps' worth, the three engine exceptions (own CANTLOSE / CANTMILLLOSE, their CANTWIN)
  block it. The s305 board renders: `Your library: 11 cards - DECK-OUT IS IN RANGE: … you have 11 cards left and you
  draw 4 per draw step (the draw step itself + Howling Mine #1 (1) + Howling Mine #2 (1) + Howling Mine #3 (1)), so
  you can survive at most 2 more draw steps and the draw step after that loses you the game (any extra draw or mill
  of yours makes it sooner, never later)`. Opponent line at one draw per step is byte-identical to wave 68.
  Cast row: ` {library: 13 -> 5 after Memory Erosion x4 - casting this mills you 8 before it resolves}`
  (`castTriggerMillScan` over `@movedTo(*|opponentstack/|mystack)…deplete:N` hooks; the s282 X row prices its
  draws against the post-mill library: `… 13 -> 5 - then this draws 12 of your 5 library cards - 0 left, which is
  7 MORE than the library holds`). Nothing milling / unknown library → no clause, X row byte-identical.
- **Pin**: `#W82-EC H5 KEY` — the mill clause is outside hold, option-set and ask keys; library countdown numbers
  live on the library line, which was never part of a key (the opponent clause has sat there since wave 68).

### M1 — a collapsed damage batch ends with its final total (`3c798eac2`)
- **False** (`…deck125-…-vs-…deck162.jsonl` seq229): `Underworld Dreams -> 1 damage to you (now 30) [x12 - … only the
  numbers in them differ]`, seat at 19.
- **Now**: `narrationBucketRuns` folds a same-source→same-player run: raw `- Opponent's Underworld Dreams dealt 12 x 1
  damage to you, 31 -> 19`; compact `Underworld Dreams -> 12 x 1 damage to you, 31 -> 19`. Unequal hits state the
  total over the count; a trajectory that does not match the sum says other life moved in between. Two handles of one
  name never fold; creature damage is not a life trajectory; the fold is idempotent; `only the numbers` gone.

### M2 — actor prefix on a cast by the seat that does not own the turn (`567319d1e`)
- **False** (`…deck146-…-vs-…deck125.jsonl` seq230; deck123 seq62): `cast Emeria's Call; cast Cancel; your Emeria's Call
  was COUNTERED by Cancel`.
- **Now**: `… ; opp cast Cancel; your Emeria's Call was COUNTERED by Cancel …` in your turn; `you cast X` in theirs;
  the turn owner's casts stay bare and still fold their payment; resolution/ETB folds bind through `State::isCastOf`.

### M7 — a turn with a Draw block carries no draw on its turn line (`6cade2221`)
- **False** (`…deck162-…-vs-…deck50.jsonl` seq15): `T13 (opp): drew.` then `Draw: … drew 6`.
- **Now**: `T13 (opp):\n  Draw: drew; opp put a card from their hand into their library; drew 6; …` — the hoisted
  draw moves to the front of the Draw group; a turn with no Draw block keeps its hoisted draw byte-identically.

### M8 — damage to a creature carries its outcome (`7c61b012c`)
- **Missing** (`…deck123-…-vs-…deck130.jsonl` seq75): `Starstorm -> 4 damage to Rorix Bladewing` — survived, unsaid.
- **Now**: ` (survives, 4 marked)` / ` (survives)` / ` (dies)` / ` (lethal, but it is indestructible: it survives)`,
  read off the engine after `Damage::resolve`; compact `Starstorm -> 4 damage to Rorix Bladewing (survives, 4
  marked); Starstorm -> 4 damage to Vampire (dies); your Vampire died`. Player-damage lines byte-identical; the
  exact-line bucket and the activation-result fold read through the note.

### M6 — an ability's damage never rides the Attack line (`ca66ecb44`)
- **False** (`…deck130-…-vs-…deck123.jsonl` seq67): `Attack: Siege-Gang Commander, … -> Siege-Gang Commander: 2
  damage, opp 4; … used Deal 2 damage with Siege-Gang Commander -> the opponent;` — Siege-Gang was blocked.
- **Now**: the raw emitter marks a creature's non-combat damage during combat phases (` by its ability (not combat
  damage)`); the compact register keeps it off `Attack:` and folds it onto the activation even when the ping is
  logged first: `Attack: Siege-Gang Commander, Goblin, Goblin -> Goblin: 1 damage, opp 3; they block: Bloodline
  Keeper blocks Siege-Gang Commander; used Deal 2 damage with Siege-Gang Commander -> the opponent: 2 damage, opp 4;
  …`. Declared-attacker combat damage is byte-identical.

### M11 — one non-lethal stack-death face (`7d5a89381`)
- **False** (`…deck146-…-vs-…deck125.jsonl` hold_event seq140+): 44 `verdict_clamped` from `held [you survive the
  stack] over live [nothing lethal on the stack]`, one per Staff of Nin ping.
- **Now**: `stackDeathVerdictKey(1,30) == stackDeathVerdictKey(0,29) == [stack death verdict: nothing lethal on the
  stack]`; `w79HoldVerdictForCompare` returns the live face (nothing to clamp); the hold key does not move when the
  ping resolves; escalation to a lethal stack still re-opens; the rendered line still distinguishes the two states
  off the live loss.

## WEAKEST EVIDENCE
1. Every "renders now" is a pure-function pin over the repro's numbers, not the live composition of the corpus
   prompt — H5's cast-row clause depends on `castTriggerMillScan` finding the `deplete:N` hook in the LIVE card
   script of Memory Erosion; the pin feeds it the string. The next corpus's `{library:` and `DECK-OUT IS IN RANGE`
   counts on the own line are the proof.
2. H3's `remaining` reads `Damageable::life`; if any engine path leaves `life < toughness` on an undamaged body
   (toughness-lowering effects after damage clears), the tag would print a phantom mark. No suite fixture drives it.
3. M6's marker is phase-gated (combat begin..end) — a creature-ability ping in a main phase is unmarked; harmless for
   the Attack line (not in combat) but the raw register's line differs between the two cases. The raw (V1) line
   gained bytes for the in-combat case.
4. H5 changes the OPPONENT line's bytes when extras (Howling Mine, Puzzle Box) exist — the wave-68 line was pinned
   only at one draw per step. Punisher-style "draw on cast" extras are counted only as fixed per-step draws.
5. The suite ran once; the flake happened to pass — no evidence about the flake either way. No A/B, no corpus.
