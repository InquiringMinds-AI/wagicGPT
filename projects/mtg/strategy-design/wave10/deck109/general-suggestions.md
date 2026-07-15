# General (core-prompt) suggestions — deck109 wave-10

## 1. CONVERGENCE FLAG (for synthesis) — the "offered = payable" false-belief class now has a 2nd seat

The wave-9 synthesis PARKED deck135's "colored/generic mana false belief overriding offered=payable"
(vs140 s24: declined an offered `Cast Astrolabe {1}` believing `{G}` cannot pay a generic `{1}`) as a
core candidate **pending a 2nd seat**. deck109 IS that 2nd seat:
- **deck131 seq19** (clean): declined an offered `Cast Rakdos Cackler {b/r}` — "we lack black mana (only
  have Red)." `{B/R}` is hybrid; red pays it. Offered = payable; the pilot violated the contract.
- **deck131 seq21** (spiral): same belief collided with the core contract the pilot QUOTED, spiraled to
  the 2048 cap, `unparsed_reply` -> fallback. (Won T11; non-outcome-changing.)

Same class (a specific false mana-payment rule overriding offered=payable), different mechanic (deck135
= colored-pays-generic; deck109 = hybrid `{X/Y}` misread as `{X}{Y}`). **Promotion trigger met** per the
wave-9 rule.

## 2. But do NOT escalate CORE PROSE — route the durable fix to REPRESENTATION + deck guides. Core = PASS.

The core ALREADY carries the contract ("Never decline a listed action because you believe ... you cannot
pay for it; if you truly could not, it would not be listed"), and the pilot **quoted it and still
failed** (deck131 s21). That is the representation-beats-instruction wall for the GENERIC contract — a
louder or longer core sentence will not close a specific-mechanic misread. The convergence is real, but
its durable fix is NOT a core-body edit. Layer-routed:
- **REPRESENTATION (durable, cross-deck — engine ledger, see notes.md):** annotate hybrid costs on the
  option line itself, e.g. `Cast Rakdos Cackler {b/r} - hybrid, pay with {R} or {B}`. This is the
  option-line-annotation rung (the skill's 4th representation shape) and covers EVERY deck with a hybrid
  card in one place (Rakdos Cackler `{B/R}`, Boros Reckoner `{R/W}`, and any future `{X/Y}`).
- **DECK GUIDE (interim, this wave):** deck109 adds the Rakdos Cackler card-cost fact (done in
  strategy.txt), mirroring the already-obeyed Boros Reckoner treatment. Other decks with hybrid cards
  should carry the same card-specific note until the option-line annotation ships.

**Core-body verdict: PASS (no prompt-body edit).** The 2-seat convergence routes to representation +
deck guides, not the core prose.

## 3. Reply-protocol layer — the ATTACK:-last protocol WORKED (positive report)

The wave-9 non-numeric `none`-head intent-collapse at the attacker seam (deck44 seq7) did NOT recur:
30/30 attacker windows fully declared this corpus, including a maximally-confused scratch reply (vs44
seq26) that still landed the correct `ATTACK: A1`. No core/reply-protocol change needed — logged as
confirmation the shift addressed the class. (SINGLE-SEAT positive; informational.)
