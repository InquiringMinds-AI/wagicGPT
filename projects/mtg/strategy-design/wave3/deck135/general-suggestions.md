# General-prompt suggestions from the deck-135 review (wave 3)

Corpus: `matchups-20260709-220729`. Deck 135 games cited by opponent + translog:
- **v109 (loss)** `1783654166-ai_baka_deck135-*.jsonl`
- **v140 (timeout)** `1783652862-...`, **v133 (timeout)** `1783652871-...`, **v131 (timeout)** `1783655257-...`
- **v44 (win)** `1783655263-...`, **v110 (win)** `1783657362-...`

All quotes of current text are from `bin/Res/ai/gpt/system_prompt.txt` as deployed in this corpus.

---

## S1. State the precedence: the deck guide OVERRIDES the general priors

**Problem observed.** A general prior defeated an explicit deck-guide directive. The deck-135
guide said of Arcum's Astrolabe: "Cast it on sight, every copy... There is almost never a
reason to hold it." In v131 seq 18 (its own Main 1, T5), qwen declined the second Astrolabe:
"Do not cast Arcum's Astrolabe yet as it is already on the battlefield and casting another
copy is redundant and mana inefficient" — the general anti-redundancy prior won. That copy
would have drawn a card AND fixed the blue-mana screw that stalled the whole game.

**Current text at fault** (two places):
- The header: "STRATEGY (priors to reason from, not absolute rules):" — says the priors are
  soft, but never says what beats them.
- Prior line: "But do not dump your whole hand or cast redundant copies just to empty your
  mana; spend it on the plays that actually advance your plan."

**Proposed change.** Add one sentence immediately after the `{STRATEGY_GUIDE}` slot or at the
top of the STRATEGY block:
> "Your STRATEGY GUIDE above was written for your exact deck. Where it conflicts with the
> general principles below, FOLLOW THE GUIDE — a specific instruction about a named card
> always beats a general principle."

And narrow the redundancy clause so it stops firing on value copies:
> "...do not cast redundant copies just to empty your mana — but a copy is NOT redundant if
> it does something when it arrives (draws a card, makes mana, adds to a count your deck
> cares about) or if your strategy guide tells you to cast every copy."

## S2. The carried plan is a note, not the truth — ground decisions in the CURRENT SITUATION and action list

**Problem observed, twice, both game-losing in kind:**
- v140 seq 58: an uncracked Prismatic Vista sat on the battlefield with its crack offered as
  option 2; qwen passed, reasoning "My Prismatic Vista ... has been tapped/sacrificed in
  previous turns" — a hallucinated memory carried through its PLAN line beat the live board.
- v131 seq 51: prompt says "Phase: Main phase 1 | It is your turn." with an 8/8 Abominable
  Treefolk castable-adjacent in hand; qwen chose "Cast nothing," reasoning "I will pass
  priority and let the opponent proceed to their End Step" — the plan written during the
  OPPONENT'S previous turn was still steering, and it misread whose turn it was.

**Current text at fault.** "During the game you will receive the game log ..., the current
board state ..., your own last stated plan, and the choices that are legal for you right
now." — the plan is introduced as a peer of the board state, with no staleness warning and
no tiebreak.

**Proposed change.** Extend that paragraph:
> "Your last stated plan is a note from your past self and may be STALE or WRONG: the
> CURRENT SITUATION and the list of legal choices are the ground truth. If an action is
> still offered in the list, it has NOT been done yet, whatever your plan says. Re-read the
> phase line every decision: when it says 'It is your turn' and a main phase, this is your
> main phase — act now, do not defer to a 'next turn' that is actually this one."

## S3. Teach what the action list already guarantees (affordability and attack legality)

**Problem observed.**
- Mana: qwen repeatedly planned to make {U} from lands that cannot ("cast Ice-Fang Coatl
  (using {G}{U} from Forest and Plains)" — v131 seq 18 plan), and repeatedly promised to
  cast cards "next turn" that stayed uncastable for 6+ turns. The engine's cast list is
  affordability-filtered, so the offered list already encodes the answer it kept getting wrong.
- Attack legality: in v109 seq 18-22 (T7), qwen cast Abominable Treefolk and then committed
  THREE consecutive decisions to "attack with Treefolk and Druid for 9 this turn." The
  attackers list offered only "A1. Boreal Druid (1/1)" — Treefolk was summoning-sick. It
  replied "A1" while narrating a two-attacker swing; the phantom 8 damage anchored a race
  plan that got it killed. The rules primer DOES state summoning sickness ("Creatures cannot
  attack the turn they enter play"), but qwen doesn't apply the abstract rule during planning.

**Current text at fault.** Nothing tells the executor that the lists are complete and
pre-validated; the primer states the rules but not the operational shortcut.

**Proposed addition** (to the "you will receive..." paragraph or the choice instructions):
> "The choice lists are complete and already rules-checked. If a spell is not in the cast
> list, you CANNOT cast it right now (usually you lack the colored mana — plan to fix that,
> not to cast it anyway). If a creature is not in the attackers list, it CANNOT attack this
> turn (creatures cast this turn without haste never can) — count this turn's damage ONLY
> from creatures actually in the list."

## S4. Mirror the lethal check with a mechanical SURVIVAL check

**Problem observed.** v109, T9: at 7 life against a board of Stromkirk Noble (3/3, growing)
plus four 1/1 goblins — exactly 7 potential swing-back damage — qwen attacked with both its
creatures including its only untapped potential blocker (seq 31, "A1, A2"), planned "next
turn ... attack for lethal," and was dead on the opponent's turn. The race was actually
winnable WITH one block held back.

**Current text at fault.** The aggressor line carries the concept but not the arithmetic:
"attack every turn with everything not needed to survive a lethal swing back" — and the
combat paragraph's math is all offense: "check for LETHAL: add the power of the attackers
that will connect..." A weak executor runs the sum it is given; it was never given the
defensive sum.

**Proposed addition** (to the combat-is-arithmetic paragraph, right after the lethal check):
> "Then run the same math in reverse before you attack: add the power of every opponent
> creature that will be able to attack you on their turn. If that total is at least your
> life, keep back enough blockers to survive it — a creature that must stay home is 'needed
> to survive the swing back' and does not attack, even if the attack looks profitable."

## S5. Put a time limit on "holding interaction"

**Problem observed.** In all three timeouts, qwen justified pass after pass with its
counterspell: v140 held TWO Force of Negation from ~T6 to the T16 timeout against an
opponent that never presented a must-counter spell (final: opponent 19 life after 16 turns);
v131 similar ("Keep Force of Negation ready..." recurs in plans while the deck cast nothing).
The deck guide now overrides for this deck, but the enabling prior is general and other
reactive decks will hit it.

**Current text at fault.** "hold instant-speed interaction for the opponent's threats ...
if no target appears this turn, keep holding." — "keep holding" is unbounded, and for a
weak executor it licenses indefinite durdling.

**Proposed change.** Bound it:
> "...if no target appears this turn, keep holding — but holding is a bet, not a plan. If
> the opponent has gone two or more of their turns without casting anything worth your
> reaction, stop reserving mana for it: develop and attack instead, and cast the reactive
> card only when a real target finally appears (or spend it if it can profitably target
> something now)."

(Deck-agnostic phrasing on purpose: keyed to observable evidence — opponent turns without a
qualifying spell — not to any card name.)

---

## Lines that WORKED in this corpus — do not weaken

- **The fetchland/cost paragraph**: "a cost is often just HOW A CARD WORKS: a fetchland is
  meant to be sacrificed for a real land..." — Deck 135's wave-2 #1 failure (fetches never
  cracked) is GONE: in all six games fetches were cracked promptly, usually the same phase
  they were played (e.g. v140 seq 2, v133 seq 2, v131 seq 2 — turn 1 cracks). One residual
  decline (v140 seq 58) was a stale-plan hallucination (see S2), not cost-refusal. Keep this
  paragraph at full strength.
- **"Your own turn is for ACTING ... passing your turn with castable cards and unused mana
  is a losing habit."** — Both wins show a cast nearly every turn (v110: T3 ramp, T5 triple
  cast, T7 Coatl+Viper, T9 Viper...). Where the timeouts still durdled, the cause was
  uncastability (mana screw) or the S1/S5 priors, not this line failing. Keep.
- **"be the aggressor by default"** — the wins attacked every turn and closed (v110 ground
  the opponent from 20 to 1 by T17); no over-aggression misplays observed except the one
  covered by S4, which needs the defensive check added, not the aggression removed.
- **The offense LETHAL check** — qwen visibly attempts the sum in its plans (v109 plans
  repeatedly total attacker damage). Its inputs were sometimes wrong (S3's phantom attacker),
  but the arithmetic habit is trained in; S3/S4 fix the inputs, don't touch the check.

## Explicitly NOT general-layer (for the record, so the merge doesn't over-fix)

- The blue-mana screw itself (fetching Forest every time) — deck-guide fault, fixed in the
  wave-3 deck-135 guide.
- Attacking two small creatures into Lightmine Field (v140 seq 37, board wiped for zero
  damage after qwen quoted the card and botched the math) — model arithmetic failure (d)
  with a deck-guide named-card patch; no deck-agnostic wording would have saved it beyond
  the existing "sloppy combat math" warning.
- The near-total absence of blockers decisions corpus-wide (3 in 21 games; deck 135's fatal
  v109 alpha-strike produced NO blockers ask) — representation/engine seam (c), not prompt
  wording. Flagged separately; no general-prompt change proposed.
