# Wave-23 reviewer brief (shared context for all 7 deck agents + synthesis)

Ninth cycle. Pool: **27, 102, 109, 140 (continuing) + 137 GW Adventures (GUIDELESS Step-0 — also
the DFC/forced-loss stress seat), 133, 131 (rotated in — the roster's LAST two guided-never-cycled
decks; find each's most recent prior review under strategy-design/wave*/deckN/).** 135/62/44
rotated out last cycle (converged canaries — 9 total now).

⚠ PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

Binary 0e6361732 — delta since wave-22's 1d8ed8194 (live-UNVALIDATED; this corpus validates):
(a) **Forced-self-discard FORCED-LOSS framing** — opponent-forced discard/sacrifice/exile of your
    OWN cards renders "pick the card you can best afford to LOSE" instead of the inverted outward
    TARGET template (deck140's pitched-both-sweepers class).
(b) **R-DFC-FLIP** — modal-DFC "Flip Side" options annotate DISPLAY TOGGLE + other face + cost,
    capped 2 consumed flips/card/turn; both faces castable via the cast menu (back face =
    alternative-cost cast).
(c) **Echo INDEX-WINS** — a well-formed in-range CHOICE index is trusted unless the echoed label
    is genuinely out-of-context; absent-card bookend recovers a clean sibling CHOICE line.
(d) **ATTACK-line CoT hijack fixed** — chooseAttackers takes the FIRST parseable ATTACK: line
    (a combat-math "Attack: Deal 1..." prose line can no longer hijack last-wins).
(e) **⚠ PILOT LIVE THIS CORPUS: repetition_penalty=1.05** (env, first sampling change ever).
    Judge it: spiral length (corpus p50 1106 / p95 7784 / max 15901 chars vs wave-22's
    1029/8186/16882 — essentially FLAT), decision quality, any prose degradation, and the NEW
    empty_reply cluster (below). The pilot verdict is a first-class synthesis output.

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-013710` — 21 games, binary
  /tmp/wagic-0e6361732, round-robin. 0 timeouts/draws, 7 cap-adjudications (14 no-gameend logs
  = exactly those 7 games; integrity clean).
- Win table: 133:5/6, 27:4/6, 109:4/6, 102:3/6, 140:2/6, 137:2/6, 131:1/6. (Pool changed again —
  cross-wave record comparison is WEAK; judge decisions.)
- **Fallbacks 20/1237 = 1.6% (UP from 0.6%) — composition shifted: 10 empty_reply (was 0 for two
  corpora), 4 unparsed_reply (was 0), 3 retracted_choice, 3 deferred_to_heuristic.**
  - The 10 empty_reply all have latency ≈ 2502ms — a uniform fast-fail (HTTP-level error), in
    two time-clusters (corpus start: deck131 s3-6 + deck140 s5-7; later: deck133 s6+s28,
    deck109 s19). Post-corpus the endpoint accepts repetition_penalty fine, so it was transient
    server-side — but this is the FIRST corpus with the pilot AND the first empties in three
    corpora; treat pilot-correlation as OPEN. Each owner of an empty: note board impact only.
  - The 4 unparsed_reply are NEW post-INDEX-WINS shapes — their owners must classify precisely
    (what did the parser see, why did no rule recover it).
  - Per-seat: 133:5, 131:4, 27:3, 137:3, 140:3, 109:2, 102:0.
- Decision kinds: ask ~850, priority, attackers, blockers, reveal; forced-loss framing fired on
  463 prompt renders; DISPLAY TOGGLE flip annotation on 60. Latency median 17.4s p90 80s.

## VALIDATION FOCUS (per owner)

- **deck140 OWNS (a)**: forced-loss framing — find every forced self-discard/sacrifice ask; did
  the model stop pitching its best cards (wave-22: Damnation+Pyroclasm lost)? Also your 3
  fallbacks, and whether the wave-22 revised guide's forced-discard rule is now redundant with
  the engine framing (crutch-lifecycle: propose demotion if so).
- **deck102 OWNS (b)**: Tergrid DFC — with the flip annotation + castable faces: did Tergrid or
  the Lantern get CAST this corpus? Flip thrash gone (cap + annotation)? 0 fallbacks at your
  seat — N9/echo clean. Also the steal engine: exercised at last?
- **deck137 (GUIDELESS Step-0 + DFC/adventure stress)**: baseline review per the skill's Step-0
  doctrine; GW Adventures = adventure-face casts (a DFC-adjacent modal shape — does the engine
  surface adventure faces cleanly to the model?); your 3 fallbacks classified.
- **deck109 OWNS (d)**: the ATTACK-hijack fix — any combat-math CoT lines in attacker replies
  now parsed to the FIRST coded line? Your s19 empty_reply. Attack declarations recorded ==
  declared (your wave-22 A2 scoring rung).
- **deck133 / deck131 (rotated-in)**: re-validate old guides under the modern engine (crutch
  lifecycle); classify your fallbacks (133:5 incl. 2 empties + reveal; 131:4 incl. 4 empties —
  wait, 131's 4 fallbacks include the s3-6 empty cluster: note whether the heuristic answers
  during the outage hurt); 131's 1/6 needs a decisions-based decomposition.
- **deck27**: 4/6 on the revised guide (free-kills-first block rule deployed last cycle — did it
  fire at the snap layer?); your 3 fallbacks; commit-lock watch (any 2nd harmful instance?).
- **EVERYONE**: the pilot (e) — in your seat's longest replies, does prose quality look degraded
  vs prior corpora (repetition penalty can harm structured output)? Any spiral shortening at
  YOUR seat? Report a per-seat pilot verdict line (better/same/worse + evidence).

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave22/strategy-writing-skill.md (the CURRENT skill). Skill proposals -> skill.md. Core ->
general-suggestions.md (17 consecutive PASSes). Construction -> notes.md. Win column
context-only. Rotation test: no-guide-mod AND no-new-work-signal = candidate.
