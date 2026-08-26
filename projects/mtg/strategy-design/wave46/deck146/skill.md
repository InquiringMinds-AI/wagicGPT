# deck146 -> strategy-writing skill proposals (wave 46)

Only proposals my own corpus evidence supports. Each names the observation, the proposed
amendment text, and how a later wave falsifies it.

---

## S1 (STRONG) — A FIRST-MATCH LIST MUST BE TOTAL OVER THE DECK'S CASTABLE CARDS

**Observation.** deck146's `THE WHOLE CAST ORDER, ONE LIST` is a seven-entry first-match list with
an explicit "Stop there - do not read the rest of the list". Over 73 own-turn cast asks it holds
46 times, and **9 of the 27 breaks are cards the list does not contain**: `Cast Silverquill
Silencer` taken 8 times, `Cast Emeria's Call` once. Most of those casts were good plays. When the
executor must go outside a list that told it not to, the instruction that gets abandoned is the
first-match discipline itself, not just that one entry.

**Proposed amendment.** *An ordered first-match list is a TOTAL function or it is not a list.
Before shipping one, enumerate every card in the deck that can appear as a "Cast ..." option and
confirm each is matched by some entry; a card with no entry is a defect in the LIST, not in the
card's own rule, and it teaches the executor to stop reading the list at all. Close the set with a
final catch-all entry that names the fallback explicitly, and state that "cast nothing" is the
LAST entry rather than a tiebreaker available at any depth.*

**Falsifier.** A deck whose list is total and whose first-match compliance does not improve, or a
corpus in which a deliberately partial list outperforms a total one.

---

## S2 (STRONG) — AN ABSOLUTE THAT CAN FORBID THE WINNING MOVE NEEDS A LETHAL CARVE-OUT ABOVE IT,
## NOT INSIDE IT

**Observation.** deck146's Hive CHECK 1 ("If it says Upkeep, the answer is PASS ... There is no
exception to this") is a good rule: it took the Hive's wasted activations from 27-of-78 down to
1-of-46. It also cost a game outright — `priority` seq 50/51, opponent at **1 life**, the animated
Hive is exactly lethal, and the absolute said PASS. The guide's other absolutes that survive
scrutiny (the per-creature attack STOP, the collapsed-tag stop) all already carry the same
carve-out in the same words: "unless the lethal check clears".

**Proposed amendment.** *When you write an absolute ("no exception to this"), ask one question:
can a board exist on which obeying it declines a win this turn? If yes, the carve-out is a
numbered check ABOVE the absolute, not a clause inside it — a reader who stops at the first
matching check must reach the carve-out first. Use the guide's existing lethal-check wording so
the deck ships one lethal test, not two.*

**Falsifier.** A corpus in which a lethal carve-out above an absolute is over-fired — the executor
claiming lethal on boards that are not lethal — at a rate that costs more than the wins it takes.

---

## S3 (MEDIUM) — A "USE JUDGEMENT" QUALIFIER ON A SELF-DAMAGING ABILITY MUST BE REPLACED BY A
## PRINTED-LIFE FLOOR

**Observation.** "0 draws you a card and loses 1 life (take it **most turns you are not desperate
for that life**; do not just pass)" produced five activations at 16, 12, 9, **5** and **2** life,
and the seat died at 0. "Desperate" is not on the screen; the printed life total is.

**Proposed amendment.** *Any ability whose cost is paid out of a resource the prompt prints
(life, loyalty, cards in library) gets a numeric floor read off that printed number, never a mood
word. "Take it when you are not desperate", "use it when you can afford it" and "spend it freely
early" are all unobservable; "at 6 or less, do not" is checkable in the render and falsifiable in
the next corpus.*

**Falsifier.** A corpus in which the floor causes card starvation losses at 6-8 life that the
unfloored version would have avoided.

---

## S4 (MEDIUM) — WHEN A FALSE INFERENCE IS REFUTED, REFUTE ITS SIBLINGS BY SHAPE

**Observation.** deck146's STOP already refuses "outnumbering them protects my card". The wave-46
break used the same falsehood in a new shape: *"Triumphant Adventurer blocks Fate Unraveler
(trades)"* — a DIFFERENT ATTACKER of its own would absorb the blocker. Both sentences reduce to
the same error (assuming control over which attacker the defender blocks), and the guide had
refuted only one instance of it.

**Proposed amendment.** *When you write a refutation, name the PRINCIPLE the false sentence
violates, then list the instances you have seen. "THEY choose which attacker their blocker stops"
generalises; "sending three when they have one blocker does not protect mine" does not. Expect the
next corpus to produce a fresh instance of a principle you refuted only by instance.*

**Falsifier.** A corpus in which principle-form refutations are followed less reliably than
instance-form ones on the same rule.

---

## S5 (WEAK, one window) — A RULES-BELIEF ERROR NEEDS THE BELIEF NAMED, NOT JUST THE ACT BANNED

**Observation.** deck146's Verse rule bans exiling your own permanent; it went 22/23. The one break
states its belief out loud: *"Exile Nadaar with Vanishing Verse to cycle it back to hand and
venture the dungeon again"*. The ban was read and reasoned around because the executor thought
exile was a bounce.

**Proposed amendment.** *A ban that keeps being reasoned around is usually sitting on top of a
false rules belief. Write the belief in the executor's own words and refute it ("exile is not a
bounce - the card is gone"), rather than adding force to the ban.*

**Falsifier.** One window is not a rate; re-check in wave 47.
