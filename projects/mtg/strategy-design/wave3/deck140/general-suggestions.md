# General-prompt suggestions from the deck-140 review (wave3)

Evidence base: corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260709-220729/`, deck 140
(0/6: one loss, five 40-min timeouts). Corpus-wide for this deck: 95 casting decisions,
86 "Cast nothing" (90%); offered-vs-taken: Rakdos's Return 26/0, Black Sun's Zenith 19/0,
Lightmine Field 26/0, Elixir of Immortality 55/0, Staff of Nin 1/0, Venser's Journal 15/1.
Reason-classification of the 86 cast-nothings: ~47 claimed lack of mana / must-tap-first,
~22 hold/wait/"premature", 17 other (mostly "board is clear so nothing is worth casting").
Game files cited below by short name: LOSS = `1783656459-...deck140...jsonl` (vs 109),
V131 = `1783652858-...`, V110 = `1783652860-...`, V133 = `1783657219-...`.

All suggestions are deck-agnostic and worded for the weak executor (one decision at a
time, no multi-turn reasoning). Quotes of current text are from
`bin/Res/ai/gpt/system_prompt.txt` as deployed in this corpus.

---

## 1. Add the affordability fact: "if it is offered, you can pay it" (HIGHEST IMPACT)

**Problem observed.** The single largest misplay class in the corpus (~47 of 86
cast-nothings, and the direct cause of the one outright loss): the pilot believes it has
no mana while the engine is offering it affordable casts. LOSS seq18-19, 21, 24, 27-28:
at 3 life with 7 untapped lands, "Cast Damnation {2}{b}{b}" listed as option 4, it picked
"Cast nothing" three turns in a row — reply verbatim: *"all my lands are untapped but I
have no mana in my pool yet, and I cannot tap lands to pay for a sorcery without fir[st
tapping]"*. It died with two sweepers in hand. The false model has two parts: (a) it
thinks tapping lands is a separate prior step, (b) it reads the board line "Mana in your
pool: (none)" as "cannot pay."

**Current text at fault.** Nothing in the prompt states the offer-implies-affordable
invariant. The rules primer covers costs only as notation ("Mana costs are written like
{2}{R}..."), and the closing cost paragraph ("weigh each action by what it COSTS you...
the action line states its cost in brackets") reinforces attention on cost without saying
the cost is already known to be payable.

**Proposed addition** (in the rules primer, after the mana-notation line — this is a
mechanics fact, so it belongs with the rules, not the strategy priors):

> Every choice offered to you is legal and affordable RIGHT NOW — if "Cast X" is in the
> list, you can pay for it this moment. Lands tap by themselves as part of paying; you
> never tap lands as a separate step. An empty mana pool ("Mana in your pool: (none)")
> is the normal state between actions and does NOT mean you lack mana. Never decline an
> action because you believe you cannot pay for it.

This belongs in the GENERAL prompt, not seven deck guides — the delusion is
deck-independent (it fires on any deck the moment the pilot glances at the pool line).
Cross-channel note for the representation lane: the "Mana in your pool: (none)" line is
what triggers the delusion; renaming/omitting it when empty would remove the trigger, but
the prompt sentence above is needed regardless as belt-and-suspenders.

## 2. Add one line of X-spell mechanics

**Problem observed.** The pilot does not know that X is chosen at cast time. V131
seq21-27: opponent creatureless, 8 lands in play, Rakdos's Return in hand and offered —
passed ~10 consecutive casting decisions, reply verbatim: *"Rakdos's Return deals 0
damage"* / *"requires X>0 to be effective, and I lack th[e means]"*. Offered-vs-taken
across the corpus: Rakdos's Return 26/0, Black Sun's Zenith 19/0. Two entire win
conditions dead to one missing mechanics fact. (Aggravated by a representation bug: the
{X} symbol renders as {l} in hand/option lines — "Rakdos's Return {r}{b}{l}" — so the
model cannot even see there is an X. Report for the representation channel; the prompt
line below is still needed since X-ness must be explained, not just rendered.)

**Current text at fault.** The rules primer explains mana notation but never mentions X.

**Proposed addition** (rules primer, adjacent to the mana-notation line):

> A cost containing {X} means YOU choose the number X as you cast the spell — the game
> will ask you for the value after you pick the cast. An X spell in your hand is never
> "worth 0"; it is worth whatever X you can pay. When asked to choose X, pick the
> largest value that accomplishes your purpose (usually the largest offered).

## 3. Convert the anti-durdle principle to checklist-with-closed-exception form

**Problem observed.** The principle is already present and the pilot QUOTES IT BACK while
violating it. V133 seq41: 10 lands, 21 life, opponent creatureless, "Cast Staff of Nin
{6}" offered — picked "Cast nothing"; its own PLAN says *"...card draw to further
stabilize before deploying a win condition like Staff of Nin or Blightsteel Colossus"*.
The same deferral shape ("stabilize first / keep answers open / next turn") appears in
all five timeout games; final life totals plateau (19/20, 21/16) for dozens of turns —
this is the mechanism behind the corpus's 13/21 timeouts, seen from the most
durdle-prone deck. An exhortation loses to a locally-plausible excuse every single turn.

**Current text at fault.** "once you are not about to die, DEPLOY your win conditions
and finishers - a game played to a passive, even standstill is a game you fail to win"
and "passing your turn with castable cards and unused mana is a losing habit". Both
true, both ignored — they state a disposition, not a decision rule, so at each single
decision the pilot can agree in principle and defer in practice.

**Proposed replacement** (for the "Your own turn is for ACTING" bullet — keep its first
sentences, replace the tail with a closed rule):

> On your own main phase, picking "Cast nothing" (or "Done"/"Hold") is allowed ONLY
> when one of these is true: (a) every castable card is a reaction whose target is not
> on the board right now (removal with nothing worth killing, a trick with no combat),
> or (b) you are deliberately keeping mana for a named instant or ability IN YOUR HAND
> that you expect to use this turn cycle. Otherwise cast something — start with your
> win condition or engine. "I will deploy once I am more stable" is the losing thought:
> if you are not about to die, stable is NOW, and next turn you will say the same thing
> again.

The closed exception list is the load-bearing change; the forbidden-thought sentence
("I will deploy once I am more stable") intercepts the exact recurring phrasing seen in
the logs — the executor pattern-matches its own wording.

## 4. Scope the hold-interaction prior to "if you actually hold instants"

**Problem observed.** A deck with ZERO instants repeatedly passed to keep mana open.
LOSS seq8, reply verbatim: *"Pass the turn with mana open to keep options for
instant-speed interaction (though I have none currently)"* — it names the prior AND
notices it doesn't apply, and obeys it anyway. Similar "pass with mana open / hold our
answers" reasoning throughout V133 and V131 while holding only sorceries.

**Current text at fault.** "hold instant-speed interaction for the opponent's threats"
(general principles line) and the timing bullet "Instant-speed cards ...: hold them for
the opponent's turn or the exact moment you need them."

**Proposed change.** Add a gate clause to both places (one clause, not a new bullet):

> Hold instant-speed interaction for the opponent's threats — IF your hand actually
> contains instants, flash cards, or usable activated abilities. If it holds none,
> there is nothing to keep mana open for: spend your mana on your own main phase.

## 5. Add a land-drop floor and a mulligan rule (both currently absent)

**Problem observed.** (a) The pilot declined its land drop to "keep hand size": LOSS
seq20 chose "Hold Isolated Chapel - do not play it now", reply: *"I need to keep my hand
size high to maximize the life gain from Elixir of Immortality"* (a confabulated reason
— wrong card). (b) It mulliganed a keepable hand for a verbal reason: V110 seq0 mulled a
3-land hand containing two cheap sweepers (*"This hand has three Pyroclasms and no
immediate way to play a land"* — false, it had three lands), kept the resulting 1-land
seven, and then made essentially no decisions for ten turns (mana screw all game; only 6
total decisions in a 40-minute timeout).

**Current text at fault.** "play a land and advance your plan every turn" exists but
nothing forbids holding a land, and the prompt contains NO mulligan guidance at all.

**Proposed additions** (deck-agnostic, one line each):

> Play a land every turn you can - declining your land drop is almost never right, and
> "keeping it in hand" gains nothing.
> Mulligan rule: keep any opening hand with 2, 3, or 4 lands; mulligan hands with 0-1
> lands or 6-7 lands. Judge by land count, not by which spells you would prefer.

## 6. Add a stale-plan guard

**Problem observed.** The carried "YOUR PLAN (as you last stated it)" line re-injects a
false belief every turn. LOSS: the plan *"I have 5 tapped lands and need to untap them
next turn"* rode along for multiple decisions while the CURRENT SITUATION it was printed
next to showed every land untapped — and the pilot kept trusting the plan over the board
(seq18-28). The plan channel turns one wrong inference into a persistent one.

**Current text at fault.** "you will receive ... your own last stated plan" — presented
with no precedence rule against the board snapshot.

**Proposed addition** (append to that sentence):

> Your last plan is history, not truth: it may be out of date or simply wrong. When
> your plan disagrees with the current board state, TRUST THE BOARD and drop the plan.

## 7. Lines that showed no misfire from this seat (do not weaken)

- **The mulligan keep it did make correctly** (LOSS seq0: kept 3 lands + two Pyroclasm
  with sound reasoning) shows the impact-based-choice prior functioning when no false
  belief interferes — suggestion 5's mulligan rule should ADD the land-count anchor, not
  replace impact reasoning.
- **"Cast sorceries on your main phase" / act-on-your-turn framing** produced correct
  behavior whenever the mana delusion was absent (the few casts that did happen — e.g.
  LOSS seq14 Pyroclasm as a clean 2-for-1 — were correctly timed main-phase plays). The
  timing bullets themselves are fine; only the hold-interaction clause (suggestion 4)
  needs the gate.
- No evidence from deck 140 on the beatdown/blocking/lethal-math priors (the deck never
  had creatures; zero attackers/blockers decisions in six games) — no opinion either way
  from this seat.

## 8. Cross-channel notes for the merge (representation, not prompt text — but adjacent)

- `{X}` renders as `{l}` in hand and option lines ("Rakdos's Return {r}{b}{l}"). Fed the
  X-spell blindness (suggestion 2). Fix in representation; suggestion 2 still needed.
- `Mana in your pool: (none)` is the trigger for the no-mana delusion (suggestion 1).
  Consider omitting the line when empty or rewording to "Mana in your pool: (none —
  lands pay automatically when you cast)". Suggestion 1 still needed either way.
