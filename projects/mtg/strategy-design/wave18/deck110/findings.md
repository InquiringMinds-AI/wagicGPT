# Deck-110 (Etched Affinity) — Wave-18 game review (4th per-deck-rotation cycle; E6-witness + equip-churn standing watch)

**Corpus.** PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-185930/`, 6 deck110 seat
translogs (`*ai_baka_deck110-0x*.jsonl`), round-robin over 49,21,35,135,62,14 (deck17 rotated OUT
as first converged exemplar; deck14 Deep Blue entered GUIDELESS). Binary `f664539a3` (wave-17 engine
batch: (a) TARGET CHOICE sub-menu framing; **(b) hand-card PUT_INTO_PLAY dead-ends filtered from the
priority menu = the Mox E6 fix**; (c) beneficial opponent-only-target cast suppression; (d) explicit
"BLOCKS: none" example + paraphrase catch; (e) "(player, life N)" suffix strip in echo matcher).
Wave-11 guide FROZEN; live `deck110_strategy.txt` md5 `00849aa9` — byte-identical since wave-8,
**11th freeze** if held. Reply protocol = answer-after-plan + name-echo (`CHOICE: N (name)`) / blocks
protocol (`BLOCKS: B#:A#`). Card facts vs `bin/Res/sets/primitives/mtg.txt`; representation facts vs
live prompts.
Owned charges: **E6-closed confirmation** (no "Cast Card Normally" lines; no NEW dead-end shapes in
priority menus); **characterize the 6 unparsed + 1 defer**; **equip-churn standing watch (4th wave)**;
**record 4/6 -> 3/6 — decisions or matchups?**

## Record: 3 WIN / 3 LOSS (win table 110: 3/6; results.tsv confirmed)

| seat opp | result | end life (my/op) | T | one-line |
|---|---|---|---|---|
| deck49  (Dragons, guided)   | **WIN**       | 11 / -7 | 6  | REVERSAL of wave-17's flood-loss; Signal Pest go-wide + Galvanic to face; clean |
| deck35  (Slivers, guided)   | **WIN**       | 23 / -1 | 8  | dominant; Plating beatdown; Galvanic s24 to face @op-3 closed it |
| deck62  (Enchantresses)     | **WIN**       | 30 / -2 | 10 | double-Vault-Skirge + Plating; Galvanic s16 to face; untroubled |
| deck21  (Goblin Gang)       | **LOSS**      | -2 / 16 | 15 | WIN->LOSS flip; **s18 block-echo PARSE FAILURE** chumped a 9/1 Memnite into a 1/1 wither + let a 3/3 through — biting, in a game lost by 2 life |
| deck135 (Modern Snow)       | **LOSS** (adj)| 13 / 14 | 8  | razor-thin ONE-life adjudication; decision-clean; s23 post-combat equip ramble (non-biting) |
| deck14  (Deep Blue, guideless) | **LOSS**   | -7 / 10 | 21 | structural aggro-vs-control grind; got opp to 10 by T8 then ran out of gas over a T21 game |

results.tsv: `110 49 0 11 -7 6`, `110 21 1 -2 16 15`, `110 35 0 23 -1 8`, `110 135 adj1 13 14 8`,
`110 62 0 30 -2 10`, `110 14 1 -7 10 21` — all consistent. **The matchups largely INVERTED vs
wave-17**: last wave deck110 BEAT deck21 (rematch) and deck135 (adj) and LOST to deck49; this wave it
LOST to deck21 and deck135 and WON deck49. On the flattest win-table yet (every deck 2-4 wins),
single-decision/variance swings decide matchups. This is a **FREEZE wave** (11th) — no decision-level
strategic class earned a guide change — but it is NOT a clean-canary wave: a NEW harness class (the
block-echo name-format parse failure) surfaced with a biting repro.

---

## FINDING 1 — E6 CHARGE / CONFIRMED CLOSED: "Cast Card Normally with Mox Opal {rules text}" appeared 0 times corpus-wide (was 5 fallbacks at this seat last wave). The wave-17 batch (b) hand-card PUT_INTO_PLAY dead-end filter shipped and works. No NEW dead-end shape *drives a fallback* in priority menus.

Last wave's #1 fallback driver — the mislabeled Mox-tap surfaced as a top-level (often SOLE) GPT action
— is **gone**. `grep`-count of "Cast Card Normally" across all option/prompt/chosen text this corpus =
**0** (was the seat's dominant fallback, 5/7 non-defer fallbacks in wave-17). E6 CLOSED, confirmed by
absence. The two-lever fix I proposed (relabel / suppress-the-no-op) was resolved via the stronger
lever (suppress hand-card put-into-play dead-ends). Layer: ENGINE/REPRESENTATION, **RESOLVED**.

**"No NEW dead-end shapes in priority menus" — one residual class characterized, NON-biting.** The E6
fix removed the *hand-card* dead-end but a related shape remains: **single-option priority menus whose
SOLE option is an "(ALREADY attached to it - this would change NOTHING)" equip re-offer**. These now
appear as `options=1` menus (deck21 s16/s17/s29, deck62 s28/s31, and the `{B}{B}` re-attach variant at
deck135 s10/s11 — note the lowercase "attach ... [cost: {b}{b}]" label). Unlike the E6 Mox menu, **the
model navigates every one of these cleanly by PASSING (choice=0) — zero fallbacks**. So this is a
dead-end SHAPE but not a dead-end FAILURE: the guide's Rule #1 + the explicit "(ALREADY ... change
NOTHING)" tag give the model a readable no-op to decline. It is the SAME underlying representation
issue as E6/E2 (surfacing behavior-neutral no-op actions, sometimes as the sole option) and is covered
by the same suppress-the-no-op lever (E2). Priority LOW (non-biting), route to the E2 ledger note.

Also-single-option and clean: the Steel Overseer activation surfaced as sole option (deck35 s21, deck14
s22/s24/s28/s31/s33 — label " 1/1 Counter with Steel Overseer [cost: Tap]", the mangled " 1/1 " prefix
is cosmetic) — all parsed, no fallback; the guide's "take the Overseer tap every turn" rule held.

## FINDING 2 — NEW HARNESS CLASS (the wave's substantive item): the BLOCKS-echo matcher accepts ONLY the `B#:A#` label format. When the model declares a block using creature NAMES, the reply goes `unparsed_reply` and the heuristic fallback makes a DIFFERENT block — biting at deck21 s18.

Two of the seat's six unparsed fallbacks are `blockers` decisions where the model wrote a **rules-sane,
clearly-intended block using creature names instead of the B#/A# labels**, and the matcher dropped it:

- **deck49 s12** (T3): reply's last line `BLOCKS: Ornithopter blocks Dragonmaster Outcast` -> unparsed
  -> choice=0. **Non-biting**: the heuristic fallback happened to make the SAME block the model wanted
  (Ornithopter blocked the Outcast, took 1, survived — events at s13). Intent and fallback coincided.
- **deck21 s18** (T7): reply's last line `BLOCKS: Memnite: Boggart Ram-Gang` -> unparsed -> choice=0.
  **BITING.** The model's plan was to block the 3/3 Boggart Ram-Gang with its huge Memnite and let the
  1/1 Scuzzback Scrapper through. The dropped reply fell to the heuristic, which made the **opposite,
  materially worse block**: events show `Memnite -> Scuzzback Scrapper` (a 1/1 WITHER creature) and
  `Boggart Ram-Gang -> your face for 3`. The 1/1's wither put a -1/-1 counter on the 9/**1** Memnite,
  dropping its toughness to 0 -> **Memnite died to a 1/1**, while the 3/3 hit face (life 14->11) and
  **survived** (it kept attacking; s20 shows it hitting for 4 more with Orcish Oriflamme). deck110 lost
  this game **-2/16** — the extra ~2 life and the surviving 3/3 are plausibly game-flipping (this is
  the wave-17 REMATCH that deck110 WON clean; the flip has a real harness cause).

**This is precisely the "paraphrase catch" the wave-17 batch (item d) added — but only for the priority
menu / "BLOCKS: none".** The block-ASSIGNMENT matcher was not extended to resolve creature NAMES to
B#/A# labels the way the `CHOICE:` name-echo already resolves card names to option numbers. Proof the
label format still works: deck49 s19, deck21 s24, deck21 s30 all wrote `BLOCKS: B1:A1` and parsed
cleanly (choice=1). So this is a pure MATCHER GAP, not a model-reasoning defect — the model's blocks
were legal and (at s18) strategically correct in shape; the harness just couldn't read the name form.

Note a secondary model bug visible inside s18 (does NOT change the finding): the model wrote "Memnite
has 9 toughness, survives with 6" — it read the 9/1's POWER as toughness. But even with that error its
intended block (trade Memnite for the 3/3, take 1 from the 1/1) was strictly better than the fallback
(trade Memnite for the 1/1, take 3 from the 3/3, leave the 3/3 alive). The harness dropped a better
play for a worse one.

**Layer: ENGINE/HARNESS (block-echo matcher), OPEN, NEW, priority MEDIUM-HIGH** (has a biting repro in
a lost game). Repros: **deck21 s18 (biting), deck49 s12 (non-biting/coincident)**. Contrast (parsed):
deck49 s19, deck21 s24/s30. **NOT guide-fixable** — reply-format/echo-parsing is the prompt+matcher's
job, layer-routed away from strategy prose; and the failure is cross-deck (every deck blocks), so the
durable fix is the matcher extension, not a deck110 guide line. See notes.md + general-suggestions.md.

## FINDING 3 — the remaining 4 unparsed are LONG position/state rambles (12-14k chars), all in the 3 LOSSES; NONE is a PLAN-caveat stale-plan spiral (H1 stays dead). This is the "giant-reply overthink" pocket, position-driven, not a decision-quality regression.

- **deck21 s25** (14.3k, ask, at **2 life**): desperation lethal-hunt. Options were only `Cast Signal
  Pest` / `Cast nothing`; the model rambled looking for a win that did not exist. Position ramble.
- **deck21 s28** (12.4k, priority equip, at **2 life**): the classic `Wait...Wait...Wait` loop —
  identical lines repeated ("I deal 5 damage, opponent goes to 11. Next turn they attack. I take 2. I
  die."). A hopeless-position spiral, not a carry-vs-prompt contradiction (the real option, Equip
  Signal Pest, was on-menu; the model just couldn't commit to a losing line).
- **deck135 s23** (14.0k, priority equip, 5 opts incl. 2 ALREADY): an equip-vs-attack DAMAGE
  OPTIMIZATION overthink (computing 20 vs 21 total damage) that never emitted a `CHOICE:`. Note it was
  **Main phase 2, AFTER combat** — equipping could not have affected that combat, so the whole
  computation was moot; the model didn't recognize the post-combat context. Non-biting (wasted round
  trip in a game already headed to a 1-life adjudication).
- **deck14 s25** (14.4k, ask land-drop, **carries the PLAN-caveat**): Steel-Overseer activation
  timing/phase confusion ("it is now untapped and ready? ... This is a bit ambiguous"). A genuine
  game-state/phase confusion, NOT stale-plan re-litigation. The caveat was present but the ramble was
  not the class the caveat targets.

**Confirmation for the brief:** none of the 6 unparsed is a stale-plan spiral / carry-vs-prompt
contradiction loop — the H1 (wave-16) class stays **dead** (resolved-watch holds). The 4 rambles are
long overthinks concentrated in desperate or confused positions; **all 4 fall in the 3 LOST games**
(deck21 x2, deck135, deck14), while the 3 WINS carried at most 1 fallback (deck49 s12, non-biting).
Ramble frequency tracks POSITION (losing/confused), not a change in decision quality.

## FINDING 4 — E2 (equip churn) standing watch, 4th wave: ZERO `(ALREADY — change NOTHING)` options taken corpus-wide again. 33 equip decisions, Rule #1 obeyed 100%. Validated-by-absence a 4th consecutive time.

33 `Equip with Cranial Plating` decisions across all 6 games; **`took_ALREADY=False` on every single
one**. Where an ALREADY re-offer sat alongside a real equip the model took the REAL equip (deck135 s15
-> Signal Pest, deck135 s20 -> Master of Etherium, deck62 s14/s20 -> Vault Skirge); where the sole
option was an ALREADY re-offer the model PASSED (deck21 s16/s17/s29, deck62 s28/s31). Every real equip
was value-positive (fresh carriers / new Platings / stacking onto the best body). Two equip decisions
went unparsed (deck21 s28, deck135 s23) but those are the Finding-3 rambles (choice=-1), NOT
ALREADY-takes. The identity-keyed net-zero-MOVE blind spot (E2's one unfixed lever) again got no
exercise — every move raised resulting power. Layer: engine ledger, **UNFIXED but non-biting, 4th-wave
validated-by-absence**. MEDIUM/dormant. The lever is unchanged: compute the no-op marker from OUTCOME
DELTA (suppress "Equip ... targeting X" when moving would not raise X's power above its current
carrier), which also subsumes the Finding-1 sole-option ALREADY menus.

## FINDING 5 — the 1 defer was a benign forced Glimmervoid land-drop (correct). Down from 3 defers last wave.

deck135 s3: `prompt=""`, `latency=-1`, `choice=-1`, `chosen="Glimmervoid"` — the harness deferred a
forced land-play to the heuristic (no LLM round-trip) and it played a Glimmervoid. Never wrong for this
deck (free untapped any-color land + artifact fodder). Correct, cost nothing. Harness working as
intended. No item. (Only 1 defer this corpus vs 3 last wave — the brief's corpus-wide "1 defer" is
this one, at this seat.)

## FINDING 6 — RECORD 4/6 -> 3/6: PRIMARILY matchups/variance, with ONE real harness-caused biting misplay. Not a decision-quality or guide regression.

- **deck135 LOSS (adj 13/14)**: decided by **ONE life** on a T8 adjudication after deck110 chipped the
  opponent 20->14 early. A coinflip-thin race; the s23 ramble was post-combat and non-biting. Variance.
- **deck14 LOSS (-7/10, T21)**: deck110 executed its plan (opponent 20->10 by T8 via Galvanic-to-face)
  then **ran out of gas** vs a stabilizing blue control deck over a 21-turn grind. Structural bad
  matchup (aggro that must kill by T5-6 vs control that survives to stabilize) — deck14 is a fresh
  guideless Deep Blue deck that plays a competent control game. Matchup, decision-clean.
- **deck21 LOSS (-2/16)**: the WIN->LOSS FLIP. Here the loss has a genuine **harness contributor** —
  the s18 block-echo parse failure (Finding 2) dropped a strictly-better block for a catastrophic one
  in a game lost by 2 life. Not a guide/strategy failure; a matcher gap.

Net: the drop is two matchup/variance losses (deck14 structural, deck135 one-life) plus one loss with a
real, fixable harness cause (deck21 block-echo). The model's STRATEGIC decisions were sound throughout
(Galvanic 6/6 to face, equips 33/33 value-positive, target sub-menus 6/6 correct). The guide is not
implicated. The actionable signal is the block-echo matcher (Finding 2), an engine item.

## FINDING 7 — standing validations (verification only, no items):
- **Galvanic Blast targeting — 6/6 to the opponent's face.** Every Galvanic target sub-menu resolved to
  `The opponent (player, life N)`: deck49 s6, deck21 s13, deck35 s24 (@op-3, closer), deck62 s16, deck14
  s10/s15. NO self-target, NO own-creature target — even though own creatures / "you" were listed as
  legal targets on several. The batch-(c) beneficial-target suppression did not need to fire. TARGET
  CHOICE sub-menu framing (batch a) worked cleanly at this seat — 0 target-sub-menu fallbacks.
- **Name-echo (`CHOICE:`)** — every parsed choice carried its name; 0 wrong remaps. The "(player, life
  N)" suffix strip (batch e) is visible working (chosen_text `The opponent (player, life 20)` parsed).
- **Attackers** — all attacker windows resolved to eligible sets; no misfires.
- **E6 hand-card dead-end filter (batch b)** — confirmed by absence (Finding 1).

---

## CHARGE VERDICTS
- **E6-closed confirmation**: **CONFIRMED CLOSED.** 0 "Cast Card Normally" corpus-wide (was 5). No NEW
  dead-end shape *drives a fallback*; one residual non-biting shape (sole-option ALREADY equip menus)
  characterized and routed to the E2 ledger. E6 -> RESOLVED.
- **6 unparsed + 1 defer characterized**: 2 = NEW block-echo name-format parse failures (deck21 s18
  BITING, deck49 s12 non-biting); 4 = long position/state rambles in the 3 losses (deck21 s25/s28,
  deck135 s23, deck14 s25), NONE a stale-plan spiral; 1 defer = benign correct Glimmervoid drop.
- **Equip-churn standing watch (4th wave)**: **no recurrence.** 33/33 equips value-positive, ZERO
  ALREADY taken, Rule #1 100%. Validated-by-absence 4th time. Dormant lever unchanged.
- **Record 4/6 -> 3/6**: **matchups/variance + ONE harness cause.** deck14 structural + deck135 one-life
  adjudication (both decision-clean) + deck21 flip via the block-echo parse failure. Not a guide/
  decision regression.

## HANDOFF
1. **FREEZE the guide verbatim** (md5 `00849aa9`, byte-identical; **11th freeze**). No decision-level
   strategic class earned a change; E6 is closed (guide already correct), block-echo is a harness matcher
   gap (layer-routed away from guide prose, cross-deck), equip churn is clean.
2. Route to notes: **E6 -> RESOLVED** (confirmed by absence); **BLOCK-ECHO NAME-FORMAT -> NEW OPEN,
   MEDIUM-HIGH** (biting repro deck21 s18; matcher must resolve creature names to B#/A# labels);
   **E2 dormant/unfixed** (validated-by-absence 4th wave; subsumes the sole-option ALREADY menus);
   **H1 stays RESOLVED** (no stale-plan spiral among the 6 unparsed).
3. Route to general-suggestions: the **block-echo name->label reconcile** is CROSS-DECK and the
   highest-leverage new representation fix (mirrors the working CHOICE name-echo). The E2
   outcome-delta no-op suppression remains the standing cross-deck item.
4. Rotation verdict in notes.md.
