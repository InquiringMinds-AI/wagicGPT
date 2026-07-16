# Deck-135 (Modern Snow) — Wave-15 game review

Corpus: PRIMARY `matchups-20260716-040138` (21-game round-robin over 44/110/133/135/140/17/21;
binary `/tmp/wagic-aad32bc7f` = target-cancel use-after-free fix + absent-echo staleness route +
attackers-name reconcile + Galvanic metalcraft fix). Wave-11 guide live = current
`deck135_strategy.txt` (FROZEN, byte-identical since wave-11). Core prompt unchanged. All six
deck135 seat games read decision-by-decision; opponent seats read at Force/combat seams. Doctrine:
win column is CONTEXT ONLY; unit of analysis is the DECISION with seq repros, layer-attributed;
adopt-a-revision bar is HIGH (veteran seat). deck135 owns: (#4) the Force async response-window
race (RESIDUAL-RACE) and — the headline — any COMPLETED live alt-cast counter; the board-line
annotation contradiction class (stale `[attacking]` + inverted freeze); the 0/6→2/6 trajectory read.

## RECORD (context only — NOT a verdict) — 2W / 4L

| game    | opp                    | outcome (deck135)              | my/opp life | turn | class |
|---------|------------------------|--------------------------------|-------------|------|-------|
| 135v140 | 140 Wipe Them Out!     | **WIN (real, won=True)**       | 18 / 3→dead | 16   | **FORCE-COUNTERED the board wipe → win** |
| 135v17  | 17 Faeries (guideless) | **WIN (adj, ahead)**           | 13 / 4      | 20   | creature-war grind, adjudicated ahead |
| 135v21  | 21 Goblin Gang         | LOSS (adj, behind)             | 8 / 17      | 14   | aggro race; RESIDUAL-RACE ate the Treefolk-save |
| 135v110 | 110 Etched Affinity    | LOSS (real, won=False)         | 8 / 20      | 8    | fast aggro + T1 creature-skip misplay |
| 135v133 | 133 Phyrexian/Oblit.   | LOSS (real, won=False)         | -6 / 14     | 12   | construction; T9 creature-skip misplay |
| 135v44  | 44 Faerie Bladecrafter | LOSS (real, won=False)         | 1 / 20      | 14   | construction (no answer to evasive 6/6) — same as wave-13 |

The record LIFTED from wave-14's 0/6 to 2/6. **The lift is part real capability gain, part variance**
(see the trajectory read). The seat's ceiling remains CONSTRUCTION (gold/double-pip creatures off a
2-Island splash → color screw + slow clock + small bodies can't race lifegain/aggro), unchanged from
wave-13. But this corpus produced the single most important seat event in the project's Force history.

---

## Q1 — FORCE OF NEGATION (owned ledger #4): the FIRST COMPLETED LIVE ALT-CAST COUNTER ever captured — deck140 seq18. Plus 3 clean RESIDUAL-RACE repros with a NEW mechanistic correlate.

Force was OFFERED as a cast 6 times and the model chose to CAST all 6 (a behavioral shift from
wave-13's single-offer/correct-decline). Of the 6, **2 reached the stack and COUNTERED their target;
3 were answered but produced NO execution (RESIDUAL-RACE); 1 was confounded by game-end.** Force
was NEVER offered against face-burn or discard this corpus, so the guide's core Force spine (never
2-for-1 a face-burn/discard) was not stress-tested on the decline side.

### CELEBRATE — deck140 seq18→seq19: the free alt-cast Force COUNTERED a board wipe. FIRST EVER.

- **seq18** (T9, opponent's turn, Black Sun's Zenith on the stack): offer rendered as the gated
  free alt-cast `Cast Force of Negation with its exile a blue card from hand cost - can target on
  the stack: Black Sun's Zenith`. deck135 held `{w}` from 1 untapped source. The model reasoned it
  perfectly and cited the guide by name: *"Black Sun's Zenith, a board wipe… The strategy guide
  explicitly states to use Force of Negation on board wipes like Black Sun's Zenith. I will exile a
  blue card (Search for Azcanta) to counter this spell for free."* choice=1.
- **seq19 events (the resolution): `Your Force of Negation: hand -> stack | Opponent's Black Sun's
  Zenith: stack -> EXILE | Your Force of Negation: stack -> graveyard`.** The wipe went
  **stack → exile (COUNTERED)**, NOT stack → battlefield (resolved). **This is the first COMPLETED
  live alt-cast Force counter ever captured at any seat — and it is a free-exile alt-cast, the exact
  path that was unvalidated end-to-end through wave-13 (0 casts). ENGINE #1 (Force never counters)
  is, on this evidence, FIXED.** It was ALSO the load-bearing play of the game (see the win chain).
- **seq30→seq31: the HARDCAST Force ({1}{u}{u}, 7 untapped sources) countered Lightmine Field** →
  `Lightmine Field: stack -> EXILE`. Second completed counter, this one on hard mana, protecting the
  T15 alpha strike. Lightmine Field is on the guide's explicit Force-target list — correct pick.

**vs140 win chain (both counters load-bearing):** T8 opp at 24, Treefolk cast. T9 Black Sun's Zenith
COUNTERED (board preserved). T10 opp 24→12 (Treefolk+Golem swing). T13 opp 8, T15 opp 3, Lightmine
Field COUNTERED protecting the final push → won=True. **A game that a resolving board wipe would have
reset was converted into a win by the now-working Force resolution.** This is the concrete proof the
resolution fix matters, and it directly drives half of the 0/6→2/6 lift.

### RESIDUAL-RACE — 3 answered Force alt-casts produced NO execution (owned ledger #4 repros)

Confirmed by grep: deck21/deck110/deck133 each have **0** `Force of Negation: hand -> stack` events —
the chosen cast never reached the stack; the targeted spell resolved anyway.

| seq | target | my mana avail | result |
|-----|--------|---------------|--------|
| deck21 seq27 | Volcanic Hammer (→Treefolk) | **(no untapped sources)** | Hammer resolved (3 dmg), Force never cast |
| deck21 seq29 | Volcanic Hammer #2 (→Treefolk) | **(no untapped sources)** | Hammer resolved, Treefolk DIED (3+3=6 on the 6/6), Force never cast |
| deck110 seq4 | Cranial Plating | **(no untapped sources)** | Plating resolved → battlefield, Force never cast |
| deck133 seq24 | Liliana of the Veil | {g}{u}{w} (3 sources) | game ended next seq — CONFOUNDED, inconclusive |

**NEW mechanistic correlate (the sharper diagnosis of ledger #4):** every RESIDUAL-RACE failure this
corpus occurred with **"(no untapped sources)"** on deck135's board; the one alt-cast that EXECUTED
(seq18) had `{w}` from 1 untapped source available. This **is NOT a latency race** — deck140 seq30
executed at 92,954 ms latency while deck21 seq27 failed at 8,127 ms. The clean separator is
untapped-mana presence, not reply time. Hypothesis: **the free alternative-cost execution path still
routes through the mana-payment / priority machinery and silently fails when the caster controls zero
untapped sources**, even though the alt-cost requires no mana. The round-3 offer-legality gate checks
only the exile target (a blue card in hand), not untapped mana, so the offer is legal but the
execution fizzles. This is exactly the ledger #4 "answered Force cast produces no execution," now with
a reproducible trigger. **It bites hardest precisely where the deck needs it** — tapped out on the
opponent's turn after deploying, facing a removal/wipe — which is the alt-cast's entire reason to
exist. In vs21 it cost deck135 its Abominable Treefolk finisher (seq27 fizzle → two stacked Volcanic
Hammers killed the 6/6 at seq29/seq30) and materially contributed to that adjudicated loss.
(deck133 seq24 had untapped mana but the game ended before the stack could resolve, so it neither
confirms nor refutes the correlate.) HIGH-value engine item → notes.md ENGINE #1 (reframed with the
new trigger + the celebrated seq18 counter).

### Force DECISION quality (independent of execution) — mixed; a discipline leak on Cranial Plating

- **CORRECT (guide-cited):** deck140 seq18 (Black Sun's Zenith = board wipe) and seq30 (Lightmine
  Field = on the guide's list). Both executed and countered.
- **DISCIPLINE ERROR:** deck110 seq4 — Force on **Cranial Plating** (an equipment/artifact) on T2
  with deck135's board EMPTY (0 creatures). Cranial Plating is NOT on the sanctioned list (wipe /
  mass-bounce / removal-on-your-only-threat) and there was no board to protect. The model
  rationalized "stop this threat." Cost was zero only because it also fizzled (RESIDUAL-RACE); the
  DECISION was wrong. Model-adherence miss; the guide's Force spine already excludes it.
- **DAMAGE-MATH ERROR (premise wrong, outcome-vindicated):** deck21 seq27 — Force on Volcanic Hammer
  "to save my Treefolk," but Volcanic Hammer deals 3 and Treefolk is a 6/6 (3 dmg is NOT lethal to
  6 toughness). The model then AT seq29 correctly noticed "Treefolk is a 6/6, survives 3 damage." So
  the seq27 premise was a damage-vs-toughness error (same family as deck110's known model rules gap).
  Ironically two stacked Hammers (3+3) later did kill it — so forcing the first one was outcome-right
  by luck, wrong by the reasoning the model gave.
- **MOOT:** deck133 seq24 — Force on Liliana of the Veil at 4 life facing lethal on board (5/5
  trample + 3/2 = 8 power vs 4 life). Countering a planeswalker did not address the lethal; deck135
  died the next seq regardless. Defensible-but-moot in a lost position; not on the sanctioned list.

**Net Force behavior shift:** wave-13 = 1 offer, correctly declined (perfect discipline). Wave-15 =
6 offers, all cast, 2 correct-and-executed (the wins), 1 clear discipline error, 1 math error, 1
moot. The model has swung from disciplined-decline to reflexive-cast. It gets WIPES right (the two
that mattered) but over-Forces marginal noncreature spells (Cranial Plating). Because the two wipe
Forces are the ones that executed and won a game, the net was positive this corpus — but the
reflexive-cast tendency is a watch item (model-side; the guide already scopes Force correctly).

## Q2 — BOARD-LINE ANNOTATION CONTRADICTION (owned): stale `[attacking]` is PERVASIVE; the inverted-freeze (wave-13 s19 shape) did NOT reproduce in a live decision.

- **Inverted freeze — NOT reproduced.** deck135 had one Treefolk ETB tap-and-freeze this corpus:
  deck44 seq26 chose Faerie Bladecrafter. The game ENDED at seq28 (two windows later), so the frozen
  Bladecrafter never had to be rendered at a later combat window — the specific wave-13 defect (a
  frozen creature annotated `untaps and can attack next turn`) had no opportunity to manifest in a
  decision. deck140's Treefolk froze nothing (auto/no untapped target). **The inverted-freeze
  contradiction is neither confirmed fixed nor reproduced this corpus — carried as UNVALIDATED.**
- **Stale `[attacking]` — WIDESPREAD and still wrong.** ~20+ battlefield lines render a tapped
  creature as `[tapped - untaps and can attack next turn] [attacking]` during ASK / priority /
  main-phase windows where NO combat is active (e.g. deck17 seq33–45 tag Ohran Viper / Icehide Golem
  `[attacking]` all through main-phase 2 windows; deck140 seq22–23 tag Treefolk+Golem `[attacking]`
  post-combat). A creature is only "attacking" during the combat it is declared in; the tag lingering
  into later phases is stale — the same class as wave-13 ENGINE #2's `[attacking]` half. This corpus
  it did NOT drive a decision wrong (all instances were in windows the model resolved or in the won
  vs17), so it is representation-noise here, but it is the identical defect and stands on the ledger.
- **Adjacent misread it enabled — deck133 seq9 (blockers).** The opponent board showed Bloodghast
  `[... untaps ...] [attacking]` and Geralf's Messenger `[... untaps ...]` (NO `[attacking]`). Only
  Bloodghast was a declared attacker. The model MISREAD Messenger as an attacker, emitted an illegal
  `BLOCKS: B1:A2` (A2 = Messenger, not attacking) → parser could not apply → unparsed_reply →
  heuristic no-block (choice=0). The heuristic outcome (take 2 from Bloodghast at 17 life) was the
  correct play anyway, so zero cost — but it shows the two near-identical `untaps…` tags (one with,
  one without `[attacking]`) are a genuine parse hazard for the model. Low stakes; noted.

## Q3 — TWO Rule #1 creature-skips (model-adherence; both in LOSSES). deck110 s3 fires the wave-13 CORE promotion trigger.

The guide's most-repeated instruction is "a creature in your Cast options ALWAYS beats a fetch /
Astrolabe / dig; anything shown in your options is payable RIGHT NOW; read the OPTION LIST, not your
hand." The model violated it twice, both times by OVERRIDING the offered creature with hallucinated
reasoning:

- **deck110 seq3 (T1) — chose Arcum's Astrolabe over an OFFERED Icehide Golem, on a FALSE mana
  belief.** Options: `[Cast Arcum's Astrolabe {1}, Cast Icehide Golem with its alternative cost {1}
  (2/2), Cast nothing]`. The model wrote *"I lack green mana to cast Icehide Golem (which requires
  {1}{g})… I cannot cast the Golem this turn"* and cast Astrolabe. **Icehide Golem was IN the offered
  list at `{1}` — therefore payable by the guide's own rule — and the model invented a `{1}{g}`
  requirement to decline it.** This is an ACTUAL offered-payable creature decline driven by a
  colored-cost hallucination — exactly the **promotion trigger** wave-13 named for the
  colored-pays-generic / trust-the-option-list CORE candidate ("cross-seat grep for an ACTUAL
  offered-payable decline is the promotion trigger"). Against fast Etched Affinity aggro, skipping
  the T1 2/2 blocker mattered; deck135 lost this game. Routes to general-suggestions.md as a
  cross-seat CORE promotion candidate (single-seat items cannot edit core lines).
- **deck133 seq18 (T9) — chose Glacial Revelation (a dig) over an OFFERED Diamond Faerie (3/3
  flyer).** Board EMPTY, opponent has Phyrexian Obliterator 5/5 trample + Geralf's Messenger. Mana
  `{g}{g}{u}{u}{u}{w}` (6 sources; Faerie needs 5 → payable, and it was in the list). The model wrote
  *"cast Glacial Revelation… to find a creature to block"* — the precise "I will dig for a bigger
  threat first" losing thought the guide names — **while a 3/3 flyer was already castable.** The
  guide's DECIDING SITUATIONS literally reads "This includes Diamond Faerie - a 3/3 flyer beats a
  dig." Model disobeyed a verbatim guide line; deck135 lost. (The spot is genuinely hard — a 3/3
  can't profitably block a 5/5 trample Obliterator — but per the guide the body deploys.)

Both are MODEL-ADHERENCE failures against instructions the FROZEN guide already states as explicitly
as possible. Per doctrine a 3rd restatement is the sanctioned-against escalation, so the guide stays
frozen; deck110 s3 promotes the CORE candidate, deck133 s18 routes to model watch.

## Q4 — Fallbacks / protocol.

- **13 fallbacks this seat: 12 `unparsed_reply` + 1 `stale_echo`.** `empty_reply = 0` (Spark-death
  flood stays closed, 3rd corpus).
- **11 of the 12 unparsed are long-latency TIMEOUTS** (155k–197k ms) where the reasoning ramble was
  cut by the ~0.75-preemption latency wall — the guard discarded them to heuristic; NONE flipped a
  game. The 12th (deck140 seq23) emitted `CHOICE: None (Pass/End Turn)` — a non-index reply the guard
  correctly rejected. deck133 seq9 (blockers) was the illegal-block drop above.
- **1 `stale_echo` — deck21 seq26 — a CORRECT catch (not a wrongful downgrade).** The reply named
  `CHOICE: 1 (Put in Play with Flooded Strand targeting Snow-Covered Forest)` into a 2-option list
  offering **Island / Plains** (Forest NOT offered). The absent-echo staleness route correctly routed
  it to heuristic rather than mis-mapping an unlisted name. Matches the brief's description of
  stale_echo as correct menu-confusion catches; no wrongful downgrade at this seat.
- **Name-echo compliance ~100%**; every CHOICE line carried `(name)`; no echo-vs-index conflict
  resolved wrong.

## Q5 — attackers name→index parse gap (wave-13 HARNESS #1): still LATENT, still 0 cost.

The model again emits mixed `ATTACK: A1, <Name>` declarations (deck21 s22/s33 `A1, Abominable
Treefolk`; deck140 s17 `A1, Abominable Treefolk`; deck17 s30 `A1, Ice-Fang Coatl, Ohran Viper,
Icehide Golem`). The parser takes the leading A-index and drops the names. **Every dropped name was
an ILLEGAL attacker** — a just-cast summoning-sick creature (Treefolk cast the same turn; the three
creatures at deck17 s30 were all cast at s27–s29 the same turn, with only A1 Boreal Druid offered).
The clamp was correct every time; 0 outcome cost. Still no outcome-coupled instance (a NAMED legal
A2+ attacker being dropped) — HARNESS #1 stays latent. Companion model tic (summoning-sickness
blindness in attack declaration) unchanged; harmless (the guide already says "count ONLY the
creatures offered in the Attackers list").

## 0/6 → 2/6 TRAJECTORY READ (owned)

The record moved 1/6 (wave-13) → 0/6 (wave-14) → **2/6 (wave-15)**. Decomposed:

1. **Half the lift is a REAL capability gain: the Force resolution now works.** The vs140 win is
   directly caused by the free alt-cast Force COUNTERING Black Sun's Zenith (seq18) — a board wipe
   that, resolving, would have reset deck135's board and killed the clock. Through wave-13 this was
   the top unfixed engine item (ENGINE #1, 0 casts, unvalidated). This corpus it fired, exiled the
   wipe, and won a game. That is a genuine, mechanism-level improvement, not variance.
2. **Half the lift is VARIANCE: the vs17 win is an adjudication in deck135's favor** (ahead 13/4 in a
   grindy creature war). It is the mirror image of wave-13's vs110 (a board-winning game robbed by
   the life tiebreak). Over enough grindy games the latency-adjudication coin lands both ways; this
   corpus it landed for deck135 once. Read it as noise, not a play-quality signal.
3. **The 4 losses are the SAME construction ceiling as wave-13**, plus two self-inflicted misplays:
   vs44 is a near-identical repeat (no answer to an evasive 6/6 Faerie Bladecrafter, dead at 1);
   vs133 and vs110 are construction losses COMPOUNDED by the two Rule #1 creature-skips (Q3); vs21 is
   an aggro race where the RESIDUAL-RACE fizzle (Q1) ate the Treefolk-save and hastened the loss.
4. **Net read:** the seat is still construction-bound and latency-adjudicated, exactly as wave-13
   diagnosed. The 2/6 is NOT evidence of a guide improvement (the guide is unchanged) — it is the
   Force-resolution engine fix landing one real win + one favorable adjudication. Do not over-read
   the number in either direction; the play quality is roughly wave-13 level, with a Force-discipline
   loosening (Q1) and two new adherence misplays (Q3) as small negatives offset by the engine gain.

## LOSS ATTRIBUTION (separate engine/model from construction)

- **vs21 (adj, 8/17, T14) — aggro race + RESIDUAL-RACE.** Goblin Gang raced; the seq27 Force fizzle
  (no untapped sources) failed to save Treefolk, which then died to stacked Volcanic Hammers. Engine
  RESIDUAL-RACE materially worsened a construction-disadvantaged race.
- **vs110 (real, 8/20, T8) — fast Affinity aggro + the seq3 T1 creature-skip.** The false-mana
  decline of an offered Icehide Golem cost the earliest blocker vs an 8/1 Cranial-Plated Memnite +
  Signal Pest curve. Construction + a model misplay.
- **vs133 (real, -6/14, T12) — construction + the seq18 dig-over-Diamond-Faerie skip.** Phyrexian
  Obliterator 5/5 trample on an empty board; the skipped 3/3 flyer wouldn't have saved it, but the
  guide-mandated body was passed for a dig.
- **vs44 (real, 1/20, T14) — pure construction.** No answer to an evasive, snowballing 6/6 Faerie
  Bladecrafter (grew via +1/+1 counters on Faerie combat damage). Small ground bodies can't block a
  flyer. Identical to wave-13's vs44 loss.

## GUIDE DISPOSITION: FROZEN (byte-identical copy of the live guide; 13357 bytes, `cmp`-verified).

No new GUIDE-fixable decision class. Every residual routes off the guide: Force resolution + the new
no-untapped-sources RESIDUAL-RACE trigger → engine (ENGINE #1, now with the celebrated seq18
counter); stale `[attacking]` + carried inverted-freeze → engine (ENGINE #2, freeze half UNVALIDATED
this corpus); the two Rule #1 creature-skips → model-adherence against verbatim frozen lines
(deck110 s3 promotes the colored-pays-generic CORE candidate); Force reflexive-cast + damage-vs-
toughness → model watch; attackers name→index → harness (latent, 0 cost); losses → construction
(restate) + latency-adjudication (corpus-wide). Nothing demotable (pause-aware: phantom-lethal /
board-read anchors saw no counterexample = kept at full prominence). Guide diff = zero bytes.

## GAME → FILE MAP (run = matchups-20260716-040138)
| opp | outcome | turns | final (me v opp) | file |
|-----|---------|-------|------------------|------|
| vs140 | WIN (real) | 16 | 18 v 3→dead | `1784198955-ai_baka_deck135-0x561cd24d51b0-vs-ai_baka_deck140.jsonl` |
| vs17  | WIN (adj) | 20 | 13 v 4 | `1784194904-ai_baka_deck135-0x5653e82f11b0-vs-ai_baka_deck17.jsonl` |
| vs21  | LOSS (adj) | 14 | 8 v 17 | `1784192503-ai_baka_deck135-0x5651688c0ea0-vs-ai_baka_deck21.jsonl` |
| vs110 | LOSS (real) | 8 | 8 v 20 | `1784194902-ai_baka_deck135-0x55fa2db78210-vs-ai_baka_deck110.jsonl` |
| vs133 | LOSS (real) | 12 | -6 v 14 | `1784201233-ai_baka_deck135-0x55dc62368ed0-vs-ai_baka_deck133.jsonl` |
| vs44  | LOSS (real) | 14 | 1 v 20 | `1784204085-ai_baka_deck135-0x55c5751e0dc0-vs-ai_baka_deck44.jsonl` |
