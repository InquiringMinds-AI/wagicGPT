# deck152 -> core-prompt / render proposals (wave 56), layer-routed

## R-5 (HIGH, render - it is the corpus's cleanest positive) - keep the combat LOOP clause, and
extend it to the same shape outside combat.
EVIDENCE. `152v126` seqs 16/19/24/32/40/46: every held A-line carried
`(their life LOOP is in play: any life they gain or you lose in this combat chains without limit -
fatal to you, ...)`. The seat declined all-out attacks six windows running with plans that
re-derive the loop correctly ("they gain life, which triggers Sanguine Bond to drain me, which
triggers Exquisite Blood ..."), and did attack with an unblockable flier at s32/s40 before the
clause's own reading told it to stop. It rendered 5 times, all at my seats, and it is doing exactly
what the perception-vs-strategy doctrine says a render annotation should: it replaced a rules
derivation the pilot cannot be trusted to do in a 26 KB prompt.
ASK: the clause is currently combat-scoped. The same two permanents make any lifegain or lifeloss
lethal on ANY window, including a lifelink cast or an activated drain in Main 2. Emit the same
sentence on the CURRENT SITUATION board line whenever both halves of a life loop are on the
opponent's battlefield, not only on attacker rows.

## R-6 (MED, render) - `Their untapped sources: N (colours they could make: ...)` closes wave-55's
E-4 and should be kept exactly as it is.
EVIDENCE. It renders on **2,718 of the corpus's prompts, 677 of mine**. Wave 55's `152v125` lost a
65-turn game to eight creature spells countered one at a time, each cast with 5-8 unmarked opponent
permanents that had to be counted by scanning for the ABSENCE of a `[tapped]` marker. This corpus's
`152v125` was won 20/-1 on turn 15 and every creature cast in it was made at a printed count of
0-3. No change asked; recorded so the line is not traded away for bytes.

## R-7 (LOW, prompt bytes) - the modal-DFC land row prints the script token before the printed name
EVIDENCE. Rows read `boulderloft pathway [PLAY THIS AS A LAND: puts "Boulderloft Pathway" onto the
battlefield ...]` and, in the priority-seam form, `- You used: boulderloft pathway with Branchloft
Pathway -> ...`. The lowercased script token is the row's LEAD and the properly-cased name follows
inside the annotation. The model echoes the row it picks, so the lowercase token is what lands in
`chosen_text` and in every subsequent narration line (`152v162` carries 54 renders of one such
narration line).
ASK: lead with the printed name. This is cosmetic for parsing (the echo binds either way - lane D
added the PARSETEST binds) but the narration is re-rendered on every later prompt, so the token
form is paid for once per prompt for the rest of the game.
