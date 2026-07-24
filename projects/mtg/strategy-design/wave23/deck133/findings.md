# Wave 23 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange) — ROTATED-IN re-validation

Corpus: PRIMARY `matchups-20260724-013710` (21 games round-robin over 27/102/109/140/137/133/131).
Binary `0e6361732`. deck133 rotated back IN (one of the roster's last two guided-never-cycled decks;
guide FROZEN byte-identical since wave-11, 12th+ consecutive freeze through wave-16). deck133 was the
GPT seat in 6 games. **Pilot live this corpus: repetition_penalty=1.05.**

**Real record 5W / 1L — the corpus TOP seat (5/6), with the MOST fallbacks (5).**

| opp | deck | result | end my/opp | mode | note |
|-----|------|--------|-----------|------|------|
| 140 | RBW Control | **WON** | 27 / -5 | clean kill | Cruel Edict grind survived; forced-loss framing obeyed; GM drains 9 for the win |
| 102 | Tergrid | **WON** | 16 / -1 | clean kill | Obliterator + Geralf's beats; declined ~8 self-target FP windows |
| 109 | Mono-R Aggro | **WON** | 11 / -4 | clean kill | GM stabilize at 5 life (drain 4), then GM drain 6; escalate-exile annotation obeyed |
| 137 | GW Adventures | **WON (adj)** | 25 / ~6 | cap-adj | huge life lead; FP killed adventure/DFC face cleanly; GM drain 8 |
| 131 | (rotated-in) | **WON** | 22 / -2 | clean kill | GM drain 4; survived the s28 empty-reply outage with no board impact |
| 27  | Zombie/Undead | LOST | -2 / 20 | 0 dmg dealt | **structural no-reach**: opp flat 20 all game, mana-light, no wall/closer drawn |

**Verdict on the 5/6:** the five wins are decision-clean — the impact-cast list, the Gray Merchant
win/stabilize rule, and the self-target-FP discipline all held, and the NEW forced-loss engine surface
was handled well (below). The one loss (vs27) is the deck's known structural face under
life-adjudication: opp_life stayed flat at 20 across the ENTIRE game (deck133 dealt literally ZERO
damage), deck133 was mana-light and drew no Phyrexian Obliterator, no castable Gray Merchant, and no
early blocker, while the opponent went to the air with 2x Glen Elendra Liege (4/5 flyers) that Fatal
Push cannot hit and Bloodghast cannot block. **No hard deciding decision error in the loss** — one soft,
non-deciding inefficiency (repeated Liliana-of-the-Veil +1 symmetric self-discard while defending;
finding 4). Guide **revised** this cycle: two surgical ADDs at the new-pool seams (findings 3+4),
everything else FROZEN.

---

## FINDING 1 — [ITEM A: self-target-only Fatal Push] — CLEAN across ~42 windows, 0 self-casts. Route: ENGINE annotation VALIDATED. PASS.

deck133's long-standing signature watch. The engine annotation `Cast Fatal Push {b} - the only legal
targets are YOUR OWN right now` was offered at **~42 windows** this corpus (vs140 x16, vs102 x8,
vs131 x18). **The model chose the own-target Fatal Push at ZERO of them** — every window resolved to
"Cast nothing right now" or a real spell (Inquisition, Bloodghast, Geralf's, Obliterator, Liliana,
Yawgmoth, Gray Merchant). Combined annotation-obeyed streak now ~72/73 across wave-15/16/23. The
wave-15 lone misfire (self-FP via card-presence hallucination) remains a one-off; the suppression
lever stays PARKED (do not build). **All real Fatal Push casts hit an ENEMY creature** (vs27 s8/s10
Vodalian Zombie -> graveyard; vs137 s3 Flaxen Intruder // Welcome Home -> graveyard — a DFC/adventure
front face, hit cleanly; vs109 s8/s16 Legion Loyalist; vs137 s10 Faerie Guidemother). ZERO
self-destroys, ZERO fizzles.

---

## FINDING 2 — [FALLBACK CLASSIFICATION] 5 fallbacks: 2 empty_reply (transient) + 3 retracted_choice (NEW class). deck133 owns ALL 3 corpus retracted_choices; 0 unparsed_reply. Route: 2 ENGINE/HARNESS, mostly non-deciding.

deck133's 5 fallbacks (the corpus max) break down as **2 empty_reply + 3 retracted_choice** — NOT the
4 new unparsed_reply shapes (those live at other seats). All 5 played the heuristic; NONE deciding.

### 2a. empty_reply x2 — transient endpoint outage, board impact NONE.
- **vs27 s6 (reveal, my20/opp20, 2502ms):** the Thoughtseize hand-reveal step. Empty -> heuristic;
  the discard/reveal flow proceeded (next record surfaces Zombie Master as a discard target). No life
  swing, no board delta.
- **vs131 s28 (ask, my18/opp16, 2502ms):** a Snow-Covered Swamp land-drop. Empty -> heuristic
  ("Decline"); at worst one deferred land drop. **deck133 WON this game (22/-2).**
- Both are the uniform 2502ms fast-fail HTTP error the brief flags as transient/server-side. **Board
  impact = none, neither deciding.** Note-only (routed to notes.md).

### 2b. retracted_choice x3 — precious post-INDEX-WINS evidence. ONE genuine, ONE engine-caused, ONE parser over-fire.
This is the class the brief flags as precious. The discriminator I applied: **count the coded
`CHOICE: N` indices in the reply and whether the FINAL coded index differs from the first-line one.**

- **vs140 s9 (ask, my20/opp20, 71s) — GENUINE retraction.** Coded CHOICE indices in emission order =
  **[2, 1]** — the reply opens `CHOICE: 2 (Decline - do nothing)`, spirals (it had pre-committed in
  its prior PLAN to cast Gray Merchant, then discovered it had only 3 mana for a 5-mana card and
  reversed), and closes `So CHOICE: 1.` This is a real two-index self-contradiction -> retracted_choice
  fired CORRECTLY. Non-deciding: the heuristic put Bloodghast in play = the model's FINAL intent;
  deck133 won. Two contributors: (i) a **stale prior-PLAN** the model carried into the new prompt; and
  (ii) a **nameless option label** — the option list serialized to `['Put in Play', 'Decline - do
  nothing']`; the card name "Bloodghast" appears only in the prompt HEADER (`Choose an option for
  Bloodghast:`), not the option label (finding 5).
- **vs137 s26 (priority, my18/opp18, 85s) — ROOT-CAUSED BY AN ENGINE BUG.** Coded CHOICE = **[1]**
  (single, sustained). The engine offered `-1/-1 Counter with Yawgmoth, Thran Physician ... [cost:
  Life, Sacrifice]` whose card text is "Sacrifice **another** creature" — but deck133's battlefield is
  `(creatures: 1)` = **only Yawgmoth** (Bloodghast is in the graveyard). The cost is UNPAYABLE, yet the
  option was listed as legal. The model correctly diagnosed the impossibility ("I have no other
  creatures ... I cannot sacrifice a creature I don't control ... the only way this is legal is if I can
  sacrifice Yawgmoth itself? But the card text says 'another'") and spiraled 85s -> retracted_choice ->
  heuristic (Yawgmoth blocks Loxodon, a safe outcome). **This is an ENGINE finding** (offering an
  unpayable ability — notes.md), not a parser fault: the "retraction" was a correct refusal of an
  illegal option. Non-deciding (deck133 won 25/~6).
- **vs137 s29 (ask, my18/opp18, 59s) — PARSER OVER-FIRE.** Coded CHOICE = **[1]** (single, sustained
  `CHOICE: 1 (Play Polluted Delta)`). The reply contains "Correction:" and "Wait," but only about a
  SUB-POINT (whether Bloodghast gets haste), never about the coded choice; the PLAN restates option 1.
  The heuristic played option 1 (Polluted Delta) = the model's intent, so no harm — but the coded
  index was **never actually retracted.** retracted_choice fired on generic self-correction PROSE while
  the coded answer stood. **Latent risk:** in a window where the heuristic's guess did NOT match the
  sustained coded intent, this over-fire would flip a correct decision to the heuristic.

**Discriminator for the parser owner (notes.md):** gate retracted_choice on an actual contradictory
second coded CHOICE index (like s9's 2->1) OR an explicit disavowal of the SPECIFIC coded action's
legality/payability (like s26's unpayable-cost realization). Do NOT fire on generic "Correction:" /
"Wait," phrases that revise a sub-point while the coded CHOICE index stands unchanged (s29). Where a
single in-range coded index is sustained, INDEX-WINS should trust it.

---

## FINDING 3 — [NEW-LEAK: forced self-loss framing (a)] — VALIDATED CLEAN at a SECOND seat; the engine framing makes a guide rule UNNECESSARY (do NOT add one). One deck-specific ADD the framing can't provide. Route: guide ADD (Bloodghast fodder).

The new pool (140 Cruel Edict/Rakdos's Return, 102 Tergrid, 27 forced discard) exercises **forced
self-sacrifice/discard** — a mechanic deck133's frozen guide never had a rule for, and the exact seam
the wave-23 engine batch newly frames (item (a), owned by deck140). deck133 hit it repeatedly and
**handled it well:**
- **vs140 s14 (FORCED sacrifice, Cruel Edict):** options were two identical Bloodghast; the model
  sacrificed **Bloodghast** — the OPTIMAL pick, because Bloodghast recurs from the graveyard on the
  next land. The engine framing said "pick the card you can best afford to lose"; the model obeyed.
- **vs27 s14 (FORCED discard):** options CB / Gray Merchant / Inquisition -> discarded **Inquisition**
  (kept the finisher and the removal). Correct affordability read.
- **vs27 s20 (FORCED discard):** options CB / Gray Merchant -> discarded **Collective Brutality**,
  keeping Gray Merchant. Defensible (kept the finisher) though situationally CB was the go-wide answer;
  a judgment nuance, not a framing failure.

**Key routing consequence:** the engine's forced-loss framing carried the "keep your best, pitch the
least valuable" instruction directly on the prompt, and the model obeyed it WITHOUT any guide rule.
Therefore deck133 needs **NO forced-loss guide rule** — adding one would be redundant with the engine
framing (the exact anti-pattern the skill's method-headline-1(a) warns against). This is a clean
cross-seat validation of framing (a): contrast wave-22 deck140, which pitched two live sweepers to a
forced loss BEFORE the framing existed; now the framing exists and a second seat handles it correctly.

**The one thing the generic framing CANNOT know** is deck-specific: for deck133, Bloodghast is UNIQUELY
affordable to lose (it comes back), a fact the engine's generic "least valuable / spare land" heuristic
does not encode. That is exactly the deck-specific edge the guide exists to provide. **ADD** (compact
clause on the existing Bloodghast jobs list): Bloodghast is the best card to give up to any forced
sacrifice/discard because it returns on your next land. This COEXISTS with the engine framing (it
sharpens the generic heuristic with a deck fact) rather than being demoted by it.

---

## FINDING 4 — [NEW-LEAK: Liliana of the Veil +1 symmetric self-discard while defending] — soft, non-deciding, but a real guide GAP. Route: guide ADD (Liliana +1 caution).

vs27 (the loss): while at 18->14->6 life being beaten by flyers, deck133 activated its OWN Liliana of
the Veil **+1 (each player discards a card)** at s13 and s19 — a SYMMETRIC discard that forced it to
pitch its own cards (Collective Brutality, s20) while it was the defender who needed to keep answers.
The +1 helped neither player on board and emptied deck133's own hand of removal. The frozen guide's
Liliana section covers ONLY the -2 (edict) mode and is silent on +1 — a coverage gap for a card the
deck runs **3 copies** of, where planeswalker mode-selection is a recurring decision. This was
**non-deciding** (the game was lost structurally — no flyer answer, no wall, mana-light) but it is a
genuine, repeatable inefficiency the new-pool control/grind matchups expose. **ADD** (compact
paragraph): +1 is symmetric — use it only when the opponent holds more cards than you or your own hand
is dead; when defending/behind, prefer -2 or hold. This is doctrinally the rotation-in new-leak the
skill's method-headline-2 says to hunt for and address.

---

## FINDING 5 — [HARNESS/SERIALIZATION] nameless option labels `Put in Play` / `Decline - do nothing`. Route: HARNESS (notes.md). Cross-seat.

At vs140 s9 (and vs27 s17, vs140 s22) the "cast this permanent" binary serialized as
`['Put in Play', 'Decline - do nothing']` — the card name lives ONLY in the prompt header (`Choose an
option for Bloodghast:`), never in the option label. This is the same degenerate-label class as
wave-16's `Becomes ` finding (a target/permanent sub-menu whose labels drop the card identity).
Combined with a stale prior-PLAN it contributed to the s9 retraction spiral (the model spent paragraphs
re-deriving what "Put in Play" referred to). Fix at the serialization layer: emit the card name in the
option label (`Put Bloodghast in Play` / `Cast Bloodghast`). LOW, cross-seat (any cast-permanent
binary).

---

## GRAY MERCHANT — 5 casts, all reasonable; the strict wait-rule is being disobeyed on the EAGER side, outcome-positive under cap-adjudication. WATCH (no change).

- **Guide-perfect (win-or-stabilize):** vs140 s39 (my18/opp4, drains 9 -> cast to WIN); vs109 s22
  (my5/opp18, drains 4 -> cast to STABILIZE, life <=10 rule); vs109 s25 (my8/opp12, drains 6 -> cast to
  stabilize) — and at s25 the model correctly DECLINED the escalate-exile option annotated `this
  exiles your ONLY eligible card, Gray Merchant - you will lose it` (castability framing obeyed).
- **Eager-but-outcome-positive (strict wait-rule says WAIT):** vs137 s37 (my17/opp18, drains 8 — cast,
  N<opp & my>10) and vs131 s31 (my18/opp14, drains 4 — cast). Both bank a life-differential swing that
  is directly win-relevant under CAP-ADJUDICATION, and both games were WON. The guide's wait-rule ("if
  N<opp AND your life >10, WAIT and drain bigger later") is written for a non-adjudicated world; under
  life-cap adjudication, banking N damage + N life now is often correct. **Not a misplay; a WATCH** —
  the wait-rule may be mildly miscalibrated for the adjudication meta, but two outcome-positive
  deviations is too thin to change the guide. No devotion-hand-count ramble flipped any GM decision
  this corpus (the "use the shown number directly" line held on the decisions that mattered).

---

## MANDATORY AUDITS

- **Fallbacks (5): 2 empty_reply + 3 retracted_choice**, all choice=-1, heuristic played, NONE
  deciding (finding 2). deck133 owns ALL 3 corpus retracted_choices and 0 unparsed_reply.
- **Self-Fatal-Push watch — CLEAN** (~42 windows declined, all real FP hit enemies; finding 1).
- **vs27 LOSS — structural no-reach** (opp flat 20 all game, 0 damage dealt, mana-light, no
  wall/closer drawn vs go-wide flyers). No hard deciding error; one soft non-deciding Liliana +1
  self-discard (finding 4).
- **Castability / escalate-exile annotation — obeyed 2/2** (declined exiling Gray Merchant to
  Collective Brutality escalate: vs27 s18, vs109 s25).
- **DFC/adventure face** (deck137's Flaxen Intruder // Welcome Home, Faerie Guidemother // Gift of the
  Fae): deck133 casts no DFCs itself; the only interaction was Fatal Push killing the front face
  cleanly (vs137 s3/s10). No toggle issue at this seat.

## PILOT VERDICT (repetition_penalty=1.05): SAME (no degradation).
deck133-seat reply char-length: **p50 1103** (corpus ref 1106 — identical), p90 6052, **p95 8276**
(corpus 7784 — marginally fatter tail, within noise), **max 14846** (< corpus max 15901). Structured
CHOICE-first format intact in all 201 non-empty replies; reasoning coherent (even the 3 retracted
spirals reach correct conclusions); NO token-level repetition artifacts (no stuck loops, no degenerate
n-gram repetition) — the long replies are content-rich (mana/devotion math, legality reconciliation),
not repetition-driven. One latency outlier (vs137 s32: 205s / 13.6k chars) — a spiral, but within the
corpus max range and not prose-degraded. No spiral SHORTENING at this seat and no worsening. Verdict:
**same as prior corpora.**

## FREEZE / EDIT CHECK
Guide revised from the wave-11 frozen baseline with **exactly two surgical ADDs** at the new-pool seams
(finding 3: Bloodghast forced-loss fodder clause; finding 4: Liliana +1 caution). Everything else
BYTE-FROZEN. No demotion debt found to retire: the FP revolt/legal-target block was audited against the
engine's "legal targets right now" annotation and KEPT (the fetch-crack-to-enable-revolt sequencing
advice exceeds what the engine surfaces); the GM "use the shown number directly / don't hand-count
devotion" crutch was KEPT (the retracted spirals prove the hand-count temptation is still live). See
notes.md for the full kept/added/routed ledger.
