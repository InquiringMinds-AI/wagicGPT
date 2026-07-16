# Skill delta (wave 15) — deck110 seat contribution

The CURRENT skill is `strategy-design/wave13/strategy-writing-skill.md`. Wave 15 at the deck110 seat
is a VALIDATION wave: two of the skill's wave-13 method-headlines shipped as engine fixes and this
corpus is the first to test them at scale. The deck110 seat proposes **NO new method headline** and
**NO cut/edit to any existing method line** — the guide freezes for the 8th consecutive wave. Every
item below CONFIRMS an existing method rung; one adds a small verification caveat.

## Confirmations of existing method (the two shipped-fix headlines both PASS)

- **Method headline 3 (verify RESOLVED effects against the game log, not the model's asserted
  magnitude) — the Galvanic Blast rung is now CLOSED and the method is what proved it.** The wave-13
  fix shipped; this corpus's Blasts logged `Damage: 2,2` (=4) at N>=3 artifacts and a single
  `Damage: 2` at N=2 — exactly Oracle-correct on both arms. The lesson held twice over: (a) the fix
  is validated ONLY by reading the engine's `Damage:` events, never the model's prose; (b) a NEW
  trap surfaced — a naive grep of the event STRING dedups the two identical `Damage: 2` lines and
  mis-reports a working 4-damage Blast as "2." **Small caveat to fold in: when an effect resolves as
  MULTIPLE identical log instances (base + oneshot conditional), COUNT the instances or read the
  life/stat delta — do not dedupe the event string.** This is the same "read RESULT not INTENT"
  principle applied to its own verification tooling.

- **Method headline 2 (score attacker declarations by parsed intended-vs-recorded, and the durable
  fix = attackers name->index reconcile respecting eligibility) — the fix SHIPPED and behaves
  exactly as specified.** Across 20 attacker windows at this seat, every eligible named creature
  unioned into the declared set; every name absent from the eligible roster was a summoning-sick
  creature the reconcile correctly EXCLUDED. No eligible attacker was dropped; the wave-13 MIXED-form
  silent-drop is not reproducible. Confirms both the SCORING method (reconstruct against the eligible
  roster + the `You declared attackers:` log, not the fallback flag or `chosen_text`) and the fix's
  eligibility-respecting union arm.

- **Outcome-delta no-op predicate (wave-10 headline 5 / E2)** re-validated by a CLEANER variant: a
  fully-executed A->B->A Plating bounce (deck135 s20->s21, 2 wasted mana) where the identity-keyed
  ALREADY marker leaves the move-target un-marked. Classify equip decisions by outcome-delta, never
  raw same-name count. Engine ledger, never a guide line — Rule #1 (obeyed 100% here) does not cover
  the net-zero MOVE shape.

- **The newly-EXERCISED-seam re-audit (wave-13 reviewer sharpening (ii)) held.** The blocker seam
  fired again (2 blocks); both were minor over-blocks with the prompt annotating the correct line
  verbatim — scored MODEL/representation-wall, NOT a guide gap, exactly as the sharpening prescribes.
  No guide line reached for.

## Verification-method note (additive, single-seat — offered, not a headline)

- **When a shipped fix's success shows up as a NEW log SHAPE (two `Damage:` instances instead of one;
  a name unioned into `declared attackers`), verify against the SHAPE, not the pre-fix grep.** The
  wave-13 verification greps were written against the BROKEN engine (one `Damage:` line, dropped
  name); re-running them unchanged post-fix silently mis-scores the fix as still-broken. After a fix
  ships, re-derive the success signature from the fixed engine's own output before counting. WATCH /
  single-seat — folds under headline 3's "read RESULT not INTENT," no new rung.

## No change to core
`general-strategy.txt` core prompt untouched from this seat (11th consecutive PASS). No 2+-seat core
candidate surfaced. The false-belief family (colored-pays-generic) produced zero offered-payable
declines at this seat again.
