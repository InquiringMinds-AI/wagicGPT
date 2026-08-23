# Wave-36 validation — seats deck158, deck36, deck116

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260822-155508` (FP8 pilot, revised wave-35
guides deployed in `bin/Res/ai/baka/`, binary bb3cc9b12). Rubric: owner ruling #1 — win rate
irrelevant; adherence + divergence quality. All counts below are full-fraction counts over the
named class, not samples. Citations are `<opp> s<seq>` within the seat's own jsonl.

Corpus-wide at these seats: **0 fallbacks, 0 parse_note** across all 18 logs
(158: 149 decisions / 36: 212 / 116: 105). deck158 went 6/6, deck36 2/6, deck116 1/6 —
context only.

## deck158 (Orcs amass) — all wave-35 teaches validated; VERDICT: FREEZE

- **Corrected amass mechanism: 0 false-belief events.** Swept every reasoning trace for
  death-feeds-the-Army claims: every death-amass statement names Easterling Vanguard (the true
  card) — 139 s14 quotes the new line verbatim ("it amasses when it DIES… the ONLY creature"),
  139 s16 quotes "Vanguard is your first chump… its death pays you a counter" and chump-blocks
  with it. 152 s8's mid-trace wobble ("if it dies, it doesn't amass" re the attacking Army)
  self-corrects in-trace; 152 s12 "let March feed the Army" refers to March's attack trigger
  (true). G1 class: **0/149**.
- **Chump order: 1 occasion, correct.** Every blockers menu this corpus was single-blocker, so
  the Vanguard→Crebain→Bowmasters order never had a multi-candidate ask. The one chump taken
  was Vanguard (139 s16). Bowmasters chump declined at 14 life (36 s14), Crebain declined at 10
  (152 s29) — both consistent with keep-the-trigger/last-resort teaching. Order rule:
  unexercised beyond the trivial case, no counter-evidence.
- **READING THE NUMBERS: both classes dead.** All 12 damage-target asks audited
  (105 s10; 116 s7; 146 s12/s17/s21; 36 s12/s17; 152 s5/s17/s22; 139 s10/s23): **12/12 correct
  kill tests, 0 P/T transpositions, 0 "6 kills the 8/8"**. The exact wave-35 F1 shape recurred —
  Intrepid Adversary (3/1) at 152 s5 — and the trace quotes the worked pair: "a (3/1) IS a
  toughness-1 creature", SHOOT taken. 36 s17: "Both are 2/1. Dealing 8 damage kills either."
  The teach "One damage kills a creature whose printed toughness is 1 and NOTHING ELSE" is
  quoted at 146 s21, 36 s12, 152 s5.
  ⚠ **Verdict-present count: 0/12.** No damage-target option line carries any toughness-verdict
  render (grepped the whole corpus for would-die/survives/kills-it class strings: zero option-line
  hits). The brief's premise "verdict lines now render" is FALSE for binary bb3cc9b12 — the
  stopgap teaches carried the entire load and worked. **Do NOT demote the two stopgaps**
  (notes.md P6 removal condition unmet).
- **"at 0 life you LOSE": consumed.** Quoted in-trace at 146 s5; zero survival-at-0 errors
  (R1 class 0/149; the only other zero-arithmetic line, 105 s13, is correct).
- Divergences: 36 s20 and 152 s31 "Cast nothing" = lethal-on-board holds, both justified
  in-trace ("Attack wins"); no F4-class payability doubt anywhere.
- **Recommendation: FREEZE.** Every revised line fired and held at the standing pilot; open
  items are render-lane (verdict lines), not guide-lane. Rotation candidate per the wave-35
  findings' own condition, now met.

## deck36 (UB artifacts) — both reworks validated, Academy finally witnessed; VERDICT: freeze after one clause

- **Academy Ruins recursion: 12/12 correct.** Twelve "Put in Library with Academy Ruins
  targeting …" asks (all in the 158 game, s12–s30), every offered target Silver Myr or Salvage
  Slasher — neither on the two-name list — and every ask answered 0/pass. The wave-35 failure
  shape (take-for-lethal-math) never recurred. Re-keyed rule: FIRED at its ask.
- **Tolarian Academy: EXERCISED (HL10.1 discharged), 27/28 correct.** The 105 game drew it
  t3; 28 in-option asks, gate question cited in the trace at all 28 ("is this YOUR Main Phase 1
  AND am I casting something this turn?"; "tap now for later" named at s21/s28/s34). One
  violation: **s41 t11** — activated in MP1 with hand = one Swamp and land drop already used,
  trace claims it will "cast Swamp" (a land is not a cast); ~12 {U} evaporated, harmless.
  Proposed one-clause amendment before freeze: "playing a LAND is not a cast — a hand of only
  lands means the answer is 0."
- **Blocking outcome-line trust: validated, F4-class 0/20.** 20 blockers asks, all replies
  legal. Free-kill lines taken 2/2 (146 s8 — Master of Etherium free-kill, the exact wave-35 F4
  scenario, now taken; 152 s9 Cathodion). The one apparent decline of a free-kill line
  (158 s31, at 1 life) blocked the lethal 12/12 instead — the correct survival override, argued
  from the printed lines. Full multi-block declarations at 139 s36 / 146 s50 are legal
  (9 and 8 real blockers incl. animated lands; engine applied all pairs).
- **Sacrifice one-per-ask: 0 occasions** (no annihilator gauntlet reached this corpus — the
  116 game ended t8). Overruns: 0. Lane-B first-segment resolution + parse_note: never needed —
  no droppable segments were generated at this seat; parse_note 0 with 0 occasions is
  consistent, not evidence of the instrument.
- **Recommendation:** apply the one-clause land-is-not-a-cast amendment, then FREEZE. The only
  unexercised section this corpus (sacrifice ladder) was already validated at arm C in wave-35.

## deck116 (GU cheat) — Rule 9 clean, near-total adherence; VERDICT: FREEZE

- **Rule 9 / illegal multi-block: 0/10.** Ten blockers asks, every reply a legal single
  assignment (or none); the wave-35 `B1:A1, B1:A2, B1:A3` shape never recurred. The re-ask
  machinery never fired **because every reply was legal** — prediction confirmed on the
  guide side; the harness re-ask docket item remains untested (no illegal reply to exercise it).
- **Farseek-first exception: 0 pure occasions.** Three asks had Farseek castable with no land
  in hand (152 s17, 36 s6, 36 s9) — all three resolved by higher-priority rules (Amulet /
  creature-enabler first; 36 s6's trace weighs Farseek explicitly and applies "Rule #5 says
  creature/enabler first"). The exception's own seam (Farseek vs Explore/ToN ordering) never
  arose; Farseek cast count: 0 again. Unvalidated-but-harmless; keep the witness caveat.
- **R7 skip-legends: 2/2 respected, 5/5 reveals correct.** ToN picks: 152 s5 Emrakul; 152 s8
  Kozilek **with Emrakul in hand** (the worked example, executed); 146 s13 Emrakul; 146 s22
  Kozilek with Emrakul+Jin in hand; 158 s13 Emrakul. Fabricate (158 s8) → Amulet. Wave-35's
  one R7 miss class: 0 this corpus.
- **R5 residue: 0 true events (wave-35: 1).** The one candidate (158 s14, "Cast nothing" with
  Farseek castable, MP1) held mana for the same-turn Amulet activation of Emrakul (s15) —
  justified per the revised rule's letter (mana spent, turn not wasted).
- R3/R4 spot: 5/5 cheat activations took the biggest real monster, incl. the new
  decline-Piper teach honored live (158 s15: Emrakul over Piper); Piper never appeared in an
  attackers menu (R4 0 combat occasions).
- **Recommendation: FREEZE** (the findings' own condition — block seam holds — is met). The
  Farseek exception ships unexercised; acceptable as an additive condition with no observed
  cost.

## ALL-seats items

- **Trade-parenthetical trust: 0 contradictions.** Across all 41 blockers asks at the three
  seats: free-kill lines taken 6/6 where no survival override applied; the two overrides
  (36-vs-158 s31; 116-vs-139 s10 partial) are justified survival/value picks argued from the
  printed lines; 0 blocks assigned into "your blocker dies, attacker lives" without purpose
  (158's Vanguard amass-chump and 36-vs-146 s26's 1-damage Pest absorb are the only death-line
  blocks, both defensible).
- **Lane-A prediction 6 (hybrid Kitchen Finks) CONFIRMED live:** Finks offered and cast at
  116-vs-36 s6 with the `[hybrid: each {g/w} …]` annotation.
- **Lane-A prediction 8 CONFIRMED at these seats:** "BOTH your main phases" fact present in 40
  prompts; 0 hallucinated pre-main casts, 0 unparsed replies.

## New defects (docket candidates)

1. **Search-ask render: "[eligible]" marker promised, never rendered** — 10/10 whole-library
   search asks say "Pick ONLY from the cards marked [eligible] below" but no option line
   carries `[eligible]`; only the inverse tag `[does NOT qualify - goes to "shuffle"]` renders
   (that tag's scoping IS correct — verified all 49 options of 116-vs-158 s8). Pilots inferred
   correctly from the ELIGIBILITY sentence every time, but the header instructs a match against
   a marker that does not exist — trust-doctrine cleanup: either render `[eligible]` or reword
   the header to "cards NOT marked". (Also "only a artifact" grammar.) Class: misleading text,
   no observed damage.
2. **Damage-target verdict lines absent (0/12)** — whatever shipped the brief's "verdict lines
   now render" claim did not reach binary bb3cc9b12's target menus. Not a regression (the
   render never existed here), but the deck158 stopgap-removal conditions must stay open.
3. Minor, pre-existing: "Mana available: 1 total" undercounts variable sources (Tolarian
   Academy games; 36-vs-105 s31/s49 traces burn tokens reconciling it against the VARIABLE
   note). The note prevents misplays; a "+ variable" hint in the total would stop the churn.

No regressions attributable to the wave-35 guide revisions or the lane-A/B fixes were observed
at these three seats.
