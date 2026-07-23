# Wave-21 reviewer brief (shared context for all 7 deck agents + synthesis)

Seventh per-deck-rotation cycle. Pool UNCHANGED from wave-20 (135,62,49,35,14,27,102 — 7-of-7
repeat, a clean repeated-opponent control). Nobody rotated out. deck102 plays its FIRST GUIDED
corpus (wave-20 wrote its initial guide). deck27/49/62/14 play their wave-20 surgical revisions;
decks 135/35 remain FROZEN (byte-identical guides).

Binary 3be7f5a67 — delta since wave-20's 95e89c204 (the wave-21 engine batch; ALL items below are
code-verified + suite-green but LIVE-UNVALIDATED — THIS CORPUS IS THEIR LIVE VALIDATION):
(a) **Partial attacker-sparing payment** — wave-20's sparing was all-or-nothing (spared every
    would-be attacker iff swing-neutral sources covered the WHOLE cost, none otherwise); the
    turn's last cast therefore tapped the attackers (this, NOT a combat-gate bug, was the
    deck35 "attackers non-issuance" — the gate was always correct). Now: attackers are drawn
    into the bill weakest-power-first only while still short, X-spells excluded.
(b) **ANSWER-FIRST reply protocol** — the model is instructed to emit its coded answer line
    (CHOICE:/ATTACK:/BLOCKS:/PUT:) FIRST, then reasoning, then PLAN: last. Parser is
    order-agnostic; truncation now leaves a parseable first line.
(c) **Parser hardening** — template-placeholder CHOICE lines dropped; a CHOICE retracted
    ("Wait, I made a mistake" class) with NO replacement routes to heuristic with the NEW
    fallback value `retracted_choice`; last well-formed CHOICE wins.
(d) **Prose-intent salvage** (combat only) — a truncated blocker/attacker reply with no coded
    line can be recovered from an explicit prose statement ("block A3"); negation-guarded,
    unambiguous-only; such records carry `choice_source="prose"`.
(e) **Reveal framing** — hand-source reveals now say WHOSE hand ("The opponent revealed their
    hand"); fixed <1> choosers (Thoughtseize/Duress class) frame pick-EXACTLY-ONE and trim
    extra picks to the first; library/multi-pick reveals keep the prior text.
(f) **Bounce-on-stack** — battlefield-only TARGET CHOICE menus prepend "the spell being cast on
    the stack is NOT a legal target..." when an opponent spell is pending; bounce-shaped cast
    options carry "[this cannot target the spell on the stack]". Annotation only.
(g) **Wither trade annotations** — "(both die)" now requires blockerPower >= attackerToughness
    or deathtouch; a surviving attacker/blocker annotates "(wither shrinks it to X/Y)".
(h) **Baka blocker fallback** — 0-power creatures skip kill-blocks and never pile as a 2nd
    blocker (lone chump still allowed). Affects HEURISTIC fallback blocks only.
(i) **Mana-count integrity** — disabled producers (noactivatedability etc.) are no longer
    counted as available mana; a refused payment click aborts the cast plan instead of
    floating mana (the deck102 Witch-of-the-Moors stall loop). Casts offered should now be
    completable; watch for any repeat-pick of the same cast with mana left floating.
(j) New config keys max_reply_tokens / repetition_penalty (repetition_penalty is OFF this
    corpus — decode behavior unchanged by config).

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260723-084938` — 21 games, round-robin
  over 135,62,49,35,14,27,102. Binary /tmp/wagic-3be7f5a67. Translogs FLAT in the run dir:
  `<epoch>-ai_baka_deckN-<ptr>-vs-ai_baka_deckM.jsonl` (owner seat = first name).
- Win table (Δ from wave-20): 27:5/6 (↑ from 3), 62:4/6 (↑ from 3), 35:3/6 (flat),
  102:3/6 (↑ from 1), 49:3/6 (↓ from 4), 14:2/6 (↓ from 3), 135:1/6 (↓ from 4).
  0 timeouts, 0 draws, 4 life-adjudicated at cap, 0 segfaults (8th clean corpus).
- **Fallbacks 0.7% (8/1119) — a 4x drop from wave-20's 3.0% (32/1067), and the composition
  CHANGED COMPLETELY: 0 unparsed_reply, 0 empty_reply (wave-20 had 29 unparsed), 5
  retracted_choice (NEW deliberate safety route — judge whether each retraction-fallback was
  CORRECT, i.e. the model really had disavowed its choice), 3 stale_echo.**
  Per-seat: 62:4, 135:1, 14:1, 35:1, 49:1, 27:0, 102:0.
- 0 prose salvages fired (nothing needed one — consistent with answer-first working).
- Decision kinds: ask 723, attackers 133, priority 136, blockers 37, gamestart 42, gameend 34,
  reveal 14 (135:12, 102:2). TARGET CHOICE framing on 717 records; the new stack-trap header
  fired on 4. Latency median 15.9s, p90 70s. Reply length p50 1.0k / p95 8.2k / max 16.9k chars
  — long deliberations still occur but no longer break parsing.

## VALIDATION FOCUS (the wave-21 engine batch — every reviewer, per own seat)

- **Partial attacker-sparing (deck35's reviewer OWNS)**: attackers decisions now appear in
  EVERY deck35 game (1-4 per game; wave-20 had a COMPLETE 0-attacker game vs49). Verify at the
  turn level: on turns where the seat cast its last affordable spell with non-haste attackers
  up, was the attack window then offered? Any Sliver still tapped for mana when a land could
  have paid: seq required. Also confirm no NEW pathology (e.g. failing to cast because too
  much was spared — a spell it could afford but declined/failed to pay).
- **ANSWER-FIRST + parser (every reviewer)**: sample your seat's longest replies (p95+) — is
  the coded line now FIRST? Did any decision degrade because the model committed before
  reasoning (answer-quality regression, not parse regression)? For each retracted_choice at
  your seat: read the reply — was the fallback correct (model really disavowed), and did the
  heuristic's answer hurt?
- **Reveal framing pick-ONE (deck102's reviewer OWNS)**: 2 reveal records — do they render
  "The opponent revealed their hand ... choose the ONE card"? Did the model pick exactly one?
  Was it the best strip? Mechanical integrity: the named card left the opponent's hand.
- **R1 second confirmation + Azcanta (deck135's reviewer OWNS)**: 12 reveal records this
  corpus. Confirm the Glacial Revelation to-hand partition on every cast (detector: "get snow"
  picks appearing in a following "goes to graveyard" list = regression). Did an Azcanta
  activation window finally occur? ALSO: deck135 fell 4/6 -> 1/6 on a FROZEN guide — decompose
  the losses (draw variance vs a batch-item side-effect vs matchup). This is the corpus's main
  quality-regression question.
- **Wither annotations (deck27's reviewer OWNS)**: every Oona's Gatewarden block pairing —
  annotation must match real lethality math; the wave-20 "(both die)" false positives must be
  gone. deck27 also jumped 3/6 -> 5/6 on its first revised guide: attribute (guide revision vs
  wither fix vs opponent-field changes).
- **Bounce-on-stack header (deck14's reviewer OWNS)**: the header fired on 4 records
  corpus-wide. For each at deck14: did the model stop self-bouncing / thrashing at the forced
  target menu (wave-20: 1 misplay + 3 fallbacks in this class)?
- **Blocker-fallback gang-chump (deck62's reviewer OWNS)**: with only 8 fallbacks corpus-wide
  the heuristic rarely fired — check the 4 deck62 fallbacks specifically: if any was a blockers
  fallback, did the heuristic still sacrifice a 0-power/Argothian? Also validate edit D
  (favorable-block litigation shortened?).
- **Cast-stall loop (deck102's reviewer, secondary)**: any repeat-pick of the same cast option
  across consecutive asks with mana left floating in the pool ("Already in pool" in the next
  ask) = the item-8 residual (c) firing; seq + both records verbatim if seen.

## Layer-routing (unchanged discipline)

Engine/harness items -> notes.md with seq repros, NEVER prompt text. Guide changes ->
strategy.txt per the CURRENT strategy-writing skill (wave-20 rev at
strategy-design/wave20/strategy-writing-skill.md). Skill-method proposals -> skill.md.
Core-prompt proposals -> general-suggestions.md (layer-routed). Deck-construction observations
-> notes.md, routed to the roster owner. Win column is CONTEXT-ONLY; decision quality is the
signal (the core prompt is symmetric).

## Per-seat wave-21 exit questions (from wave-20 synthesis — answer them explicitly)

- 135: R1 second confirmation? Azcanta activation observed? (both YES -> rotation candidate);
  retracted-CHOICE lock-in (HARNESS-1) gone?
- 49: develop-seam fallbacks dead (0 unparsed at early Main-1)? burn-face rate held without
  over-generalizing the key-blocker exception? "either is fine, commit" prevented blind casts
  when burn was lethal? (Record dropped 4/6 -> 3/6 — decompose.)
- 62: edit D shortened the favorable-block litigation? N8 blocker-sac fixed at the heuristic?
  over-deliberation-of-a-settled-decision persistent? (4 fallbacks this corpus — the most.)
- 14: bounce-on-stack trap + header dropped the misfire count? E2 truncation class gone
  (answer-first)? Record fell 3/6 -> 2/6 — decompose.
- 27: late-stall durdle revision validated in a GRIND matchup? wither annotation fixed?
  5/6 — what drove the jump?
- 35: 0-attacker games gone? payment fix clean (no tapped-attacker regressions)? fragile 14k
  near-misses converted to real fallbacks or still parsing?
- 102: initial guide (Step 0-ter): symmetric-trap fire-gate stopped Pox self-harm? deploy/attack
  floor moved opponent life? Thoughtseize pick-ONE stopped over-selection? any Tergrid game?
