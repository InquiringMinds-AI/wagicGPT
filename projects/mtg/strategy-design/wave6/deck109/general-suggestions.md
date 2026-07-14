# General / core-prompt suggestions (from deck109 wave-6)

Layer-routed per the skill. Burden of proof is on core; single-seat evidence cannot cut or add a core
line — these are OBSERVATIONS for the synthesis agent to weigh against cross-deck convergence.

## 1. [CORE STOPGAP — removal condition now MET from this seat; flag for cross-deck confirmation]
The core `hand -> stack` cast-happened anchor.

The core carries a stopgap: the cast-happened `hand -> stack` verification anchor, whose stated
removal condition is "dies when the Hellrider-class engine no-op is fixed." **deck109 is the canonical
repro for that class, and this wave the defect is FIXED at the source:** the "no-op" was the deck's
combo-hold `#HINT` vetoing the model's explicit Hellrider pick until `2+ creatures on battlefield`, and
commit **353f1a150** ("Combo-hold hints no longer veto the GPT model's explicit pick") removed that
veto. This corpus: 2/2 Hellrider casts resolved (incl. the formerly-vetoing light-board condition), 0
cast no-ops corpus-wide in the sweep.

Routing: the anchor is deck-agnostic CORE text, so single-seat evidence cannot cut it — but this is the
seat that OWNED the defect, so the synthesis agent should **actively check the cross-deck no-op sweep**:
if no deck shows a cast no-op this wave (the corpus-wide "2 fallbacks / 2035" figure suggests none are
this class), the anchor's removal condition is met and it can be retired from core in a deliberate A/B.
Do NOT retire it on this seat alone; do put it on the removal shortlist with 353f1a150 cited.

## 2. [FIXED — no core action] Reflexive high-life blocking.
The wave-5 per-deck blocker floor fixed it for deck109 (4 high-life blocks → 0). This stays POSTURE
(racing decks default to no blocks; control decks default to blocking) — do NOT promote "creatures
don't block" to core. The wave-5 general-suggestions asked whether OTHER racing decks (44, 110, 131,
135) show the same reflexive high-life blocking; deck109 no longer does, so from this seat there is no
pressure for a core representation/placement change. Defer to cross-deck tally.

## 3. [FIXED — no core action] Reply-parser desync.
0 desyncs across 6 games; head-first parser stays clean. No change.

No new core doctrine proposed from this seat.
