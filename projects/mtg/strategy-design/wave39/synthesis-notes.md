# Wave-39 synthesis notes — TWENTY-FIRST per-deck-rotation cycle

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260823-040744`, binary **eabf463b1**, pilot
`qwen36-35b-a3b` (FP8 35B; standing serve :8084 — NEVER cycle, owner ruling), -j 3, **no cap**
(owner ruling 2026-08-21). Pool = 7: holdovers **146 / 139 / 152 / 36** + three Step-0 GUIDELESS
entrants **125 Revelation Fracture (UW draw-go control) / 126 Sanguine Blood (WGB wall-combo) /
162 Draw and Die! (UB forced-draw punish)**. Deliverables this wave: three INITIAL guides, one
hold-seat validation pass, `wave39/engine-ledger.md` (the wave-40 step-1 docket), and this record.

## 1. Corpus summary

| measure | value |
|---|---|
| games | 21 matchups, **20/21 ended naturally** |
| decisions | **1,436** |
| fallbacks | **8 (0.56%)** — `validation_reject_reask_exhausted` ×5 (NEW class), `all_assignments_illegal` ×1, `unparsed_reply` ×2 |
| completeness loss | **1** — 139v125 core-dumped at t15 (Affinity SBA); no paired `gameend`, its `results.tsv` row is bogus |
| crash signatures | 1 (new class); the wave-37 ALord/AAMover class **absent, with its precondition exercised twice** |
| suite / PARSETEST | 1047/0 + 30/0 · 627/0 |

**Every anomaly clustered on the NEW decks — rotation working as designed.** All five
`validation_reject_reask_exhausted` records are ONE window at the deck125 seat; the crash is the
139v125 pairing; both `unparsed_reply` are 146v152.

**Two status.md hypotheses are REFUTED and retired:**
1. *"5 consecutive defers"* and *"validation_reject_reask_exhausted ×5"* are **the same five
   records**, not two anomalies.
2. *"likely a new-deck menu the c3 staleness gate churns on"* — **wrong**. The c3 gate is not
   involved, no menu is stale. It is the CASTING seam: `latency_ms −1`, empty `prompt`/`reply`,
   `coded_answers 0` on all five ⇒ **no model call happened at any of them**; they are local
   rejects replaying a cached answer. Mechanism pinned to `AIPlayerBaka.cpp:3422` +
   `AIPlayerGPT.cpp` ≈L8060-8150 (ledger #2).

**The wave's biggest single finding is a convergence: three seats independently found that a
COUNTERED spell narrates as `resolved`** (deck125 §5.3, deck126 E1, deck162 E-162a). `"was
countered"` appears **0 times in any prompt across all 21 games** despite the W35 emitter existing
at `AIPlayerGPT.cpp:2036`. Convergence across independent reviewers is the campaign's strongest
merge signal; it is ledger **#3** with all three seats' repros attached.

## 2. Per-seat verdicts

### Step-0 entrants (initial guides authored; these seats do not rotate)

**deck125 — Revelation Fracture.** 169 model decisions + 5 engine defers over six games; **0
protocol fallbacks**. Preserve ledger is exceptional: land drop 36/36, X announcement 5/5 largest,
Staff activation 11/11, cast arrival 61/61, sweeper timing 10/11, Path targets 8/8. The seat's #1
finding is a **column of zeros**: Emrakul is offered **0 times in 34 prompts holding it** — `{15}`
against an observed mana ceiling of 12 — yet "Emrakul" appears in **144 of 169 reasoning traces**
and changed a real decision (vs146 s30: Path held because *"Emrakul … wins the game. I just need
to survive"*). Routing is GUIDE (the fact is rendered nowhere), and the guide replaces the plan
rather than arguing with it. Second class: **interaction spent on things that cannot kill you** —
9 of 25 counters on mana producers / walls / 0-power bodies, decisively a 5-mana Fall of the Gavel
on a Silver Myr while a Master of Etherium beat the seat from 26 to −7. Seat verdict: **guide
authored, well-grounded, CONSTRUCTION-CEILINGED** (roster flag, not a guide failure). This seat
must never be the witness for a combat-seam fix — it had **0 attackers and 0 blockers windows in
six games**, structurally.

**deck126 — Sanguine Blood.** 202 decisions, **0 fallbacks**, `reasoning_degenerate` 0.00 on every
record. The combo is deliberate, not stumbled into: tutor targets **4/4 correct** off 39-61-option
reveal menus, the pilot names the loop unprompted in four games, and the engine implements the
Exquisite Blood ↔ Sanguine Bond loop faithfully to opponent-0 (three firings, arithmetic
reconciled). **"BOTH pieces at once" is 3/3 → win, 3/3 → loss, and Exquisite Blood was simply
NEVER DRAWN in all three losses** — assembly was not misplayed, it was undrawn, so the #1 slot is
correctly taken from the leak measured across the WINS: **22 Overgrown Battlement activations, all
22 in UPKEEP, 21 with the mana gone by the next decision, and 5 of 8 blocker windows opened with a
Battlement tapped out of blocking**. Seat verdict: **guide authored; the seat's own losses route to
CONSTRUCTION** (75 cards / 39 lands — roster flag).

**deck162 — Draw and Die!.** 121 decisions, **0 fallbacks**, 6/6 natural ends. **The reviewer brief
was WRONG and the seat caught it**: the brief said deck162 "went winless"; the six `gameend`
records say **3/6**. Recorded here because a false record in a brief pushes a reviewer toward
manufacturing a posture rule for the #1 slot; this seat instead chose its #1 off decision evidence
— **the inverted incentive is not sequenced**. Three windows in one game (`139v162` s7/s10/s13)
where a punisher was in the cast list, the model wrote down in its own words that its punishers
were "not in play", and cast the symmetric draw engine anyway (*"No, Master of the Feast is the
engine/threat"*; *"It doesn't strictly matter"* — the order it called immaterial cost the punisher
a full turn cycle). Positive control in the same corpus: `125v162`, engine T3 → punisher T5, an
unbroken drain, won at **19 of 20 life**. Second leak: **Fog Bank offered 21 / cast 1**, and both
losses to creature decks have `Fog Bank on the battlefield = 0`. Seat verdict: **guide authored**.

### Hold seats

**deck139 — VALIDATED, guide unmodified.** All three wave-38 predictions hold: `MUTATE PLACEMENT`
×10, `MUTATE — CHOOSE THE HOST` ×8, PLAN-carry 7/8 (the 8th is the designed ambiguity negative,
`uniqueNamedIn` correctly returning empty), **intent-mismatch 0**, and **10/10 mutate casts arrived
on the intended host with over/under honoured in both directions**. Zero generic-wording
regressions. The wave-37 docket item "mutate host-intent carry" is **CLOSED**.

**deck36 — VALIDATED, guide unmodified.** The wave-38 ALord/AAMover SEGV fix is confirmed with a
**positive control**, not merely by absence: `152v139` seq 20 resolved Elite Spellbinder against
`Opponent hand size: 0` — the `all(opponenthand)` empty-hand reveal branch that segfaults on
`5104a2433` — and the game reached a natural end; the non-empty clause also ran (`152v126` s21-22).
All six deck36 games carry paired `gameend` records. Zero `Segmentation fault` / `core dumped` in
any of the 21 `.stderr` files.

**deck152 — two guide-lane violations, guide mods SPECIFIED below.** Trade-drift **VALIDATED 0/8**
(second corpus). RETURN-SWING armed twice: **FOLLOWED once** (152v36 s40 — the lethal-now exception
doing real work, all seven attackers into a dead opponent, WON) and **VIOLATED once** (152v139 s21
— life 4, their Illuna 6/6 arms the gate, sole attacker Wolf 3/3 into an opponent at 17, answered
`ATTACK: A1` with `PLAN: Attack with Wolf for free damage` — the exact phrase the guide
pre-emptively bans, in the exact scenario the guide spells out; the seat died the next combat).
Blocks-none default **VIOLATED ×1** (152v139 s11 — Luminarch Aspirant chumped a Pollywog Symbiote
at life 20 to save 1 damage), which makes the wave-37 "value-poor chump" WATCH a **confirmed n=2
class across two corpora**. Katilda-vs-infect **NULL** for a second consecutive corpus (no infect in
the pool since 105 rotated out).

**deck146 — chump anchor VALIDATED, hold-back gate NULL.** Chump anchor 2/2 fired-and-followed
(146v162 s33 at `you would be at 0 - LETHAL`, survived at 3; 146v139 s20 under its real
flying-constraint), "I take the damage either way" 0 occurrences, MAIN-2 pump anti-pattern 0
recurrences with 2/2 pump casts arrival-traced in MAIN 1. **The hold-back gate never armed in 21
attackers asks**: the highest opposing total power opposite this seat all corpus was **3**, against
life ≥ 8 — the wave-39 pool (0-power walls, artifact chaff, creatureless control) **structurally
cannot arm it**. Two corpora, one arming total. Not a defect; a measurement gap with a named
requirement (below).

## 3. Rotation adjudication

Rule applied, verbatim: *"when a deck strategy gets no modifications after a round, and produces no
signal for additional work(bugs, engine changes), that specific deck rotates"* — **BOTH conditions
must hold to exit.**

| seat | guide modified this wave? | seat signal for additional work? | VERDICT |
|---|---|---|---|
| **139** | No | **YES** — party to the un-fixed Affinity crash (its mutate pile is the unusual state), owes the 139v125 RERUN, and owns ledger #7 (mutated-pile wrong-text render) | **HOLD** |
| **36** | No (nor in wave-37) | **NO** — crash-fix validation CLEARED with a positive control; no ledger item is scoped to this seat | **EXIT → canary** |
| **152** | **YES** (two surgical edits, specified in §4) | yes (guide lane) | **HOLD** |
| **146** | No | **YES** — owns ledger #6 (`all_assignments_illegal`, 146v36 s22) and its hold-back gate is unadjudicable on n=1 | **HOLD** |
| **125 / 126 / 162** | Step-0 initial guides authored | — | **HOLD** (Step-0 seats stay for one validation corpus; they never rotate on their debut) |

**One exit, one entrant** (replacements enter ONE at a time, first pass GUIDELESS):

> **ENTER: deck123 "Intruders Of Thraben" (WBU Intruder Alarm combo)** — the runners-up list's
> strongest candidate, held back in wave-39 only because seat 126 already filled the tutor-combo
> lane. Alarm + Thraben Doomsayer / Bloodline Keeper = infinite tokens (Alarm's ETB-untap trigger
> verified present). Step 0, **no guide**, one guideless corpus, then an initial guide.

**Wave-40 pool = 146, 139, 152, 125, 126, 162, 123** (7 seats; canary list grows to 27 with 36).

**Named condition attached to 146's hold** (so the next adjudication is not another null): the
hold-back gate needs a beefy-board opponent. 139 and 152 are both still seated and both produce
mid-game boards with total power ≥ 8 — **the wave-40 review of seat 146 must report the arming
count from the 146v139 and 146v152 games specifically**, and if the gate is still 0/2-armed after
that, it is retired rather than carried a third time.

**deck36's exit is clean**: guide byte-frozen, unmodified across two consecutive waves, its one
pending validation discharged with a positive control, no seat-scoped ledger item. Its guide stays
deployed (canary decks keep their guides).

## 4. deck152 guide modifications — EXACT edits (NOT applied by this seat)

Target file: `projects/mtg/bin/Res/ai/baka/deck152_strategy.txt` (deployed guide; `bin/Res` is
gitignored — a step-1 agent applying these needs `git add -f` if it commits). Both edits are
surgical: no section is added, no working teach is touched, and the two load-bearing tokens the
seat already validated (the parenthesis-wins block, the trade-drift drift-to-catch paragraph) are
left byte-identical.

### Edit 1 — move the sole-attacker clause INTO the RETURN-SWING arming test

Rationale: the violated instance is exactly the clause the guide already spells out, so this is a
**salience** failure, not a coverage gap — the sentence sits in the paragraph tail, and the model
is doing its counting up in the arming test. Move it to where the counting happens, and attach the
falsity fact keyed to the model's own words (*"free damage"*).

**BEFORE** (lines 36-47, verbatim):

```
- BEFORE YOU WRITE "ATTACK:", RUN THE RETURN-SWING CHECK - every attackers decision, first.
  Arming test, read straight off the prompt: YOUR LIFE is at or below the POWER of their biggest
  creature (the first number on its battlefield line). A creature whose line shows +1/+1 counters
  is GROWING (the Orc army class): count it at its power PLUS ONE, because it will be bigger by
  the time it swings back. Their creatures being tapped this turn does NOT protect you - they
  untap and swing back next turn, and a creature you sent attacking cannot block. When the check
  arms, keep your biggest ground creature HOME as a blocker and attack with the rest - even when
  it is your ONLY listed attacker ("ATTACK: none" is the right answer then), even when their
  board "cannot block this turn". EXCEPTION: if the attack you can send RIGHT NOW is lethal
  (unblockable damage at or above their life), send everyone - a dead opponent never swings
  back. When your life is above that number, this check changes nothing: attack per the rules
  above.
```

**AFTER**:

```
- BEFORE YOU WRITE "ATTACK:", RUN THE RETURN-SWING CHECK - every attackers decision, first.
  Arming test, read straight off the prompt, in this order:
    (1) YOUR LIFE is at or below the POWER of their biggest creature (the first number on its
        battlefield line). A creature whose line shows +1/+1 counters is GROWING (the Orc army
        class): count it at its power PLUS ONE, because it will be bigger by the time it swings
        back.
    (2) COUNT THE ATTACKERS THE LIST OFFERS YOU. When (1) armed and the list offers exactly ONE
        attacker, your answer is "ATTACK: none" - that one creature is your blocker. The thought
        "it is free damage" is ALWAYS FALSE here: that damage costs you the block.
  Their creatures being tapped this turn does NOT protect you - they untap and swing back next
  turn, a creature you sent attacking cannot block, and a line reading "[tapped - cannot attack
  or block this turn]" says nothing about next turn. When the check arms and the list offers TWO
  OR MORE attackers, keep your biggest ground creature HOME as a blocker and attack with the
  rest. EXCEPTION: if the attack you can send RIGHT NOW is lethal (unblockable damage at or above
  their life), send everyone - a dead opponent never swings back. When your life is above that
  number, this check changes nothing: attack per the rules above.
```

### Edit 2 — scope the no-chump line above a life number the render already prints

Rationale: n=2 across two corpora, two different shapes; the existing rule ("unless a listed block
trades up, saves your life this combat, or stops poison") did not bind because none of its three
escapes is a NUMBER the model is already reading. Key it to the window's own forecast substring
(HL2: a threshold rule names its number by the render substring that precedes it) — the same
string deck146's validated chump anchor uses, at the opposite sign.

**BEFORE** (lines 88-91, verbatim):

```
Answer "BLOCKS: none" and take the damage unless a listed block trades up, saves your life this
combat, or stops poison counters (see the infect line below). Katilda is a mana source, not a
trading piece: she blocks to save your life, never to swap with a 1/1 mana creature - and she
joins INFECT blocks only from "8 of 10" poison upward (see the infect rule for the exact test).
```

**AFTER**:

```
Answer "BLOCKS: none" and take the damage unless a listed block trades up, saves your life this
combat, or stops poison counters (see the infect line below). THE CHUMP TEST - use the number the
window already prints for you, "you would be at N": when N is 10 or more, a block whose
parenthesis reads "(your blocker dies, attacker lives)" is a chump - leave that attacker unblocked
and keep your creature, however big the attacker is. Take a blocker-dies block only when N is 9 or
less, when the header says the attack IS lethal, or when the attackers are tagged [infect].
Katilda is a mana source, not a trading piece: she blocks to save your life, never to swap with a
1/1 mana creature - and she joins INFECT blocks only from "8 of 10" poison upward (see the infect
rule for the exact test).
```

**Falsifiable predictions for the wave-40 seat-152 review**: (a) RETURN-SWING violations at
sole-attacker windows **1 → 0**, and zero replies containing "free damage" at an armed window;
(b) blocker-dies assignments at a printed forecast of 10 or more **1 → 0**; (c) no regression —
trade-drift stays **0/N** and declared blocks at forecast ≤ 9 do not fall (the wave-31 pitfall:
a brake written against an over-block becomes the next corpus's paralysis).

## 5. Guide-draft violations (sanity vet against the skill rungs) — NOT applied by this seat

The three drafted guides were vetted against the negative-framing ruling, the trust doctrine,
never-hard-suppress, the frequency-claim rung (HL6, with its mandatory pre-ship grep), the
card-fact rail, and the stopgap-register convention. **Nine findings; V1-V3 and V6-V7 are
substantive.** Exact fixes given; the drafts are NOT edited here.

**V1 — [deck126, HIGH] the #1 rule anchors on a render string the ledger says is FALSE.**
`deck126_strategy.txt` L11 and L128 quote `Add 1 green mana with Overgrown Battlement` verbatim as
the identifier for the window it tells the pilot to pass. That label is ledger **#9** — it
hard-codes "Add 1" while the card adds {G} per defender (proved false in-game at `126v162`
s20→s21, `Already in pool: {g}{g}{g}{g}`). `deck126/findings.md` §7 item 2 states *"(none written
against the Battlement option label) — the #1 rule tells the pilot to pass that window, so the
guide never has to quote or contradict E2's 'Add 1'"* — **that statement is factually wrong about
the shipped text**, and when #9 lands the quoted anchor stops matching and the deck's #1 rule
silently stops firing.
*Fix, L10-12*: replace
`At your Upkeep the engine will offer you` / `   1. Add 1 green mana with Overgrown Battlement [cost: Tap]` / `Answer CHOICE: 0 (pass). Every time.`
with
`At your Upkeep the engine will offer you a single action: tapping Overgrown Battlement for green` / `mana. Answer CHOICE: 0 (pass). Every time.`
*Fix, L128*: replace `Your Upkeep, only option "Add 1 green mana with Overgrown Battlement": answer CHOICE: 0 (pass)`
with `Your Upkeep, and the only offered action taps Overgrown Battlement for mana: answer CHOICE: 0 (pass)`.
The findings' own prediction (a) already names phase-keying as the fallback; this makes it the
primary, and it is correct under both the current and the fixed label.

**V2 — [deck126, HIGH] an internal contradiction at the blocking seam.**
L77-79 name a real exception (*"The one blocker you leave out: neither enchantment is on your
battlefield AND that blocker's line reads 'your blocker dies, attacker lives'"*), then L80 asserts
`"BLOCKS: none" is right only when the list offers you no blockers` — which forbids the answer the
exception requires when every offered block is that shape. A self-contradicting pair at a decision
seam is worse than either half alone.
*Fix, L80*: replace with
`"BLOCKS: none" is right only when every blocker the list offers you falls under the line above.`

**V3 — [deck162, HIGH] the guide teaches the pilot to disbelieve a render.**
L113: *"Its option line may read '{right now: draws 0}'; the card still draws every draw step."*
Trust doctrine, consequence (3): **never ship a guide teach that hedges against the render** —
route around a false surface with a decision rule that does not DEPEND on the broken value, fix the
surface, never teach doubt. Dictate is already gated by Rule 2 (which reads the battlefield line,
not the annotation), so the clause buys nothing and costs the render's credibility everywhere else.
It is also absent from the seat's stopgap register (§10), which is where a line like this would
have to live if it were kept.
*Fix, L112-113*: delete the second clause —
`DICTATE OF KRUPHIX {1}{u}{u} - EACH player draws an extra card each draw step. Same gate.`
The render item stays on the ledger as **#10**.

**V4 — [deck125, MEDIUM, convention] two unregistered stopgaps; the findings have no stopgap
register at all.** `deck125/findings.md` runs §1-§10 with **no stopgap register** — the convention
deck126 (§7) and deck162 (§10) both follow. Two shipped lines are stopgaps against open ledger
items: L54-55 (*"The stack prints only a name … The card's rules text is in the OPPONENT DECK list
at the top of this prompt"*), which routes around ledger **#4**, and L90-91 (*"If you choose
Sphinx's Revelation and your very next prompt still shows Sphinx's Revelation in your hand with
your mana unspent, cast something else this turn and take it again next turn"*), which routes
around ledger **#2/#8**. Both are correctly SHAPED (they route around, they do not teach doubt) —
the defect is that neither carries a removal condition, so they will silently outlive their
defects and become the stale compensating prose the pitfall list names.
*Fix*: add a `## STOPGAP REGISTER` section to `deck125/findings.md` with two rows —
L54-55, patches ledger #4, **removal condition**: delete when a stack line carries the spell's
type/cost/(P/T), i.e. when `/usr/bin/grep -c "on the stack: .*{" <corpus>/*.jsonl` is non-empty;
L90-91, patches ledger #2/#8, **removal condition**: delete when the corpus shows zero
`validation_reject_reask_exhausted` records and no duplicate back-to-back cast asks.

**V5 — [deck125, MEDIUM] a self-contradicting threshold in the counter rule.**
L52-53: *"A name you do not recognise: counter it when their battlefield line already shows a
creature, or when your life is 12 or less; **below that**, let it resolve and answer the permanent
afterwards."* "Below that" reads as *below 12*, which is inside the condition it is supposed to
exclude. HL2's class (a threshold whose variable is not bound to the render substring beside it).
*Fix, L53*: replace `below that, let it resolve and answer the permanent afterwards.` with
`otherwise, let it resolve and answer the permanent afterwards.`

**V6 — [deck162, HIGH, card fact] "never dies" is false.**
L48: *"Fog Bank prevents ALL combat damage dealt to it and by it, so it blocks the same attacker
every turn forever and **never dies**."* Fog Bank dies to everything that is not combat damage —
and the wave-40 pool contains Path to Exile ×4, Supreme Verdict ×4, Final Judgment ×2 (deck125) and
Tribute to Hunger (deck126). The card-fact rail applies to the guide's own causal claims, not only
to numbers.
*Fix, L47-49*: replace `so it blocks` … `never dies.` with
`so it blocks the same attacker every combat and takes no damage from it. Removal still answers it,
so a Fog Bank in your hand blocks nothing - put it on the battlefield.`
(the last clause already exists at L49 and is preserved, not duplicated).

**V7 — [deck162, MEDIUM, card fact] "and the wall lives" over-claims beyond the registered
stopgap.** L66-67: *"A block by Shield Sphere or Fog Bank STOPS that attacker's damage completely -
none of it reaches your life total, and the wall lives."* Shield Sphere is 0/6; a 6/6 attacker
kills it. The registered stopgap (§10) covers only the STOPS-the-damage half, which is true; the
survival tail is not, and the block line's own parenthesis already reports it.
*Fix, L66-68*: end the sentence at `none of it reaches your life total.` and continue
`Whether your wall survives is printed in that block's own parenthesis - read it there.`

**V8 — [deck162, LOW, pre-ship grep] a banned frequency token.**
L127: *"FATE UNRAVELER stays home **by default**."* `by default` is on HL6's mandatory pre-ship
grep list. The rule itself is right (and correctly carries the lethal exception, which a blanket
"never attack" would have cost the vs146 win).
*Fix, L127*: replace `stays home by default.` with
`stays home unless the lethal check below clears.`

**V9 — [deck125, LOW, category] a sorcery filed under "permanents".**
L44-45 heads a list *"every permanent that hits you or grows them EVERY turn"* and includes
**Idyllic Tutor**, which is a sorcery that fetches an enchantment.
*Fix, L45*: move it to its own line after the list —
`  - and Idyllic Tutor: it is not a permanent, but it fetches the enchantment half of their combo.`

**Clean on every other axis.** No frequency claim inside any mechanic description; no bare
mid-guide DON'T beside a positive instruction (every falsity fact FOLLOWS its positive rule, is
phrased as a falsity about the thought, and appears at most once per rule); no guide hard-suppresses
a legal play (deck125 explicitly instructs *"If 'Cast Emrakul' is ever actually printed in your
numbered list, take it"*, the exact never-hard-suppress form the owner's X=0 ruling generalises);
every threshold in all three drafts except V5 is bound to a quoted render substring; every quoted
render string in all three per-claim ledgers was grepped non-zero against this corpus except the
mulligan `"having already taken"` string, which is source-verified at `AIPlayerGPT.cpp:11756` and
correctly flagged UNEXERCISED at all three seats.

## 6. Next-wave order

1. **Step 1 — the engine batch**, from `wave39/engine-ledger.md`, four worktree lanes
   (A crash / B decision seam / C narration truth / D option render, + optional E card data). Merge
   order engine → parser/seam → render; the merged-tree gate on master is the only gate that counts.
2. **Apply the guide work**: the two deck152 edits (§4) and the nine guide-draft fixes (§5) —
   V1 must land BEFORE ledger #9 changes the Battlement label. Deploy the three initial guides to
   `bin/Res/ai/baka/deck{125,126,162}_strategy.txt`.
3. **Rotation**: retire deck36 to the canary list; seat **deck123 GUIDELESS**.
4. **The 139v125 RERUN** the moment lane A lands — a wave-40 corpus that still has 20/21 natural
   games has not discharged the completeness invariant.
5. **Corpus**: 7 seats (146/139/152/125/126/162/123), `-j 3`, **no cap**, standing serve :8084.
   ⚠ **Before it runs, settle deck126's construction.** The working tree already carries an
   uncommitted `deck126.txt` edit — `-Forest (SHM) * 16` / `+Forest (SHM) * 1`, i.e. the roster
   flag's suspected typo corrected, taking the deck from 75 cards / 39 lands to 60 / 24. No one on
   this wave made it. **The wave-39 deck126 guide was authored against the 75-card list**, so if
   the edit stands, seat 126's wave-40 deltas are confounded by a construction change and must be
   attributed as such rather than read as guide performance (the wave-35 attribute-deltas-correctly
   rule). Confirm with the owner and commit it, or revert it — but do not run the corpus with it
   unrecorded.
6. **Review**: the three Step-0 seats get their FIRST GUIDED validation (each carries its own
   falsifiable predictions — deck126 §8, deck162 §11, deck125's rule-#1 belief count); 152 gets its
   two edits validated; 146 reports the hold-back gate's arming count from 146v139 and 146v152
   **specifically**, or the gate is retired.
