# Wave-50 review carry-list (assembled 2026-08-27 at step-1 merge)

Corpus: seventh fair-hand corpus. Binary = master 67aeffe45 (lanes V d9144d62c, Z 5b6b44c44,
X c37805358, Y 7858a403f, W b39b80375 merged in that order). PARSETEST 1930/0
(1783 + 22 + 41 + 76 + 8). Suite expected 1142/0 + 39 AI/0 (V +1 AI, W +3 +1 AI). Guides =
wave-49 reviewer revisions (0dac414f2) + boundary pass (ce3c7a90c). Same pool/recipe.

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides (mulligan sections odds-based; skill #131 twelve-hand self-check);
synthesis owns skill + general; the core loop is the purpose. Adjudicate render predictions
by the emitter's actual string. Baselines: Baka fallbacks 0.10-0.31%; 21/21 natural x8
(wave 49: 21/21, max seat 129, max 16 priority windows/turn, max prompt 34,333).

## Set B — engine changes since the wave-49 corpus (each with its lane's prediction)
- V d9144d62c (D1): AIPlayerBaka main-phase branch clears `nextCardToPlay` after the pick is
  consumed; interrupt branch drops a carried pick no longer in hand. Prediction: casts with
  tapped == 2x pips 0/N (was 12/234); a counter on the stack followed by a payment attempt for
  the countered spell in stderr: 0.
- Z 5b6b44c44 (D11/D12/D18): repeat row on token makers reads `[you control M creatures right
  now; ... e.g. "CHOICE: 2 (<action> x<N>)"]`, `[repeat:]` tag leads with `you control M
  creatures;`; mana-only single-row windows with floating pool residue skipped unless THIS seat
  took a mana row this turn:phase (`manaOnlyWindowSkippable`); narration receipt `- Paid {cost}
  for <card> with <sources>` at every committed payment. Predictions: takes of exactly 50 with
  no arithmetic 0; verbatim `x<N>` echo -> `repeat_count_reask`; all-mana single-row windows
  with an empty stack reaching the model 0 (was 3); one `- Paid` per cast/activation with a
  mana cost (two = a D1 recurrence); generic pips paid from a multi-colour source while a mono
  source could pay: 0/N by string.
- X c37805358 (D3/D5/D13/D14/D16/D17): edict rows say who gains (`they gain 5 - the
  sacrificing player gains, not you` on `targetcontroller` scripts); `legal targets right now:`
  collapses identical entries (`Vampire #1-#322 (...) x322`); sweeper K stated as `without a
  restriction against attacking` (own turn) / `able to attack right now` (theirs); `[named: X]`
  tag + `<Owner>'s <card> named X` narration; `DRAW FORECAST (theirs):` on the punisher's own
  seat; `(text omitted)` on MDFC cast-mode rows. Predictions: gain-recipient disagreeing with
  the primitive 0/N; no option row > 3,000 chars, no prompt > 30,000 from the target clause;
  `(0 able to attack)` 0 occurrences; casts of a named card by the punished seat 0 (was 3);
  `DRAW FORECAST (theirs)` on 100% of `DRAW PUNISHERS ... yours` prompts (was 0/63); bare
  `// <name>"}` endings 0 (was 6).
- Y 7858a403f (D7/D8/D9/D10/D19): CHOICE-line grammar — CLEAN vs REJECTION (verdict/negation
  tail), rejections never execute (`rejected_line_skipped`); repeat count read from every
  choice-anchored line, last named wins; nested-paren exemplar parses, `askExemplar` strips
  `(P/T)`; `CHOICE: 0` on a no-pass ask -> last clean in-range sibling (`no_pass_sibling_taken`)
  else one `[RE-ASK]` (`no_pass_reask`); plan expiry keyed on CONTENT (verdict opener or
  names-no-action), raw echo count only reported as `plan_echo_count`; `(your plan's target is
  not on this window)` on TARGET CHOICE; short garbage decodes retried, classified
  `degenerate_decode`. Predictions: executed lines with a verdict tail 0/N; exemplar-equal
  replies failing 0/N; `CHOICE: 0` reaching Baka without sibling/re-ask 0/N; "The game is
  lost" carries 0 (was 5) while the loop-lockout `ATTACK: none` plan carries uninterrupted;
  garbage decodes reaching Baka as `unparsed_reply` 0/N.
- W b39b80375 (D2/D4/D6): MTGRevealingCards driver acts only on its OWN chooser
  (`ownChooser()`), arms option two when option one was consumed elsewhere, holds while option
  one's StackAbility is unresolved; cleanup discard routed through `AIPlayerBaka::
  cleanupDiscard` (highest-MV policy) with a GPT bundled `PUT: n, m` ask (`discard` translog
  kind) and narration `Cleanup discard (hand 9, limit 7): you chose ...`; `{this hits YOUR
  permanent}` on self-target damage/destroy rows + `Every action here targets your own
  permanent; 0 (pass) is the usual answer.` when all rows are self-target. Predictions:
  `You revealed N cards but none was a legal target` at a foreign chooser 0; edict resolutions
  with no sacrifice 0/N; every cleanup `You discarded` preceded by a `discard` record N/N;
  self-target damage/destroy rows taken 0 (was 2). NOTE: the ledger's "target rows vanish
  main1->main2" is the designed two-decline cap, not drift — re-key that prediction to "rows
  retired only after 2 pass-declines".

## Set A — wave-49 guide edits: per wave49/deck*/edit-texts.md + wave49/reviewer-boundary-pass.md.

## Carried (wave49/engine-ledger.md D15-D17, D20-D26)
R39 feed count (deck162 lane exhausted, D15); FORECAST mirror shipped (D16 -> verify); MDFC
marker shipped (D17 -> verify); D20 frontend grouping + O(n^2) RenderSpell (owner item); D21-D26
residuals; lane W's `REVEAL_DBG` runtime-gated trace (file convention; owner's rule prefers
compile-time gating — candidate cleanup); lane R's `parseBlockAssignments` range re-scan count.
