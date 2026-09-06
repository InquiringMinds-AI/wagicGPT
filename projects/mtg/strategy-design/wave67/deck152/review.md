# Wave 67 - deck 152 (Bant Human Midrange) per-deck review

Record 4/6. Seats read: the six `*deck152-*-vs-*` translogs in
`/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-102638/`, plus the deck130 and
deck123 opponent seats for context. 251 decisions, 2 fallbacks, 0 `unparsed_reply`,
0 hung/crashed games. All seq citations below are from the deck-152 seat file unless the
opponent seat is named.

## 1. Game by game

**152 v 126 - WIN 34/-1, turn 11** (`1788708415-...deck152-...-vs-...deck126`). Clean curve
(Adversary t5, Sigarda t7, Katilda + second Adversary t9). Deciding decision seq 22: with
*Sanguine Bond* alone on their line and their battlefield at 0 creatures, the seat declared all
four attackers for 19 into 18 and wrote the right reason - "no creatures to block, so the
converter does not trigger". That is the one-converter-name rule of MATCHUP POSTURE executing
correctly, in the exact shape earlier corpora lost to.

**152 v 162 - WIN 26/-31, turn 15**. Deciding decision seq 19-20: *Fateful Absence* held as an
instant and fired in their DRAW step at **Ob Nixilis, the Hate-Twisted**, the planeswalker the
REMOVAL section names as Absence's one job. Board then snowballed to the seq-44 ten-attacker
alpha (37 power into 2). The seat also correctly declined Intrepid Adversary's valor counter
(seq 6, 11) with no spare mana.

**152 v 146 - WIN 24/0, turn 20**. Deciding decisions: seq 19 block B2:A1 (Elite Spellbinder
kills Nadaar and lives - ladder rung 1, taken); seq 30 `ATTACK: A1>W1` sending Sigarda at Lolth,
Spider Queen; seq 37 the lethal three-attacker alpha at their 12. No ladder or A-line stop was
overridden anywhere in the game.

**152 v 125 - WIN 20/-1, turn 21**. Survived TWO *Supreme Verdicts* by re-deploying every main
phase (seq 22 Ranger Class into an empty board, seq 30 second Ranger Class, seq 47 Sigarda,
seq 50/52 both Brutal Cathars) - #3 RULE's deploy floor doing exactly what it is for. The HOLD
row skipped 53 windows in this game (`hold_windows_skipped` on the gameend record).
Two fallbacks, both benign: seq 16 `engine_answered` (a reveal with no legal target), seq 20
`index_name_conflict` - the reply wrote "CHOICE: 2 (Sphinx's Revelation)" against row 3; the
re-ask at seq 21 was answered correctly. Cost of the game: ~19 windows spent on two Teferi +1
activations (seq 36-43, seq 53-62) that between them tapped an already-`[tapped]` Tundra twice
and untapped one of the seat's own creatures - see HIGH-2.

**152 v 130 - LOSS 0/5, turn 22**. Not a guide loss. Deck 130 is mono-red land destruction: by
turn 21 the seat's OWN graveyard held Overgrown Farmland, Hengegate Pathway and two Barkchannel
Pathways (seq 31 battlefield line), it was stuck on 3-4 lands, and BOTH copies of Tovolar's
Huntmaster sat in hand printed `[cannot pay now: needs 6 mana, you have 3 untapped sources]` for
five turns. I checked seq 32 as a candidate misplay - attacking with the lone Moonrage Brute
under a header reading "you would be at 0; that would KILL you" - and it is NOT one: the
opponent seat's log (`1788708423-...deck130-...-vs-...deck152`, seq 64 prompt, turn 22) shows
*Hammer of Bogardan* killing the Brute in their main phase, so holding it back would not have
bought a block either. The last decision of the game (seq 33) is HIGH-1 below.

**152 v 123 - LOSS -119/8, turn 18** - the lopsided finish, traced in full.
The seat was AHEAD the whole game: 20 life to their 8, six creatures cast, two Bloodline Keepers
answered. It lost to one card sequence, and to one of its own decisions.
- seq 13-15 (t7), seq 21-22 (t11), seq 27 (t13): normal development; the Clue draws and the
  Ranger Class level were all taken on rows that named at most one creature in their
  `{paying this taps: ...}` clause, so #2a RULE was respected.
- **seq 17 (t9) is the deciding decision, and it is a STRATEGY miss against a rule the guide
  already states.** The *Tovolar's Huntmaster* row read
  `{paying this taps: Briarbridge Tracker, Katilda, Dawnhart Prime - they cannot attack this
  turn}` - THREE names - in MAIN 1. #2a RULE: "In Upkeep or Main 1, a row whose clause names TWO
  OR MORE creatures is PASS, always, whatever the row is: attack first, then take the same row in
  Main 2." The seat took it. The combat that followed (seq 18) offered exactly ONE A-line, Wolf
  #1 (2/2), against a lone Thraben Doomsayer, at their 15 life. Attacking first would have sent
  the 4/3 vigilance Tracker, Katilda and the Wolf into one blocker - roughly 5 damage - and the
  6/6 plus two Wolves would still have landed in Main 2 on the same mana. The game ended with
  the opponent alive on 8.
- seq 24 (t13) and seq 30 (t15): a Brutal Cathar each turn, each exiling a Bloodline Keeper. The
  engine chose both targets without an ask (the only legal target). Correct play, and the trap
  is real MTG: on t16 *Damnation* killed both Cathars, BOTH Keepers returned at once
  (seq 43 log: "Opponent's Bloodline Keeper entered the battlefield from exile (x2)"), and with
  *Intruder Alarm* already down plus *Lightning Greaves* equipped the Keeper made 21 Vampires in
  one main phase, 34 fliers by turn 18, 138 damage. This half is variance, not a guide error -
  but note the guide's Cathar line ("it COMES BACK if Cathar dies") does not price stacking a
  SECOND Cathar exile onto a board where a sweeper is a known card of theirs (Damnation was in
  their graveyard on the seq-30 screen, so a second copy was live).
- seq 37-42 (t17): with 23 creatures opposite, the seat cast Sigarda, then spent the rest of the
  turn on Teferi's +1 and got nothing from it - see HIGH-2. seq 43: `BLOCKS: none` into 34
  attackers, correct, nothing to be done.

## 2. Engine / interface / card items

**HIGH-1 - Moonrage Brute's ward is mis-described AND leaks the opponent's hidden zones.**
Repro: `1788708423-ai_baka_deck152-0x56004fd2cda0-vs-ai_baka_deck130.jsonl`, seq 33 (turn 22,
7 life, lethal incoming). Rendered header:
`TARGET CHOICE for Moonrage Brute - its "this spell costs 3 life more" ability (this spell/ability is already on the stack and needs a target ...)`
and then 21 rows, of which:
`2-4. Stone Rain [sorcery] [opponent's library] ... x3`, `6-9. Molten Rain [sorcery] [opponent's library] ... x4`,
`1. Spark Spray [instant] [opponent's hand] ...`, `16-20. ... [opponent's graveyard]`,
`21. Hammer of Bogardan [sorcery] [opponent's stack] ...`.
Twelve of the opponent's LIBRARY cards and one HAND card were printed to the model. That is
hidden information, and it is not a narration bug - it comes from the card script:
`bin/Res/sets/primitives/borderline.txt:73998` reads
`... target(*[instant;sorcery]|opponentzones) ...`, where `opponentzones` spans hand, library,
graveyard and stack. Ward should see only the spell that targeted the creature; `opponentstack`
is the existing zone token for that (198 uses in the same file). The same `opponentzones` pattern
is on every ward script in the file (lines 23962, 32049, 44450, 77467, 81349, 81420,
83434-83440, ...), so this leaks on any ward creature in the pool, either seat.
Second defect on the same ask: the ability is labelled as a *choice of target* put to the ward's
controller, when ward is automatic and the 3 life is the ATTACKER's to pay. The model believed
it was exiling Rorix Bladewing and paying 3 of its own 7 life ("Moonrage Brute costs {3} life to
activate, leaving me at 4 life") - every word of that false. It answered row 21 and the ward then
worked correctly (opponent seat log: "You chose Hammer of Bogardan's mode 'pay 3 life' / You lost
3 life (now 5)"), so this cost no game here; it burned the seat's last decision on a fiction.

**HIGH-2 - Teferi's +1 stage-1 gate carries no target information, and there is no pass row
after it.** Repro: `1788708435-...deck152-...-vs-...deck123.jsonl`, seq 39, 40, 41 (turn 17).
Each of the three windows renders the entire menu as two bare rows:
```
Choose an option for Teferi, Who Slows the Sunset:
1. choose a creature
2. Decline - do nothing
```
No count, no names, no "of theirs / of yours", no indication that TAP vs UNTAP is a later
window. The seat's stated plan named its target ("tap Bloodline Keeper #1 (the one with
Lightning Greaves)") and it still declined all three gates, spending 39 s + **357 s** + 31 s
(`deadline_pct` 4.4 / 39.6 / 3.4) to answer nothing. The information the model needed exists one
window later - seq 39 of the v125 seat prints
`choose opponent land [TAPS their land - it cannot be tapped again for the rest of THIS turn. But it UNTAPS in THEIR untap step ...]`
- but reaching it is irreversible: stages 2 and 3 both print "this ask has no pass row". So the
seat is asked to commit before it is told anything, and the v125 seat duly committed into a
stage-3 list whose every row was dead: seq 40 and seq 57 offered six then eight opponent lands,
ALL printed `[tapped]`. Fix: put the live-target census on the stage-1 row itself
("choose a land - 6 of theirs, all [tapped]; 8 of yours, 3 tapped"), or restore a decline row at
stage 2/3.

**MED-3 - the +1's three picks arrive in an order the render never states, and it is not the
card's order.** v123 seq 39/40/41 = land, creature, artifact. v125 seq 38 (land), 42 (creature),
43 (artifact). Nothing on the screen says how many gates are still coming or which, so a model
that declines gate 1 meaning "not the artifact" has already answered the land. Printing
"pick 2 of 3: creature" on the header would cost one line.

**MED-4 - a stage-1 gate for a permanent type the opponent does not have is still asked.**
v125 seq 59: "choose a creature" was put up with the opponent at zero creatures; the stage-3 list
(seq 60) held only the seat's own three bodies. Suppressing the gate when the polarity the model
wants has no legal object, or naming the polarity on the gate, removes a whole window.

**LOW-5 - `later_answer_ignored` at a decision that mattered.** v123 seq 37 (t17, 275 s,
`deadline_pct` 30.5): the reply ran past its answer, argued with itself
("Wait, actually, Sigarda enters tapped? No, she enters untapped."), and emitted a second CHOICE
line; `ignored_line: "CHOICE: 1 (Cast Sigarda, Champion of Light)"`, `parse_note:
later_answer_ignored`. First-wins was correct here (both lines agreed), so this is only a
data point for lane AV: the shape survives, and it survives on the longest replies.

**LOW-6 - overrun rate at this deck's seats is 204/251 (81%)**, above the corpus 71%. The
overruns cluster on windows where the model is doing arithmetic in the PLAN line (seq 30/32 of
the v130 seat are 200+ word mana recomputations of a cost the row already priced
`[cannot pay now: needs 6 mana, you have 3 untapped sources]`).

## 3. Guide verdict: EDIT (narrow)

Two changes, both paid for by a cited decision, and one of them corrects a statement in the live
guide that is now FALSE against the render. Revised guide: `wave67/deck152/strategy.txt`
(70,890 bytes, in the 41-71.5 KB band).

**Edit 1 - the TEFERI +1 paragraph.**
BEFORE: *"The +1 asks you in sequence for up to one artifact, up to one creature and up to one
land, each as its own 'choose your X' / 'choose opponent X' pair ... The prompt does not print
those two verbs anywhere - the card text on the row is cut off before them, every render - so
read them here and do not re-derive them from the menu labels."*
AFTER: the ask is described as the three-stage cascade it actually is - stage 1 the only window
that will accept a decline, stage 2 the polarity pair whose brackets DO print both verbs in full,
stage 3 the named list, neither of the last two carrying a pass row - with the order stated as
engine-chosen rather than artifact-creature-land.
PAID BY: v123 seq 39/40/41 (three stage-1 declines against the seat's own stated target, 428 s
of latency, a turn's decisions at 17 life) and v125 seq 38-40 / 55-57 (gate taken, then forced
onto an all-`[tapped]` list). The "does not print those verbs" sentence is refuted verbatim by
v125 seq 39. A guide sentence that tells the model the screen is lying, when the screen is not,
is the one class of edit worth spending bytes on.

**Edit 2 - MOONRAGE BRUTE's ward ask.** Added under the WEREWOLVES section and as one
DECIDING SITUATIONS line: the "this spell costs 3 life more" TARGET CHOICE is the ward, it costs
you nothing, most of its rows are cards not in the game, and the answer is the single row printed
`[opponent's stack]`.
PAID BY: v130 seq 33 - the seat's last decision of the game, answered on a rationale in which
every clause was false.

**NOT edited, deliberately.** The seq-17 Huntmaster miss (the game's deciding decision) is
already covered word-for-word by #2a RULE, including a "WHAT THIS COST" of the same shape.
Adding a second cost paragraph for a rule that is stated, exampled and simply not followed is
restatement, not instruction. Same for the seq 40/57 `[tapped]` picks: the bullet exists
("A target printed '[tapped]' is a wasted pick"); what was missing was WHERE to stop, which
Edit 1 supplies in one clause.

No general-guide proposal and no skill proposal: nothing here generalises past this deck's two
cards, and both findings are engine-shaped rather than doctrine-shaped.

## 4. What I did NOT check

- I did not read the other five decks' seats, nor the corpus-wide counters (`unparsed_reply`
  shapes, the 1233 s record, `reveal_stall_forced`, the reservation-decline latch) - engine seat.
- I did not adjudicate any lane AV/AW/AX/AY/AZ prediction; where a lane's mechanism showed up in
  these seats (first-wins at v123 seq 37, the HOLD row's 53 skipped windows at v125) I recorded
  the observation without scoring it.
- I did not verify Moonrage Brute or Brutal Cathar against Scryfall; the primitive's own `text=`
  line matches the Oracle wording I relied on ("Ward-Pay 3 life", "exile target creature an
  opponent controls until this creature leaves the battlefield"), and the defect I report is in
  `auto=`, not in the printed text.
- I did not test the proposed `opponentzones` -> `opponentstack` change (no edits under
  `bin/Res`, no build, per the brief); it is a reading of the script plus the zone token's 198
  existing uses in the same file.
- Mulligans: all six games kept the opening hand at the first or second look, so this corpus
  exercised none of RULE -1's low-M branches.
