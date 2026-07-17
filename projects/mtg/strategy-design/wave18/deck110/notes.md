# Deck-110 wave-18 — development notes (layer-routed engine/harness ledger + rotation verdict)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-185930/`, 6 deck110 seat translogs. Binary
`f664539a3` (wave-17 batch: (a) TARGET CHOICE sub-menu framing; (b) hand-card PUT_INTO_PLAY dead-end
filter = E6 fix; (c) beneficial opponent-only-target cast suppression; (d) "BLOCKS: none" example +
paraphrase catch; (e) "(player, life N)" suffix strip). deck17 rotated OUT; deck14 (Deep Blue) entered
guideless. Wave-11 guide FROZEN (live `deck110_strategy.txt` md5 `00849aa9`, byte-identical **11th
freeze**). Answer-after-plan + name-echo; blocks via `BLOCKS: B#:A#`. Card facts vs
`bin/Res/sets/primitives/mtg.txt`. Every item carries a repro; engine/harness items live HERE, never in
guide text. Record: **3 WIN / 3 LOSS** (49,35,62 win; 21,135,14 loss). Matchups largely INVERTED vs
wave-17 (beat 49; lost 21,135) on the flattest win-table yet.

## E6 — ENGINE / REPRESENTATION: RESOLVED (confirmed by absence). The "Cast Card Normally with Mox Opal {rules text}" mislabeled mana-ability activation is GONE.
Last wave's #1 fallback driver (5/7 non-defer fallbacks, the degenerate SOLE-option Mox tap). This wave:
**0 occurrences of "Cast Card Normally" corpus-wide** (grep over all option/prompt/chosen text). The
wave-17 batch item (b) — filtering hand-card PUT_INTO_PLAY dead-ends from the priority menu — shipped and
works; this is the stronger of the two levers I proposed last wave (relabel vs suppress-the-no-op). E6
CLOSED. No NEW dead-end shape *drives a fallback*. One residual NON-biting shape remains: `options=1`
priority menus whose sole option is an "(ALREADY ... change NOTHING)" equip re-offer (deck21 s16/s17/s29,
deck62 s28/s31; `{B}{B}` re-attach variant deck135 s10/s11) — the model PASSES all of them cleanly (zero
fallbacks). Same class as E2 (behavior-neutral no-op surfaced as sole option); folded into the E2 lever,
priority LOW. Status: **RESOLVED** (re-verify by absence next corpus).

## BLOCK-ECHO NAME-FORMAT — ENGINE / HARNESS (NEW, OPEN, priority MEDIUM-HIGH): the blockers reply matcher accepts ONLY `B#:A#`; a name-format block declaration goes unparsed and the heuristic makes a different block. BITING at deck21 s18.
CHARACTERIZATION: the `blockers` matcher does not resolve creature NAMES to the B#/A# roster labels the
prompt just printed. Two repros:
- **deck21 s18 (T7) — BITING.** Reply last line `BLOCKS: Memnite: Boggart Ram-Gang` -> unparsed ->
  choice=0 -> heuristic fallback. Model INTENT: block the 3/3 Boggart Ram-Gang with the 9/1 Memnite, let
  the 1/1 Scuzzback Scrapper (wither) through. Fallback ACTUAL (from events): Memnite -> Scuzzback
  Scrapper (1/1 wither), Ram-Gang -> face for 3. The 1/1's wither counter reduced the 9/**1** Memnite to
  0 toughness -> Memnite DIED to a 1/1; the 3/3 survived and kept attacking (s20: +4 with Orcish
  Oriflamme). deck110 lost -2/16 (this is the wave-17 rematch it had WON clean). The dropped reply was a
  strictly-better play (even accounting for a model P/T misread inside it — it called the 9/1 "9
  toughness"; its intended trade was still better than the fallback's).
- **deck49 s12 (T3) — non-biting.** `BLOCKS: Ornithopter blocks Dragonmaster Outcast` -> unparsed ->
  choice=0; the heuristic coincidentally made the SAME block (Ornithopter blocked the Outcast, survived).
PROOF it's a pure matcher gap: `BLOCKS: B1:A1` parsed fine at deck49 s19, deck21 s24, deck21 s30. LEVER:
extend the block matcher to resolve creature names -> B#/A# labels (the exact analogue of the working
`CHOICE:` name-echo), tolerating `Bname: Aname` and `Bname blocks Aname`. The wave-17 "paraphrase catch"
(batch d) covered the priority menu / "BLOCKS: none" but NOT name->label block ASSIGNMENTS. Owner:
block-reply parsing in AIPlayerGPT harness. Cross-deck (every blocking deck). NOT guide-fixable
(reply-protocol layer; redundant with the prompt example; wrong scope for a deck110 strategy guide).

## H1 — HARNESS (stays RESOLVED-WATCH): the wave-16 stale-plan spiral class did NOT recur; NONE of the 6 unparsed is a carry-vs-prompt contradiction loop.
The 6 unparsed this wave: 2 block-echo parse failures (above), 4 long position/state rambles. The 4
rambles: deck21 s25 (desperation lethal-hunt @2 life), deck21 s28 (hopeless `Wait...Wait...` loop @2 life
— but the real option was on-menu; a losing-line stall, not a stale-plan re-litigation), deck135 s23
(equip-vs-attack damage optimization in Main phase 2 AFTER combat — moot computation, never emitted
CHOICE), deck14 s25 (Steel-Overseer timing/phase confusion; CARRIED the PLAN-caveat but the ramble is
state-confusion, not the class the caveat targets). No self-blast anywhere; no carry-vs-prompt
contradiction loop. All 4 rambles fall in the 3 LOST games; the 3 WINS carried at most 1 non-biting
fallback. H1 stays **RESOLVED-WATCH** — re-open only on a self-blast or a genuine carry-vs-prompt loop.

## E2 — ENGINE / REPRESENTATION (UNFIXED, dormant): equip no-op marker is TARGET-IDENTITY-keyed, blind to net-zero MOVES. DID NOT RECUR — validated-by-absence, 4th wave.
33 equip decisions, `took_ALREADY=False` on ALL. Where an ALREADY sat beside a real equip, the real equip
was taken (deck135 s15/s20, deck62 s14/s20); where the sole option was ALREADY, the model passed (deck21
s16/s17/s29, deck62 s28/s31). Every real equip value-positive. The two unparsed equip decisions (deck21
s28, deck135 s23) are Finding-3 rambles, not ALREADY-takes. Identity-keyed net-zero-MOVE blind spot got
no exercise (all moves raised power). ROOT & LEVER unchanged: compute the marker from OUTCOME DELTA —
suppress "Equip ... targeting X" when moving would not raise X's resulting power above the current
carrier (this ALSO subsumes the sole-option ALREADY menus noted under E6). Owner: AIPlayerGPT equip-option
serialization. Priority MEDIUM/dormant.

## DEFERS — 1 deferred_to_heuristic (down from 3): benign, CORRECT.
deck135 s3 — `prompt=""`, `latency=-1`, `choice=-1`, `chosen="Glimmervoid"`: harness deferred a forced
land-drop to the heuristic (no LLM call), which played a Glimmervoid. Never wrong for this deck. Harness
working as intended. No item.

## DC1 — DECK-CONSTRUCTION / META (standing; owns 2 of the 3 losses).
- deck14 -7/10, T21: Deep Blue (fresh guideless). deck110 got the opponent 20->10 by T8 then ran out of
  gas over a 21-turn grind — structural aggro-must-kill-by-T5-6 vs stabilizing control. Matchup.
- deck135 adj 13/14, T8: razor-thin ONE-life adjudication after an even early race. Variance.
- (deck21 -2/16 is the 3rd loss but is NOT DC1 — it has a harness cause, the block-echo failure above.)
Mulligan NOTE (reaffirmed): opening keeps were again 1-creature and the deck went 3/3 with sound play —
do NOT tighten the mulligan rule. Not guide tasks.

## Positive validations (continuity, no action)
- Galvanic targeting 6/6 to the OPPONENT'S FACE (deck49 s6, deck21 s13, deck35 s24, deck62 s16, deck14
  s10/s15). No self-target; batch-(c) suppression didn't need to fire. TARGET CHOICE sub-menu framing
  (batch a) clean at this seat, 0 target-sub-menu fallbacks.
- Name-echo 0 wrong remaps; "(player, life N)" suffix strip (batch e) visibly working.
- Attackers reconcile sane. Steel Overseer sole-option activations all parsed (deck35 s21, deck14
  s22/s24/s28/s31/s33 — cosmetic " 1/1 " label prefix, harmless).

## ROTATION VERDICT (per-deck rotation rule) — HOLD (marginal; weaker "must-keep" than last cycle).
Rule: rotate a veteran out only if (1) its guide takes NO modification AND (2) its seat surfaces NO new
work signal.
- Condition 1 MET: guide FROZEN (11th freeze, terminal — no decision-level strategic class in ~8 waves).
- Condition 2 NOT met: the prior blocker to rotation (E6) is now CLOSED — but a NEW harness class surfaced
  this cycle with a biting repro (BLOCK-ECHO NAME-FORMAT, deck21 s18). Last cycle's stated rotation
  precondition was "E6 fixed AND a subsequent corpus confirms no new class" — E6 IS fixed, but the
  "no new class" half FAILED.
DECISION: **HOLD this cycle.** deck110 currently holds the two clean block-echo repros and the biting
example, so it is the concrete witness while that item is OPEN. CAVEAT (weaker than E6's hold): unlike E6
(Mox/affinity-specific, only exercisable here), the block-echo matcher gap is CROSS-DECK — any
combat-heavy seat can own it (deck21/deck62 block more than this aggro deck does). So if synthesis
prefers, it may rotate deck110 out AND reassign the block-echo witness to a block-heavy seat; that is
defensible. Absent that, HOLD one more cycle. deck110 becomes a CLEAN rotation candidate once the
block-echo matcher is fixed AND a subsequent corpus confirms no new class (and it remains a useful
regression canary for the Galvanic-finisher, equip outcome-delta, and now the block-echo fix).
