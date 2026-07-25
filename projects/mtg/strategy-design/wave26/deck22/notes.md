# deck22 (Giants!) wave-26 notes

- NEW LEDGER ITEM R-CHANGELING-TYPE-INFERENCE (representation, quality; single-seat, deck22):
  changeling bodies render `[changeling] "This card is every creature type."` but the pilot does not
  infer the tribal-role consequences (Universal Automaton not recognized as a Giant that enables
  Blind-Spot Giant / counts for Behemoth/Sovereign/Calamity). Repros: vs131 s1 (mulliganed a keep
  citing "no other Giant" while holding the Automaton), vs131 s4 (bottomed Automaton+Blind-Spot),
  vs18 s1 (same mulligan misread). Candidate fix: on a changeling permanent/spell, when the current
  deck or a battlefield card has a type-keyed restriction/anthem/count (aslongas(other TYPE), lord(TYPE),
  foreach(TYPE)), annotate the changeling option/battlefield line with the relevant type:
  `[changeling: counts as Giant]`. Interim: the guide teaches it as rule #1. Low priority (one seat),
  but the class recurs for any changeling/tribal deck.
- c1 (target enumeration) VALIDATED at this seat: vs102 s18-21 Inferno Titan 3-damage split fully
  enumerated the opponent-face target, picked 3x, no "(+N more)" truncation.
- c3 (PT-pump delta+result) VALIDATED: vs102 s30/s31 firebreathing rendered `+1/+0 until EOT
  (6/6 -> 7/6)` / `(7/6 -> 8/6)`, no stat-conflation.
- c4 parser hardening: 0 fallbacks / 0 mis-picks across 74 deck22 decisions; no HARDENING MISFIRE.
- Representation note (NOT a bug): Borderland Behemoth renders its BASE (4/4) on the cast option
  (vs27 s19) though its +4/+4-per-other-Giant static resolves on entry; the guide states the scaling.
  Watch only if a pilot ever declines/undervalues it citing the base P/T (not observed this corpus).
- No engine bug, no timeout, no draw at this seat. Oracle cross-check: Universal Automaton / Calamity
  Bearer / Sunrise Sovereign all match engine primitives (Scryfall-verified) - no card-script divergence.

ROTATION: Step-0 seat, does not rotate. New-work signal genuine (changeling-type-inference face +
representation-lever candidate). Initial guide targets: Automaton-is-a-Giant identity (rule #1,
cross-phase to mulligan+bottom), per-item attack floor over the Giant list, Mogg Sentry role.
