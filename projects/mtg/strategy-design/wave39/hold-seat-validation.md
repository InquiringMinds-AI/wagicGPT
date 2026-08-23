# Wave-39 hold-seat validation (seats 139 / 36 / 152 / 146) + anomaly-D classification

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-040744/` — binary **eabf463b1**,
pilot `qwen36-35b-a3b`, 42 seat logs, **1,436 decision records, 8 fallbacks (0.56%)**.
20/21 games ended naturally; **139v125 CRASHED at t15** (Affinity SBA `__dynamic_cast`,
separately docketed in `wave39/crash/`) — its two seat logs carry NO `gameend` record and
its `results.tsv` row (`adj1`) is bogus. Data from that game is used below only up to the
crash, as decision evidence.

Method: RESOLUTION-VERIFICATION (every claim traced to a RESULT event — a rendered string,
a `chosen_text`, or a battlefield arrival in a later prompt's narration), and
ARRIVAL-TRACING (a wave-38 fix counts as LIVE only where its rendered string is found in a
wave-39 prompt). All searches via `/usr/bin/grep` + `python3` over the raw `.jsonl`.

---

## A. Seat 139 — mutate role headers + host-intent carry (wave-38 lane `5ea04c6ba`) — **VALIDATED**

The wave-38 lane made three falsifiable predictions. All three hold.

### A1. Role headers are LIVE (wave-37: both dead) — CONFIRMED

`MUTATE PLACEMENT` appears in **10** wave-39 prompts; `MUTATE - CHOOSE THE HOST` in **8**.

| seat log | MUTATE PLACEMENT | CHOOSE THE HOST | carry line |
|---|---|---|---|
| deck139 vs deck125 | 1 | 1 | 1 |
| deck139 vs deck146 | 2 | 1 | 1 |
| deck139 vs deck152 | 2 | 2 | 2 |
| deck139 vs deck162 | 3 | 3 | 2 |
| deck139 vs deck126 | 2 | 1 | 1 |
| **total** | **10** | **8** | **7** |

Zero generic-wording regressions: a scan of every prompt whose option list contains a
`N. mutate over` / `N. mutate under` line found **0** without the `MUTATE PLACEMENT`
header (wave-37's failure mode — the capitalized-only compare against the
parser-lowercased label — is dead). Citation, 139v125 seq 22 (t12):

> `MUTATE PLACEMENT for Everquill Phoenix: ... Choose, and state in a PLAN: line which`
> `creature you intend as the HOST - the host is picked at the NEXT ask, and your plan is`
> `shown there.`
> `1. mutate over [this card goes ON TOP: ...]`  `2. mutate under [...]`

The **10 → 8** gap is correct-by-design, not a dropped header: 139v146 seq 24 and
139v126 seq 17 each had exactly ONE legal host on the battlefield, so no model call was
made (single-outcome rule). Both were arrival-traced anyway — see A3.

### A2. The PLAN-carry renders back at the host ask — CONFIRMED, 7/8

7 of the 8 host asks carry the commitment line. Citation, 139v152 seq 35 (t10):

> `... Pick the ONE creature to mutate onto, and answer with its name. At the over/under`
> `step your PLAN named Dryad of the Ilysian Grove as the host - you may still pick`
> `differently, but that was your stated intent.`

The one host ask WITHOUT the line (139v162 seq 36, Illuna) is the designed negative: the
plan stated at seq 35 named *Brokkos, Migratory Greathorn and both Pollywogs* — several
distinct candidates, so `uniqueNamedIn` correctly returned empty and no carry rendered.
The ambiguity guard fires live, exactly as its PARSETEST negative asserts.

### A3. Intent-mismatch count where the carry rendered: **0** — CONFIRMED

Every host pick matched its carried intent, and every merged pile ARRIVED on the named
host (traced through the `[mutated pile — ... N card(s) underneath: X]` render in the
next prompt of the same seat log):

| seq (placement→host) | carried intent | picked | pile underneath (arrival) |
|---|---|---|---|
| 139v125 s22→s23 | Arboreal Grazer | Arboreal Grazer #1 | `card underneath: Arboreal Grazer` ✓ |
| 139v146 s15→s16 | Arboreal Grazer | Arboreal Grazer #1 | `card underneath: Arboreal Grazer` ✓ |
| 139v152 s16→s17 | Dryad of the Ilysian Grove | Dryad of the Ilysian Grove | `card underneath: Dryad of the Ilysian Grove` ✓ |
| 139v152 s34→s35 | Dryad of the Ilysian Grove | Dryad of the Ilysian Grove #2 | `card underneath: Dryad of the Ilysian Grove` ✓ |
| 139v162 s17→s18 | Dryad of the Ilysian Grove | Dryad of the Ilysian Grove | `card underneath: Dryad of the Ilysian Grove` ✓ |
| 139v162 s26→s27 | Arboreal Grazer | Arboreal Grazer #1 | `card underneath: Arboreal Grazer` ✓ |
| 139v126 s30→s31 | Everquill Phoenix (UNDER) | Everquill Phoenix | `cards underneath: Migratory Greathorn, Migratory Greathorn` ✓ |
| 139v162 s35→s36 | *(no carry — ambiguous plan)* | Pollywog Symbiote #1 | `card underneath: Pollywog Symbiote` ✓ |
| 139v146 s24 (single host) | Overgrown Tomb (plan) | *auto* | s25: `Everquill Phoenix (7/7) ... 1 card underneath: Overgrown Tomb` ✓ |
| 139v126 s17 (single host) | Everquill Phoenix (plan, UNDER) | *auto* | s18: `Everquill Phoenix ... 1 card underneath: Migratory Greathorn` ✓ |

**10/10 mutate casts landed on the intended host; over/under intent honoured in both
directions** (139v126 s30 is a clean UNDER: Greathorn goes beneath, the pile keeps
Everquill's name and 4/4 — the exact wave-37 failure shape, now correct).

**Verdict A: VALIDATED.** Not a null — the class was exercised 10 times across 5 games.
The wave-37 docket item "mutate host-intent carry" can be CLOSED.

---

## B. Seat 36 — wave-38 ALord/AAMover SEGV fix (`5e6e4ccf9` / `7f33d0dc7`) — **VALIDATED**

### B1. Crash class absent — CONFIRMED

All six deck36 games completed naturally with `gameend` records on BOTH seats
(36v162 t8, 152v36 t12, 36v126 t12, 139v36 t9, 146v36 t14, 36v125 t20). Corpus-wide, 20
of 21 games carry paired `gameend` records; the sole exception is **139v125**, whose
stack is the Affinity/`gameStateBasedEffects` crash — a **different** signature from the
`ALord::_added` → `AAMover::resolve` → `Player::opponent()` SEGV this fix targets. Zero
`Segmentation fault` / `core dumped` strings in any of the 21 `.stderr` files.

### B2. Positive control — the exact crashing precondition was EXERCISED and survived

The wave-37 crash needed Elite Spellbinder (or Check for Traps / Grief / Humiliate) to
resolve **while the opponent's hand held no nonland card**, driving the
`all(opponenthand)` reveal clause. Card location check: Elite Spellbinder ×4 is in
**deck152** (not deck36 — the w37 crash was the 152 seat casting into the 36 seat);
Humiliate in deck146 is a `#SB:` sideboard line and never entered play.

**Repro found — 152v139 seat log, seq 20, turn 9:**

> `Opponent hand size: 0 | Opponent library: 44 cards`
> `2. Cast Elite Spellbinder {2}{w} (3/1) {card text: "Flying -- When Elite Spellbinder`
> `   enters, look at target opponent's hand. ..."}`
> reply: `CHOICE: 2 (Cast Elite Spellbinder)`

Narration at seq 21: `- Your Elite Spellbinder resolved and entered the battlefield`,
and the board line shows it in play. The game continued to a natural end at t10. **This
is the empty-hand reveal branch that segfaults on 5104a2433, resolving cleanly on
eabf463b1.** Not a lucky miss — the precondition was met head-on.

The non-empty branch was also exercised (152v126 seq 21-22): `- You targeted Sanguine
Bond ... with Elite Spellbinder's exile a non-land card ability`, resolved, game natural.
So both `auto=` clauses of the corrected script ran live.

**Verdict B: VALIDATED** (crash class absent AND the precondition exercised twice).
Spellbinder/`all(*|opponenthand)` behaviour: **exercised and correct**, not a null.

---

## C. Seats 152 / 146 — wave-37 new teaches, second corpus

### deck152 (6 games; 8 blockers asks, 20 attackers asks)

| teach | verdict |
|---|---|
| Trade-drift (restate-then-override the parenthesis) | **VALIDATED — 0/8** |
| RETURN-SWING check | **VIOLATED 1 of 2 armings** |
| Blocks-none default (value-poor chump) | **VIOLATED ×1** |
| Katilda-vs-infect precedence | **NULL — not exercised** |

**Trade-drift: 0 occurrences in 8 blockers asks.** Every declared assignment agrees with
its own parenthetical; no reply restates a `(your blocker dies, attacker lives)` line and
then argues past it. Favourable-parenthesis blocks were taken (152v36 s7 `B1:A1 (you kill
it, your blocker lives)`; s35 `B1:A3 (you kill it...)`; 152v146 s25 `B2:A3 (both die)`)
and unfavourable ones declined (152v139 s17, 152v146 s12). Target 0 met, second corpus
running.

**RETURN-SWING: armed twice.**
- **FOLLOWED — 152v36 seq 40 (t12).** Life 5; their biggest is Master of Etherium 8/8
  → armed (5 ≤ 8). Answer: `ATTACK: A1..A7` (all seven). Correct: the *lethal-now
  exception* applies — opponent at 4 life, both their creatures `[tapped]`, incoming
  4+2+9+3+3+3+3 = 27 unblockable. Game ended that turn, opponent at −24, 152 WON. The
  exception clause is doing real work, not just the gate.
- **VIOLATED — 152v139 seq 21 (t9).** Life **4**; their biggest is Illuna 6/6 → armed
  (4 ≤ 6). Opponent at 17, sole attacker Wolf 3/3 → **not** lethal, so the exception does
  not apply and the guide's explicit clause covers it verbatim: *"even when it is your
  ONLY listed attacker ('ATTACK: none' is the right answer then), even when their board
  'cannot block this turn'"* — the prompt showed Illuna `[tapped - cannot attack or block
  this turn]`, the precise decoy the teach names. Answer: `ATTACK: A1` +
  `PLAN: Attack with Wolf for free damage`. Consequence traced at **seq 22 (t10)**: Wolf
  is `[tapped - cannot attack or block this turn]` on the blockers ask, 11 damage
  incoming at life 4, only Elite Spellbinder available; the seat died that combat.
  *Honest scoping*: with one 3/3 against 6-trample + 4 + 1 the block would not have saved
  it either — the violation is of the RULE, not provably of the game. But this is the
  teach's exact scenario answered wrong, with the phrase the guide pre-emptively bans
  ("free damage") in the plan.

**Blocks-none default: 1 violation.** 152v139 seq 11 (t6, life 20, forecast "you would be
at 13 — NOT lethal"): `BLOCKS: B1:A2` put Luminarch Aspirant (1/1) in front of Pollywog
Symbiote for `(your blocker dies, attacker lives)` — a chump that saves **1** damage at
life 20 and throws away a key threat. The guide's rule is *"Answer 'BLOCKS: none' and take
the damage unless a listed block trades up, saves your life this combat, or stops poison"*
— none of the three. Not trade-drift (the parenthesis was obeyed, not overridden); this is
the wave-37 "value-poor chump" WATCH item **recurring in a new shape**, so the watch is now
a confirmed n=2 class across two corpora.

**Katilda-vs-infect: NULL.** Zero infect/poison in the wave-39 pool (deck105 rotated out);
the precedence sentence had no occasion. Second consecutive corpus un-exercised.

### deck146 (6 games; 6 blockers asks, 21 attackers asks)

| teach | verdict |
|---|---|
| HOLD-BACK GATE (their total power ≥ your life → ATTACK: none) | **NULL — never armed** |
| Chump anchor ("you would be at" ≤ 10 → block) | **VALIDATED — 2/2 fired-and-followed** |
| "I take the damage either way" ban | **VALIDATED — 0 occurrences** |
| MAIN-2 pump anti-pattern | **VALIDATED — 0 recurrences, 2/2 pump casts in MAIN 1** |

**Hold-back gate: NULL.** All 21 attackers asks were computed for arming (sum of every
opposing creature's current power, tapped included). The gate never armed — the highest
their-total-power reached opposite this seat was **3** (Fate Unraveler 3/4 in the vs162
game) against life ≥ 8, and vs36 / vs126 / vs125 the opposing boards were Ornithopter/Pest
0-power chaff, 0/4 walls, or literally creatureless. Wave-37's 1/1 arming was the whole
prior evidence base; wave-39 adds none. **No violations either** — no attack was sent into
an arming board. The teach remains unfalsified but under-tested; it needs a beefy-board
opponent (139 or 152 mid-game) to get a real reading.
*Adjacent, correct*: 146v139 seq 19 answered `ATTACK: none` with a 3/2 into an untapped
3/3 vigilant blocker — the right call under the #2-rule "dies for nothing" clause, not a
gate firing.

**Chump anchor: 2/2 fired and followed** (the only two asks whose forecast was ≤ 10):
- 146v162 seq 33 (t11), life 3, `you would be at 0 - LETHAL`. Answer
  `BLOCKS: B1:A1, B2:none, B3:none` — Spider 2/1 in front of Fate Unraveler 3/4 for
  `(your blocker dies, attacker lives)`. Exactly the anchor's teach: a non-trampler blocked
  by anything deals ZERO, so the 2/1 buys the whole 3 life. Survived at 3.
- 146v139 seq 20 (t11), life 13, `you would be at 3`. Answer `BLOCKS: B1:A1` — Silencer
  onto the biggest attacker it is legally allowed to block (A2 Everquill Phoenix 7/7 is
  **flying**; B1 has no evasion answer, so A1 was the biggest reachable non-trampler).
  Anchor followed under its real constraint.

All four >10-forecast asks handled without drift (146v162 s25 declined a
blocker-dies line at forecast 16; 146v152 s23 took `(you kill it, your blocker lives)`).

**"Either way": 0 occurrences** corpus-wide in deck146 replies. Weaker evidence than
wave-37 (this pilot's replies are terse — CHOICE + PLAN, no visible reasoning), so read
this as "no self-derived use", not as proof the anchor is internalized.

**MAIN-2 pump: 0 recurrences.** Three Silverquill Command casts, all in the vs125 game:
- seq 53 (t34, **MAIN 1**): plan *"Pump Nadaar and return Triumphant Adventurer, attack
  with both"*; modes `creature gains 3/3 and return creature`; attacked same turn. ✓
- seq 72 (t38, **MAIN 1**): plan *"pump Barrowin (+3/+3 flying) ... Attack with Barrowin
  and Goblin"*; arrival-traced at seq 77's attackers ask — **Barrowin (6/6)** attacking
  (printed 3/3 + the pump), same turn. ✓
- seq 24 (t20, MAIN 2): the ONE main-2 cast, and it **did not take the pump mode** — the
  plan named the return + draw modes; the spell was countered before mode selection. Not
  an instance of the anti-pattern.
No "pump now, attack next turn" plan anywhere. Class stays dead across two corpora.

---

## D. Anomaly — `validation_reject_reask_exhausted` ×5 — CLASSIFIED

### Where

All 5 are in ONE seat log, ONE turn, ONE phase, consecutively:
`1787476069-ai_baka_deck125-...-vs-ai_baka_deck139.jsonl`, **seq 11–15, turn 5, Main
phase 2**, `kind: defer`, `choice: -1`, `chosen_text: "Sphinx's Revelation"`,
`options: 2`, `latency_ms: -1`, `prompt: ""`, `reply: ""`.

They are the **same five records** as status.md item 2 ("5 consecutive defers, deck125
seat, turn 5 seq 11-15") — one anomaly, not two. `latency_ms -1` + empty prompt means
**no model call happened at any of the five**: they are local rejects replaying a cached
answer, not a churning ask.

### Ask shape

**Not a menu, not blocks, not targets — the CASTING decision**, and specifically an X-spell
with **zero X slack**. Preceding record seq 10 is the real model call:

> Mana available: **3 total** ... `Island {u}; Glacial Fortress {w} or {u}; Plains {w}`
> Hand: ... `Sphinx's Revelation {u}{u}{w}{x} [instant]`
> `1. Cast Sphinx's Revelation {u}{u}{w}{x} {card text: "You gain X life and draw X cards."}`
> `2. Cast nothing right now`
> reply: `CHOICE: 1 (Cast Sphinx's Revelation)` / `PLAN: Cast Sphinx's Revelation for X=1`

Three mana pays exactly `{u}{u}{w}`, leaving **X = 0** — "gain 0 life and draw 0 cards".
(The model's own plan says X=1; it cannot afford it. Contrast seq 18–19 at t8 with 4
floating mana, where the same cast validated and the announce menu offered `1. X = 1`.)

### Mechanism

The DIVERGENCE-C loud re-ask in `AIPlayerGPT::FindCardToPlay`
(`projects/mtg/src/AIPlayerGPT.cpp`, the `for (int attempt = 0; ; attempt++)` loop,
≈L8060–8150; the exhaustion write is **L8135–8150**):

1. The cheap cast-menu filter offers Sphinx's Revelation (it is castable at X=0).
2. The pick is priced through `AIPlayerBaka::FindCardToPlay(pMana, "*")` with
   `aiForcedCandidate` set. In `AIPlayerBaka.cpp` **≈L3336–3368** the X-slack penalty runs
   `shouldPlayPercentage - (shouldPlayPercentage * 1.9f) / (1 + xDiff)`; with
   `xDiff == 0` that is `P − 1.9P` = **negative**, and the roll gate
   `if (shouldPlayPercentage > 0)` never zeroes a negative, so `chance > shouldPlayPercentage`
   is true for every roll and the candidate is `continue`d. `validated == NULL`.
   *This is the same arithmetic as the documented deck137 silent-abort signature* — the
   comment block at L3336 names "an X spell with zero slack" as one of the three witnessed
   DIVERGENCE-C trigger shapes. The reject is substantively RIGHT (an X=0 Revelation does
   nothing); it is the OFFER and the CHURN that are wrong.
3. `lastChance` is true at attempt 0 because `candidates.size() <= 1` — Revelation was the
   only castable card. So the class jumps straight to `..._reask_exhausted`; there is no
   intermediate `validation_reject_reask` record, which is why the corpus shows 5
   exhaustions and 0 plain rejects.
4. **The churn**: the rejected line is *deliberately* not added to `mStuckCastLines` (see
   the comment at ≈L8078 — doing so would change the next tick's full menu and turn a cache
   hit into a fresh HTTP call). So on the next priority poll the identical menu is rebuilt,
   `askModel` replays the cached `CHOICE: 1` (`latency_ms -1`), validation rejects again,
   and another exhaustion record is written. The loop only ends when the phase advances —
   here, five polls.

**Cost**: 5 wasted priority windows, 0 tokens, no wrong play (the heuristic answered and
also cast nothing). **Harm**: the model is never told; the log looks like a 5× repeated
failure; and the model's stated plan ("for X=1") shows it had already mispriced the spell
from a surface that offered an unaffordable-in-practice cast.

**The status.md hypothesis (c3 staleness gate churning on a new-deck menu) is WRONG** —
the c3 gate is not involved, no menu is stale, and no new-deck menu shape is implicated.
Clusters on deck125 only because deck125 is the only pool deck with an X spell.

**Not a code change here** (per brief) — ledger items D1/D2 below.

### Two other fallbacks, for the record

- `all_assignments_illegal` ×1 — 146v36 seq 22 (t13): answered `BLOCKS: B1:A1` where the
  option line offered B1 only `A2`/`A3` (A1 Ornithopter has **flying**; Nadaar does not
  reach). Guard caught it, heuristic answered. The blocker line states which attackers a
  blocker MAY block but never says WHY the others are absent — an evasion tag on the
  attacker line (`A1 ... [flying — your ground blockers cannot block it]`) is the
  perception-lane fix shape.
- `unparsed_reply` ×2 — both 146v152: seq 8 (t4, priority) replied in prose with no
  `CHOICE:` line; seq 36 (t16, priority) replied `ATTACK: Nadaar, Selfless Paladin, Goblin`
  — the **attackers protocol leaking into a priority ask**. n=2, self-healing, watch only.

---

## Engine-ledger candidates for wave-40 (ranked by severity)

1. **[HIGH] Affinity SBA crash (139v125 t15)** — already the top docket item in
   `wave39/status.md`; restated here only because it is the corpus's one completeness loss
   and blocks the 139v125 rerun. Not investigated by this pass.
2. **[HIGH] D1 — X-spell zero-slack casts are OFFERED then rejected.** The cast menu's
   601.2c-style filter should also drop an X spell whose *announceable* X is 0 and whose
   scripted effect at X=0 is a no-op — price it with `ManaEngine::maxAnnounceableX` at
   menu-build time (`AIPlayerGPT.cpp` candidate loop ≈L8000–8055), the same number
   `AIPlayerBaka.cpp` L3362 already uses to reject it. Removes the false offer at the
   source and stops the model planning around an X it cannot pay (the seq-10 plan said
   "for X=1" with 3 mana). Severity: burns whole priority windows on a control deck whose
   entire plan is X-spell sizing.
3. **[HIGH] D2 — exhaustion should stop the churn.** When `lastChance` fires (re-ask
   budget spent, or `candidates.size() <= 1`), add the rejected line to `mStuckCastLines`
   for the remainder of the turn (`AIPlayerGPT.cpp` ≈L8135–8150). The comment's
   cache-stability objection applies to the *intermediate* re-ask, not to the give-up: once
   the window is conceded to the heuristic there is no answer left to protect. Turns a 5×
   repeat into 1 record. Cheap, contained, and independently useful even if D1 lands.
4. **[MEDIUM] Mutated-pile rules text shows the WRONG card's text.** Board and target
   option lines render a merged pile as `<top card name> (P/T) [combined keywords] -
   "<BOTTOM card's rules text>"`. Two clean citations: 139v162 seq 27 offers
   `Migratory Greathorn (3/4) [mutate] - "You may play an additional land on each of your
   turns..."` (that is **Dryad's** text; Greathorn is on top) and 139v162 seq 36 offers
   `Brokkos, Apex of Forever (6/6) [trample, reach, mutate] - "Reach -- When Arboreal
   Grazer enters..."` (**Grazer's** text). The `[mutated pile]` annotation truthfully
   promises "the combined abilities of every card in the pile", then the quoted text shows
   one card's — and not the named one. Trust-doctrine violation (a true statement in the
   wrong scope). Fix shape: render the pile's text as the concatenation the annotation
   already promises, or attribute it (`"<name>: ..."` per card).
5. **[MEDIUM] Blocker lines do not say why an attacker is unblockable by a given blocker.**
   `all_assignments_illegal` (146v36 s22) came from the pilot assigning a ground 4/4 to a
   flier. The attacker line carries `[flying]` but the blocker line's "may block A2, A3"
   is the only negative signal, and the model read past it. Add the reason to the attacker
   line, or an explicit `(none of your blockers can block A1 — flying)`. Perception lane,
   not guide prose.
6. **[LOW] Priority-ask protocol leakage.** `unparsed_reply` ×2 in one seat log, one of
   them an `ATTACK:` line answering a priority ask (146v152 s36). n=2, self-healing. Watch;
   revisit if it reaches n≥4 in one corpus.
7. **[LOW / guide lane, not engine] deck152 value-poor chump, now n=2.** Wave-37 flagged
   the vs139 s23 Katilda chump as a WATCH; wave-39 adds 152v139 seq 11 (Luminarch Aspirant
   chumped into a 1/3 at life 20 to save 1 damage). Two corpora, two shapes → the
   blocks-none default needs an explicit no-chump-above-life-N scoping line in
   `deck152_strategy.txt`, and deck152's freeze should carry this as its named watch item.
8. **[LOW / guide lane] deck152 RETURN-SWING needs a sole-attacker restatement.** The
   violated instance (152v139 s21) is exactly the "your ONLY listed attacker" clause the
   guide already spells out — so this is a *salience* failure, not a coverage gap. Consider
   moving the sole-attacker sentence out of the paragraph tail and into the arming test
   itself, where the model is already counting.
9. **[INFO] deck146's hold-back gate is under-tested.** Two corpora, one arming total. Not
   a defect; a note for whoever adjudicates 146's freeze — the gate cannot be called
   validated on n=1, and the wave-39 pool (0-power walls, artifact chaff, creatureless
   control) structurally cannot arm it. If a reading is wanted, it needs 146 seated against
   139/152 mid-game boards, not more games against 125/126/162.
