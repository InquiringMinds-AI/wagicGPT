# deck152 -> strategy-writing skill: proposals (wave 31)

Seat mode this wave: **Step 0-ter** (first GUIDED corpus for the wave-30 initial guide).
All four proposals come out of that validation. Each is tagged with its seat count and, where
it is single-seat, with the already-adopted rung it is the mirror of -- I am not asking for new
doctrine on one witness, I am asking to close the other half of a rule the skill already holds.

---

## P1 (doctrine, single-seat but a MIRROR of an adopted rung) -- an initial guide must never encode a FREQUENCY or RARITY claim about a mechanic the baseline corpus did not EXERCISE

**Proposed addition to Step 0-bis (writing a NEW guide from scratch), and cross-referenced from
Step 0-ter rule 5.**

> When the guideless corpus contains ZERO instances of a mechanic, you have learned nothing
> about how often it happens -- you have learned that your six games did not sample it. Write
> the mechanic's RULE and both of its outcomes. Never convert non-observation into an authored
> rarity claim ("this rarely happens", "you will usually not see", "treat X as its default
> state"), because a rarity claim is a licence for the pilot to stop planning, and it is the one
> class of guide error that gets WORSE the better the guide otherwise is: the pilot obeys it.
> Where the frequency genuinely matters to the play, say what the TRIGGER is and let the board
> answer ("the battlefield line shows Day or Night -- read it").

**Worked example (this seat).** The wave-30 guide, authored against a corpus in which no
werewolf ever flipped, wrote:

> "They flip to their stronger NIGHT side only if a whole turn passes with NO spells cast, and
> flip back if someone casts two spells in a turn - **this rarely happens, so treat them as
> their day-side bodies** (Cathar 2/2 with the exile, Huntmaster a 6/6 that makes two Wolves)."

Wave-31 flipped werewolves in **4 of 6 games** (deck146 seq23 t15, deck137 seq27 t13, deck139
seq16 t9, deck148 seq45 t19). The rule half of the teach was right; the frequency half was
manufactured from an unexercised sample, and it told the pilot not to plan around a 7/7 that
makes two Wolves every time it attacks. Note the trap's shape: the wave-30 reviewer CORRECTLY
recorded "the flip was NOT exercised this corpus / remains UNVALIDATED at this seat" in
findings and notes -- and the guide written in the same pass still asserted rarity. The
honest-uncertainty note and the guide sentence were authored by the same reviewer in the same
hour and contradicted each other. That is why this needs to be a written rung and not a
reminder to be careful.

**Why this is not new doctrine.** Step 0-ter rule 5 already holds the TRIM half: *"Do not trim
a class merely because it did not recur this corpus if its precondition never engaged."* The
skill therefore already knows non-recurrence is not evidence. P1 is the AUTHORING half of the
identical principle, and the skill currently leaves it open.

---

## P2 (method, single-seat, mirror of the wave-18 "apportion the concurrent engine fix" rung) -- GUIDE/ANNOTATION RECONCILIATION: when the wave's engine batch changes an option annotation, grep the DEPLOYED guide for prose about that surface before revising anything else

**Proposed addition to Step 0-ter, immediately after the wave-18 apportionment rung.**

> The apportionment rung asks "would the engine change alone have killed this class?" -- a
> question about CREDIT. Ask the adversarial twin first, because it is about CORRECTNESS: **did
> the engine change make any sentence of the deployed guide FALSE?** An h-batch that rewrites
> an annotation ships new authoritative text into the same option line the guide talks about,
> and guide prose that contradicts a rendered annotation loses (the perception-vs-strategy
> doctrine) -- so the stale sentence does not merely go unused, it burns the guide's
> credibility at the exact seam the engine just fixed. Procedure: from the reviewer brief's
> h-item list, extract the surfaces touched; `grep` the deployed `deckN_strategy.txt` for each
> surface's vocabulary; reconcile or delete every hit BEFORE you spend revision budget on new
> teaches.

**Worked example (this seat).** h4 shipped, verbatim in the option line:
`in this engine only the currently-shown face can actually be played as a land`. The deployed
guide said:

> "You can also flip a Pathway to its blue face before playing it."

Directly contradicted by the annotation, and independently false (N-152a: back-face land
placement is structurally blocked). One grep for `Pathway` / `flip` would have caught it. The
same sweep also retired an obsoleted teach for free -- the guide's #1 rule carried
*"If your only listed action is 'Flip Side ...' the correct answer is 0 (pass)"*, which h5's
auto-pass makes unreachable (0 such windows this corpus, down from 121).

---

## P3 (review INSTRUMENT, measurement-only, adoptable single-seat) -- the CAST-NOTHING RATE, SPLIT BY WINDOW COMPOSITION

**Proposed addition to the Step 0-ter instrument set, sibling to the 0-attacker durdle
instrument and the burn-target face-rate.**

> For any deck whose guide carries a deploy floor, tabulate every `Casting decision` ask and
> split the `Cast nothing right now` answers into two buckets: **(a) windows where the ONLY
> offer was instant-speed removal** (a hold is legitimate) and **(b) windows where at least one
> CREATURE was offered** (a decline against the floor). Report the (b) count with life totals
> and own-creature count. The raw rate is uninformative; the split is the finding. Biases no
> guide content -- it only makes an existing signal countable.

**Worked example (this seat).** Raw: `Cast nothing` 3/39 (wave-30) -> 7/49 (wave-31) reads as a
regression. Split, wave-31 is **5 legitimate instant-holds** (deck139 seq20/27/29, deck158
seq11/13 -- Fateful Absence was the only offer, exactly as the guide teaches) and **2 fatal
declines**, both in the one loss with a pilot component: deck146 seq21 t13 (life 3, three casts
offered) and deck146 seq26 t15 (**life 1**, four casts offered incl. Katilda / Brutal Cathar /
Elite Spellbinder). Without the split the guide revision would have been aimed at removal
discipline, which is working; with it, the revision is a deploy floor, which is what lost the
game. The bucket-(b) replies also hand you the mechanism verbatim -- here a locally-plausible
chump-block fear at 1 life: *"If I cast a creature now ... I will likely need to block next
turn. If I block with my new creature against the Adventurer, I die to deathtouch."*

---

## P4 (review DISCIPLINE, 2 seats: deck146 wave-30 probe + deck152 wave-31) -- extend the HARNESS-BLOCKED-COMPLIANCE rung with its inverse: audit for CASTABLE-BUT-NOT-OFFERED

**Proposed addition to the wave-26 method-headline-6 rung (harness-blocked compliance).**

> The rung currently checks that a wrong-LOOKING decline was actually OFFERED cleanly -- it
> catches the case where the option was present but the model's answer or the target preview
> was mangled. Add the inverse audit, which the current rung cannot see: **in every LOSS, take
> the deck's two or three signature cards, and for each `Casting decision` ask in which one of
> them is in `Your hand`, check whether it appears in the offer list; when it does not, verify
> payability by hand from the untapped-source list and the primitive's `mana=`.** An option that
> is silently ABSENT leaves no fallback, no defer, and no trace in any counter -- it is the
> arrival-tracing doctrine applied to the OFFER side. Positive controls are free: find a window
> where the same card WAS offered and diff the two boards; the difference is the mechanism.

**Worked example (this seat).** Sigarda, Champion of Light (`mana={1}{G}{W}{W}`) sat in hand,
payable, and unoffered at deck146 seq21 t13 and seq26 t15 -- the second at 1 life, in a game the
seat lost, with the model's own PLAN naming her for three consecutive decisions. Zero fallbacks
were logged; the seat's fallback rate for the whole corpus is 0/243. The positive control
(deck137 seq28 t13, Sigarda OFFERED with only FOUR untapped sources) isolated the mechanism in
one diff: white from **Deserted Beach** (whose first `auto=` is `{T}:add{W}`) vs white from
**Overgrown Farmland** (whose first `auto=` is `{T}:add{G}`). That pointed straight at
`ManaEngine::potentialMana`'s `used[card] = true` after a card's first mana ability -- a defect
the codebase already documents and already fixed on the `hasInstantResponse` path only
(`LegalActions.cpp:296-301`) while the cast-offer path stayed strict. Full write-up in notes.md
(N-152c). Second seat: deck146's wave-30 reviewer ran a hands-vs-asks probe of the same shape
(recorded as "no N-146a offer-gap found") -- the probe exists in practice at two seats but is
not written into the skill, so it fires only when a reviewer happens to think of it.

---

## Explicitly NOT proposed

- **Any core-prompt change.** See general-suggestions.md (PASS).
- **A guide line about the `(printed X/Y)` face bug or the Sigarda offer gap as ENGINE facts.**
  Both are representation/engine defects -> notes.md. The guide gets only what a pilot can
  ACT on: read the current P/T not the printed tail, and cast the best thing that IS offered
  rather than stalling for an absent one. Prose asserting engine internals would be the exact
  layer error the skill forbids.
- **A blocking-protocol change** off deck148 seq17's one-line `B2:A1, B2:A2` violation. The
  prompt already forbids it three lines above, the first-wins parser absorbed it, and the
  applied block matched the model's own corrected reasoning. Single benign occurrence.
