# GPT-layer audit LEDGER v2 (2026-09-15 19:40) — rebuilt on Astra's OWN cross-review (cross-astra-genuine.md); supersedes LEDGER.md

Owner ruling 16:10: "Wait for 7pm. Have astra do it. I want the extra pov explicitly." This ledger records where Astra's genuine verdicts DIFFER from the Fable stand-in's (cross-fable-as-astra-seat.md), and what that changes for the cleanup branch `gpt-cleanup` (built on LEDGER.md v1; all gates clean; UNMERGED).

## 1. Astra vs the stand-in — the extra point of view, measured
| | stand-in (Fable as Astra) | Astra genuine |
|---|---|---|
| CONFIRM / REFUTE / CANNOT-DETERMINE mentions | 89 / 16 / 14 | 81 / 50 / 20 |
Astra refutes three times as much. The refutations cluster in four places, none of which the stand-in raised:
- (a) **Counters**: Astra refutes the "no consumer" premise outright — `corpus-stats.py:465` iterates every integer counter, so every counter IS consumed; a zero is an observation, not dead code; and several counters Fable listed as zero are NOT zero (`forced_close_events` 26, `forced_close_unrecorded` 13, `hold_events` 180, `hold_reopened_new_lethal` 7, single-outcome 3/60). Astra's position: deleting a mechanism and deleting a consumed counter are separate decisions; only the duplicate alias (C22) is confirmed removable.
- (b) **Never-rendered emitters**: `cannotPayNowClause` (live, payment-reach conditions incl. floating mana), `paymentNoLifeCostClause` (row-specific fact, not a non-event), `menuFitTag` empty-lostRows branch (empty = "no other row lost after THIS choice", discriminating) — all REFUTED as DELETE. The nine census false positives and the rare-card families: KEEP confirmed.
- (c) **Parser tolerance** (X4/§1.8): REFUTED as expendable — robustness is an owner requirement independent of the thinking regime; keep name/word tolerance; remove only the semantic vetoes (no-op re-ask, label-less-rejected-by-trailing-prose).
- (d) **Fable F4(c) "three totals" / F10 rebuild / F15 lifetime / I9 halving / X1's simpler key / X9 "one K" / §1.6 hold twins**: refuted or narrowed with mechanism (different scenarios need LABELS not equal numbers; the seam is not re-entered in flight; the mutex defect stands; a combined menu still returns one action; the proposed key omits life; spent ≠ left; the hold twins differ in occurrence/rank handling).
Astra CONCEDES on its own side: the reveal helper is engine-called; counters have a generic consumer (narrow "write-only"); and holds F3 (watchdog), F4 (mutex), F6/F7 (parser), F9 (land status) as HIGH/MED.
Everything the stand-in confirmed that Astra also confirms (F1 opp-turn verdict 47 windows, F2 gate, F3 withholds 1-7, F4 a/b/d, F5, F6, F7, F8, F9, F11, F12, F13, F14 most, C3/C6/C8-C10/C12-C22, X2/X5/X6/X7/X8, I1-I8/I11-I14 directionally) STANDS with two genuine verdicts.

## 2. What changes for the cleanup branch (revision scope; nothing else)
| item | branch did | Astra says | revision |
|---|---|---|---|
| R1 gameend counters | deleted ~18 rows under the "per-record join" rule | consumed generically; several nonzero; separate decision from the mechanism | RESTORE every deleted gameend row except the C22 duplicate alias; a counter whose mechanism was deleted stays deleted with it (e.g. single-outcome menu, cross-phase replay) — say which in the report |
| R2 `noAnswerClassFor` fold (8 → 2 sites) | folded four overloads into one | the 4/5-arg forms add HTTP/transport classification; precedence must be preserved | VERIFY the fold preserves the classification precedence with a pin on the transport/HTTP cases; else restore the overloads |
| R3 L5 CrackBackFacts | one total per window | F4(c): cover-after-casting vs best-block-with-existing are DIFFERENT scenarios — label them, do not force equal numbers | VERIFY the struct renders each scenario with its own label; a forced-equal number is a new lie |
| R4 pregame bottom order | made the TEXT say the engine bottoms in hand order | MED: CR 103.5 "in any order" — the model's order is a LEGAL CHOICE, and the ruling says never remove one | HONOUR the model's order in `pregameChooseBottomInner` (fill the queue in the answer's order); the text then says what happens |
| R5 (verify only) | left `cannotPayNowClause`, `paymentNoLifeCostClause`, the menuFitTag branch, the six refuted helpers, the stripper spellings, the hold twins, parser tolerance untouched | keep all | confirmed untouched by diff — no action |
Sections A (the eleven flaws) and B/C otherwise stand: every A item has two genuine confirmations.

## 3. Owner's review list — unchanged from LEDGER.md §D except: I9 (fold land drop into the cast menu) — Astra REFUTES the "halves calls" claim (a combined menu still returns one action; the cast question re-puts after the land enters); the unification can still be proposed on other grounds. I13 (hand-only discard frame) — Astra: needs a deliberately different frame, the pregame flag drops the board.

## 4. OWNER RULINGS on §D (2026-09-16, verbatim where he spoke)
- P1 (invariants → system prompt) and P2 (stop re-shipping card text): "1 and 2 seem like they might negatively impact the play quality. I think we didnt originally restate the card text? Well, worth a try maybe, but definitely needs ab testing if we do it." FACT: per-row card-text glosses entered 2026-08-19/20 (5c6d66183, 6a54e87e6 — waves 34/35); the 2026-07-09 prompt design did not restate card text; `{effect:}` on permanents is wave 77 (a7a77683b). → A/B ONLY: two binaries from one commit differing only by P1+P2, same 21-game round-robin, judged by blind-seat misplay counts (the census's experiment), never win rate. Not before the loop resumes.
- P3 collapse hand entries: yes. P4 shorten hand tags: sure. P6 one clause per number: yes. P8 range grammar: yes. P10 whole-order blocker answer: yes (a reply-shape extension for ORDERING only; the golden protocol text otherwise untouched; the lane states the exact added text in its report). P11 telemetry schema: yes. P12 navigable by responsibility: yes. P13 re-measure the W81 folds: yes.
- P5 narration compaction: "Im definitely interested in making this more compact and efficient. Id like to see a sample of what that would read like." → sample presented 2026-09-16 (below, §5); awaiting his read before any lane.
- P7 cleanup-discard without the log: "try it. Revert if it degrades behavior."
- P9 land drop folded into the cast menu: "No, this can be an important tactical step, especially in a graveyard shenanigans deck." → CLOSED, do not re-propose.

## 5. P5 sample (from a real wave-80 prompt, 152v125 seq 15, turns 6-8; 26 lines / ~1,040 B → 9 lines / ~430 B; every fact kept, phase markers only where a decision or event needs them)
BEFORE (verbatim):
=== Turn 6 - opponent's turn === / - Phase: Draw / - Opponent drew a card / - Phase: Main phase 1 / - Opponent played Seachrome Coast / === Turn 7 - YOUR turn === / - Phase: Draw / - You drew Brutal Cathar / - Phase: Main phase 1 / - You played Overgrown Farmland / - Paid {2}{w} for Elite Spellbinder (3 sources) / - You cast Elite Spellbinder / - Your Elite Spellbinder resolved and entered the battlefield / - You targeted Supreme Verdict with Elite Spellbinder's ability (exile a non-land card) / - You used: exile a non-land card with Elite Spellbinder targeting Supreme Verdict / - Opponent's Supreme Verdict was exiled from the opponent's hand / - Phase: Attackers / - You declared attackers: Katilda, Dawnhart Prime / - Phase: Combat damage / - Your Katilda, Dawnhart Prime dealt 1 damage to the opponent (now 18) / === Turn 8 - opponent's turn === / - Phase: Draw / - Opponent drew a card / - Phase: Main phase 1 / - Opponent played Plains / - Phase: Main phase 2 / - Opponent cast Lightmine Field / - Opponent's Lightmine Field resolved and entered the battlefield
AFTER (proposed):
T6 (opp): drew; played Seachrome Coast.
T7 (you): drew Brutal Cathar; played Overgrown Farmland.
  Main 1: cast Elite Spellbinder ({2}{w}, 3 sources) -> resolved; its ETB exiled Supreme Verdict from their hand.
  Attack: Katilda, Dawnhart Prime -> 1 damage, opp 18.
T8 (opp): drew; played Plains.
  Main 2: cast Lightmine Field -> resolved.
Rules of the fold: a phase marker prints only when something happened in it; draw + land in the turn header line; "paid X for Y / cast Y / Y resolved" become one line with the cost in parentheses; "targeted … / used … / was exiled" become one ETB line; damage carries the new life total. Lost: nothing factual; the order of events within a phase is preserved. Judged by the owner's criterion on the composed document, then by the same A/B instrument as P1/P2.

## 6. OWNER RULINGS 2026-09-16 (cont.): P5 sample — "That looks good. Ok." → P5 APPROVED as specified in §5. "Use fable agents to code. Make it so." → the proposals lane runs on Fable (the Opus lane was stopped before it had made any change).
