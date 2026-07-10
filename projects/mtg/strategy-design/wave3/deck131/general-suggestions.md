# General system-prompt suggestions — from the deck-131 review (wave3)

Reviewer: deck-131 agent. Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260709-220729/`,
deck 131 (blue-red spellslinger, 12+ maindeck instants) — 1 win (vs109), 1 loss (vs44),
4 timeouts (vs140, vs135, vs110, vs133). Headline stats behind everything below:

- **Opponent-turn priority windows: 4 total across 6 games — zero in 5 of the 6.**
  (All 4 were in the vs44 loss, during one spell/combat cluster on turn 10.)
- **Cast-nothing rate: 70 of 93 casting decisions (75%)** chose "Cast nothing right now"
  while at least one spell was castable.
- **Never cast across the whole corpus:** Counterspell (0), Dissipate (0), Essence
  Scatter (0), Aetherize (0), Opportunity (0), Elixir activation (0). Artificer's
  Epiphany: cast once — drawn turn 8, cast turn 16 (vs140).
- **Attackers decisions:** 1 in a 30+-turn game (vs140) — the pilot attacked once at T8,
  then never presented combat again against a creatureless opponent board. Blockers
  decisions: 0 for deck131 (3 corpus-wide).
- Worst stall: vs140, turns 10–30, life frozen 20–19, ~10 untapped lands, seven
  castable spells in hand, "Cast nothing" every turn until the 40-min timeout.

These are deck-131 numbers, but every mechanism below is deck-agnostic: the same prompt
lines act on every deck that holds an instant.

---

## S1 (HEADLINE). The instant-timing prior instructs a posture the harness cannot express

**Problem observed.** The prompt tells every deck to hold instants for opponent-turn
moments that the harness almost never offers. The pilot obeys: it holds, the window never
comes, the cards rot, the mana idles, the game stalls. Deck 131 banked 12+ instants on
this prior and got 4 opponent-turn decision windows in 6 games; its counters were never
castable-when-wanted even once. This is not deck 131's quirk — any deck's instants are
governed by the same lines, and 13/21 corpus games timed out.

**Current text at fault** (two places):
- Line 6: "hold instant-speed interaction for the opponent's threats"
- Line 21: "Instant-speed cards (instants, flash creatures, activated abilities): hold
  them for the opponent's turn or the exact moment you need them. ... if no target
  appears this turn, keep holding."

"Keep holding" is an unconditional loop with no exit — and the executor ran it for 20
straight turns.

**Proposed replacement** (for line 21's bullet, and echo in line 6):

> Instant-speed cards: you will RARELY be asked for a decision during the opponent's
> turn — do not build your game around waiting for one. Split your instants in two:
> (a) PURE REACTIONS (counterspells, and tricks that only work on an attacking or
> stack target): hold these, but never keep mana open for them at the cost of skipping
> your own plays — if the game gives you a window, it will appear in your legal actions
> as a spell on the stack or creatures attacking you; act then. (b) EVERYTHING ELSE
> (card draw, burn, bounce, buffs, anything with a useful target right now): these are
> YOUR-TURN spells. Cast them on your own main phase when they advance your plan. An
> instant held for a perfect moment that never arrives did nothing at all.

## S2. "Act at the latest USEFUL moment" — the latest moment usually never arrives

**Problem observed.** Same mechanism as S1, but this is the framing sentence that
licenses it. The pilot's PLAN lines quote this logic back constantly ("I will save my
interaction for their next attack or a more dangerous spell" — vs44 seq 15, while its
board and clock went nowhere).

**Current text at fault.** Line 20: "Act at the latest USEFUL moment - which depends on
the card type:"

**Proposed replacement:**

> Act at the EARLIEST moment that gets full value - which depends on the card type:

One word each way; it flips the executor's default from "wait" to "act" while the
card-type bullets still carry the real timing rules.

## S3. The anti-passivity line exists but LOSES to the instant-timing prior — give it rank

**Problem observed (and a partial WORKED).** Line 19 ("passing your turn with castable
cards and unused mana is a losing habit") is correct and did visible work: in the win
(vs109) the pilot deployed Prism Ring T1 and two payoff creatures on curve. But for any
hand that is mostly instants, line 19 and line 21 give opposite orders, and the executor
resolves the conflict toward holding — 75% cast-nothing. A weak executor needs the
priority between conflicting rules stated, not inferred.

**Current text at fault.** Line 19 (no change to its content) + the unresolved conflict
with line 21.

**Proposed addition** (end of line 19's bullet):

> If this rule and the instant-timing rule below point different ways - a hand full of
> instants, mana available, no reactive window in sight - CASTING beats holding. Do not
> choose "Cast nothing right now" on consecutive turns while you have a castable spell
> and unused mana.

The final sentence deliberately names the literal option text: "Cast nothing right now"
is an in-game surface the executor sees every turn, and it is its favorite exit.

## S4. Card draw has no line at all — and it is the engine of every stalled game

**Problem observed.** Nothing in the general prompt tells the pilot what draw spells are
for. Deck 131 sat on Artificer's Epiphany for 8 turns and never cast Opportunity in six
games — meanwhile its win condition (Guttersnipe) reached its hand in only 1 of 6 games.
The stalls weren't just "didn't act"; they were "never dug for the cards that end the
game." This generalizes: in any timeout, the first question is what the deck should have
been casting all those turns, and the answer is almost always its card draw.

**Current text at fault.** Absence (lines 19/21 cover acting and timing generically; line
26 covers cards-vs-tempo but not draw-spell timing).

**Proposed addition** (new bullet, or a sentence in line 19):

> Card-draw spells are FUEL, not reactions: cast them on your own turn with spare mana
> the turn you can afford them, even at sorcery-speed value, to find your threats and
> answers. Never save a draw spell for a better moment - drawing cards IS the better
> moment. In a stalled game, casting your draw spell is the play that ends the stall.

## S5. Combat needs an attack-floor keyed on what the pilot can SEE

**Problem observed.** vs140: after one attack at T8, the pilot declared no attackers for
20+ consecutive turns into a board with ZERO opposing creatures, holding two 2/1s and
later rebuilding them. Line 18's beatdown prior and line 27's lethal check assume the
pilot presents combat at all; nothing gives the trivial case as an order.

**Current text at fault.** Line 27 opens with arithmetic ("check for LETHAL...") and
conditions ("check whether enough damage gets through to matter...") — all judgment, no
floor. Line 18's "attack every turn with everything not needed to survive a lethal swing
back" is scoped to "as the aggressor," which the pilot must first self-assign.

**Proposed addition** (top of line 27's combat bullet):

> First, the free case: if the opponent has NO untapped creatures, attack with every
> creature you control - there is no blocker, the damage is free, and small hits win
> long games. Declaring no attackers turn after turn against an empty or tapped-out
> board is how games are thrown away.

This is executable from the board line alone ("Opponent battlefield: ... [tapped]") with
zero strategic inference.

## S6. "Keep enough mana untapped" needs its cost stated

**Problem observed.** Line 25 ("keep enough mana untapped to do so - not only for
interaction") and line 28 ("tapping out when you needed interaction" as a listed error)
both push mana-open; combined with S1's prior the executor treats open mana as intrinsic
value. Deck 131 ended turn after turn with 5-10 untapped lands that never bought
anything — the harness rarely lets banked mana be spent reactively.

**Current text at fault.** Line 28: "tapping out when you needed interaction or your own
key ability held up" (as a common error, with no counterweight).

**Proposed addition** (to line 19 or 25):

> Unused mana at the end of your turn is normally WASTE, not safety. Keep mana open only
> for a specific card in your hand that you expect to use before your next turn;
> otherwise spend it. You will not usually be punished for tapping out.

## S7. Small inoculation: an offered action is not a recommendation

**Problem observed (pilot handled it CORRECTLY — this is fencing, not a fix).** In the
vs44 loss the priority window offered "Cast Card Normally with Downsize" four times while
every opponent creature had shroud — the spell was offered with no valid/worthwhile
target. The pilot reasoned it out and passed correctly all four times, but that was the
model being strong in one spot; the prompt can make it cheap for weaker moments.

**Proposed addition** (one sentence, near line 30-31's action-weighing paragraph):

> The list of legal actions can include spells with no worthwhile - or even no legal -
> target; being listed is not a recommendation.

## S8. Checked and NOT at fault: the mana-pool line / affordability

For deck 131 I found no affordability misplays to attribute to the general layer: ask
decisions arrive pre-filtered to castable spells, and at its few priority windows the
pilot's mana reasoning ("I only have {U}, Counterspell needs {U}{U}") was correct
(vs44 seq 15-19). Logging this so the synthesis agent knows the "Mana in your pool:"
representation was examined against this deck and came up clean — if other deck agents
saw pool-line confusion, it's not universal.

## WORKED — do not weaken these lines

- **"be the aggressor by default" / beatdown framing (line 18):** the one win (vs109)
  was exactly this — deployed on curve, attacked every turn, raced a burn deck to 6 and
  closed at 3 life. The prior lands when the deck's hand lets it act at sorcery speed.
- **Choose by IMPACT (line 24):** both single-target Cyclonic Rift casts in the corpus
  picked the genuinely highest-impact permanent (the lone unleashed Rakdos Cackler vs109;
  Cranial Plating — the engine piece, not a creature — vs110). Targeting quality was
  consistently fine; it's timing/posture that fails.
- **"NEVER cast one into an empty board or empty stack just to spend mana" (line 21):**
  zero such misfires in six games. Keep the sentence — but note it currently reinforces
  the S1 hold-loop; it should survive INSIDE the S1 replacement's "pure reactions"
  clause, scoped to counterspells and targetless tricks only.
- **The cost-payment paragraph (line 31):** no cost-refusal misplays observed for this
  deck (nothing sacrificial in the deck to test it hard, but Prism Ring/Elixir casts were
  not refused over their costs when taken).
- **The PLAN-line mechanism itself:** the pilot's stated plans consistently referenced
  guide/prompt language — evidence the strategy text IS being read and followed. That is
  exactly why wrong priors are expensive: obedience is high.

## One structural note for the synthesis agent

The instant-timing prior (S1/S2) is the general prompt's single biggest lever on the
timeout epidemic: it applies to every deck, it is obeyed, and the harness cannot honor
it. Whatever final wording wins the merge, it must pass this test: **a pilot holding a
hand of instants, who is never offered an opponent-turn decision, must still take
game-advancing actions every turn under the revised text.** If other agents' corpora
show decks that DID get regular priority windows (e.g. decks that provoked more stack
activity), scope the reframe rather than deleting reactivity outright — but for the
observed harness, your-turn proactivity must be the stated default for every card that
has a useful your-turn mode.
