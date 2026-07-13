# Skill-revision proposals — deck131 reviewer, wave 4

Against `strategy-design/wave3/strategy-writing-skill.md`. Evidence base: 12-game wave-4
corpus (2W/8L/2TO), findings.md in this dir, and the guide revision in strategy.txt.
Format per proposal: QUOTE (wave-3 passage) -> EDIT (replacement or addition) -> WHY.

## What the skill got right this wave (no edit, but confirm in synthesis)

- **Step 0 demote-and-compress worked exactly as written.** The wave-3 #1 rule
  (cast-every-turn) was followed in the new corpus; I demoted it below a new #1 and
  compressed it, spending the headline on the new leak (blocking). The skill's wave-loop
  is the right shape — the #1 RULE slot is wave-scoped, and Step 0 already says so.
- **"Confirm before you accuse" paid off again**: the Island-flood losses looked like a
  Mountain-first rule violation until the per-record prompt check showed the Mountain was
  never in hand before the turn it was played (ep1783922461, Island T2-T8, Mountain played
  the very turn it was first offered). Rule obeyed; cause was variance + manabase.
- **The reply/PLAN-mining and offered-vs-taken machinery** correctly separated model
  passivity from having-nothing-castable in the durdle games.

---

## P1. Name the blocker-seam bad default (reflexive blocking) as a first-class diagnostic

QUOTE (Step 2, decision kinds): "attackers / blockers — is it playing its role? A
beatdown deck declaring 'none' over and over, or a control deck chump-racing, is
misplaying."

EDIT (append to that bullet): "The blocker seam has its own bad default, symmetric to
'Cast nothing' at the cast seam: offered a block, the executor BLOCKS — 'there is an
attacker, I have a creature' — regardless of role. Audit every `blockers` record against
the deck's plan: a racing deck that blocks is spending its clock; check what the blocker
was (win condition? token?) and what it blocked (a real threat? a 1/1 utility creature?).
A guide that says 'attack every turn' but is silent on blocking has NOT set the blocker
seam's default — silence there is a block-by-default."

WHY: recurred across 4 of my 12 games and both runs, including Guttersnipe (the deck's
entire kill) traded to a 1/1 Ohran Viper (ep1783915602 T16) and a token chumped into a
first-striker for zero effect (ep1783915599 T5). The wave-3 skill's only blocker note is
one clause, and its Step 4 skeleton says "Combat notes — only if the deck has non-obvious
combat," which reads as skippable; for any racing deck, blocking IS the non-obvious combat.

## P2. New diagnostic: partial answers to bundled asks (under-commitment inside one decision)

QUOTE (Step 2, diagnostic patterns list — no current entry covers this).

EDIT (add pattern): "**Partial answers to bundled asks.** Bundled decisions (attackers,
blockers) let the pilot answer with a SUBSET. Count, per attackers record, creatures
declared vs creatures listed: a pilot that repeatedly declares one attacker while three
are listed is losing damage invisibly — no single record looks wrong. The guide fix is a
count-and-match procedure ('count the list; your reply names all of them'), not a louder
'attack every turn' (which the pilot already satisfies with one attacker)."

WHY: ep1783920139 T31 — board had 2 Pyromancers + Guttersnipe + tokens, declaration was
"Guttersnipe" alone, repeatedly. The wave-3 executability audit checks whether windows
ARRIVE; now that bundled asks exist, the new leak class is using only part of the window.
"Attack every turn" was technically obeyed — which is why only a per-record subset count
catches it.

## P3. Attribute the residual: bound what a guide can fix, and route the rest upstream

QUOTE (Step 2): "separate three things a guess would blur: bad play vs.
never-had-the-option vs. bad hand (variance)."

EDIT (append a fourth output + procedure): "...and, across the whole corpus, attribute the
RESIDUAL. When the same deployed guide produces both faithful-execution WINS and losses,
diff them: if the wins are the guide working as written and the losses share a precondition
the guide cannot create (no threat drawn, color starved), the residual is DECK
CONSTRUCTION, not guide or model. The deliverable for that share is an explicit upstream
flag (manabase skew, threat density) plus guide text that teaches the deck to play the
hand it was actually dealt — not louder exhortations to execute a plan whose pieces
aren't there. A fast discriminator: opponent life at the moment of death. Losses ending
with the opponent near 20 mean the deck never had a plan running; check the hand/draw
stream before blaming the pilot."

WHY: this was the central judgment call of my review. deck131's 2 wins were textbook;
its 8 losses ended with opp at 13-27 life, and traced to 6 creatures + 14 Island/8
Mountain vs an all-red payoff suite. Without this step I would have written (useless)
stronger beatdown prose; with it, the guide got a real two-gear plan and the decklist
got a construction flag.

## P4. Two-gear identity keyed to ONE visible board fact (extends "role by matchup")

QUOTE (Step 1): "Role by matchup. State the DEFAULT posture in one line, then the narrow
condition that flips it, as IF-THEN keyed on the opponent archetype."

EDIT (append): "When a deck is a reactive shell around a NARROW win package (few
threat cards), the posture is decided less by the matchup than by which half of the deck
was DRAWN. Write the identity as TWO GEARS switched by one fact the pilot can see in its
own prompt ('is Guttersnipe or Young Pyromancer on your battlefield or in your cast list
right now? YES -> race gear; NO -> dig gear'), and write the passive gear as actions per
turn (cast draw spells to find the package, spend the mana), never as waiting. A
single-identity guide ('you are the beatdown in every matchup') is a lie in half the
deck's games, and the pilot durdles incoherently in exactly those games."

WHY: deck131's losses were the guide's beatdown identity colliding with threatless hands.
The matchup-keyed flip in the current skill couldn't express this — the flip condition is
the pilot's own draw, which IS visible in the prompt, unlike archetype inference.

## P5. Resolve the Step-4/Step-5 tension: the DO-NOT section should be positive-form

QUOTE (Step 4, skeleton item 9): "DO NOT — a tight list of the 3-5 specific misplays you
actually observed, each as a flat imperative phrased as the exact situation qwen was in."

EDIT (replace): "OBSERVED-SITUATION RULES — the 3-5 specific misplays you actually
observed, each restated as what TO DO in that exact situation ('when a Mountain and an
Island are both in your hand, choose Play Mountain'), phrased as the situation qwen was
in. Reserve literal DO-NOT/NEVER phrasing for the rare case with no positive equivalent,
and then follow Step 5's brake rules (scope + release + exception named inside it)."

WHY: Step 5 already teaches "prohibitions over-fire — prefer positive sequences," but
Step 4's skeleton then prescribes a DO-NOT section, and authors follow the skeleton. The
deck131 A/B is direct evidence negative framing bleeds beyond its target: a negatively
framed draw-spell clause suppressed draw casts (1 -> 8 per 6 games after positive
rewrite), and the fix held this wave (12 draw casts / 12 games). My wave-4 guide converted
the entire inherited DO-NOT section to positive equivalents with no loss of content.

## P6. Every standing preference rule needs a countable completion condition

QUOTE (Step 5, brake rules): "A brake that must exist needs (a) a SCOPE keyed to something
visible on the board, (b) a numeric FIRE TRIGGER / release..."

EDIT (add a sibling bullet at the same level): "The same applies to standing PREFERENCE
rules, not just brakes: any 'prefer X over Y' rule runs FOREVER unless you give it a
countable completion state on the board ('choose Play Mountain — until you have two
Mountains on the battlefield; Islands after that'). An unterminated preference is obeyed
past its purpose and starts costing in the other direction. Key the terminator to a count
the pilot can take from the printed battlefield, not to a judgment ('enough red')."

WHY: wave-3's Mountain-first rule worked but was open-ended ('you only need 1-2' is
advice, not a terminator); on a 14-Island deck an unterminated red preference eventually
mis-sequences blue-heavy hands. Cheap, general, and the pattern (numeric release) already
exists in the skill for brakes — this just extends it to preferences.

## Not proposing (considered, rejected)

- Updating the executability-audit EXAMPLE (deck131's zero priority windows is now fixed
  engine-side): the audit's method is unchanged and the example still teaches it; a
  synthesis-level footnote "(since fixed)" is enough. P2 covers the audit's new frontier.
- A lifegain/unwinnable-matchup section: deck131's vs140 timeouts are real but the lesson
  (some matchups are lost at deck construction) is already covered by P3's residual
  attribution; a matchup-tree rule would violate the skill's own CUT list.
