# Wave-25 reviewer brief (shared context for all 7 deck agents + synthesis)

Eleventh cycle. Pool unchanged: 27, 102, 109, 133, 131, 137, 59 (deck59's FIRST GUIDED corpus
— its wave-24 initial guide deployed).

⚠ PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

## Two batches under live validation this corpus (binary 62d8783a5)

**A. The RULES-FIX batch (f0500ad66) — the first corpus on the rules-fixed engine:**
(a1) turn 1 now begins at UNTAP with a real upkeep; only the starting player's DRAW is
skipped — turn-1 upkeep triggers fire; (a2) untap and cleanup grant NO priority (no casts/
activations there); (a3) combat roles clear at end of combat (deferred while another
declare-attackers is scheduled — extra-combat cards persist); (a4) the planeswalker
damage-REDIRECT menu is GONE (player-aimed damage hits the player); (a5) creature-Battles
can't attack/block; (a6) day/night + city's blessing markers excluded from suite zone
accounting. EVERY reviewer: watch for behavior shifts from these rules changes at your seat
— both improvements (decisions the model previously botched due to the old rules) and any
NEW confusion (e.g. the model expecting the old redirect menu, narration changes at turn 1).

**B. The wave-25 step-1 batch (62d8783a5):**
(b1) protocol example de-fanged ("CHOICE: 3 (Cast Example Card)") — the example-leak FP
classes should be structurally gone; (b2) NATURAL-STOP precedence — a naturally-terminated
reply takes the model's FINAL answer (last coded line / last prose bottom-list on the pregame
bottom ask); truncated replies keep answer-first; ATTACK stays answer-first; (b3)
echo-qualifier containment trust; (b4) shockland ETB choices annotate consequences; (b5)
wither shrink respects first-strike ordering; (b6) persist returns narrated + trade
annotations carry return clauses.

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-035605` — 21 games, binary
  /tmp/wagic-62d8783a5, -T 3000 (bumped per the latency findings), REPPENALTY=1.05 standing.
- Win table: **59:6/6 (first guided — from 3/6 Step-0)**, 109:5/6, 102:4/6, 27:2/6, 131:1/6,
  133:1/6, 137:1/6. 1 timeout-draw + 4 adj (10 no-gameend logs = exactly those 5 games).
- **FALLBACKS: ZERO true model-fallbacks across 1,299 decisions** — 0 unparsed, 0 empty,
  0 retracted_choice, 0 stale_echo. The only 15 fallbacks are deck137's by-design convoke
  defers. This is the corpus the parser stack converged. Reply lengths p50 1088 / p95 7997 /
  max 15217 (long deliberations persist but nothing breaks).
- Retry fired 0x (correct if no decode-garbage occurred — spot-check your longest replies).

## Per-seat focus

- **deck59 (FIRST GUIDED, 6/6)**: validate the wave-24 initial guide (hybrid-pip mana fact —
  did the green-mana confusion/vetoes/mulligan-anxiety die? persist sweeper-asymmetry — did
  it wrath while behind now?); the persist narration + trade-annotation return clauses (b6)
  are live at your seat — cite their effect on block/wrath decisions. 6/6 with what decision
  quality? (A perfect record can still hide taxes.)
- **deck27**: natural-stop at bottom asks (b2) — any pregame bottom records: did the final
  list win over a wrong first line? The wave-24 colorstuck class must be gone. Also wither-FS
  (b5) at the Oona seat: annotation correctness round 4.
- **deck102**: steal windows AT LAST? (2-wave validation debt); DFC round 3; pw-redirect
  removal (a4) — your Corrupt/drain spells now always hit the player: any decision shifts?
- **deck109**: Pillar 2-corpora clock CLOSES this corpus (0 mis-costings with trigger present
  = rotate-ready); burn discipline round 5 — (a4) also affects you: burn always hits face now
  when aimed there, no redirect menu records should exist corpus-wide (verify at your seat).
- **deck133**: retraction class post-example-de-fang — 0 retracted corpus-wide: confirm the
  s21-class shape (quoting the example) simply parses now; Liliana round 3.
- **deck131**: Guttersnipe guard round 3; turn-1 upkeep (a1) — any upkeep-trigger cards
  at your seat now firing a turn earlier than prior corpora? decisions-based read on 1/6.
- **deck137**: convoke defers round 3 (15 again — still harmless?); adventure surfaces round
  3; the timeout-draw is yours or adjacent — decompose under the new -T 3000 (did the bump
  help — compare decision counts vs wave-24's caps).
- **EVERYONE**: rules-shift watch (batch A above); shockland annotation (b4) where shocklands/
  pathway lands appear.

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave24/strategy-writing-skill.md (CURRENT skill). Skill -> skill.md. Core ->
general-suggestions.md (19 consecutive PASSes). Win column context-only. Rotation test:
no-guide-mod AND no-new-work-signal = candidate. NOTE: the campaign's termination condition
is approaching (guided roster exhausted; parser converged) — rotation verdicts should be
UNSENTIMENTAL: converged seats rotate, and if no new-work signals exist anywhere, say so
plainly (the loop's endgame is real).
