# Wave-11 review — deck109 (Hellrider, mono-red aggro / RDW)

**Corpus:** PRIMARY `matchups-20260715-072254`, round-robin, deck109 seat = 6 games. Binary
`/tmp/wagic-54a5a9d01` (wave-10 engine batch + max_tokens 4096 + parse ramble guards +
stack-target annotation fix + fetch pre-targeting keying). Wave-10 guides (deck109 carries the
+1 hybrid line added last wave). Core prompt unchanged (7th PASS). Answer-after-plan reply protocol.
Guide in play = the LIVE `deck109_strategy.txt` (byte-identical to wave-10's revised output).

**Record: 5/6** — beat 135, 140, 110, 44, 131; LOST to 133. Per the USER DOCTRINE this is context
only. NOTE the matchups **inverted** vs wave-10 (last wave: lost 44, beat 133; this wave: beat 44,
lost 133) — pure n=6 draw-speed variance on the two structural-edge-case opponents (untouchable
flyers 44; grindy devotion+Obliterator 133). The unit of analysis below is the DECISION.

| seat file (epoch) | opp | winner | end (my / opp, turn) |
|---|---|---|---|
| 1784118181 | 135 | 109 | 17 / -2, T11 |
| 1784121838 | 140 | 109 | 20 / -5, T9 |
| 1784122253 | 110 | 109 | 9 / 0, T9 |
| 1784122965 | 44  | 109 | 13 / -5, T11 (**beat the untouchable-flyers deck this time**) |
| 1784123208 | 133 | opp133 | 0 / 6, T11 (**loss** — Obliterator/devotion race lost on draw speed) |
| 1784123504 | 131 | 109 | 18 / -4, T13 |

---

## HEADLINE — the wave-10 HYBRID-MANA line is VERIFIED end-to-end (CLOSED, kept at full prominence)

The wave-10 guide edit (teach that Rakdos Cackler's `{B/R}` is a HYBRID symbol, one red pays it,
castable off a single Mountain, and if listed it is affordable) was this seat's ONLY decision-level
failure class last wave. **This corpus closes it end-to-end.** All three decline/spiral signatures
from wave-10 (deck131 seq18/19/21) went to ZERO, and both directions checked out:

- **0 "lacking black mana" / "no black" / "lack black" declines** across all 6 games (was 3 windows
  wave-10). `grep -c` = 0 in every seat file.
- **0 spiral markers** ("game is broken" / "is a trick" / "misleading" / "maybe the option") across
  all 6 games (was 1 full-budget spiral -> `unparsed_reply` -> `-1` wave-10). The wave-10 Ledger
  Item B loop truncation did NOT recur at this seat.
- **Offered `{b/r}` casts TAKEN 2/2.** Both windows the engine offered `Cast Rakdos Cackler {b/r}`:
  - vs44 seq3 (T1, mana `{R}`): CHOICE 1 = cast. Reply: *"Rakdos Cackler costs {B/R}, which is a
    hybrid cost payable with a single Red mana."* — **direct positive echo of the new guide line**,
    read and internalized verbatim; preferred it over Stromkirk Noble as the bigger unleash body.
  - vs131 seq3 (T1, mana available): CHOICE 1 = cast, clean develop-the-board reasoning.
- **Unleash "1/1 Counter" taken 5/5** (vs135 s25, vs110 s7, vs44 s4, vs133 s7, vs131 s4) — the
  guide's "ALWAYS take the 1/1 Counter" held.

**Verdict on the line (confirmed-fired-representation terminus):** this is the *shipped-and-obeyed*
close of the wave-10 headline-4 ladder (deck-guide card note as the interim rung, tried and now
VALIDATED). **KEEP at full prominence — do NOT demote.** Two reasons, both doctrine:
(1) the sample is THIN (only 2 direct offer windows this corpus; the guide-card-note is the SOLE
carrier of the fact); (2) the durable cross-deck backstop (Ledger Item A: the option-line hybrid
annotation `... {b/r} - hybrid: pay with {R} or {B}`) **did NOT ship** this binary — verified 0
`- hybrid:` strings in any deck109 prompt. Per the pause-aware / thin-sample demotion doctrine, a
stopgap whose replacement has not shipped and whose class cleared on few windows stays at full
prominence. The guide is therefore FROZEN byte-identical (the line stays exactly as written).

## NEW find (single-seat, reply-protocol layer) — plan fixates on an UNLISTED card -> out-of-range index

**vs135 seq12, T7, myL19 oppL17.** Options were exactly four: `Cast Pillar of Flame {r}` (1),
`Cast Searing Spear {1}{r}` (2), `Cast Krenko's Command {1}{r}` (3), `Cast nothing right now` (4).
The PLAN abandons its own previously-planned LISTED play ("My previous plan to cast Krenko's Command
is too slow") and fixates on **Hellrider — which is NOT among the options** ("I have enough mana to
cast the 3/3 Hellrider right now... I will cast Hellrider this turn"), then emits **`CHOICE: 5`**
(out of range for 4 options) -> parser returns `-1` -> heuristic fallback.

This is a **2nd witnessing seat** for the wave-10 **deck110 P3** single-seat sub-variant (current
skill, headline 2: *"the plan fixates on an UNLISTED card and emits a poorly-mapped fallback label
that contradicts its own sub-conclusion about the listed options"* — WATCH, promote only with a 2nd
seat). deck109 vs135 seq12 supplies that 2nd seat. It is DISTINCT from the deck140 P2 answer-last
trailing-index defect (there the reasoning is CORRECT and only the index contradicts it; here the
reasoning reaches an ILLEGAL choice — an uncastable card — and the out-of-range `5` faithfully
encodes that illegal choice, as if Hellrider were an imagined 5th option). Routing: **reply-protocol
/ representation layer** (notes.md Ledger Item A' + general-suggestions convergence flag). It is
**NOT a guide edit and NOT a core edit** — the guide already forbids fixating on an uncastable card
("Do not decline a listed creature because you are fixated on a card you cannot pay for"), and the
convergence gate forbids a single/second-seat item adding a core line. Non-game-changing (vs135 was
WON; the heuristic fallback resolved it); the abandoned LISTED play (Krenko's Command) was the
correct develop, so the cost was mild tempo only.

## Blocker seam — chump-on-LETHAL-only OBSERVED OBEYED 1/1 (the deck raced even harder this wave)

Only ONE blocker window all corpus (the deck raced so hard it almost never defended — 1 vs 2 last
wave):

| seat | seq | T | my/opp | surfaced line | choice | verdict |
|---|---|---|---|---|---|---|
| 135 | 7 | 4 | 20/19 | `deal up to 2 - you would be at 18 - NOT lethal` | `BLOCKS: none` (choice 0) | CORRECT (Ice-Fang Coatl+Golem 3 dmg; keep goblins to attack) |

- The NOT-lethal branch was OBSERVED OBEYED (a real NOT-lethal precondition window occurred and
  "declare NO blockers" was obeyed). Sample is THIN (1 window) and the LETHAL branch + exception-(a)
  (gang-block a token-maker/lifegain you could not burn) had NO block window this corpus, so the
  BLOCKING block stays at full prominence (freeze-check).
- **Obliterator-block guardrail: ZERO test windows AGAIN (3rd consecutive wave).** vs133 (the
  Obliterator deck) cast Phyrexian Obliterator (113 hits in the opp seat) but deck109 got **0
  blocker-kind windows** in that game — it RACED (attacked every turn, took the hits, never declared
  a block). The NEVER-block-damage-punisher absolute neither fired nor was violated. **KEEP at full
  prominence** — untested guardrail against a known game-loser. Punisher-rider USE verification
  remains at the deck133 seat (engine ledger #3, still owed).

## Attacker seam — 26/26 full declaration, 0 intent-collapse (the wave-9 "none" class stays DEAD)

Every attacker window (26 across 6 games; fewer than last wave's 30 only because the games were
shorter/faster) declared **all** offered attackers, with the count growing as the board developed
(A1..A5 plus named Goblins). No non-numeric `none` head, no answer-last stale-head collapse. Steady.

## Phantom-lethal watch (carried) — CLEAN, no recurrence

Four replies asserted "lethal"; all four are genuine or correctly-hedged, and in **every** case the
pilot still DEPLOYED (the phantom-lethal class is a PLAN asserting lethal on a hallucinated board and
DECLINING development — that did NOT recur):

- vs110 seq18 (opp 4): genuine — Hellrider + Legion Loyalist FS/trample, "at least 9"; cast Hellrider
  then swung. CORRECT.
- vs131 seq20 (opp 10): genuine w/ Hellrider pings (5 attackers = 5 unblockable pings + combat);
  cast Hellrider, attack all. CORRECT.
- vs133 seq17 (opp 8): correctly HEDGED ("path to lethal this turn **or next**"); cast the only
  listed creature (Stromkirk Noble). CORRECT deploy.
- vs131 seq19 (opp 10): correctly computed 8 power -> opp to 2 -> "lethal **next turn**"; played the
  land. CORRECT (no false current-lethal claimed).

## The vs133 LOSS = matchup / draw-speed variance, NO decision error

vs133 is the devotion + Phyrexian Obliterator + Gray Merchant grind deck. deck109 played the guide's
race line exactly: burn to FACE by default (opp 20->18->17->14->13->10->8->6), a body deployed every
turn, all attacks declared. Its board was killed T9 ("Legion Loyalist and Stromkirk Noble were killed
last turn"), it had no reach left in hand, and the opponent's faster clock (Obliterator + devotion
online) closed at T11 (deck109 20->18->15->11->0). It got the opponent to 6 but lost the race by a
turn. No lethal was missed; no misplay. deck109 WON this exact matchup last wave by racing it out T8
— the only difference this wave was draw speed. Deck-construction weakness (no card advantage / no
answer to a grindier deck), already named; do NOT relitigate at 5/6.

## Fallbacks (4 total) — none changed a game, none are hybrid spirals
| seat | seq | kind | cause | resolved to | game impact |
|---|---|---|---|---|---|
| 135 | 12 | ask | **unlisted-card fixation** (plan wants Hellrider; emits `CHOICE: 5` out of range) | `-1` (heuristic) | none — won T11; mild tempo (abandoned correct Krenko's Command) — see NEW find |
| 135 | 14 | ask | empty_reply (HTTP 120s timeout, known corpus class) | `-1` (heuristic) | none — won |
| 135 | 24 | ask | empty_reply (120s timeout) | `-1` (heuristic) | none — won (opp already at 3) |
| 131 | 5  | ask | empty_reply (120s timeout) on a Play/Hold Mountain land drop | `-1` (heuristic) | none — won |

3 of 4 are the KNOWN corpus-wide empty_reply (120s timeout) class (WAGIC_GPT_TIMEOUT 240s shipped
next corpus — do not re-diagnose). All 4 landed in games deck109 WON.

## Frozen guardrails / spine — held (freeze-checked verbatim against this corpus)
- **#1 DEPLOY-AND-ATTACK + "declare EVERY creature that can attack"** — 26/26 attacker windows fully
  declared; every castable creature deployed. (The one develop MISS is the vs135 s12 unlisted-card
  fixation — a reply-protocol defect, not a guide-obedience miss; the guide already forbids it.)
- **BURN face-default + Pillar `{R}` corrective** — 0 self-target burn (0 across all target windows),
  0 false `{1}{R}` cost beliefs; killed Young Pyromancer (vs131 s7) and lifelink Vault Skirge
  (vs110 s11) as the named exceptions.
- **Rakdos Cackler `{B/R}` hybrid note** — VERIFIED end-to-end (see HEADLINE); frozen at full
  prominence.
- **BLOCKING re-anchor (block ONLY on surfaced LETHAL)** — NOT-lethal branch OBSERVED OBEYED 1/1;
  LETHAL branch + exception-(a) + Obliterator absolute untested this corpus, KEPT.
- **Unleash "1/1 Counter"** — always taken (5/5).
- **Legal-target names** (engine change #3) rendered; no fabricated-no-target confusion.
- **EVASIVE/UNTOUCHABLE BOARDS note** — load-bearing; vs44 WIN this wave against this exact class
  (won the race the guide prescribes; the note earns its place).
- **adj rows:** none for deck109 (all 6 games decisive). No `[deals 0]` / 0-power blocker windows.
  No fetches, no discard, no artifacts (all N/A for this decklist).

## Guide verdict: FROZEN (byte-identical to the live guide)

No decision-level failure class this corpus routes to the guide. The one wave-10 edit (hybrid line)
is VERIFIED and stays exactly as written; the one NEW find (unlisted-card fixation) routes to the
reply-protocol layer, not the guide. `strategy.txt` is byte-identical to `deck109_strategy.txt`.
Attacker/blocker seams 27/27 clean; 96/100 ask windows emitted a valid parsed CHOICE (the 4
fallbacks above are the 3 timeout + 1 out-of-range windows).
