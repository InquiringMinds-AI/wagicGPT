# deck131 wave-24 — dev notes (engine / harness / model / deck-construction; self-contained).

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-125739/`, 6 deck131 games, **0W/6L**.
Binary /tmp/wagic-d7ff8b6c4 (step-1 batch + PRE-GAME MULLIGAN PHASE + N9 hotfix). Pilot: rep_penalty=1.05.
Live guide = wave-23 REVISED (10262 bytes, deployed==wave23 byte-verified). This wave: one additive
BOTTOM-STEP edit -> 10910 bytes. deck131 is deck0 vs 59/137, deck1 vs 133/109/102/27 (see results.tsv).

Seat->game map (results.tsv): vs59 1784915864 (L -5/16 T17) | vs133 1784917071 (L -4/7 T12) |
vs137 1784917739 (adjL 11/15 T12) | vs109 1784917937 (L -3/20 T12) | vs102 1784925296 (adjL 13/32 T14) |
vs27 1784927696 (L -1/12 T18). Zero fallbacks at this seat.

Layer-routing: engine/harness/model items live HERE, never in the guide. Deck-construction flags are
RESTATED for the user, not relitigated.

## Reproducible engine/harness/model ledger

### I-1 — [MODEL-COMPREHENSION, WATCH, guide-unfixable] tapped-state misread RECURRED in a new matchup (vs59 s23)
- vs59 s23 (attackers, T12): reply `ATTACK: A1, A2` (Elemental + Guttersnipe) with reasoning *"Kitchen
  Finks and Restless Apparition... Both are currently tapped... Tapped creatures cannot block."*
- The s23 prompt opponent battlefield: `Kitchen Finks ... [tapped - untaps and can attack next turn];
  ... Restless Apparition {w/b}{w/b}{w/b} (2/2) [persist]`. **Restless Apparition carries NO [tapped]
  tag — it was UNTAPPED.** The model conflated "one tapped" into "both tapped" — the SAME class as
  wave-23 I-1 (vs27 Lord of the Undead).
- Result (s24 events): `Damage: 2 dealt by Restless Apparition to Guttersnipe`; `Your Guttersnipe:
  battlefield -> graveyard` (Apparition also died then returned via persist). 131 lost its reach engine.
- **The wave-23 per-creature [tapped]-read guard did NOT prevent this.** BUT the same guard demonstrably
  WORKED 4/4 in the vs27 matchup it was written for (see I-3). So the guard reduces but does not eliminate
  a stochastic model-comprehension frailty. The prose is already explicit and correct; louder is the
  sanctioned-against move -> guide UNTOUCHED on the guard. Cross-seat WATCH: tapped-state per-creature
  misread is now confirmed at 2 matchups at THIS seat (vs27 w23, vs59 w24) plus the guard's successes;
  promote to a cross-seat combat-safety item if a 2nd SEAT shows it. Repro: for kind=='attackers' with
  Guttersnipe among options, diff the reply's claimed tapped-set vs the prompt's opponent-battlefield
  [tapped] tags.

### I-2 — [HARNESS/MODEL, answer-first extraction, guide-unfixable] CHOICE-line contradicts correct reasoning (vs27 s30)
- vs27 s30 (attackers, T15): the reply REASONS correctly and quotes the guide — *"three untapped creatures
  that can block... you must leave Guttersnipe home... the correct combat declaration is to attack with
  none"* — then a *"Correction: Wait, looking at the prompt again..."* spiral, and the reply's FIRST line is
  `ATTACK: A1`. The harness locked the answer-first first line -> Guttersnipe swung into a 4/3 wither
  Gatewarden and died (s31 events: 4 -1/-1 counters + 4 damage, `Guttersnipe: battlefield -> graveyard`).
- Root: the board read was CORRECT and the guide was FOLLOWED in prose; the failure is answer-first
  first-line extraction overriding a correct conclusion reached later in the reply. NOT guide-addressable
  (no guide prose can fix a first-line-vs-body contradiction). Distinct from I-1 (that was a wrong read;
  this was a right read + wrong extraction). Class: HARNESS answer-first / N9 family. The answer-locked
  RETRY did NOT fire and correctly so — this is a coherent reply, not decode-collapse. Repro: for
  kind=='attackers', compare the reply's leading `ATTACK:` line against its own concluding recommendation;
  a coherent reply whose body concludes "attack none" but whose first line names an attacker is this class.

### I-3 — [GUARD VALIDATION, positive] the per-creature [tapped]-read guard held 4/4 in its origin matchup (vs27)
- vs27 s12/16/21/25 (attackers, T7-13): Guttersnipe available every time; model chose "no attackers" all 4,
  each with a correct per-creature untapped read. **s12 is the exact wave-23 break spot**: `Lord of the
  Undead (2/2)` with NO [tapped] tag, and the model read it as untapped and HELD (last wave it misread the
  same card as tapped and swung into it). No-block discipline also clean 3/4 (s14/18/26). This is the guard
  working on decision-input. Net for the guard: works when complied with (I-3), fails stochastically (I-1),
  and can be overridden by answer-first extraction even on a correct read (I-2).

### I-4 — [HARNESS, RESOLVED] the pregame mulligan phase closed wave-23's I-2 (missing mulligan record)
- Wave-23 vs133 logged NO mulligan ask (I-2: a STEP-1 hand kept without a decision surfacing). This wave,
  ALL 6 games — including vs133 — opened with a proper turn-1 London-mulligan `ask`. The pregame phase
  (86df3b482) is the fix. vs27 additionally exercised a mulligan->bottom path (kind=='bottom') cleanly:
  correct N (1 bottomed at a mull-to-6). No double-asks, no wrong-N, no narration anomaly observed. CLOSED.

### I-5 — [MODEL-ADHERENCE, WATCH, guide-covered] lifegain cast under pressure + non-lethal Guttersnipe block
- Prism Ring / Elixir casts: vs59 T0/T16, vs133 T1/T10, vs109 T1/T5/T7 (life 14, being raced), vs102
  T7/T15, vs27 T1/T11. Most are early low-tempo or leftover-mana turns the guide permits; the vs109 T5/T7
  lifegain at life 14 under mono-red pressure is the guide-forbidden "lifegain in a race" WATCH, and vs27
  s31 is the non-lethal Guttersnipe block (model's own math -> life 1, not 0). All are model-adherence
  softness the guide ALREADY forbids (protect/race discipline relaxes under pressure). Single instances,
  none decisive. Do NOT escalate the frozen lifegain/never-block lines off these (sanctioned-against).

## Interface validations (clean this corpus)
- **Mulligan 6/6 correct by STEP-1** (5 KEEP with 1-2 Mountains + YP; 1 MULLIGAN of a no-red hand, vs27).
- **Guttersnipe attack discipline 7/8** (held all untapped boards, swung all all-tapped boards); 1 miss (I-1).
- **No-block discipline 3/4** ("no blockers" on non-lethal; 1 non-lethal Guttersnipe block, I-5).
- **Attacker declarations pure-index/name, all parsed** (no name-leak fallback; 0 fallbacks total).
- **Zero fallbacks** (no empty_reply/retracted_choice/stale_echo/unparsed) — cleanest deck131 corpus.

## DECK-CONSTRUCTION ledger (RESTATED for the user; user owns; not relitigated)
The standing 8+-wave diagnosis holds and the 0/6 is its clean expression (see findings MANDATE 1):
- **Threat density 6/60** (3 Guttersnipe + 3 YP). Three games ran on a SINGLE payoff (137, 109, 102) —
  variance the deck cannot absorb; two games (59, 133) had BOTH payoffs and STILL lost the race.
- **No reach vs lifegain** — vs102 (opp to 32) is the textbook loss; 131 could not push damage.
- **No early defense vs aggro** — vs109 (mono-red) killed 131 by T12 with 131 dealing ZERO damage.
- **Cannot out-race bigger/recursive decks** — vs133 (curved both payoffs, opp to 7, still died) and
  vs59 (persist go-wide, only 4 damage dealt) are the ceiling: the deck's clock is too slow for the field.
- Manabase 14 Island / 8 Mountain: no screw loss this corpus (mulligan gate + 5/6 kept 1-2 Mountains).
- Sideboard candidates unchanged (Spellheart Chimera reach, Aetherling finisher) for the no-reach axis.
- These are the user's to decide; recorded, not argued.

## For a future deck131 reviewer — gotchas
- n=6, single run: win-rate is noise; 0/6 here vs 1/6 wave-23 vs 2/6 in the quarantined N9-overfire run
  are all the SAME construction-terminal deck under draw variance — do not read a trend.
- deck131 is deck0 in vs59/vs137 rows and deck1 in the rest THIS corpus (positions vary per row) — read
  the my_life field in each record, do not assume a fixed column.
- Adjudicated games (vs137, vs102) write NO gameend for this seat; read final life from records/results.tsv.
- **Score Guttersnipe attack windows against the ACTUAL [tapped] tags** (I-1) AND check the first
  `ATTACK:` line against the reply's own conclusion (I-2) — two distinct failure modes this corpus.
- The `bottom` record (kind=='bottom') is the new London-mulligan bottom decision — check it bottomed the
  lowest-value cards (Prism Ring/Elixir first); vs27 s3 kept Prism Ring, the miss the new guide line targets.

## ROTATION VERDICT

**DO NOT ROTATE OUT — guide MODIFIED this wave (Gate 1 fails) AND new-work signal present (Gate 2).**
- Gate 1 (guide-mod?): FAILED to be a candidate — the guide WAS modified (additive BOTTOM-STEP line for
  the brand-new London-mulligan bottom phase, addressing the observed vs27 s3 miss). A revised guide is
  never a rotation candidate; it must re-validate next corpus (does the model now bottom Prism Ring/Elixir
  first on a mulligan keep?).
- Gate 2 (new-work-signal?): also present. (a) The tapped-read guard is only PARTIALLY validated — it held
  4/4 in vs27 but a fresh tapped-misread appeared vs59 (I-1); the frailty is not closed and needs more
  matchups with partly-tapped blocker boards. (b) A NEW harness/model failure surfaced (I-2 answer-first
  first-line overriding a correct conclusion) — a cross-seat candidate worth watching. (c) The pregame
  bottom phase is a first-outing surface with one observed suboptimal decision.

**If the 0/6 is construction-terminal — say so plainly for the roster owner:** YES. This 0/6 is a
CONSTRUCTION TERMINUS, not a piloting failure. Zero fallbacks, mulligan gate 6/6 correct, Mountain-first
clean, the Guttersnipe guard working in its origin matchup — the pilot played the deck correctly and it
still lost every game because the deck cannot race the field (too few threats, no reach vs lifegain, no
early defense vs aggro). No guide edit changes this record. The lever is DECK CONSTRUCTION (threat
density / reach / early interaction), which is the owner's call — the sideboard already carries the
candidate swaps (Spellheart Chimera, Aetherling). deck131 is not rotation-eligible until the bottom-line
and the guard both re-validate AND the seat surfaces no new work — earliest is the wave-25 review.
