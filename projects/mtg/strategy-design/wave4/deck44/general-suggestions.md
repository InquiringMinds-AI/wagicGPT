# Deck44 — proposed modifications to the general strategy (core prompt), wave 4

Evidence base: deck44's 12 wave-4 games (7-5). Line numbers refer to the LIVE
`bin/Res/ai/gpt/system_prompt.txt`. Each proposal states its ROUTING (core prompt /
per-deck / ENGINE) with reasoning — the layers question is the heart of this wave's
headline finding.

---

## P1 (core prompt, STOPGAP) — Close the reaction-window loop: an offered counterspell means a spell is pending NOW

**Current text (line 22, two places):**
> "Pure reactions (counterspells, combat tricks, removal that cannot hit a player):
> these need a TARGET - never cast one into an empty board or empty stack just to
> spend mana. Hold them, but holding has LIMITS. Whenever the opponent's play gives you
> a real moment to respond AND you can pay for a response, the game opens a window and
> asks you - you never need to guess when that moment is; it arrives as a decision with
> your legal choices listed."

This line does the setup and stops one inference short of the payoff. It says the window
will arrive — but not how to RECOGNIZE that the current decision IS that window. The
board representation never lists the stack (verified: 0 occurrences of the stack in any
CURRENT SITUATION block across my corpus), so the executor looks at the board, finds no
pending spell, and the line's own "never cast into an empty stack" clause hands it the
exit: my pilot passed live counter windows in game after game with PLANs saying "there is
no spell on the stack to counter" / "casting a counterspell without a target is illegal"
(e2540 s5/s8/s11/s15 — Young Pyromancer, Guttersnipe, Cyclonic Rift all resolved
uncontested; e2880 s28/s30 — died at 8 life holding two Counterspells and two Arcane
Denials, opponent at 7). Counterspell was cast 0 times in 12 games. The "empty stack"
caution, written to prevent wasted casts, is currently backing the false belief.

**Proposed replacement (same bullet, revised middle):**

> Pure reactions (counterspells, combat tricks, removal that cannot hit a player): these
> need a TARGET. Hold them, but holding has LIMITS. You never need to guess when the
> moment to respond is - the game opens a window and asks you, and the OFFER IS THE
> SIGNAL: every listed action is legal RIGHT NOW, so when "Cast <a counterspell>" is
> offered during the opponent's turn, an opponent spell IS on the stack at this moment
> waiting to resolve - that is the only reason the option can appear. Find it: it is the
> last game-log line reading "Opponent's X: hand -> stack" with no "stack ->" line after
> it. Judge whether THAT spell is worth countering; the thought "there is no spell on the
> stack" is always false while the option is listed. A reaction is wasted only when you
> fire it at a target that doesn't matter - not by trusting an offered window.

**Why core and not per-deck:** the tell is INTERFACE truth — a consequence of line 34's
own "every listed choice is legal AND PAYABLE" contract — and every deck with instants
pays the same tax (the coordinator corpus will show whether other decks' counters are
equally dead; mine were 0/12). It is one inference the executor provably cannot make
unaided, applied to a surface every deck sees.

**Verification note for the merger:** I audited every window in my 12 games where a
counter option was offered — every single one had a live pending spell in the log tail.
The tell has no observed counterexample. Re-run the same audit on the other six decks'
corpora before shipping; if any deck shows a counter offered over a genuinely empty
stack, the wording must weaken from "always false" to a look-and-check procedure.

**STOPGAP marker:** delete this passage (reverting near the current wording, minus the
unscoped "empty stack" clause) when the P2 engine fixes land — a fact carried in the
representation beats the same fact carried as prompt inference, and every prompt line
costs every deck attention.

## P2 (ENGINE, not prompt text) — The real fix is representation and priority timing

Explicitly NOT proposing prompt text for these; argued here because the routing decision
is the deliverable.

1. **Surface the stack in CURRENT SITUATION** — e.g. `On the stack (you may respond):
   Opponent's Guttersnipe {1}{r}{r}`. The deciding fact moves from a fragile log-tail
   inference to the highest-attention surface. Instruction cannot substitute for
   visibility on a weak executor: the wave-3 prompt already implied everything needed
   (offered = legal; the window "arrives as a decision") and the model still reasoned
   itself out of it 100% of the time.
2. **Label reaction options with their pending target** — `Cast Counterspell {u}{u} —
   counter Guttersnipe`. This makes P1's entire inference chain unnecessary at the exact
   decision point, and also fixes target selection for multi-spell stacks.
3. **Priority timing defect:** in e2880 (vs 110) the killer spell (Master of Etherium,
   0/0 printed that became 11/6 and dealt 9) went `hand -> stack` and `stack ->
   battlefield` with NO deck44 decision window in between — the windows deck44 did get
   arrived at later, already-resolved moments. No prompt or guide text can fix a window
   that never opens; this is the same class as wave-3's blockers-seam silence (since
   fixed) and needs the same seam-level engine attention.

Fix 3 is a correctness gate for 1-2: surfacing the stack only helps at windows that
actually open pre-resolution.

## P3 (core prompt, no change — audit result) — Wave-3 lines confirmed earning their keep in this corpus; no dead weight found

Audited against all 12 games:
- **Mana/auto-tap contract (line 4) + offered=payable (line 34):** the wave-3 "no mana"
  false-belief class is GONE from my corpus (one residual hybrid hesitation in 12 games,
  down from four-turn refusals). Working.
- **Act-floor + anti-deferral (line 19), attack floor + lethal math (line 29):** cast-
  nothing-with-creature-castable happened twice in 12 games (both in won games); the
  pilot attacked every turn and ran explicit lethal arithmetic in wins. Working.
- **Mulligan default (line 30):** all 12 keeps were defensible by its own land-count
  rule. Working (though the seam produced 0 mulligans — untested in the shipping
  direction).
- **Life-threshold release valve (line 22, "your life is 10 or less"):** did NOT save
  deck44 at 8 life with four counters in hand — but it lost to the P1 false belief
  ("no target exists"), not to its own wording; a release valve cannot fire on a card
  the pilot believes is uncastable. Expect P1/P2 to make this line effective as-is.

I propose no deletions: every audited line is either observably working or blocked by
the P1 defect. A line that is followed cannot be distinguished from an unnecessary one
by observational data alone — removal experiments are a separate, deliberate A/B, not a
wave edit.
