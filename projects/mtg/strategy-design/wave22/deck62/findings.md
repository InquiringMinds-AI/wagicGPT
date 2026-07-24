# deck62 (Enchantresses) — REVISED-GUIDE ROUND 6 validation findings (wave 22) — the N9 witness seat

Corpus: `matchups-20260723-173843`, binary `1d8ed8194`. deck62's SIXTH guided corpus. The guide is byte-frozen at
`Res/ai/baka/deck62_strategy.txt` (8893 bytes; `cmp` IDENTICAL to wave21/deck62/strategy.txt and to this wave's
strategy.txt). 6 owner translogs, **149 decisions** (ask 114, attackers 25, blockers 10; 0 reveal, 0 priority-at-seat).
Record **4/6** (unchanged from wave-21's 4/6). deck62 was held in the pool for exactly one reason: to WITNESS the
HARNESS-N9 retraction-detector fix (reference-scoping, batch item b) against the seat that owned 3 of wave-21's
false positives.

Bottom line: **the N9 false-positive class is GONE — and confirmed the STRONG way, not the weak way. The exact
wave-21 FP reply shape (a clean first-line `CHOICE` + a downstream "Wait,"/"Actually," self-correction in the PLAN
prose) is NOT merely absent this corpus — it is STILL BEING PRODUCED 27 times, including 10 in the EXACT wave-21 FP
context (a `Play Forest` / `Hold Forest` land-drop menu), and EVERY ONE parsed clean (stated CHOICE == executed
choice; 0 fallbacks). deck62 had 4 fallbacks in wave-21 (3 of them N9 over-fires); this corpus it has 0. The
reference-scoped detector (item b: the retraction phrase must precede a line-leading `PLAN:` AND reference the chosen
option) correctly no-fired on all 27 same-shape replies.** Edit D re-validated round 2 (10 clean blocks, no
favorable-block spiral); the snap-then-revise pattern is still present and still rescued by last-wins — this corpus on
a BLOCKS ask with real stakes (vs27 s16), not just land drops. N8 gang-chump remains UNEXERCISED (0 block fallbacks).
Guide disposition: **BYTE-FREEZE (round 6)**. Rotation: **deck62 is now a clean rotation CANDIDATE — its named
keep-reason (N9 witness) has DISCHARGED** (see notes.md).

## Record: 4/6 — unchanged; matchup-shape floor, decisions healthy

| vs | result | life (me/opp) | turn | read |
|---|---|---|---|---|
| deck102 (Tergrid)  | **WIN** | 10 / -1 | 12 | Ground beatdown; attacked every combat T2-T12. Win line fired. |
| deck27  (Zombies)  | **WIN** | 12 / -4 | 8  | Canopy Spider pumped to 5/9+ (Ancestral Mask), go-wide with Saprolings; opp walled less than the wave-21 loss. |
| deck109 (Mono-R Aggro) | **WIN** | 9 / -1 | 12 | Stabilized vs haste aggro (Legion Loyalist / Stromkirk / Ash Zealot), out-carded into the win. |
| deck140 (RBW Control) | **WIN (adj)** | 20 / 2 | 22 (cap) | Life-adjudicated: deck62 **20** vs the grinder's **2** — dominant. Ground "the grinder" out; decisively ahead at cap. |
| deck44  (Faerie Tempo) | LOSS | -1 / 5 | 13 | Race loss — attacked every turn T6-T12 but out-tempo'd. Not a durdle; a clock deficit. |
| deck135 (snow/Treefolk) | LOSS (adj) | 7 / 17 | 11 (cap) | Life-adjudicated 7 vs 17 — the recurring out-sizes matchup floor (Icehide Golems, Ice-Fang Coatl deathtouch flyers). |

Newcomers (109/44/140) have no same-pool baseline this cycle, so cross-wave record comparison is WEAK evidence (pool
swapped). Same structural verdict as waves 18-21: **turn-8-13 ground/go-wide deck; floors against faster (deck44) and
bigger (deck135) clocks, and the guide cannot draw a faster clock.** Win column is context-only per Step 0. Decision
quality is the signal: **149 decisions, 0 without a coded line, 0 empty chosen_text, 0 fallbacks; attack-every-turn
fired in all 6 games, 0 `ATTACK: none`, 0 durdles.**

## OWNED ITEM 1 — N9 validation: the FP class is GONE, confirmed on the LIVE SHAPE (strong evidence)

**deck62 had 4 fallbacks in wave-21 (3 retracted_choice N9 over-fires + 1 stale_echo). This corpus: 0 fallbacks.**
The brief's per-seat table (140:4, 102:3, 135:1, everyone else 0) and my independent sweep agree — deck62 is clean.

The brief asked for the STRONG validation, and it is available: not "the shape didn't recur" (weak — could just mean
the behavior stopped) but "the shape recurred and now parses." **The wave-21 FP shape is a reply with exactly one
first-line `CHOICE` and a downstream self-correction phrase ("Wait," / "Actually," / "Correction:") living in the
PLAN/reasoning body. That shape appears 27 times this corpus, and all 27 parsed clean (single `^CHOICE:` line, stated
option == executed `chosen_text`).** Of the 27, **10 are the EXACT wave-21 FP context — a `Play Forest` / `Hold Forest`
land-drop menu with a "Wait,"/"Actually," in the PLAN body**:

- vs102 s19, vs140 s22, vs140 s32, vs27 s5, vs27 s11, vs44 s2, vs44 s3, vs135 s15, vs109 s5, vs109 s19 — every one
  `CHOICE: 1 (Play Forest)`, parsed to `Play Forest`, no fallback.

**Verbatim witness — vs27 s11 (T6, land-drop, the wave-21 FP shape reproduced and PASSING):** menu
`['Play Forest', 'Hold Forest - do not play it now']`. Reply opens `CHOICE: 1 (Play Forest)`, then the PLAN spirals
with repeated self-correction phrases — *"...2 Forests on board = 2 mana. **Wait,** the board says 'Mana available:
{g}{g}{g}...'"*, *"**Wait,** I need to check the 'Mana available' line again"*, plus a paragraph re-deriving
protection-from-green mechanics — none of which retracts the land drop. Parsed clean: `choice=1`, `chosen_text='Play
Forest'`. **This is precisely the vs27 s10 / vs135 s5 / vs102 s3 wave-21 over-fire shape; the reference-scoped
detector no-fired.** (Note: the literal token `"Correction:"` did not appear anywhere this corpus, but `"Wait,"` — the
other phrase present in wave-21's FP replies, e.g. the vs27 s10 "Wait, Fists says..." — recurs 27x. So the evidence is
shape-present, not the weaker shape-absent.)

**Both of the corpus's 2 remaining retracted_choice fires are at deck102's seat (vs62 s21, vs140 s29) — NOT deck62**
(brief). deck62, the seat that OWNED the over-fire, now produces the triggering shape 27x with zero fires. The
detector fix landed exactly where it was aimed. Whether deck102's 2 remaining fires are correct disavowals or residual
FPs is deck102's owned call this wave.

## OWNED ITEM 2 — edit D (favorable-block litigation) ROUND 2: re-validated, spiral class still dead

10 blocker decisions, **all committed a first-line `BLOCKS`, all parsed, 0 fallbacks, 0 chump-litigation spirals.**
The favorable-block annotations rendered and were obeyed without re-litigation:

- **vs102 s16 (T7):** `BLOCKS: B1:A1, B2:A2` — both Saprolings into attackers, annotation "you kill it, your blocker
  lives". 965 chars, committed on line 1, no chump agonizing. Edit D's target class, handled clean.
- **vs135 s9 (T6):** `BLOCKS: B2:A1` — Yavimaya (2/2) into Icehide Golem (2/2), annotated "(both die)". 6768 chars but
  `chump` mentioned 0 times — legitimate defensive trade math (which body to trade), NOT a chump-block spiral, and it
  committed to a single BLOCKS line. Not the wave-20 15.5k spiral class.
- **vs135 s22 (T10):** `BLOCKS: B2:A1, B2:A2, B3:A3, B3:A5` — 4463 chars, committed. (Rules-slip below.)

No block reply this corpus re-litigated whether a favorable trade is a chump-block. **Edit D's decisive exception
("a favorable block with a REAL creature is a trade, not a chump — make it and move on") holds across a second
corpus.** Discharged again.

## OWNED ITEM 2b — snap-then-revise: STILL present, STILL rescued by last-wins — this time on a BLOCKS ask with stakes

The brief asked whether the wave-21 snap-then-revise pattern (first-line answer worse, last-line corrected, rescued by
last-wins) persists. **It does, and this corpus it fired on a consequential BLOCKS ask, not just a land drop:**

- **vs27 s16 (blockers, T7, 18/13):** snap `BLOCKS: B1:A2` (chump the 0/1 Birds of Paradise into Walking Dead —
  a GUIDE-VIOLATING chump-the-dork, exactly what the #1 defensive rule forbids when not stopping lethal) -> 6118 chars
  of reasoning that explicitly cites the guide (*"taking damage while ahead is often correct"*, *"block only where the
  trade favors you"*, "losing a card for no combat damage is bad") -> re-emits `BLOCKS: none` (correct: 18->12 is not
  lethal, keep the dork). **Parser took last-wins -> `none` (`chosen='no blockers'`, `choice=0`). The snap answer was
  WORSE and the reasoned last answer was BETTER; last-wins captured the improvement.** The reply did NOT truncate (it
  completed through PLAN), so the correction landed.

The two attacker multi-coded-line replies (vs135 s21 `['A2','A2']`, vs109 s23 `['A1','A1']`) both re-emitted the SAME
choice — no divergence. **The structural exposure flagged in wave-21 — a truncation BETWEEN the snap answer and the
correction — did NOT bite: 0 unparsed, 0 empty, 0 truncated replies this corpus.** It remains a latent cross-seat
watch (general-suggestions #1), now with a fresh BLOCKS-ask witness proving the pattern reaches consequential asks, not
only land drops.

## OWNED ITEM 3 — N8 blocker-fallback gang-chump: UNEXERCISED (report null, again)

**Zero blocker fallbacks this corpus** — all 10 blocker decisions committed a first-line `BLOCKS` and parsed clean
(`chosen_text` populated and coherent on every one). The heuristic block-fallback path — where the item-(h) gang-chump
fix (0-power creatures skip kill-blocks / never pile as a 2nd blocker) lives — **NEVER RAN at this seat.** Answer-first
continues to make block fallbacks non-existent at deck62 (as in wave-21). Report null: N8 could not be validated here
and, structurally, cannot be validated at this seat — deck62 under answer-first does not produce block fallbacks. N8's
validation must come from a seat/corpus where a block decision actually falls back with a valuable 0-power body
present. See notes.md N8.

## VALIDATION FOCUS — answer-first still holding; over-deliberation is now pure latency, not parse failure

- **Answer-first + parser: no regression.** All 149 decisions parsed (0 missing coded line, 0 empty chosen). The
  longest replies still lead with a coded line: **vs135 s24 (13,511 chars, 198.5s latency — the corpus max) is a CAST
  ask that opened `CHOICE: 1 (Cast Ancestral Mask)` and parsed clean**, then correctly picked its OWN creature at the
  target step (s25: `Druid of the Cowl [your battlefield]`, chosen over 5 opponent creatures on the menu — no
  enchant-opponent-gift violation even under a 13.5k-char spiral). This is the wave-21 headline-2c pattern confirmed:
  the >12k-char over-deliberation spiral is undiminished as a token/LATENCY cost (median 16.9s, p90 74.4s, max 198.5s)
  but answer-first has fully neutralized its PARSE harm.
- **Carryover — attack-every-turn / A-list: clean.** All 25 attacker decisions parsed, 0 fallbacks, 0 `ATTACK: none`,
  0 durdles. Go-wide fired in the wins (e.g. vs27 s16 events "declared attackers: Canopy Spider, Saproling, Saproling").
- **Carryover — enchant-opponent-gift ban / Argothian-first / engine-protection: no violations observed** (vs135 s25
  is the positive proof — own-creature target picked from a mostly-opponent menu).
- **Carryover — computed-P/T distrust pocket: one SOFT residual, decision-harmless.** vs27 s19 shows a mild
  recount-of-printed-P/T during combat-survival math ("my calculation of the Spider's power might be wrong. Let's
  recount the enchantments on Spider. Board says 'Canopy Spider (13/15)...'") — the model briefly distrusts its own
  arithmetic vs the printed 13/15, reconciles to the board, and does NOT misplay (game won). The other two signature
  hits (vs135 s18 "recount mana", vs135 s24 prospective Mask math) are benign. This is the N6 over-deliberation
  behavior surfacing as latency, not the harmful distrust pocket returning; the guide line ("TRUST that printed [X/Y]")
  already targets it. Not guide-actionable — latency-class, route to the model/decode watch.

## Minor model rules-slip (not guide/fallback) — carried class

**vs135 s22 (blockers):** the model declared `BLOCKS: B2:A1, B2:A2, B3:A3, B3:A5` — assigning one blocker (B2) to TWO
attackers and another (B3) to two — an illegal double-block. The engine resolved to first-legal: `Saproling blocks
Ice-Fang Coatl; Saproling blocks Icehide Golem`. Same minor rules-error class as wave-21's vs14 s21 (model declares one
blocker against multiple attackers; engine takes the first). Not a fallback, not a guide concern — a model rules error
the engine handles. One-line note only.

## Guide disposition: BYTE-FREEZE (round 6) — no edit warranted

Every taught class held; the N9 witness role discharged clean; every open item is HARNESS/ENGINE lane and unreachable
by guide prose. No guide contradiction, no #1-band leak, no new model-fixable pathology. strategy.txt is a
byte-identical copy of the deployed guide (`cmp` clean, 8893 bytes). See notes.md for the rotation verdict — with the
named keep-reason (N9 witness) discharged, deck62 is now a clean rotation candidate.
