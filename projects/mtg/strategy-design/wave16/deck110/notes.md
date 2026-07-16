# Deck-110 wave-16 — development notes (self-contained; layer-routed engine/harness ledger + rotation verdict)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-084532/`, 6 deck110 seat translogs
(`*ai_baka_deck110-0x*.jsonl`). Binary `/tmp/wagic-69793930a`. deck140 rotated OUT, deck62
(Enchantresses) entered guideless. Wave-11 guide FROZEN (live `deck110_strategy.txt` md5 `00849aa9`;
byte-identical **9th freeze**). Answer-after-plan + name-echo (`CHOICE: N (name)`). Choices 1-indexed.
Card facts vs `projects/mtg/bin/Res/sets/primitives/mtg.txt`. Every item carries a repro; engine/
harness items belong HERE, never in prompt/guide text. Record: **4 WIN / 2 LOSS** (62,44,135,17 win;
133,21 loss). Both losses NEW this wave (won both last wave); DC1-owned (flood/matchup), see DC1.

## H1 — HARNESS / HIGH / NEW: the `YOUR PLAN (as you last stated it)` carry re-injects STALE reasoning; caused a self-Galvanic-Blast on the model's own Steel Overseer (outcome cost, in a LOSS).

Every decision prompt carries a `YOUR PLAN (as you last stated it): ...` block = the model's PREVIOUS
full PLAN prose. Fine when the plan is a clean resolved intent; HARMFUL when it is stale relative to
the advanced state (card already cast, phase/menu changed, or the prior plan ended mid-"Wait...").

REPRO (a) — CLEAN CAUSAL BLUNDER, deck21 seat, T2 race vs Goblin Gang, game LOST:
- s5: cast Steel Overseer (choice 2). Correct.
- s6: menu now `[Cast Galvanic Blast, Cast nothing]` (Overseer already cast). Carried plan still reads
  "I will cast Steel Overseer ... I will NOT cast Galvanic Blast yet." Plan names an off-menu action;
  model emits **CHOICE:1 = Cast Galvanic Blast** (the action its prose explicitly rejected). fb=None.
- s7: TARGET select `[you, Steel Overseer(your bf), opponent, Raging Goblin]`. Carried plan says "cast
  Steel Overseer" -> model picks the NAME-matching option = its **own Steel Overseer**. fb=None.
- s8 log: `Damage: 2 + Damage: 2 dealt by Galvanic Blast to Steel Overseer -> Steel Overseer:
  battlefield -> graveyard`. Metalcraft (3 artifacts) Blast killed its own just-cast creature =
  self-inflicted 2-for-1 (lost a body + wasted a Blast) in a race it went on to lose -5/16.
  Name-echo did NOT catch it: the echoed string "Steel Overseer (your battlefield)" IS an offered option.

REPRO (b) — UNPARSED SPIRALS seeded by carry-vs-prompt contradiction (~14k chars each -> ramble guard
-> `unparsed_reply` -> heuristic):
- deck21 s13: carry describes an Upkeep menu ("list only shows Counter with Steel Overseer") while the
  current menu is Main-phase `[Galvanic, Memnite, nothing]`. Also the board line `Steel Overseer
  [tapped - untaps and can attack next turn]` (wave-15 board-contradiction witness) fed the loop.
- deck133 s9: carry asserts "Mana available: {r} from 1 untapped source" while the current prompt reads
  `{r}{w} from 2 untapped sources`. Model re-litigates the mismatch until it times out.

REPRO (c) — prose-vs-emitted-CHOICE divergence, deck135 s15: prose "I will NOT equip Steel Overseer
yet" but CHOICE:2 = equip Steel Overseer (the emitted choice followed the current menu and was RIGHT;
the prose was the stale carry). Same family.

ROOT: prompt assembly re-injects the full prior PLAN, including its unresolved "Wait..." tail, without
reconciling it against the CURRENT menu/state. LEVER: when the carried plan's named action is not on
the current menu (state advanced), DROP or truncate the carry; OR carry only the last resolved
`CHOICE:` line, not the rambly PLAN. Owner: `AIPlayerGPT.cpp` prompt assembly. Priority HIGH.
Cross-deck (every seat carries a plan); deck110 = cleanest repro with an OUTCOME cost. NOT guide-fixable
(guide text can't defuse a prompt-assembly bug). Ties wave-11 headline-1 (PLAN/CHOICE mismatch single
root) to a concrete harness mechanism. Fixing H1 should also cut the corpus unparsed-loop rate.

## E4 — RESOLVED / standing — Galvanic Blast metalcraft VALIDATED at corpus scale (11 casts, both arms).
COUNT `Damage:` instances (do NOT dedupe the string) vs surface `Artifacts in play: you N`:
metalcraft OFF (N<3): deck133 s16, deck44 s14 = single `Damage:2` = 2. Correct.
metalcraft ON (N>=3): deck62 s14(4), deck17 s9(3)/s22(4), deck21 s8(3)/s15(5), deck44 s17(3)/s26(6),
deck135 s18(5)/s26(6) = two `Damage:2` = 4 every time. Correct. The wave-13 flat-2 card-script bug
stays CLOSED. VERIFICATION TRAP re-confirmed live: a string-dedup grep reports a 4-dmg Blast as "2";
count instances or read the life delta. deck21 s8's 4 correctly hit — but the TARGET was the model's
own Overseer (see H1). Damage engine correct; targeting was the harness bug. NO guide text.

## E5 — RESOLVED / standing — attackers name->index reconcile VALIDATED (17 windows).
Every eligible named creature unioned in (MIXED `A1,<name>`: deck62 s9 `A1, Etched Champion`; pure
index: deck62 s16/s21, deck44 s30/s34 spreads). Every summoning-sick name excluded (deck135 s6
`A1, Steel Overseer` -> only Signal Pest kept). `no attackers` hold-backs correct (deck21 s18 @6 life;
deck135 s20). NO eligible attacker dropped. See M1(a) for the summoning-sickness model side-signal.

## E2 — ENGINE/REPRESENTATION (unfixed): equip no-op marker is TARGET-IDENTITY-keyed, blind to net-zero MOVES. DID NOT RECUR this wave — validated-by-absence.
Only 1 Cranial Plating attached in every game. All equips legit by OUTCOME DELTA:
- deck135 s10 equip Signal Pest -> s15 MOVE to Steel Overseer = UPGRADE (7->9 power, tougher body); the
  outcome-delta lever would ALLOW this (raises resulting power above current carrier). Then held.
- deck17 s14 equip Vault Skirge -> s20 Vault Skirge DIES (traded w/ Plumeveil) -> s22 re-equip Steel
  Overseer = legit post-death rebuild (Rule #1). Then held.
- deck133 s18 equip Vault Skirge; deck62 equip Etched Champion (protection carrier #1). All held.
ZERO `(ALREADY — change NOTHING)` options taken corpus-wide (Rule #1 100%). The identity-keyed blind
spot (net-zero DOWNGRADE move) got no exercise because the model made only value-positive moves. ROOT
& LEVER unchanged: compute the marker from OUTCOME DELTA — mark/suppress "Equip ... targeting X"
whenever moving would not raise X's resulting power above the current carrier's. Owner:
`AIPlayerGPT.cpp` equip-option serialization. Priority MEDIUM/dormant.

## E6 — REPRESENTATION (open): "Cast Card Normally with Mox Opal" mislabel — NOT EXERCISED this wave.
No cast-with-mana-source option shape appeared at this seat (all 5 Mox interactions were plain
`Cast Mox Opal`, casting the artifact itself, serialized cleanly, handled correctly). The wave-15
witnesses were both in one specific deck44 board state that did not recur. Not-exercised != not-fixed;
ledger stays OPEN. Owner: `AIPlayerGPT.cpp` cast-with-mana-source serialization (show the CARD/action,
not the rock's `{Metalcraft - {T}: Add one mana...}` text). Priority LOW-MED. Cross-deck (any mana rock).

## M1 — MODEL WATCHES.
(a) **Summoning-sickness not tracked (recurs, harmless).** deck135 s6 named a summoning-sick Steel
Overseer as an attacker; the reconcile excluded it. Latent risk only if it ever gates a lethal/hold.
Harness mitigation if it bites: board-line tag `[can't attack yet — summoning sick]`. Cross-seat Q.
(b) **Blocker over-block (wave-15 M1(b)) — did NOT recur.** Only defensive windows: deck62 s10
`no blockers` (correct, took 2 while ahead), deck21 s18 `no attackers` (correct hold-back @6). Closed
this wave; keep the cross-seat question open.
(c) **Reasoning-tax / ramble rate elevated (7 unparsed + 2 defer, ~5.6%; deck17 3-unparsed hotspot),
NON-biting.** All heuristic-answered; only deck21's spiral had a downstream cost (H1). Big non-fallback
rambles in the deck62 WIN too (s10 8.5k, s12 12.8k chars) — all self-corrected. H1 is one amplifier.

## DC1 — DECK-CONSTRUCTION / META (standing, reaffirmed — owns the loss column).
Both losses share the precondition the guide cannot create:
- deck133 0/16: Inquisition of Kozilek + Fatal Push removed its one threat, then it drew Glimmervoid,
  Glimmervoid, Island, Cranial Plating T6-T12 (verified per-turn hand line) = pure flood vs a B/x
  attrition/recursion deck. Correct play; no legal damage path at the end.
- deck21 -5/16: land-heavy draw (3 Citadel + Mox, bodies burned off) vs fast red + the H1 self-blast.
Not guide tasks. Mulligan NOTE: all 6 opening hands this wave were 1-creature keeps and the deck went
4/2 — the guide's `keep 2+ creatures` is universally overridden and the deck wins anyway; do NOT
tighten it (would ship 4 winners). If ever tuned: trim ~0-power filler + Mox/land toward mid bodies +
reach/evasion redundancy.

## Positive validations (no action, continuity)
- Name-echo: 0 wrong remaps. The dangerous deck21 s7 self-target was a CORRECT echo of an offered
  option string; fault is H1's stale carry naming the wrong card, not the echo mechanism.
- Rule #1 (equip-once) obeyed 100% — 0 `(ALREADY change NOTHING)` options taken corpus-wide.
- E3 re-derived artifact count agrees with surface N in every window; Glimmervoid-fold-in reasoning
  persists (deck62 s12/s13) but self-corrects; no metalcraft-gated pick flipped.
- Auto-tap color-stranding NOT observed (Glimmervoid + Mox Opal any-color buffer; every listed colored
  cast fired). Lightmine/wither not present this corpus (deck140 rotated out).
- deck62 (guideless newcomer opponent) played AGAINST cleanly from deck110's side: Etched Champion
  protection carrier walked past green Saprolings for the win — the guide's carrier-priority #1 held.

## ROTATION VERDICT (per-deck rotation rule) — HOLD: do NOT rotate deck110 out this cycle.
Rule: a veteran rotates out only if its guide takes NO modification AND its seat surfaces NO new work
signal. Condition 1 MET (guide FROZEN, 9th freeze, terminal). Condition 2 NOT met — the seat surfaced
a NEW, outcome-costing HARNESS repro this cycle:
  - **H1 (NEW/HIGH):** stale plan-carry -> self-Galvanic-Blast on own Steel Overseer (game-costing) +
    the two ~14k unparsed spirals + prose/CHOICE divergences. Clean cross-deck repro, first captured here.
  - deck110 remains the canonical VALIDATOR for the Galvanic finisher (11/11 both arms) and the equip
    outcome-delta behavior (validated-by-absence) — these are exercised at scale only at this seat.
So: HOLD. This is now a HARNESS-CANARY seat, not merely a near-rotation candidate — it keeps surfacing
novel prompt-assembly bugs with outcome coupling. It becomes a clean rotation candidate only once H1
(plan-carry) and the dormant E2/E6 levers are addressed AND a subsequent corpus surfaces no new class.
The guide itself needs nothing further.
