# Deck-135 (Modern Snow) — Wave-16 game review

Corpus: PRIMARY `matchups-20260716-084532` (21-game round-robin over 44/110/133/135/17/21/62;
binary `/tmp/wagic-69793930a` = wave-15 engine batch: free alternative casts at zero mana
[Force's exile-pitch considered when tapped out], stale_echo option-subset-of-echo superstring
matcher, hybrid-pip affordability annotation). deck140 rotated OUT; deck62 (Enchantresses) entered.
Wave-11 guide live = current `deck135_strategy.txt` (FROZEN, byte-identical since wave-11). Core
prompt unchanged. All six deck135 seat games read decision-by-decision; opponent seats read at the
Force/combat seams (deck21 and deck110 opponent logs read for Force resolution). Doctrine: win
column is CONTEXT ONLY; unit of analysis is the DECISION with seq repros, layer-attributed; adopt-
a-revision bar is HIGH (veteran seat). deck135 owns this cycle: (a) the free-Force-at-zero-mana fix
exercise check; (b) the pre-flagged s33 stale_echo suspect; (c) the stale `[attacking]` board-tag
class; (d) the 1/6 record construction-vs-play split (pool's weakest deck).

## RECORD (context only — NOT a verdict) — 1W / 5L

| game    | opp                     | outcome (deck135)        | my/opp life | turn | class |
|---------|-------------------------|--------------------------|-------------|------|-------|
| 135v62  | 62 Enchantresses (new)  | **WIN** (19/5 dominant)  | 19 / 5      | 11+  | creatures stuck vs a SLOW deck — the ceiling working |
| 135v21  | 21 Goblin Gang          | LOSS (real, dead)        | -4 / 20     | 9    | fast aggro + GREEN-screw; **0 creatures cast all game** |
| 135v110 | 110 Etched Affinity     | LOSS (real, dead)        | -3 / 20     | 8    | fast Affinity aggro; 2 small bodies, outraced |
| 135v17  | 17 Faeries              | LOSS (real, dead)        | -9 / 18     | 11   | deployed 4 creatures, lost to evasion (flyers) |
| 135v133 | 133 Phyrexian/black     | LOSS (real, dead)        | -7 / 10     | 14   | 2x 1/1 Ice-Fang only; slow/small vs black |
| 135v44  | 44 Faerie Bladecrafter  | LOSS (real, dead)        | -4 / 17     | 16   | **deployed 6 creatures**, still lost to evasive 6/6 — pure construction, 3rd corpus |

Note both truncation caveats: the vs62 game has NO `gameend` record in either seat log (both truncate
mid-turn-11 with deck135 at 19 vs 5 — dominant), so it is scored the WIN that the brief's win table
(135: 1/6) assigns. **All 5 losses are REAL deaths (`won=False`), NOT life-adjudicated** — deck135
was genuinely killed to -3/-4/-4/-7/-9, three of them by turn 8/9/11. This is a deck getting run over,
not robbed by the latency tiebreak. The record moved 1/6 (w13) -> 0/6 (w14) -> 2/6 (w15) -> **1/6
(w16)**; the drop is variance around an unchanged construction floor (see the split, section d).

---

## (a) FREE-FORCE-AT-ZERO-MANA FIX — EXERCISE CHECK (owned). Verdict: the tapped-out free-cast path got NO clean exercise; a NEW distinct fizzle mechanism surfaced.

Four Force casts were CHOSEN this corpus (down from wave-15's 6; the reflexive-cast swing persists).
Grep of all six seat logs shows **ZERO `Force of Negation: hand -> stack` events at the deck135
seat**; the only Force events are `library -> hand` (drawing it). Resolution was recovered from the
OPPONENT seat logs.

### The ONE Force that FIRED — deck21 s19 (free alt-cast, COUNTERED Volcanic Hammer) — but with mana UP, so it does NOT exercise the tapped-out fix; and the DECISION was a discipline error.
- **s19** (T9, opp's turn, Volcanic Hammer on the stack): offer = `Cast Force of Negation with its
  exile a blue card from hand cost - can target on the stack: Volcanic Hammer`. choice=2, exiling
  Ice-Fang Coatl (a legal blue card).
- **Resolution (deck21 opponent log s17->s20):** `Opponent's Force of Negation: hand -> stack` then
  `Your Volcanic Hammer: stack -> exile | Opponent's Force of Negation: stack -> graveyard`. The
  Force FIRED and COUNTERED (stack->exile). Engine resolution works — consistent with wave-15's
  deck140 seq18.
- **BUT it does NOT validate the zero-mana/tapped-out path.** The s19 prompt shows `Mana available:
  {u}{u}{c} from 3 untapped sources` — deck135 was NOT tapped out. This is the free alt-cast firing
  with mana available, the same regime wave-15 already validated (deck140 seq18 had `{w}` up). The
  69793930a fix specifically admits the exile-pitch **when tapped out**; that path saw no clean firing.
- **The DECISION was WRONG (Force discipline error).** Volcanic Hammer deals 3 to deck135's FACE (at
  9 life -> 6). The guide states verbatim: *"NEVER Force a burn spell pointed at your own face - take
  the damage and keep the blue card to cast your creatures."* The model Forced face burn, exiled
  Ice-Fang Coatl (a creature it needed), and **deck135 died the same turn anyway** (s20 gameend,
  -4 — lethal came from the board, not the countered Hammer). A textbook 2-for-1 against face burn
  with zero benefit. Same reflexive-cast class as wave-15's Cranial Plating over-Force, now on face burn.

### THREE Force casts that FIZZLED — deck110 s8/s16/s25 (hardcast offered while unpayable -> silent defer). NEW mechanism, distinct from wave-15 ENGINE #1b.
All three offers were the **HARDCAST** mode: `Cast Force of Negation {1}{u}{u} - can target on the
stack: <Cranial Plating | Galvanic Blast | Galvanic Blast>`. Each was chosen (choice=1), followed by
a `kind=defer, fallback=deferred_to_heuristic, latency=-1` record — and **NONE reached the stack**
(no `hand -> stack` in either seat log; the target spells resolved).

| seq | target | mana available | blue card in hand? | result |
|-----|--------|----------------|--------------------|--------|
| deck110 s8  | Cranial Plating (equip)    | `{g}` (1 source)      | NO | defer -> fizzle, Plating resolved |
| deck110 s16 | Galvanic Blast (face burn) | `{g}` (1 source)      | NO | defer -> fizzle, Blast resolved |
| deck110 s25 | Galvanic Blast (face burn) | `{g}{u}{c}` (only 1 U) | NO | defer -> fizzle, Blast resolved |

**Mechanism (tagged): the HARDCAST Force `{1}{u}{u}` was OFFERED while genuinely unpayable.** deck135
had `{g}` (s8/s16) or `{g}{u}{c}` (s25 — still short a second U), never `{1}{u}{u}`, and had **no
blue card in hand** to pitch (hand across these windows: Gelid Shackles {w}, Into the North {1}{g}x,
Misty Rainforest, Force itself), so the free alt-cast was correctly NOT offered. The engine offered
only the hardcast, the model selected it, the `defer` step could not complete the unpayable cast, and
it silently fizzled. This is a DISTINCT offer-legality gap from wave-15's #1b (which was a *free
alt-cast* fizzling when tapped out): here it is the *hardcast* mode offered without any mana-payability
check. FIX DIRECTION: gate the hardcast Force offer on actual `{1}{u}{u}` availability; if the caster
can pay neither the mana nor the exile alt-cost (no blue card), do not offer Force at all. -> notes.md
ENGINE #1c.

**MODEL error rides on top.** In all three deck110 replies the model reasoned it was casting the
*free alt-cast* — *"I can pay by exiling a blue card from my hand (Into the North)"* — while selecting
the offered *hardcast* option. **Into the North is a GREEN sorcery ({1}{g}), not a blue card**; there
was no legal exile target. The model both (i) mismatched the offered hardcast option with free-alt-cast
reasoning, and (ii) hallucinated a card's color. Cost was zero only because the cast fizzled anyway;
the decisions were also all bad targets (Cranial Plating not sanctioned; Galvanic Blast = face burn,
guide says never). -> general-suggestions (Force reflexive-cast drift) + model watch (card-color error).

### Exercise-check bottom line
- Free alt-cast RESOLUTION: re-confirmed working (deck21 s19 countered a burn spell), with mana up.
- Zero-mana / tapped-out free-cast path (the actual 69793930a fix): **UNVALIDATED** — no clean firing
  instance existed. The only free alt-cast had 3 untapped sources; the tapped-out windows offered the
  unpayable hardcast, not the free alt-cast (no blue card to pitch). NOT-EXERCISED != NOT-FIXED — carry
  the wave-15 VALIDATION TODO forward.
- No fizzle this corpus matched the wave-15 #1b signature (free alt-cast + "(no untapped sources)"). The
  three fizzles are the NEW hardcast-unpayable mechanism (#1c). Both #1b and #1c want the same guard:
  offer legality must check payability (mana OR a legal exile card) before listing a Force cast.

## (b) s33 stale_echo SUSPECT (owned): CORRECT catch, NOT a wrongful downgrade. Brief mislabeled the game (vs133, not vs21).

The pre-flagged echo `search basic land with Prismatic Vista targeting Snow-Covered Forest` occurs at
**deck135-vs-deck133 s33** (the brief says "vs21 game" — that is a mislabel; the string is in the
vs133 seat log, `1784211695-...-vs-ai_baka_deck133.jsonl`).

- **Menu actually shown (1 legal action):** `1. search basic land with Prismatic Vista targeting
  Snow-Covered PLAINS [cost: Life, Tap, Sacrifice]`. The engine pre-targeted **Snow-Covered Plains**.
- **Model reply:** `CHOICE: 1 (search basic land with Prismatic Vista targeting Snow-Covered FOREST)`
  — it echoed **Forest**, substituting a different (unoffered) target for the offered Plains.
- **Verdict: CORRECT stale_echo, not a wrongful downgrade.** The echo is NOT a superstring of the
  offered option — it swaps `Plains -> Forest` (a different land type), so the wave-15 superstring
  false-positive fix correctly did NOT match it. This is genuine menu confusion (the model wanted a
  Forest for its green need but only a Plains-targeting option was offered), exactly the class
  stale_echo is meant to catch. Cost ~= 0: only one option existed, so the heuristic took the same
  action the index pointed at. The fix is working as intended.

**Second stale_echo this seat — deck44 s18 — same class, also CORRECT.** Menu offered `1. ...Flooded
Strand targeting Snow-Covered Island` and `2. ...targeting Snow-Covered Plains` (Flooded Strand fetches
only Plains/Island). Model echoed `CHOICE: 1 (...targeting Snow-Covered FOREST)`. Forest was never
offered (and Flooded Strand can't fetch it); the echo is not a superstring; correct catch. Underlying
MODEL error: it believes Flooded Strand can fetch a Forest — a fetch-color knowledge gap, but the
guard protected against executing the mis-map. **Both stale_echos at this seat are correct catches;
zero wrongful downgrades. The superstring fix caused no false negatives here.**

## (c) STALE `[attacking]` BOARD-TAG (owned): REPRODUCED, benign this corpus.

The wave-15 ENGINE #2 `[attacking]`-half defect reproduced. Confirmed genuinely-stale instances
(current phase = Main phase 2, YOUR turn — no combat active — with own TAPPED creatures still tagged):

- deck133 s36 — Ice-Fang Coatl `[tapped - untaps and can attack next turn] [attacking]`
- deck62 s29 — Icehide Golem x2 tagged `[attacking]`
- deck44 s34 / s35 — Icehide Golem tagged `[attacking]`

A creature is "attacking" only during the combat it is declared in; the tag lingering into Main
phase 2 (post-combat) is stale — identical to wave-13/15's defect. (Instances at deck133 s35 [Blockers,
your turn], deck44 s28 [Combat damage], deck44 s37 [Blockers, opponent's turn] were checked and are
LEGIT in-combat tags, not stale.) **It drove NO wrong decision this corpus** — all stale instances were
Main-phase-2 windows where the tagged creature is tapped and inert, so the tag is cosmetic; deck62 s29
was an unparsed_reply anyway. Representation-noise, 0 cost — but the identical defect stands on the
ledger. FIX (unchanged): clear `[attacking]` on any creature not in the current declared-attackers set.
Freeze-half of ENGINE #2 (inverted Treefolk-freeze) had no opportunity again this corpus (see notes).

## (d) 1/6 RECORD — CONSTRUCTION vs PLAY SPLIT (owned). Verdict: construction-dominant + variance; NOT a play regression.

**Creature deployment by game** (from cast-choice grep):
- vs62 (WIN): Icehide Golem, Abominable Treefolk, Icehide Golem — creatures stuck, raced a slow deck.
- vs44 (LOSS): SIX creatures (Golem, Ice-Fang, Ohran Viper, Golem, Diamond Faerie, Boreal Druid) — a
  full board, still lost T16 to an evasive snowballing 6/6 Faerie Bladecrafter. **Pure construction**
  (no evasion answer), a near-identical repeat of wave-13 AND wave-15's vs44.
- vs17 (LOSS): 4 creatures (Golem, Ohran Viper x2, Boreal Druid), lost T11 to Faeries — small ground
  bodies can't handle flyers. Construction.
- vs133 (LOSS): only 2x Ice-Fang Coatl (1/1 flyers), slow/small vs a black deck. Construction.
- vs21 (LOSS): **0 creatures cast the entire game**, dead T9. Its {g}-requiring creatures (Ice-Fang
  Coatl x2 in hand) were stranded — the s19 mana line shows `{u}{u}{c}` with NO GREEN. Classic
  color-screw off the thin 2-Island / fetch base; Into the North (the green fixer) itself needs {g},
  a chicken-and-egg trap, and its fetches (Flooded Strand) couldn't produce green. **Construction-
  dominant** with minor tempo slop (s3 "Play no land right now"; s11/s15 "Cast nothing" — but nothing
  castable, mostly digs available).
- vs110 (LOSS): 2 small bodies (Boreal Druid T3, Icehide Golem T5), outraced by Affinity by T8.
  Construction (slow clock vs fast aggro).

**PLAY errors this corpus:** the only material misplays were Force-discipline (deck21 s19 face-burn
Force that lost Ice-Fang; deck110 x3 bad-target Forces that fizzled harmlessly). **No Rule #1
creature-skip this corpus** — an IMPROVEMENT over wave-15 (which had two: deck110 s3 and deck133 s18).
The two grep hits for "creature offered, non-creature chosen" (deck133 s18, deck17 s22) are search-
target-resolution windows (no legal-actions block; the model is picking which land a fetch/search
retrieves), NOT creature-skips.

**Split verdict:** the 1/6 is CONSTRUCTION + variance. All 5 losses trace to the unchanged
construction ceiling — slow clock (Abominable Treefolk is the only real closer, lands T6-T16), gold /
double-pip creatures off a thin blue/green splash that color-screws (vs21), and no answer to evasion
(vs44/vs17). The one WIN (vs62) is that ceiling working: creatures stuck, the opponent was slow, life
moved. The wave-15 -> wave-16 dip (2/6 -> 1/6) is variance: wave-15's 2/6 leaned on a favorable
adjudication (vs17) plus the now-ROTATED vs140 Force-wipe-protection win; this corpus deck135 beat the
one slow deck it faced (62) and was killed outright by everything faster or evasive. Play quality on the
deployment axis is if anything slightly better (no creature-skips); the Force discipline is the one axis
that WORSENED.

## GUIDE DISPOSITION: FROZEN (byte-identical copy of the live guide; 13357 bytes, `cmp`-verified).

No new GUIDE-fixable decision class. Every residual routes off the guide: the Force fizzle mechanisms
(#1b tapped-out free-cast unvalidated, #1c NEW hardcast-unpayable-offer) -> engine; stale `[attacking]`
+ carried inverted-freeze -> engine (ENGINE #2); Force reflexive-cast on face burn / non-sanctioned
targets -> MODEL-ADHERENCE against verbatim frozen lines (the guide already says "NEVER Force face
burn" and lists the sanctioned targets — a 3rd restatement is the sanctioned-against escalation) ->
model watch + general-suggestions; card-color / fetch-color hallucinations (Into the North "blue";
Flooded Strand fetches Forest) -> model watch; losses -> construction (restate; user owns decklist) +
variance. Nothing demotable (pause-aware: no counterexample to any anchor). Guide diff = zero bytes.

## GAME -> FILE MAP (run = matchups-20260716-084532)
| opp | outcome | turns | final (me v opp) | deck135 seat file |
|-----|---------|-------|------------------|-------------------|
| vs62  | WIN (dominant, no gameend rec) | 11+ | 19 v 5 | `1784214769-ai_baka_deck135-0x556cbdbe1180-vs-ai_baka_deck62.jsonl` |
| vs21  | LOSS (real) | 9  | -4 v 20 | `1784213621-ai_baka_deck135-0x55f0247c9230-vs-ai_baka_deck21.jsonl` |
| vs110 | LOSS (real) | 8  | -3 v 20 | `1784220684-ai_baka_deck135-0x5629f221c170-vs-ai_baka_deck110.jsonl` |
| vs17  | LOSS (real) | 11 | -9 v 18 | `1784220249-ai_baka_deck135-0x55f8367b13c0-vs-ai_baka_deck17.jsonl` |
| vs133 | LOSS (real) | 14 | -7 v 10 | `1784211695-ai_baka_deck135-0x55fd9db1aee0-vs-ai_baka_deck133.jsonl` |
| vs44  | LOSS (real) | 16 | -4 v 17 | `1784215284-ai_baka_deck135-0x560b40545ce0-vs-ai_baka_deck44.jsonl` |
