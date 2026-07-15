# Wave-10 review — deck109 (Hellrider, mono-red aggro / RDW)

**Corpus:** PRIMARY `matchups-20260715-031825`, round-robin, deck109 seat = 6 games. Binary
`/tmp/wagic-c8c054be8`, wave-9 guides (deck109 guide byte-identical to wave-8/wave-9), the FIRST
corpus under the **answer-after-plan reply protocol** (scratch reasoning -> PLAN: -> final
`CHOICE:`/`ATTACK:`/`BLOCKS:` line), max_tokens 2048. Guide in play = the LIVE `deck109_strategy.txt`.

**Record: 5/6** — beat 133, 110, 131, 135, 140; LOST to 44. Per the USER DOCTRINE this is context only.
The unit of analysis below is the DECISION.

| seat file (epoch) | opp | winner | end (my / opp, turn) |
|---|---|---|---|
| 1784105766 | 44  | opp44 | -14 / 2, T14 (**loss** — untouchable flyers+shroud, raced back) |
| 1784105908 | 133 | 109 | 18 / -2, T8 (raced devotion out before Obliterator/GM online) |
| 1784109135 | 110 | 109 | 7 / -5, T9 |
| 1784110077 | 131 | 109 | 20 / -6, T11 |
| 1784110926 | 135 | 109 | 18 / -6, T13 |
| 1784112074 | 140 | 109 | 20 / 0, T13 |

---

## HEADLINE (NEW decision-level failure class) — HYBRID-MANA "offered=payable" false belief, deck131 seq18/19/21

**The class:** the pilot misreads a HYBRID mana symbol `{b/r}` as requiring *two* mana (black AND red),
forms the false belief "I can't pay it (I have no black)," and declines / fails to cast an **OFFERED**
(therefore payable) creature. `{B/R}` is hybrid — ONE red mana pays it. The engine correctly offered
`Cast Rakdos Cackler {b/r}` (payable with the pilot's `{R}`); the pilot fabricated the can't-pay belief.

This is deck109's shape of the wave-9 **parked "offered = payable" core candidate** (deck135 vs140 s24:
declined an offered `Cast Astrolabe {1}` believing `{G}` cannot pay a generic `{1}`). Same class —
*a specific false mana-payment rule overriding the offered=payable contract* — now at a **2nd seat**.
Per the wave-9 synthesis, a 2nd witnessing seat is the promotion trigger.

Repro span (game vs131, deck109 WON T11 — non-outcome-changing, but three real errors):

- **seq18, T9, opp 10, mana `{r}{r}`** — correctly cast Ash Zealot `{r}{r}`. The carried PLAN already
  contains the seed: *"We cannot cast the Rakdos Cackler in hand because we lack black mana (only have
  Red)."* False.
- **seq19, T9, opp 10** — options were only `[Cast Rakdos Cackler {b/r}, Cast nothing]`. Pilot chose
  **Cast nothing**, PLAN: *"We cannot cast the Rakdos Cackler in hand because we lack black mana (only
  have Red)."* This is the **clean repro**: an OFFERED (payable) creature declined on a can't-pay belief,
  violating the #1 DEPLOY rule (a free body for next turn thrown away). Mirror-identical to deck135's
  Astrolabe decline.
- **seq21, T9, opp 3, mana `{r}`** — options `[Cast Rakdos Cackler {b/r}, Cast nothing]`. Here the false
  belief COLLIDES with the core `offered=payable` contract, which the pilot *quotes*: *"Never decline a
  listed action because you believe ... you cannot pay for it; if you truly could not, it would not be
  listed."* Unable to reconcile "listed = payable" against its certainty that `{b/r}` needs black, the
  pilot **spirals the entire 2048-token budget** on the contradiction ("Maybe the option is a trick and
  the game is broken? ... Perhaps the 'Mana available' line is misleading") and never emits a `CHOICE:`
  line -> `unparsed_reply` -> heuristic fallback `-1`. (This is a **spiral/loop** truncation, NOT a
  length-cut truncation like the other two fallbacks below.)

**Why it is real and not variance:** the pilot casts hybrids fine most of the time — Rakdos Cackler cast
successfully at deck44 s25, deck110 s11, deck131 s3; Boros Reckoner `{r/w}{r/w}{r/w}` cast repeatedly
(deck135, deck140). The misfire is **intermittent**: it appears once the pilot stops to *reason
explicitly about the mana line*, at which point it reads `{b/r}` as `{b}{r}`. So the generic
offered=payable contract is present-and-correct but does not inoculate against the specific mechanic
misread (representation-beats-instruction wall for the *generic* contract).

**Routing (guide rung is UNTRIED — take it first):**
1. **DECK GUIDE (this wave's edit):** the guide already teaches the parallel fact for the OTHER hybrid
   card — Boros Reckoner `{R/W}` -> *"three Mountains cast it"* — and the pilot OBEYS it (casts Boros
   Reckoner cleanly every window). The Rakdos Cackler entry carries **no** cost note, so the guide rung
   for THIS card was never tried. Add the card-specific fact: `{B/R}` is hybrid, one red pays it, it is
   a RED card castable off a Mountain, and if it is listed you can afford it. This attacks the ROOT
   false belief before it can form. See strategy.txt. **This un-freeze is justified** — a NEW
   decision-level failure class (offered creature not cast + token-cap spiral), and this is the RESUME
   wave, not the pause.
2. **general-suggestions.md:** flag the 2nd-seat convergence for the offered=payable class to synthesis.
   Do NOT escalate core PROSE — the pilot quoted the contract and still failed; the durable cross-deck
   fix is REPRESENTATION (option-line hybrid annotation), not louder core wording.
3. **notes.md ledger:** engine option-line hybrid annotation (`... {b/r} - hybrid: pay {R} or {B}`) as
   the durable generic backstop; plus the spiral-vs-length-cap truncation distinction.

## Blocker seam — NOT-lethal branch OBSERVED OBEYED 2/2 (high-life gang-block class stayed dead)

Only TWO blocker windows this corpus (the deck raced so hard it rarely defended). Both rendered the
lethal-arithmetic line and both were played CORRECTLY:

| seat | seq | T | my/opp | surfaced line | choice | verdict |
|---|---|---|---|---|---|---|
| 133 | 16 | 7 | 20/8 | `deal up to 2 - you would be at 18 - NOT lethal` | `no blockers` | CORRECT (Bloodghast 2/1; take 2, keep board for lethal swing) |
| 135 | 7 | 4 | 18/19 | `deal up to 2 - you would be at 16 - NOT lethal` | `no blockers` (choice 0) | CORRECT (Icehide Golem 2/2; keep goblins to attack) |

- The wave-9 headline class (high-life gang-block, e.g. wave-8 deck110 s13 gang-blocking a 6/6 at 19)
  is **OBSERVED OBEYED** — a real NOT-lethal precondition window occurred and the "declare NO blockers"
  rule was obeyed. Per the pause-aware demotion doctrine this class was OBSERVED OBEYED, but at only
  2 windows (thin) and the LETHAL branch + exception-(a) had NO window this corpus, so the block rule
  stays at full prominence (see strategy.txt freeze-check).
- **Obliterator-block guardrail: ZERO test windows AGAIN.** deck133 (the Obliterator deck) was raced
  out T8; grep of the deck133 seat file = **0** "Obliterator" hits (never cast into a combat the pilot
  answered). The NEVER-block-damage-punisher absolute neither fired nor was violated. KEEP at full
  prominence — untested guardrail against a known game-loser. No punisher-rider USE data at this seat
  (route to the deck133 seat, engine ledger #3).

## Attacker seam — 30/30 full declaration, 0 intent-collapse (the wave-9 "none" class is DEAD)

Every attacker window (30 across 6 games) declared **all** offered `A#.` attackers — verified against
the A-lists (e.g. deck110 s23 declared all 6; deck140 s29 both; single-attacker windows had only 1 legal
attacker). The wave-9 witness — deck44 seq7's **non-numeric `none` head** parsing to option 0 = no
attackers — did **NOT recur**. The answer-after-plan protocol is a clean win here: even the messiest
window (**vs44 seq26**, at 2 life with confused scratch reasoning — *"There is no winning move here? Let
me re-read the board..."*) still landed the correct final line `ATTACK: A1`. Reasoning-before-answer let
the model think through confusion without committing a stale head.

## The vs44 LOSS = matchup / variance, NO decision error

deck44 is the untouchable-board matchup (flyers + shroud: Surveilling Sprites, Archmage of Echoes). The
pilot played the guide's race line correctly: all 4 burn spells sent to FACE (2 Pillar + 2 Searing Spear,
20->18->16->14->11->8), opponent taken from 20 to 3 by T11. Endgame (opp at 3, then 2) had **no reach
left in hand** (all burn spent, correct per "race the face" against a shroud board); the opponent
stabilized behind unblockable/untargetable flyers and raced back (deck109 14 -> 2 -> -14). No lethal was
available to miss: seq24 attacked all-in at opp 3 (they blocked with flyers); seq25/26 the pilot's own
PLAN correctly recognized it could not reach 2 damage. This is the EVASIVE/UNTOUCHABLE-BOARD structural
weakness the guide already names — deck construction, not play. (Won this matchup last wave on the same
race; pure n=6 variance.)

## Fallbacks (3 total) — none changed a game

| seat | seq | kind | cause | fallback resolved to | game impact |
|---|---|---|---|---|---|
| 131 | 21 | ask | **hybrid-belief SPIRAL** (loop, not length) | `-1` (no cast) | none — won T11; a body not deployed (see HEADLINE) |
| 135 | 7 | blockers | length cut at 2048 | heuristic = `no blockers` | none — heuristic MATCHED the intended (correct) no-block |
| 140 | 16 | ask | length cut mid-calc (Searing Spear vs Stromkirk) | `-1` | none — **recovered next window** (seq17 cast Searing Spear at face) |

deck135 s7 is a mild positive: the heuristic fallback landed the same correct choice the plan was
reaching. deck140 s16 self-healed within the same turn. Only deck131 s21 is a genuine decision loss, and
it is the HEADLINE's spiral shape, not the generic length-cap class.

## Frozen guardrails / spine — held (freeze verbatim except the Rakdos Cackler cost note)
- **#1 DEPLOY-AND-ATTACK + "declare EVERY creature that can attack"** — 30/30 attacker windows fully
  declared. (The one deploy MISS is the hybrid belief at deck131 s19, fixed by the guide edit.)
- **BURN face-default + Pillar `{R}` corrective** — 0 self-target burn (0/39 target windows), 0 false
  `{1}{R}` cost beliefs. Face-by-default obeyed.
- **BLOCKING re-anchor (block ONLY on surfaced LETHAL)** — NOT-lethal branch OBSERVED OBEYED 2/2.
- **Unleash "1/1 Counter"** — always taken.
- **Legal-target names** (engine change #3) rendered in 39 windows; no fabricated-no-target confusion.
- **adj rows:** none for deck109 (all 6 games decisive). No `[deals 0]` / 0-power blocker windows. No
  fetches (N/A). Artifact-tag completeness N/A (deck109 runs 0 artifacts).

## Guide verdict: REVISED (minimal, one card-cost line)

One decision-level failure class this corpus (hybrid-mana offered=payable) justifies the first deck109
guide edit in several waves: teach that Rakdos Cackler's `{B/R}` is hybrid and red-payable, mirroring the
already-obeyed Boros Reckoner treatment. Everything else is frozen and freeze-checked against this corpus.
Label compliance 153/155 (98.7%); the 2 non-compliant are the two truncation fallbacks above.
