# General system-prompt suggestions from the deck-110 review (wave 3)

Evidence base: deck110's 6 games in `matchups-20260709-220729` (0/6 — five 40-min timeouts,
one loss 1v18 to deck135). Log citations use `<epoch>` + seq. Key games:
- LOSS vs 135: `1783657362-...deck110...jsonl` — 88% of casting decisions were "Cast nothing."
- vs 44: `1783654819` — refused a free 4-damage Blast and all 3 offered equips over mana misconceptions.
- vs 140: `1783652860` — held Cranial Plating in hand 10+ turns, attacking with 0-power dorks.
- vs 133: `1783652864` — re-equipped an already-equipped creature turn after turn.

Deck110's guide-level faults are fixed in `wave3/deck110/strategy.txt`; the items below are the
(b) attributions — problems the GENERAL layer caused or permitted, with proposed deck-agnostic wording.

---

## 1. State that the numbered action list is affordability-pre-checked (HIGHEST PRIORITY)

**Problem observed.** qwen constantly declines listed casts because it believes it cannot pay.
It reads "Mana in your pool: (none)" as "I have no mana," and reasons wrongly about colors:
- Loss vs 135, seq 3: untapped Island on battlefield, `Cast Signal Pest {1}` listed; reply: "I
  cannot cast Signal Pest or Cranial Plating because I have no mana sources available (my Island
  is untapped and produces Blue, while I need Red or generic mana)." (Blue pays generic.)
- Same game, seq 6: two untapped lands incl. a Mountain, `Cast Signal Pest`/`Cast Cranial Plating {2}`/
  `Cast Galvanic Blast {r}` all listed; reply: "my Mountain is untapped but I have no way to
  produce Red mana." Chose Cast nothing.
- vs 44, seq 16: untapped Darksteel Citadel + Mox Opal on board; passed equip AND Steel Overseer:
  "My current mana pool is empty, and I have no untapped lands to pay for the {1} Equip cost."
This one misconception produced 44–88% cast-nothing rates per game and is my prime suspect for
the corpus-wide timeout epidemic (13/21): games durdle, never close, and each decision costs
~60–70s of inference, so a 40-min game is only ~35–50 decisions.

**Current text at fault.** Nothing states the filtering; the board format ("Mana in your pool:
(none)") plus the priors' mana-conservation language leaves qwen to derive castability itself,
and it derives it wrong. This is a general fact about the interface, not a per-deck fact.

**Proposed addition** (in the closing block, right after "the choices that are legal for you
right now"):
> Every action in your numbered list is legal and PAYABLE right now — the game has already
> checked your mana and will tap your lands automatically when you cast. Untapped lands ARE
> available mana; "Mana in your pool: (none)" is the normal state and never a reason to pass.
> Never decline a listed action because you believe you lack the mana or colors to pay for it.

## 2. Give the hold-instants prior a numeric release valve

**Problem observed.** Instants get held literally forever. Loss vs 135: two Galvanic Blasts in
the opening hand were still in hand at T17 with qwen at 1 life, never cast at anything across
the whole game (seq 8: "I will hold it in case they cast a non-creature spell I can counter" —
it is burn, not a counterspell). vs 44, seq 15: with lethal-path board, refused a free 4-damage
Blast at a 16-life opponent: "Although I have the mana to deal 4 damage... I have no untapped
lands to equip Cranial Plating this turn."

**Current text at fault.** "Instant-speed cards ...: hold them for the opponent's turn or the
exact moment you need them. ... if no target appears this turn, keep holding." The counterweight
sentence ("Direct damage and burn are part of your CLOCK, not reactions — as the aggressor,
spend them to pressure or finish") exists but loses to "keep holding" because it has no trigger
a weak executor can evaluate.

**Proposed change.** Keep the hold-instants default, but append a concrete release:
> Exception — damage spells are your clock, not your shield: if the damage spells in your hand
> plus your attackers add up to the opponent's remaining life, fire everything at the opponent
> NOW; and once the opponent is at 8 life or less, send damage spells at their face rather than
> holding them. A burn spell still in your hand when the game ends was a wasted card.
(The existing lethal-check sentence in the combat paragraph covers attacks; this puts the same
arithmetic where the hold-instants prior lives, which is where the hoarding decision is made.)

## 3. "Keep enough mana untapped" needs an explicit priority under deploying

**Problem observed.** Mana-conservation reasoning beat deployment all corpus long. Loss vs 135,
seq 13: two untapped Mountains, `Cast Cranial Plating {2}` listed; reply: "I will cast nothing
this turn to conserve my mana. I need to leave at least one land untapped to pay the {1} equip
cost ... next turn" — repeated in some form every turn; the Plating was cast on T12. vs 140:
five consecutive attack turns with an unequipped 0/1 + 0/2 while "keeping my mana open for the
crucial Cranial Plating equip on my next turn" (seq 36, T12 — the equip never happened).

**Current text at fault.** "keep enough mana untapped to do so - not only for interaction"
(activations paragraph) — correct in intent, but stated without rank, and a weak executor lets
"save mana" veto "cast anything." My deck guide had the same flaw amplified ("DO NOT TAP OUT"),
now removed — but the general line needs the rank stated too.

**Proposed addition** (same paragraph):
> Deploying outranks saving: never skip casting a creature or a key permanent this turn in
> order to keep mana open for a future turn — cast it now and take the activation next turn
> if you must. It is fine to tap out to develop your board.

## 4. Repeated activations that gain nothing (waste loops)

**Problem observed.** vs 133 (`1783652864`), seq 13–20: Memnite already shows "(4/1) (printed
1/1)" — Plating attached — yet qwen re-takes "Equip with Cranial Plating targeting Memnite
[cost: {1}]" turn after turn "to ensure the +5/+0 bonus is active," burning mana and wall-clock
decisions. The engine keeps offering the action; qwen cannot tell "available" from "still useful."

**Current text at fault.** "Each turn, look at your own permanents, take the activations that
advance your board" — with no stop condition, this actively encourages the loop.

**Proposed addition** (same sentence):
> ...take the activations that advance your board — but an activation whose effect is already
> in place gains nothing when repeated: if the equipment is already attached (the creature's
> displayed power already shows the bonus) or the effect is already active, do not take the
> same activation again.

## 5. Chosen number must execute the stated plan

**Problem observed.** Plan/number mismatch: vs 44, seq 6 and 7 — the reply PLAN says "Cast
Cranial Plating now to get it on the battlefield, then equip it" while the chosen option is
"Cast nothing right now." Twice in a row. The right play was fully reasoned and then not taken.

**Current text at fault.** The closing instruction ("Then pick the play whose gain most clearly
exceeds its cost") never binds the pick to the stated plan.

**Proposed addition** (closing block):
> Your chosen number must EXECUTE the first step of your own plan this decision. If your plan
> says to cast or activate something that is in the list, pick that number — never describe an
> action and then choose "Cast nothing" or pass.

## 6. Anti-deferral: "next turn" repeated is a red flag the prompt can name

**Problem observed.** The dominant durdle rationale is a perpetually deferred plan: "I will
untap next turn, cast Cranial Plating, and immediately equip it" (loss vs 135, seq 13), "wait
for my next turn to play a land and deploy my artifact engine" (seq 8), "cast Plating next
turn" (vs 140, every attack turn) — the promised turn never comes.

**Current text that partially works.** "Your own turn is for ACTING ... passing your turn with
castable cards and unused mana is a losing habit" — good, and it should NOT be weakened; qwen
just routes around it via the mana misconception (#1) and the conservation veto (#3). With those
fixed, add one reinforcing clause here:
> If you are planning to take an action "next turn" that you could legally take this turn, take
> it now — a plan deferred every turn is a pass.

## Lines that WORKED — do not weaken

- **Aggression/attack priors.** "be the aggressor by default," "attack every turn with everything
  not needed to survive." Every one of deck110's 14 attackers decisions attacked with everything
  available, with correct battle-cry and protection reasoning quoted in the replies (e.g.
  `1783655261` seq 14: attacks with Signal Pest + Etched Champion citing battle cry and
  protection-from-all-colors). Passivity here was a casting problem, never a combat problem.
- **The activations prior in its positive direction.** Steel Overseer's tap was taken when
  offered with mana understood (vs 44, seq 9), and equips WERE taken in the games where the
  mana misconception didn't fire (vs 133: 3 equips taken). The additions in #3/#4 are scoping,
  not reversal.
- **"Know how YOUR deck wins ... DEPLOY your win conditions."** When qwen did act, its PLAN text
  consistently named the correct deck plan (Plating + equip + Blast finish) — the strategic
  understanding is there; the failures were mechanical (payability, deferral, plan/number
  binding). Keep the win-condition framing as is.

## Representation notes (not general-prompt text, logged for completeness — (c) layer)

- "Mana in your pool: (none)" is the single most misleading surface element; if the
  representation can annotate it (e.g. "Mana in your pool: (none) — N untapped mana sources
  available"), suggestion #1 becomes even stronger.
- Mulligan was asked twice in identical form back-to-back (loss vs 135, seq 0–1) — harmless but
  burns ~1 min of wall clock per duplicate decision at ~60-70s per LLM call.
