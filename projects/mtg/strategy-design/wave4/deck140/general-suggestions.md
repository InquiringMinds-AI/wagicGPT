# Deck-140 proposals for the GENERAL layer (core prompt / assembler / engine) — wave 4

Evidence base: 12 deck140 games (1W/5L/6T) across both wave-4 runs; findings.md in this dir.
Live input audited: `bin/Res/ai/gpt/system_prompt.txt` (37 lines, quoted by line number below).
Rule applied throughout: every deck pays for each core-prompt line, so only GENERAL defects
get core text; everything else is routed per-deck or to the platform.

---

## Proposal 1 — The X-announcement index/value mapping is a GENERAL defect; fix it at the ASK, reinforce in the core interface block

**Current state:** core prompt line 4 says *"A cost containing {X} means YOU choose the number
X as you cast the spell - an X spell is never 'worth 0'; it is worth the largest X you can
pay."* That line WORKED (deck140 now casts its X spells — 0/26 in wave 3, ~17 takes in wave 4)
but it teaches the VALUE and says nothing about the REPLY CHANNEL. The X menu numbers options
`1. X = 0`, `2. X = 1`, ... and the pilot systematically replied its intended VALUE as the
option number (realized X below its own PLAN-stated intent in 15/17 X decisions; 4 collapsed
to X=0 — e.g. reply `1` with PLAN "Black Sun's Zenith with X=3 ... destroying them
immediately").

**Layer argument.** This is not deck knowledge — it is a property of the interface every deck
with an {X} cost, a "choose a number", or any future quantity menu will hit, and it is
seat-symmetric. Putting it in seven guides is seven copies of platform documentation inside
strategy text (and new decks would silently lack it). It belongs at the GENERAL layer — and
within that layer, the strongest placement is the DECISION POINT, not the system prompt: the
system prompt is read once and competes with strategy; a line printed at the ask arrives at
the exact moment of the error with nothing between it and the reply.

**Recommended routing (in order of strength):**
1. **ENGINE (best, flagged in findings.md):** reverse the X menu so the LARGEST affordable X
   is option 1. This makes the menu obey the project's own "usually-correct option first"
   ordering convention (X=0 is the worst legal value and is currently option 1), aligns with
   the model's measured first-option bias, and converts the observed collapse-to-1 failure
   into max-X instead of a blank. No prompt text needed at all.
2. **ASSEMBLER (cheap, immediate):** append one fixed line to every X/quantity ask:
   `Reply with the OPTION number, not the X value itself. Option 1 means X = 0.` (One
   sentence, only ever shown at the menu it governs — zero cost to non-X decisions.)
3. **CORE PROMPT (fallback / belt-and-suspenders):** extend line 4:
   *"...it is worth the largest X you can pay. **When the game asks you to announce X, the
   menu lists X = 0 first and the largest X last, and your reply is the OPTION number, not
   the X value - pick the line whose text reads the X you want.**"*
Route 1+2 together make the per-guide teaching (my deck140 #1 RULE) removable next wave.

## Proposal 2 — Strengthen line 36's plan-execution clause with NUMERIC binding (core prompt)

**Quote (line 36, tail):** "...your chosen number must EXECUTE your own plan: if your plan
says to cast or activate something that is in the list, pick that number; never describe an
action and then choose 'Cast nothing' or pass."

**Addition (same sentence):** "...choose 'Cast nothing' or pass. **When your plan names a
NUMBER - an X value, a quantity, a number of targets - check before sending that the option
TEXT you picked contains that exact number; the option's position in the list is not the
number it stands for.**"

**Why + layer:** the existing clause binds plan->action but not plan->magnitude; all 17 X
mismatches sailed through it (the pilot DID pick "cast Rakdos's Return" per plan, then
un-executed its own X). Numeric plan-binding is deck-agnostic and covers future menus
(modal counts, damage splits), so core, not guide. One sentence.

## Proposal 3 — Damage-activation targeting arithmetic (core prompt, one clause in line 27)

**Quote (line 27):** "But activate an ability only when its effect changes something THIS
turn - a creature that can now attack or block, damage dealt, a card drawn..."

**Addition (same bullet):** "...a card drawn. **An ability that deals N damage may target a
creature only when N is at least that creature's toughness (it dies); otherwise the damage
did nothing - send it at the opponent's face instead.**"

**Why + layer:** deck140 wasted 10 of 50 Staff of Nin pings on 2-toughness creatures
(7 on the same 2/2 Guttersnipe) — the ping resolved, changed nothing, and cost the deck its
only clock; line 26's "choose by IMPACT" is too abstract to produce the N-vs-toughness
comparison. The arithmetic is universal (any pinger, any deck, any wave) and one clause —
core. (My guide carries a Staff-specific version; this line would let next wave demote it.)

## Proposal 4 — Closing-won-games / pacing: mostly NOT a core-prompt problem — route to platform, with one optional core clause

Audit finding: deck140's grind-stalls (life frozen 10+ turns, e.g. R1v131 t38, R2v131 t36)
happen WITH the pilot obeying line 17 — finishers deployed, Staff firing every turn. The
residual stall causes are (a) the X blanks (Proposal 1), (b) deck reach vs a rebuilding
opponent (deck-tier, not prompt), and (c) infra: two of the six "timeouts" were
latency-starved games (avg 14.9s/decision, 52s spike, t8/t14 — the -j concurrency artifact),
and control mirrors generate 3-4x the decisions per game (98 records vs 24-36), so wall-clock
exposure scales with archetype. **No new core pacing prose is warranted on this evidence** —
line 17 is already the strongest anti-passivity text in the stack and it is being followed.
Routing instead:
- **HARNESS/PLATFORM:** the wave-3 stalemate-detector proposal stands (end as `draw/stall`
  after N turns of frozen life+board; `end_reason` column in results.tsv) — it separates
  can't-close from slow-inference, which no prompt can.
- **OPTIONAL core clause, only if other decks' wave-4 logs show won-position durdling that
  line 17 misses** (mine don't): extend line 17's deploy sentence with *"and once deployed,
  point your repeatable damage at the OPPONENT every turn - life totals you are not
  attacking do not decay on their own."* I flag it for the synthesis agent but do not push
  it on deck140 evidence alone.

---

## Audit of existing core-prompt lines across deck140's 12 games

| Line | Verdict | Evidence |
|---|---|---|
| L4 "X never worth 0 / largest X you can pay" | FOLLOWED but INSUFFICIENT | X spells now cast (0/26 -> ~17); value teaching didn't cover the reply channel (15/17 undershoot) — see Proposal 1 |
| L4 auto-tap / empty pool normal + L34 "listed = payable" | WORKING | no-mana declines fell 55% (wave 3) -> 11%, and most residuals are legitimate sweeper-holds |
| L17 deploy-your-win-conditions / "stable is NOW" | WORKING | finishers deployed in 10/12 games (up from near-never in wave 3); the WIN executed Blightsteel+Staff exactly as written |
| L35 stale-plan grounding | WORKING | wave-3's plan-poisoning death spiral did not recur in 12 games |
| L36 execute-your-plan | PARTIAL | binds action but not magnitude — Proposal 2 |
| L26 choose-by-IMPACT | MISREAD at the margin | did not produce N≥toughness reasoning for pings — Proposal 3 |
| L29 combat arithmetic, L22 hold-reactions, L23 flash | NOT EXERCISED (not dead weight) | deck140 has ~no creatures/instants; these lines are for other decks — no change proposed |
| L30 mulligan default | NO SIGNAL | no observed mulligan disasters this corpus |

## Platform alternatives for the synthesis agent to weigh (prompt-vs-engine routing)

1. **X-menu reversal (engine)** — largest X first; strongest single fix, makes Proposal 1's
   text layers and my guide's #1 RULE progressively removable. Also audit OTHER
   ascending-numeric menus for the same worst-option-first shape.
2. **Per-ask protocol line at X/quantity menus (assembler)** — "reply with the OPTION number,
   not the X value" printed at the menu; cheapest, ships without engine changes.
3. **Option labels carrying consequence** — e.g. `1. X = 0 (spell has no effect)`; makes the
   blank self-describing if reversal is rejected.
4. **Stalemate detector + `end_reason`/`final_life`/`turns` in results.tsv (harness)** —
   wave-3 proposal, still open; required to stop timeout counts penalizing decision-dense
   archetypes (control decks: 3-4x records/game; two of deck140's six timeouts were pure
   latency starvation at -j concurrency, avg 14.9s/decision).
