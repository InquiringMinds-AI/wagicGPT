# General-prompt suggestions from the deck-135 review (wave 4)

Input: LIVE `bin/Res/ai/gpt/system_prompt.txt` (line numbers below refer to it as
deployed in this corpus). Evidence: deck135's 12 seat translogs, R1 =
`matchups-20260712-192832`, R2 = `matchups-20260712-230636`. Each proposal states
its layer verdict: CORE vs PER-DECK vs ENGINE/REPRESENTATION, with reasoning —
every deck pays for each core line, so I only argue CORE where the defect showed
up as deck-agnostic.

---

## G1. Complete the interface contract: the offer itself proves the TIMING — add the sorcery-window fact

**Current (line 34):** "Every listed choice is legal AND PAYABLE right now - the
game has already checked your mana, colors, and targets... Never decline a listed
action because you believe it is illegal or that you cannot pay for it; if you
truly could not, it would not be listed."

**Proposed addition (one sentence, after "it would not be listed"):**
> "That includes TIMING: a sorcery, creature, or other main-phase cast appears in
> your list ONLY during your own main phase with an empty stack — so when you see
> 'Cast <card>', it is by definition YOUR turn and YOUR window; 'it is the
> opponent's turn' and 'wrong phase' are impossible for a listed cast."

**Why:** R2 vs140 — "Cast Diamond Faerie" listed 22 times, taken 0; the refusals
reasoned "I have no mana" (covered by the existing payability clause, which still
lost) and "wrong phase; it is the opponent's turn" (seq 33 area) — the second
belief is NOT explicitly covered: "believe it is illegal" is abstract, and the
model routed around it via turn-identity, not legality. The addition closes that
route by making the offer itself the proof.

**Layer: CORE.** Deck-agnostic (the coordinator notes deck133 hit the same
class), one sentence, and it extends a contract paragraph that already exists —
no new concept for other decks to pay for.

## G2. Rewrite the aggressor attack line as a per-creature runnable check with a floor at the attackers seam

**Current (line 18, tail):** "As the aggressor, attack every turn with everything
not needed to survive a lethal swing back; a faster clock wins the race, not
clean trades." And (line 29, opening): "Combat is arithmetic. First, the free
case: if the opponent has NO untapped creatures, attack with every creature you
control..."

**Proposed replacement for the line-29 opening (keep the free case, then add the
non-free case as a test instead of leaving it to posture):**
> "Combat is arithmetic — run it per creature, never as a mood. The free case: if
> the opponent has NO untapped creatures, attack with everything. When they DO
> have untapped creatures, each of your creatures still attacks if ANY of these
> holds: (a) no untapped opponent creature can legally block it, (b) its
> toughness is greater than the biggest legal blocker's power (it survives being
> blocked), or (c) being blocked profits you (the blocker dies, or you draw a
> card, or your attacker is expendable and theirs is not). Declaring 'no
> attackers' is a misplay unless EVERY listed attacker fails all three tests. 'The
> opponent might block' is not a reason to stay home - a block you priced in is
> your math working."

**Proposed addition (same paragraph, punisher case):**
> "An effect that punishes attackers (damage to each attacker, a tax per
> attacker) changes the NUMBERS, not the rule: recompute per attacker - often
> attacking with exactly ONE creature that survives the effect is free - and
> attack with the subset that survives. Stopping all attacks against a punisher
> hands the opponent the game at their leisure."

**Why:** the single strongest outcome split in my 12 games: wins declared 3-8
real attacks, losses/timeouts 0-1. Both vs140 timeouts (R1 27 turns, R2 32
turns) were total attack freezes against Lightmine Field even though a solo
Ohran Viper (1/3) survives the 1 damage; R2 vs44 (loss, opponent at 20 when
deck135 died) had 1 real attack in 56 decisions with no punisher at all — pure
friction-avoidance. The existing "attack every turn with everything" posture is
exactly the prose form the executor abandons at the first plausible excuse;
line 29's arithmetic frame is already trusted (the model runs the lethal sums it
is given), so put the attack decision INSIDE the arithmetic frame.

**Layer: CORE** for both. The per-creature test and the punisher rule are
deck-agnostic (any deck, any punisher — Lightmine is just deck140's instance),
they REPLACE existing prose rather than growing the prompt much, and deck131's
reviewer independently proposed the same runnable-checks shape for the BLOCKING
line — the two rewrites belong together as one combat-paragraph overhaul.
Per-deck guides then only instantiate names (my Rule #1 already does).

## G3. Turn-ownership confusion: the core line exists and still loses — the remaining fix is REPRESENTATION, not more prose

**Current (line 35):** "Re-read the phase line every decision - when it says it
is your turn and a main phase, this IS the turn your plan called 'next turn':
act now."

**Verdict: NO new core text.** This wave-3 line is present, correct, and was
still bypassed (R2 vs140 seq 33: plan says "Cast Diamond Faerie this turn",
phase line says its own Main 1, chose pass). Another paragraph of the same
instruction has no mechanism to work where the first didn't. Two better routes:

- **ENGINE/REPRESENTATION:** stamp turn ownership ON the choice list header,
  where the model's attention demonstrably is (it reads the numbered options —
  G1's false beliefs were ABOUT them): e.g. "Your legal actions (it is YOUR
  Main Phase 1):". The phase line sits far above the list, past the game log and
  battlefield; the header is adjacent to the decision.
- **CORE, covered by G1:** the sorcery-window sentence grounds turn identity as
  a property of the offer itself, which is the form of grounding that survives
  the model's attention pattern.

**Why:** distinguishing "instruction missing" from "instruction present but
losing at distance" — the second is a representation problem. Adding prose for
it would tax every deck for a line already proven insufficient.

## G4. Plan/number mismatch enforcement: core line present and losing — route to ENGINE (reply-choice consistency check)

**Current (line 36, tail):** "...your chosen number must EXECUTE your own plan:
if your plan says to cast or activate something that is in the list, pick that
number; never describe an action and then choose 'Cast nothing' or pass."

**Verdict: NO new core text; propose an ENGINE check instead.** Observed losing
twice in game-relevant spots: R2 vs140 seq 33 (plan "Cast Diamond Faerie this
turn" → chose pass) and R2 vs44 seq 12 (reply literally says "this is likely a
mistake given the strategy guide" → passes anyway). The parser already extracts
the choice; it could cheaply detect "reply names an action string that appears
in the option list, but the consumed choice is pass/Cast nothing" and RE-ASK once
with a one-line notice ("your plan named option N - confirm or change"). That
converts a systematic executor slip into a second chance at the exact seam,
without adding a word to the prompt every deck pays for.

**Why:** same reasoning as G3 - the instruction exists; repetition is not a
mechanism. The mismatch is machine-detectable, which makes it engine territory.

## G5. Stale-plan carry: text contract is working, but plan-echo POLLUTION is an engine defect

**Gap (no current line at fault):** many deck135 replies begin with a stray
fragment of the PREVIOUS decision's text before their own number and PLAN, e.g.
R2 vs140 seq 41-57 replies open with ". Cast Glacial Revelation {2}{g}  PLAN:..."
regardless of what is being asked. The carried-plan CONTENT rules (line 35) are
earning their keep — the model does re-ground on the list in most decisions —
but the mechanical echo means each reply's first tokens are the previous
intention, which is the deferral loop's raw material (same plan re-affirmed
verbatim seq 67-73, T16→T20, never executed).

**Proposal: ENGINE/REPRESENTATION.** Check how the last-plan text is spliced
into the next prompt/reply seam; strip any leading echo before logging/parsing,
and consider truncating the carried plan to its PLAN: line only. No core-prompt
change — no wording can fix tokens the harness itself prepends.

---

## Audit of existing core lines across my 12 games

**FOLLOWED / EARNING THEIR KEEP (do not weaken):**
- **Line 36 fetch/cost paragraph** ("sacrifice a fetchland the first time its
  activation is offered"): cracks prompt in all 12 games; zero cost-refusals.
  The residual duplicate-answer noise is the engine re-offer loop, not this text.
- **Line 22 hold-limits with the two-turn release**: Force of Negation went from
  wave-3's pure hoard to correct wipe-countering (R2 vs140 seq 61 Damnation, seq
  75 Pyroclasm). Holding-passivity persists in reduced form (15 of 43
  cast-nothing decisions still cite Force) — keep the valve, let deck guides
  tighten per-deck.
- **Line 19 act-floor + closed exception**: cast-nothing rates are low in 10 of
  12 games; both wins vs133 cast nearly every turn. The two vs140 games are the
  exception and their durdle was combat-side (G2), not cast-side.
- **Line 29 reverse/survival math**: WORKS at the blockers seam — R2 vs109 seq 35
  assigned all three blockers at exactly-lethal 7 life, correctly. Fails
  intermittently at the ATTACKERS seam at low life (R2 vs110 seq 21: attacked
  with both blockers at 3 life, died). Verdict: keep; the attack-side trigger
  belongs in G2's per-creature frame and per-deck reinforcement, not new prose.
- **Line 34 completeness clause** ("if a creature is not in the attackers list it
  CANNOT attack"): no phantom-attacker plans in this corpus (wave-3 F8's
  summoning-sick Treefolk plans did not recur).

**PRESENT BUT LOSING (fix is elsewhere — see G1/G3/G4):** payability clause
(line 34) vs "I have no mana"; phase re-read (line 35); plan/number binding
(line 36).

**DEAD WEIGHT — none claimable from one seat.** Lines I never saw exercised
(X-spells, hybrid mana, equipment re-activation) are simply outside this deck's
cardpool; a single-deck reviewer cannot vote them out. Flag for the synthesis
agent: only cross-deck agreement should cut core lines.

---

## Engine-track items carried forward (not prompt work, listed for the platform owner)

1. Fetch re-offer loop persists (wave-3 F2): same crack re-answered 2-4x across
   Main1→Main2→Upkeep (R2 vs44 seq 2-3/23-24/31-32/36-37; R2 vs140 seq 2-4).
   Still the largest single inference-budget drain and timeout fuel.
2. Reply-choice consistency re-ask (G4).
3. Choice-list header turn stamp (G3).
4. Plan-echo splice cleanup (G5).
5. Degenerate-reply detection: R1 vs109 seq 17's "** ** **..." reply consumed an
   empty blockers decision silently; a repetition-collapse check could re-ask.
