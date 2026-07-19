# deck62 (Enchantresses) — REVISED-GUIDE ROUND 4 validation findings (wave 20)

Corpus: `matchups-20260719-151849`, binary `/tmp/wagic-95e89c204`. deck62's FOURTH guided corpus. The
wave-19 revision (round 3) is live at `Res/ai/baka/deck62_strategy.txt` (edit A: anti-spiral generalized
to "cannot win this turn, AHEAD or BEHIND"; edit B: defensive protect-the-engine clause; edit C: mulligan
"decide from what the hand HAS"). 6 owner translogs, **152 decisions** (ask 114, attackers 24, blockers 14).

Record **3/6** (up from 2/6). Per Step 0 the record is context. Bottom line: **the taught classes HELD
into round 4 (P/T distrust pocket still DEAD, no 0-attacker durdles, no enchant-opponent gifts, no A-list
unparses), and the story of the corpus is the BLOCK SEAM: both blockers-class fallbacks are
reasoning-spiral-to-truncation with NO committed BLOCKS line — a shape the wave-20 extended salvage
CANNOT and CORRECTLY did NOT catch — after which the baka blocker HEURISTIC gang-blocks and needlessly
SACRIFICES the Argothian engine.** One of the two (vs14 s26) is an echo-test-positive GUIDE-CAUSED spiral:
the model loops verbatim litigating edit B's chump-block clause against the prompt's "trade favors you"
annotation. That is the one surgical revision this wave (scope-sharpen edit B); everything else freezes.

## Record: 3/6 — matchup-shape result, wins fired the taught line

| vs | result | life (me/opp) | turn | read |
|---|---|---|---|---|
| deck102 | **WIN** | 19 / 0 | 11 | Clean go-wide beatdown vs guideless Tergrid. Attacked every viable turn (Druid+Saprolings); opp 18 to 0 steadily. Taught attack-every-turn/go-wide line fired. |
| deck14  | **WIN** | 16 / -5 | 14 | Grindy win vs Deep Blue. Early 0-attacker turns were all opts=1 (empty eligible set); alpha strike T14 (Canopy + 2 Yavimayas). One block fallback (s26, WON anyway). |
| deck27  | **WIN** | 11 / -6 | 10 | Go-wide vs Zombies; Birds + 4-Saproling alpha strikes s22/s24. opp 20 to dead. Win line fired. |
| deck135 | LOSS | 0 / 13 | 10 | 135 fast board (8/8 Abominable Treefolk + Ice-Fang flyers by T8). Block fallback s19; behind survival-math unparse s22. Matchup floor. |
| deck49  | LOSS | 0 / 20 | 9 | Red aggro/Dragons; deck62 NEVER had an eligible attacker (all 3 attack decisions opts=1), raced 20 to 4 by T8. Matchup floor. |
| deck35  | LOSS | -22 / 2 | 11 | Go-wide Slivers raced back from 10 to kill from 20 to -22 in 3 turns. Go-wide mirror lost on tempo. Matchup floor. |

Three wins (all fired the attack-every-turn/go-wide beatdown), three losses all matchup floors against
faster/wider clocks (49 aggro never let a board form; 35 Slivers out-tempo; 135 out-sizes). Same structural
verdict as waves 18/19: **turn-8-13 ground/go-wide deck; the guide cannot draw a faster clock.** The
record ticked up on pool luck (drew guideless 102; beat 14/27), not a play change.

## MANDATE 1 — Salvage-extension LIVE VALIDATION: both blocker fallbacks, and why salvage correctly did NOT fire

The wave-20 perception bundle extended `salvageLoopedChoice` to BLOCKS/ATTACK/PUT. deck62's two
blockers-class fallbacks are the live test at this seat. **Result: the extension did not fire at either,
and that is CORRECT — neither reply contains a committed BLOCKS line for salvage to rescue.**

- **vs14 s26 (blockers, T11, 16/18 AHEAD).** Opp attacks with Lord of Atlantis (2/2). Options tag B3
  Yavimaya (5/5) "(you kill it, your blocker lives)" — a clearly-good free block. **Reply 15,525 chars.
  Grep for `BLOCKS:` finds only `BLOCKS: none` (x2, as reasoning asides), the OPPOSITE of the model's
  stated intent ("So I block", "I will block with Yavimaya").** The tail is a verbatim decode-repetition
  loop: *"Wait, the prompt says 'block only where the trade favors you'. ... The trade is favorable. So I
  block."* repeated to truncation. No coherent looped BLOCKS commitment -> salvage has no target ->
  heuristic. The baka heuristic then **gang-blocked the 2/2 Lord with two Argothian Enchantresses (each
  1/2), killing the Lord but SACRIFICING one Argothian engine** (s27 events: "Argothian Enchantress:
  battlefield -> graveyard"), when B3 Yavimaya kills it for free. Non-fatal (game won), but a materially
  worse block than the model intended.
  - **Salvage verdict: correctly no-fire.** The only literal BLOCKS token is `none`, which contradicts the
    plan; salvaging it would have been WRONG. This is reasoning-spiral-to-truncation, not a format-drift or
    looped-choice shape the extension targets.
  - **This is a GUIDE-CAUSED spiral (echo test positive).** The model quotes edit B's chump-block lines
    verbatim ("Do NOT chump-block with Argothian Enchantress... just to absorb a few points") and cannot
    resolve them against the prompt's "block only where the trade favors you" and the guide's "taking
    damage while ahead is often correct." A 5/5-into-2/2 favorable trade is NEITHER a chump nor an
    absorb-non-lethal — but the three competing clauses gave the model an unresolvable question to fixate
    on. Per Step 0 "a rule the executor CITES while misplaying is a rule to rewrite," and per the
    intra-guide-contradiction rung, this is the one revision target this wave.

- **vs135 s19 (blockers, T8, 13/19 behind).** Opp attacks with Icehide Golem (2/2), Ice-Fang Coatl (1/1
  fly/deathtouch), Abominable Treefolk (8/8 trample). **Reply 11,541 chars, NO `BLOCKS:` line at all** —
  an ADVANCING block-math spiral (enumerating Option 1 vs Option 2 with real damage arithmetic) that
  truncates mid-"Option 2: Block Coatl with Argothian. Block G...". Heuristic then **gang-blocked the 2/2
  Golem with BOTH Yavimaya (4/6) AND Argothian (0/1)** — Yavimaya alone kills the Golem, so the Argothian
  was sacrificed for nothing (s20 events: "Argothian Enchantress: battlefield -> graveyard"). Face took
  Treefolk 8 + Coatl 1 = 9 -> life 4; game lost T10 (matchup floor).
  - **Salvage verdict: correctly no-fire** (no BLOCKS line emitted; pure pre-commit truncation). NOT
    guide-caused — this reply is an advancing block-math spiral with no guide echo; the anti-spiral line
    (edit A) is scoped to lethal-HUNT on attack, not to defensive block math.

**Cross-cutting engine finding (both instances): when a block reply unparses -> heuristic, the baka blocker
fallback GANG-BLOCKS a small attacker with the 0/1 shroud engine even when a single real blocker suffices,
needlessly sacrificing the draw engine.** This is the exact "protect the engine on defense" concern edit B
was written for — but it lives in the HEURISTIC, below the model's answer, so guide prose cannot reach it.
Route to notes.md as an engine item (baka blocker-fallback should prefer non-0/1, non-shroud blockers and
avoid over-committing bodies to a lethal-safe block).

## MANDATE 2 — Computed-P/T distrust pocket: DEAD into round 4 (evidence)

**No recount-of-already-printed-P/T anywhere in the corpus.** Tight-signature grep across all 6 owner logs
("actually N/N", "is it really", "recount", "printed N/N but") = 0 hits. Every Blanchwood mention (16
across the corpus) is legitimate PROSPECTIVE cast-value math — computing what Blanchwood WOULD make a
creature if cast (vs27 s19/s20: "make the Birds a 7/8 or 8/9"; vs102 s15; vs14 s13) — Blanchwood not yet
attached, so unbannable, identical verdict to waves 18/19. The TRUST-printed-[X/Y] line is HELD DEAD. Watch
stays retired.

## MANDATE 3 — Blanchwood recount-ban + A-list carryover

- **Blanchwood recount-ban: HELD DEAD** (see Mandate 2).
- **A-list-only attacks: ZERO attacker fallbacks; zero A-list unparses.** All 24 attacker decisions parsed
  clean. Every "no attackers" this corpus had **opts=1 (only the none option = empty eligible set)** — so
  there were NO 0-attacker durdles; the model correctly declared none only when nothing was eligible
  (vs49's three attack turns all opts=1 = never developed a board vs aggro; vs135 T5/T7 opts=1). The
  attack-every-turn rule is WORKING — steady beatdowns in all three wins, no durdle in any game.
- **Duplicate-name combat resolution WORKED (positive control for the wave-20 #N batch).** vs14 s36 the
  model named "Canopy Spider, Yavimaya Enchantress, Yavimaya Enchantress" against a list holding TWO
  distinct Yavimayas (A3 7/9, A4 7/7); the engine mapped the repeated bare name to BOTH distinct creatures
  and all three attacked — no #N ordinal even needed. vs135 s25 "Saproling blocks Treefolk; Saproling
  blocks Treefolk" (two Saprolings, both resolved). Duplicate-name mapping is clean at this seat.

## The 5 ask/stale fallbacks — bucketed by mechanism

1. **vs135 s22 (ask, T9, 4/19 BEHIND).** Cast Fists/Web/nothing. ADVANCING survival-math spiral ("Cast
   Fists... I die. This is bad. Wait, Y...") -> truncation. This is edit A's target class (lethal/survival
   hunt) but at a matchup floor (4 life, dead T10); edit A did not shorten it enough to parse. Advancing,
   not decode-repetition.
2. **vs35 s17 (ask, T8, 20/10 AHEAD).** Cast menu. Model reasons CORRECTLY (2nd Primal Rage redundant,
   Gaea's Anthem static, Birds a dork, Canopy enters tapped -> "best play is Fists"), reaches the decision,
   then keeps deliberating downstream turns ("Next turn...") -> truncation before the CHOICE line. **AHEAD
   over-deliberation of a settled CAST decision** — NOT lethal-hunt, so edit A doesn't cover it; the guide's
   redundant-global note IS obeyed in the reasoning. Below the guide layer (decode length).
3. **vs14 s34 (ask, T14, 16/9).** Trivial "Play Forest / Hold Forest". Model spirals into board-state
   reconstruction confusion ("Wait, did we block the Air Elemental?") -> truncation over a one-line
   decision. Won anyway. Over-deliberation of a trivial ask.
4. **vs49 s18 (ask, T8, 7/20).** Fists target sub-menu (only opponent creatures legal). Model reaches a
   VALID choice ("CHOICE: 1 (Dragonmaster Outcast)") but the giant reply ALSO emits the literal answer
   TEMPLATE ("CHOICE: [Number] ([Name])" x2) and a contradictory "CHOICE: Argothian Enchantress" (shroud,
   illegal) -> parser can't disambiguate -> unparse. **Reply-protocol leak: the model parroted the format
   template as if it were an answer.** Matchup floor. Engine/harness item (N-template-echo).
5. **vs27 s5 (ask, T2, stale_echo).** Cast menu (Fists/Web/Blanchwood/nothing). Model planned the land drop
   first and emitted "CHOICE: 1 (Play Forest)" — a name that matches NO option in this cast menu ->
   stale_echo -> heuristic. **Cross-menu sequencing confusion** (answered the land-drop step inside a cast
   menu; the same class as wave-19 N7). Routed correctly by the stale_echo->heuristic path; the model's
   real intent (Fists on Birds) was reasonable. Minor.

Bucket totals: 2 advancing-spiral-to-truncation (vs135 s22 behind, vs14 s26-block guide-caused), 1 ahead
over-deliberation (vs35 s17), 1 trivial-decision over-deliberation (vs14 s34), 1 reply-template parrot
(vs49 s18), 1 cross-menu sequencing (vs27 s5), plus 1 advancing block-math (vs135 s19). **None is the
computed-P/T pocket.** The dominant driver shifted from wave-19's decode-REPETITION loops to
advancing-spiral / over-deliberation truncations (only vs14 s26's tail is a verbatim repetition loop, and
it was fueled by the guide contradiction).

## Wave-19 edit validation

- **Edit A (anti-spiral generalized to "cannot win this turn, AHEAD or BEHIND"): OBEYED where it fires,
  but the truncation class it targets persists as DECODE length, not as reduced spiraling.** The
  ahead/behind lethal-hunt still truncated (vs135 s22 behind, vs35 s17 ahead over-deliberation). Edit A is
  a behavioral branch; it cannot stop a decode-length truncation once the model enters exhaustive
  enumeration. No regression, no clear win — hold it (it is cheap and correctly scoped now).
- **Edit B (defensive protect-the-engine): OBEYED at its clear target windows, but its WORDING fueled a
  costly spiral.** Its target behavior (chump the 0/1 engine to absorb NON-lethal while comfortable) did
  NOT recur: the model correctly TOOK non-lethal damage while ahead at vs14 s10/s16 (20/20, 18/20) and
  vs35 s18 (20/10) — "no blockers" rather than chumping the engine. The only 0/1-engine block (vs49 s22,
  Argothian blocks a 1/1 at 4 life) is a desperate floor spot (dead next turn regardless), plausibly a
  lethal-chump. BUT at vs14 s26 edit B's chump-block clause became fixation fuel for a verbatim decode
  loop that ended in the heuristic sacrificing an Argothian anyway. **Edit B works for its case but its
  scope bleeds into over-deliberation on unrelated favorable blocks.** -> surgical scope-sharpen (below).
- **Edit C (mulligan clause): no mulligan over-planning fallback this corpus** (wave-19's vs110 s1 shape
  did not recur; but deck110 wasn't in this pool, so this is untested, not vindicated — keep at full
  prominence per PAUSE-AWARE).

## Positive controls (wave-20 batch landing at this seat)

- **Combat options_text now present at ALL deck62 combat records** (blockers AND attackers carry the A#/B#
  annotation lines) — the wave-19 deck14 observability gap is closed at this seat.
- **Trade-outcome annotations still accurate** — no mislead across the 14 block decisions; the "(you kill
  it, your blocker lives)"/"(both die)"/"[deals 0 - only absorbs damage]" tags matched board math.

## Guide disposition: SURGICAL REVISION (round 4), ONE scope-sharpening edit

**Freeze everything obeyed** (turnaround/OBEYED-BUT-LOSING mode: record improved, losses are matchup
floors, obedience metrics healthy): auras-draw/pump, enchant-opponent-gift ban, Argothian-first,
trust-printed-[X/Y], attack-every-turn, A-list, edit A, edit C — all HELD, verbatim.

**One edit (edit D), aimed only at the new leak (vs14 s26):** scope-sharpen edit B's defensive clause so
the model cannot mistake a favorable REAL-creature block for the forbidden 0/1-engine chump-block. Add a
decisive short-circuit: when the prompt tags a block "(you kill it, your blocker lives)" or "(both die)"
on a real (non-0/1) creature and you want the attacker gone, MAKE that block and move on — it is a trade,
not a chump; do not re-litigate whether a good block counts as chump-blocking. This resolves the
intra-guide contradiction that fueled the vs14 s26 decode loop, is purely ADDITIVE (freezes edit B's
active tokens), and is framed low-confidence (a decode loop can fixate on anything; this removes the
specific fuel the guide supplied). NOT a rotation candidate: guide-mod = yes; new-work-signal = yes (the
baka blocker-fallback engine-sac and the reply-template-parrot both need another corpus).
