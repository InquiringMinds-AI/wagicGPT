# deck105 -> strategy-writing-skill proposals (wave 32)

Seat: deck105 Infected Wounds (BG infect), STEP-0 guideless debut, **6/6 — the campaign's first
Step-0 perfecta**. All proposals carry their witness; single-seat items are tagged.

---

## P1 (PROMOTION-GRADE, 2-seat convergence with wave-31 HL8) — a Step-0 seat with a PERFECT record
needs the preserve ledger AND a second question the ledger cannot answer: WAS THE RECORD EARNED
AT THE SEAT, OR MANUFACTURED BY A RENDER DEFECT AT THE OPPONENT'S SEAT?

Wave-31 HL8 built the PRESERVE LEDGER for a strong Step-0 seat (deck158, 5/6) and framed the risk
as "do not legislate over a working seam." deck105 exposes the *other* half. Its ledger is
flawless (29/29 attacker declarations, 1/90 cast-nothing, 2/2 targets, 0/148 fallbacks) — and the
record is still not a clean measurement, because the ENGINE was feeding the six opponents a false
statement at the seam that decided every game:

> `Your life: 20. Unblocked, these attackers deal up to 5 - you would be at 15 - NOT lethal:
> block only where the trade favors you; taking damage while ahead is often correct.`

Against infect attackers all three claims are false. It fired in **12 of 12** infect blocker
windows across the six games; 4 ended in `no blockers`; and the *killing* swing of the vs36 game
was declined with the engine's sentence quoted back verbatim (deck36 s25 t12: *"Since the total
damage (5) is not lethal (I would drop to 15) ... I declare no blocks"*), after which poison went
6 -> 11 and deck36 lost at 20 life.

THE RULE, as a Step-0 obligation (proposed as a new Step 0-bis rule 0b, beside the preserve
ledger): **at a Step-0 seat that wins, run a CROSS-SEAT AUDIT before the record is reported —
read the OPPONENTS' logs at the seam the wins were decided on, and check whether the surface the
opponents were shown was TRUE about this deck's mechanic.** A deck whose mechanic is unusual
(alt-win, damage that is not life loss, an unrendered resource) is exactly the deck whose
opponents get a stale surface, because every render was written against the ordinary case. If the
audit finds a false surface, the record is reported as **CONFOUNDED**, the ledger item is routed
to notes.md, and the synthesis is told what to re-measure after the fix — never "deck N is
strong." Cheap to run: one grep of the opponents' prompts for the seam's fixed string, one
cross-tab of that string against `chosen_text`.

This is the OPPONENT-SEAT face of the wave-31 HL5 path-scoped sweep ("enumerate every path that
emits that surface") — but the missing path here is not a sibling code path, it is a sibling
SEAT, which no existing rung tells a reviewer to open. It is also the natural companion of the
wave-31 promotion of the consumer-seat idea: the reviewer verifies from the seat of the player
the surface is being shown to.

## P2 (PROMOTION-GRADE, extends wave-31 HL2 to a new object) — the verify-oracle rail must cover
the deck's WIN CONDITION and RESOURCE, and a resource the prompt never renders is a #1-slot item
BEFORE any strategy teach.

Wave-31 HL2 extended the rail from NUMERIC facts to PERSISTENCE facts ("does the body STAY?").
deck105 adds the layer above the card: **does the prompt render the RESOURCE this deck's win
condition is denominated in?** Here the answer is no, at every surface simultaneously — life
area, battlefield lines, narration, option lines and both combat seams — while the engine holds
the number (`Player::poisonCount`) and renders it for the HUMAN frontend (`GuiStatic.cpp:138`).
The cost is measurable and it is not a misplay, it is a belief the pilot cannot settle:

- vs36 s17 t12: *"I am at 20 life and need 10 poison counters to win; I currently have 0."*
  (actual 6). It won that game while writing *"bringing the opponent to 5/10"* on the swing that
  reached 11.
- vs146 s23 t15, a LAND DROP: *"the opponent ... has 11 poison counters (2+1+2+1+2+1+2 = 11)"*
  (actual 8), then *"I need 20 poison. I have 11."* — the threshold conflated with their LIFE.
- vs146 s22 t14, 8,827 chars / 143.6 s: *"If they have 11 poison counters, they have already
  lost. The game should be over. ... If the game is still going, then my count is wrong."*

THE RULE: **before writing a Step-0 guide, name the RESOURCE the deck's win condition is
denominated in and check it is RENDERED. If it is not, that is the seat's #1 finding, it routes
to the representation ledger, and the guide gets an explicitly-labelled STOPGAP counting
procedure with a removal condition — never a strategy teach that assumes the number is
available.** Corollary for the counting stopgap itself, and this is the part a reviewer gets
wrong: **when a hand-count has already been observed to come out WRONG, do not ship a more
careful counting procedure — ship a decision rule that does not DEPEND on the count.** deck105's
guide therefore bounds the count to one clause and then says "if the arithmetic is unclear,
assume you are closer than you think and attack with everything," because that is the correct
play at every value of the count. A procedure whose output is known to be unreliable must not be
load-bearing for any decision.

## P3 (single-seat, deck105; generalises the wave-31 HL3 payoff-permanent rung to a WIN-CONDITION
resource) — when a deck's clock is a COUNTER that never resets, the blocking-seam preservation
rule must be keyed to the ENGINE'S OWN rendered life forecast, not to a value judgement.

HL3 gave the shape: a role/value teach evaporates at the combat seam under local re-derivation,
so write a POSITIVE seam-labelled rule with a life-total off-ramp. deck105 supplies the cleanest
witness yet and one refinement. The witness: at vs146 s18 t12 the pilot blocked with Skithiryx —
4 of the 10 counters it needs, per swing — one decision after its OWN PLAN read *"Next turn,
attack with Skithiryx, Corpse Cur, and Plague Stinger to deliver 7 poison counters and win the
game."* Three more infect bodies were traded off at 19-20 life (vs36 s11, vs152 s8, vs139 s8).
The refinement: HL3's off-ramp is stated as a life-total condition the author picks; here the
prompt already prints an exact, per-window, arithmetic answer to that condition
(`you would be at N - NOT lethal`), so the rule is keyed to READING it rather than to a
remembered threshold. **Prefer an engine-surfaced number for the off-ramp of a preservation
rule wherever one exists** — it survives board states the author never saw, and it is the same
prefer-surfaced-over-hand-computed rung the skill already holds, applied to a GATE rather than to
a magnitude. (Caveat that must travel with it: verify the surfaced number is TRUE for the seat
you are teaching. It is true at deck105's own blocker seam — its opponents deal ordinary damage —
and false at the opponents' seams, which is P1.)

## P4 (single-seat, deck105; sharpens Step-5 rule 6, the mulligan numeric floor) — a mulligan
criterion must be checked for SATISFIABILITY against the decklist, not only given a floor.

Step-5 rule 6 (wave-31, deck139) requires a mulligan teach to carry a numeric terminator. deck105
shows a failure the terminator does not catch: a criterion the DECK CANNOT MEET. At vs146 s1 the
pilot shipped a 4-land, 3-cheap-infect-creature, two-colour opener reasoning *"Mulliganing for a
hand with at least one 1-drop"* — **deck105 contains zero 1-mana cards** (cheapest MV is 2:
Blight Mamba {1}{G}, Plague Stinger {1}{B}, Necropede {2}, Ichorclaw Myr {2}). It mulliganed
again, kept at 5, and bottomed BOTH of its spells to keep a five-card hand of five lands. A
numeric floor would have capped the spiral at two but would not have stopped the FIRST mulligan,
which was the error. THE RULE: **when writing (or reviewing) any mulligan teach, run the keep
criterion against the decklist and confirm a hand satisfying it EXISTS at a reasonable rate; and
in the guide, state the deck's actual cheapest cost as a flat fact and pre-empt the curve
criterion the pilot will otherwise invent** ("your cheapest card costs TWO mana; 'no one-drop' is
never a reason to mulligan - you will never find one"). Note the shape: this is the wave-31 HL10
WRONG-EXEMPLAR defect appearing at authoring time rather than at revision time — the pilot's
self-generated criterion names a card class the deck does not contain.

## P5 (single-seat, deck105; measurement instrument, refines wave-31 HL9) — when the low-stakes
tax rows are read first, sort the "I cannot cast X" strings and count DISTINCT beliefs, not
instances.

HL9 says sort the tax table and read the LOW-STAKES rows first. At this seat that worked exactly
as advertised — four of the top eight replies are LAND DROPS (8,683 / 8,358 / 5,346 / 5,262
chars) — but the useful step was the next one: grepping the whole corpus for the
"cannot cast / cannot afford / not enough mana" family returned **61 hits across 148 replies**,
and collapsing them yielded exactly TWO unsettled beliefs (the poison count, and generic-cost
payability off an off-colour land — re-derived from scratch eleven times inside ONE reply at
deck116 s9). The instance count is noise; the DISTINCT-BELIEF count is the finding and it is what
sizes the guide. Cheap addition to the rung: **after sorting the tax table, run one regex over
all replies for the misgiving family and report distinct beliefs with their instance counts.**
It also gives a free cross-check on an i-item: 61 re-derivations produced **zero** false declines
of an offered Cast, which is how i12 was validated for correctness while being shown not to touch
tax at all.

## P6 (single-seat, deck105; a rung for the UNEXERCISED CARD at a Step-0 seat) — HL6's
no-manufactured-rarity rail needs a positive authoring form for a card the corpus never drew.

HL6 forbids converting non-observation into a rarity claim. It does not say what to WRITE. Two of
deck105's fourteen non-land cards never appeared in a single prompt (Hand of the Praetors, drawn
0 times in 6 games; Relic Putrescence, drawn 4 times and cast 0 times), and both are
build-arounds. The form used in this guide, offered as the rung: **write the card's RULE, write
what to do when it IS present, and close with an explicit no-op clause for its absence** ("Cast it
the turn you can, then keep casting creatures. **If it is not in your hand, nothing above
changes.**"). The closing clause is the load-bearing part — it prevents the wave-20 ABSENT-ENGINE-
CARD failure where a guide's spine keys on a card the pilot rarely sees, while still teaching the
card, and it contains no frequency claim in either direction. Pair with the findings-side verdict
UNEXERCISED and a probe-deck note.
