# Dev notes — deck109 wave-13 (engine/harness/reply-protocol ledger with repros)

## Record: 5/6 (context only per doctrine) — matchup + draw-speed variance
Beat 110, 131, 44, 133, 135; LOST to 140. Result INVERTED vs wave-11 (then: lost 133 / beat 140-class;
now: crushed 133 20-0 / lost 140). n=6 draw-speed variance on the structural-edge opponents. Guide
FROZEN byte-identical (md5 9b82b500ac675be6936d2a31739ca615; `diff` vs live = empty).

## Corpus / provenance
- Run: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-183228` (21 games round-robin,
  decks 44/109/110/131/133/135/140). Binary `/tmp/wagic-a75e2625e` (Force r2 cost-target fallback +
  Force r3 offer-legality/dead-end removal + d5d7b1f2e auto-tap two-pass + c3e6d95b2 name-echo protocol
  + 42e8bc180 BLOCKS:none + 240s timeout). Spark @ GPU_MEM_UTIL=0.75 (no crash; latency cost ->
  13/21 games life-adjudicated). Core prompt unchanged (8th PASS). First clean post-contamination corpus.
- deck109 seat files (opp): 1784158351 (140), 1784158356 (110), 1784160756 (131), 1784162895 (44),
  1784168784 (133), 1784170894 (135).

## ENGINE-FIX VERIFICATION (this seat)
- **empty_reply = 0** at this seat (wave-12/13a flood was Spark dying; CLOSED). 3 `unparsed_reply` ask
  fallbacks (vs131 s17, vs44 s16, vs135 s12) — all the benign long-ramble discard class (no CHOICE line
  reached before the guard fired; heuristic answered), all in WON games. Do NOT re-diagnose.
- **Name-echo protocol (c3e6d95b2):** 100% compliance at this seat; 0 echo-vs-index conflicts (every
  `CHOICE: N (name)` echo matches option N). No reconcile arm invoked -> no wrong remap possible here.
- **BLOCKS: none (42e8bc180):** 1 window (vs131 s25), correct all-decline via the PARSEABLE
  `B1:none, B2:none, B3:none` form, NOT-lethal branch obeyed. Not a misparse of intended blocks.
- **Force of Negation (Force r2/r3):** N/A — deck109 has no counterspells.
- **Auto-tap two-pass (d5d7b1f2e) / color-stranding:** N/A — 20 basic Mountains, single color, no
  stranding surface. Brief item #5 (mana-line trust) N/A for the same reason.
- **Legal-target names:** rendered across all burn windows; 0 self-target burn picks.
- **adj rows:** vs140 (adj0, deck109 behind 5-21) + vs110 (adj1, deck109 ahead 15-4) cross-checked
  against seat life — consistent. Latency-driven, not play-quality (brief's binding note).

## LEDGER ITEM A (engine / harness — attackers parser) — MIXED index+name ATTACK form silently drops the named attacker
**NEW this wave; sharpens the brief's corpus-wide `ATTACK: <name>` ledger candidate.**
The attackers parser 1-based-resolves `A<n>` tokens but does NOT resolve NAME tokens on the same line.
Three reply forms observed:
- **FULL-INDEX** (`A1, A2, A3`): parses. 17/30 windows.
- **FULL-NAME** (`Hellrider`; `Rakdos Cackler, Legion Loyalist, Legion Loyalist`): UNPARSED ->
  `choice=-1`, `fallback=unparsed_reply`, `chosen_text='None'` -> baka heuristic attacks with ALL
  eligible -> OUTCOME-NEUTRAL for aggro. Repros: vs140 s33 (`ATTACK: Hellrider`; combat log shows
  Hellrider dealt 3+1), vs131 s19 (log: `declared attackers: Ash Zealot, Rakdos Cackler, Legion
  Loyalist`), vs135 s13 (log: `declared attackers: Stromkirk Noble, Rakdos Cackler, Ash Zealot`).
- **MIXED** (`A1, Rakdos Cackler`): PARTIAL PARSE -> takes the A-indices, DROPS the name, records a
  NON-FALLBACK `choice=N (count)` with fewer attackers than intended (SILENT — no fallback field).
  Repros (6 dropped declarations / 5 windows / 3 games):
  | seq | T | line | intended | recorded | dropped |
  |---|---|---|---|---|---|
  | vs140 s7  | 3  | `A1, Rakdos Cackler` | 2 | 1 | Rakdos Cackler |
  | vs140 s40 | 31 | `A1, Rakdos Cackler` | 2 | 1 | Rakdos Cackler |
  | vs110 s10 | 3  | `A1, Rakdos Cackler` | 2 | 1 | Rakdos Cackler |
  | vs110 s14 | 5  | `A1, A2, Stromkirk Noble, Hellrider` | 4 | 2 | Stromkirk Noble + Hellrider |
  | vs135 s8  | 3  | `A1, A2, Rakdos Cackler` | 3 | 2 | Rakdos Cackler |
  **Combat-confirmed:** vs140 s7 plan "attack with both to deal 3 damage total (1 from Noble, 2 from
  Cackler)"; next log `You declared attackers: Stromkirk Noble` (Rakdos held).
**Proposed engine change (route here, NEVER the guide/core):** in the attackers parse arm, after
consuming `A<n>` tokens, resolve any residual NAME tokens against the eligible-attacker roster by
unique-name match (mirror the name-echo/parseChoice reconcile) and UNION into the declared set. Fixes
the mixed-form silent drop AND makes the full-name form declare the exact named creatures instead of
falling to the heuristic. Owner: the attackers-record parse site in AIPlayerGPT.cpp.
**Outcome coupling (reported separately per wave-11 method):** OUTCOME-NEUTRAL in the 4 wins; a real
but NON-DECISIVE clock tax in the vs140 loss (dropped 2+2 Rakdos damage in a race lost 5-21 to a
lifegain-control deck deck109 could not out-punch regardless). Do NOT inflate to the loss cause; do NOT
downgrade because the corpus was mostly won — vs140 s7 is the clean, loss-decoupled-enough regression
fixture. Scoring caveat: invisible to window-count and `chosen_text`; score by parsed intended-vs-
recorded (this is why wave-11 mis-scored "26/26 full declaration").

## LEDGER ITEM B (engine / representation, cross-deck) — annotate HYBRID mana costs on the option line
**STATUS: still UNSHIPPED; the deck-guide card notes are VALIDATED as the interim carrier.**
0 `- hybrid:` strings in any deck109 prompt this corpus — option lines still render bare
`Cast Rakdos Cackler {b/r}` / `Cast Boros Reckoner {r/w}{r/w}{r/w}`. The card notes drove the hybrid
class to zero AND the engine executed every hybrid cast:
- Rakdos Cackler `{b/r}` cast + `hand->stack->battlefield` 5/5 (vs140 s5, vs140 s38, vs110 s6,
  vs131 s9, vs135 s6).
- Boros Reckoner `{r/w}{r/w}{r/w}` (triple hybrid off three Mountains) cast + executed 2/2
  (vs131 s27, vs133 s14).
- 0 "lacking black mana" declines; 0 hybrid spiral markers; Unleash "1/1 Counter" 4/4.
**Proposed engine change (unchanged, still owed):** append `- hybrid: pay each with {R} or {B}` (resp.
`{R} or {W}`) to any option whose cost carries a `{X/Y}` symbol. Owner: the option-text builder in
AIPlayerGPT.cpp (same site that renders `- legal targets right now: ...`). Interim card notes must NOT
be demoted until this ships (thin sample + sole carrier).

## Blocker-seam verification (1 window, correct)
| file | seq | T | my/opp | surfaced line | choice | verdict |
|---|---|---|---|---|---|---|
| 1784160756 (131) | 25 | 12 | 15/? | `up to 1 - you would be at 14 - NOT lethal` | `B1:none, B2:none, B3:none` (all-decline) | CORRECT (NOT-lethal branch OBSERVED OBEYED; parseable form) |
- Only 1 blocker window all corpus (deck raced). LETHAL branch + exception-(a) had NO window -> KEEP.
- **Obliterator-block class: ZERO test windows AGAIN (4th consecutive wave).** vs133 cast Phyrexian
  Obliterator but deck109 raced it out 20-0 by T8 (0 blocker-kind windows). Guardrail neither fired nor
  violated -> KEEP at full prominence. Punisher-rider USE verification still owed at the deck133 seat.

## Burn discipline
- 0 self-target burn. The self-target scan's one hit (vs140 s17, own Goblin) was a FORCED edict
  ("Choose the target for Sacrifice") — correctly sacrificed a tapped Goblin. Not a burn decision.
- Young Pyromancer token-maker exception FIRED: vs131 s7->s8 Searing Spear -> Young Pyromancer (choice
  2 over face), died s9. All other burn to FACE (9/9 default windows).

## strategy.txt freeze-check (FROZEN — byte-identical to the live guide)
`diff strategy.txt bin/Res/ai/baka/deck109_strategy.txt` = empty; md5 both 9b82b500ac675be6936d2a31739ca615.
Every line held; no decision-level failure class routes to the guide (the mixed-form attacker drop is a
harness/parser item -> Ledger Item A).

## Deck-construction observations (upstream, unchanged — do NOT relitigate at 5/6)
No card advantage / no reach vs a lifegain-control grind (lost vs140: opp lifegain engines Venser's
Journal + Staff of Nin pulled it back to 22 while deck109 flooded ~10 Mountains, first Hellrider T25).
No answer to evasion (flyers/shroud/pro-red) — beat 44 (untouchable flyers) on the race this wave, won
vs110 (affinity) same class. Structural ceilings, surfaced by draw speed; inherent to RDW.

## Resume handoff
1. Guide FROZEN byte-identical; the wave-10 hybrid line is now verified at ENGINE-EXECUTION level
   (Rakdos Cackler + Boros Reckoner both cast->stack->battlefield).
2. Two engine/harness items routed OUT of the guide: (A) NEW — attackers name->index reconcile arm must
   cover BOTH full-name (heuristic->exact) AND mixed (silent-drop) forms; (B) option-line hybrid-cost
   annotation STILL UNSHIPPED, keep the card notes.
3. Brief items at this seat: #3 BLOCKS:none PASS (1 window, parseable all-decline); #4 name-echo PASS (0
   conflicts); #1/#2/#5 N/A (mono-red, no counters/color-stranding).
4. Watches held: Obliterator-block class (4th zero-window wave, KEEP); LETHAL-branch chump (untested).
