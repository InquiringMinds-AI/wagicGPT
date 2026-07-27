# deck146 -- wave-31 proposals for the strategy-writing skill

Two proposals, both evidenced by this seat's central finding: a guide that had passed three
consecutive freeze validations was carrying three false card facts, one of them game-costing.

---

## Proposal 1 (STRONG) -- the verify-oracle rail must be applied RETROACTIVELY, and a full-guide card-fact audit must gate any FREEZE or EXIT

**What the skill has today.** Wave-28 headline 3 promoted a hard rail at the guide-DRAFTING layer:
every numeric card fact in a guide being drafted or revised must be checked against Oracle/primitive
before it ships (wave-30 skill, lines ~461-478). Wave-30 headline 3 generalized it to the engine
render. Both are DRAFT-TIME rails.

**The gap.** Neither rail ever looks back at guides written BEFORE the rail existed. deck146's guide
was authored pre-rail and has since been frozen twice and validated three times, and it contains:

- `Hive of the Eye Tyrant is a manland: pay {1}{B}` -- the primitive is
  `auto={3}{B}:name(Becomes beholder)`, i.e. **four mana, double the guide's number**. A clean
  numeric card-fact error of exactly the class the rail names; it survived because no reviewer ever
  re-read the guide against the primitives, only checked whether its teaches fired.
- `Acererak (on enter)` listed among "sticky value creatures", with no mention that the primitive
  bounces him to hand every time (see Proposal 2) -- 59 impossible plans and one lost game.
- `Your MDFC cards (Emeria's Call, Agadeem's Awakening) can be played as LANDS - a hand with one of
  them has an extra land` -- structurally unofferable per h4/N-152a, confirmed at this seat
  (vs158 seq29 t14, land options `['Play Plains','Hold Plains']` with Agadeem's in hand).

Three false teaches in one "converged" guide is not a deck146 accident; it is what you should expect
from any guide drafted before wave-28 and then only ever validated by the
does-the-teach-fire method. The freeze/exit machinery is currently blind to them by construction:
**a teach that is false but never exercised fires zero times and therefore looks fine.**

**Proposed rail.** Add to the freeze/rotation gate in Step 0 (and to the near-terminal exit
checklist): *before a guide may be declared frozen, or a seat may be projected for exit, run the
verify-oracle rail over the ENTIRE deployed guide -- every mana cost, activation cost, MV, P/T,
loyalty number, targeting restriction and zone claim -- against `Res/sets/primitives/*.txt`, not just
over the lines being revised this wave. Record the audit in findings.md as an explicit line item.
A seat with an unaudited guide is not eligible for an exit projection.* Cost is bounded (a deck guide
is ~110 lines and a handful of greps) and it is a one-time cost per guide, since after the first
audit the draft-time rail keeps it clean.

**Corollary for the reviewer method.** Distinguish two verdicts that the current method conflates:
*teaches that fired correctly* (a behavioural check -- deck146 passed this three times) and *teaches
that are true* (a card-fact check -- deck146 has never passed this). Only the second licenses a
freeze. Phrase the freeze verdict as two lines, not one.

---

## Proposal 2 (STRONG) -- extend the verify-oracle rail from NUMERIC facts to PERSISTENCE facts: does the creature this guide names actually STAY?

**The gap the numeric rail does not cover.** Acererak the Archlich has no wrong number in the guide.
Its cost, its P/T and its venture trigger are all stated correctly. The defect is an **omission of
persistence**: primitive `borderline.txt`,
`auto=if type(Tomb of Annihilation[dungeoncompleted]|myzones)~equalto~0 then name(Return to hand)
moveTo(hand)` -- he returns to hand on ETB unless Tomb of Annihilation is completed, and the same
guide (correctly) tells the model never to pick Tomb. So he is structurally never a body in this
deck, and the guide's framing ("sticky value creatures that VENTURE INTO THE DUNGEON") asserts the
opposite by category.

**Why this is a distinct, generalizable failure mode and not a one-card slip.**

1. The render cannot cure it -- `Cast ...` option lines carry no card text at this seat (0 of 163;
   see notes.md N-146i), so the model sees `Cast Acererak the Archlich {2}{b} (5/5)` and a printed
   power that will never attack.
2. The belief is DURABLE against contradicting evidence. The model repeatedly reads the bounce in
   the narration and states it (vs152 s11 *"Acererak returned to my hand, so I cannot cast him
   now"*; vs152 s18 *"Wait, looking at the log: 'Your Acererak the Archlich: battlefield -> hand'
   implies he returned to hand"*) and then re-adopts the false belief on the very next decision --
   **59 attack-with-Acererak planning passages across 3 games.** Observed narration does not
   overwrite a category belief the guide installed.
3. It converts into concrete play-quality loss at the highest-stakes seam. vs148 seq18 t8: four
   mana, three Vanishing Verses in hand, `Armament Master` (the Kor lord,
   `auto=this(gear = 1) lord(other creature[kor]|myBattlefield) 2/2` scaling to 18/18) offered as a
   legal Verse target -- the model cast Acererak instead, reasoning verbatim *"He is a 5/5 threat
   that forces the opponent to either sacrifice a creature... This immediately improves my board
   state and pressures them further... removing it isn't urgent yet."* The Kor board swung 12 the
   next turn (19 -> 7) and the game was lost 0-12. A false persistence fact bought a violation of
   the guide's own **#1 RULE**.

**Proposed rail addition (Step 1 "read the deck" and the drafting checklist).** *For every creature
the guide names as a body, blocker, attacker or "sticky" value permanent, verify against the
primitive that it actually REMAINS on the battlefield. Cards that self-bounce, self-sacrifice,
self-exile or are conditionally returned on ETB must be described by what they DO, never by what
they cost and print -- "a {2}{B} repeatable venture that returns to your hand", not "a 5/5 that
ventures". State the non-persistence explicitly and negate the three plans it invites: do not attack
with it, do not block with it, do not count its power.* Note the conditional form is the trap: the
condition (`if you have not completed Tomb of Annihilation`) makes it look situational, when the rest
of the guide has already decided the condition permanently.

**Placement note that generalizes.** When two teaches in the same guide INTERACT to make a third
fact absolute -- here "never pick Tomb" + "Acererak bounces unless Tomb is complete" = "Acererak
always bounces" -- the guide must state the collapsed absolute, not leave the model to compose the
two. This is the same authoring lesson as the wave-26 R-ADV-FACE face (an option shows a real number
belonging to something you do not get); the shared root is **the guide describing a card's printed
identity instead of its realized behaviour in THIS deck.**

---

## Observation, NOT a proposal -- a guide imperative can license an off-menu invention

This seat's single fallback (vs122 seq18 t8, decomposed in findings.md (e)) emitted
`CHOICE: 4 (Cast Lolth, Spider Queen)` into a legal set of `{0=pass, 1=becomes beholder}`, with Lolth
neither on the menu nor affordable (5 MV against `Mana available: {b}{w} from 4 untapped sources`),
justifying itself by quoting the guide: *"The strategy guide explicitly states: 'Do not leave Lolth
or Kaya rotting in hand...'"*.

This is the second witness at this seat of the invent-an-off-menu-option shape (wave-30 vs152 s17 was
the first), and the driver has shifted from the model's own PLAN to a GUIDE IMPERATIVE. I am NOT
proposing a rail from it: the guide's text is already correctly conditioned ("The moment you can
**afford** one, CAST IT"), the core prompt already states lists are complete, and one witness of a
correctly-worded imperative being stripped of its condition does not establish that imperative
phrasing is the cause. Recording it so a third witness at another seat can be recognized as a
pattern rather than re-derived -- if it recurs, the candidate rail is "pair every CAST-IT imperative
with its own negative clause (`if it is not on the list you cannot cast it, no matter what this
guide says`)", which is cheap and testable.
