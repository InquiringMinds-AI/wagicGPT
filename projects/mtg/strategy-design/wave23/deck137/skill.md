# deck137 skill proposal (wave 23)

## PROPOSAL (single-seat -> WATCH/tag pending a second adventure or alternative-cost deck)

**Step 0-bis, rule 3 — add a FIFTH idiosyncrasy face: the ALTERNATIVE-COST DUAL-OPTION card whose
alt-cost option carries the OTHER face's stats.** Rule 3 currently names four idiosyncrasy faces
(belief-that-VETOES 17/21; card-text FABRICATION 62; target-SUB-MENU seam 49; SYMMETRIC-cost read as
one-sided 102). deck137 (Adventures) is a fifth, distinct shape: a single card the engine offers as
TWO cast options — a base-cost face and a "with its <alternative> cost" face — where the alternative
option MISLABELS itself with the primary face's (P/T) and no effect text. The executor reads the
(P/T) as the body it will deploy by paying the alternative cost and casts the wrong thing.

- Mechanism split from the four existing faces: it is NOT a fabricated ability (62) and NOT a fully
  hidden fact — the fact is MISRENDERED (the option shows a real (P/T), just the wrong face's), so the
  model is actively misled rather than uninformed. Closest sibling is the 102 SYMMETRIC-cost face
  (the number is right but its meaning is misread) — here the (P/T) is right for the OTHER face and
  misread as belonging to this option.
- Evidence: deck131 s15 (confirmed wrong outcome — cast an adventure bounce spell believing it was a
  3/1 body, self-bounced its own flyer); deck27 s19 (9.3k tax talking past the label). See findings.md.
- Authoring move (matches the existing rung's doctrine — annotate/teach, never suppress): make rule
  #1 a CAPABILITY TABLE (one plain line per card: what the CREATURE does, what the SPELL does) plus
  the corrective fact "the (P/T) on the alternative-cost option is the creature you get LATER from
  exile, NOT what you deploy now." This is the same corrective template as the 62 capability table,
  extended to a per-FACE table. Pair with the representation-ledger item (annotate the alt-cost option
  with its effect text) so the guide block demotes once the option carries the effect.
- Generality note for the skill: this shape recurs for ANY engine card offered as multiple cast
  options at different costs where the alt-cost option inherits the primary face's annotation —
  adventures, and plausibly other "alternative name(...)" cards (convoke/overload/etc. render
  "with its <X> cost" too, though those share ONE effect so the mislabel is milder). Tag single-seat;
  promote to a named rule-3 face if a second alternative-cost-dual-option deck reproduces the misfire.

**Convoke tax observation (candidate, weaker — single-seat, likely engine-side not skill).** The
convoke X-cost cards (March, Loxodon) drove three 12-13k tax spikes and all 3 of the corpus's
deferred_to_heuristic fallbacks. The tax is the model re-deriving "can I afford it / do I convoke /
which do I tap" when the engine already offers a payable "with its convoke cost" option and defers the
tap-selection to the heuristic. This is better fixed by (a) a guide line ("convoke option offered ->
you can afford it, engine taps for you, don't recompute" — shipped in strategy.txt) and (b) an
engine annotation on the convoke option ("X = N tokens; your creatures will be tapped automatically").
Not a skill-doctrine change — noting it so a second convoke deck can decide whether "trust the offered
alternative-cost option, the engine handles payment" earns a general rung alongside TRUST-THE-RENDERED-
NUMBER (they are the same family: trust the surface, don't re-derive the mechanic).
