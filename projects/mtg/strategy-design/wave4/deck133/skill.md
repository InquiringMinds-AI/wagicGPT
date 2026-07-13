# Skill revision proposals from deck133 (wave 4)

Against: `strategy-design/wave3/strategy-writing-skill.md`. Evidence: 12-game wave4
corpus (runs 20260712-192832 / -230636, 3/12, 3 timeouts), wave4/deck133/findings.md,
and the guide revision in wave4/deck133/strategy.txt. Each proposal: quoted passage →
replacement/addition → why. Every proposal changed what I actually wrote this wave.

---

## P1. Winning-template extraction — transcribe the pilot's own good game (Step 2)

**Quote (Step 2, "the single highest-value move"):** "Compare the games qwen played
WELL against the ones it LOST and find the one repeated difference."

**Addition (append to that paragraph):** "When the comparison surfaces a game where
the pilot handled the problem situation CORRECTLY, don't just extract the difference
as a rule — TRANSCRIBE the winning sequence itself into the guide as a numbered
procedure, in the order the pilot executed it, and tell the pilot it is its own
proven line ('the exact line that won you the burn matchup last run'). A sequence
the executor has already performed under this prompt stack is stronger evidence than
any theory of what it *should* do: it is known to be reachable from the options the
engine actually offers, expressible in moves the pilot actually takes, and
compatible with the pilot's habits. Prefer transcription over invention whenever the
corpus contains the win."

**Why:** deck133's aggro problem had its solution inside its own corpus — the vs109
run2 win (removal on their creatures → Obliterator wall → block → race) against the
same opponent it lost to in run1. The wave3 skill told me to find the *difference*;
nothing told me the difference was already a finished procedure to copy. The guide's
DEFENDER block is that game, numbered 1-4.

## P2. Timeout attribution — check who was AHEAD at the cap (Step 2 diagnostic list)

**Quote (Step 2 diagnostic patterns):** "Failure to close — end-state sweep. Print
each game's final life totals; a game where the opponent's life PLATEAUED for many
turns while qwen had gas is a failure to close."

**Addition (new diagnostic bullet beside it):** "Timeout attribution — never read a
`timeout` row as a play failure until you check WHO WAS AHEAD at the cap. Print
final life totals for every timeout. Winning-but-slow (pilot ahead, opponent at low
or even negative life) is a different diagnosis with different owners: opponent
lifegain/reset effects (Elixir of Immortality), inference latency × decision count,
and engine adjudication gaps — none of which a guide line fixes. Adding
anti-passivity pressure on account of a winning-but-slow timeout regresses the
guide against a problem it does not have. Only a timeout where the pilot was behind
or stalled with gas in hand is guide evidence."

**Why:** wave3's timeouts all traced to non-casting, and the skill's framing quietly
assumes timeouts = passivity. In wave4 ALL THREE deck133 timeouts were the pilot
ahead (opp at 2, 5, and -10 — the last already won on board). Without this check I
would have re-tightened anti-passivity language that the corpus shows is now obeyed.

## P3. Role by matchup → per-turn ROLE CHECK on board-readable facts (Step 1 + Step 4.1)

**Quote (Step 1):** "Hand qwen the answer as IF-THEN keyed on the opponent archetype
('you are the beatdown unless the opponent is a faster aggro deck that's ahead on
board — then trade to survive')."

**Replacement:** "Hand qwen the answer as a per-turn ROLE CHECK keyed on facts it
can read off the CURRENT board and life totals — never on the opponent's archetype,
which it cannot classify. Good keys: 'their creatures are hitting you — your life is
falling while theirs is not', 'they have flyers you cannot block', 'their board is
empty'. Give each role its own numbered action line (see winning-template
extraction), state that the check reruns EVERY turn, and close with the invariant
that survives the flip (deck133: 'you still cast a creature every turn in BOTH
roles') so the posture switch can't be read as permission to go passive."

**Why:** deck133's wave3 guide had a one-shot archetype flip ("Only flip vs faster
all-in aggro/burn") and the pilot never executed it — both vs44 Faerie games ended
with the opponent at 20 life while the pilot kept beatdown behavior. "Faster all-in
aggro" is a classification; "flyers you cannot block" is a line in its prompt.

## P4. Life-as-resource needs a BUDGET, not just permission (new Step 5 rule)

**Quote (Step 5 / Step 2 cost-line item):** "Any deck action whose cost LOOKS bad
but is routine (fetch sacrifice, life payment, phyrexian mana, sac-for-value) needs
an explicit 'this cost is the point — always pay it.'"

**Addition (append):** "When the deck pays LIFE across several cards (fetches +
Thoughtseize + Arena + Yawgmoth in deck133), 'always pay it' is only half the rule
— unbudgeted, it compounds into the pilot racing itself. Sum the deck's self-damage
surfaces; if there are 2+ of them, add a budget line keyed to the role check: name
which life payments continue when defending and which stop ('when defending, play a
Swamp and a creature instead of Thoughtseize; skip Arena'), each phrased as the
positive alternative action, never as a list of forbidden cards."

**Why:** wave3's fetch fix worked — and generalized: vs44 run1 the pilot bled 20→13
by turn 7 from its own fetches + Thoughtseize + late discard while flyers raced it,
and vs135 run1 it cast Arena mid-race and died at -4 with the opponent at 3. The
skill taught permission for costs; nothing taught when the permission is spent.

## P5. The LIST-ANCHOR — every guide gets a "pick only from the numbered list" line
(Step 5 rule + general-layer promotion candidate)

**Addition (new Step 5 bullet):** "Anchor the plan to the option list: include one
line telling the pilot to verify the card it intends to play APPEARS in the numbered
list before writing its plan, and if it does not, to choose the best card that IS
listed and say so ('it is not castable right now'). The pilot's plan text is not
constrained by legality — it will plan an unaffordable cast and then select 'Cast
nothing' rather than reconcile the plan with the menu. Flag this line for promotion
to the general prompt: it is deck-independent, and once the general layer carries it
every guide can drop it."

**Why:** vs135 run2, seq30-31: reply says "PLAN: Cast Gray Merchant of Asphodel
immediately" while the CHOSEN action is "Cast nothing right now" — Merchant costs 5,
the pilot had 4 lands, so it wasn't offered; the pilot defaulted to nothing instead
of re-planning from the list. Same failure family as wave3's plan-carryover, one
level deeper: the plan diverges from legality at the moment it is written.

## P6. Convert the DO NOT section to positive checks (Step 4.9 + Step 5)

**Quote (Step 4 skeleton, item 9):** "DO NOT — a tight list of the 3-5 specific
misplays you actually observed, each as a flat imperative... the highest-signal part
of the guide."

**Replacement:** "CLOSING CHECKS — the 3-5 observed misplays, each converted to the
positive action that replaces it ('crack the fetch the moment the option appears'
rather than 'do NOT refuse to crack'). Negative framing BLEEDS: a proven platform
lesson is that a 'do NOT save them...' clause suppressed the positive instruction
beside it. Reserve bare NEVER/DON'T for at most one line, on the single worst leak,
where no positive restatement is as sharp. For a capability restriction (can't
block, can't target), state where the capability GOES instead: deck133's 'Bloodghast
CANNOT BLOCK' became 'as defender its jobs are: devotion, Yawgmoth fodder, Brutality
escalate fuel' — the restriction plus its positive redirect."

**Why:** the wave3 skill still mandates a DO NOT list even though the
negative-framing-bleeds lesson was already established; my wave4 guide deleted the
9-line DO NOT block entirely (its habits are now obeyed) and the where-the-bodies-go
conversion was accepted as a model example. The skill should teach the conversion,
not the list.

## P7. Revision waves: demote-and-compress rules the new corpus shows OBEYED (new
Step 3 rule)

**Addition (Step 3, after the include-ranks):** "On a REVISION pass (a corpus exists
for the current guide), re-tally each existing rule against the new corpus first.
A rule the pilot now follows (deck133: affordability 75%→36% cast-nothing, fetch
cracking, Bloodghast casting 0→19) gets COMPRESSED to its one-line core, keeping its
early position — the behavior may be prompt-dependent, so keep the anchor, but its
explanation, its cited failure stats, and its DO NOT reinforcements have done their
job and now only dilute the new #1 rule. Delete self-citations of fixed failures
('last run you picked Cast nothing 76 of 101 times') — they describe a pilot that no
longer exists. The attention freed is the budget for the new wave's lever."

**Why:** the wave3 skill is written for authoring from scratch; it has no procedure
for the maintenance pass, and the guide would otherwise only grow. My wave4 revision
spent this freed space (mana paragraph 9→3 lines, DO NOT block deleted) on the ROLE
CHECK and discard-window blocks while staying at 83 lines.

---

## What the wave3 skill got RIGHT for me (keep; no edits)

- **Offered-vs-acted tally + reply mining** found every headline number this wave
  (cast-nothing 36%, Gray Merchant 3/6 offered-castable, 10/24 late discards) and
  the plan/choice mismatch would be invisible without reading `reply` against
  `chosen_text`.
- **Confirm-before-you-accuse** kept two non-problems out of the guide: the "no
  mana" phrase still appears in replies but no longer vetoes casts (8/18 cast
  anyway in vs131 run1), and the long vs131 grind's 32 cast-nothings were correct
  (hand was targetless removal vs a creatureless deck).
- **Turn-numbered deploy script, numeric GO thresholds, per-card drain constants,
  bounded HOLDs** — all show up in the corpus as executed behavior; the wave3
  [deck133] amendments are validated and should be kept verbatim.
- **The new-fields tooling asks from wave3 findings #8 (chosen_text, turn/phase,
  latency_ms) shipped and cut this review's cost enormously** — the
  offered-vs-acted and latency analyses were one-liners this time.
