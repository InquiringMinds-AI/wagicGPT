# deck152 -> development notes (wave-33)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260819-070739`, binary `bdb30d8e7`.
All items engine/render/harness lane. Seq citations are deck152-seat unless labelled otherwise.
Every delta below is attributed against the INTERIM CHANGES list (`948578887..bdb30d8e7`).

---

## CLOSED THIS CORPUS (recorded so the ledger can drop them)

### N-152f -- **CLOSED.** `deferFlexibleSource` per-ability fixed; arrival-traced on the broken branch.

The wave-32 hypothesis (SELF test source-scoped, OTHER test ability-scoped -> mutual deferral on
an all-flexible manabase) is confirmed by the fix's live behaviour. Two arrivals, not offers:

- **deck36 s26 t12** -- `Barkchannel Pathway {G}` + `Deserted Beach (W/U)` + `Lair of the Hydra
  {G}` + `Overgrown Farmland (G/W)`, `Mana available: 4 total`, Teferi `{2}{U}{W}` = exactly 4.
  Every white source flexible; the only blue source is also a white source. Offered, cast,
  `stack -> battlefield` at s27, `+1` activated. **This is the exact fixture spec wave-32 wrote
  (`Scrubland, Scrubland, Darksteel Citadel` + a `{1}{W}{B}` spell), realised on live data.**
- **deck116 s29 t12** -- three flexible white sources (`Deserted Beach`, `Overgrown Farmland`,
  and `Katilda, Dawnhart Prime` tapping under her own lord ability), mono-green Barkchannels,
  Sigarda `{1}{G}{W}{W}` offered, cast, `stack -> battlefield` at s30.

**Negative-control ladder in the SAME game** (deck116 s12/s19/s24): with the Deserted Beach as the
board's only white AND only blue source, Teferi is correctly NOT offered at three consecutive
windows, and Sigarda correctly not offered at s24; the offer appears at s29, the window
`- Your Overgrown Farmland: hand -> battlefield` fires. One-source delta, isolated.

**Full false-suppression sweep at this seat: 0 / 44 casting windows.** N-152f needs no further
corpus. Recommend the two fixtures from wave-32's spec be registered as regression tests if they
are not already, and that the POSITIVE one be confirmed green on `bdb30d8e7` (HL10's
binary-order rung: fixture and corpus must be checked on the same binary before an item retires).

### N-152d layer 2 -- **CLOSED.** `(printed X/Y)` no longer false on the board snapshot.

`AAFlip::testDestroy` resetting `isFlipped` post-transform + `cardShowsOtherFace()` works.
Counted over all 12 logs of my six games: `Moonrage Brute (3/3) (printed 2/2)` **22 -> 0**;
`Tovolar's Packleader (7/7) (printed 6/6)` 0 -> 0 with Packleader now rendering **24/24 clean**
(wave-32's zero was coverage, this one is proof -- Packleader reached the board-snapshot path 19
times at my seat and 5 at deck139's). Board-snapshot renders of a transformed werewolf: **7/7
clean**; enumerator renders **2/2 clean**.

Positive controls confirm the annotator still fires at true deltas -- 51 distinct tagged renders,
including `Tovolar's Huntmaster {4}{g}{g} (7/7) (printed 6/6)` x6 under an Intrepid Adversary valor
anthem (Huntmaster is `subtype=Human Werewolf`; every other creature moved with it).

---

## N-152k -- **MEDIUM, NEW. `{0}` is printed for a cost that is ABSENT, not zero.** Caused by i5.

**Attribution.** This is i5 / N-36b, the six falsy-zero print guards routed through
`manaCostToken()` -- NOT the transform work. Verified against the wave-32 corpus
(`matchups-20260727-115847`): `Moonrage Brute {0}`, `Tovolar's Packleader {0}`, `Day {0}`,
`Night {0}` all have **zero** occurrences there; the battlefield line rendered them bare
(`; Day`, `; Night` x81 in wave-32). All four appear in wave-33.

**Counts, my six games (both seats):**

| render | count | truth |
|---|---|---|
| `Mox Jet {0}` / `Mox Sapphire {0}` | 222 | CORRECT -- the intended fix |
| `Day {0}` / `Night {0}` | 142 | FALSE -- a designation marker is not a card |
| `Moonrage Brute {0}` | 11 | FALSE -- a transformed back face has no mana cost |
| `Tovolar's Packleader {0}` | 13 | FALSE -- same |

Verbatim, deck152 s27:
```
Your battlefield (creatures: 1): ...; Moonrage Brute {0} (3/3) [first strike, nightbound]; Deserted Beach; Night {0}
```
and deck139 s21 (opponent view of my board):
```
...; Tovolar's Packleader {0} (7/7) [nightbound]; Wolf #3 (2/2); Wolf #4 (2/2); Night {0}; ...
```

**Why it matters, not just why it is wrong.** deck36's wave-32 guide was given the falsity-fact
*"a card with no cost printed on its line costs {0}"* (HL9.4's authoring consequence). The i5 fix
makes that rule TRUE for Moxen and FALSE for every transformed back face and every day/night
marker, in the same prompt -- and `{0}` reads as "castable for free", which directly contradicts
the Flip Side annotation's own closing promise (*"this card is NEVER cast as its other face"*)
that sits three lines away.

**Cost so far: ZERO.** No reply at this seat reasons about a `{0}` back face; the Flip Side
prohibition held 33/33. This is a latent false fact, reported before it bites, not a measured loss.

**Spec.** Distinguish "cost is zero" from "cost does not exist" at the emitter, not at the guard.
`manaCostToken()` should return the empty string when the card has no mana cost object at all
(transformed back faces, designation markers, tokens) and `{0}` only when a cost object exists
whose converted cost is 0. **Discriminator, not a diagnosis:** the two populations are already
separable in the corpus -- Moxen have `mana={0}` in their primitive, Moonrage Brute / Tovolar's
Packleader / Day / Night have **no `mana=` line at all** in `borderline.txt`. Whatever the code
path, that is the test.

**Fixture spec (HL5.4 -- ride the broken branch, not the sibling that passes):**
- POSITIVE, currently failing: a board containing a transformed `Moonrage Brute` and a `Night`
  designation; PARSETEST pins that neither line contains `{0}`.
- NEGATIVE GUARD, must stay green: a `Mox Jet` in hand and on the battlefield; both lines must
  still contain `{0}` (this is the i5 fix and must not regress).
The existing i5 fixture, if any, rides the Mox branch -- the branch that works.

---

## N-152l -- **MEDIUM, HARNESS/RENDER. The blocker eligibility list is per-blocker and per-attacker, and the seat's one fallback is a read of it.**

Not a defect claim about the render -- the render is correct and complete. Recorded because it is
the trigger shape the new `all_assignments_illegal` class needs in the ledger.

**deck36 s8 t5**, `kind=blockers`, `fallback: all_assignments_illegal`,
`pruned_pairs: "Intrepid Adversary -> Glaze Fiend"`, `choice_source: None`, `chosen_text: None`.

```
A1. Silver Myr (1/1) deals 1
A2. Glaze Fiend (2/3) deals 2 [flying]
Your available blockers (with, for each attacker it may block, the naive 1-on-1 trade - before other blockers, pump or combat tricks):
B1. Intrepid Adversary (3/1) [lifelink] - may block A1 (both die)
```
Reply line 1: `BLOCKS: B1:A2`. B1 is a ground creature, A2 flies; B1's line enumerates `A1` only.

**The class works as designed** -- the pruned pair is named, the exit is a labelled fallback rather
than the silent `unparsed_reply` this would have been on `948578887` (this closes N-139s at one
witness). Two observations for the harness lane:

1. The `- may block A1 (both die)` clause is the ONLY place the legality is stated, and it is a
   trailing qualifier on a line whose head is the blocker's own stat block. The seat read the head
   and the attackers' stat blocks and never reached the qualifier. A front-loaded form
   (`B1. [may block: A1] Intrepid Adversary (3/1) [lifelink] - A1: both die`) would put the
   constraint where the parser-facing content already is. **Recorded, not proposed** -- one witness,
   and the guide-side clause is cheaper to try first (it is in this wave's revision).
2. `pruned_pairs` proved its worth immediately: without it the record would say only "the whole set
   was illegal" and the diagnosis would have needed the reply text. Keep the field.

---

## N-152m -- **LOW/WATCH, INSTRUMENT. Two cap adjudications at one seat, both latency-driven, neither decision-starved.**

Recorded for the synthesis cap/`-j` decision, with the numbers rather than an opinion.

Latency at this seat, 211 decisions: **median 17.0s, p90 82s, p95 130s, max 194s; 27 decisions
over 60s and 18 over 100s.** Reply lengths are FLAT vs wave-32 (p50 1049 vs 1123, p95 7774 vs
7562, max 15082 vs 14458), so this is not a prose spiral at this seat -- it is the per-decision
cost at `-j 3`.

- **vs36 (WON on adjudication, 12-6 at t14)**: 38 decisions in 3000s = 79s/decision.
- **vs116 (LOST on adjudication, 15-21 at t15)**: 45 decisions in 3000s = 67s/decision, and the
  game's two largest replies are s39 (`reveal`, 15,082 chars, 187s) and s38 (`priority`, 11,573
  chars, 129s). Board alive at the cap: Sigarda attacking, Teferi at loyalty, 3 creatures.

Both games were competitive and unfinished. The correct read is that the cap is biting on wall
time, not on a model that will not commit -- **`commit_retracted` is 0/211 at this seat and no
reply abandoned a committed answer.** If the synthesis weighs 3000->3600 or `-j 3` -> `-j 2`,
these two games are the evidence for it; the protocol change is orthogonal here.

---

## CARRIED, cited, not re-diagnosed

- **N-152g (legend suppression, `LegalActions.cpp:66`/`:256`, fix staged wave-34)**: **0
  suppressed windows witnessed.** The trigger predicate (a legend in hand while a same-name legend
  is on your own battlefield) came within one decision of firing -- deck36 held two Teferi and
  resolved one at s26 -- but s26 was the game's last casting window before the cap. Ship the fix on
  the code reading; this corpus neither confirms nor falsifies.
- **N-152a (MDFC back-face land placement)**: still unofferable. Unchanged.
- **N-152h (MDFC-spell Flip Side wording, third path)**: **UNEXERCISED here.** The string
  `the other face appears there as an alternative-cost cast` has 0 hits in my twelve logs; deck152
  draws no MDFC spells. Must be cited from a seat that does.
- **N-152j (name-form `BLOCKS:` lines)**: **the class is dead in my games.** Zero `unparsed_reply`
  across all twelve logs (wave-32: both corpus instances were in deck152's games, at the opponent
  seats). i8's three legs held.
- **N-158k**: exercised against me and CORRECT on the cast line (deck158 s21:
  `Cast Feed the Swarm {1}{b} - legal targets right now: Moonrage Brute (costs you 0 life)`). I did
  not observe the COMMIT-seat target menu; no new occurrence from me.
- **N-158e (no flash windows on Baka's own turn)**: unchanged, not counted against any guide.

---

## Low-severity observations (no action requested)

- **The wave-32 transform-DFC hand-display oscillation did not recur.** Zero model-issued
  `Flip Side` actions this corpus (33 offers, 0 taken), and no snapshot showed a hand card
  alternating faces. Consistent with the oscillation having been driven by the model's own toggles.
- **The pilot now obeys the completeness guarantee on a CORRECT suppression without spiralling.**
  deck116 s20 verbatim: *"the prompt explicitly states: 'If a spell is a numbered "Cast..." option
  it IS payable... If a spell is not in the cast list you CANNOT cast it right now'. Since Teferi
  is not listed in the 'Your legal actions' section, you cannot cast him this turn."* 1,300
  characters, no contradiction claim, correct outcome. Compare the wave-32 kilobyte spirals at the
  same seam. The core prompt's completeness sentence is doing exactly its job now that the offer
  side is honest.
- **Katilda counted as a mana source, verified end-to-end**: deck116 s29 renders
  `Mana available: 8 total` over 7 lands + Katilda, and the Sigarda cast that window needed her.
  First live witness of `lord(creature[human&<colour>]|myBattlefield) {T}:add{<colour>}` reaching
  the mana line.
- **Plan-field length**: 34 of 211 prompts (16.1%) carry a `YOUR PLAN` block over 400 chars, none
  over ~800 (deck36's wave-32 seat was 39% with several 600-800). Not a spiral driver at this seat.
