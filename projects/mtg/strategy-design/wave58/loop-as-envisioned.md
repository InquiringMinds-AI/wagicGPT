# The loop as the owner envisioned it (verbatim, 2026-09-03) — AUTHORITATIVE; supersedes every
# loop-mechanics description elsewhere (memory, briefs, skill). "everything else in how the loop is
# operating is an invention of claude."

step one: fix and improve - fix all known bugs, consider interface improvements that should give
clearer and more complete information to the opponent llm allowing it to make better choices,
improve parsing of those choices, consider structural improvements to the opponent llm interface
that reduce the inference turns required and or should produce more successful actions from the
opponent llm. merge, test suite, fix aparent issues until done.

step two: gather test information from headless play - 7 decks each facing one another creates a
pool of data reflecting the current state of the engine and how the opponent plays those decks as
it operates under the current engine and guide.

step three: evaluate the information - each deck has an agent assigned that reads the transcripts
of the deck, compares the actions of the llm to the instruction set, finds issues in the game, the
interface, and the guide, finds any other informaton worthy of surfacing for the core loop. the
agent updates the guide if needed, proposes general guide changes if needed, and proposes a
revision to the skill for making strategy guides if needed. the agent elevates to the orchestrator
any information relevant to the core loop such as interface issues, engine bugs, incorrect card
implementation, etc. then a synthesis agent reviews those proposals for the general guide, then
the strategy guide skill, with a final edit to thye general guide if needed.

loop to step one.

---

## Mapping of current practice (Claude's inventions named; wave 58 runs on the three steps only)

KEEP — it IS a step:
- Step one = the fix/improve lanes (known bugs incl. the owner's Vita reports; interface, parsing,
  structural improvements), then merge + test suite + fix until done (the hermetic gate).
- Step two = the 21-game round-robin corpus of the 7 decks, headless, full games (Invariant 00).
- Step three = ONE AGENT PER DECK (7), each: reads its deck's transcripts, compares the LLM's
  actions to the guide, finds game/interface/guide issues, surfaces core-loop information,
  updates the guide IF NEEDED, proposes general-guide changes IF NEEDED, proposes a skill revision
  IF NEEDED, elevates engine/interface/card items to the orchestrator; THEN one synthesis agent
  reviews the general-guide proposals, then the skill proposals, then a final general-guide edit
  if needed.
- The owner's standing rulings that sit inside a step: full games or the test failed (00); one
  lategame prompt surfaced per wave, non-blocking (his 2026-08-19 review criterion); verify Oracle
  before card fixes; no hard caps on legal choices; guides are the deck agents' to change.

DROP — Claude's inventions (not run in wave 58 unless the owner re-adds them):
- The 45-item ranked DOCKET with a falsifiable prediction per item, the review-carry-list,
  the PASS/FAIL/UNTESTED prediction-adjudication tables, the three-number literal audit (#188),
  the "discharge" bookkeeping. (Elevated items go to the orchestrator as a LIST; step one fixes
  known bugs — the orchestrator picks what to fix.)
- The boundary-pass reviewer lane (a fourth pass over the guides).
- The engine-narration seat and the 2-3-deck seat groupings (his shape: one agent per deck).
- Per-wave skill AMENDMENT numbering / the R-numbered general-strategy ledger as growth machines
  (proposals are made only IF NEEDED; synthesis edits the general guide and the skill, it does not
  append a numbered amendment every wave).
- The fixed six-lane wave shape cut from the docket (step one is "fix all known bugs" + the
  improvements the orchestrator judges worth making — a list, not a quota).
- Rotation machinery / canaries / exit projections — NOT in the owner's statement (the record
  attributes a per-deck rotation rule to him on 2026-07-16; conflict flagged to him).
- The wave-58 draft brief clauses (rotation/null-outcome/falsifier gates) — superseded by this
  statement; only "updates the guide IF NEEDED" survives, which already implies the null outcome.

Cost consequence: step three = 7 deck agents + 1 synthesis; step one = as many lanes as there are
known bugs and judged-worthwhile improvements. Model routing (Codex-first for mechanical lanes) is
a separate owner call, unchanged by this statement.
