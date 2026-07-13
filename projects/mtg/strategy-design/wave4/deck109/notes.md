# Wave-4 deck109 — working notes (catch-all)

Organized by audience. Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260712-192832` (r1)
and `matchups-20260712-230636` (r2); deck109 seat epochs and results tabled in
`wave4/deck109/findings.md`.

---

## For the ENGINE OWNER — two filing-ready bugs + one representation fix

### BUG 1: parseChoice keeps the LAST in-range integer — echoed option text hijacks the choice

**Location:** `projects/mtg/src/AIPlayerGPT.cpp`, `AIPlayerGPT::parseChoice()` (line ~1180).
The plan is split off before parsing (call site ~1310), so the scanned text is the reply's
pre-`PLAN:` part — or the WHOLE reply when no `PLAN:` marker exists (`consumePlan` at ~786
requires the literal colon-marked token).

**Mechanism (confirmed against source, not inferred):** the loop scans every integer in the
text and keeps the last one that falls in `[0, optionCount]`. The in-range guard was added to
fix the OLD last-digit-blind bug (see the comment block), but any echoed mana cost `{2}{r}{r}`,
stat `(3/3)`, or prose life total "life 2" that happens to be in range still wins over the
model's leading choice number. All 8 observed desyncs reproduce exactly:

| record | reply head (verbatim shape) | optionCount | parsed | consumed |
|---|---|---|---|---|
| r1 1783905418 s24 | `1\nCast Pillar ... (player, life 2) to deal 2 damage...` (no PLAN:) | 6 | 2 | own-lethal thrown away — **lost the game** |
| r1 1783908401 s45 | `1. Cast Hellrider {2}{r}{r} (3/3)\n\nPLAN:` | 2 | 2 | "Cast nothing" |
| r1 1783908401 s46/s49/s50 | same echo shape | 2-3 | last in-range | "Cast nothing" |
| r2 1783917446 s27 | `3. Cast Krenko's Command {1}{r}\n\nPLAN:` | 4 | 1 | wrong card |
| r2 1783917446 s29 | `4. Boros Reckoner (3/3) [your battlefield]...` | 4 | 3 | **"Yourself (player)"** as burn target |
| r2 1783917446 s32 | `2. Cast Krenko's Command...` | ? | 1 | wrong card |

**Fix proposal:** prefer the FIRST in-range integer at the HEAD of the text (skip leading
whitespace; accept `N`, `N.`, `N)` forms); only if the head yields nothing, fall back to the
first in-range integer anywhere (never the last — the tail is where echoed stats and prose
numbers live). Keep the in-range guard. Regression fixtures: the six replies above, plus a
conforming `1\nPLAN: ...` (must stay 1) and the old `(2/4)`-echo case the comment describes
(must not regress to -1).

**Companion text change (already proposed in general-suggestions G1):** the HOW TO REPLY
contract block in the code-appended protocol + "Reply with ONLY the number" suffix. Text and
parser must ship together — the contract documents what the parser enforces.

### BUG 2: chosen "Cast Hellrider" silently no-ops (board-state-conditional)

**Symptom:** the cast option is offered, chosen, and correctly parsed (verified `choice` and
`chosen_text` both = Cast Hellrider), but nothing happens: no mana spent, card stays in hand,
no `hand -> stack` in the game log, decision flow advances. The pilot re-picks it every main
phase: 16 consecutive no-ops in r1 1783910629 (loss, opponent life frozen at 17 for 11 turns),
43 in r1 1783908401 (loss, opponent reached 181 life).

**Evidence discriminators already established:**
- NOT a parse bug: parsed choice matches intent in every no-op record.
- NOT a blanket Hellrider block: the same action resolved normally in r1 1783902515 s17
  (`hand -> stack -> battlefield`, attacked same turn).
- NOT the opponent countering: no stack interaction in the logs; and in r1 1783908401 **Boros
  Reckoner cast fine in the same game** while Hellrider no-op'd.
- Correlates with board width: resolved with 5 own creatures on battlefield; no-ops at 0-1.
  (Counted via creature-count on the `Your battlefield:` line at each attempt.)
- Mana check: `Mana available:` count identical between MP1 and MP2 of the same turn at every
  no-op (e.g. 6/6, 7/7, 8/8 in 1783910629 T17/T19/T21) — a resolved cast would drop it by 4.

**A/B repro pair:** r1 1783910629 s27 (no-op, 6 untapped, empty board) vs r1 1783902515 s17
(success, 5 untapped, wide board).

**What to instrument:** log the return path of the cast attempt in the GPT ask handler — when
the chosen MTGAbility/action is consumed, record whether `reactToClick`/payment actually fired
and what it returned. Suspect space: Hellrider is the deck's only `@combat(attacking)`-trigger
haste creature; something in the cast-legality or targeting chain for it appears satisfied at
menu-build time but fails at execution when the board is (near-)empty. The menu offering an
action whose execution then fails silently is the defect regardless of which layer declines it
— either the offer or the execution is wrong, and the discrepancy is currently invisible.

### REPRESENTATION FIX: attribute forced opponent-turn menus

"Choose the target for Sacrifice" arrives during the opponent's turn with no indication of who
forces it or why (opponent's Cruel Edict / Phyrexian Obliterator trigger). One confused reply
("critical phase mismatch", r1 1783908401 s6) parsed to nothing. One string fixes it:
`Choose the target for Sacrifice (forced by opponent's Cruel Edict)`. The pilot handled the
picks themselves well (7/7 correct least-value sacrifices) — the confusion cost is in the
reply quality and parse risk, not the choice.

---

## For the NEXT REVIEWER — the checks that found both bugs (reusable recipes)

Both engine bugs came out of two cheap per-record checks over the JSONL, before any game
reading. Run them FIRST; they change what the rest of the review means.

**1. Desync flag (reply-vs-consumed):** per record, compare the first integer at the head of
`reply` against `choice` (and `chosen_text`). Any mismatch = harness defect, not a misplay.
~15 lines of python:

```python
import json,re
for line in open(path):
    r=json.loads(line); rep=(r.get("reply") or "").lstrip()
    m=re.match(r"^(?:A)?(\d+)",rep)
    if m and r.get("choice") is not None and int(m.group(1))!=r["choice"]:
        print(r["seq"], r["kind"], m.group(1), r["choice"], r.get("chosen_text","")[:60])
```

**2. Chosen-vs-happened (no-op detector):** for every cast-type `chosen_text`, check (a) the
next record's `Mana available:` count dropped, and (b) the game log grew a `hand -> stack`
line for that card. Same pick repeating with neither = engine no-op. Counting re-selections of
the identical `chosen_text` per game is the 30-second version (43x jumps off the screen).

**3. Ground aggregate scans before citing them.** Two of my Task-1 aggregates were misleading
until adjudicated record-by-record: "mana delusion" regex hits were mostly the model correctly
discussing the OPPONENT's blockers, and 7 of 9 "own-side target picks" were forced Sacrifice
menus (correct plays). The scan finds candidates; only the full record convicts.

**4. `chosen_text` is the field that makes all of this cheap** (new since wave-3). Pair it
with `options` (now a real count) and the digest loop in wave3's skill Step 2.

Also: my wave-3-format citation scheme (result+opponent+run+seq) survived a two-run corpus
well; keep epochs tabled once in findings.md and cite `sN` after that.

## For the MODEL EXPERIMENTER — qwen35 pilot profile at maximum guide-adherence

Deck109 is the ceiling case: what the executor looks like when guide + general prompt both
land. Useful as the control seat for prompt/model A/Bs.

- **Near-perfect directive obedience:** 67/67 combats attacked, 14/14 unleash counters, burn
  35:4 face-vs-creature, 7% cast-nothing rate (13/174), 0 mana delusions post-representation-fix.
  When a rule is imperative, keyed to visible board syntax, and non-conflicting, this model
  executes it essentially always.
- **Residual slip class 1 — intent-number mismatch (rare, real):** r2 1783918764 s21: plan
  text says "dealing 2 damage to the opponent" (option 1), reply leads with 3 (own Reckoner).
  The model wrote a number that contradicts its own sentence. Frequency ~1 per corpus; a
  two-pass decode ("state the option text you chose, then the number") would likely eliminate
  it, at latency cost.
- **Residual slip class 2 — format drift under excitement:** the desync-triggering replies
  (echoing the option line, or dropping `PLAN:`) cluster at high-stakes moments — lethal
  available, game about to end (1783905418 s24-25 has visible "Wait, ..." reasoning cascades).
  The reply contract targets exactly this; measuring conformance rate before/after G1 lands is
  a clean experiment.
- **Self-correction works:** the model catches its own stale plans against the board
  ("Wait, I misread the board state. My Hellrider is in my hand" — r1 1783910629 s40). The
  stale-plan spiral of wave-3 did not recur; plan carryover is now net-positive at this seat.
- **It cannot detect engine no-ops unaided:** 43 identical re-picks without ever questioning
  the loop. There is no "this isn't working" prior; the G3 `hand -> stack` anchor supplies the
  missing perception. Worth testing whether the anchor generalizes (does the model verify other
  actions' effects once taught one verification?).

## For the DECK BUILDER

- Deck109's list is doing its job: 8/12 with two of the four losses purely engine-caused, and
  the r2 run (5/6) is closer to the deck's true strength. No construction changes indicated.
- Hellrider (namesake, never drawn in wave-3, cursed by the no-op in wave-4) has still barely
  demonstrated its payoff in logged play: exactly one resolved cast in 18 games across two
  waves. If the wave-5 corpus wants signal on Hellrider-centric guidance, either the engine
  fix must land first or a scripted scenario (TestSuiteAI) should force it — self-play variance
  will not deliver it.
- Boros Reckoner's bounce menu ("Choose the target for Boros Reckoner") is a small
  target-choice trap (see s21 above); any deck running damage-bounce/ENRAGE-style triggers
  should carry a one-line "pick the OPPONENT" note until the intent-number slip class is gone.

## PROCESS NOTES for the next wave

- **Timeout economics fixed themselves at this seat:** 0/12 timeouts (wave-3: 13/21 corpus-wide
  with a 40-min cap). The auto-skip + representation changes appear to have ended the mutual
  stall regime for aggro seats at least. Keep the wave-3 §7 adjudication proposal on file for
  seats that still stall.
- **The wave-3 tooling asks paid off measurably** (`chosen_text`, `Mana available:`,
  `latency_ms`, structured turn/phase). Still missing and still wanted: a final summary record
  per game (end reason, final life, turn count — I reconstructed these from `gameend` records
  and results.tsv), and prompt/guide version provenance per run.
- **Two runs > one big run:** the r1→r2 delta (3/6 → 5/6, same guides) was itself informative —
  it bounded variance and stopped me over-reading r1's losses as behavioral. Per the
  rare-event-power principle, keep at least two runs per wave before attributing win-rate
  movement to guide changes.
- **Sequencing note for the synthesis agent:** my four files interlock — findings.md carries
  the evidence tables, strategy.txt carries interim copies of two mitigations (reply contract,
  cast fallback) that should be STRIPPED when general-suggestions G1/G3 land, and skill.md P4
  documents that removal obligation. Merge order: engine fixes > protocol text > guide cleanup.
