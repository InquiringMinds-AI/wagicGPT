# deck140 wave-10 — development notes (engine/harness/model/deck-construction; layer-routed, with repros)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-031825`, deck140 seat files
`*ai_baka_deck140-*.jsonl` (6 games). Fields: `prompt, reply, choice, chosen_text, options_text,
my_life, opp_life, latency_ms, turn, seq, kind, events, fallback`. Opponent creature count prints as
`Opponent battlefield (creatures: N)`; tapped attackers render `[tapped - untaps and can attack next
turn]`. Binary `/tmp/wagic-c8c054be8`. NOTE: `choice` is recorded 1-based (verified: 192/200
CHOICE-bearing decisions match the 1-based read of the written `CHOICE:N`; the 8 exceptions are all
`CHOICE: 0` -> "pass").

## HARNESS / TOOLING
1. **`tools/intent-collapse-metric.py` is uncalibrated for the answer-after-plan protocol — recalibrate
   before trusting its cross-arm counts.** Under answer-LAST it reports `non-numeric heads: 215/215`
   (deck140 seat) — its `head_int` parser is now vacuous by design (every reply is prose-first). Its
   `reversal + concluded-option != choice` heuristic flagged 21 deck140 windows (135 corpus-wide vs the
   120 head-first control) but hand-audit shows ~all 21 are FALSE POSITIVES: `plan_concluded_option`
   matches the ubiquitous "hold Rakdos" mention in the plan tail and mis-indexes multi-option lists, so
   it flags CORRECT sweeps/holds (seq38 Wrath, seq23/12 Damnation, seq16 correct cast-nothing). It ALSO
   missed all 4 genuine mismatches (item 3 below). REPRO: `python3 tools/intent-collapse-metric.py <dir
   with the 6 deck140 files>`. FIX DIRECTION: parse the trailing `CHOICE:N` as the answer; derive the
   plan's concluded option from its NAMED action ("I will cast/hold X") matched to option text with
   stronger disambiguation; report a mismatch only when the named action != the recorded choice.
2. **A truncated answer-LAST reply can mis-select and EVADE the `unparsed_reply` guard.** vs44
   (`*0x559855409b40*`) seq34 t25: the reply hit the 2048-token cap mid-deliberation (a long
   land/mana-enumeration loop, `latency_ms` 70238) BEFORE emitting any `CHOICE:` line, yet
   `fallback: None` and `choice: 1` (= option 0, "Cast Rakdos's Return") were recorded — the plan had
   explicitly REJECTED Rakdos ("firing Rakdos's Return now is a waste of the kill shot") and was heading
   to Cruel Edict. The forced X-menu at seq35 then took X=0 -> a Rakdos's Return kill shot spent for 0
   damage/0 discard. Non-fatal (deck140 won vs44). SUGGESTED GUARD: flag as `unparsed_reply` (or a new
   `truncated_no_label`) any reply that ends without a `CHOICE:/ATTACK:/BLOCKS:` label even when a stray
   parse yields a choice — do not silently record the parse. Also relevant to brief engine-check #8
   ("any reply STILL cut before its answer line?"): YES, this one, and the answer-LAST protocol makes
   truncation costlier than answer-FIRST (the whole reasoning is discarded rather than the head captured).
   REPRO: `[r for r in seq34 record]` — reply has no `CHOICE:` substring; `fallback` is None.
3. **The 11 flagged fallbacks did NOT change any game.** 10 `unparsed_reply` + 1 `empty_reply`
   (choice -1), all on land drops, Elixir activations, or already-lost/won-anyway positions
   (vs133 seq15 & vs109 seq17 were both at life 1). vs135 seq16 (`unparsed`, choice 0 -> pass) landed on
   a C=0 board where cast-nothing was correct anyway (self-healed seq17). Keep the 4096-cap change; the
   deck140 long control-mirror deliberations are the ones that flirt with the cap.

## MODEL / REPLY-PROTOCOL (route to the reply-protocol A/B; do NOT touch the guide)
4. **Answer-after-plan residual = trailing PLAN/CHOICE mismatch that casts-a-SPELL-over-a-HOLD.** The
   head-first intent-collapse is structurally gone (no head number), but the plan's correct prose
   conclusion is contradicted by the trailing `CHOICE:N`, asymmetrically toward casting. 4 windows / 215:
   - vs44 (`*0x559855409b40*`) seq34 t25 — truncation variant (item 2): wasted Rakdos X=0.
   - vs133 (`*0x55e7bcdf1780*`) seq13 t12 — stale plan reasoned about Damnation (already cast seq12,
     not offered) + said "hold Rakdos's Return, X=3 not lethal"; `CHOICE: 2` cast Rakdos -> seq14 X=3
     into opp-19. 2nd kill shot wasted; loss was reach-starvation regardless.
   - vs131 (`*0x55908b386710*`) seq11 t8 — plan "Holding Pyroclasm is correct... cast Staff of Nin";
     `CHOICE: 1` cast Pyroclasm into `(creatures: 0)`. Won (adj 27-26).
   - vs133 (`*0x55e7bcdf1780*`) seq5 t2 — plan "opponent has no creatures to target with Cruel Edict.
     Holding removal is correct"; `CHOICE: 1` cast Cruel Edict into `(creatures: 0)`. Won.
   The model reliably NAMES its choice in prose, so an A/B arm that reconciles the trailing index against
   the named action catches all 4. deck140 = 2nd witnessing seat (after deck135). Distinct from
   head-first collapse (no head exists) and from classic plan/choice mismatch (here reasoning PRECEDES
   and is CORRECT; only the emitted index is wrong).

## ENGINE / REPRESENTATION
5. Representation is CORRECT and being READ. `(creatures: N)`, `[tapped - untaps...]`, and the shroud
   tag (`Sleep-Cursed Faerie {u} (4/4) (printed 3/3) [flying, shroud]`, vs44 seq37/38) all render and
   the pilot's PLAN quotes them accurately (Finding 1, Finding 2). `- legal targets right now: the
   opponent` renders on Cruel Edict casts (brief engine-check #3) with no confusion. The `events` field
   made the resolved-vs-asserted / counter check machine-readable (vs131 seq18 Counterspell on Lightmine
   visible). No new representation ask for deck140.
6. `Artifacts in play: you N | opponent M` rendered on the affinity/control boards (vs110, vs131) and
   the pilot did not miscount or hold on artifacts (Finding 1 artifact belief-kill, no resurrection).
   (Brief engine-check #6 is deck110's to adjudicate; deck140 saw no artifact-count misread.)

## INFRA (confirmed shipped; standing item)
7. **adjudicate-by-life-at-cap SHIPPED and confirmed** — deck140's wave-9 general-suggestion #1. The
   wave-9 AHEAD-but-timed-out grinds are now adj wins: vs110 last decision t18 life 18/17 = `adj0 18 17
   18`; vs131 t32 life 27/26 = `adj0 27 26 32` (gameend absent = capped). `-j` latency saturation still
   drives long games (per-game cumulative model latency ran into the tens of minutes on the 60-100
   decision grinds) but NO LONGER costs games. If a decision-count cap or higher `WAGIC_GPT_TIMEOUT` /
   `-j 3` is ever wanted it is a comfort, not a correctness need now.

## DECK-CONSTRUCTION (user owns; flags only, restated not relitigated)
8. **Reach starvation + no early speed bump is the ENTIRE loss column** (vs109 t13 mono-R, vs133 t15
   mono-B — both swept faithfully, held Rakdos correctly, out-raced). vs109 seq15-19 is the clean
   picture: at life 8 then life 1 the only offered spell was the below-lethal held Rakdos — nothing left
   to sweep with, 1-damage Staff clock far too slow. Options in priority (unchanged from wave 9): (a) a
   cheaper/second recurring clock that survives a counter; (b) a 1-2 mana early lifegain/blocker vs
   aggro (Lightmine/Venser's arrive too late); (c) more untapped RED so Pyroclasm {1}{r} is a reliable
   turn-2/3 sweeper. 14 sweepers is the right density — needs TEETH, not text. vs131 shows the control
   face of the same gap: Blightsteel {12} folds to one counter and Rakdos X=13 fell short of opp-29 —
   the win there came only from the cap adjudication, not from closing.
