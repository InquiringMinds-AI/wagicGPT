# General / core-prompt suggestions (from deck109 wave-5)

Layer-routed per the skill. Burden of proof is on core; single-seat evidence cannot cut or add a
core line — these are OBSERVATIONS for the synthesis agent to weigh against cross-deck convergence,
not unilateral core edits.

## 1. [CORE PRESENT but LOSING at distance -> route to PER-DECK, flag for cross-deck check]
Reflexive blocking by a racing deck at high life.

The core prompt already carries strong blocker guidance:
> "When you would stay above 8, blocking is optional and 'no blockers' is often the right answer:
>  block only when you can name the gain..."

Yet deck109 (a pure racing deck) reflexively blocked 4 times, ALL at >=18 life, at least 3 of them
pure clock-waste (Goblin into a 2/2 at 20; two Goblins into one 2/2 at 20; Boros Reckoner held to
block a 1/1 mana dork at 20 while opp at 7). The core line is present but LOSING to a local
"there's an attacker, I have a creature" default.

Routing: this is POSTURE (racing decks default to NO blocks; control decks default to blocking), so
the deck-specific floor stays in the guide — I added a positive blocker-seam floor to deck109's
strategy.txt keyed to the 8-life threshold (reusing the core's existing number, per the
threshold-reuse rule). Do NOT promote "creatures don't block" to core — it would misplay every
control deck.

**Cross-deck action for synthesis:** if OTHER racing/aggro reviewers (44, 110, 131, 135) also report
reflexive high-life blocking this wave, that is convergence that the core's blocker paragraph is too
buried/soft to reach the blocker seam. In that case the deck-agnostic improvement is a
REPRESENTATION/placement change, not new doctrine: the core already says the right thing, so the fix
is to make "above 8 life, 'no blockers' is the default" a sharper standalone sentence at the blocker
decision rather than a subordinate clause mid-combat-paragraph. Only act on cross-deck agreement.

## 2. [FIXED - no core action] Reply-parser desync.
The head-first parser fix is confirmed live (0 desyncs across deck109's 6 games). The reply-format
contract belongs in the reply-protocol (code-appended) layer, already covered; guides can now shed
their interim output-format copies (deck109's is removed in this wave's strategy.txt). No core-prompt
change.

## 3. [ENGINE - not a prompt item] Hellrider cast no-op.
Routed to the engine ledger (see notes.md), NOT the prompt. The core's cast-happened `hand -> stack`
anchor stopgap remains necessary and correct — it is exactly what let the deck109 guide's fallback
rule detect and route around the no-op this wave. Keep that core stopgap until the engine no-op is
fixed.

No new core doctrine proposed from this seat.
