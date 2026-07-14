# Wave-8 synthesis notes

Corpus: PRIMARY control arm `matchups-20260714-044131` (round-robin, 6 games/deck, decks
44,109,110,131,133,135,140, wave-7 guides, UNCHANGED core, full wave-7 engine ledger shipped in the
binary). SECONDARY arm `matchups-20260714-064838` = the `hand -> stack` anchor-removal A/B. Seven
seats reviewed. Mode across the pool: CLOSING wave — the representation fixes the last three waves
designed all shipped and were put to the end-to-end test; the method's job was to codify the terminus,
not open new diagnoses.

## Win table (CONTEXT ONLY — n=6 is noise; per-seam obedience + the representation ledger are the signal)
44: 5/6 (top, frozen spine) · 109: 4/6 (matchup/variance, spine frozen) · 110: 5/6 (marker stable) ·
133: 3W/2L real (135v133 wedge excluded; turnaround) · 131: 1/6 (OBEYED-BUT-LOSING, 4th wave) ·
135: 0/4+2TO (OBEYED-BUT-LOSING, construction-bound) · 140: reach-starved, overshoot FIXED 8->0.
Per the USER EVALUATION DOCTRINE these are cited as context, never as a verdict on a guide.

## Core verdict: PASS (5th consecutive wave, no prompt-body edit)

- **The scheduled `hand -> stack` anchor A/B RAN and the removal is ADOPTED + LIVE** (commit 6d02cb68c;
  the anchor sentence is GONE). A/B result: cast-nothing 41.5% vs 41.9%, consecutive same-pick 12 vs 12,
  same-name double-casts 13 vs 14 — no regression; defect fixed at source (353f1a150 + e11b97a77).
- **`general-strategy.txt` is an unchanged copy of the live post-A/B prompt.** No new core edit.
- **All four single-seat core candidates fail the convergence gate** (2+ seats or completion of a core
  line): deck131 `damage>=toughness=dead` (1 seat, WATCH); deck135 reply-protocol intent-collapse
  (reply-protocol LAYER, not prompt-body; 1 instance); deck140 shroud-vs-non-targeted (1 seat, not
  representation-carried, WATCH); deck140 tapped-attacker (now CARRIED by the shipped `[tapped...]` tag,
  core-prose case dissolved). Core carries no cast-anchor stopgap anymore.

## SKILL — adopted (seat citations)

1. **[3+ seats — headline] The confirmed-fired-representation DEMOTION terminus GENERALIZES across fix
   shapes, with shape-specific verification signals; the DELETION end propagates across guides.**
   deck140 P1 (re-anchor-to-count == surfaced-count, same fix two layers) + deck131 #1 (absent-option
   fix, verified by the OPTION APPEARING, 12 co-offers vs 0) + deck133 #1 (deletion end, GM `+2`
   workaround deleted, `{drains 8}` resolved as 8). Folded into the Step-0 verify-FIRED terminus.
2. **[deck140 P2] Demote a belief-kill block PER-BELIEF against what the representation literally
   surfaces** — a count carries "how many," never "how your spell interacts"; tapped/artifact demote,
   shroud survives. Same Step-0 terminus.
3. **[deck140 P3+P4] Validate a re-anchor by defect-class->0 AFTER the castability/intra-turn join** (7
   raw held-candidates, all 7 non-overshoot on inspection); **a validated re-anchor = TURNAROUND mode**
   (demote+freeze, hold any new fire condition to a watch). Extends confirm-before-accuse + turnaround.
4. **[deck110 P1] A NEW engine-surfaced value re-opens the manual-recompute audit** — run it against
   every value the engine STARTS surfacing this wave (`Artifacts in play: you N`, accurate 180/180,
   ignored while the pilot hand-counts Glimmervoid+lands). Into the prefer-surfaced-number bullet.
5. **[deck135 P1] An else-branch of a priority rule needs its own IMPERATIVE scoped to a countable
   condition, not a permission, and must NAME the anti-pattern the executor will invent.** Displacement
   third witness (Into the North declined 4× to "hold mana for a creature," died at 5 life). New Step-5
   bullet; completes wave-5 DISPLACEMENT + wave-7 deck140 "state both branches together."
6. **[deck131 #2] A keep-side GUARDRAIL can INVERT into a false KEEP** — strict-ordered gate (scarce
   resource decides alone), subject the keep on the scarce resource, no keep-side clause names a
   standalone-triggerable payoff (the "Young Pyromancer ALWAYS a keep" token stood alone, kept a
   0-Mountain hand). New Step-5 bullet; corrects the wave-7 deck131 P3 line that ADDED the guardrail.
7. **[deck44 P1] Score a VALUE-FLOOR rule by DECISION-CORRECTNESS, not raw usage-rate** — the counter
   seam fell 13/14->9/15 raw but was 15/15 CORRECT. Into the Step-2 offered-vs-taken tally; the USER
   EVALUATION DOCTRINE at the metric layer.
8. **[deck135 P3 — diagnostic, WATCH] Reply-protocol intent-collapse (answer-before-reasoning)** as a
   distinct harness-sweep sub-class under PLAN/CHOICE MISMATCH (head emitted before the PLAN reasons to
   a DIFFERENT same-window option). Route to reply-protocol, never score against a guide; durable fix a
   reply-protocol A/B. Adopted as a Step-2 diagnostic distinction only.

## SKILL — rejected / held (seat citations)

- **[MISDIAGNOSIS — do not re-propose] Offer-side no-legal-target suppression** (deck44 finding #1 +
  general-suggestions #1). Post-review probe: both cited GFTT records had opponent `(creatures: 0)` but
  deck44's OWN nonartifact Faerie on board — "destroy target nonartifact creature" legally targets your
  own creature (601.2c), so the offer was correct; a Baka census confirmed the genuinely-targetless
  case is already suppressed (6994/6994 castable=0). NOT an engine gap; NOT a ledger item; does NOT
  block any demotion. The residual is a representation NICETY (below), and deck44's guide fallback is
  STRATEGY, not a contract workaround.
- **[single-seat -> WATCH] `damage>=toughness=dead` core rules-fact** (deck131 #3) — per-deck this wave.
- **[single-seat, reply-protocol layer] intent-collapse core-prompt edit** (deck135) — no prompt-body.
- **[single-seat -> WATCH] shroud-vs-non-targeted core mechanics-fact** (deck140) — not representation-
  carried; genuine core candidate on a 2nd seat.
- **[dissolved] tapped-attacker-still-a-threat core prose** (deck140) — the shipped `[tapped...]` tag
  carries it (86 firings, obeyed).

No method line was cut. Boundary re-check (task item 4): the restatement-ban object list (Inputs #2),
the four-layer routing discipline (CORE / PER-DECK / REPLY-PROTOCOL / ENGINE-REPRESENTATION, in
Output/handoff), and the single-seat-can't-cut-a-core-line epistemic are all intact and unaltered; every
wave-8 adoption is additive method or a Step-bullet refinement, and every single-seat core candidate was
routed to a WATCH, not a core edit.

## RANKED ENGINE LEDGER (rank = leverage × cross-seat breadth × strength-of-evidence)

**OPEN (ranked):**

1. **FETCH / repeatable-activation RE-OFFER — CONSUME-ON-CHOOSE (+ don't re-list every priority
   window).** deck135 headline + deck110 + deck133. The decline-suppression (wave-7 item 4) FIRED —
   held-fetch decline windows fell ~62/game -> ~27/game (deck135: vs140 54, vs131 38, vs44 22, vs109
   19, vs110 2) — but NOT eliminated: the crack re-appears each turn as the board changes, so the
   residual 22–54 persists and is still the #1 driver of control-deck decision counts -> timeout/latency
   exposure. deck133 corroborates a priority re-ask multiplier (repeated single-option "Put in Play with
   Bloodstained Mire" re-asks, mana unspent). Fix: once a crack activation is CHOSEN, consume/withdraw
   it; offer a held fetch's crack at most once per main-phase window and suppress it on opponent turns
   while tapped out. Detect via the verbatim `Put in Play with <fetch> targeting <land>` string across
   consecutive seqs. Highest live leverage; multi-seat.

2. **BLOCKER-SEAM — complete the PUNISHER / `[deals 0]` / not-lethal rider (the UN-shipped half of
   wave-7 #5).** deck109 headline + deck44. The #5 lethal-arithmetic line ("Unblocked... you would be
   at M - NOT lethal") SHIPPED and fired 3/3 (deck109) but is PRESENT-AND-LOSING — the pilot read it and
   blocked anyway in both loss games (gang-blocked a 6/6 at 19 life; blocked Phyrexian Obliterator into
   its sac-punisher at 10 life). The PUNISHER rider did NOT fire for Obliterator (vs133 seq22: A2 =
   "(5/5) [trample]" with no sacrifice-text, while A1 got a full `{text:}` rider). deck44 adds the
   `[deals 0]` gap (Archmage win-con blocked a 0/2 Ornithopter, no marker). Fix: annotate a blocker
   option with the attacker's punisher rider (`auto=@damaged(this)...sacrifice`) and a `[deals 0]` /
   `[not lethal]` stamp; consider a HARDER directive form of the not-lethal rider. This is the routing
   target for the 3-guise reflexive-block convergence — the shipped half proved insufficient alone.
   Cross-seat (109+44); NOT a core sentence.

3. **REPRESENTATION nicety — annotate the option line when a mandatory-target spell's only legal targets
   are the caster's OWN permanents.** deck44 (reframed per the post-review probe; being implemented
   main-side). Mirrors the "can target on the stack: <name>" annotation. NOT a contract violation (a
   legal self-target exists), so this is a clarity nudge, not a bug fix — it lets the pilot avoid a
   self-target without leaning on the per-deck STRATEGY fallback (name an enemy target).

4. **EVIDENCE-TOOLING (reviewer-facing).** SHIPPED this wave and confirmed: `gamestart` header (removed
   ~7× epoch-arithmetic toil — deck44/deck131), `defer`/`deferred_to_heuristic` record (deck110/deck44),
   `events` narration-delta (carried GM `+8/-8` and the vs131 `Blightsteel: stack -> library` counter —
   deck133/deck140). STILL OPEN: an explicit per-record `resolved: battlefield|countered|fizzled|
   graveyard` field (deck133 — `events` helps but doesn't remove the manual scrape); a per-seat
   timeout-adjudication / `gameend` line in the jsonl (results.tsv now fills life0/life1/turn, but the
   seat file's final state is still read off the last decision — deck131/deck140); the OPPONENT token in
   the jsonl FILENAME (deck44 — game->file mapping still needs the results.tsv cross-reference).

5. **CONTROL-MIRROR TIMEOUT ADJUDICATION / cap-by-decision-count.** deck140 + deck131. results.tsv
   adjudication made timeout TRIAGE possible (vs135 timed out with deck140 AHEAD 21-16; vs131 with the
   finisher countered), but the structural fix (stalemate detector / adjudicate-by-life-at-cap /
   cap-by-decision-count) remains open. Attacked indirectly by ledger #1 (fewer decisions).

6. **Land-play validation edge (low).** deck110: a normal first-land Glimmervoid failed validation and
   deferred to the heuristic (which replayed the same land). Zero harm; worth an engine glance only if
   it recurs across seats.

7. **Representation nudge (optional, low).** The accurate `Artifacts in play: you N` / `(creatures: N)`
   counts are ignored in favor of a hand-count (deck110); fixed guide-side. A future bold/relabel is the
   lever; the number itself needs no change.

8. **Cosmetic: empty `[counters:]` bracket still renders once counters reach 0** (deck44, carried waves
   5–7). Wastes tokens, looks like a glitch; suppress when the list is empty.

**RESOLVED / CLOSED this wave (moved off the top — verify still-fired next wave):**

- **Land-drop enumeration (wave-7 #1) — SHIPPED + FIRED.** deck131 12/12 co-offers, Mountain by name;
  deck110 16 co-offers. The interim hold-to-reach-your-color stopgap DROPPED from the 2-color guides.
- **Gray Merchant drain display (wave-7 #6) — SHIPPED + VERIFIED.** deck133 `{drains 8}` = 8; the "+2"
  workaround DELETED; propagation sweep CLEAN (no other guide carried it — deck44 only lists GM as a
  threat).
- **`(creatures: N)` + `[tapped - untaps...]` tag (wave-7 #3) — SHIPPED + FIRED.** deck140 tag 86×,
  dissolved the tapped/artifact false-belief holds (overshoot 8->0); deck110 count accurate 180/180.
- **No-legal-target PICK-TIME deferral (wave-7 #2) — CURED.** Corpus-wide 1 `fails validation`
  (Glimmervoid's deliberate gate). The "offer-side gap" was a misdiagnosis (see above) — item closed.
- **135v133 Astrolabe turn-2 LIVELOCK — fixed post-corpus** (oracle honors cast restrictions +
  cast-seam livelock breaker); confirmed present, excluded from play-quality.

**HARNESS KNOBS:** use `-j3` (or raise `WAGIC_GPT_TIMEOUT` well above the fallback floor) for the wave-9
review run — control-mirror latency spiked to 52–68s (deck140/deck135) and a control deck makes 90–104
decisions/game, so identical per-decision latency can burn the wall-clock cap while the pilot plays
correctly. IMPROVEMENT vs wave 7: 0 fallback cascades this corpus, so latency now costs wall-clock, not
corrupted decisions (wave-7's vs135 19/34 empty-reply cascade did not recur).

## WAVE-9 WATCH LIST (promote at a 2nd seat unless noted)

- **`damage>=toughness=dead`** core rules-fact (deck131, 1 seat) — a clean core mechanics-fact on a 2nd
  "my N-toughness creature survives N damage" instance; per-deck falsity-fact this wave.
- **Reply-protocol intent-collapse** (deck135, 1 instance) — arm the metric (recorded choice == the
  reply's own concluded option?); the fix is a reply-protocol A/B, judged by decision-quality.
- **Shroud-vs-non-targeted** core mechanics-fact (deck140, 1 seat) — NOT representation-carried; promote
  on a 2nd removal/control seat.
- **Tapped-attacker-still-a-threat** — now representation-carried (the `[tapped...]` tag); watch leaning
  representation (extend the tag if a 2nd deck holds despite it), not core prose.
- **Multi-copy equip churn** (deck110, 1 seat; did NOT recur costly this wave) — promote on a 2nd
  multiples-of-an-equipment deck; the fix is suppress the re-offer once EVERY copy is attached.
- **Free-reaction-card-cost** (deck135 Force of Negation, 1 seat) — need a 2nd free-alt-cost reaction
  deck (Force of Will / Solitude class); per-deck face-burn redirect stands.
- **Snowball-single-threat high-life fire** (deck140, 1 seat) — a real opportunity miss, but legislating
  a high-life fire re-opens the wave-7 overshoot. WATCH, do NOT widen.
- **Decline-a-listed-BODY / phantom-lethal-fixation** (deck109, held 1 seat) — did NOT recur in the body
  shape this wave (a card-specific burn-cost misread appeared instead). Stays 1 seat.
- **Blocker-seam present-and-losing** — watch whether completing the punisher/`[deals 0]` rider (ledger
  #2) actually closes the reflexive-block convergence, or whether the representation is bypassed too.

## VERIFY-FIRED INHERITANCE (grep the wave-9 corpus)

- Re-equip marker still fires ~16/0 (deck110); re-inflate the demoted guide Rule #1 if the no-op floor
  slips. Held stable this wave.
- Land-menu co-offer still 12/12 (deck131) — if it regresses, the land fix regressed.
- `(creatures: N)` + tapped tag still surfaced (deck140) — if removed, the tapped/artifact holds return.
- GM `{drains N}` still the resolved total (deck133).

## DECK-CONSTRUCTION FLAGS STAND (user owns the decklists — restate, don't relitigate)

- **deck131:** threat density 6/60 is now the **#1** flag (manabase DOWNGRADED — the land-menu fix
  recovered the color games it was blamed for 3 waves; red reached the board on curve 5/6). Cut the 4
  Prism Ring for reach/threats; maindeck Spellheart Chimera. Cannot beat lifegain+sweeper at any pilot
  quality without a non-incremental finisher.
- **deck140:** reach starvation + no closer is the ENTIRE loss/timeout column now that the overshoot is
  fixed — the deck needs TEETH (a closer that survives a counter and beats an aggro rebuild) + a little
  early defense/lifegain vs aggro + a couple more untapped red sources; 14 sweepers is the right density.
- **deck135:** manabase strands its 2-color win creatures (blue-thin); slow clock structural. NEW: part
  of the "strand" is piloting (own fixing declined) — addressed guide-side this wave.
- **deck133:** no early blocker (Bloodghast can't block); no reach/flyer answer except Gray Merchant
  (density 4/60, no selection); discard self-damage = the close-race margin.
- **deck44:** black-source density + 1–2 cheap flyers over the 3rd/4th reactive card; the single loss
  ROTATES by matchup/draw (variance) — the reactive half is a slow keep vs a faster ground clock.
- **deck109:** no answer to evasion (flyers/shroud/pro-red); devotion+Obliterator+Gray Merchant (133) is
  a sibling hard matchup. Frozen at 4/6.

## APPLY-STEP WARNINGS

1. **Gray Merchant "+2" sweep: CLEAN.** Only deck133 (the owner) carried the display workaround and it
   deleted it; the other six drafts carry no "+2" GM note (deck44 line 72 merely lists GM as an opponent
   threat to counter). No edits needed to the reviewers' drafts for this.
2. **Do NOT carry deck44's "GFTT stopgap is load-bearing / must-not-demote because the representation
   never shipped" framing.** It rests on a misdiagnosed engine gap (a legal self-target existed; the
   truly-targetless case is already suppressed). Keep deck44's "name an enemy target" fallback as
   STRATEGY (target-by-impact / don't-target-your-own, which the core already covers), not as a contract
   workaround, and do NOT put an offer-side-suppression item on the ledger.
3. **general-strategy.txt is a copy of the LIVE post-A/B prompt** (anchor already removed, commit
   6d02cb68c). Core = PASS; no core apply step. Verified 0 anchor references in the copy.
4. **Run the wave-9 review corpus at `-j3` / raised timeout** so control-mirror decision quality is not
   manufactured by empty replies. The 135v133 Astrolabe livelock is fixed post-corpus and should not
   reproduce.
