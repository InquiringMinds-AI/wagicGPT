# Deck-135 (Modern Snow) — Wave-13b game review

Corpus: PRIMARY `matchups-20260715-183228` (21-game round-robin over 44/109/110/131/133/135/140;
binary `/tmp/wagic-a75e2625e` = Force round-2 cost-target fallback + **Force round-3 offer-legality /
dead-end removal** + auto-tap two-pass + name-echo protocol + BLOCKS:none + 240s timeout). Wave-11
guides live = current `deck135_strategy.txt` (FROZEN, all 7 byte-identical since wave-11). Core prompt
unchanged (8th PASS standing). This is the FIRST clean validation corpus after the wave-12/13a Spark-
crash contamination. All six deck135 seat games read decision-by-decision; opponent seats read at
Force/combat seams. Doctrine: win column is CONTEXT ONLY; unit of analysis is the DECISION with seq
repros, layer-attributed; adopt-a-revision bar is HIGH. deck135 owns the Force-of-Negation round-3
verification (brief item #1). Game->file map at bottom.

## RECORD (context only — NOT a verdict) — 1W / 5L

| game    | opp (name)              | outcome (deck135)                | my/opp life | turn | class |
|---------|-------------------------|----------------------------------|-------------|------|-------|
| 135v133 | 133 Phyrexian Asphodel  | **WIN** (real kill, 133 to -8)   | 12 / -8     | 14   | clean win |
| 135v110 | 110 Etched Affinity     | LOSS (adj1 — lost by ONE life)   | 9 / 10      | 12   | **ENGINE/LATENCY-robbed (deck135 was WINNING on board)** |
| 135v131 | 131 Mind Control        | LOSS (adj1)                      | 8 / 25      | 16   | construction (small bodies can't race lifegain) |
| 135v140 | 140 Wipe Them Out!      | LOSS (adj1)                      | 15 / 29     | 18   | construction (Damnation blowout, no rebuild) |
| 135v44  | 44 Faerie Archmage      | LOSS (adj0-for-44)               | ~3-8 / 20   | 14   | construction (no answer to evasive 6/6) |
| 135v109 | 109 Hellrider           | LOSS (real death, fast aggro)    | 0 / 20      | 9    | construction/curve (mull-6, no early defense) |

deck135 is the historically weakest seat (win table 135:1/6). **Play quality is SOUND this corpus** —
Force discipline perfect, creatures on curve, combat healthy, no develop-paralysis. The seat's losses
separate cleanly: **1 is engine/latency-inflicted (vs110, deck135 was the heavy favourite and lost only
the life-total tiebreak when the 2400s cap cut the game a turn early), 4 are genuine CONSTRUCTION losses**
where deck135 was behind on BOTH board and life (gold/double-pip creatures off a 2-Island splash → color
screw + slow clock; small bodies can't race lifegain/aggro). Guide FROZEN; every residual routes off it.

---

## Q1 — FORCE OF NEGATION round-3 (brief item #1 — I OWN this): VERIFIED CLEAN. Dead-end gone, 1 legal offer, correctly declined, 0 casts, 0 RESIDUAL-RACE.

The round-3 shipment (a75e2625e — alt-cast offers gated on a payable exile target; standalone
"exile a blue card" dead-end action removed from the GPT menu) is **VALIDATED at this seat**:

- **Force offered as a CAST exactly ONCE this corpus: deck44 s22** (T8, opponent's turn, MP1). The
  option renders as the gated alt-cast, correctly formed:
  `Cast Force of Negation with its exile a blue card from hand cost  - can target on the stack: Go for the Throat`.
  deck135's hand held blue cards (Search for Azcanta {1}{u}, Ice-Fang Coatl {g}{u}, the Force itself,
  Snow-Covered Island) → a payable exile target existed → the offer is legal. **0 standalone
  "exile a blue card" dead-end actions anywhere in the corpus. 0 dead-end picks.** The round-3 fix fired.

- **The single legal offer was DECLINED, and the decline is CORRECT** (evaluated as a DECISION per brief):
  the opponent cast Go for the Throat targeting **its OWN Sleep-Cursed Faerie**. Forcing it would cost
  deck135 TWO cards (Force + an exiled blue card) to stop the opponent from killing *its own* creature —
  strictly card-negative for zero board benefit. The model reasoned it out correctly (verbatim s22):
  it identified the target was the opponent's own creature, computed the 2-for-1 ("I am down two cards…
  the opponent is down a creature either way"), correctly noted it is NOT a discard spell (so the
  discard-exclusion clause is not the operative reason), and concluded "let Go for the Throat resolve…
  preserve my card advantage." This is a clean, correctly-reasoned decline — not a reflexive pass. The
  guide's Force spine (spend only on a board wipe / mass-bounce / removal on your ONLY threat) held.

- **Force NEVER reached the stack this corpus.** Grepping all seat `events` for "Force of Negation"
  returns only DRAWS, an Inquisition/Thoughtseize discard, and Azcanta REVEALS — no cast reaching the
  stack. Therefore **0 Force casts → 0 RESIDUAL-RACE instances at this seat.** The async response-window
  race (which eats answered casts on the opponent's turn) could not manifest: there was no answered cast
  to eat. No "casting Force of Negation (model's pick)" with no execution.

- **deck140 s35/s36 "Force" occurrences are NOT counter windows** — they are Azcanta, the Sunken Ruin
  card-reveal selections (Force appears as a card being revealed to hand from `look at the top four`),
  correctly excluded from the Force-cast tally.

Force resolution ENGINE bug (ledger #1, still unfixed by design) produced **no new resolution evidence**
this corpus (0 casts). Carried unchanged (notes.md ENGINE #1).

## Q2 — BLOCKS (brief item #3): 0 "BLOCKS: none" at this seat; the 3 real blocks parsed and executed correctly.

- **0 `BLOCKS: none` firings** at the deck135 seat (grep of all replies = 0). deck135 contributes none of
  the corpus-wide 6 all-decline firings, so there is no misparse-of-intended-block risk to check here.
- **3 real blocks, all `BLOCKS: Bn:An` form, all executed** (verified in `events`/next-combat): deck131
  s11 (Ice-Fang Coatl blocks Young Pyromancer, `B1:A1`), deck133 s16 (Icehide Golem blocks Bloodghast,
  `B1:A1`), deck109 s7 (Ice-Fang Coatl blocks Legion Loyalist, `B1:A2`). The indexed form parses cleanly.
- **1 blockers fallback (deck109 s19)**: `unparsed_reply`, choice=0 heuristic — NOT a `BLOCKS: none`.
  This was a reasoning-tax TIMEOUT (see Q4), in an already-lethal position. Distinct class, not a decline.

## Q3 — NAME-ECHO (brief item #4): 100% compliant, NO wrong remap, the one genuine out-of-range correctly REJECTED.

- Every CHOICE line carries `(name)`; no echo-vs-index conflict resolved wrong at this seat.
- **One genuine out-of-range: deck44 s36** — the reply emitted `CHOICE: 3 (Glacial Revelation)` into a
  **2-option list where Glacial Revelation was NOT offered** (an unlisted/unaffordable card the plan
  fixated on). The reconcile arm **correctly REJECTED it to heuristic** (choice=-1, `fallback=unparsed_reply`)
  rather than mis-remapping the absent name onto a wrong listed option. This is the desired behaviour: the
  name-echo remap did NOT paper over an out-of-range index. (All other apparent "out-of-range idx=0" hits
  in the automated scan are `CHOICE: 0 (pass)` priority passes — benign.)

## Q4 — NEW ENGINE FINDING: stale `[attacking]` tag + inverted/absent freeze-state annotation on a Treefolk-frozen creature drove a reasoning-tax EXPLOSION → decision TIMEOUT (deck109 s17/s19).

The strongest new item this corpus. In vs109, deck135 cast Abominable Treefolk at s16 and chose **Boros
Reckoner as its ETB tap-and-freeze target at s17** (Reckoner was the opponent's only untapped creature).
Treefolk's freeze taps the creature and it does NOT untap on the controller's next untap. Yet at **s19
(the fatal Blockers window)** the battlefield line renders Reckoner as:

  `Boros Reckoner (3/3) [tapped - untaps and can attack next turn] [attacking]`

Two defects stack into a self-contradiction:
1. **Stale `[attacking]` tag** — Reckoner is NOT a declared attacker this combat: the numbered Attackers
   list is A1 Stromkirk / A2 Cackler / A3 Ash only, and the harness's own `up to 8` damage line correctly
   EXCLUDES Reckoner's 3 power. The `[attacking]` tag is left over from a prior turn's state.
2. **Freeze not represented; actively INVERTED** — a frozen creature is annotated `untaps and can attack
   next turn`, the opposite of its true state.

The board surface thus asserts four `[attacking]` creatures against a 3-attacker list and an `up to 8`
that only accounts for three — and the model spent its **entire reply** oscillating to reconcile the
contradiction (re-deriving the damage math ~6 times, hunting for whether Reckoner is/ isn't attacking),
hit the token/latency wall, and produced `unparsed_reply` → heuristic fallback (choice=0). **This is the
same SHAPE as the wave-10 Force "NO legal target right now" contradiction that was fixed — a self-
contradictory representation multiplying a weak model's derivation length — at a NEW locus (the BOARD-
STATE serialization, not an option annotation) and with a NEW, worse consequence: under the latency cap
the tax doesn't merely inflate reasoning, it TIMES OUT the decision and hands it to the heuristic.**
Outcome-neutral here (the position was lethal regardless — 8 power incoming vs 4 life, one blocker), but
in a NON-lethal position this exact contradiction would push a critical block to a fallback. HIGH-value
ledger item (notes.md ENGINE #2). Fix direction: (a) clear `[attacking]` on any creature not in the
current declared-attackers set; (b) represent Treefolk-frozen state (`frozen - will not untap`) and
suppress the `untaps and can attack next turn` annotation for frozen permanents.

## Q5 — NEW HARNESS/ENGINE (latent, brief's flagged candidate): attackers name→index parse gap. Harmless this corpus.

The model repeatedly emits MIXED `ATTACK: A1, <Name>` declarations. The parser takes the leading A-index
and **DROPS the trailing NAME token**:
- vs110 s16 `ATTACK: A1, Ice-Fang Coatl` (Coatl not offered — just cast / summoning-sick) → attacked A1 only
- vs110 s25 `ATTACK: A1, Abominable Treefolk` (Treefolk just cast) → A1 only
- vs133 s25 `ATTACK: A1, Diamond Faerie` (Faerie just cast) → A1 only
- vs44 s19 `ATTACK: A1, Boreal Druid` (Druid a mana creature, not offered) → A1 only

**Every dropped name this corpus was an ILLEGAL attacker** (a just-cast summoning-sick creature or a
mana-only Druid) that was NOT among the offered A-options, so the drop cost ZERO damage — the parser
clamped correctly to the legal attacker. **But the class is latent-dangerous**: if the model ever names a
creature that IS a legal A2+ attacker, it would be silently dropped. This is exactly the brief's flagged
ledger candidate (an attackers name→index reconcile arm mirroring parseChoice's name-echo). Note the
instances; do NOT inflate — currently 0 outcome cost. Routes to notes.md HARNESS #1.

Companion MODEL tic: the same four instances show **summoning-sickness blindness** — the model believes a
just-cast creature (or a mana Druid) can attack the turn it entered. The engine's "creatures that can
attack" list correctly excludes them and the model mostly trusts that list elsewhere, so this is a minor
representation tic, harmless (clamped every time). Model-side; no guide line (the guide already says
"count ONLY the creatures offered in the Attackers list"). notes.md MODEL.

## Q6 — colored-pays-generic false belief (brief item #5 / parked CORE candidate): FIRED then SELF-CORRECTED at vs110 s5. No wrong action; candidate stays parked.

Wave-11 reported the belief did NOT recur at this seat. **This corpus it DID surface in first-draft
reasoning** at vs110 s5 (T4 cast window, options [Cast Icehide Golem {1}, Cast nothing]): the model's
opening line was *"I cannot cast Icehide Golem because I have no generic mana available; my only untapped
source is a Snow-Covered Plains which produces {W}."* — the false belief firing. **But it SELF-CORRECTED
within the same reply** (*"Wait… {1} is a generic cost. {W} can pay {1}. My previous thought process was
flawed"*) and cast Icehide Golem (choice=1). **No wrong action; the cheapest creature deployed on curve.**
Per doctrine this is NOT a hardening (the belief fired but did not cause a decline). It is, however, a
slight ESCALATION from wave-11's clean pass (first-draft error present, corrected) — the belief is
model-UNSTABLE, not un-taught. **CORE stays PASS / candidate stays PARKED** (the core body already states
"any color can pay a generic cost"; a 3rd copy is the sanctioned-against escalation). Cross-seat grep for
a 2nd deck's ACTUAL offered-payable decline remains the promotion trigger (general-suggestions.md).

Auto-tap stranding (brief item #5, deck44's issue): **not present at this seat.** 0 offered-payable casts
that the engine rejected (no `cannot pay`/`insufficient mana` events). deck135's mana failures are
COLOR-SCREW from construction (gold/double-pip creatures off 2 Islands + fetches), a distinct problem
from deck44's generic-vs-blue auto-tap stranding.

## Q7 — Azcanta-as-CREATURE hallucination (wave-11 Q5 watch): did NOT recur. Watch does not harden.

The wave-11 single-window residual (deck44 s34: pilot believed Search for Azcanta flips into a 4/4 flying
CREATURE blocker) **did NOT reproduce** this corpus (targeted grep = 0). In vs140 the model transformed
Search for Azcanta at s33 and then used Azcanta, the Sunken Ruin CORRECTLY as a card-selection land
ability (`look at the top four`, s34-s36). The wave-11 watch (a KEY CARDS micro-note) does NOT clear the
bar and is NOT adopted.

## Q8 — Fallbacks / protocol / combat spot-checks.

- **Fallbacks: 8 `unparsed_reply` this seat** (deck131 s12/s32, deck140 s15/s24, deck44 s32/s36, deck133
  s9, deck109 s19) — the corpus-wide long-reasoning-ramble class, correctly discarded by the guard
  (choice=-1 heuristic; choice=0 on the s19 blocker). **empty_reply = 0** (the wave-12/13a Spark-death
  flood is CLOSED). **All 8 self-healed and NONE changed a game** — the two in even-life early positions
  (deck140 s15, deck131 s12) were LAND-PLAY windows where the heuristic picked a land and the model's
  NEXT window cast on curve (deck140 s16 Treefolk, deck131 s13 Into the North). The rest are in lost or
  the-won positions.
- **Combat healthy** where the draw supplied bodies: deck133 Treefolk+Diamond Faerie closed for 10
  (the win); deck131/deck110 pushed real Treefolk pressure; Boreal-Druid carve-out held (attacked only
  when profitable/last body). No reflexive high-life chump.
- **Develop on curve** everywhere the draw allowed (Golem T4 vs110, Treefolk T8 vs109/vs140/vs133). No
  dead-board develop-paralysis in any game (the wave-10 class stays dead).
- **Reasoning-tax windows** (latency contributors to the adjudication regime): vs110 s6 (stale YOUR PLAN
  echo still says "cast Golem" AFTER it was cast at s5 → the model burns a large reply confused about why
  the cast isn't offered, suspecting the engine "thinks {W} can't pay {1}", before correctly fetching);
  vs110 s8 (very long mana-availability derivation, correct); vs109 s19 (the contradiction timeout, Q4).
  Seat latency: med 12.2s / p90 104s / max 210s — consistent with the corpus-wide GPU_MEM_UTIL=0.75
  preemption. These pushed the board-winning vs110 game into life-adjudication.

## LOSS ATTRIBUTION (separate engine-inflicted from play-quality)

- **vs110 (adj1, 9/10, T12) — ENGINE/LATENCY-INFLICTED. deck135 was WINNING.** At the 2400s cap deck135
  was one life behind but held an 8/8 trample Abominable Treefolk + 1/1 deathtouch-flying Ice-Fang Coatl,
  both attacking, opp at 10 with only a TAPPED 5/2 Ornithopter and no other blocker. Next turn deck135
  swings for 9 into a lone tapped blocker = the heavy favourite. The life-only adjudication mis-scored a
  won board as a loss because the clock cut the game a turn early. This directly answers the brief's "was
  the seat ahead for the right reasons": deck135 was BEHIND on life but decisively AHEAD on board. Golem
  cast T4 on curve; no play miss. The canonical "won-board-lost-adjudication" fixture.
- **vs131 (adj1, 8/25, T16) — construction.** Small bodies (1/1 Coatl, 1/3 Viper) can't race opp at 25
  (Prism Ring lifegain) with 5 creatures; the closer never stuck. Behind on board AND life.
- **vs140 (adj1, 15/29, T18) — construction.** Opp Damnation (s18, T10) wiped Icehide Golem; deck135
  couldn't rebuild vs 29 life + Staff of Nin. **NO missed Force** — Force was in LIBRARY (revealed by
  Azcanta at s35), not hand, when Damnation resolved, and no counter-window was offered. Genuine loss.
- **vs44 (adj0-for-44, ~3-8/20, T14) — construction.** No answer to opp's evasive 6/6 Faerie Bladecrafter;
  deck135's small ground bodies can't block a flyer. Contains the correct Force decline (Q1).
- **vs109 (real death, 0/20, T9) — construction/curve + the Q4 timeout.** Mulligan to 6 with no early
  defense; Boros/Hellrider aggro raced deck135; Treefolk landed T8 (too late, at 4 life); T9 lethal alpha
  strike. The s19 fatal block timed out on the Q4 contradiction, but the position was already lethal.
- **vs133 (WIN, real kill 12/-8, T14).** Treefolk + Diamond Faerie for lethal 10. Clean.

Net: play quality SOUND; 1/5 losses engine/latency-robbed, 4/5 genuine construction losses. The seat's
ceiling is CONSTRUCTION (color screw + slow clock + small bodies can't race lifegain/aggro), compounded
by the latency-adjudication regime that robbed the one board-winning game.

## GUIDE DISPOSITION: FROZEN (byte-identical copy of the live guide; 13357 bytes).

No new guide-fixable decision class. Every residual routes off the guide: Force resolution → engine
(carried, 0 data); stale-[attacking]/freeze annotation → engine ledger (NEW); attackers name→index →
harness ledger (NEW, latent); summoning-sickness over-declaration → model watch (harmless); colored-pays-
generic → core (fired-then-corrected, parked); Azcanta-as-creature → watch (did not recur); losses →
construction (restate) + latency-adjudication (corpus-wide engine/harness). Nothing demotable
(pause-aware: phantom-lethal/board-read anchors untested this corpus = kept at full prominence).

## GAME -> FILE MAP (run = matchups-20260715-183228)
| seat epoch | opp | outcome | turns | final (me v opp) | results row |
|------------|-----|---------|-------|------------------|-------------|
| 1784160545 | vs110 | LOSS (adj1) | 12 | 9 v 10  | `135 110 adj1 9 10 12` |
| 1784162578 | vs131 | LOSS (adj1) | 16 | 8 v 25  | `135 131 adj1 8 25 16` |
| 1784162946 | vs140 | LOSS (adj1) | 18 | 15 v 29 | `135 140 adj1 15 29 18` |
| 1784165346 | vs44  | LOSS (adj0-for-44) | 14 | ~3-8 v 20 | `44 135 adj0 20 8 14` |
| 1784167746 | vs133 | WIN (real) | 14 | 12 v -8 | `135 133 0 12 -8 14` |
| 1784170894 | vs109 | LOSS (real) | 9  | 0 v 20  | `135 109 1 0 20 9` |
