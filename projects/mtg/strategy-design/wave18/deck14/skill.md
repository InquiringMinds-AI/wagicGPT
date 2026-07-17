# deck14 (Deep Blue) — skill.md — proposals for the strategy-writing skill

Guideless first pass under Step 0-bis. All items are guide-authoring METHOD; none touches core.

## PROPOSAL 1 (NEW rung, single-seat WATCH, strong) — the COMBAT-ANSWER FORMAT-DRIFT class: a
## combat-math-heavy deck spikes reasoning tax AT blockers/attackers AND drifts out of the coded
## reply format, producing unparsed fallbacks that DISCARD a reasonable decision.
deck14 is the first seat where the dominant fallback source is COMBAT FORMAT DRIFT, not target/cast
menus. 3 of its 5 fallbacks are combat decisions with 8.6k / 10.2k / 13.1k-char replies (vs62 s34,
vs62 s37, vs135 s26). In s34/s37 the model reached a SENSIBLE block and then wrote the last line as
`BLOCKS: Gravelgill Axeshark: Saproling (1/3), Lord of Atlantis: Saproling (1/1)` / `BLOCKS: Air
Elemental:Canopy Spider, ...` — using CARD NAMES instead of the `B#:A#` codes the parser needs. The
decision was thrown away purely on format. The tax is deck-intrinsic: mono-blue combat is math-heavy
(flying evasion, Lord pump folded into [X/Y], persist recursion, "block while ahead?"). Method: for
ANY deck whose combat is derivation-heavy (flyers + lords/anthems + recursion), pair the "you rarely
block / trust the rendered [X/Y]" rule with an explicit FORMAT-DISCIPLINE line — "answer ONE short
line using the B#/A# CODES, never card names, never a trade enumeration." This is a THIRD face of
trust-the-rendered-number (the COMBAT face) plus a format guardrail. The DURABLE fix is engine-side
(parser accepts card-name block/attack assignments — notes.md E1); the guide line is the interim.
Promote off single-seat on a second combat-format-drift seat.

## PROPOSAL 2 (reinforcement, no new text) — the 0-ATTACKER durdle signature (deck35's wave-17
## instrument) fires CLEAN at a new deck TYPE, and sharpens WHICH decks are at risk.
deck14's only "structural" loss, vs35, has ZERO `attackers` records the entire game (dead turn 10);
all 3 wins have 6-8 attack-decisions. The instrument caught it independent of win/loss. Sharpening:
the durdle risk is HIGHEST for decks whose card pool LOOKS like control (bounce/draw/steal) even
though the deck is tempo/aggro — the blue toolkit actively invites the model into a passive control
posture. Corroborates the instrument AND Step-0-bis rule (d) "a first guide often COUNTERMANDS a
generic core reflex": deck14's countermand is "you look like control, you are tempo — attack." Same
shape as deck21's "hold-instant-speed" countermand and deck49's "burn is a clock, not removal."

## PROPOSAL 3 (reinforcement, no new text) — the INVENTED-OPTION-NUMBER failure is a new face of the
## existing "trust/obey the offered option list" meta-rule: the model reasons from its HAND/PLAN and
## answers with an option number that is not on the menu.
vs110 s8: menu offered `1. Cast Boomerang` / `2. Cast nothing`; model answered `CHOICE: 3 (Cast
Gravelgill Axeshark)` — a nonexistent number, sourced from its hand not the list. Distinct from the
wave-13/15 "decline a listed option you believe unpayable" face (that OVER-restricts; this one
OVER-reaches). Same root (menu is authoritative), opposite direction. Present-and-disobeyed → the
durable lever is engine/representation, not a core restatement (core already carries the meta-rule).
The guide off-ramp ("obey the offered list; if your card is not listed it is not castable now") is
the interim, and converges with the existing family — no core edit warranted.

## Convergence summary
- Confirmed general (already in skill): mana-is-trivial rule-#1 (mono-color); attack-floor on first
  pass for an aggro/tempo deck; countermand-a-core-reflex; the 0-attacker durdle instrument.
- New this seat (single-seat WATCH): combat-answer FORMAT-DRIFT rung (Proposal 1) — the combat face
  of trust-the-rendered-number + a format guardrail, paired with an engine parser lever.
