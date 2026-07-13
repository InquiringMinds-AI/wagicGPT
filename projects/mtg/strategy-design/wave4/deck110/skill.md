# Deck110 proposals for revising the strategy-writing skill (wave 4)

Context for the synthesis agent: deck110 is the turnaround case — wave 3 it was 0/6
(five timeouts) under a guide-caused paralysis; wave 4, after the wave-3 revision plus
the representation fix (`Mana available: ... from N untapped sources`), it went 9-2-1t,
best in the pool. So my proposals skew toward two situations the wave-3 skill
under-serves: revising a guide that WORKED, and defending against engine re-offer
loops. Every proposal below changed (or should have changed) what I actually wrote in
`wave4/deck110/strategy.txt`. Format: quoted wave-3 passage → proposed edit → evidence.

---

## P1. Waste loops need a named FIX PATTERN: done-signal + per-turn count brake + named release

**Wave-3 passage (Step 2 diagnostic list):**
> "**Waste loops** — the inverse of offered-and-passed: a repeatable-cost action
> re-taken every turn for no new effect (re-equipping an already-equipped creature 'to
> ensure the bonus is active'; untapping a summoning-sick creature). Burns mana AND clock."

and (Step 5):
> "Include state-recognition cues ('(5/1) (printed 1/1)' means the equipment is ALREADY
> attached)."

**Proposed replacement/addition.** Keep the diagnostic, but the skill currently stops at
"include a state-recognition cue" — and that exact cue WAS in the wave-3 deck110 guide
("If your creature's power already shows the bonus ... do NOT equip it again") and still
lost: wave-4 logs show 47 equips taken where ~12 were justified, including FIVE
consecutive re-equips at `1783903101` T19 and T23 with `(6/2) (printed 2/2)` printed on
the very board the model was reading. Add a fix-pattern rule to Step 5:

> **When the engine re-offers a completed action every window (equip, attach, repeat
> activation), a lone state-recognition cue does not hold. Write the defense as THREE
> parts, all mandatory: (a) a DONE-SIGNAL the executor can read off the board line
> itself, stated as a perception procedure ("two power numbers — like '(6/2) (printed
> 2/2)' — mean the bonus is already on it; the equip is DONE"), (b) a COUNTABLE
> per-turn brake ("take at most ONE Equip in a whole turn") that caps the damage even
> when (a) misfires, and (c) a NAMED RELEASE so the brake doesn't over-generalize
> ("equip again only when the equipped creature DIED or a strictly better carrier
> arrived"). State where the saved resource goes instead ("your remaining mana belongs
> to creatures and Blasts") — a brake with a positive destination out-competes the
> re-offered line; a bare prohibition just loses to it again.**

**Why.** The re-offer means the model is re-prompted with the temptation every single
priority window; a cue it must remember loses to an option it can see. The count brake
is the only piece that is robust to the cue being misread, and the release + destination
keep it from becoming next wave's paralysis (the skill's own deck110 cautionary tale).
Flag upstream too (see P3): the real fix is the engine suppressing/annotating no-op
lines — wave-3 findings #5, still unfixed after two waves.

## P2. Step 0: "audit last wave's rules — did each one get FOLLOWED?" needs the demote-by-working procedure spelled out

**Wave-3 passage (Step 0):**
> "A rule that now gets followed has done its job — DEMOTE and compress it (keep it,
> shorter; deleting it risks regression) and spend the reclaimed space on the new leak."

**Proposed addition (extend that bullet):**

> **Demotion is a three-part move, not a deletion-lite: (1) the proven rule loses the
> #1 slot but KEEPS its two load-bearing sentences verbatim — the corrective fact and
> the quote-and-forbid line ("the thought 'I have no mana' is always false when the
> cast is listed") — because those exact words are what the executor pattern-matched
> into compliance; (2) everything in the rule the general prompt has since absorbed is
> deleted from the guide, not compressed (check the CURRENT general prompt line by
> line against the rule before compressing); (3) the vacated #1 slot goes to the new
> biggest leak. Record in your handoff WHICH sentences you kept and why, so the next
> reviser doesn't re-compress the kept core into nothing over successive waves.**

**Why.** This is exactly the judgment call I had to improvise: deck110's mana rule was
the turnaround's engine (cast-nothing fell from 44-88% per game to near-zero; 7
borderline cases in 487 decisions), the coordinator's instruction said "strengthen it,"
and the skill said "demote and compress" — with no guidance on WHAT survives
compression. The failure mode over multiple waves is rule erosion: each wave shaves a
working rule until it regresses, and nobody can say which sentence was the active
ingredient. Keeping the fact + the named forbidden thought, deleting only what the
general layer now carries, is a repeatable resolution of "strengthen" vs "compress."

## P3. Menu-noise defusal lines are a STOPGAP CLASS — mark them removable in the guide handoff, tied to the platform defect

**Wave-3 passage (Output and handoff):**
> "...plus every corrective-fact line that belongs in the general prompt or board
> representation (representation defects found in log mining — misleading pool text,
> mangled cost rendering — get reported upstream too; the guide carries a defensive
> line only while the defect is live)."

**Proposed addition:**

> **Treat interface-defect defusal lines as a tracked class with a lifecycle. In the
> handoff, list each one as a triple: the guide line, the platform defect it patches
> (with the log citation), and the removal condition ("delete this line when the
> engine relabels mana-ability activations"). A defusal line explains what a
> misleading label ACTUALLY does and redirects ("'Cast Card Normally with Mox Opal'
> just makes mana with Mox Opal — your creatures and spells come first"); it never
> teaches strategy. These lines are pure attention tax once the defect is fixed, and
> they are invisible to a future reviser who doesn't know the defect existed — the
> explicit removal condition is what lets wave N+1 delete them safely.**

**Why.** My guide now carries two such lines (the Mox Opal label defusal; the {B}{B}
attach-line note), deck44 wave 3 carried the counter-tell equivalent, and nothing in
the skill distinguishes them from strategy content. Without the removal condition,
stopgaps accrete forever; with it, the platform fix propagates to every guide in one
sweep. Same mechanism the skill already uses for corrective-fact promotion — this just
extends it to menu-label defects and makes the removal trigger explicit.

## P4. Step 0 needs a TURNAROUND-PRESERVATION mode: when last wave's revision worked, revise under a no-regression discipline

**Wave-3 passage (Step 0):**
> "**Keep what worked, verbatim where possible** — churn in lines qwen already obeys is
> pure risk."

**Proposed replacement (expand into a procedure):**

> **When the previous revision demonstrably WORKED (the deck's record or its key
> metric turned around), run the revision as a constrained diff, not a rewrite:
> (1) Before drafting, list the guide lines the logs show being OBEYED — a line echoed
> in winning-game PLANs, or whose target misplay vanished from the corpus — and freeze
> them: they may move position but their wording does not change. (2) Draft by editing
> a copy of the deployed guide, never from a blank page. (3) After drafting, produce
> the freeze-check: for every frozen line, show where it survived (verbatim, moved, or
> demoted-per-P2) — a frozen line you cannot point to is a regression you just wrote.
> (4) Aim the NEW text at the new leak only; resist improving working sections' prose
> ("if it reads awkward but the logs show it landing, it stays awkward").**

**Why.** The skill's one-sentence version gave me no verification step, and the stakes
are asymmetric here: deck110 is the pool's proof that a "faithful" full rewrite can
take the strongest deck to 0/6 (the skill's own Pitfalls cite it). What I actually did
— carry the lethal check, carrier order, and Blast-at-face wording verbatim; keep the
mulligan line; confine new prose to the equip rule, the race section, and the DO-NOT
rebuild — followed this procedure implicitly. The freeze-check makes it auditable: the
synthesis agent or the user can diff the frozen list against the draft instead of
re-reading both guides.

## P5. Position on negative framing (for the merger — requested, reasoned, not an accident)

**What I did:** kept a rebuilt DO-NOT list (3 items, wave-4-observed only) and kept the
quote-and-forbid mana line, in a corpus whose proven lesson is "negative framing
bleeds."

**Position: the bleed lesson applies to negative framing INSIDE a rule's operative
text, not to the two constructs the skill already endorses.** Distinguish three cases:

1. **A DON'T adjacent to (or inside) a positive instruction** — this is what bled
   ("do NOT save them..." suppressing its neighbor). Write these positively; my
   equip rule, race section, and attack rules are all positive-form for this reason.
2. **Quote-and-forbid of a recurring WRONG THOUGHT** ("the thought 'I have no mana' is
   always false when the cast is listed") — this is an interception keyed to the
   executor's own phrasing, and it is load-bearing: it survived from wave 3 into the
   turnaround. It reads as fact-correction, not as a behavioral DON'T, and there is no
   positive paraphrase with the same trigger (the executor must recognize its own
   sentence). KEEP the construct; the skill already mandates it.
3. **The terminal DO-NOT list** — a short, closed, evidence-cited list at the guide's
   TAIL, after all positive rules, each item restating a misplay the corpus actually
   contains and each carrying its positive redirect in the same sentence ("...the mana
   belongs to creatures and Blasts"). At the tail it cannot suppress a neighboring
   positive instruction (there is none after it), and it serves a distinct function:
   it is the executor's pattern-match surface for the exact situation replay. The
   wave-3 skill's own Step 4 skeleton ends with it; I kept it deliberately.

**Proposed skill edit (Step 5, the prohibitions bullet):** append —
> "Positive-form is for OPERATIVE rules. Two negative constructs remain sanctioned:
> quote-and-forbid of a verbatim recurring wrong thought (an interception, not a
> posture), and the terminal DO-NOT list — kept short, evidence-cited, placed LAST so
> it has no positive neighbor to suppress, with each item carrying its positive
> redirect in the same sentence."

**Why.** If the merger reads my guide's DO-NOT list as an unreasoned violation of the
negative-framing lesson, the synthesis will either strip a proven construct from all
seven guides or keep it inconsistently. The distinction above is the actual decision
rule I used, and it is checkable against placement and wording.

## What the skill got RIGHT for me (keep — evidence from this wave)

- **Step 0 revision mode as a whole.** "Expect the failure to have moved downstream"
  named my wave exactly: mana paralysis fixed → the re-equip loop became the #1 leak.
- **The offered-vs-taken tally and reply mining.** The 47-equips number and the
  `(6/2) (printed 2/2)`-on-board-while-re-equipping evidence came straight from the
  prescribed digests; without them I'd have written another cue, not a count brake.
- **"Never teach against a non-problem."** Three wave-3 DO-NOTs (hold-Plating,
  skip-Blast-at-face, plan/number mismatch) never fired in wave 4 and were dropped —
  the drop-stale-rules bullet paid for itself in reclaimed length.
- **Named exceptions inside absolutes / numeric releases.** The chump-block release
  ("would drop you to 8 or less") and the Overseer exception ("unless Overseer itself
  must attack for lethal THIS turn") are direct applications and I'd have written
  over-firing absolutes without them.
- **Role-by-matchup as default + flip keyed to visibles.** The race section (named
  deck109 threats as the trigger) is the skill's own pattern; the wave-3 deck110 guide
  lacked the flip and both losses walked through that gap.
