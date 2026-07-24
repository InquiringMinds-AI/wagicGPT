# deck131 wave-23 — dev notes (engine / harness / model / deck-construction; self-contained). ROTATED IN.

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-013710/`, 6 deck131 games, **1W/5L**.
Binary `/tmp/wagic-0e6361732`. Pilot live: repetition_penalty=1.05. Live guide = wave-11 FROZEN
(9036 bytes), now revised this wave (one surgical Guttersnipe-attack edit -> 9902 bytes).
deck131 = deck1 (deck0 is the opponent) in EVERY results.tsv row this cycle.

Seat->game map (results.tsv):
vs140 1784875030 (adjL 20/27,T26 — OUTAGE game, 4 empty_reply s3-6) | vs137 1784878502 (L -2/27,T12) |
vs109 1784880409 (L 0/20,T8) | **vs102 1784880626 (adjWIN 12/3,T15)** | vs133 1784884240 (L -2/22,T16) |
vs27 1784886254 (L 0/20,T14).

Layer-routing: engine/harness/model items live HERE, never in the guide. Deck-construction flags are
RESTATED for the user, not relitigated.

## Reproducible engine/harness/model ledger

### I-1 — [MODEL-COMPREHENSION, GAME-DECIDING, guide-fixed] Guttersnipe attacked into an UNTAPPED blocker MISREAD as tapped (vs27 s15)
- vs27 s15 (attackers, T7): reply `ATTACK: A1` (Guttersnipe) with reasoning *"the opponent has no
  untapped creatures to block (Vodalian Zombie and Lord of the Undead are both tapped)."*
- The s15 prompt opponent battlefield line: `Vodalian Zombie (3/3) [tapped - untaps and can attack
  next turn]; ... Lord of the Undead {1}{b}{b} (2/2)`. **Lord of the Undead carries NO [tapped] tag —
  it is UNTAPPED.** The model conflated "one creature is tapped" into "all are tapped."
- Result (s16 events): `Damage: 2 dealt by Lord of the Undead to Guttersnipe`; `Your Guttersnipe:
  battlefield -> graveyard`. 131 lost its only reach; opp life stayed 20 the rest of the game.
- Root = MODEL comprehension (the representation is correct and unambiguous — tag present on one,
  absent on the other). NOT an engine bug (a tapped creature did NOT block; the untapped one did).
- Guide-side mitigation applied this wave (per rotation new-leak doctrine): a Guttersnipe-only attack
  guard instructing a PER-CREATURE [tapped]-read. Cross-seat WATCH candidate for the synthesis:
  tapped-state per-creature misread on partly-tapped boards is a general combat-safety comprehension
  risk, not deck131-specific. Repro: for `kind=='attackers'` where Guttersnipe is A1, diff the reply's
  claimed tapped-set against the prompt's `[tapped]` tags on the opponent battlefield line.

### I-2 — [HARNESS/ENGINE, VERIFY] no mulligan decision surfaced for vs133 (a STEP-1 mulligan hand kept without a choice)
- vs133 opening hand (from s1 game-log): `Downsize; Essence Scatter; Young Pyromancer; Island; Island;
  Island; Downsize` — no Mountain / no Artificer's Epiphany / no Opportunity = a guide STEP-1 MULLIGAN.
- **No `ask` record with "Mulligan decision" in its prompt exists for this game.** s1 is already a
  land-drop ask ("Play Island now?"). Every OTHER deck131 game this corpus logged a mulligan record;
  vs133 did not. 131 flooded (0 payoffs cast) and lost.
- VERIFY: was 131 denied a mulligan step here (engine/harness), or is the mulligan for this seat/side
  simply not emitted to the log under some condition (e.g. on-the-play vs on-the-draw, or a decision
  the heuristic auto-resolved)? If the pilot never gets to answer the mulligan on some games, the whole
  STEP-1 gate (this guide's most-cited line) is silently skipped on those games. Route: HARNESS
  mulligan-emission ledger. Repro: grep each deck131 game for an `ask` whose prompt contains "Mulligan
  decision"; vs133 has none.

### I-3 — [ENDPOINT/INFRA, transient] the 4 empty_reply (vs140 s3-6) are a CORPUS-START transient, NOT the rep_penalty pilot
- All 4 latency ~=2502ms (uniform HTTP fast-fail). deck131's vs140 game is the EARLIEST-started game
  of all 21 (start_epoch 1784875030, rank 1/21). The empties cluster in the earliest games (mine +
  deck140 s5-7, deck133). Later requests WITHIN the same games succeeded with rp=1.05 in payload.
- Conclusion: endpoint warmup/outage at corpus start, not a per-request repetition_penalty rejection.
  Board impact of the outage on vs140: NONE (see findings Mandate 1 — 131's life never moved; game
  lost on the lifegain-stall construction terminus at T26, Guttersnipe not resolved until T27).

### I-4 — [GUIDE-vs-ENGINE, latent, no harm this corpus] #1-RULE overpromises target-gated reaction spells as no-target Guttersnipe fuel
- Guide #1 RULE: *"cast ... Hydrolash, Aetherize (deal 2 even with no attackers to bounce) ... even if
  they do nothing else."* Engine reality: Aetherize/Hydrolash require attacking creatures as a legal
  target and are NOT offered on your own main phase with no attackers (vs27 s14: Guttersnipe out, own
  MP1, Aetherize NOT in the cast options — also unaffordable there). Essence Scatter (needs a creature
  spell on the stack) similarly never a no-target fuel option.
- No game impact this corpus (the model never attempted an unoffered spell; it correctly "cast nothing"
  when only lifegain was offered). Left UNTOUCHED in the guide (below the bar — no harm, and the engine
  simply doesn't surface the option so the false claim can't be acted on). WATCH: if a future corpus
  shows the model fixating on / mis-echoing an unoffered Aetherize as an available fuel option, revisit.

## Interface validations (clean this corpus)
- **Mountain-first 11/11** co-offers chose Mountain (6th consecutive clean corpus; wave-8/9/10/11/13/23).
- **Attacker declarations 4/4 pure-index** (`ATTACK: A1` x4), 0 mixed, 0 pure-name -> the wave-13 L2
  name-leak class NOT reproduced at this seat (small n=4, all single-attacker A1 windows). The (d)
  ATTACK-line CoT-hijack fix: no combat-math prose hijack observed (deck109 owns (d)).
- **Mulligan keeps 5/5 correct where offered** (vs140/137/109/102/27 all had Mountain + a payoff/draw ->
  guide-correct KEEP). The wave-13 L1 mulligan STEP-1 misfire did NOT recur (no no-red hand was KEPT via
  a mulligan decision this corpus). vs133's screw was a NON-offered mulligan (I-2), not a misfire.

## DECK-CONSTRUCTION ledger (RESTATED for the user; user owns; not relitigated)
The standing 8-wave diagnosis holds; the losses remain construction-bound and the win is the YP-swarm
2nd-win-path plus variance:
- **Threat density 6/60** (3 Guttersnipe + 3 Young Pyromancer). vs140 fielded a lone/late payoff into a
  lifegain wall; vs137 single YP no Guttersnipe; vs109 lost payoffs to aggro tempo; vs133 drew NO red at
  all. The 1 win (vs102) came from an unmolested YP + token race, not a play change.
- **No reach vs lifegain** — vs140 is the textbook loss: 131's life never dropped (20 all game) yet it
  lost because it could not push damage through a lifegain deck; Guttersnipe resolved T27, after the
  adjudication turn. Same shape as wave-13's vs140.
- **No early defense vs aggro** — vs109 (mono-red) killed 131 by T8; the deck has no cheap blocker /
  early interaction, only reactive counters that don't stop a fast board.
- **Manabase 14 Island / 8 Mountain leans wrong for two red win conditions** — vs133 is the direct cost
  (flooded on Islands, never drew the red to cast either payoff). Interface clean (Mountain-first 11/11);
  a draw/count problem, not an unofferable-menu artifact. Sideboard carries Spellheart Chimera (reach) +
  Aetherling (evasive finisher) as candidate swaps for the no-reach axis.
- These are the user's to decide; recorded, not argued.

## WATCH (single-seat, unscored)
- **tapped-state per-creature misread** (I-1) — promote to a cross-seat combat-safety item if a 2nd seat
  shows the model misreading a partly-tapped opponent board. deck131's guide now mitigates it locally.
- **lifegain-in-a-race** (vs137 s14/s16 Prism Ring + Elixir while being raced; vs27 durdle-lifegain post-
  payoff) — the model relaxes the guide's lifegain de-prioritization under pressure. Guide already
  forbids it (model-adherence, like wave-13 L1). Do NOT escalate the frozen lifegain lines off this.
- **chump-block-when-not-lethal** (vs109 s12, YP into Hellrider at 12 life, non-lethal incoming) — guide-
  covered ("no blockers unless incoming puts you at 0"); model-adherence, single instance, don't touch.

## For a future deck131 reviewer — gotchas
- n=6, single run: win-rate is noise. deck131 = deck1 in EVERY results.tsv row this cycle (winner `0`
  or `adj0` = OPPONENT won; `adj1`/`1` = 131 won). Only vs102 (adj1) is a 131 win.
- Adjudicated games (vs140, vs102) write NO per-seat `gameend`; read final life from results.tsv life0/
  life1 by position (131 = life1).
- **Score Guttersnipe attack windows against the ACTUAL [tapped] tags** — the model can claim "all
  tapped" when only one is (I-1). For `kind=='attackers'`, diff the reply's claimed tapped-set vs the
  prompt's opponent-battlefield [tapped] tags before trusting a "safe swing" rationale.
- **Check for a MISSING mulligan record** (I-2) before attributing a screw loss to a keep-misfire — vs133
  was never offered the mulligan at all.

## ROTATION VERDICT

**DO NOT ROTATE OUT — revised this cycle, carries a new-work signal by construction.**
- Gate 1 (guide-mod?): FAILED to be a rotation candidate — the guide WAS modified this wave (one
  surgical Guttersnipe attack-side edit filling a game-deciding gap the rotated-in Zombies pool
  exposed). A revised guide is never a rotation candidate; it must be re-validated next corpus.
- Gate 2 (new-work-signal?): also present — the Guttersnipe attack-trade fix is UNVALIDATED (the
  vs27 misread must be re-tested against a matchup that presents a partly-tapped blocker board), and
  two harness/engine items are open at this seat (I-1 tapped-misread cross-seat WATCH, I-2 missing
  mulligan record on vs133).
- Re-validation focus next corpus: (1) does the pilot now HOLD Guttersnipe back when an opponent
  creature lacks the [tapped] tag? (needs a matchup with creature blockers — Zombies/aggro), and
  (2) does a mulligan decision get offered on every game, or was vs133 a genuine harness omission?

deck131 is NOT at rest: it just discharged 12 waves of frozen status by finding a real leak, and the
fix needs its confirming corpus. Rotation-eligible only after the Guttersnipe attack-guard validates
AND the seat surfaces no new work — earliest is the wave-24 review.
