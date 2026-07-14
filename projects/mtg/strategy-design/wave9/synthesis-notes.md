# Wave-9 synthesis notes — THE PARKED LEDGER (project pause after this wave)

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-094942` (round-robin, 21 games,
decks 44,109,110,131,133,135,140). Binary `/tmp/wagic-a40671057` (wave-8 engine ledger + own-targets
annotation + cast-restriction gating in the oracle + cast-seam livelock breaker + broadened punisher
rider). Wave-8 guides. Post-A/B core (`hand -> stack` anchor GONE, commit 6d02cb68c). 3 timeouts, all
latency (see ledger #5). Seven seats reviewed. **This is the FINAL wave before a project pause (user
decision 2026-07-14).** Everything below is written for a COLD resume: self-contained, with repros,
ranked.

## Core verdict: PASS (6th consecutive wave, no prompt-body edit)

- No 2+-seat core candidate. `general-strategy.txt` is a verbatim copy of the LIVE post-A/B prompt
  (0 `hand -> stack` references verified). No core apply step.
- **Parked single-seat core candidate:** deck135 "colored mana pays a generic cost / offered =
  payable" (vs140 s24, seat 1784042098: pilot declined an OFFERED `Cast Arcum's Astrolabe with cost
  {1}` reasoning "I have no generic mana, my only land is tapped for {G}" — FALSE, {G} pays a {1}
  generic, and offered = payable). Same FAMILY as the core's existing "Mana pool (none) read as I
  cannot cast." Guide-patched locally (deck135 strategy point 3b). PROMOTION TRIGGER: a 2nd seat
  declining an OFFERED cast on a "colored can't pay generic" / "can't afford the listed option" belief
  -> then add a core line ("any spell shown in your options is payable right now; your colored mana
  pays generic costs"). RESUME ACTION: grep other seats' declines of offered casts for this belief.
- Reply-protocol intent-collapse is NOT a core prompt-body item — it owns the code-appended
  reply-protocol layer (ledger #1 below).

## SKILL adoptions (see the wave-9 changelog in the skill for full citations)

1. **[headline] Pause-aware demotion precondition** (deck109) — demote a guardrail only once its class
   was OBSERVED OBEYED, never merely absent; a closing wave freezes untested guardrails against known
   game-losers. + thin-sample per-class re-anchor validation.
2. **Reply-protocol intent-collapse now MULTI-SEAT (131/135/140/109) — A/B DUE, routing = fixed
   doctrine, + reasoning-vs-outcome metric split (131) + non-numeric-head shape (109).**
3. **else-branch develop-ORDER gates on the OPTION LIST not the HAND + names every SHAPE (fetch-crack),
   de-biased from one color** (deck135 P1); **+ fixed-card-cost reactive exclusion = the ECONOMY rule**
   (deck135 P2).
4. **OPTION-LINE ANNOTATION = a 4th confirmed-fired-representation shape, verified by mis-cast->0 AND a
   reasoning-tax drop; the last rung before the number when a surfaced fact is fabricated over**
   (deck44 P1 + deck133 #3).
5. **Name the invented anti-pattern in 3 more shapes** — inverted-rationale priority entry (deck44 P2),
   resurrection-in-a-new-guise (deck140 Ref2), boundary-value-misresolve = wording defect not
   disobey-wall (deck140 Ref1).
6. **Verify an opponent-rendered representation at the OPPONENT's seat** (deck133 #2).
7. **[ledger-routed] tag-completeness — a READ instruction over a surfaced value only partially kills a
   re-derivation belief; the durable fix is engine tag-completeness** (deck110 E3; parked ledger #3).

## WAVE-9 VALIDATION TABLE (which shipped fixes were confirmed this corpus)

| shipped fix | wave-9 result | evidence |
|---|---|---|
| 135v133 turn-2 Astrolabe LIVELOCK breaker | FIXED — matchup completed (T11 win); Astrolabe casts via `alternative cost {1}` in every deck135 game | deck135 notes; deck133 notes |
| Own-targets annotation ("only legal targets are YOUR OWN") | 47× on deck44, **0/47 self-target casts**; reasoning tax 519 vs 871 chars; 166× corpus-wide | deck44 notes/skill P1 |
| Broadened PUNISHER rider (Obliterator sac text) | SIGHTED + RESPECTED — rendered on incoming Obliterator, both defenders declined the block | OPP135 seq38, OPP131 seq56 (deck133 #2) |
| Chump re-anchor (block ONLY on surfaced LETHAL) | 3/3 correct where windows occurred; high-life gang-block class DIED (was wave-8 misplay) | deck109 blocker table (110 s4/s14, 131 s8) |
| Strict-ordered MULLIGAN gate | reasoning-correct 1/1, keepable-kept 5/5, no false-mulligan; false-KEEP-by-hijack DEAD at reasoning level | deck131 R1, mulligan tally |
| Develop-imperative (Rule #1 else-ORDER) | DIRECT case defect -> 0 (vs44 casts Into the North on curve, takes opp to 2); fetch-crack sub-case persists -> guide revised | deck135 P1 |
| Sweep trigger (count-anchored `(creatures: N)`) | validated a 2nd corpus — 0 overshoots except reply-protocol/wording; both C=0 sweeps traced to reply-protocol | deck140 skill/notes |
| Gray Merchant bare-N drain display | 3 casts drains 6/8/10, all == resolved (incl. own +2 devotion); 3 roles exercised | deck133 #1 (vs110 s28, vs140 s41, vs135 s28) |
| Land-menu enumeration (Mountain co-offer) | Mountain-first 9/9 (10 co-offers; 1 non-Mountain = a correct 2nd-land-drop decline) | deck131 R2 |
| GM combined-lethal self-bridge (positive) | pilot cast GM off-condition (N=10<14) because drain 10 + 7-swing = lethal — self-bridged, no clause needed | deck133 #7 |
| `(creatures: N)` + `[tapped...]` tag | still surfaced + READ accurately (pilot quotes it); 180/180 accurate at deck110 | deck140 #4, deck110 E3 |
| Post-A/B core (anchor removed) | 0 engine no-ops, 0 desyncs, 0 defer, 0 unparsed across all seats — no regression | all seats |

Corpus health: 0 desyncs, 0 defer/`deferred_to_heuristic`, ~1 empty_reply fallback (deck140 vs110 t10,
inside a 120s latency spike), 0 validation failures corpus-wide (1 deliberate Glimmervoid gate).

## RANKED PARKED LEDGER (rank = leverage x cross-seat breadth x strength-of-evidence)

### TOP RESUME ITEM

**1. REPLY-PROTOCOL INTENT-COLLAPSE A/B — now 3-4 seats, DESIGN ARMED, needs a corpus.**
The head-first reply protocol commits the answer token BEFORE the model reasons in its PLAN; when the
PLAN reasons to a DIFFERENT option for the SAME window, the choice stays locked to the stale head. This
is the single highest-leverage open item and the top resume task.
- Witnesses (4 seats, 2 waves): deck131 (vs44 seq1 mulligan — head `1`=Keep, PLAN concludes Mulligan
  and rejects the bait, NOT self-corrected, one-shot -> FINAL, cost the game; file
  `1784046898-ai_baka_deck131-0x559d9770f980.jsonl` seq1); deck135 (vs131 s14 — head `4`=Cast nothing,
  PLAN reverses "wait, I *can* cast Coatl!", self-corrected s18; seat 1784047950); deck140 (2-3
  instances incl. vs131 t40 head cast Wrath into creatures:0 seat `*0x557960b3d5e0*` seq61, vs109 t10
  head Cast-nothing at 1 life seq10; mostly self-heals on the next priority window); deck109 (vs44 seq7
  attackers — NON-NUMERIC head, literal word `none`, parsed to option 0 = no attackers = WORST at the
  attacker seam; file `1784040589-ai_baka_deck109-0x55c31fc62f20.jsonl` seq7).
- Armed A/B (harness, SYMMETRIC core — judged by decision-quality, NOT win-rate):
  - Arm A: accept the LAST bare integer emitted in the reply (post-reasoning), not the first.
  - Arm B: move the CHOICE/number emission to AFTER the PLAN in the reply-format contract
    ("PLAN: ...\nCHOICE: N").
  - Arm C (non-numeric head repair): when the head token is non-numeric, do NOT default to 0 — scan the
    PLAN for the intended option / re-prompt / at minimum log a parse failure.
  - Optional: reconcile a trailing named-option index against the head.
- METRIC to arm across the corpus: count records where reply head != the option the PLAN's final
  sentence names (recorded choice == the reply's OWN concluded option?). vs131 s14 and deck131 vs44 seq1
  are clean labeled positives to score against.
- Distinct from PLAN/CHOICE MISMATCH (there the head matches a genuinely-DIFFERENT intended action;
  here head and reasoning target the SAME window, head simply stale-first). Route to the code-appended
  reply-protocol layer, NEVER re-word a guide over it.

### HIGH (engine/representation)

**2. FETCH / repeatable-activation RE-OFFER — CONSUME-ON-CHOOSE.** deck135 headline + deck133 + deck110.
The wave-7 decline-suppression FIRED and the tax fell further: declined fetch-crack windows/game 62
(wave 7) -> 27 (wave 8) -> **~17 avg wave 9** (deck135: vs140 45, vs44 24, vs133 22, vs109 16, vs110
15, vs131 6; avg 21.3 all-in / 16.6 excl. the vs140 grind). NOT eliminated — the crack re-appears every
priority window as the board changes, so the residual persists and is still the #1 driver of
control-deck decision counts -> timeout/latency exposure (45 of the vs140 seat's priority windows).
deck133 corroborates a single-option "Put in Play with <fetch>" re-ask multiplier (mana unspent).
FIX: once a crack activation is CHOSEN, consume/withdraw it; offer a held fetch's crack at most once per
main-phase window; suppress on opponent turns while tapped out. DETECT via the verbatim
`Put in Play with <fetch> targeting <land>` / `search basic land with <fetch> targeting <land>` strings
across consecutive seqs. Highest live engine leverage; multi-seat.

**3. REPRESENTATION completions (the option-line / tag annotation family).** Ranked sub-items:
  - **(a) `[no legal target]` marker for the ZERO-target case** (deck44). The own-targets annotation
    fires only when the pilot HAS its own creatures as targets; when a mandatory-target removal has ZERO
    legal targets (pilot holds no creature AND opponent holds none) the option renders BARE and the
    pilot still gets confused (repro: `1784046898-...-deck44` s54, a 900+ char plan looping "Young
    Pyromancer is an artifact? No... Wait..."). Add a `[no legal target]` marker (or suppress the
    option). This is what lets deck44's GFTT no-target fallback be FULLY demoted next cycle — until then
    KEEP the fallback (conditional-surface double-coverage).
  - **(b) Thoughtseize / hand-attack-discard EMPTY-HAND annotation** (deck133 #3). When a "target player
    reveals hand, you choose a card to discard" spell is offered and the target's hand is empty of
    discardable cards, annotate: `Cast Thoughtseize {b} - opponent's hand is EMPTY; this only costs you
    2 life`. The fact (`Opponent hand size: 0`) is surfaced AND forbidden in TWO guide spots and the
    pilot STILL fabricates over it (vs131 seq58 T19: "the opponent may have a removal spell") — the last
    rung is the option itself. Generic across seats (any hand-attack discard spell). Repros: vs131 s58
    (hand 0), vs131 s19 (hand 1), vs44 s21 (hand 2, a loss), vs140 s46 (hand 2).
  - **(c) `[deals 0]` / `[not lethal]` / punisher-rider stamp on BLOCKER options** (deck44 + deck109 +
    deck133). The blocker-seam lethal line SHIPPED and holds where windows occur (deck109 3/3), and the
    Obliterator sac-rider now renders + is respected (OPP135 s38 / OPP131 s56) — but the `[deals 0]` gap
    (wave-8 Archmage-blocks-a-0/2-Ornithopter) is unfixed (did not recur only because vs110 had no
    blocker phase). Complete the family: annotate a blocker option with `[deals 0]` / `[not lethal]` and
    the attacker's punisher rider. This is the routing target for the 3-guise reflexive-block
    convergence — NOT a core sentence (rejected 5×).
  - **(d) per-permanent TAG-COMPLETENESS** (deck110 E3). `Artifacts in play: you N` is accurate 180/180,
    but the per-permanent battlefield line tags only SOME artifacts `[artifact]` (Cranial Plating/Elixir
    yes; Darksteel Citadel + artifact CREATURES no). A pilot that RE-DERIVES gets it wrong in BOTH
    directions (under-counts untagged artifact lands/creatures, over-adds Glimmervoid + basics: vs44 s7,
    vs109 s28, vs140 s59/s65). No proven loss yet (the deck floods past metalcraft-3 fast). FIX: tag
    EVERY artifact consistently, leave Glimmervoid/basics untagged, so a re-derived count AGREES with N.
    Method point (into the prefer-surfaced-number rung): verify BOTH the summary offset AND the per-item
    tags agree, not just the summary. Owner: battlefield-serialization in `AIPlayerGPT.cpp`. Low urgency.
  - **(e) empty `[counters:]` bracket cosmetic** (deck44, carried waves 5-9). Renders once counters
    reach 0 (`Sleep-Cursed Faerie ... [counters: 1x st...]`). Wastes tokens, looks like a glitch;
    suppress when the list is empty.

### MEDIUM (infra / harness / evidence-tooling)

**4. CONTROL-MIRROR TIMEOUT — adjudicate by life-at-cap (the timeout column is FAKE).** deck140 + deck131
+ deck135 + deck110. All 3 wave-9 timeouts were latency-starved games the timing-out deck was AHEAD or
even (deck140 vs135 behind-excluded; vs131 grind AHEAD 29/25; vs110 grind). deck140 per-game cumulative
latency: vs135 1259.6s/63 dec, vs131 1356.5s/103 dec, vs110 1297.0s/62 dec — vs the 3 real losses at
239-427s/18-37 dec. Corpus median ~10.4s, p90 ~29s. FIX (either resolves it): (a) a decision-count /
stalemate cap that ADJUDICATES BY LIFE at the cap (results.tsv already fills life0/life1/turn); OR
(b) run quality-sensitive matchups at `-j3` / raised `WAGIC_GPT_TIMEOUT`. Attacked indirectly by ledger
#2 (fewer decisions). ⚠ HARNESS KNOB for the resume run: use `-j3` or a raised timeout — a control deck
makes 90-104 decisions/game and identical per-decision latency burns the wall-clock cap while the pilot
plays correctly.

**5. EVIDENCE-TOOLING (reviewer-facing, carried).** (a) Explicit per-record
`resolved: battlefield|countered|fizzled|graveyard` field — `events` carries the life/zone deltas (used
to verify the 3 bare-N GM casts) but a resolution field removes the manual narration-scrape and prevents
win-mechanism mis-attribution (deck133 #5, deck140 #5). (b) The OPPONENT token in the jsonl FILENAME —
game->file mapping still cross-references results.tsv (deck44). Low effort, high value for every reviewer.

### LOW (single-seat watches — do NOT legislate; full repros for a cold check)

**6.** `damage >= toughness = dead` core rules-fact (deck131, 1 seat; 0 recurrence wave 9 — precondition
did not recur). Promote on a 2nd deck asserting "my N-toughness creature survives N damage."
**7.** shroud-vs-non-targeted core mechanics-fact (deck140, 1 seat; ZERO shroud creatures this corpus —
"no data" is NOT "resolved"). Promote on a 2nd removal/control seat.
**8.** SNOWBALL single evasive threat high-life fire (deck140, no new instance) — a real miss, but
legislating a high-life fire RE-OPENS the wave-7 overshoot. WATCH, do NOT widen.
**9.** free-reaction-card-cost (deck135 Force of Negation, 1 seat) — target-side now covered by the
economy-rule adoption; the alt-cost side still needs a 2nd free-alt-cost deck (Force of Will / Solitude).
**10.** multi-copy equip churn (deck110 E2, 1 seat; did NOT recur costly) — suppress the equip re-offer
once EVERY copy is attached; promote on a 2nd multiples-of-an-equipment deck.
**11.** phantom-lethal / single-attacker-lethal-into-a-blocker (deck109 + deck110 M1, 1 seat each,
non-outcome-changing). deck110 vs109 s25: declared only Memnite (7/1), asserted lethal, opponent chumped.
**12.** within-turn double-sweep at C=0 (deck140, 1 harmless instance vs110 t22 — likely a re-offer +
stale-plan interaction). Promote only if it recurs.
**13.** Liliana-the-Last-Hope +1 as removal not seen (deck133, dormant watch, no diagnostic window).

## DECK-CONSTRUCTION FLAGS STAND (user owns the decklists — restate, do NOT relitigate)

- **deck131:** threat density 6/60 is the #1 flag (manabase downgraded — the land-menu fix recovered the
  color games; red on curve). Two wave-9 losses were CLOSE (vs110 opp 9, vs133 opp 5). +2-3 threats/reach
  (Spellheart Chimera doubles as reach), cut a Prism Ring. No non-incremental finisher = can't beat
  lifegain+sweeper at any pilot quality.
- **deck140:** reach starvation + no closer is the ENTIRE loss/timeout column now the overshoot is fixed.
  Needs TEETH: a closer that survives a counter (Blightsteel {12} folds to one Essence Scatter) + a 1-2
  mana early speed bump/lifegain vs aggro + a couple more untapped red sources for Pyroclasm {1}{r}. 14
  sweepers is the right density.
- **deck135:** manabase strands its 2-color win creatures (Coatl {G}{U}, Viper {1}{G}{G}, Treefolk,
  Diamond Faerie off 2 Snow-Island + 2 Snow-Plains + fetches); slow clock structural. Part of the strand
  was piloting (own-fixing declined) — addressed guide-side this wave (point-3 rewrite).
- **deck133:** no early blocker (Bloodghast can't block); no reach/flyer answer except Gray Merchant
  (density 4/60, no selection); discard self-damage (Thoughtseize 2 + fetch 1s) = the close-race margin
  (a 1-copy Thoughtseize->Inquisition shift cuts it).
- **deck44:** black-source density vs the black spell load + 1-2 cheap flyers over the 3rd/4th reactive
  card. The counters EARN their slots (protected all 4 wins) — "trim the counters" is the WRONG lever.
  The single loss ROTATES by matchup/draw (variance): the all-reactive slow keep vs a faster ground clock.
- **deck109:** no answer to evasion (flyers/shroud/pro-red); no card advantage / mana sink (archetype-
  inherent). Devotion+Obliterator+Gray Merchant (133) is a sibling hard matchup (beaten this wave by
  racing it out). Frozen; do not change at 6/6.

## APPLY-STEP WARNINGS

1. **general-strategy.txt = PASS.** It is a verbatim copy of the LIVE post-A/B prompt (anchor already
   removed, commit 6d02cb68c; 0 anchor references verified). NO core apply step this wave.
2. **Guides that changed this wave (apply these):** deck44 (counter category #2 WHY-as-imperative +
   value-floor face-burn carve-out + SITUATIONS line + observed-pool card names); deck135 (Rule #1 point
   3 develop-branch rewrite: option-list gate + fetch-crack-for-missing-color + offered=payable /
   green-pays-generic; Force discard exclusion; MANA green-fetch clause; uncastable-key-card extension);
   deck140 (SWEEP TRIGGER boundary "16 or LOWER - 16 counts" + "recent turn" wording; tapped invented-
   phrase clause; BSZ partial-sweep guardrail qualification; Lightmine second-copy line). deck109, deck110,
   deck131, deck133 = FROZEN (byte-identical or a single freeze-safe de-specification — deck131 de-
   specified a corpus-stale self-citation). Ship Res guides with `git add -f` (`bin/Res` is gitignored).
3. **Gray Merchant "+2" sweep: CLEAN** — only deck133 carried the workaround (deleted wave 8); no other
   guide references it.
4. **Do NOT re-propose the "offer-side no-legal-target suppression" ENGINE item** — it was a wave-8
   misdiagnosis (a legal self-target exists via 601.2c; the truly-targetless case is already suppressed
   6994/6994). The wave-9 `[no legal target]` ledger item (#3a) is a REPRESENTATION nicety for the
   distinct ZERO-target case, not a contract-violation fix. Keep deck44's "name an enemy target" as
   STRATEGY, not a contract workaround.
5. **Run any resume review corpus at `-j3` / raised `WAGIC_GPT_TIMEOUT`** so control-mirror decision
   quality is not manufactured by empty replies (ledger #4). The 135v133 livelock is fixed — retire the
   wedge caveat from the deck135/deck133 checklists.

## RESUME ORDERING (if the project restarts)

1. **Run the reply-protocol intent-collapse A/B** (ledger #1) — armed, multi-seat, symmetric-core,
   judged by decision-quality. The single highest-leverage open item; touches decks 131/135/140/109.
2. **Engine: fetch consume-on-choose** (ledger #2) — cuts control-deck decision counts, which also
   attacks the timeout column (#4).
3. **Representation completions** (#3): `[no legal target]` marker (finishes deck44's demotion),
   empty-hand annotation (deck133), `[deals 0]`/punisher blocker rider (closes the reflexive-block
   convergence), tag-completeness (deck110), empty-`[counters:]` cosmetic.
4. **Infra: adjudicate-by-life-at-cap** (#4).
5. **Then re-run selfplay** and validate the guide edits that shipped this wave (deck135 fetch-crack
   develop drop toward 0; deck44 counter-inversion class -> 0 vs an affinity seat; deck140 boundary/BSZ).
6. Deck-construction is the user's lever, not the guide's — the OBEYED-BUT-LOSING seats (131/135/140)
   are at their guide terminus; further wins come from the decklists and the harness, not the prose.
