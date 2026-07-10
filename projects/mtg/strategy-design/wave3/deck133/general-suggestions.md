# General-prompt suggestions from the deck 133 review (Phyrexian Asphodel, 0/6)

Evidence base: 6 games (1 loss vs deck109, 5 timeouts vs 110/135/44/131/140), run
`matchups-20260709-220729`. Corpus stats for this deck: 101 casting decisions, 76
chose "Cast nothing"; Bloodghast cast 0 times in 77 offers, Gray Merchant 0/25,
Collective Brutality 0/46; 61 replies asserted "no mana / cannot cast" while the
same record's board showed untapped lands. Every timeout traces to non-casting.

Suggestions are ordered by strength of evidence. Each: problem observed (cited),
current text at fault (quoted), proposed wording.

---

## 1. State the affordability / auto-tap fact (STRONGEST single fix in this corpus)

**Problem observed.** The pilot systematically reads the board line `Mana in your
pool: (none)` as "I cannot cast anything," and refuses every offered cast. 61
replies across 6 games assert lack of mana; in most of them the same prompt's
battlefield line shows 4-5 UNTAPPED Swamps. Two decisive examples:
- vs deck131 (timeout), seq 73-77: opponent at 4 life, six castable spells in hand
  (including a lethal Gray Merchant drain line), five untapped Swamps — four
  consecutive turns of "Cast nothing... I have no untapped mana to cast spells (my
  lands are tapped)" [they were not] until the 40-minute cap.
- vs deck109 (loss), seq 31-32: at 6 life with 4 untapped Swamps and Bloodghast /
  Collective Brutality / Liliana castable, replied "I have no mana available to
  cast any of my spells... my lands are untapped" and passed to its death.

**Current text at fault.** Nothing states the affordability contract. The primer
explains cost NOTATION only: "Mana costs are written like {2}{R} (two generic plus
one red)." The closing paragraph lists what the pilot receives ("the current board
state... and the choices that are legal for you right now") — "legal" does not, to
a weak executor, imply "payable," and nothing explains that lands are tapped
automatically or that an empty mana pool is the normal resting state.

**Proposed addition** (two placements; both cheap):
- In the rules primer, immediately after the mana-notation line:
  "Lands produce mana by tapping. You never need to tap lands yourself: when you
  choose to cast a spell, the game taps your lands to pay automatically."
- In the closing per-decision paragraph, after "the choices that are legal for you
  right now":
  "Every choice offered to you is legal AND affordable RIGHT NOW - the game has
  already checked your mana and will tap your lands to pay. 'Mana in your pool:
  (none)' is the normal state between actions, not a constraint; your untapped
  lands ARE your available mana. Never decline an offered cast because you believe
  you lack mana."

**Companion (representation layer, not prompt text).** The board line itself
invites the misread. Consider rendering it as e.g. `Mana available: N untapped
lands (pool: none)` — that would let every guide/prompt line above be shortened
later. I had to embed the corrective fact in the deck-133 guide (#1 rule); once
the general layer carries it, delete it from every deck guide.

## 2. Bound the hold-instants prior — holding a card must never hold the turn

**Problem observed.** One Fatal Push in hand repeatedly justified casting NOTHING
for the whole turn. vs deck44 (timeout): "Cast nothing this turn to preserve mana
and keep Fatal Push in hand for instant-speed interaction" — while creatures sat
castable in hand and the board was empty; the same shape recurs across games
(Fatal Push: 40 offers, cast once; 36 of its declines were into "Cast nothing").
The prior generalizes in a weak executor from "hold this instant" to "hold
everything."

**Current text at fault.** "Instant-speed cards (instants, flash creatures,
activated abilities): hold them for the opponent's turn or the exact moment you
need them." — correct advice, stated unbounded. Also "keep enough mana untapped"
(synergy bullet) reinforces the all-purpose mana-preservation excuse.

**Proposed wording** (append to the instant-speed bullet):
"Holding an instant means holding THAT CARD, not your turn: still cast your
creatures, sorceries, and permanents this turn. Never choose 'Cast nothing' on
your main phase just to protect an instant in hand - a held instant plus an
undeveloped board loses to any clock."

## 3. Break the deferred-plan loop ("next turn" procrastination)

**Problem observed.** The pilot's carried PLAN line makes deferral
self-reinforcing: it writes "I will attack / cast it next turn," re-reads its own
plan at the next decision, and re-defers — indefinitely. vs deck131: "attack with
my full board next turn" while declaring nothing, for multiple consecutive turns
with the opponent at 4 life; vs deck109: the same "on my next turn I will..."
formula in nearly every plan while turns 2-5 passed with no spell cast. This
interacts with the PLAN-carryover mechanism, so it is a general-layer problem, not
a deck one.

**Current text at fault.** "Your own turn is for ACTING." states the principle but
gives the executor no way to notice it is the future self its last plan deferred
to. Nothing addresses the pilot's own carried plan.

**Proposed addition** (to the ACTING bullet, or beside "YOUR PLAN" in the
per-decision framing):
"If your last stated plan said you would do something 'next turn' or 'later,' that
time is NOW - execute it this turn or state what visibly changed. Never re-defer
the same action twice."

## 4. Make casting the explicit DEFAULT on the pilot's own main phase

**Problem observed.** 76 of 101 casting decisions were "Cast nothing" — with
castable threats offered in nearly all of them (Bloodghast 77 offers/0 casts,
Obliterator 32/1, Gray Merchant 25/0). The existing anti-passivity language is
principle-shaped and lost every time to the excuses above (no-mana belief,
mana-preservation, deferral).

**Current text at fault (works, but not strong enough alone).** "A spell sitting
in your hand does nothing - passing your turn with castable cards and unused mana
is a losing habit." Good line — keep it verbatim — but it describes a habit rather
than binding the immediate choice.

**Proposed addition** (immediately after that sentence):
"On your own main phase, casting something is the DEFAULT: if the choices list
offers a creature, threat, or engine you can cast, pick one unless you can name a
concrete, visible-on-the-board reason not to. 'Cast nothing right now' is the
exception and needs that reason."

This stays deck-agnostic; deck guides then only need to name WHICH card, not fight
the passivity itself.

## 5. Sharpen the cost paragraph for fetchlands: immediacy, not just willingness

**Problem observed.** The pilot accepts that fetches are meant to be sacrificed
but treats WHEN as free: vs deck109 seq 7, with one land in play, it declined the
fetch activation — "I will not fetch a second land this turn as it costs life and
offers no immediate tempo advantage" — and stalled on 1-2 lands for three turns;
uncracked fetches also sat through several end steps in other games (vs deck135,
vs deck131) before eventually being taken.

**Current text at fault.** "a fetchland is meant to be sacrificed for a real land,
and paying life, tapping, discarding, or sacrificing a spent or expendable
permanent to advance your plan is that card doing its job. Do not refuse a cost
that moves you toward winning." — establishes willingness but not timing, and "Do
not refuse" leaves "later" open.

**Proposed wording** (one clause appended):
"...that card doing its job. Take such costs PROMPTLY - in particular, sacrifice a
fetchland the first time you are offered its activation; 1 life is never a reason
to delay making a land."

## 6. (Lower confidence) One line on zones: a card in hand must be cast

**Problem observed.** A recurring hallucination that graveyard-recursion applies
to a card in HAND: the pilot repeatedly declined to cast Bloodghast "because
landfall will return it from the graveyard for free" — but it had never been cast,
so it was never IN the graveyard (vs deck44: "Casting a new spell now is
unnecessary... triggers Bloodghast's landfall ability to return it from the
graveyard" with Bloodghast in hand). Deck-agnostic version of the confusion:
recursion text on a card is read as making casting unnecessary.

**Proposed addition** (rules primer, one line):
"A card in your hand does nothing until you cast it. Abilities that return a card
from your graveyard only work once that card has actually been in play or been
discarded - they are never a reason to skip casting it."

I rate this lower because the evidence is one card's text pattern (though it
recurred across games); check whether other decks' reviews saw the same
zone-confusion shape before spending a primer line on it.

---

## Lines that WORKED — do not weaken

- **"Choose by IMPACT, not by convenience."** Deck 133's discard-target picks were
  consistently strong under it: took Pillar of Flame from a burn deck (vs 109, seq
  16), Cranial Plating from the equipment deck (vs 110, seq 10), Young Pyromancer
  from the spells deck (vs 131, seq 25). No weakest-card strips observed. Keep
  verbatim.
- **Combat/lethal arithmetic + aggressor bullets.** Whenever the pilot actually HAD
  creatures, attack declarations were sound: correct multi-attacker swings with
  damage sums in the plan (vs 131: "attack with both... 3 + 5 = 8, reducing them to
  4"), correct singles elsewhere, and its one "no attackers" was a correctly-read
  tapped-creature turn (vs 44). The failure was upstream (nothing deployed), not in
  these lines.
- **"passing your turn with castable cards and unused mana is a losing habit"** —
  right diagnosis, keep; suggestion 4 only adds a binding default on top of it.
- **The cost paragraph's fetch sentence** — the pilot never called sacrificing a
  fetch a mistake per se; only timing failed (suggestion 5).

## Priority note for the merge

If only one change survives synthesis, take suggestion 1 (affordability/auto-tap).
For this deck it is upstream of everything: the beatdown bullet, the ACTING bullet,
the deploy-win-conditions bullet, and the entire deck guide were all inert while
the pilot believed it could not pay. The 61-reply count is directly reproducible:
grep the deck133 translogs for "no mana|cannot cast|can't cast" and compare each
hit's reply against the battlefield line in the same JSONL record.
