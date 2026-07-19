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

### Reveal / surveil / scry displays

A `reveal:`/surveil/scry effect opens a **reveal display** — a card box the player
clicks IN to choose which revealed cards go where (surveil: graveyard vs. back on
top). Driving one from a fixture:

- **While a reveal display is open, a card-name click lands IN the display.**
  `TestSuiteGame::getCard` searches the reveal zone FIRST whenever a reveal display
  is open (an `OpenedDisplay` backed by a populated reveal zone), so a click resolves
  to the revealed copy — never a same-named copy elsewhere (its pre-reveal library
  home, a second printing) — exactly as a human clicking the on-screen display would.
  When no reveal display is open this is inert: library/tutor clicks keep their normal
  resolution.
- **Pace the click AFTER the reveal opens.** A TRIGGERED reveal opens a beat late: the
  trigger must resolve first. Declare/arm the trigger source, then advance so the
  trigger resolves and the display opens BEFORE you click — one `next` for an
  attack/combat-damage trigger (assert the phase you land on: BLOCKERS after one
  `next` from attackers, DAMAGE after two), or a `goto <phase>` pump-in-place for an
  upkeep trigger (a second `goto upkeep` after you have reached upkeep spends one pump
  cycle without leaving the phase, letting the trigger resolve). A click issued before
  the display opens is consumed against the library and is NOT retried.
- **Select, then finalize.** With the display open, click each card to bin by NAME
  (one command per card for a multi-card `<upto:N>` surveil), then `revealnext`
  (JGE_BTN_NEXT) to finalize the `<upto:N>` selection; the unselected remainder
  auto-resolves back on top. `revealok` (JGE_BTN_OK) also advances a single-option
  reveal. You do NOT need a `choice 0` before the clicks — the reveal's option menu
  auto-picks "put in graveyard" (option one) via the menu-default and re-queues your
  click onto its target chooser. Use non-basic library cards only (basics are not
  clickable), and remember the LAST-listed `library:` card is the top (revealed first).
- **`aicode=` cards need the `interactivereveal` opt-in.** Cards that carry an
  `aicode=` surveil/scry (Grim Flayer, Sultai Ascendancy, and other `_SURVEIL2_/3_`
  cards; also search-tutors like Armillary Sphere, Collected Conjuring) substitute a
  headless heuristic for the interactive display when an AI controls them — and the
  scripted suite seat IS an AI subclass, so by default it takes that substitute (which
  it cannot select from). To test the interactive selection instead, put
  `interactivereveal` as the FIRST `[DO]` line: it sets `mForceInteractiveReveal`, so
  `GenericRevealAbility`/`GenericScryAbility::resolve` opens the real display and your
  reveal-card clicks drive it. Use it ONLY when you are driving the display — omit it
  and an aicode fixture keeps the (deterministic-to-that-fixture) aicode path it was
  written for. A card with NO `aicode=` (Appendage Amalgam) always uses the interactive
  display and needs no opt-in. See `macro_surveil1_appendage_amalgam.txt` (attack
  trigger, no aicode), `macro_surveil2_sultai_ascendancy.txt` (upkeep trigger, aicode,
  multi-select) and `macro_surveil3_grim_flayer.txt` (combat-damage trigger, aicode,
  select three) for worked examples.

### Pay-or-decline prompts (extra costs / ward): `paycost` and `cancelcost`

Some cards arm a **pay-or-decline extra payment** (`GameObserver::mExtraPayment`) — a
prompt that counters/fizzles the effect unless a mana cost is paid. Ward is the
canonical case: `_WARDn_` triggers when the creature is targeted by an opponent's
spell/ability and offers that opponent's controller the choice to pay `{n}` or have
the spell countered. Two `[DO]` commands drive this prompt; a human presses a mana
key to complete it or `JGE_BTN_SEC` to decline, and the scripted seat has neither.

- **`paycost`** — complete the payment. It taps the paying player's free untapped
  producers to cover the extra cost (`ManaEngine::autoTapForCost`, the same path a
  human's casting-payment takes), then the engine's own auto-pay resolves the paid
  branch on the next tick. The payer must control enough untapped producers, or hold
  enough floating pool mana, to cover the cost; if it cannot, `paycost` logs loudly
  and leaves the prompt untouched (author a `cancelcost` after it, or expect the
  engine's own auto-decline). NOTE: if the pool already covers the cost when the
  prompt arms, the engine auto-pays with no command needed — put the mana in an
  untapped **producer** (a nonbasic land / mana rock), not the floating pool, when
  you want `paycost` itself to be the acting command.
- **`cancelcost`** — decline the payment. It chooses the **unpaid branch** of the
  still-open pay-or-decline menu (the counter/fizzle option — the last menu item),
  the human's "don't pay" click. Use this even when the payer *could* pay: it is the
  only way to decline while holding enough mana (a fully-broke payer auto-declines on
  its own). It is exempt from the menu-default (so it reaches the open menu rather
  than being pre-answered onto the paid branch).

Driving the full ward sequence — `A` casts a spell at `B`'s warded creature:

```
[DO]
Shock                        # cast the spell
Dancing Sword Construct      # target the warded creature
choice 0                     # answer the ward trigger's menu (open pay-or-decline)
Shock                        # re-click the SPELL to satisfy ward's target(spell) chooser
paycost                      # ...or `cancelcost` to decline
```

The re-click of the spell after `choice 0` is load-bearing: ward's script targets the
triggering spell (`target(*|opponentzones)`), and that chooser — owned by the defender
— is what actually wedges the scripted seat, upstream of the payment itself. Only real
mana-tax ward exists in the card pool (all `_WARDn_`); there are no life/discard forms.
See `Res/test/lexicon/macro_ward_pay.txt` and `macro_ward_decline.txt` for worked
examples.

## INIT state lines

**Repeated same-zone lines in one player block OVERWRITE, they do not append** —
`inplay:Megrim` followed by `inplay:Witness` leaves ONLY the Witness in play, and
the silently-missing permanent reads as "trigger never fired" downstream (this
false negative produced a phantom engine-bug report). Always comma-join:
`inplay:Megrim,Witness`.

**Comma-carrying card names in zone lists: escape the comma as `^`** —
`inplay:Anafenza^ the Foremost` (the parser replaces `^` with `,` before lookup,
the engine's existing escape convention). Unescaped commas split the name into
garbage that silently never loads — this single trap manufactured THREE phantom
engine-bug reports (legendarylandwalk, combattoughness/Doran, Anafenza).

**`inplay:` cards are CAST during setup, in list order, firing their ETBs** — a
card with an ETB draw (Spark Rupture) can pull a LATER-LISTED card out of the
library before the harness places it, silently deleting it from the scenario.
List draw-ETB cards LAST and account for the drawn card in your asserts.

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

## Harness driving facts (2026-07-19 session)

- **Suite summary lines go to STDERR.** Always capture `> log 2>&1`; a
  `2>/dev/null | grep` pipeline silently loses the "Test suite finished"
  line and failure output.
- **`WAGIC_TESTSUITE_FILE` takes a REGISTRY file** (one Res/test-relative
  fixture path per line), never a fixture directly — feeding it a fixture
  makes every line parse as a filename ("Could not load test file") and
  the run hangs.
- **`choice N` is 0-based.**
- **`p1` / `p2` are the [DO] player-click tokens.**
- **`may`-prompts are MENUS**: answer with `choice 0` (accept). Clicking
  the trigger's source card does NOT answer them.
- **Damage-vs-loyalty arithmetic must be exact** in planeswalker fixtures:
  a survivor is indistinguishable from a fizzle unless you finish with a
  second burn whose sum exactly equals starting loyalty (the
  Shock-finisher pattern, see probe/sparkcaster-era fixtures).
- **The registry has no size cap anymore** (files[1024] fixed 6c22c63c2),
  but the incident is the reason any full-suite segfault right after the
  pregame tests on an OLD binary means "registry > 1024", not a code bug.
- **Async reveal fixtures**: `WAGIC_REVEAL_TEST_ASYNC="Name1,Name2"` makes
  the scripted seat run the real interactive-AI reveal driver with those
  picks (see probe/glacial_revelation_async.txt header for the recipe);
  such fixtures are run explicitly, not registered.
