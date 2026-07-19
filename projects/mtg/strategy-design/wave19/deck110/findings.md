# Deck-110 (Etched Affinity) — Wave-19 game review (5th per-deck-rotation cycle; block-echo-fix witness + equip-churn 5th-wave standing watch)

**Corpus.** PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260719-092058/`, 6 deck110 seat translogs
(`*ai_baka_deck110-0x*.jsonl`), round-robin over 27,35,49,14,62,135 (deck21 rotated OUT as 2nd converged
exemplar; deck27 Zombies entered GUIDELESS). Binary `72b05535d` — LARGE delta since wave-18's `f664539a3`:
**(a) wave-19 engine batch `4becc83be`** — combat name tolerance (parseBlockAssignments name→label second
pass), per-pairing TRADE OUTCOME annotations on block options ("(both die)"/"(you kill it, your blocker
lives)"/"(your blocker dies, attacker lives)"), alt-cast pitch surfacing, salvageLoopedChoice (last
well-formed CHOICE re-parsed); (b) GPT reveal seam (deck135's domain, not here); (c)
suppression→annotation replacement; (d) bug-grind + Oracle sweep. Wave-11 guide FROZEN; live
`deck110_strategy.txt` md5 `00849aa9` — byte-identical since wave-8, **12th freeze**. Reply protocol =
answer-after-plan + name-echo (`CHOICE: N (name)`) / blocks (`BLOCKS: B#:A#` OR now `BLOCKS: name:name`).
Card facts vs `bin/Res/sets/primitives/mtg.txt`; representation facts vs live prompts.
Owned charges: **BLOCK-ECHO NAME-FORMAT resolution confirmation** (wave-18's biting OPEN item — did combat
name tolerance collapse the block fallbacks?); **TRADE OUTCOME annotation validation** (first corpus with
them — did block quality move, any mislead?); **characterize the 2 unparsed**; **equip-churn standing watch
(5th wave)**; **record 3/6 → 5/6 — engine batch or matchups?**

## Record: 5 WIN / 1 LOSS (win table 110: 5/6; results.tsv confirmed)

| seat opp | result | end life (my/op) | T | one-line |
|---|---|---|---|---|
| deck35  (Slivers, guided)      | **WIN**        | 20 / -1 | 6  | fast clean beatdown; Galvanic s6 to face @20 opened, closed T6 |
| deck49  (Dragons, guided)      | **WIN**        | 14 / -2 | 8  | s12 annotation-guided optimal block (Master of Etherium kills Outcast, survives); clean |
| deck14  (Deep Blue, guided)    | **WIN**        | 14 / -2 | 10 | REVERSAL of wave-18's structural grind loss; double face-Galvanic + go-wide |
| deck62  (Enchantresses)        | **WIN**        | 20 / -9 | 10 | untroubled; deck110 never dropped from 20 |
| deck135 (Modern Snow)          | **WIN** (adj)  | 20 /  1 | 11 | REVERSAL of wave-18's one-life adj LOSS; dominated to opp-1, cap one attack short of lethal |
| deck27  (Zombies, guideless)   | **LOSS**       | -9 / 14 | 14 | ONLY loss; raced/ground by a go-wide Zombie board; ran out of reach after 2 early face-Galvanics |

results.tsv: `110 27 1 -9 14 14`, `110 35 0 20 -1 6`, `110 49 0 14 -2 8`, `110 14 0 14 -2 10`,
`110 62 0 20 -9 10`, `110 135 adj0 20 1 11` — all consistent. **Record JUMPED 3/6 → 5/6.** The two wave-18
losses that came back as WINS this wave are exactly deck135 (was one-life adj LOSS) and deck14 (was
structural grind LOSS); deck21 — whose wave-18 loss was *caused* by the block-echo parse failure — ROTATED
OUT. The one new loss is the guideless newcomer deck27 (a fast go-wide Zombie deck, a genuinely hard
race). This is a **FREEZE wave (12th)** — no decision-level strategic class earned a guide change — AND a
clean-canary wave: the wave-18 biting harness class (block-echo name-format) is CONFIRMED FIXED, only 2
non-biting unparsed, equip clean, Galvanic 6/6.

---

## FINDING 1 — BLOCK-ECHO NAME-FORMAT: CONFIRMED FIXED (the wave's headline). The wave-19 combat name-tolerance batch (parseBlockAssignments name→label second pass) shipped and works. A name-format block declaration now parses cleanly; block fallbacks at this seat COLLAPSED to ZERO.

Wave-18's #1 substantive item — the `blockers` matcher accepting ONLY `B#:A#`, so a name-format block went
`unparsed_reply` and the heuristic made a different (at deck21 s18, catastrophically worse) block — is
**resolved**. Direct proof at this seat:

- **deck27 s23 (T14) — NAME-FORMAT PARSED CLEAN.** Reply last line `BLOCKS: Ornithopter:Zombie Master`
  → `choice=1`, `chosen_text="Ornithopter blocks Zombie Master"`, `fallback=None`. This is the exact shape
  that BIT at wave-18 deck21 s18 (`BLOCKS: Memnite: Boggart Ram-Gang`). The name→label second pass resolved
  it. (The block itself was moot — deck110 was at 2 life facing 14 across 5 attackers, dead regardless; the
  annotation correctly said the 0/2 Ornithopter "kills nothing" — but the PARSE is the point, and it worked.)
- **deck49 s12 (T5) — LABEL-FORMAT still parses** (regression check): `BLOCKS: B2:A1` → `choice=1`,
  `chosen="Master of Etherium blocks Dragonmaster Outcast"`. Both reply forms now parse.

**Block fallback count this corpus at this seat = 0** (6 block decisions: deck27 s14/s20 `BLOCKS: none`,
deck27 s23 name-format, deck49 s12 label-format, deck14 s20/s28 `BLOCKS: none` — every one parsed). The
wave-18 OPEN item (block-echo name-format, MEDIUM-HIGH, biting repro deck21 s18) → **RESOLVED, confirmed
by parse + collapse.** Layer: ENGINE/HARNESS (combat matcher), CLOSED. Re-verify by absence next corpus.

## FINDING 2 — TRADE OUTCOME annotations: VALIDATED CORRECT, and at deck49 s12 the annotation DEMONSTRABLY DROVE THE OPTIMAL BLOCK. No mislead observed. Block quality moved UP.

First corpus with per-pairing trade annotations on block options. Both real blocks observed:

- **deck49 s12 — annotation → correct block (positive).** Options: `B1. Vault Skirge (2/2) ... may block A1
  (your blocker dies, attacker lives)`; `B2. Master of Etherium (4/4) — may block A1 (you kill it, your
  blocker lives)`. The model chose **B2:A1** — the strictly-correct kill-and-keep over chumping with the
  Skirge. The annotation labeled the good trade ("you kill it, your blocker lives") and the model took it.
  This is the annotation *improving* decision quality, not just decorating it.
- **deck27 s23 — annotation correct in a lost position.** `B1. Ornithopter (0/2) [deals 0 - this block
  kills nothing, it only absorbs damage] - may block A1..A5 (your blocker dies, attacker lives)`. The
  annotation correctly told the model the block trades nothing; the block was moot (14 damage vs 2 life,
  lethal regardless). No mislead — the printed outcomes matched reality.

**No spot where the annotation printed a wrong outcome.** Layer: ENGINE (block-option serialization),
validated positive. New standing validation (Finding 7).

## FINDING 3 — the 2 unparsed are the SAME giant-reply overthink pocket (13–14k chars), both prose-conclusion-without-protocol-token, so salvageLoopedChoice could NOT fire. NEITHER is a block; one carries a mild carry-vs-prompt whiff.

- **deck27 s18 (T10, 10 life, ask land-drop)** — 14.2k chars. Options were only `Play Mountain` / `Hold
  Mountain`. The model over-computed a lethal race, decided in prose ("My plan: Play the Mountain. Do not
  attack with Signal Pest") but never emitted `CHOICE: N`. Carries a **mild carry-vs-prompt whiff**: it
  kept re-reading a carried plan ("the prompt says 'Your plan: ... I will attack with Signal Pest'. I need
  to write a new plan.") and looped on reconciling it. This is a faint touch of the H1 stale-plan class —
  but on a *land-drop* menu, fell to the heuristic (choice=-1), non-biting. Not a self-blast, not a genuine
  contradiction spiral.
- **deck49 s7 (T2, 20/20, ask cast)** — 13.1k chars. Options `Cast Vault Skirge` / `Cast Cranial Plating`
  / `Cast nothing`. Mana/affinity confusion ("Play Land is not an option") — rambled, concluded in prose
  ("I will choose Cast Cranial Plating") without a `CHOICE:` token. Early game, won the game, non-biting.

**salvageLoopedChoice read:** both fell through (choice=-1). The batch re-parses the *last well-formed
CHOICE line* — but neither reply ever emitted the protocol token; both ended on **freeform prose
commitments** ("I will play the Mountain" / "I will choose Cast Cranial Plating"). So salvage had nothing
well-formed to recover. This characterizes what still rambles at this seat: not stale-plan spirals, but
long overthinks that reach a plain-language decision and never wrap it in `CHOICE:`. See notes.md (engine
item: extend salvage to catch prose commitment lines, cross-deck/dormant).

## FINDING 4 — E2 (equip churn) standing watch, 5th wave: ZERO ALREADY-takes again. 6 equips taken, all value-positive; 24 menus offered an ALREADY option, ALL declined. Rule #1 obeyed 100%, validated-by-absence a 5th consecutive time.

6 `Equip with Cranial Plating`-class decisions taken across the 6 games; **`took_ALREADY=False` on every
one**. 24 menus surfaced an "(ALREADY attached — change NOTHING)" option and the model declined all 24
(passes / real-equip alternatives). Fewer equip decisions this corpus than wave-18's 33 (shorter games,
less equip churn), but zero misfires. The identity-keyed net-zero-MOVE blind spot (E2's one unfixed lever)
again got no exercise — every real equip raised resulting power. Layer: engine ledger, **UNFIXED but
non-biting, 5th-wave validated-by-absence**. MEDIUM/dormant; lever unchanged (compute the no-op marker from
OUTCOME DELTA — suppress "Equip … targeting X" when moving would not raise X's power above its current
carrier; subsumes the sole-option ALREADY menus).

## FINDING 5 — RECORD 3/6 → 5/6: PRIMARILY matchups/variance (flat win-table + deck21 rotated out + rematch reversals), with the block-echo engine fix removing a latent biting failure mode. Not a decision-quality or guide improvement to credit.

- **deck135 (was one-life adj LOSS → now adj WIN):** deck110 sat at 20 the whole game vs the snow deck and
  ground opp 20→15→14→12→11→7→1, attacking every turn (s9/s14/s20/s27/s36). The cap fell one attack short
  of lethal. Same razor-thin adj boundary as wave-18, landed on the winning side this time — variance.
- **deck14 (was structural grind LOSS → now WIN 14/-2):** deck110 raced with double face-Galvanic (s10
  opp→14, s17) plus go-wide and closed by T10 before the blue deck stabilized. Better draw / faster clock;
  the wave-18 loss was the mirror (ran out of gas). Matchup/variance.
- **deck21 (wave-18 block-echo LOSS) ROTATED OUT** — the one loss with a real harness cause last wave is
  simply not in this corpus.
- **deck27 (new loss):** guideless go-wide Zombies out-raced deck110. deck110 spent both Galvanics early on
  face (s6 @opp-20, s15 @opp-18→14), reached opp-14 by T8, then had no reach/answers to a 4–5 creature
  Zombie board and was ground 20→-9. Early face-burn vs holding for reach is a judgment call the guide
  explicitly permits ("fire Blasts at face"); single-seat, single-game variance — does NOT earn a guide
  change (doctrine: win-rate is context, single-seat items cannot add/cut core lines).

Net: the jump is TWO rematch reversals landing on the winning side of thin/variance boundaries, plus the
removal of the deck21 block-echo victim, against one hard new guideless race. The model's STRATEGIC
decisions were sound throughout (Galvanic 6/6 to face, equips 6/6 value-positive, s12 optimal
annotation-block). The guide is not implicated in either direction.

## FINDING 6 — durdle check at the adj game: NOT durdling. deck110 attacked every turn in the deck135 adj game and reached opp-1; the cap is the artifact, not a 0-attacker stall.

Brief asks whether my seat durdled in the adj game. deck135 (adj0): attackers declared T2/T4/T6/T8/T10
(s9/s14/s20/s27/s36), opp driven to 1 life, deck110 untouched at 20. Decisive aggression capped one swing
short. No 0-attacker durdle turns in the adj game. (The deck27 LOSS did have a T10 "no attackers" turn at
10 life — a defensible hold-to-block per the guide's 8-or-less rule — but that game ended in a loss to a
faster board, not adjudication.)

## FINDING 7 — standing validations (verification only, no items):
- **Galvanic Blast targeting — 6/6 to the opponent's face.** Every Galvanic target sub-menu resolved to
  `The opponent (player, life N)`: deck27 s7 (@20), deck27 s16 (@18), deck35 s6 (@20), deck14 s10 (@19),
  deck14 s17 (@14), deck135 s8 (@19). NO self-target, NO own-creature target — even with own creatures /
  "you" listed as legal targets. TARGET CHOICE sub-menu framing clean at this seat, 0 target-sub-menu
  fallbacks.
- **E6 (Mox "Cast Card Normally" dead-end) — CONFIRMED CLOSED a 2nd wave.** `grep` count of "Cast Card
  Normally" across all option/prompt/chosen text = **0** (was the seat's dominant fallback pre-wave-18).
- **TRADE OUTCOME annotations** — correct, and drove the optimal block at deck49 s12 (Finding 2).
- **Name-echo (`CHOICE:`)** — every parsed choice carried its name; 0 wrong remaps. "(player, life N)"
  suffix strip visibly working.
- **Attackers** — all attacker windows resolved to eligible sets; no misfires. Steel Overseer cast/tap
  parsed clean (deck135 s25).

---

## CHARGE VERDICTS
- **Block-echo name-format resolution**: **CONFIRMED FIXED.** deck27 s23 `BLOCKS: Ornithopter:Zombie Master`
  parsed to choice=1; label-format still parses (deck49 s12); block fallbacks collapsed to 0. Wave-18's
  biting OPEN item → RESOLVED.
- **TRADE OUTCOME annotation validation**: **CORRECT, no mislead, quality-positive** — drove the optimal
  block at deck49 s12; correct-in-a-lost-position at deck27 s23.
- **2 unparsed characterized**: both = giant-reply overthink (13–14k chars), prose-conclusion without
  `CHOICE:` token → salvageLoopedChoice couldn't fire; deck27 s18 carries a mild carry-vs-prompt whiff
  (land-drop, non-biting), deck49 s7 mana confusion (early, non-biting). No block among them.
- **Equip-churn 5th wave**: **no recurrence.** 6/6 value-positive, 0/24 ALREADY-takes, Rule #1 100%.
  Validated-by-absence 5th time. Dormant lever unchanged.
- **Record 3/6 → 5/6**: **matchups/variance + block-echo engine fix removing a latent failure mode.** Two
  thin-boundary rematch reversals + deck21 (block-echo victim) rotated out, vs one hard guideless race
  (deck27). Not a guide/decision change.

## HANDOFF
1. **FREEZE the guide verbatim** (md5 `00849aa9`, byte-identical; **12th freeze**). No decision-level
   strategic class earned a change; E6 closed, block-echo now fixed (both harness matcher gaps that this
   seat witnessed are resolved), equip clean, Galvanic clean, annotations helping.
2. Route to notes: **BLOCK-ECHO NAME-FORMAT → RESOLVED** (confirmed by parse, deck27 s23); **E6 → RESOLVED**
   (2nd-wave absence); **TRADE OUTCOME annotations → VALIDATED** (correct + block-quality-positive, deck49
   s12); **salvageLoopedChoice / giant-reply overthink → dormant engine item** (2 unparsed lost to
   prose-without-token; salvage doesn't catch prose commitments; cross-deck); **E2 dormant/unfixed** (5th
   wave validated-by-absence); **H1 stays RESOLVED-WATCH** (mild carry-vs-prompt whiff at deck27 s18,
   non-biting land-drop, not a spiral).
3. Route to general-suggestions: the block-echo fix VALIDATED (close the wave-18 cross-deck item); the
   salvageLoopedChoice prose-commitment gap is the standing cross-deck representation item; E2 outcome-delta
   no-op suppression remains standing.
4. Rotation verdict in notes.md — deck110's wave-18 self-set clean-rotation precondition (block-echo fixed
   AND no new class) is now MET → **ROTATION CANDIDATE**.
