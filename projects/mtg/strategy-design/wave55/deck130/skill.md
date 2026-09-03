# deck130 -> strategy-writing skill proposals (wave 55)

Companion to `wave55/deck123/skill.md` (S1-S7 there). These are the proposals my deck130 evidence
carries on its own. Engine defects are in the seat file's ENGINE list, not here.

## S8. A MARKER THAT SELECTS A ROW ANSWERS "WHICH", NEVER "WHETHER" - AND A GUIDE MUST SAY WHICH QUESTION IT ANSWERS, BECAUSE THE PILOT WILL PROMOTE IT.
The X menu now carries two marker strings and they mean different things:
`[<- most kills at any affordable X that costs you nothing - at X=N that is THEIRS: ...; YOURS:
none]` fires when some X is free, and the new `[<- best trade: the most of THEIRS at the smallest
cost to YOURS - at X=N that is THEIRS: ...; YOURS: ...]` fires when no X is free. At `130v146`
s34/s36 the pilot reached the second one on a menu whose best row read
`THEIRS: Silverquill Silencer; YOURS: Goblin x2` - one of theirs for two of its own - took it, and
wrote a PLAN quoting the marker's own words. The deck's cast order forbids that cast (entry 1
requires the THEIRS list to name TWO OR MORE), and the decision belonged one screen earlier at the
cast row, where "Cast nothing right now" still exists.
**Proposed rule.** Whenever a guide teaches "the marked row is the answer", it must state in the
same breath what the marker is not: the marker is scoped to the menu it is on, and the menu is
already past the decision to cast. The wording that carries this is a two-clause pair - WHICH is
decided here, WHETHER was decided at the row that opened this menu - and the reviewer names the
screen where WHETHER lives. This generalises past X menus to every forced ask in the pool
(target menus, the London bottoming ask, the discard ask): a helper annotation inside a
no-pass-row screen can only rank the options it is printed on.

## S9. WHEN A RULE'S GATE IS A PROXY, THE CORPUS EVENTUALLY FINDS THE CASE WHERE THE PROXY AND THE REAL TEST DISAGREE - AND THE FIX IS THE REAL TEST, NOT A THIRD THRESHOLD.
deck130's cycling rule gated Lay Waste on a LAND COUNT ("cycle at 8 or more lands, or below THREE
with a Rain in hand; at four to seven it is castable and it is cast"). The count is a proxy for
"can I cast it this turn", and Lay Waste costs `{3}{R}`. Every one of this corpus's four wrong
cycles sat in the gap: `130v152` s18 and `130v146` s28 cycled at THREE lands, where the proxy says
"castable, do not cycle" but the mana line said 3 and the spell needs 4, so the only alternative
was passing; `130v126` s22 and s23 cycled TWICE in one turn at five lands, where the proxy says
"castable" and it was - and the guide had no sentence about doing it twice.
The rewrite (D55-4) keys the rule to `Mana available: N total`, which the engine prints on every
decision, and the two failure shapes both disappear.
**Proposed rule.** A gate written on a count that the pilot must convert (lands -> mana, creatures
-> power, cards -> castable cards) is a proxy. Where the engine prints the converted quantity
directly, the rule is keyed to the printed quantity. Where it does not, the reviewer says so and
files the render ask. The tell that a proxy is failing is a corpus of breaks clustered at the
proxy's own boundary, which is exactly the shape here (3, 3, 5, 5 against a threshold at 3 and 8).

## S10. TWO SEPARATE FLOORS ON THE SAME DECISION ARE ONE FLOOR THE PILOT WILL HALF-APPLY; MERGE THEM INTO ONE WRITTEN LINE.
The land plan carried two independent stops in two different sections: "at FIVE or more of their
lands the spell is dead" (HOW THE DECK WINS) and "at 8 life or less, stop destroying lands" (YOUR
CARDS). `130v152` s50 broke both at once - 7 life, their board on SIX lands - and the reply shows
the pilot holding exactly one of them in mind: "Land count is 6, which is >4, so land destruction
is generally low value, but this is the only playable card". It reasoned about the land floor,
overrode it, and never mentioned the life floor at all.
D55-6 merges them into a single written line - `Their lands <N>; my life <L>` - written before the
cast, in the same shape as the `Damnation: N, K, M` and `Starstorm: THEIRS ...; YOURS ...` lines
that this pool already uses and that measurably ship (deck123 wrote its three-number Damnation line
on every Damnation window this corpus).
**Proposed rule.** Where two or more thresholds gate the SAME action, the guide states them as one
written line with one blank per threshold, in one place, and the other sections point at it. A
threshold the pilot does not have to write down is a threshold it can omit silently; the
write-it-down device is the pool's most reliably-obeyed construct and it is cheap.

## S11. "THIS IS THE ONLY PLAYABLE CARD" IS A NAMED FAILURE MODE ACROSS THIS POOL, AND IT DESERVES ONE SENTENCE IN EVERY GUIDE THAT HAS A HOLD RULE.
The same sentence, in the model's own words, produced breaks at both my seats this corpus:
`130v152` s50 "this is the only playable card and it deals 2 damage to the opponent as a bonus";
`123v125` s83 "dead, but only way to dig for Bloodline Keeper against counter deck". Both menus had
`Cast nothing right now` printed on them. Wave 54 recorded the identical shape at deck123 ("cast
second Intruder Alarm (dead, but only way to proceed)") and at deck130 ("it is the only active play
I have"). The failure is not about any card; it is a standing preference for action over the pass
row, and it survives every card-specific paragraph written against it.
**Proposed rule.** Every guide in this pool carries one short, identically-worded paragraph naming
the pass row as a play and naming this sentence as the thing that overrides it. deck123's live
guide already has a version of it ("WHEN EVERY CAST ROW ON THE MENU IS PRICED DEAD, ENTRY 8 IS THE
PLAY, AND IT IS NOT INACTION") and deck123's dead-cast count moved 5 -> 2 with it in place, which
is the evidence that it works; deck130 has no equivalent and took two of its three worst casts
under exactly that reasoning. Recommend the synthesis lift deck123's paragraph verbatim into the
guides that lack one, rather than each reviewer re-inventing it.

## S12. A RUNG THAT GOES TO ZERO WITH A LARGE DENOMINATOR SHOULD BE REWRITTEN AS THE DENOMINATOR, NOT DELETED AND NOT LEFT AS A STORY.
deck123's edict rungs this corpus: **171** rows reading `they control 0 creatures - at 0 this does
nothing` and **52** reading `- they choose which one`, with **0 takes** on either. deck130's:
**10** `DRAW PUNISHERS` renders, 0 draws, fifth corpus. Those paragraphs currently carry
wave-52/53 anecdotes ("this deck answered that row's number on turn 44 of a game it was losing")
which are now three corpora stale and cost more lines than the numbers do.
Replacing the anecdote with the denominator (A55-12) is shorter, is re-derivable by the next
reviewer, and - the part that matters - tells that reviewer whether the rung is HELD or merely
UNTESTED. A deleted paragraph tells them nothing and the rung silently reopens.
**Proposed rule.** At each wave, a rung with 0 breaks over a denominator of 20+ rows has its
anecdote replaced by "N rows offered, 0 taken, Mth corpus" and keeps its imperative. A rung with 0
breaks over a denominator under ~5 is marked UNTESTED in the seat file and its guide paragraph is
left alone. The distinction is the whole point: deck123's `[second copy:` Alarm rung shows 0 takes
this corpus over **2** rows (wave 54: 20), and reading that as a closure would be wrong.

## S13. THE ATTACK LIST IS A DIFFERENT NUMBER FROM THE CREATURE COUNT, AND ANY DECK WHOSE ENGINE TOUCHES UNTAPPING NEEDS THAT SAID ONCE, EXPLICITLY.
`123v126` s53: 37 creatures on the battlefield, opponent at 20, and `Your creatures that can
attack:` printed FOUR rows. Intruder Alarm's `doesnotuntap` (mtg.txt:58850) means only creatures
made on a PREVIOUS turn are ever available, and this deck fired 21 of its 24 activations on its own
turn across 10 own-turn chain turns against 1 opponent-turn one. Every guide in this pool has an
attack rule written on a count of some kind, and deck123's Branch B literally said "A, the
creatures on your 'Your creatures that can attack:' list" - correct, and still read as "my
creatures" by a pilot holding a 37.
**Proposed rule.** Where a deck's own engine can make its creature count and its attack-list length
diverge (an untap-denial effect, mass summoning sickness, vigilance-less tappers), the guide states
the divergence once, as its own named fact, with the corpus number attached - not as a parenthesis
inside an arithmetic rule. The reviewer's trigger for looking is mechanical: any card in the sixty
whose primitive contains `doesnotuntap`, `tap` as a cost on a token maker, or a mass-untap trigger.
