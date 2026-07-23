# deck27 skill.md — proposals for the strategy-writing skill (wave-21)

Seat: Zombies (UB tribal), wave-20 revision deployed. Single-seat by default; layer-routing enforced
(single-seat items CANNOT add/cut a skill core line). This corpus produced no new headline rung — it
CONFIRMS existing rungs with unusually clean worked examples, and sharpens one existing note.

## Confirmations (existing rungs, strong worked examples)

- **Record-vs-tax disagreement (rule 1) CONFIRMED again, now in the OTHER direction.** wave-20 this
  seat rose 2/6->3/6 with FLAT tax (a false-positive record read). wave-21 it rose 3/6->5/6 with tax
  COLLAPSING (0 fallbacks). But decomposition shows the +2 is **mostly draw/opponent variance** (a
  repeated-opponent control: 3 favorable flips + 1 unfavorable), while the durable signal is the tax
  collapse from answer-first. Lesson for the skill's "read the tax, not the record" rung: the record
  can move for reasons ORTHOGONAL to the tax (here, variance) in BOTH directions — so a record jump
  is not evidence a guide/engine change worked; you must attribute per-game. Reinforces the rung; no
  text change.

- **"Write the off-case even if the winning corpus never needs it" (rule 6) — the off-case STILL
  wasn't needed, and that is itself the lesson.** The wave-20 late-stall durdle revision has now gone
  TWO corpora (its authoring corpus and this one) without its trigger state occurring: both times the
  deck drew evasion and won the grind before entering a walled stall. This is the expected behavior of
  a low-frequency off-case, NOT evidence the revision is wrong or should be cut. Skill implication
  (candidate note, single-seat so not promoted): **an off-case revision may require MANY corpora to
  draw its trigger; do not delete or "revise" an unexercised off-case for lack of validation — hold it
  and keep re-flagging the trigger condition.** Recorded for synthesis; hold pending a 2nd seat that
  shows the same multi-corpus-unexercised pattern before proposing a rung.

- **Model-vs-representation doctrine CONFIRMED, cleaner than wave-20.** This corpus shows BOTH sides in
  one place: (a) a REPRESENTATION defect fixed at the engine (wither "(both die)") immediately stopped
  its induced fallback — representation fixes work; (b) a MODEL/DECODE failure (ahead-position
  repetition loop, vs135 s18) recurred a 3rd time and was NOT fixable by prose — but was NEUTRALIZED
  by a PARSER change (answer-first), not a guide change. Sharpens the existing doctrine: model-layer
  failures that prose can't touch are sometimes best defused at the PARSER/harness layer (make the bad
  output harmless) rather than the decode layer (prevent it). No core change; supports the layer-first
  routing rung.

## WATCHES (unpromoted)

- **Free-block-decline (model-obedience): 2nd instance, DID NOT RECUR this corpus.** Trigger card
  (Bloodmark Mentor vs49) not drawn. Stays a WATCH; promote only on a DIFFERENT deck where an explicit
  favorable-block rule is disobeyed.

- **Ahead-position degeneration loop: 3rd instance reached (vs135 s18).** Crosses the wave-20
  hold-for-3 threshold, BUT it is a decode-layer item (route to notes.md / model ledger), and
  answer-first already defuses its harm. Candidate skill note: **when a recurring failure is
  decode-layer AND a parser/harness change has made it harmless, prefer NOT to open a new decode-rung
  campaign — track it, try the cheap knob (repetition_penalty), and treat the harm-defusal as the
  primary win.** Single-seat; recorded for synthesis, not proposed as a core edit.

## No new core-prompt candidate from this seat
Every item is single-seat and/or owns a layer below core (engine annotation = closed; parser =
validated; decode = defused; guide off-case = unexercised). See general-suggestions.md for routing.
