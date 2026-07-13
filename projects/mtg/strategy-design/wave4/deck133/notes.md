# Wave-4 deck133 — working notes for wagicGPT (catch-all, by audience)

Everything from this seat's review that doesn't belong in findings.md (review),
strategy.txt (guide), skill.md (method), or general-suggestions.md (core prompt).

---

## For the ENGINE owner

### E1. State-based loss not applied: opponent at -10 life, game continued (P0)
**Repro citation:** run `matchups-20260712-230636`, game deck140 vs deck133
(start_epoch 1783921422), deck133 seat log `1783921425-ai_baka_deck133-0x55cc021249d0.jsonl`.
Exact sequence (turn 15, deck133's Main phase 1):
- seq58: pilot casts Gray Merchant of Asphodel; devotion drain resolves —
  `opp_life` 3 → **-10**, `my_life` 15 → 28 (13-point drain, both sides consistent).
- seq59 (same phase): pilot is asked ANOTHER casting decision at opp_life=-10 and
  casts Liliana of the Veil.
- seq60: priority window, passes. seq61: an ATTACKERS decision is still issued at
  opp_life=-10. Harness then caps the game; results.tsv row says `timeout`.
A player at <=0 life must lose to state-based actions the next time they're checked.
At least two more decision points elapsed with life at -10.
**Suspicions to check, in order:** (a) whether the lifeleech drain (`lifeleech:` in
Gray Merchant's `auto=`) bypasses whatever life-set path triggers the loss check;
(b) deck140's replacement/lifegain permanents — it runs 2x Elixir of Immortality and
4x Venser's Journal, and opp life was observed RISING (18→21, run-1 game) — check
none of these wedges the SBA loop; (c) whether SBAs simply aren't re-checked between
consecutive asks inside one main phase. Note the mirror case does end games: deck133
lost at -3/-4/-5/-9 five times, so death-by-damage adjudicates; death-by-lifeleech
at large negative values is the suspect path.
**Impact:** at least 1 of deck133's 3 "timeouts" was a fully-won game; win-rates in
both corpora are distorted wherever Gray Merchant (or any big drain) is the finisher.

### E2. Single-option priority re-asks are the decision-count multiplier (carried from wave3 #4, still live)
Game `1783922461` (vs131, 29 turns): 132 records, **62 priority**, 36 of them bare
"pass", plus the same single-option fetch activation re-offered across consecutive
phases (upkeep/main/end) until taken. Each is a full round trip at 6-10 s median.
A within-turn "declined this exact single-option offer" memo (drop phase from the
ask-cache key for single-option priority offers, invalidate on board change) would
have cut this game by ~25-30 model calls with zero decision-quality cost. This is
now the main TIMEOUT driver post-wave3 (passivity is fixed; grind games still cap).

## For the HARNESS owner

- **results.tsv: add final life totals (and last turn) to every row, at minimum on
  `timeout` rows.** Both wave3 and wave4 reviews had to open every translog to learn
  that a timeout was winning-but-slow. The gameend record already carries the data
  for completed games; timeouts need the harness to snapshot it at cap.
- **Concurrency note for corpus design:** deck133 seat median latency was 8-11 s
  with max 59-68 s across these runs — consistent with the known `-j` saturation.
  The three timeouts each burned only ~8-12 min of deck133-side inference; the cap
  is consumed by BOTH seats + decision count. For decision-quality corpora on grindy
  pools (lifegain decks 131/140 present), either raise the game cap, lower -j, or
  accept that grind matchups will cap and score them by final life, not `timeout`.
- The c2 translog fields (chosen_text, turn/phase, latency_ms) shipped since wave3
  turned this review's headline stats into one-liners — worth keeping stable; a
  gameend record on TIMEOUT games too (it's currently absent — the three timeout
  logs have no gameend record) would complete it.

## For the NEXT deck133 REVIEWER (process record)

- **Timeout attribution method (the wave-4 flip):** before reading any timeout as a
  play/guide failure, print final `my_life`/`opp_life` from the last record. Wave3's
  timeouts were all losing-passive; wave4's were ALL winning-but-slow (opp at 2, 5,
  -10). The same `timeout` label inverted meaning between waves. Check who was ahead
  FIRST, then attribute: ahead → engine/harness/matchup; behind-with-gas → guide.
- **Normalize for the pool:** decks 131 and 140 both run Elixir of Immortality
  (131 also showed it turn 1); games against them run long by construction. Expect
  the same two matchups to dominate the timeout column every wave.
- **The corpus contains its own guide material:** the vs109 pair (lost run1, won
  run2) is the cleanest A/B this deck has produced; if the wave-4 guide's ROLE CHECK
  works, the vs44 Faerie matchup is where it will show (both wave-4 games lost with
  opponent at 20 — flyers race us on the ground). Watch specifically: discard casts
  on turn >=4 (was 10/24), Obliterator cast+BLOCK when defending, and whether
  Bloodghast attacks into standing blockers stop.
- Grep-ready signatures: plan/list mismatch = reply `PLAN: Cast <X>` where `Cast <X>`
  is absent from the prompt's numbered lines AND chosen_text is `Cast nothing`;
  no-mana verbalization is now mostly benign — verify against chosen_text before
  counting it as the wave3 pathology.

## For the MODEL-EXPERIMENTS track (qwen35 pilot profile, post-wave3-fixes)

**Executes reliably now:** affordability (casts from the list, 98 casts/12 games);
turn-numbered deploy scripts; multi-attacker lethal arithmetic (sums in plans check
out); impact-ranked discard/removal target picks (consistently the right threat);
sustained ability loops when the payoff is stated per-activation (17 Yawgmoth
activations + 4 proliferates in one game); fetch cracking on first offer.
**Remaining failure signatures (ranked by cost):**
1. **Plan/list disconnect** — plans an action the menu doesn't offer, then picks the
   decline option instead of re-planning (vs135-2 seq30-31). The plan register is
   not legality-checked; candidate mitigations: the core-prompt completion clause
   (general-suggestions #1), or a structured pre-choice extraction forcing it to
   name which LISTED option its plan uses.
2. **Posture inertia** — holds "aggressor" across a losing race; needs the trigger
   facts pre-chewed (damage-flow test). It never spontaneously re-derives role.
3. **Late-game spell selection under flood** — casts what it's been rewarded for
   casting (discard) past the point of value; window rules ("turn 1-3 spell") work
   where priors don't.
4. Verbal residue: still SAYS "no mana / lands tapped" while acting correctly —
   harmless now, but any experiment scoring replies (not choices) will overcount it.
A/B suggestion: same seeds vs a larger model on the vs44 matchup would isolate
whether posture inertia is representation-bound or model-bound — the trigger facts
are all in the prompt.

## For the DECK BUILDER

- **Gray Merchant density vs role:** he is the deck's only blocker-independent
  damage AND its stabilizer, but at 4 copies/60 with no tutors or card-selection he
  was castable only 6 times in 12 games (cast 3). If deck133 should showcase the
  drain plan, consider +1 Merchant or cheap selection (the deck already owns
  Phyrexian Arena; a second Arena raises both draw and devotion) — or accept that
  ~half its games are won by beats and the guide now covers that.
- **Discard suite cost in THIS pool:** 8 one-mana discard spells (4 Thoughtseize =
  2 life each) in a 7-deck pool where at least 3 opponents (44 Faeries, 109 burn,
  110 affinity) race. Thoughtseize's life cost is real here — both vs44 losses had
  self-inflicted life (fetches + Thoughtseize + Arena) as a major damage source.
  A 1-2 copy shift from Thoughtseize toward Inquisition (or a cheap removal spell)
  would suit the pool; guide-level mitigation is in place either way.
- Bloodghast's `cantblock` makes the deck structurally weak to flyers (no reach
  anywhere in the 60). vs44 is likely unwinnable on the ground without the
  Obliterator-wall + drain line; that's now the guide's job, but a single
  sideboard-style swap (e.g. Plague Engineer already in the SB) would help if SB
  logic ever lands in selfplay.

