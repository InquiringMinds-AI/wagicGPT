# Wave-52 review carry-list (assembled 2026-09-01 at corpus launch)

Corpus: ninth fair-hand corpus, `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260901-223430/`.
Binary = hermetic `make -B` of master dddb6f62b (last engine commit c9dc6b75c); gate 1210
(2 = lifeline/merrow concurrency pair, solo-green) + 44 AI/0; PARSETEST 2121/0. Model:
qwen36-35b-a3b (FP8) on Spark :8084 (the 122B is retired - owner 2026-09-02). Guides = wave-51
reviewer revisions (5d0c24d28) + boundary pass (6686392d3). Same pool/recipe as wave 51
(-j 21, -T 0, WAGIC_GPT_TIMEOUT=900, MAXTOKENS=6000, REPPENALTY=1.05).

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides; synthesis owns skill + general; the core loop is the purpose. Adjudicate
render predictions by the emitter's actual string. Baselines from wave 51: 21/21 natural,
2,171 decisions, 5 fallbacks, max seat 301, max prompt 33,536, 452 `- Paid` receipts / 0
no-source.

## Guide-literal drift to check FIRST (wave-51 B3 obligation, NOT done at deploy)
deck123 and deck126 guides still cite the retired `[already owned:` literal (deck123 x1, deck126
x1) and deck123 cites `[from exile]` x3; lane K replaced the emitter forms (see D10/D11 below).
Re-key those sentences in your revision; count how often the model met the NEW literal.

## Set B - engine changes since the wave-51 corpus, each with its falsifiable prediction

Wave-52 step-1 lanes (the wave-51 docket D1-D17):
- G 8c3e10e28 (D1 livelock, D2 label echo, E-1 PUT own-label): while a repeat-N plan
  executes, no cast/land ask is opened; stale-drop streak scoped to one board. Predictions:
  `stale_livelock` 0/N (wave 51: 1, deck123 vs126 seq 48); `Cast nothing` echoes never
  execute an edict row carrying "does nothing" (0/N); PUT: seams never consume a CHOICE: line
  (deck162 vs146 seq 17 shape 0/N).
- H d58aeba6b (D4): Overgrown Battlement / Tolarian Academy foreach producers tapped ONCE for
  their full output. Prediction: `{paying this taps: <wall>, <wall>}` for a cost one wall covers
  0/N; `- Paid` receipts naming two Battlements for <= their single output 0/N.
- I 4bdca64bf (D5 / H1): instant-speed casts offered at the AI seat's OPPONENT-turn priority
  stops (empty stack, NextGamePhase on top). Prediction: opponent-turn windows with an instant
  in hand and no Cast row 0/N (wave 51: 41/51); Terror-class removal resolving on the
  opponent's turn N > 0.
- J aa8509282 (D6 index/name, D14 prose verdicts): exact short name outranks a disagreeing
  index; ambiguous conflicts re-ask once (`index_name_conflict`); prose pass verdicts after the
  CHOICE line trigger `plan_choice_conflict` on ANY taken row; `plan_missing` re-ask for a
  counted repeat take with no PLAN. Predictions: `name_over_index` recorded with the named row
  executed N/N; index-executes-a-dead-row (deck123 vs125 seq 48 shape) 0/N;
  `decision_reversed_in_prose` re-asks: count + what the second answer did. NOT verified live
  by the lane - this corpus is the gate.
- K b1fba8eff (D7 life-cost, D9 self-hit, D10 owned forms, D11 from-exile): `{paying this
  costs you N life (...) - you would be at K}` / `- this KILLS you`; `{this hits YOUR
  permanent}` on exile/graveyard/library/hand moves and sacrifice inside loyalty/activated
  wrappers; `[legendary: ...]` / `[second copy: ...]` / tutor `[finds only ...]` forms;
  `{castable from exile - your card, exiled by ..., it costs {N} more ...}`. Predictions:
  Talisman/painland cast at <= the damage with no life clause 0/N; a self-hitting exile/bounce
  row taken at <= 1 life 0/N (Kaya -3, wave 51); a legendary second copy cast with no clause
  0/N; tutor rows warning against the half the pilot LACKS 0/N.
- L 0f7dff3b0 (D8 collapse, D12 repeat row, D13 animated land, D15/D16/D17 pricing):
  target-handle rows collapse to `#a-#b`; a `becomes` row on an already-animated land is
  withheld for the turn; the planner taps an animated land LAST; -N/-N rows carry
  DIES/SURVIVES; fetch rows say "makes no mana"; cast rows carry the `{spends K of M}`
  clause. Predictions: longest non-log prompt segment < 30 KB with no 20+-row Equip block;
  `becomes` re-taken in the same window 0/N (wave 51: 3/9); animated land tapped for mana
  the turn it was animated 0/N (wave 51 D13); -N/-N rows show DIES/SURVIVES N/N.

Owner-report batches merged AFTER the lanes (not LLM-targeted; report windows only):
- bd515aad7: per-half instant timing for split/adventure/aftermath (ManaCost::instantSpeed);
  ~60 scripts to Oracle; keyword lint 0 warnings. Prediction: an adventure/omen SORCERY half
  offered at instant speed 0/N; instant halves offered on the opponent's turn N > 0.
- c15106f4d: Calamity Bearer = doubling trigger (not double strike); Drake Hatcher; morph/
  alternative cast-mode menus priced through the planner; phase automation holds while a
  human display is open (human seat only). Prediction: Bearer damage doubled at normal timing.
- 4abcf5dd0: phases survive one tick before automation may leave them (Delver upkeep root
  cause). Prediction: upkeep-trigger reveals fire N/N when the phase is auto-skipped.
- 2026-09-01 Vita batch (c68ac1366..9385f6f6a): `bycontroller` counter-trigger scoping
  (Hapatra), `[-sub_hydra]`-class subtype vs keyword collisions, evolve compares at
  resolution, Wilt-Leaf discard-to-play abilities live, Envoy bestow reduction, aura rehook
  re-binding, Hurkyl reveal soft-lock, equip double-registration/leak, Pridemate art. Corpus
  relevance: any seat with counters-matter / evolve / equipment; predictions: an equip
  activation menu growing per cycle 0/N; evolve triggering off an enters-with-counters
  creature N/N.
- c1a0bf814 option-preserving autotap: HUMAN seat only; AIPlayerGPT `{paying this taps}`
  forecasts pass preserveOptions=false -> forecast == the AI's actual taps. Prediction:
  forecast-vs-`- Paid` source mismatch 0/N (report any).
- 5ca11d6e2 Baka targeting (ATransformerInstant verdict; live-ability judge): affects the
  HEURISTIC fallback only. Prediction: a Baka-fallback pump aimed at the opponent's creature 0/N.
- 1245b7d68 transcripts: dev-build feature; corpus games write User/transcripts/ - not a
  review input this wave (harvest them with the corpus for later replay).

## Set A - guide edits carried (wave-51 reviewers + boundary pass B1-B8)
Adjudicate per seat file (wave51/seats/) as in wave 51: each edit's predicted decision class
disappearing; the #153 single-property mulligan audit across six guides (B-pass) -
record mulligan offered/taken/kept per seat under the new-baseline rules.
