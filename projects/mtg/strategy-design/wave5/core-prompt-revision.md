# Core-prompt revision (wave 5) — exact edits for `bin/Res/ai/gpt/system_prompt.txt`

`system_prompt.txt` is byte-identical to `general-strategy.txt`, so the revised
`wave5/general-strategy.txt` IS the new core prompt; apply these three edits to
`bin/Res/ai/gpt/system_prompt.txt` to bring it in line. The core prompt is an
ENGINE-OWNER file — this synthesis does NOT edit `bin/Res`; it hands the owner the
exact old→new. All three cleared the convergence gate (2+ decks) or complete an
existing core line; no other core changes are made this wave.

Verify after applying: `git diff --no-index bin/Res/ai/gpt/system_prompt.txt strategy-design/wave5/general-strategy.txt` should be empty.

---

## Edit 1 — RETIRE the counterspell "offer-is-the-signal" stopgap; point at the shipped ON THE STACK section; add the mana-discipline fact

The wave-4 core carried a STOPGAP for stack-invisibility: a tell that derived the pending
spell from the game-log tail ("the last `hand -> stack` line with no `stack ->` after it")
and a falsity-fact ("the thought 'there is no spell on the stack' is always false while the
option is listed"). The engine has since SHIPPED the stack as a first-class CURRENT SITUATION
section ("ON THE STACK, waiting to resolve...") with controller/name/tag/targets, and counter
options are now labeled with what they can hit ("Cast Counterspell - can target on the stack:
Master of Etherium"). The stopgap's removal condition is MET — retire it and point at the real
representation. Also fold in the wave-4-E3 finding (NOT a bug: a response window only opens when
a response is PAYABLE; a seat that tapped out on its own turn gets no window on the opponent's
turn) — so "died with counters in hand" is a MANA-DISCIPLINE leak.

OLD (the `- Pure reactions ...` sub-bullet):
> - Pure reactions (counterspells, combat tricks, removal that cannot hit a player): these need a TARGET, so never fire one at nothing just to spend mana - but the OFFER IS THE SIGNAL: you never need to guess when the moment to respond is; the game opens a window and asks you, and every listed action is legal right now. When "Cast <a counterspell>" is offered during the opponent's turn, an opponent spell IS on the stack at this moment waiting to resolve - that is the only reason the option can appear. Find it: it is the last game-log line reading "hand -> stack" with no "stack ->" line after it; judge whether THAT spell is worth countering. The thought "there is no spell on the stack" is always false while the option is listed. A reaction is wasted only when you fire it at a target that does not matter - never by trusting an offered window. Holding a card must never hold your TURN - keep casting your other spells. Spend a held answer NOW when the target you were waiting for is already on the battlefield or stack, when the threat it could answer is growing each turn, or when your life is 10 or less - dying with answers unspent in hand is worse than spending them early. If the opponent has gone two of their turns without casting anything worth a reaction, stop reserving mana for one: develop and attack instead.

NEW:
> - Pure reactions (counterspells, combat tricks, removal that cannot hit a player): these need a TARGET, so never fire one at nothing just to spend mana. When spells or abilities are waiting to resolve, the CURRENT SITUATION shows an "ON THE STACK" section naming each one, its controller, and its targets (top resolves first, and you may respond now), and your reaction option names what it can hit ("Cast Counterspell - can target on the stack: Wrath of God"). Read that section and the option label, and judge whether the named spell is worth answering; a reaction is wasted only when you fire it at a target that does not matter. But a reaction can only be USED when the game gives you a window, and it gives you one only while you can actually PAY for a response: if you tap out on your own turn, no window opens on the opponent's turn and your held answer rots in your hand. So when you are keeping a counter or instant for the opponent's turn, LEAVE its mana untapped - deploy the rest of your hand, but keep the reaction's mana open. Holding a card must never hold your TURN - keep casting your other spells. Spend a held answer NOW when the target you were waiting for is already on the battlefield or stack, when the threat it could answer is growing each turn, or when your life is 10 or less - dying with answers unspent in hand is worse than spending them early. If the opponent has gone two of their turns without casting anything worth a reaction, stop reserving mana for one: develop and attack instead.

Rationale / evidence: deck44's counter seam fired 2/27 windows because the pending spell lived
only in the log tail and the pilot fabricated a false resolution over it (`e1248 s3`, `e5169 s30`,
`e8528 s26`); the ON THE STACK line + labeled option is the direct representation fix and is now
LIVE. Retiring the tell (rather than deck44's proposed *extension* of the falsity-fact with a
second wrong phrasing) is correct because the tell is obsolete, not under-worded.

---

## Edit 2 — COMPLETION: the act-floor is satisfied by the HIGHEST-impact play; deploy the threat before same-turn value/ramp/dig (convergence: deck135 G1 + deck133 #2 + deck131)

Append to the end of the `- Your own turn is for ACTING.` bullet.

OLD (bullet ends):
> ... Do not cast redundant copies just to empty your mana - but a copy is NOT redundant if it does something when it arrives (draws a card, makes mana, adds to a count your deck cares about) or if your guide says to cast every copy.

NEW (bullet ends):
> ... Do not cast redundant copies just to empty your mana - but a copy is NOT redundant if it does something when it arrives (draws a card, makes mana, adds to a count your deck cares about) or if your guide says to cast every copy. The act-floor is satisfied by your HIGHEST-impact legal play, not the cheapest one to hand: when your main phase offers BOTH a creature or threat AND a lower-impact value spell (a cantrip, ramp, a fetch, a discard spell), cast the THREAT this turn and spend any leftover mana on the value spell - a body on the board advances your plan, a dig only helps you find one, and casting a cheap spell "to do something" while your win conditions sit in your hand is durdling, not acting.

Rationale / evidence: three seats show the same shape — casting a low-impact value spell to
"satisfy the act-floor" while the win conditions sit in hand. deck135 (durdle on Search/fetch/
Astrolabe over a body; opp ended 18–23 in every loss), deck133 (discard-flood: casting the
cheapest Thoughtseize/Inquisition while raced — deck133 general-suggestions #2 explicitly
predicted a core line justified at 3+ decks), deck131 (Prism Ring value-sink in every durdle
loss). It COMPLETES the existing "deploy a threat ... Deploying outranks saving" + "choose by
IMPACT" lines rather than adding new doctrine.

---

## Edit 3 — Name the ON THE STACK section in the "During the game you will receive" paragraph (support for Edit 1)

OLD:
> During the game you will receive the game log (everything that has happened so far), the current board state (each card's current power/toughness, counters, and anything attached to it), your own last stated plan, and the choices that are legal for you right now.

NEW:
> During the game you will receive the game log (everything that has happened so far), the current board state (each card's current power/toughness, counters, and anything attached to it) - including, whenever spells or abilities are waiting to resolve, an ON THE STACK section (top resolves first; you may respond now) - your own last stated plan, and the choices that are legal for you right now.

---

## NOT adopted for core (routed elsewhere)

- **deck44 #1** (extend the counter falsity-fact with the second wrong phrasing "the spell already
  resolved / moved to the battlefield"): OBSOLETED by Edit 1. The falsity-fact machinery is retired,
  not extended — the stack is now shown, so there is no tail to fabricate over.
- **deck109 #1** (reflexive high-life blocking → make "above 8 life, no blockers is the default" a
  sharper standalone core sentence): SINGLE-SEAT. Only deck109 reported it (deck131/135 blocked
  correctly; deck110/44 did not flag it). Does not clear the convergence gate. Kept per-deck
  (deck109 added its own 8-life blocker floor) and put on the wave-6 CONVERGENCE WATCH: if 2+ racing
  reviewers report reflexive high-life blocking next wave, promote to a standalone sentence at the
  blocker seam (representation/placement, not new doctrine).
- **deck131** Guttersnipe cast-triggers-payoff exception ("a reaction with no target is still 2 to
  the face while Guttersnipe is out"): PER-DECK by construction — only deck131 runs such permanents.
  Revisit if future decks add Talrand/Firebrand-Archer/Murmuring-Mystic-class cards.
