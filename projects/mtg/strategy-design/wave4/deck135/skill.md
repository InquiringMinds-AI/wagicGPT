# Deck-135 proposals: revisions to the strategy-writing skill (wave 4)

Format per item: QUOTE (wave-3 skill passage) → EDIT (replacement or addition) →
WHY (one line, deck135 evidence). Ordered by how much each would have changed what
I actually wrote this wave. All citations: R1 = matchups-20260712-192832,
R2 = matchups-20260712-230636, deck135 seat translogs.

---

## P1. Over-correction is a first-class wave failure mode; every PURSUIT rule ships with a countable completion condition

**QUOTE (Step 0):** "Suspect your own defaults. Any default the old guide
prescribed ('fetch Forest by default,' 'hold X for Y') is a candidate CAUSE of the
new failure..."

**EDIT (extend the same bullet, and add a matching Step 5 requirement):**
- Step 0 addition: "Audit last wave's #1 RULE specifically for OVERSHOOT: the
  louder a fix, the more the executor over-applies it. A cured leak often returns
  inverted — measure whether the fixed behavior now runs past its purpose (count
  decisions spent on it per game vs. decisions spent on the win condition)."
- Step 5 addition (sibling of the brake rules): "Brakes need releases — and so do
  PURSUITS. Any rule that tells the pilot to seek/acquire/prioritize something
  ('get an Island,' 'build to 4 lands,' 'assemble X') must carry a COUNTABLE
  completion condition stated in the rule itself ('ONE Island or one Astrolabe in
  play is ENOUGH — after that, every fetch takes Forest and your turns go to
  attacking'). A pursuit without a done-state becomes the deck's whole
  personality, exactly like a brake without a release."

**WHY:** wave-3's loud "GET AN ISLAND BY TURN 3" cured the blue screw and became
wave-4's durdle — 15/65 (R1 vs44) and 16/59 (R2 vs109) of all decisions were
fetch-activations hunting Islands while creatures sat home; the deck owns 2
Islands total. The skill's release requirement currently covers only brakes/holds.

## P2. The act-floor generalizes: every decision KIND with an observed passivity leak gets its own floor, written as a per-item checkable procedure over the offered list

**QUOTE (Step 5):** "The act-floor comes first, above any hold language...
'cast a spell every turn; a turn where you cast nothing is a misplay unless
[closed exception].'"

**EDIT (generalize the bullet):** "The act-floor comes first — and it is
PER-SEAM, not global. 'Cast every turn' does not transfer to the attackers seam:
a pilot obeying the cast floor can still declare 'no attackers' forever. For each
decision kind where the logs show passivity (casting, attackers, activations),
state that seam's own floor AS A CHECKABLE PROCEDURE the pilot runs over the
offered list — one yes/no test per listed item with the answer's consequence
('a creature attacks if ANY of: no untapped blocker / toughness > biggest
blocker's power / it trades up or draws a card'), plus a closed exception for the
empty choice ('no attackers' ONLY when every listed attacker dies for zero
gain). Posture prose ('be aggressive,' 'attack every turn') loses to the first
plausible friction; a per-item test does not."

**WHY:** deck135 had the cast floor and still lost every passive game at the
ATTACK seam — wins declared 3-8 real attacks, losses/timeouts 0-1; R2 vs140 froze
all attacks for 20+ turns over Lightmine Field despite a guide line saying
"attack every turn" (seq 44, 59, 74).

## P3. Verify the MECHANISM you are about to teach against the `auto=` line — including your own causal claims, not just cited numbers

**QUOTE (Inputs #3):** "Oracle text you remember, the printed cardface, and the
engine `auto=` script can all DISAGREE — the script wins. Verify the exact
numbers you will cite against it."

**EDIT (strengthen scope):** "...Verify the exact numbers you will cite against
it — AND every MECHANISM claim: who a trigger hits, what a restriction scopes,
which zone/step it fires in. This applies to your OWN analysis in findings, not
only to guide sentences: an author's rules intuition and the executor's stated
beliefs are both unreliable, and a reviewer who adopts the pilot's (or their own)
mechanical story without reading the `auto=` will ship a guide rule teaching a
false exemption. Litmus: for each 'X avoids/only affects Y' claim in your draft,
point to the `auto=` token that says so."

**WHY:** my own findings claimed Diamond Faerie "dodges Lightmine" (fliers
exempt) — Lightmine's `auto=@each blockers:foreach(creature[attacking]...)
damage:1 all(creature[attacking]...)` hits ALL attackers; the check at
guide-writing time caught it, but only because I re-read the `auto=`. The current
skill wording reads as "verify numbers," which I had done.

## P4. Name the interface-contract anchor as a standard TOOL against false-belief vetoes

**QUOTE (Step 5):** "Teach the mechanic qwen provably lacks — as fact, then
refute the false belief verbatim... Reinforce one in the deck guide only when the
new logs show it still losing, and then tie it to the specific card..."

**EDIT (add a named pattern to that bullet):** "The strongest corrective anchor
is the INTERFACE CONTRACT, restated as a property of the visible list tied to the
named card: 'when Cast <card> is in your list, the engine has already checked —
you CAN pay, and it IS your main phase.' This converts an unwinnable argument
about the world-model ('I have no mana,' 'it is the opponent's turn') into a fact
about the thing the pilot is looking at. Prefer it over re-teaching the
underlying rule (pools, phases, turn structure) — the pilot doesn't need to
understand mana to trust the list."

**WHY:** R2 vs140 — "Cast Diamond Faerie" offered as a numbered option 22 times,
taken 0, with the refusals reasoning "I have no mana" (seq 35, 36, 39) and
"wrong phase; it is the opponent's turn" while a sorcery-speed cast sat in the
list; the general prompt's abstract version of the contract was present and
losing.

## P5. Scope triage: pre-computable → guide; runtime-computation failures → model-experiments track

**QUOTE (Step 3, CUT list):** "CUT: anything the general prompt already covers;
general Magic theory; ... anything you can't turn into a 'do/don't with a named
card'..."

**EDIT (add a triage rule to Step 3):** "Model-capability failures get triaged by
one question: can the guide PRE-COMPUTE the answer so the pilot needs no runtime
reasoning? If yes, it is guide material — do the arithmetic in the rule itself
('one attacker = 1 damage total; Viper 1/3 survives and draws you a card'). If
the pilot would still have to compute/compare/parse at runtime (degenerate
output, illegal assignment syntax like 'B1:A1, B1:A2', arithmetic over a quoted
card it must perform fresh each time), do NOT spend guide lines on it — route it
to the model-experiments residuals list as a litmus case. Guide lines against
model incapacity are dead weight that dilutes the lines that work."

**WHY:** deck135's Lightmine failure looked like a capability wall (retrieves the
card, fails the comparison — both waves, opposite directions) but was
guide-fixable by pre-computing the solo-attacker case; the "** **" degenerate
blockers reply (R1 vs109 seq 17) and the illegal double-block (R1 vs44 seq 35)
are not, and belong in the residuals track.

## P6. Resolve the quote-and-forbid vs. negative-framing-bleeds tension (flag for synthesis)

**QUOTE (Step 5):** "...quote-and-forbid the recurring wrong thought by name
('NEVER pick Cast nothing with the excuse I have no mana — that excuse is always
false when a cast is offered')..."

**EDIT (constrain, don't delete):** "Quote-and-forbid collides with the proven
negative-framing-bleeds lesson. Keep it under three constraints: (1) it FOLLOWS
its positive rule in the same block, never leads; (2) phrase it as a FALSITY FACT
about the thought ('these thoughts are ALWAYS FALSE when a Cast option is
listed: ...'), not as a DO-NOT imperative; (3) at most one such line per rule.
[SYNTHESIS: I used two falsity-fact lines in the deck135 guide's Rule #2 and
judged them load-bearing against the 22/0 Diamond Faerie refusal — other decks'
evidence should decide whether even the falsity-fact form suppresses adjacent
positive text.]"

**WHY:** the wave lesson "a clause phrased as 'do NOT save them...' suppressed
the positive instruction next to it" is method-level, but the wrong-thought
interception is field-proven too; the skill currently carries both without
acknowledging they pull apart.

## P7. Update Inputs #4 to the wave-4 translog schema, and require option-LINE counting in the tallies

**QUOTE (Inputs #4):** "Fields: `kind` (ask|priority|attackers|blockers),
`prompt` ..., `reply` ..., `choice` (the index it picked). (`options` is just a
count — ignore it.)"

**EDIT (replace):** "Fields now include `chosen_text` (resolved text of the
chosen option — use it, don't re-derive from `choice`), `turn`, `phase`,
`my_life`/`opp_life`, `latency_ms`, and `kind` gains `gameend`. For the
offered-vs-taken tally, count a card as OFFERED only when it appears in a
NUMBERED OPTION LINE (`^\\d+\\. ...`) of the prompt — never by substring over the
whole prompt: card names recur in the narration, the carried PLAN, and battlefield
lines, and substring counting silently corrupts the tally in both directions.
Distinguish 'Cast <card>' offers from activation/targeting mentions of the same
card."

**WHY:** my first digest matched substrings and produced inverted/garbage
offered-vs-taken numbers (e.g. "Force 6/48"); the corrected option-line count
(Diamond Faerie: offered 22, taken 0) became the wave's headline finding — the
skill's tally instruction, followed naively on the new schema, mis-measures.

---

## What the wave-3 skill got RIGHT for me (keep, no edit)

- **Step 0 "expect the failure to have MOVED downstream"** predicted this wave
  exactly (blue screw fixed → close-the-game became the leak); it is the reason I
  looked past the fixed Rule #1 instead of re-polishing it.
- **The win/loss mirror lever** (compare wins to losses for the one flipped
  variable) again produced the headline in minutes: attack counts split cleanly
  by outcome.
- **"Confirm before you accuse"** — reading full prompts before judging saved me
  from mislabeling the correct block-all-to-survive play (R2 vs109 seq 35) as
  over-blocking, and surfaced that the survival math sometimes WORKS (which
  changed the fix from "teach the sum" to "trigger the sum before attacks").
- **Executability audit** — attacker/blocker seams now demonstrably fire (9
  blocker decisions this corpus vs 3 in 21 games), so posture rules about
  blocking are now writable; the audit is what licenses that change.
