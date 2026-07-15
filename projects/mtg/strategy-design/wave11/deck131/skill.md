# deck131 skill contribution — wave 11

Read the wave-10 synthesis (`strategy-design/wave10/strategy-writing-skill.md`) as the CURRENT
skill; this file records what the deck131 seat adds/confirms in wave 11. Per the epistemics rule,
NO single-seat evidence proposes a new method line — everything below either CONFIRMS an existing
rung or is routed as a harness/watch observation. deck131's guide is FROZEN byte-identical (7th
consecutive freeze); the seat's value this wave is one CONFIRMED method point and one HARNESS
finding that sharpens an existing method rung.

## Confirmed (not new): the answer-last mulligan close and the YP-swarm second-win-path both VALIDATED a 2nd corpus.

- The wave-10 headline (answer-last kills the mulligan intent-collapse; the concluded decision now
  lands) holds a 2nd corpus: 7/7 mulligan decisions correct both directions, the one no-red opener
  reasoned the gate and LANDED on Mulligan. No method change; the rung is closed.
- **The vs135 WIN is the first end-to-end field proof of the guide's YP-swarm "second win path"**:
  Guttersnipe never cast, deck131 raced deck135 from 20 to 1 on Young Pyromancer + Elemental tokens,
  attack-every-creature obeyed (5 named attackers by T13), instants cast as token fuel. This is the
  positive mirror of the confirmed-fired terminus: a guide LINE (not a representation fix) predicted
  the exact win shape and the field produced it. It reinforces the doctrine that when a seat is
  obeyed-but-losing, wins arrive from CONSTRUCTION/variance (drawing the redundant payoff) — not
  from guide prose — so freezing an obeyed guide is correct even across a 0/6 -> 1/6 swing.

## Method SHARPENING (extends wave-10 headline 2 — obeyed-in-reasoning, leaks at a lower layer): the leak layer can be the BLOCKER PARSER, and the fallback is not inert — it can make the FORBIDDEN play.

Wave-10 headline 2 established that an option-line annotation can be OBEYED-IN-REASONING yet leak
at the number/parse layer, and that a reviewer must score obedience against the REPLY'S REASONING,
not the recorded CHOICE. deck131 wave-11 adds the sharpest instance yet, on the BLOCKER seam:

- The NEVER-block-Guttersnipe absolute fired PERFECTLY in reasoning at all 3 captured windows
  (vs109 seq13, vs135 seq17/30 all reasoned it and emitted `BLOCKS: none`), yet at vs109 the ENGINE
  blocked Guttersnipe into a fatal trade. Root cause: **bare `BLOCKS: none` (the model's natural
  all-decline format) is systematically UNPARSED** — corpus-wide, `BLOCKS: none` -> unparsed 4/4,
  `BLOCKS: B<n>:none` -> parsed 3/3. An unparsed blocker reply falls to the baka blocker heuristic,
  which is NOT a no-op: it can (and at vs109 did) assign the pilot's protected creature to block.
- **Sharpening for the reviewer's method: when scoring a block-side guardrail, the recorded
  `chosen_text` on a blockers record can DIVERGE from what the engine executed.** For an unparsed
  blockers reply, `chosen_text='None'` records the PARSE result, not the executed combat — the baka
  heuristic may still have blocked. Confirm block-side obedience by (a) reading the reply's reasoning
  AND (b) cross-checking the NEXT record's combat `events` (or opponent-seat combat log), never by
  `chosen_text` alone. This is the block-seam extension of wave-10's "score the reasoning, not the
  recorded choice" + wave-10 notes' "score against chosen_text, not choice" — here even chosen_text
  is unreliable for the blockers kind under the unparsed-none bug.
- Routing (firm doctrine, unchanged): this is a HARNESS/PARSE defect. The fix is to accept bare
  `BLOCKS: none` / "no blockers"; the guide MUST NOT be re-worded to emit `B1:none` (that papers a
  parser bug in prose — the sanctioned-against escalation). A guardrail rule (NEVER-block-X) is only
  as good as the parser accepting the model's natural decline format.

## Confirmed (not new): the 4096 cap SPLITS the confusion-spiral class — most conclude, the deepest time out.

The brief's deck131 ask (do spirals now conclude, or burn to the HTTP timeout?) resolves as a SPLIT,
not a clean win: under 4096, the majority of mana/dead-spell deliberations now reach a valid,
CORRECT terminal CHOICE (vs140 seq37/52/56 and vs133 seq16 each produced 4.4-5.6k-char reasoning
ending in a correct pass/cast). The DEEPEST spirals run past 120s and hit the HTTP-client timeout ->
empty_reply (13 this seat, all latency 120003-120004ms). No method change: this is the known
corpus-wide HTTP-timeout class (240s harness fix shipped next). The reviewer note worth carrying:
raising the token cap converted a truncation class into a LATENCY class — the fix is the timeout,
not the guide, and the spiral's ROOT (model distrust of "no untapped sources" / generic-mana
payment) is a model-comprehension item for the harness eye, still not a guide line.

## No core, no guide edit this wave.
Core stays PASS at this seat (no 2+-seat core candidate). The guide is frozen byte-identical. Every
finding routes below the guide (harness/reply-protocol/representation) or to construction.
