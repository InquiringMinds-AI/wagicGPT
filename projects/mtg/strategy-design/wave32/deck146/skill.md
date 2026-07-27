# deck146 -> strategy-writing skill proposals (wave 32)

Three proposals. P1 is the strongest -- it is a direct, second-consecutive-wave failure of the
wave-31 HL1 gate at the very seat that produced HL1, and the failure mode is structural.

---

## P1 (PROMOTION-GRADE) -- The card-fact audit must be run on the WHOLE guide by an agent that did
## NOT write the revision, and its output is a per-claim LEDGER, not a verdict.

**Evidence.** Wave-31 established HL1: a retroactive full-guide card-fact audit gates every freeze
and exit. deck146's wave-31 revision fixed the three false facts it had found (Acererak, Hive,
MDFC) and all three are now TRUE and validated (findings (a), (h), (i)). But the wave-32 audit of
that *same revised guide* found **four more false card facts plus a self-contradiction the revision
itself created**:

- Vanishing Verse "any type ... artifact ... even a monocolored land" -- structurally impossible
  (N-146j), and it cost the vs36 game.
- Pelakka Predation "you choose a creature or planeswalker card" -- the primitive is
  `target(*[manacost>=3]|reveal)`: MV>=3, ANY type.
- Pelakka Predation "Its back face is a land if you need the drop" -- the *identical* MDFC class
  the same revision removed two paragraphs earlier, left standing on a different card.
- Silverquill Command "draw a card and drain 1" -- the mode costs YOU the life.
- Lines 99-100 vs 103-108: "MDFC cards can be played as LANDS" and "MDFC ARE NOT LANDS FOR YOU",
  both shipped, 4 lines apart. The pilot litigated it live (deck139 seq2) mid-mulligan-spiral.

**The structural point.** Wave-31 ran an audit and still shipped five defects, because the audit
was performed by the same pass that wrote the fixes, and it naturally re-read the lines it had
just written. Two blind spots follow mechanically and both fired:
(a) **The revision's own new text is unaudited** -- the writer trusts what they just verified for
one card and does not re-verify the sentence they wrote about it.
(b) **A corrected class is assumed corrected everywhere** -- the MDFC teach was fixed on Emeria's
Call and Agadeem's Awakening and missed on Pelakka Predation, three paragraphs away. This is the
wave-31 HL5 path-scoped-fix shape, appearing inside a GUIDE instead of inside engine code.

**THE RULE (three parts).**
1. **The audit is a LEDGER, not a verdict.** Produce one row per checkable claim -- claim as
   written, the primitive line that adjudicates it, TRUE/FALSE. "Audited, clean" is not an audit
   result; the ledger is. deck146's wave-31 findings recorded the audit as prose about three
   cards; nothing forced a sweep of the other twenty.
2. **Audit the guide you are SHIPPING, after you finish writing it** -- not the guide you
   inherited. Every sentence in the deliverable is in scope, including the ones written this wave.
3. **When a false-fact CLASS is fixed, grep the whole guide for the class, not the card.** Fixing
   "MDFC back faces are unofferable" on two cards and leaving it on a third is the same defect
   shipped twice. The class-sweep is one grep and it is mandatory.

**Cost:** bounded and one-time-plus-delta per guide. **Payoff:** deck146 has now lost or degraded
a game to a false guide fact in three consecutive corpora (wave-31 vs148 Acererak, wave-32 vs36
Verse), each time from a guide the loop had declared converged.

---

## P2 -- A guide that names a MULLIGAN heuristic owes a NUMERIC FLOOR, because a preference read at
## the wrong hand size becomes a filter that eats the game.

**Evidence.** deck146's guide has a mulligan paragraph with no floor. Its heuristics are sound in
isolation ("keep 2-5 lands with both colors"; "prefer a hand with an early play"). The pilot
applied the *preference* as a *requirement* at hand sizes where no requirement is affordable, and
mulliganed to ZERO CARDS in 2 of 6 games:

- deck158 seq5, at mulligan 5 (keeping 3), holding **five lands** + 2x Lolth: *"The strategy guide
  emphasizes keeping hands with 2-5 lands *and* an early play; this hand fails the 'early play'
  criteria completely."* -> Mulligan.
- deck139 seq7, at mulligan 6 (keeping 1), holding four lands + Soul Shatter + Verse: *"The
  strategy guide explicitly advises keeping 2-5 lands with early plays; this hand has too many
  lands."* -> Mulligan.

Both quote the guide as the licence for the decision. The render is faultless -- it prints
`having already taken 4 mulligans you will bottom 4 cards on a keep (keeping 3)` on every ask --
so this is a strategy failure, not a perception one, and the guide is the only channel.

**THE RULE.** A mulligan teach must contain **one absolute expressed in cards, stated before the
heuristics**: *"never keep fewer than N cards; at N or below the bar is <single cheapest testable
condition>."* Heuristics that follow must be explicitly labelled TIEBREAKERS between keepable
hands, in the same paragraph, or the executor will read the softest word ("prefer") as a gate.
Generalisation of the wave-31 HL3 shape: **a guide that states a preference owes the boundary at
which the preference stops applying** -- HL3 located that boundary at a SEAM (combat), this locates
it on a RESOURCE AXIS (hand size), and both fail the same way, by a local re-derivation that has
no floor to hit.

**Second seat, converging:** deck139's revised guide received a mulligan floor this same wave for
a 2.50-mull/game spiral. Two seats, two archetypes, one shape -- this belongs in Step 4's guide
skeleton as a required element of any mulligan section, not as a per-seat fix.

---

## P3 -- The executor reasons in stock verbal formulas; a render annotation cures the STATE but not
## the SENTENCE, so a guide teaching a seam must negate the formula by name.

**Evidence (small, but clean).** The i8 tag `[summoning sick - cannot attack this turn, but CAN
block]` renders 101 times at this seat and demonstrably reaches the model (deck105 s12: *"Corpse
Cur is summoning sick and cannot attack, but it \*can\* block"*). Yet at deck152 s47 the reply
says, in **adjacent sentences**:

> The opponent has no untapped creatures to block. Intrepid Adversary is summoning sick and
> cannot attack, but it \*can\* block.

The annotation was read and the stock phrase was emitted anyway. Same at deck105 s25. Neither cost
anything here, but the shape is the wave-31 annotation-wording finding one level up: a correct
annotation does not delete a habitual sentence, it only sits beside it.

**THE RULE (modest, for Step 5).** When a render annotation exists to kill a specific FALSE
SENTENCE the model likes to write, the guide's corresponding teach should quote the false sentence
and forbid it by name -- *"never write 'they have no blockers' when any creature on their side is
tagged CAN block"* -- rather than restating the true fact in different words. The executor pattern-
matches on phrases; a phrase is cheapest to displace with a competing phrase. Cost: one clause.

**Not proposed as a defect and not proposed for deck146's guide this wave** (zero cost observed);
recorded so a future seat with a costly instance has the precedent.

---

## NOT PROPOSED (checked and rejected)

- **A rule about answer-first protocol violations.** deck105 s23 emitted `CHOICE: 2` first,
  deliberated 106s, reversed, and closed `CHOICE: 1`; the parser resolved to the final, reasoned
  answer, which was correct. No guide or skill change is warranted -- this is parser behaviour
  working, and it is logged in notes.md for the ledger only.
- **Anything about the Verse target list.** The option annotation ("the only legal targets are
  YOUR OWN") is doing its job perfectly -- the pilot obeyed it 9/9. The defect is underneath it in
  the engine (N-146j); the guide change is a card-fact correction, not a new authoring rule.
