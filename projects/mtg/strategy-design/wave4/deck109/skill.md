# Skill-revision proposals from the deck109 wave-4 cycle

Source experience: deck109 review (12 games, 2 runs, 8/12) + guide revision. Central fact of
this cycle: **all four deck109 losses were engine/harness defects** (reply-parser digit-grab;
Hellrider cast no-op), found from translogs alone, while the wave-3 behavioral leaks were
measurably fixed. The wave-3 skill's Step 2 is built to find MODEL misplays; it was silent on
how to detect ENGINE misplays, on what a guide should do about them, and on when to demote a
rule the pilot now obeys. Proposals below are edits against
`strategy-design/wave3/strategy-writing-skill.md`, formatted quote → change → why.

---

## P1. Step 2 digest recipe: mandate the reply-vs-consumed-choice check (desync detection)

**Quote (Step 2, "Extract, don't eyeball"):** "…prints, per decision: the `kind`, the phase,
life totals, hand, both battlefields, the numbered options, the TEXT of the CHOSEN action
(regex the `choice`-indexed line out of the `prompt`), and a snippet of the `reply`…"

**Change (append to that sentence):** "…and a DESYNC FLAG: compare the first standalone
integer at the head of the `reply` against the recorded `choice` (translogs now carry
`chosen_text`, so the consumed action is a stored fact, not a regex). Any record where they
differ is a harness defect, never a model decision — collect them all before judging play
quality, and attribute those plays to the harness, not to the pilot or the guide."

**Why:** deck109 had 8 such desyncs across 5 games, one of them game-losing (exact-lethal
Pillar consumed onto a 1/1 instead of the face, r1 epoch 1783905418 s24). The mechanism (parser
grabs a digit from echoed option text — `{2}{r}{r}`→2, `(3/3)`→3, "life 2"→2) was only
findable by this per-record comparison. Without it a reviewer scores these as misplays and
writes guide text against a non-problem.

## P2. Step 2 diagnostic patterns: add "re-selection with no state change = engine no-op"

**Quote (Step 2, diagnostic patterns list):** "- **Offered-many-times, passed-every-time** — a
capability qwen doesn't understand it should use."

**Change (add a sibling pattern):** "- **Chosen-many-times, HAPPENED-never (engine no-op).**
The same action selected repeatedly (correctly parsed) while the game state never reflects it.
Verification anchors, all in the record itself: (a) the `Mana available:` count is IDENTICAL
before and after the 'cast' (same turn, MP1 vs MP2) — a real cast spends mana; (b) the GAME LOG
never shows the card's `hand -> stack` transition — the log prints every real cast as
`hand -> stack` then `stack -> battlefield`; (c) the card is still in the hand line next
decision. Two or more consecutive no-ops of the same pick = engine bug: stop counting it as
passivity, find a game where the SAME action resolved (the A/B repro), and file it."

**Why:** deck109's "Cast Hellrider" no-op'd 16x in one loss and 43x in another while the model
did exactly what the guide ordered; the same action resolved normally in a win (the A/B that
proves it's board-state-conditional). Without this pattern the obvious-but-wrong reading is
"the model durdled for 11 turns" and the obvious-but-wrong fix is louder CAST HELLRIDER text —
which is what caused the 43x loop in the first place.

## P3. New Step 5 rule: engine-defect mitigation — what the guide carries, and what it must not

**Quote (Step 5 header):** "## Step 5 — Word it for the executor (non-negotiable style rules)"

**Change (add a rule):** "- **When Step 2 found an ENGINE defect, the guide carries a
BEHAVIORAL FALLBACK, never an explanation.** The real fix is an engine bug report (file it
separately with the repro records). In the guide: do NOT mention that the engine is buggy, do
NOT restate game rules to 'correct' it, and do NOT prohibit the affected action — the executor
must keep taking it when it works. Instead give a positively-framed fallback keyed to state the
pilot can SEE: name the observable check (for casts: 'a real cast shows `hand -> stack` in the
GAME LOG; if your pick is still in your hand with your mana unspent…'), then name the
alternative action to take THIS decision, then permit a retry NEXT turn, with a cap ('one retry
per turn'). The fallback must degrade gracefully: when the engine bug is later fixed, the line
becomes harmless."

**Why:** deck109's revised guide needed exactly this shape twice (cast-fallback rule; reply
contract). The naive alternatives all fail: 'Hellrider is broken, don't cast it' loses the
games where it resolves; silence loses the 43-repick games; an explanation wastes the
attention budget on theory the executor can't act on. The `hand -> stack` anchor matters
because it converts 'did my cast work?' from a judgment into a look-up the weak model can do.

## P4. Inputs/Step 3: reply-format contracts are GENERAL-layer furniture — guides only reference them

**Quote (Inputs #2):** "**Your guide LAYERS ON TOP — never RESTATE any of this; add only what
is specific to THIS deck.**"

**Change (append to Inputs #2):** "The same layering applies to the INTERFACE itself: the
reply-format contract ('first character of your reply is the option number, bare number on its
own line, no option text or mana costs before it, then PLAN:') is a property of the harness,
identical for every deck, and its violation cost is parser desyncs (see the desync check in
Step 2). It belongs in the GENERAL system prompt, stated once with a RIGHT/WRONG example pair
— the wrong example being an echo of option text (`2. Cast Hellrider {2}{r}{r} (3/3)`), since
echoed costs/stats are exactly the digits the parser mis-grabs. A per-deck guide should carry
this block only as an INTERIM measure while the general layer lacks it, tagged for removal;
seven copies of interface rules is the restatement failure this section already bans."

**Why:** I wrote the HOW TO REPLY block into deck109's guide because that was my only writable
surface, but every observed desync (8 in deck109's logs alone) is deck-independent — the same
prompt suffix ("Reply with the number of your choice, then your PLAN: line") produces the same
violation shape in every seat. One general-layer block with the example pair fixes all seven
decks at once and frees ~10 lines of every guide's attention budget. Synthesis agent: if
multiple decks propose this, merge into ONE general-prompt suggestion and strip the per-guide
copies.

## P5. Step 3/Step 4: compression discipline for WON battles — an evidence threshold

**Quote (Step 3):** "Include a line ONLY if it changes a decision qwen will actually face and
gets wrong (or would)."

**Change (append):** "The same test governs REMOVAL on revision: a rule whose battle is WON
gets compressed, not kept at full prominence and not deleted. Demote a block to a 1-4 line
reference when ALL of: (a) obedience is quantified and total-or-near across the corpus (count
it: violations / opportunities, e.g. 0 delusion instances in 174 casting asks, 67/67 combats
attacked, 14/14 unleash taken); (b) the fix now lives at a stronger layer (a representation
line like `Mana available: N untapped sources`, or the general prompt) so the guide is no
longer the only defense; (c) the compressed line keeps the one load-bearing FACT (what to look
at), dropping the persuasion. Keep full prominence while either (a) or (b) is unmet. Delete
outright only what wave evidence shows the pilot never needed."

**Why:** deck109's 16-line '#1 RULE — COUNT YOUR MANA' block earned demotion to 4 lines this
wave (0 genuine delusions; the `Mana available:` line now carries the fact), and the freed top
slot went to the reply contract — the current biggest loss source. Without a stated threshold,
revisions only ever ADD, guides grow past one screen, and the skill's own 'length dilutes'
warning loses to loss-aversion. The threshold also protects against premature deletion: (b)
blocks cutting a rule whose obedience depends on the guide text still being there.

## P6. Step 2 attribution taxonomy: three buckets become four

**Quote (Step 2 opening):** "…separate three things a guess would blur: **bad play vs.
never-had-the-option vs. bad hand (variance).**"

**Change:** "…separate FOUR things a guess would blur: **bad play vs. never-had-the-option vs.
bad hand (variance) vs. HARNESS DEFECT (the pilot chose right and the system did something
else — desync, no-op, misrender).** The `prompt`, `reply`, `choice`, and `chosen_text` fields
together resolve all four as fact. Only bad play is the guide's to fix; harness defects go on
the engine bug ladder with repro citations, and the guide gets at most a mitigation (Step 5
engine-defect rule)."

**Why:** this is the one-line version of what this whole cycle taught. Deck109's review would
have produced a completely wrong guide (louder Hellrider text, anti-self-targeting rules) if
the losses had been attributed inside the old three-bucket taxonomy, which structurally assumes
the system faithfully executes the model's pick.

---

## What the wave-3 skill got RIGHT for this cycle (keep; evidence)

- **Extract-don't-eyeball + read-the-reply**: both engine bugs were found exactly this way;
  the digest loop paid for itself again.
- **Guide-echo as attribution evidence**: the pilot echoed the mana rule back correctly
  ("lands untap at start of turn, attacking doesn't tap lands", w44 r1 s12) — echo validated
  the line AND justified its compression (P5).
- **Perception procedures beat rules statements**: the wave-3 mana fix worked; its success is
  the model for P3's `hand -> stack` anchor (give the pilot a thing to LOOK AT).
- **Clock audit on wins**: still worth running; deck109's wins closed mostly on-curve this
  wave, which is itself the evidence the wave-3 fixes landed.
- **Stress-test absolutes / named exceptions**: the Obliterator matchup line went in as a
  named-card rule per this discipline; no change needed.
