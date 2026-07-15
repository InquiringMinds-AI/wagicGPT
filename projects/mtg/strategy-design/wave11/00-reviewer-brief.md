# Wave-11 reviewer brief (shared context for all 7 deck agents + synthesis)

Second cycle after the resume. This corpus validates the wave-10 engine batch at scale; guides
are nearly unchanged (deck109 +1 hybrid line, others frozen), so the review's center of gravity
is (a) engine-fix verification and (b) any decision-quality classes now visible beneath the
fixed interface noise.

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-072254` — 21 games, round-robin
  over decks 44,109,110,131,133,135,140. Binary /tmp/wagic-54a5a9d01 (wave-10 batch + max_tokens
  4096 + parse ramble guards + stack-target annotation fix + fetch pre-targeting keying).
  Wave-10 guides (deck109 +1 hybrid line). Core prompt unchanged (7th PASS).
- Baselines: wave-10 `matchups-20260715-031825` (2048 cap, pre-guards), wave-9
  `matchups-20260714-094942` (head-first protocol control).
- Win table: 109:5/6, 133:5/6, 44:3/6, 140:3/6, 110:3/6, 135:1/6, 131:1/6. 0 raw timeouts,
  6 life-adjudicated (adj0/adj1 rows in results.tsv).

## KNOWN CORPUS-WIDE (do not re-diagnose; account for in decision attribution)

- **72 empty_reply fallbacks (4.7%)** = HTTP 120s timeouts on tail generations that the 4096
  ceiling now lets run to completion. HARNESS FIX SHIPPED for next corpus (WAGIC_GPT_TIMEOUT
  default 240s, commit 95cf9f5f9). Treat like wave-10's truncation class: note any decision
  where the heuristic fallback changed a game, do not re-diagnose the mechanism.
- unparsed_reply fell 60 -> 11 (the 4096 + ramble-guard fix validated at the corpus level).
- The stack/NO-legal-target contradiction is GONE (0 prompts corpus-wide).
- Zero defer records.

## Engine-fix verification list (confirmed-fired-representation doctrine)

1. **Ramble guards**: any prose-hijack cast left? (wave-10's self-Fatal-Push class — a fallback
   record whose reply is a formless ramble must show fallback=unparsed_reply, choice -1, and the
   HEURISTIC's action, never a prose-scanned integer cast.)
2. **Fetch pre-targeting keying**: the chosen-but-unresolved crack re-ask (wave-10 deck44
   s5-s7, 3 windows for 1 crack) — gone? Fetch windows corpus-wide: 115 (vs 96 wave-10, 216
   wave-9) — attribute the small rise (different game trajectories vs a real re-ask class).
3. **deck109 hybrid line**: any {B/R} Rakdos Cackler decline recurrence? (wave-10 seq18/19/21
   class.) Also the colored-pays-generic belief at any seat (parked core candidate — the
   representation route is ledger #5).
4. **Force of Negation**: STILL BROKEN by design this corpus (ledger #1 not yet fixed — the
   free-alt-cost cast resolves without countering). deck135's reviewer: quantify the bleed and
   verify the fixed annotation no longer adds confusion; the ENGINE fix is next cycle's top item.
5. **Auto-tap color-stranding** (ledger #2, not yet fixed): deck44's reviewer — did it recur?

## Doctrine (BINDING, unchanged)

Win-rate is context only. The unit of analysis is the DECISION with seq repros. Guides are at
or near their terminus on this pool: adopt-a-revision bar is HIGH (a new decision-level failure
class, not drift). The pause-aware demotion doctrine applies. Layer-routing enforced.

## Output contract (per deck agent, sequential, in strategy-design/wave11/deckN/)

findings.md -> strategy.txt (full guide, byte-identical if frozen) -> skill.md ->
general-suggestions.md -> notes.md. The CURRENT skill = strategy-design/wave10/
strategy-writing-skill.md. Convergence gate: single-seat items cannot add/cut core lines.
