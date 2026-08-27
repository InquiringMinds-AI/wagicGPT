# Wave-51 review carry-list (assembled 2026-08-27 at step-1 merge)

Corpus: eighth fair-hand corpus. Binary = master c010ddb47 (owner Vita fixes ca8a71e14 ability
borders + dd8663146 morph liveness; lanes A 42f2eff2b, B df8ff7641, C 4cf3745a7, E 9c772cd3d,
F 7c0a04d1d, D ac7c5f5be). PARSETEST 2022/0 (1930 + 22 + 34 + 22 + 14). Suite expected
1154/0 + 41 AI/0 (A +5, B +5 non-AI; D +2 AI). Guides = wave-50 reviewer revisions
(1b3009db2) + boundary pass (d1f700d4b). Same pool/recipe.

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides; synthesis owns skill + general; the core loop is the purpose. Adjudicate
render predictions by the emitter's actual string. Baselines: Baka fallbacks 0.10-0.31%;
21/21 natural x9 (wave 50: 21/21, max seat 166, max prompt 25,793, 3 fallbacks all stale_echo).

## Set B — engine changes since the wave-50 corpus (each with its lane's prediction)
- Vita/human-seat oracle fixes (ca8a71e14, dd8663146): ability borders priced by strict
  potential + planner; morph Face-Up ability reaped after the flip and the oracle skips
  superseded (`->next`) sources. Not LLM-visible; no corpus prediction.
- A 42f2eff2b (D1, card script): Soul Shatter / Flare of Malice / Riveteers Charm sacrifice the
  highest-MV creature OR planeswalker (`*[creature;planeswalker;manacost=convertedcost:highest:
  creature^planeswalker:myBattlefield]`). Predictions: edict resolutions with no sacrifice
  0/N; Ob Nixilis-class walkers sacrificed when highest.
- B df8ff7641 (D2, engine): affordable AI casts now route through ManaEngine::planPayment
  (payTheManaCost derives a plan when none is in mind); deferFlexibleSource defers a dual's
  colour to any unused mono source; pass 1 judges on-pip per CARD and only mono/colourless
  sources; `aipay` harness directive. Prediction: `- Paid` receipts spending a multi-colour
  source while a mono source of an unneeded colour sat untapped: 0/N on plannable costs
  (was 87/362 by lane B's count, 69/386 by the seat's); residual only on {X} / foreach-Coffers.
- C 4cf3745a7 (D3/D4): in-range index + off-menu name -> `off_menu_name_in_range` ->
  `named_row_reask` family, never `stale_echo` to Baka (`mPrevWindowRows` exemption for a
  genuine echo of the previous window); `x0` on the repeat row = pass (`repeat_count_zero_pass`);
  CHOICE count vs PLAN `this window: pass`/`stop reached` -> one `plan_choice_conflict` re-ask.
  Predictions: in-range `stale_echo` fallbacks whose name is not in the prior window 0/N (was
  3/3); `ran 1 time (you named 0)` 0; repeat takes with a pass-PLAN and no conflict record 0.
- D ac7c5f5be (D5/D17/D18/D10): `- Paid` on every payment path incl. the potential-mana
  walk; a no-source payment prints `with no source tapped` (LOUD — grep it); receipt sources
  joined by `; `; JSONL `turn` = narration turn; Soul Shatter-class rows carry `{right now:
  they sacrifice <name> (MV N, their highest)}` / tie count / 0-board clause. Predictions:
  `- Paid` on every mana-cost activation N/N (was 6/9 on land animations); `with no source
  tapped` lines = the engine under-pay made visible (report count); `turn` == last `=== Turn
  N` in events N/N; edict clause victim == narrated sacrifice N/N.
- E 9c772cd3d (D6/D7/D8/D9): ability damage rows `{right now: takes N damage - DIES/SURVIVES
  (loyalty L, K left)}`; Upkeep animation rows `[Upkeep offer: ... offered again in your main
  phase / will NOT return]`; `[already owned: <names> on your battlefield]` on second-copy and
  tutor rows; `{spends K of your M untapped mana sources this turn; <card> in your hand needs
  K}` on ability rows. Predictions: ability-damage takes at a target with toughness/loyalty
  > N: 0 (was 2); Upkeep animations followed by `ATTACK: none`: 0 (was 2/11); dead Tutor /
  second-copy casts with the tag rendered: 0 (was 2+2); Hammer-class returns over a castable
  body with the clause rendered: 0.
- F 7c0a04d1d (D11/D16/D30): `{feeds: the opponent draws N extra card(s) per turn; converters on
  your battlefield: K - <names>}` on cast rows of draw engines (discard punishers counted);
  `(text omitted)` on `{target text:}` back faces; D30 measurement (log share 62.8% aggregate,
  78% of the 20 largest; ~375 B/turn log growth) — no change. Predictions: every Howling
  Mine / Master of the Feast / Dictate / Font / Puzzle Box / Forced Fruition cast row carries
  `{feeds:`; deck162 engine casts at `converters ... 0` with opp 0 creatures 0 (was 5/9);
  bare `// <Name>"}` endings 0 corpus-wide (was 34 hits).

## Set A — wave-50 guide edits: per wave50/deck*/edit-texts.md + wave50/reviewer-boundary-pass.md
(cleanup-discard orders added for 130/146/152/162 — #143 PUT-vs-PLAN metric owed this wave;
deck146 Soul Shatter keyed to Oracle).

## Carried (wave50/engine-ledger.md D12-D15, D19-D31)
Hive over-tap under the planner (lane D saw 5 taps for a 4-mana activation — pin/fix); the
walk-taps-nothing Hive-in-Upkeep shape (now loud); D31 frontend grouping (owner); log share
63-83% of the largest prompts (decision pending — latency did not move).
