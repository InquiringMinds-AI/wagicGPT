# Notes — deck44 wave 22 (self-contained; layer-routed engine/harness/model items)

Run: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260723-173843/`. Binary `1d8ed8194`.
deck44 = deck1 in every game except vs140 (deck0). Record 4W-2L. 192 decisions; answer-first 192/192;
0 fallbacks; 0 self-GFTT. Win column context only (doctrine).

game -> result: vs27 L(kill -4,T10) | vs135 W-adj(10/4,T14) | vs62 W(kill,T13) |
vs102 W-adj(17/2,T15) | vs109 W(kill,T13) | vs140 L-adj(18/24,T35).

## Guide change this wave: EDIT (3 diffs, net +2 lines) — the crutch-lifecycle demotion the rotation earned
Deployed live md5 `bd097b0c4cf8524fb6cc30b75312cabe` (173 lines, frozen since wave-13). Revised
`strategy.txt` = 172 lines. Diffs: (1) DEMOTE GFTT friendly-only walkthrough (5->3 lines, keep anchor +
marker string); (2) DELETE redundant SITUATIONS bullet duplicating the engine-annotated self-GFTT case;
(3) ADD Cruel-Edict has-a-creature clause (observed muddle vs140 s6/s21). See findings F1/F2.

---

## ENGINE / HARNESS — CONFIRMATIONS (validations, not new defects)

### C1 (CONFIRMED CLOSED — engine ledger #7, own-target-only removal) self-GFTT structurally defended
Wave-16 requested suppression of a top-level removal cast whose only legal targets are friendly. The
durable fix shipped as an OPTION-LINE ANNOTATION (not hard suppression): the GFTT option now carries
`(warning: this would harm your own side - only correct if you are deliberately triggering something)`.
Confirmed FIRED on ~28 windows (deck135 s7/s9/s10/s12/s14/s16/s17/s19/s21/s22; deck140
s18/s20/s21/s25/s27/s28/s31/s32/s33/s37-s43/s47-s53) and OBEYED 0/28 self-GFTT. Detector used:
`chosen_text contains 'Go for the Throat' AND 'YOUR OWN'` -> 0 hits. The annotation route reached the
terminus (fired + obeyed) -> the guide crutch was DEMOTED per the crutch-lifecycle rung. Owner: none —
behaving correctly; logged as the closure of a previously-open ledger item.

### C2 (CONFIRMED CLOSED — wave-16 secondary model-watch) stack-vs-battlefield removal targeting
The same GFTT option now also carries `[this cannot target the spell on the stack - battlefield
permanents only]`. This directly answers the wave-16 model-watch (the caster believed GFTT could kill a
creature on the STACK). At vs140 s27 the model read it and correctly countered Pyroclasm rather than
reaching for GFTT. Owner: none — closed.

### C3 (CONFIRMED — translog serialization) options now logged as a LIST
Wave-16 nit #2 ("options logged as an INT COUNT, not the list") is FIXED: every record carries
`options_text` as the full option list. Reviewer-facing improvement confirmed. (Still no explicit
`resolved:` field — zone deltas scraped from `events`/next-prompt narration, as before. LOW.)

### C4 (CONFIRMED WORKING — colored-needs-first payment, the deck44 wave-11 finding) held-mana discipline intact
deck44's wave-11 finding drove the colored-needs-first payment fix. Outcome-level confirmation this
corpus: counters fired on the opponents' turns (vs140 s11 Lightmine Field T9; s27 Pyroclasm T21), i.e.
deck44 left {u}{u} untapped rather than tapping out on its own turn. Obyra {u}{b} cast off a mixed
Island/Swamp board (vs140 s16) resolved without a color fizzle. 0 uncastable-cast refusals from
color-payment error in any game. Owner: none — fix honored.

---

## MODEL WATCHES (route to model-experiments, not any guide)

### M1 (decode-layer, harm already defused by answer-first) ahead/idle repetition loop
vs27 s8: a trivial land-drop ("Play Island") produced a 15,804-char reply that loops and re-derives and
is cut mid-sentence. `CHOICE: 1 (Play Island)` is line 1; the parser locked the correct decision before
the degenerate tail. Same family as deck27's wave-21 ahead-position loop, appearing at deck44's seat.
Harm = 0 (answer-first). Do NOT open a decode-rung campaign; track + try repetition_penalty. The two
longest replies (vs27 s8 15.8k, s15 14.8k) both sit at low-stakes/idle decisions — the loop correlates
with low-information prompts.

### M2 (Sleep-Cursed Faerie untap timing) paid {1}{u} in Upkeep against guide
vs135 s26/s27/s28 (T13, Upkeep): model paid the {1}{u} untap to accelerate the wake, which the guide
says to do only in Main Phase 1. Marginal waste (game won; mana unneeded those turns). Present-and-
disobeyed — a prose rule the model overrode, not a guide-authoring gap. Watch whether it recurs at a
seat where the wasted mana would have mattered before treating as a class.

### M3 (Cruel Edict reasoning muddle — ADDRESSED in guide) 
vs140 s6, s21: model applied the guide's "Cruel Edict with no creature does NOTHING" line while it DID
have a creature, visibly second-guessing itself. It reached the correct answer (let a cheap body / then
Obyra die rather than Arcane Denial into their 2 cards) but the confusion was guide-induced (rule only
covered the no-creature case). Fixed with one clarifying clause (guide diff #3). Logged here for the
model-watch angle: the model treats guide lines as rules to match rather than principles to apply, so an
incomplete rule produces a muddle — a general argument for stating the PRINCIPLE, not just the case.

---

## DECK-CONSTRUCTION FLAG (for the decklist owner — persists waves 5-16, reconfirmed) — TWO-SIDED color pinch
16 Island / 10 Swamp must serve BOTH double-blue counters ({U}{U} Counterspell) AND black creatures/
removal (Obyra {u}{b}, Bladecrafter {2}{b}, GFTT {1}{b}), so either color can be the pinch:
- **wave-16:** BLACK-screw (Obyra/GFTT/Bladecrafter dead on all-Islands, vs62 — a loss).
- **wave-22:** BLACK-screw AGAIN (Bladecrafter/GFTT dead on all-Islands, vs27 — a loss; model explicitly
  named the missing black source at s24). The recurring loss signature is the reactive-heavy no-black-clock
  draw. Levers (owner's call, decklist not guide): (a) more flexible-cost fixing (Oona's Gatewarden's {u/b}
  is repeatedly the only castable creature under screw); (b) 1-2 more cheap flyers to raise the aggro floor
  so close races (vs140 lifegain grind) don't stall out. The counters EARN their slots (carried the vs140
  Lightmine/Pyroclasm survival + multiple wins). Route: construction notes.

## ROTATION VERDICT (explicit, per brief)
**Guide: EDIT this wave (crutch demotion + 1 clause), then FREEZE. Seat: ROTATE OUT.**
Rationale: (1) the rotated-in re-validation is DISCHARGED — the old guide was audited against the current
skill, its two stale crutches DEMOTED/DELETED per the crutch-lifecycle rung, and the durable engine fix
they crutched (self-GFTT) confirmed fired+obeyed 100%. (2) Response-window / counter / held-mana / combat
/ answer-first ALL validated CLEAN — no new seat-only work signal remains. (3) Both losses route off-seat
(construction color-screw; lifegain-race matchup), neither guide-authorable beyond the clause already
added. (4) The residual signals (M1 decode-loop, M2 untap-timing) are model/parser-lane and do not depend
on deck44 staying in the active pool. The two rotation gates now AGREE: guide-mod happened THIS wave (the
demotion the rotation existed to perform) but is terminal, and no new-work-signal replaces it. Deploy the
revised guide, then rotate the seat out with the frozen guide as canary.

## One-line handoff
Rotated-in re-validation DONE: old guide's self-GFTT crutch DEMOTED (durable engine warning confirmed
fired 28x + obeyed 0/28) + 1 Cruel-Edict clause added; 4W-2L, answer-first 192/192, 0 fallbacks, 0
self-GFTT, counters/held-mana/combat all clean. Losses = black color-screw (vs27) + lifegain-race
matchup (vs140-adj). Wave-16 stack-targeting model-watch also closed by the engine annotation. Rotate out.
