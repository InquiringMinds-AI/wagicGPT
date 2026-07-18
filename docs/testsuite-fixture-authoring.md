# Writing test-suite fixtures for the Wagic card-script lexicon

This is the authoring contract for gameplay-behavior fixtures under
`projects/mtg/bin/Res/test/`. It exists because a 543-fixture generation pass produced
a 55% failure rate, and one-by-one triage showed ~90% of those failures were fixture
authoring defects, not engine bugs. Every rule below is named after a failure class
that actually occurred, with its measured frequency. Follow the procedure and the
checklist; they are the difference between a test suite that finds engine bugs and one
that manufactures noise.

## The prime directive: predict the outcome from evidence, not from memory

The single largest failure class (64 fixtures, "wrong-expectation") was asserting an
outcome the cards cannot produce: a 1/2 first-striker expected to survive a 2/2 trade,
a 0-power creature expected to kill something, a card's real ability misremembered,
combat arithmetic done wrong, an engine-correct result (shroud making a spell
*uncastable*, not *cast-and-fizzled*) asserted as its cast-happens variant.

Before writing a single `[ASSERT]` line:

1. **Read the card's actual definition.** For real cards: its `text=`, `auto=`,
   `power=`/`toughness=` lines in `projects/mtg/bin/Res/sets/primitives/*.txt` (grep
   by name). The primitive text is what the engine enforces — not your memory of the
   card, and not the printed cardface. If the card only exists in `unsupported.txt`,
   it does NOT load — pick another carrier (4 fixtures failed on unloadable cards).
2. **Do the arithmetic on paper.** Power, toughness, counters, lords (including a
   witness's OWN lord ability applying to itself), first/double strike, trample,
   toxic. Then write the exact life totals and zone counts that arithmetic gives.
3. **Know the uncastable-vs-fizzle rule.** When protection/shroud/hexproof/split
   second/targeting restrictions remove the only legal target, the spell CANNOT BE
   CAST: it stays in hand and its mana stays unspent. Do not assert it in the
   graveyard with mana paid.
4. **Know these verified engine conventions:**
   - The LAST-listed `library:` card is the TOP of the library.
   - `moveto(exile)` sends a card to its OWNER's exile zone, not the caster's.
   - Day/Night: entering day/night puts a marker CARD on the battlefield — count it.
   - Cast sorceries/instants end in their owner's graveyard — count them.
   - Landhome abilities (`mountainhome` etc.) sacrifice the creature when its
     CONTROLLER lacks the land — give the controller the land, not (only) the defender.

## Synthetic test cards (`test_primitives.txt`)

Second-largest class (46+, "bad-synthetic-card"):

- **Every `type=Creature` witness MUST have `power=` and `toughness=` lines.** A P/T-less
  creature is a 0/0 and dies to state-based actions before your trigger can ever fire.
  This single omission killed an entire ~25-fixture trigger-witness family.
- **Copy ability forms from real cards, never invent syntax.** Before using a
  construct, grep the primitives for a real card that uses it and copy that shape
  exactly. Failures from invented forms: `removemana(green)` (real form
  `removemana({G})`), bare `proliferate` without the `notatarget(proliferation)`
  chooser, `lifeleech:3 target(opponent)` (real form `lifeleech:-3 opponent`),
  `grant +1/+1 grantend` (grant carries a nested `ability$!...!$`), activated
  `poolsave` (real form is static `mypoolsave`), `target(permanent|...)` (no real
  card uses it — use `target(creature|...)` / `target(land|...)`). If NO real card
  uses the form you need, that absence is information: the form is probably
  unsupported, and your fixture becomes an engine-question, not a regression test.
- Give the witness exactly the abilities under test. A stray `{T}:donothing` or a
  self-applying `lord(...)` contaminates the observation.
- An ability granted by ETB (soulbond pairing, imprint) does NOT exist when you
  pre-place the card via `[INIT]` — INIT placement skips ETB. Cast the card in `[DO]`
  when the mechanic depends on entering the battlefield.
- **A card NAME must not contain a driver-command substring** ("choice ", "goto",
  "next", "reveal", "p1", "p2", "yes", "no", "eot", "ai", "human") — clicking it is
  parsed as the command instead ("Lexicon Choice Wrapper Witness" was silently
  unclickable).
- Trigger-response design: a trigger that responds to life gain WITH a life gain
  (`@lifed(player):life:1`) re-triggers itself forever — pick a response that
  doesn't feed its own event.
- A creature carrying an activatable `{0}:` ability cannot be declared as an
  attacker by clicking it (the ability menu intercepts the click and menu-defaults
  to Cancel) — give such witnesses `mustattack` when they need to attack.
- Ids are HARD-CAPPED at 2100000999 — the loader aborts above it.
- **Macros**: invoke `_MACRO_` names with a separating space before any appended
  effect (`_METALCRAFT_ life:1`, never `_METALCRAFT_life:1`) — some definitions
  end flush with their last token. And copy the COST conventions from real
  invocations: e.g. graveyard-activated macros like `_SCAVENGE_`/`_ETERNALIZE_`
  carry the exile-self as the `{E}` token in the card's own activation cost
  (`autograveyard={4}{G}{E}:_SCAVENGE_(...)`) — the macro body does not add it.

## Script pacing (44+, "phase-pacing"; 9 more, "runaway-goto")

The pump pulls ONE `[DO]` command per settle interval, and when the script runs out
the harness asserts IMMEDIATELY — it does not wait for the phase to advance or the
stack to settle.

- **Count your phase advances.** From COMBATATTACKERS, declaring attackers then
  reaching COMBATEND takes THREE `next` commands (attackers→blockers→damage→end), not
  two. Landing exactly on the asserted phase is part of the test's job. Overshooting
  (extra `next` sails past COMBATEND into Main 2/End) fails the same way undershooting
  does.
- **Never assert a phase your script contradicts.** `goto end` then `[ASSERT]
  FIRSTMAIN` is a guaranteed failure (23+ fixtures did exactly this). Assert the phase
  you actually land on — usually `END` after a `goto end`.
- **`goto <phase>` re-issues until satisfied — a goto that can never be satisfied is
  an infinite game** (the suite burns its 1M-update runaway cap, minutes per fixture).
  Known never-satisfiable gotos: `goto blockers` when no attacker exists (or after
  your effect removed the attacker), `goto`-looping across a turn boundary
  (`goto end` + `goto upkeep`), goto-ing while a spell hangs in targeting mode with no
  legal target. Pace with `next` instead; cancel or never cast the uncastable spell.
- Valid phase tokens only: `BLOCKERS`/`goto blockers` is NOT a token (use
  `combatblockers`); an invalid token parses as no-op/INVALID and your assert compares
  against garbage. Transient phases (Cleanup) can't reliably be landed on.
- A lethal game state (10 poison, 0 life) ends phase progression — don't expect to
  keep advancing to COMBATEND after the kill.

## Who can act (12+, "non-active-caster"; plus interrupt protocol)

**The script drives ONE seat: the active player.** A spell sitting in the non-active
player's hand will be clicked but never cast — 12 fixtures silently no-op'd their
whole scenario this way (the "opponent responds with Shock" pattern).

- Design the scenario so the ACTIVE player casts everything, or split across turns so
  each caster acts on their own turn.
- The defending seat CAN declare blocks: during the blockers step, click the blocker,
  THEN click the attacker to assign it (clicking only the blocker declares nothing —
  "combat-block-protocol" failures). During the attackers step, clicking a defender's
  creature does nothing.
- There is no ad-hoc instant-speed response window; opponent-interrupt scripting needs
  the interrupt protocol (`no`/`yes` lines), not bare casts.

## Menus and choices (13+, "menu-timing")

`choice N` must be the PENDING command on the tick its menu arms — menus arm on the
settle tick, one command earlier than intuition suggests. No filler between the
menu-arming action and its answer. Unscripted menus get the menu-default (Cancel if
cancelable, else first option) — that default silently consumed may-prompts, target
choosers, alternative-cost menus, and convoke confirmations in 13 fixtures. Abilities
with `target(...)` need the target-selection click(s) after the source click — even
`target(this)` needs the second click on the source itself. When the target
selection arms as a MENU, answer it with `choice N` (a menu index), not the target
card's name — a name click there yields "menu default: cancel". Player-targeted
choosers (proliferate's poison selection) are driven with `p1`/`p2`.

Known undrivable shapes (don't author fixtures that depend on them): menu/chooser
based ALTERNATIVE-COST casts (subtype offering, suspend) never initiate from the
scripted seat — it is a heuristic AI (`ishuman`=0, and several such cards are gated
`compare(ishuman)`); mana-cost alternatives (emerge) drive fine. BASIC LANDS cannot
be clicked by the suite (printing-id mismatch) — use nonbasic lands anywhere a land
must be clicked, revealed, or selected. The turn-1 draw step is skipped — draw-step
mechanics (dredge) need the game advanced to a later turn's draw.

## INIT state lines

**Repeated same-zone lines in one player block OVERWRITE, they do not append** —
`inplay:Megrim` followed by `inplay:Witness` leaves ONLY the Witness in play, and
the silently-missing permanent reads as "trigger never fired" downstream (this
false negative produced a phantom engine-bug report). Always comma-join:
`inplay:Megrim,Witness`.

## Asserts are exhaustive (11+, "zone-list-vs-count"; 5+, "mana-float")

- **An omitted zone line asserts ZERO cards in that zone.** Account for everything:
  the cast sorcery in the graveyard, the discarded card, the remaining library, tokens
  you created, day/night markers. `*` wildcards count one card each — count them.
- **An omitted `manapool:` line asserts an EMPTY pool (exact equality).** Supply
  exactly the mana the scenario spends — a padded pool leaves a float and fails.
  Cost-reduction tests especially: compute the reduced cost and fund exactly that.
  And check color requirements card-by-card: `{4}{W}` cannot pay `{2}{W}{W}` (5
  fixtures under-colored their pools and their spell never cast).

## Names and data hygiene

- **No comma-carrying card names in comma-separated zone lists** ("Minamo, School at
  Water's Edge" loads as two garbage names). If the scenario needs such a card, pick a
  comma-less equivalent (6 fixtures).
- `[DO]` lines are DRIVER commands (card names, `next`, `goto`, `choice N`, `no`,
  `p1`/`p2`) — card-script syntax like `draw:2` in `[DO]` is silently swallowed.
- Synthetic ids: unique, in the reserved 2100000000-2100000999 range.

## The authoring loop (non-negotiable)

1. Write the fixture per the rules above.
2. **Run it solo before registering it** (from `projects/mtg/bin`):
   ```
   printf '<entry>\n' > Res/test/probe/_authoring.txt
   env -u WAYLAND_DISPLAY -u DISPLAY WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 \
     WAGIC_TESTSUITE_THREADS=1 WAGIC_TESTSUITE_FILE=test/probe/_authoring.txt \
     WAGIC_TEST_PRIMITIVES_FILE=test/lexicon/test_primitives.txt \
     timeout -k 5 60 ./wagic 2>&1 | tail -30
   ```
3. If it fails, READ THE LOG — the trace shows what actually happened ("Can't find
   card:", "menu default: cancel", the phase reached, the life totals). Diagnose
   against the classes above; almost every first-draft failure is one of them.
4. A fixture is DONE only when it has been OBSERVED PASSING, or when you have positive
   evidence the residual failure is the ENGINE's (the fixture is clean per every rule
   here, and a minimal probe variant isolates the misbehavior). Engine findings go to
   `projects/mtg/strategy-design/lexicon-triage/engine-bugs.md` with expected vs
   actual and the trace evidence; the fixture's registry entry moves to
   `_known_failures.txt` until the bug is fixed. NEVER weaken an assert to make a test
   pass, and NEVER register a fixture you haven't run.
5. Batch-verify before handing off: run your full set through the registry and count
   `==Test Failed !==` AND `==Test timed out (game never ended)==` — both must be 0
   across your new fixtures.

## Checklist (run per fixture before registering)

- [ ] Every card name exists in loadable primitives (not unsupported.txt); no commas
      in zone-list names.
- [ ] Synthetic creatures have power= and toughness=; every synthetic ability form is
      copied from a real card.
- [ ] Expected life/zones computed from the cards' actual stats and verified
      arithmetic; uncastable ≠ cast-and-fizzled.
- [ ] All casters are the active player (or act on their own turns); blocks assign
      blocker→attacker.
- [ ] Phase advances counted; asserted phase is the phase the script lands on; no
      unsatisfiable goto.
- [ ] Every menu/target click answered at its arming tick.
- [ ] All zones accounted for (cast spells, tokens, markers, library remainder);
      mana funded exactly.
- [ ] Observed passing solo, or filed as an engine finding with evidence.
