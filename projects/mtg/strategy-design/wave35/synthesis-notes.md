# Wave-35 synthesis notes — TWENTIETH per-deck cycle (guide-review edition, 3 corpora)

Pool = 7, UNCHANGED. First guide-review cycle since wave-33 (wave-34 = calibration).
Material: THREE corpora (owner directive), 63 games, ALL natural completions, zero caps
(caps abolished). Pilot going forward: **Qwen3.6-35B-A3B-FP8** (owner ruling 2026-08-22).
Review criterion (owner, verbatim): win rate irrelevant; adherence + divergence quality.

## The corpus-level story

**The guide layer remains the largest authored loss source — and this cycle proved it at
three model tiers simultaneously.** Every arm-C (FP8) loss traced by a seat agent lands on
authored guide text, not pilot ceiling: deck146's mulligan-license contradiction and
missing bottoming rule (both game-losing), deck139's wrong mutate-over imperative (quoted
verbatim into the misplay), deck158's false amass line (obeyed at arm A, refuted at arm C),
deck152's missing return-swing check. The wave-33 lesson ("losses are authored, not
absent") generalizes: a wrong guide line is obeyed in proportion to the pilot's adherence
quality — the BETTER the pilot, the more a guide bug costs.

**Cross-seat promotion — the trade-parenthetical trust rule.** FOUR seats independently
authored the same teach this cycle (152 rule-4 hardening, 116 rule-9 second half, 36
blocking amendment, 105 printed-outcome rule): "the parenthetical outcome on a block line
is the engine's computed fight result — believe it, deathtouch included." Under the
pooling rule this is promoted to a CORE/render candidate for wave-36 step 1: state it
ONCE in the combat ask (or general prompt) and retire four per-guide crutches. Until the
core fix ships, the guide-side copies stand (belief-correcting crutches, 2-corpora bar).

**Second cross-seat item — generic blockers hint scoping.** Three seats (105, 152, 36)
hit the same collision between the general hint ("block only where the trade favors you"
/ "taking damage while ahead is often correct") and deck-specific numeric/infect rules.
152 and 105 scoped it guide-side; the render-side fix (hint yields to deck rules, and
"ahead" is about LIFE never poison) is docketed.

## Rotation adjudication (adherence/maturity-shaped per ruling; UNSENTIMENTAL)

ALL SEVEN HOLD for wave-36 — but five seats are ONE clean FP8 validation corpus from
freeze/exit candidacy: 105 (converging; freeze after B-line annotation validates), 158
(rails proven at three tiers; G1 dies with this revision), 139 (both real defects were
guide text, now fixed), 116 (near-mature; validate rule 9 + harness re-ask), 36 (hold —
Academy section ships UNVALIDATED, HL1 gate). 146 and 152 not ready (new load-bearing
teaches need their corpus). **Wave-36's corpus is therefore a VALIDATION wave**: same
seven seats, revised guides, FP8 pilot, standard recipe.

## Wave-36 step-1 docket (consolidated: tier reviews + this cycle's notes.md files)

ENGINE (new this cycle): Grazer-put shockland enters UNTAPPED on payment (rules bug,
139v116 s8); Silverquill fused-mode target ask ESCALATED — recurrence with a harmful pick
at arm C (146 notes, primitive-verified repro); Intrepid valor menu lists unaffordable
options (engine partial-pays, 152 s19); echo/index conflict resolves to INDEX with no
fallback signature (152: "CHOICE: 11 (add 5 counters)" executed "add 10" — invisible);
noise-only upkeep window invites hallucinated casts (152 vs105 s32).
ENGINE (carried): Spellbinder lands-only exile targets; 9-turn no-attacker-ask anomaly;
Agadeem no-op cast offered (WITNESS-DISQUALIFIED at 146 this wave, needs a probe);
Kitchen Finks hybrid castability; Ichorclaw becomes-blocked missing from B-line AND
naive-trade line (OUTRANKS the guide stopgaps — fix here retires them); 105
poison-final-attack missing blockers ask.
HARNESS/PROTOCOL: RE-ASK on illegal multi-block replies (HIGH — recoverable game loss);
one-answer-per-ask overrun now corrupts engine picks SILENTLY (fallback:None — HL11
instrument gap) and parser takes LAST CHOICE line where FIRST honors intent; empty-reply
transport drop (152 s12).
RENDER: the two cross-seat promotions above; print toughness + damage VERDICTS on
damage-target lines (158 P6); "at 0 life you LOSE" in blockers step 2; damage-order asks
bogus keyword lists + missing format contract; name the LAND in shockland pay/tap option
short-names (all three arm-C 139 fallbacks clustered there); MDFC "flip first, then Play
Land" sentence; mana line "any colour pays generic" clause.
CORE: PASS at all seven seats individually (29th consecutive); the two pooled promotions
above are the only core-lane items.

## Bookkeeping

Guides DEPLOYED this commit (7/7, full revisions). Win tables (context only): arm C
158 5/6, 152 5/6, 105 3/6, 36 3/6, 146 2/6, 116 2/6, 139 1/6. Corpus health at arm C:
0.4% fallbacks, ~0.2% budget hits, 0 degenerate, latency p50 120s, wave wall 3.5h.
Budget: FP8 reasoning p50 well inside 6000; a smaller-budget proposal can ride the
wave-36 validation corpus per the owner's standing note.
