# deck137 notes.md — engine/harness/ledger items (wave 24)

## R-ADV-FACE — CLOSED (acceptance PASSED). [REPRESENTATION] FACT surface shipped; crutch demoted.

The wave-23 ledger item R-ADV-FACE ("the alternative/adventure-cost cast option prints the WRONG
face's (P/T) and no effect text") is RESOLVED by step-1 fix (c). Adventure-cost options now render
`Cast NAME // ADV with its adventure cost {cost} {adventure spell: <full effect text> (Then exile
this card. You may cast the creature later from exile.)}` — real effect text, NO creature (P/T). The
creature-face option keeps its (P/T). Verified on all adventure-cost renders (deck131 s32/s40,
deck59 s20, deck102 s18, deck102 s42-Usher). The wave-23 CONFIRMED misfire (Shepherd//Usher self-
bounce) did NOT recur — deck102 s42 read the bounce effect and declined it correctly; adventure-
spell casts (deck59 s35, deck133 s19 Heart's Desire) are now effect-driven. The removal condition is
met and the guide's interim P/T-TRAP crutch has been DEMOTED this wave. No further action; close the
ledger item.

## LEDGER — R-STALE-ECHO-QUALIFIER: INDEX-WINS residual gap on qualifier-decorated in-range indices. [PARSER]

deck102 s4 (Temple Garden shockland ETB). Options `1. pay 2 life` / `2. tap`. Reply first line
`CHOICE: 2 (Tap Temple Garden)` — well-formed, IN-RANGE (2), label "tap" matches opt2. It downgraded
to stale_echo (choice=-1, heuristic answered) instead of being trusted by INDEX-WINS rule (c). Cause:
the model decorated the bare option word "tap" with the card name ("Tap Temple Garden"), and
"Temple Garden" collides with the recently-seen sibling option "Play Temple Garden" (s3) — the same
qualifier-name-prefix-breaks-option-N-match shape wave-23 recorded as stale_echo's fourth downgrade
cause (deck14/deck62 spell-name prefixes on TARGET menus), here recurring on a 2-option shockland
ETB menu. INDEX-WINS (c) did not recover it.
- FIX (parser, ledger): when a `CHOICE: N (...)` has an in-range N and the parenthetical contains the
  option-N name PLUS a qualifier token that happens to match a DIFFERENT (recent/stale) option's
  name, prefer the in-range index N over the qualifier-collision stale_echo. Same remedy the wave-23
  target-menu fourth-cause needed, generalized past target menus.
- Harm: NIL (arguably negative-harm) — the heuristic paid 2 life to keep Temple Garden untapped,
  enabling the model's own turn-1 Flower cast (s5); entering tapped would have wasted turn 1. So this
  is a FALSE POSITIVE with no cost this time, but the mechanism can bite where the fallback diverges.

## LEDGER — R-SHOCKLAND-ETB-UNLABELED: shockland "pay 2 life / tap" ETB menu has no subject. [REPRESENTATION]

Sibling of the wave-22 R-PAINLAND item (self-damaging-mana life-cost surfacing, deck102). The Temple
Garden ETB choice renders as `Choose an option for :` (empty subject after "for") with bare options
`1. pay 2 life` / `2. tap`, giving no signal that this is the "enters tapped unless you pay 2 life"
shock decision. The model burned ~1,500 chars at deck102 s4 unsure whether the prompt was
"malformed." Repros both times a shockland enters (deck131 s3 the model guessed right at "pay 2
life"; deck102 s4 it stale-echoed). FIX (representation): label the menu, e.g.
`As Temple Garden enters: pay 2 life (enters untapped) / do not pay (enters tapped)`, mirroring how
other ETB/cost menus carry their subject and consequence. Low deck-priority but corpus-general
(every dual/shockland in every deck hits it).

## LEDGER — R-BLOCKERS-CONFLICTING-LINES: multi-BLOCKS reply partial/ambiguous application. [PARSER, flag-not-diagnosed]

deck59 s39 (life 6, facing four 2/2 persist attackers = 8 lethal; blockers B1 Human 1/1, B2
Lovestruck Beast 5/5). Reply (13,642 chars — deck137's max this corpus) opened with an ILLEGAL
first line `BLOCKS: B2:A1, B2:A2, B2:A3, B2:A4` (one blocker onto four attackers, forbidden by the
prompt's own "AT MOST ONE attacker" line), then re-derived the constraint over ~8k chars and reached
a coherent CORRECT plan `B2:A4` (Beast on the Fear creature) + `B1:A1` (Human chump). Recorded
outcome chosen_text = "Human blocks Safehold Elite" = B1:A1 ONLY — matches NEITHER the illegal first
line (B2:A1) NOR the model's full corrected 2-block plan. This is a partial/ambiguous application of
a reply carrying conflicting BLOCKS: lines.
- FLAG (do not over-claim): I cannot fully resolve from the log which BLOCKS line the parser
  consumed or reconcile the recorded single-chump against the combat math (model stayed at 6 life
  through t11, then lost by adjudication — not a kill). The blockers parser's behavior on a reply
  whose FIRST BLOCKS line is illegal (all-on-one-blocker) and whose LATER prose lines carry the
  corrected assignment is worth the parser owner's inspection. Repro: deck59 s39 (log
  1784919879-ai_baka_deck137-0x557cf14ea0d0-vs-ai_baka_deck59.jsonl).
- Deck-side mitigation already shipped: strategy.txt now carries a blocking-under-lethal commit rule
  so the model doesn't open with the illegal assignment. But if the parser mis-applies conflicting
  lines, that is engine-side and independent of the guide.

## HARNESS — deck137 is the corpus's decision-density + timeout risk. [note]

- The 27vs137 TIMEOUT-DRAW decomposed as a LATENCY-BOUND slow grind, NOT a stall: deck137 seat 52
  LLM decisions x median 18.5s = 1,835s (one 197.2s spike); deck27 seat 22 x 16.4s = 517s; combined
  2,352s ~ the 2,400s cap. Pregame took normal time. deck137 makes ~2.4x the opponent's decisions by
  turn 10 (adventures double cast options, convoke adds defers, go-wide multiplies creature/attacker
  choices) — it is the pool's most decision-dense deck and the one most likely to hit the cap under
  saturated -j3 latency. RECOMMENDATION: for corpora including deck137, raise WAGIC_GPT_TIMEOUT or
  drop to -j2, or accept that deck137 grinds are expected to cap-adjudicate. Not a play failure.
- The 19 deferred_to_heuristic (convoke tap-payment) fallbacks are all by-design and HARMLESS this
  corpus — the heuristic's tap-selection never stranded a wanted attacker or blocker (verified
  deck131 t5/t7/t9 attacks preserved; deck59 t8 convoke did not strand the t11 blockers). No item;
  noted so synthesis knows deck137 remains the sole defer source (19 of the corpus's 22 fallbacks).

## CARD-SCRIPT / ORACLE-VERIFICATION — no new items.

Wave-23's Lovestruck/Emmara/Loxodon/Once-Upon-a-Time engine-vs-Oracle checks stand (all AGREE with
Oracle; no fix needed). No new card-behavior discrepancies surfaced this corpus.

## ROTATION VERDICT

**deck137 does NOT rotate.** Its guide was MODIFIED this wave (R-ADV-FACE crutch demoted + a new
blocking-under-lethal commit rule added) AND there is a fresh new-work signal (the blockers-seam
spiral + the R-STALE-ECHO-QUALIFIER / R-SHOCKLAND-ETB / R-BLOCKERS-CONFLICTING-LINES ledger items).
This is only its initial-guide ROUND 2 — one guided corpus is not enough to judge convergence, and
the guide just changed, so the new guide is an unvalidated hypothesis that must run again. Keep in
pool. (Rotation candidacy needs no-guide-mod AND no-new-work-signal; deck137 fails both.)
