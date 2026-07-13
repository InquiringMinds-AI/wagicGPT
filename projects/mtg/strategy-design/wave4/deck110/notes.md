# Deck110 wave-4 notes — everything that doesn't live in the other four files

Evidence base as in findings.md: 12 games / 487 decisions, runs
`matchups-20260712-192832` + `matchups-20260712-230636`. Organized by audience.

---

## For the ENGINE owner

### E1. Attachment markers on board lines (highest-value single change I can name)
Add attachment state to BOTH sides of an equip relationship in the board rendering:
- creature line: `Memnite (5/1) (printed 1/1) [equipped: Cranial Plating]`
- equipment line: `Cranial Plating {2} [artifact] [attached to Memnite]`

Why markers beat the power-delta cue we teach today: the `(printed X/Y)` tag fires for
ANY stat modifier — lords (Master of Etherium gives other artifact creatures +1/+1),
+1/+1 counters (Steel Overseer), battle cry — so "two power numbers = equipped" is a
heuristic with false positives in this very deck, and it requires the pilot to infer a
hidden relation from a derived number. A marker states the relation. It also erases a
small internal contradiction in the core prompt: the stale-plan grounding line teaches
"if an action is still offered, it has NOT been done," while the re-offered Equip line
is precisely a DONE action still being offered — the marker gives the pilot a fact that
outranks both heuristics. Reproduce the failure this fixes: `1783903101` seq44-48 and
seq61-65 (five consecutive re-equips each, board showing `(6/2) (printed 2/2)`
throughout).

### E2. No-op activation suppression/annotation — TWO WAVES OPEN
Wave-3 deck110 finding #5, re-confirmed in wave 4 at higher severity (27 re-equips
onto already-equipped creatures corpus-wide for this seat): the legal-actions list
re-offers `Equip with Cranial Plating targeting X [cost: {1}]` when the Plating is
already attached to X. Suppress it, or annotate `[already attached]`. E1 makes this
survivable; this makes it impossible. Either alone helps; both together end the class.

### E3. Mana-ability labels read as spell casts
`Cast Card Normally with Mox Opal` and `Play Land with Mox Opal` are how Mox Opal's
tap-for-mana surfaces in the priority menu (`1783920257` seq4 — the model picks it and
its PLAN confabulates about what it did). Label should say what it does, e.g.
`Tap Mox Opal for one mana of any color`. Until fixed, per-deck guides carry a defusal
line (mine does, flagged removable in skill.md P3 form).

### E4. [NEEDS INVESTIGATION] A blockers window that never became a blockers ask
`1783920257` T9 (opponent's turn, Blockers phase, seq40): deck110 has an untapped
Steel Overseer (1/1), four attackers incoming, and the seat receives only a `priority`
record (it passes) — no `blockers` ask is logged that turn (the game's only blockers
ask is T5 seq19). Either the engine legitimately auto-resolved (if so, why here and
not T5?) or a bundled-blockers trigger condition missed this board. Blockers asks DO
fire in this corpus (6 across 12 games — wave-3 finding #6 is fixed), so this is a
gap case, not a dead seam. Worth one probe-deck game with a guaranteed untapped
blocker to confirm the trigger condition.

### E5. Fallback path: clean bill of health
`fallback` is absent in all 487 records (wave 3: 8% empty-reply). Whatever combination
of -j3 pacing / timeout raise / retry did this — it worked; noting so the fix doesn't
get accidentally reverted as "unused."

## For the NEXT deck110 REVIEWER

- The wave-4 guide (`wave4/deck110/strategy.txt`) moved the equip discipline to #1 and
  demoted the mana rule to a 5-line block. The FIRST thing to check in wave-5 logs:
  (a) re-equip count (wave-4 baseline: 47 equips total, ~27 waste; target: ≤2 per
  game), and (b) that cast-nothing did NOT regress when the mana rule shrank (wave-4
  baseline: near-zero delusion passes). If (b) regressed, restore the wave-3 mana
  block verbatim before touching anything else.
- Frozen lines under the P4 constrained-diff discipline (do not reword if they are
  being obeyed): the LETHAL CHECK arithmetic, carrier order, Blast-at-face-when-≤8,
  the mulligan countable, "Memnite and Ornithopter cost {0} - cast them the moment you
  see them."
- Removable-when-platform-fixes-land lines in the guide (skill.md P3 triples): the Mox
  Opal label defusal (remove on E3), the two-power-numbers perception rule and the
  `{b}{b}` attach note (simplify on E1/E2 — the count brake "ONE equip per turn"
  should survive even then; it is cheap and robust).
- Both losses are to deck109 Hellrider. If wave 5 still loses that matchup with THE
  RACE section in play, the next lever is deployment order (the model equips before
  second-creature casts even when both are affordable) — consider an explicit main-
  phase sequence: "creatures, THEN equip with the last {1}."

## For the MODEL-EXPERIMENTS track (qwen35 pilot profile, 487-decision sample)

**Mechanically reliable** (near-100% in this corpus): numbered-choice protocol; plan/
number binding (0 mismatches — wave 3 had them recurring); bundled attacker/blocker
reply syntax; cost legality incl. phyrexian ({1}{p(B)} Vault Skirge cast 16x, no
confusion); threshold rules keyed to a visible number ("opponent at 8 or less → face"
followed every time it applied); closed-exception act-floors.

**Unreliable, with a sharp boundary:** rules whose trigger requires DERIVING state
from the board rather than READING it. The repeated-activation rule was in the core
prompt AND the deck guide all 12 games; 27 violations anyway, and in 9 of them the
reply ECHOES the rule's own vocabulary ("to ensure the bonus is active") while
violating it. Vocabulary compliance ≠ semantic compliance: the model has learned the
rule's words as things to SAY, not a check to RUN. Same shape in the blocking loss
(`1783920257` seq39: recites trade-quality reasoning from the core prompt's blocking
line while the survival arithmetic said chump).

**Experiment this suggests:** A/B the attachment marker (E1) against the current
power-delta cue with guides held constant. If markers kill the re-equip loop where two
layers of instruction text did not, that is the second confirmation (after the mana
line, see process note below) that representation beats instruction for this executor
— which would re-rank the whole backlog toward representation work.

**Pacing datum:** the pool's best deck used 20-83 decisions/game; its wins cluster at
T7-13. Decision count correlates with hostile-board games, not with winning — the
27-turn win (`1783903101`, 83 decisions) was durdle vs Lightmine Field, mostly the
equip loop.

## For the DECK BUILDER / difficulty-roster track

- deck110 is currently the pool's strongest seat (9-2-1t) — top-tier slot in any
  difficulty ladder built from this pool. Its losses come only from the pool's fastest
  aggro (deck109); its durdle risk only vs prison-style boards (deck140's Lightmine
  Field). That is a real rock-paper-scissors shape worth preserving in roster design.
- Lightmine Field (deck140, 4 copies) is a structural counter to go-wide artifact
  aggro: `@each blockers: damage = number of attackers to each attacker` kills
  Memnite/Pest/Skirge boards that attack in numbers. The wave-4 guide handles it with
  a one-line exception (attack with only the protected creature). Deck builders making
  anti-aggro decks: this card is proven to slow this archetype by ~18 turns.
- Glimmervoid is NOT an artifact (`type=Land`, no artifact type) — it does not count
  for metalcraft, and it sacrifices itself at end step if you control no artifacts.
  Mox Opal and Darksteel Citadel DO count. A generated deck that leans on Glimmervoid
  for artifact count will silently miss metalcraft; keep this trap in the card-notes
  for the deck generator.
- Mox Opal's mana ability is metalcraft-gated (dead mana rock below 3 artifacts) —
  fine in this shell, a trap if the generator ports it into a low-artifact deck.

## For the PROCESS record — the turnaround as a natural experiment

Wave 3 → wave 4, same model, same matchup pool: 0/6 (five timeouts) → 9/12 (one
timeout). Four things changed at once (board representation's `Mana available:` line;
the guide rewrite; the wave-3 general-prompt revision; platform fixes — auto-skips,
bundled asks, whatever killed the 8% fallback), so total attribution is confounded.
But the corpus contains one clean contrast:

- The failure whose REPRESENTATION changed (mana visibility) died completely —
  delusion passes 44-88%/game → 7 borderline in 487, most of them correct.
- The failure whose representation did NOT change (attachment state) PERSISTED at
  full strength through two layers of instruction text saying the right thing — 27
  waste re-equips, 9 with the rule's vocabulary in the violating reply.

Same executor, same wave, same guide-and-prompt attention budget: instruction text
plus a fixed representation beat the failure; instruction text alone did not. That is
the strongest evidence yet for wave-3 finding #1(d)'s hypothesis ("representation
fixes beat instruction fixes for interface facts"), obtained for free. The general
lesson for prioritization: when a misplay traces to state the board doesn't SHOW,
route the fix to representation first and treat guide text as the stopgap — the
reverse order costs a wave.

Secondary process note: the win-rate signal validated the wave-3 decision to judge
guides by decision quality, not record — deck110's 9/12 still contains its worst
misplays (the equip loops live in WINS, e.g. `1783903101`). Reading only losses would
have missed the biggest remaining leak.
