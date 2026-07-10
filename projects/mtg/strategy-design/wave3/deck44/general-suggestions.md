# General-strategy suggestions from the deck-44 review (UB Faerie tempo, 1-2-3 in corpus)

Evidence base: all six deck44 games in `matchups-20260709-220729` (win vs131; losses vs109,
vs135; timeouts vs140, vs110, vs133), read decision-by-decision from the translogs. Citations
are `<opponent> s<seq>` into `*-ai_baka_deck44-*.jsonl`. All suggestions are deck-agnostic;
proposed wording is written for the weak one-decision-at-a-time executor.

---

## S1. The "hold instant-speed cards" prior has NO release valve — it is the root failure of this corpus

**Problem observed.** The hold prior beat every deploy/anti-passivity prior at the moment of
choice, in three distinct shapes:

- *Flash creatures held forever, never flashed.* vs131 (the WIN, despite this): hand of
  2x Scion of Oona + Obyra (all flash) from turn 1; "Cast nothing right now" chosen in both
  mains of T5 and main 1 of T7 with mana available (s4-s6); the creatures were also never
  cast on the opponent's turn — first creature hit play T7 main 2. Pure lost turns.
- *Answers hoarded to death.* vs109 (loss): held Go for the Throat + Counterspell from T3
  to death at 4 life while Stromkirk Noble grew 1/1→4/4 (s2-s19). vs135 (loss): let the
  game-ending 13/13 Abominable Treefolk resolve and kill it while holding Counterspell +
  Arcane Denial the whole game (s15-s23).
- *Threats not cast to "keep counter mana up."* vs140 (timeout, ahead 20-9): "Cast Archmage
  of Echoes" offered at s53/s58 with an EMPTY opponent battlefield; chose "Cast nothing" both
  times; its stated PLANs repeatedly cite holding counters. Archmage (a 4-of win condition)
  was cast ZERO times across all six games.

**Current text at fault** (STRATEGY prior, timing bullet):
> "Instant-speed cards (instants, flash creatures, activated abilities): hold them for the
> opponent's turn or the exact moment you need them. [...] if no target appears this turn,
> keep holding."

"Keep holding" is unbounded, and nothing anywhere says when holding ENDS. The weak executor
takes the unbounded branch every time.

**Proposed replacement** (same bullet, rewritten with release valves):
> - Instant-speed cards (instants, activated abilities): hold them for the opponent's turn or
>   the exact moment you need them — but holding has a LIMIT. Spend a held answer NOW when any
>   of these is true: the best target you have been waiting for is already on the battlefield
>   or stack; the threat you could answer is growing each turn; or your life is 10 or less.
>   Dying or falling hopelessly behind with answers unspent in hand is a certain loss — worse
>   than spending them slightly early.
> - Flash creatures are CREATURES first: cast them in your main phase like any other creature.
>   Hold one past your main phase only when you will actually use the surprise THIS turn cycle
>   (an expected attacker to ambush, a spell you expect on the stack). Never hold a flash
>   creature across multiple turns.
> - Holding mana open is only a play if you can NAME what you are holding it for. "Keeping
>   options open" with no named target is passing the turn, and passing is losing.

## S2. The anti-passivity prior is a posture; at the decision point it needs to be a procedure over the option list

**Problem observed.** The general prompt already says the right thing abstractly, and the
executor still chose "Cast nothing right now" with castable, plan-advancing creatures over and
over: vs140 s24/s29/s39/s44/s53/s58/s64 (12+ cast-nothing picks in one game, opponent board
EMPTY for most of them); vs131 s4-s6; three of six games hit the 40-minute wall in a life
plateau — a failure mode `results.tsv` records as "timeout" but is really "never closed."

**Current text** (correct, but demonstrably not strong enough at the moment of choice):
> "Your own turn is for ACTING. [...] passing your turn with castable cards and unused mana is
> a losing habit."
> "once you are not about to die, DEPLOY your win conditions and finishers - a game played to a
> passive, even standstill is a game you fail to win."

**Proposed addition** (keep the existing lines; add a decision-surface rule right after the
"Your own turn is for ACTING" bullet):
> When the choice list offers casting a creature or other permanent that advances your plan,
> "Cast nothing right now" is the WRONG pick unless you can name the specific better use for
> that mana this turn cycle. Choosing "Cast nothing" on consecutive turns while holding
> castable spells means you are losing the game slowly — deploy.

Rationale for the synthesis agent: postures compete with (and lose to) the hold prior of S1;
a rule keyed to the visible option text ("Cast nothing right now") fires exactly where the
mistake happens.

## S3. State that the option list is pre-vetted: offered = legal = affordable (and teach hybrid mana)

**Problem observed.** vs133 s6-s12: the engine OFFERED "Cast Oona's Gatewarden {u/b}" off four
Islands for four consecutive turns; the model's PLAN said "I cannot cast my creatures
(Bladecrafter or Gatewarden) without a Swamp" and it picked "Cast nothing" every time. It
second-guessed the legality of an engine-offered action and durdled turns 5-8 of a game that
then timed out. The engine only ever lists legal, payable actions — the model doesn't know that.

**Current text at fault:** nothing states it. The mana primer covers only
> "Mana costs are written like {2}{R} (two generic plus one red). W=white U=blue B=black R=red G=green."

**Proposed additions:**
1. To the mana primer line, append:
> Hybrid symbols like {U/B} can be paid with EITHER of the two colors.
2. To the closing "During the game you will receive..." paragraph (or the rules primer), add:
> Every listed choice is legal and affordable RIGHT NOW - the engine has already checked costs,
> colors, and targets. Never refuse an offered action because you believe it is illegal or that
> you cannot pay for it; judge only whether it is a GOOD play.

Both are pure rules facts, deck-agnostic, and cheap (two lines). The second also guards every
other deck against the same class of self-invented illegality.

## S4. The "activate your permanents" prior needs a brake: activations must change something THIS turn

**Problem observed.** Mana burned on repeatable activations at useless times: vs133 s21-s22
paid "Untap with Sleep-Cursed Faerie [cost: {1}{U}]" TWICE on the turn the creature was cast
(summoning-sick — untapping achieved nothing); vs140 s30/s59/s65 paid it in main phase 2 /
post-combat against an opponent with zero creatures. Five wasted activations observed, each
one turn's development lost. The deck guide can patch its own card, but the failure shape is
general: the activation prior pushes USE without a usefulness test.

**Current text at fault:**
> "Many permanents do nothing until you ACTIVATE their ability - equipment must be equipped,
> and tap/mana/sacrifice/pump abilities must be fired. Each turn, look at your own permanents,
> take the activations that advance your board..."

**Proposed addition** (one sentence appended to that bullet):
> Activate an ability only when its effect changes something THIS turn - a creature that can
> now attack or block, damage dealt, a card drawn. Paying an activation whose effect does
> nothing this turn (untapping a creature that cannot act, pumping outside combat) is wasted
> mana - skip it.

This is the brake matching the existing gas; the corpus shows the gas alone misfires.

## S5. Minor: a general mulligan default

**Problem observed.** deck44 shipped 3 of 6 opening hands, including (vs131 s0) 4 lands +
a creature + three interaction spells — a clearly keepable 7 — and mulled into a ONE-land six
it only survived by topdecking. Primary attribution is the deck guide's own loud line leaking
into the mulligan seam (fixed in the wave3 deck44 guide), but the general prompt currently
gives the executor NO mulligan anchor at all, so any deck guide wobble decides alone.

**Proposed addition** (one line, in the rules primer or priors):
> Mulligan default: keep any 7-card hand with 2-5 lands and at least one castable spell for the
> early turns; a kept 7 is usually better than a random 6. Your deck guide may override this.

Low confidence relative to S1-S4; include only if other deck agents saw over/under-mulling too.

## What WORKED — do not weaken in the merge

- **The lethal-arithmetic prior.** vs131 s17: the model explicitly ran the sum in its PLAN
  ("2 + 2 + 4 + 5 = 13. Opponent Life: 4. 13 > 4. I win the game."), attacked with everything,
  and won on schedule. Keep "Combat is arithmetic... check for LETHAL" verbatim.
- **Aggressor-by-default / attack-every-turn.** Once creatures were on the battlefield, deck44
  declared attacks nearly every combat in every game (e.g. vs140 s7/s13/s26/s41/s55; vs135
  s10/s14/s18/s20). The passivity problem is upstream (casting), not in combat posture.
- **The cost-payment paragraph** ("a cost is often just HOW A CARD WORKS... Do not refuse a
  cost"). Faerie Bladecrafter's cast option carried a "{right now: life -2}" annotation and was
  cast without cost-refusal dithering in three games (vs110 s7, vs133 s14/s24, vs135 s17). No
  cost-refusal misplays observed in this corpus.

## Not general-prompt faults (recorded so the merge doesn't mis-attribute)

These surfaced in deck44's games but are harness/representation issues, already flagged upstream —
no prompt wording can fix them:
- Reply-vs-consumed-choice mismatch: vs109 s18 reply says "4. Stromkirk Noble" but choice 3 was
  consumed — Go for the Throat destroyed its OWN creature.
- Empty-reply fallbacks logged as `choice: -1` (vs109 s17, vs140 s56).
- The blockers seam almost never asks (1 blockers decision in six deck44 games; 7-damage swings
  taken with untapped potential blockers vs109 and no blockers record).
