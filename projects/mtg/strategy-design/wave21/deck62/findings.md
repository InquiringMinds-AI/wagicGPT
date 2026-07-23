# deck62 (Enchantresses) — REVISED-GUIDE ROUND 5 validation findings (wave 21)

Corpus: `matchups-20260723-084938`, binary `3be7f5a67`. deck62's FIFTH guided corpus. The wave-20 revision
(round 4, edit D — scope-sharpen of edit B's defensive chump-block clause) is live at
`Res/ai/baka/deck62_strategy.txt` (byte-verified identical to wave20/deck62/strategy.txt). 6 owner translogs,
**153 decisions** (ask 121, attackers 23, blockers 9). Record **4/6** (up from 3/6).

Bottom line: **the guide has CONVERGED and edit D is VALIDATED — the exact wave-20 favorable-block spiral is
now a clean 917-char one-line commit — and answer-first + parser hardening ELIMINATED the truncation-unparse
class (0 unparsed, 0 empty this corpus, down from wave-20's 29 unparsed). But the wave-21 parser gained a NEW
defect that deck62 is the corpus's prime witness for: the `retracted_choice` detector OVER-FIRES on deck62's
signature over-deliberation-of-a-settled-decision, discarding a CORRECT, committed, first-line, uncontested
CHOICE. All 3 of this seat's retracted_choice fallbacks are this over-fire (zero outcome harm this corpus
because all 3 were land drops, but a latent bug that would discard a correct answer on a consequential ask).**
This is exactly the collision the brief flagged as "the ONE place the new parser could be over-firing." Guide
disposition: **BYTE-FREEZE** (no guide edit warranted — the open items are all HARNESS/ENGINE lane).

## Record: 4/6 — matchup-shape swing on a near-frozen guide

| vs | result | life (me/opp) | turn | read |
|---|---|---|---|---|
| deck35  | **WIN** | 2 / -1 | 12 | Go-wide beatdown vs Slivers (prior "floor" — flipped W). Attacked 3->8 creatures every turn s17/19/21/23; opp 20 to dead. Attack-every-turn/go-wide line fired. |
| deck14  | **WIN** | 10 / -15 | 14 | Grindy Voltron win vs Deep Blue. Yavimaya + Saproling wave; the wave-20 s26 block class did NOT recur (s21 clean). |
| deck49  | **WIN** | 11 / -9 | 10 | vs Red/Dragons (prior "floor" — flipped W). Board formed this time (s22/s29 alpha strikes); go-wide closed. |
| deck102 | **WIN** | 20 / 0 | 8 | Clean beatdown vs Tergrid; Yavimaya attacks s12/s18, opp 20->0. Win line fired. |
| deck27  | LOSS | 0 / 20 | 9 | vs Zombies (prior WIN — flipped L). Opp's protection-from-green + unblockable (Inkfathom) + flyers walled the go-wide plan; never developed power. Matchup/draw floor. |
| deck135 | LOSS | 14 / 17 (adj) | 11 | 135 out-sizes (13/13 Abominable Treefolk + Ice-Fang flyers). Life-adjudicated at cap. Matchup floor. |

The +1 record is pure pool/draw variance on a guide that changed only edit D: vs35 and vs49 (both wave-20
"matchup floors") flipped to WINS, vs27 (a wave-20 win) flipped to a LOSS. Same structural verdict as waves
18-20: **turn-8-13 ground/go-wide deck; the record floors against faster/wider clocks and the guide cannot
draw a faster clock** (see general-suggestions #4). Win column is context-only per Step 0; decision quality is
the signal and it is healthy.

## OWNED ITEM 1 — classify all 4 fallbacks; adversarial audit of the retracted_choice detector

**deck62's 4 fallbacks = 3 retracted_choice + 1 stale_echo. There are ZERO blockers fallbacks this corpus**
(all 9 blocker decisions parsed clean, fb=null). The 3 retracted_choice are ALL FALSE POSITIVES — the detector
over-fired. Verbatim audit:

### The 3 retracted_choice — every one is a DETECTOR OVER-FIRE (the brief's flagged collision, confirmed)

All three are **land-drop asks** ("Play Forest" vs "Hold Forest"). In each, the model emitted the CORRECT
answer `CHOICE: 1 (Play Forest)` as its mandated FIRST line (answer-first working perfectly), then
over-deliberated a DOWNSTREAM PLAN detail using self-correction language ("Wait", "Correction:"), and **NEVER
retracted or re-emitted a different CHOICE for the land-drop question.** Confirmed each reply contains exactly
**one** `^CHOICE:` line (the correct first one) — no second/contradictory CHOICE, no disavowal of the choice.

- **vs27 s10 (T6, 18/20).** `CHOICE: 1 (Play Forest)` first, well-reasoned. The PLAN section then spirals on
  whether it can cast Fists of Ironwood (no legal target — Argothian has Shroud), quoting "Wait, Fists says..."
  and "Correction: I cannot cast Fists...". The retraction is about a FUTURE cast, not the land drop. Model
  reaffirms "I will play the Forest and pass" throughout. **Heuristic played the land** (battlefield went 3->4
  Forests at s11). Zero harm.
- **vs135 s5 (T5, 20/20).** `CHOICE: 1 (Play Forest)` first. ~9k-char PLAN spiral comparing Primal-Rage-vs-
  Fists+Druid sequencing and mana arithmetic — "Wait, Fists says...", "Correction: You cannot target her...".
  Reaches the right conclusion (Druid + Fists). The land-drop CHOICE is never touched. Heuristic handled the
  land. Zero harm.
- **vs102 s3 (T2, 20/20).** `CHOICE: 1 (Play Forest)` first — the reply even QUOTES the guide ("Play a land
  every turn you can — declining your land drop gains nothing"). PLAN then spirals on whether {1}{G} for
  Argothian is payable off two Forests ("Wait...", "Correction:..."), correctly resolving that {1} is generic
  and {G}{G} pays it. Land CHOICE never disavowed. **Events confirm** "Your Forest: hand -> battlefield". Zero
  harm.

**Diagnosis (adversarial verdict): the retracted_choice route is keying on retraction PHRASES ("Wait" /
"Correction:") appearing anywhere after the CHOICE line and, finding no replacement CHOICE, concluding the
CHOICE was retracted — when the retraction actually targets the downstream PLAN reasoning, not the CHOICE.**
This is precisely deck62's wave-20 N6 pattern (over-deliberation-of-a-settled-decision: reach the right answer,
keep deliberating a next step, use self-correction language) now colliding with the wave-21 detector. The
detector cannot tell "I retract my choice" from "let me correct my downstream reasoning." All three had a
single, well-formed, in-range, UNCONTESTED CHOICE line that the parser should simply have taken.

**Was each retraction real? NO — none of the three disavowed its CHOICE. Did the heuristic answer hurt? NO —
all three were land drops the heuristic plays correctly (deck62 loses nothing when the heuristic answers a
land-drop).** So this corpus's harm is zero. But the bug is latent and non-trivial: the same over-fire on a
CAST or TARGET ask (where the model settles the right cast, then self-corrects a downstream plan detail) would
DISCARD a correct answer and let the heuristic pick blind. Routed to notes.md as HARNESS-N9 (over-fire) with
the fix direction; deck62 is the corpus's prime witness (owns 3 of the 5 corpus retracted_choice, all
over-fires).

### The 1 stale_echo — a reply-protocol name leak (distinct class)

- **vs14 s29 (stale_echo, T10, 10/18) — Web TARGET CHOICE.** Menu instruction: "answer with the chosen
  TARGET's name (not 'Web')". Model wrote `CHOICE: 4 (Web targeting Yavimaya Enchantress)`: the **number 4 is
  correct** (option 4 = Yavimaya Enchantress, the right target — the 4/4 beater), but it prepended "Web
  targeting" — the exact thing the instruction forbade — so the parenthetical name did not match option 4's
  name -> stale_echo -> heuristic. **The heuristic Web'd a 1/1 Saproling (->1/3 reach) instead of Yavimaya**
  (confirmed at s32 board: "Saproling (1/3) ... {attached: Web}"). Marginal harm: Web adds no power; reach on a
  disposable chump is defensible and left the beater free — but it did NOT execute the model's clear intent.
  Game won. Sibling of the wave-20 N7-template reply-protocol leak: the model injected a spell-name qualifier
  into the CHOICE parenthetical. Routed to notes.md (HARNESS): a `CHOICE: N (...)` whose N is in-range and whose
  parenthetical CONTAINS the option-N name as a substring should prefer N over stale_echo.

## OWNED ITEM 2 — N8 blocker-fallback gang-chump (item h): UNEXERCISED at deck62

**Zero blocker fallbacks this corpus** — all 9 blocker decisions parsed clean. The heuristic fallback block
path (where wave-20's engine-sac lived: gang-chumping a 2/2 with the 0/1 Argothian) NEVER RAN at this seat, so
the item-(h) fix (0-power creatures skip kill-blocks / never pile as a 2nd blocker) **could not be validated
here.** Report null. N8 remains open and needs a seat where a blocker decision actually falls back — which
answer-first has made rare (wave-20's two block fallbacks were truncation-before-commit; this corpus every
block committed a first-line `BLOCKS:`). See notes.md N8.

## OWNED ITEM 3 — edit D (favorable-block litigation): VALIDATED — the spiral class is DEAD

The single most decisive result. Wave-20's costliest guide-caused defect was **vs14 s26** (T11): offered
`BLOCKS: B2 Yavimaya into A1 Lord of Atlantis` tagged "(you kill it, your blocker lives)", the model looped
15,525 chars litigating whether that favorable trade counted as edit B's forbidden chump-block, truncated with
only `BLOCKS: none` (opposite of intent), fell back, and the heuristic sacrificed an Argothian.

**This corpus, the SAME block recurs at vs14 s21 (Yavimaya into Lord of Atlantis, favorable trade) and is now:
`BLOCKS: B2:A1, B2:A2` on line 1, 917 chars, clean parse, ZERO chump-block re-litigation.** The reply commits
immediately and reasons cleanly ("my Yavimaya is a 9/9 that can kill both attackers ... without taking any
damage"). Edit D's decisive exception ("a favorable block with a REAL creature is a trade, not a chump — make
it and move on") landed exactly as designed; combined with answer-first (coded line first), the favorable-block
spiral is gone. (Minor rules slip, unrelated to edit D: the model declared one blocker against two attackers
— an illegal double-block; the engine resolved it to the first, `Yavimaya blocks Lord of Atlantis`. A model
rules error, not a guide or edit-D concern.)

Across all 9 block decisions: **8 are short (622-1190 chars) and none re-litigates chump-block.** The one long
block (vs135 s21, 9786 chars) is NOT a chump-litigation spiral — it is legitimate hard defensive
damage-minimization math (Ice-Fang Coatl 1/1 deathtouch + Abominable Treefolk 13/13 trample vs one real
blocker), and it parsed clean to the CORRECT final answer (see answer-first section). Edit D discharged.

## OWNED ITEM 4 — N7-template parrot: did NOT recur; parser-drop untested here

**Zero `[Number]`/`[Name]` template-placeholder parrots in any reply this corpus** (grep clean across all 6
games). The wave-20 N7-template case (vs49 s18) did not recur — answer-first (emit the coded line first, before
the model can wander into echoing the format template) almost certainly suppressed it. The parser's
template-placeholder drop (item c) therefore had nothing to drop at deck62 -> **untested at this seat** (report
null, not validated).

## VALIDATION FOCUS — ANSWER-FIRST + parser: working; no parse regression; a latent truncation exposure

- **Truncation-unparse class ELIMINATED.** All 6 of deck62's longest ask replies (10.0k-12.9k chars) START
  with a coded `CHOICE:` line and ALL parsed clean. Wave-20's long replies (up to 15,525 chars) truncated and
  fell back; this corpus's max is 12,947 chars with **0 unparsed, 0 empty**. Answer-first + parser hardening
  did exactly what they were meant to: long deliberations still occur but no longer break parsing.
- **Answer-QUALITY: no regression — but ONLY because "last well-formed CHOICE/BLOCKS wins" rescues it.** At
  deck62 the model reasons-then-REVISES, and its snap first-line answer is repeatedly the WORSE option:
  - **vs14 s26 (ask):** snap `CHOICE: 4 (2nd Yavimaya)` -> reasoning shows Fists is better (Argothian draw + 2
    tokens) -> re-emits `CHOICE: 2 (Fists)` last. Parser took 2. Snap answer improved by reasoning.
  - **vs135 s21 (block):** snap `BLOCKS: B2:A1` (block the deathtouch Coatl — the plausible "both die" instinct)
    -> real math shows blocking the 13/13 Treefolk saves 8 life (14 vs 6) -> re-emits `BLOCKS: B2:A2` last.
    Parser took B2:A2 (chosen=Yavimaya blocks Abominable Treefolk). Snap answer was the inferior block; the
    reasoned correction landed.
  In both, answer-first's snap-commit was WORSE than the model's reasoned conclusion, and last-well-formed-wins
  captured the improvement. **Latent exposure:** if a reply truncates AFTER the snap answer but BEFORE the
  correction, the worse snap answer executes. None truncated this corpus (0 unparsed), so it did not bite — but
  it is the structural risk answer-first carries for a revise-heavy model, and it is worth watching. See
  skill.md #2 and general-suggestions #2.

## Carryover checks (taught classes — all HELD)

- **Attack-every-turn / A-list: clean.** All 23 attacker decisions parsed (0 fallbacks, 0 A-list unparses, 0
  over-naming). Every `ATTACK: none` had opts=1 (empty eligible set) EXCEPT vs135 s13 (opts=3), where the
  eligible set was Argothian (0/1), Birds (0/1), and a Druid (1/3) crippled by Gelid Shackles into a
  deathtouch wall — a REASONABLE hold, not a durdle (disqualifier check: no viable-power attacker). No
  0-attacker durdles this corpus. Go-wide beatdowns fired in all four wins.
- **Computed-P/T distrust pocket: DEAD (5th consecutive corpus).** Tight-signature grep = 0
  recount-of-already-printed-P/T. Every Blanchwood/aura reference is prospective cast-value math. Retired watch
  stays retired (one-line null per the skill's cheap-negative-grep rung).
- **Enchant-opponent-gift ban / Argothian-first / engine-protection: no violations observed.**

## Guide disposition: BYTE-FREEZE (round 5) — no edit warranted

Edit D is VALIDATED (its target spiral class is dead). Every other taught class held. The corpus's open items
— the retracted_choice detector over-fire (N9), the stale_echo name-leak, and N8 (unexercised) — are all
HARNESS/ENGINE lane, unreachable by guide prose. No guide contradiction, no #1-band leak, no new model-fixable
pathology surfaced. strategy.txt is a byte-identical copy of the deployed guide. See notes.md for the rotation
verdict (the guide is converged; the only thing keeping deck62 in the pool is harness-witness value for the N9
over-fire fix).
