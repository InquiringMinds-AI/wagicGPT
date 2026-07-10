# General-prompt suggestions from the deck-109 (Hellrider, mono-red aggro) review

Corpus: matchups-20260709-220729. Deck 109 went 4-1-1 (best in corpus): WON vs 44, 135,
133, 140; LOST vs 131; TIMED OUT vs 110. Games cited by opponent + the deck-109 translog
epoch: w44=1783652855, L131=1783652856, w135=1783654166, t110=1783655262, w133=1783655265,
w140=1783656460. `seq` = record index in that translog.

Each suggestion: observed problem (with citation), the current general-prompt text at
fault (quoted), and proposed replacement/addition wording. All proposals are deck-agnostic
and written for the weak one-decision-at-a-time executor.

---

## S1. The hold-instants prior beats both the game state and deck-guide overrides — restructure the timing bullet

**Observed.** The pilot repeatedly held damage instants "for the opponent's tricks" while it
was the unthreatened aggressor with the opponent at low life and mana to spare:
- w140 seq 28: opponent at 8, six untapped lands, Searing Spear + Pillar in hand — "Cast
  nothing… I need to preserve my instant-speed burn (Searing Spear) for the opponent's
  potential tricks or blockers." The opponent had an EMPTY board. Game closed on turn 16
  instead of ~12.
- t110 seq 9/13: two Spears held across turns "as a backup to kill a potential hidden
  blocker," ending in the 40-minute timeout with lethal in hand (see S2).

This happened DESPITE the deck guide having a whole anti-hoarding section and despite the
general prompt's own burn carve-out — because the carve-out trails the bullet.

**Current text.** "Instant-speed cards (instants, flash creatures, activated abilities):
hold them for the opponent's turn or the exact moment you need them. …" with the corrective
buried after the sub-bullets: "Not every removal or burn spell is an answer to hold. Direct
damage and burn are part of your CLOCK, not reactions - as the aggressor, spend them to
pressure or finish (usually at the opponent's face) rather than hoarding them."

**Proposal.** The executor reads "hold them" and stops; the exception must come FIRST and
be keyed on something visible. Reorder and harden:

> - Instant-speed cards: FIRST ask what the card does. If it deals damage that can hit the
>   opponent's face, it is part of your clock — as the aggressor, cast it at the opponent by
>   the end of your own turn rather than saving it; "keeping it for their tricks" is how
>   aggressive decks lose slowly. Hold an instant only if it is purely reactive (a
>   counterspell, a combat trick, targeted removal you cannot point at a player) — those need
>   a TARGET; never cast one into an empty stack or empty board, and keep holding if no
>   target appears.

(Keep the second-main default for sorceries/creatures as is — see S6 for one addition.)

## S2. The lethal check never fires after combat — make it run TWICE, and name the second main phase

**Observed.** Three separate games where exact or near-lethal burn sat in hand through the
second main phase with the mana available:
- t110 seq 17 (the disaster): turn 8, Main phase 2, opponent at 6, hand = two Searing Spears
  (3+3 = exactly 6), FOUR untapped Mountains. Its own previous plan even said "cast Krenko's
  Command… then Searing Spear… should be lethal." Reply: "Cast nothing right now," deferring
  everything to a next turn that effectively never came — the game ran to the 40-minute
  timeout. A won game, forfeited to the clock.
- w44 seq 15: turn 8, Main phase 2, opponent at 4, Spear (3) + Pillar (2) in hand, 3 untapped
  lands — cast nothing; won two turns later.
- w140 seq 42: Main phase 2, opponent at 3, Spear in hand, 7 untapped lands — cast nothing
  ("my attackers are already dealing 5 damage, this is lethal" — combat was already over).

**Current text.** "Combat is arithmetic. Each turn, check for LETHAL: add the power of the
attackers that will connect, plus damage from any 'when this attacks / when this deals
damage' triggers, plus burn you can send to the opponent's face; if that total is at least
the opponent's life, attack with everyone and spend the burn - never leave lethal on the
table to keep cards in hand."

The check is framed as a pre-combat computation; the executor never re-runs it once combat
is done, and post-combat is exactly where burn-only kills live.

**Proposal.** Add one sentence to that bullet (or its own line):

> Run the lethal check TWICE every turn: before declaring attackers, and AGAIN in your
> second main phase. Second-main version: add up the damage of the spells and abilities you
> can pay for with your untapped mana right now; if the total is at least the opponent's
> life, cast them all at the opponent NOW — the game ends this turn.

## S3. Missing rules fact: attacking does not tap lands (a recurring, game-losing delusion)

**Observed.** In 4 of 6 games the pilot asserted its lands were tapped when the board text
showed them untapped, and chose "Cast nothing" on that false premise:
- t110 seq 15: FOUR untapped Mountains on the battlefield line; reply: "I have no untapped
  mana sources (all three Mountains are tapped from attacking on Turn 6)."
- t110 seq 13, w140 seq 16: "all lands are tapped from attacking" — same hallucination,
  post-combat, lands visibly untapped.
- L131 seq 23/25/26: "no mana available (all my Mountains are tapped from attacking)" with
  3-4 untapped Mountains and castable creatures in hand, at 6 life vs 6 — a direct
  contributor to the loss.

The model has internalized "attacking taps things" and over-applies it to lands, and also
seems to believe lands stay tapped across turns. Nothing in the current rules paragraph
addresses this.

**Current text.** The rules paragraph covers summoning sickness, "Tapped creatures cannot
block," and combat damage — nothing about what attacking taps or when permanents untap.

**Proposal.** Add to the rules paragraph (near the summoning-sickness sentence):

> Declaring an attack taps the attacking CREATURES only — your lands never tap from
> attacking, and everything you control untaps at the start of your turn. The mana you can
> spend right now equals your untapped lands: count the lands in your battlefield list that
> are NOT marked [tapped] before deciding you "have no mana."

(Complementary (c)-layer fix, flagged for the representation owner: an explicit
"Untapped lands: N" line in the CURRENT SITUATION block would kill this delusion at the
source. The prompt-fact above is the general-strategy-layer share of the fix.)

## S4. Affordability is pre-checked — say so

**Observed.** The pilot declined listed cast options while claiming it could not pay for
them: w140 seq 14 — "Cast Krenko's Command", "Cast Searing Spear", "Cast Pillar of Flame",
"Cast Boros Reckoner" all offered as numbered options (4 untapped lands); reply: "I lack
the mana to cast any of my spells… cast nothing." Same premise in the S3 citations. The
engine only offers actions that are currently legal and payable, but the prompt never says
so, so the model re-derives affordability and gets it wrong.

**Current text.** "…and the choices that are legal for you right now." (The word "legal"
does not carry "affordable" for the executor.)

**Proposal.** Strengthen that sentence:

> …and the choices that are legal for you right now. Every listed action is already
> checked by the game engine: if "Cast X" appears in the list, you CAN pay for it and cast
> it right now. Never decline an option because you believe you lack the mana — if you
> truly lacked it, the option would not be listed.

(Verify with the engine owner that the options list is strictly affordability-filtered in
all cases before shipping this wording; every instance in this corpus was consistent with
that.)

## S5. Summoning sickness is being read as "don't cast it" — close the inference

**Observed.** The pilot repeatedly held creatures in hand because they could not attack the
turn they were cast:
- L131 seq 13: "Do not cast Rakdos Cackler this turn as it has summoning sickness and cannot
  attack, meaning it contributes zero damage this turn." Same again at L131 seq 20. The
  creature then stayed in hand for the rest of a game lost at 6 life vs 6.
- w140 seq 37-42: Rakdos Cackler held with 7 untapped lands across the final turns.

The rules paragraph states the (true) summoning-sickness rule; the executor converts it
into a casting veto, delaying its own clock a full turn per creature.

**Current text.** "Creatures cannot attack the turn they enter play (summoning sickness)
unless they have haste." — and the act-every-turn bullet does not address the inference.

**Proposal.** Append one clause to the rules sentence:

> Creatures cannot attack the turn they enter play (summoning sickness) unless they have
> haste — but a creature must be ON the battlefield a turn before it can attack, so cast
> your creatures anyway: summoning sickness is never a reason to keep a creature in your
> hand.

## S6. The "later" loop: plans that defer to next turn, then defer again — anchor "later" to this turn's second main

**Observed.** A stable failure shape across t110 and w140: the stated PLAN each turn
promises "next turn I will untap, cast X and Y, and finish them" — and next turn produces
the same plan again (t110 seq 13→15→17 is three consecutive deferrals of the same Krenko's
Command + Spears line, ending at the timeout; w140 turns 12-16 likewise). The second-main
default gives the executor a respectable-sounding place to park inaction.

**Current text.** "Non-instant cards…: cast them in your SECOND main phase (after combat)
by default…" and "passing your turn with castable cards and unused mana is a losing habit."

**Proposal.** Add one sentence to the second-main bullet:

> "After combat" means THIS turn's second main phase, not some future turn: if you reach
> your second main phase with the spell still in hand and the mana untapped, cast it now.
> A plan that says "next turn" for something you can pay for this turn is a misplay.

## S7. Own-side targeting guard (cheap, one line)

**Observed.** L131 seq 19: the pilot cast Pillar of Flame, its reply said "targeting the
opponent's face," and its actual menu pick was option 9 — its OWN Goblin token (marked
"[your battlefield]" in the option text), which died. Single occurrence in this corpus, but
it is a pure downside with a one-line guard.

**Current text.** The choose-by-IMPACT bullet covers picking among enemy targets; nothing
forbids own-side picks.

**Proposal.** Append to the choose-by-IMPACT bullet:

> When targeting your own damage or removal, never choose your own creatures, your own
> permanents, or yourself unless the card's text requires it or you state the reason in
> your plan. Options marked "[your battlefield]" are almost never the target you want.

---

## What WORKED at the general layer — do not weaken (evidence from the corpus's best record)

- **"Decide who is the beatdown… be the aggressor by default"** and **"As the aggressor,
  attack every turn with everything not needed to survive"**: deck 109 declared wide attacks
  in essentially every combat of all six games (e.g. L131 seq 21 "A1, A2, A3, A4, A5"; w133
  seq 16 five attackers; w135 seq 18 seven attackers), never sat home to block, and its four
  wins were all delivered primarily by creature combat. The aggressive posture priors are
  landing — the failures were post-combat spell handling, not combat posture.
- **The anti-passivity "Your own turn is for ACTING" bullet**: main-phase-1 development
  mostly happened (land + a cast nearly every turn early). The observed durdling was
  localized to the false-mana belief (S3/S4) and the timing/lethal seams (S1/S2/S6), so fix
  those seams rather than escalating the general anti-passivity language.
- **"Pay costs that advance your plan"**: every unleash-style "1/1 Counter" option was taken,
  in every game it appeared (w44 seq 6, w133 seq 6, w140 seq 7, t110 seq 6) — consistent,
  correct, no hesitation. Cost-paying priors are calibrated.
- **The lethal-check bullet's pre-combat half**: attack-based lethal was found fine (w133,
  w135 closed on-curve through combat). Keep it; S2 only extends it past combat.
