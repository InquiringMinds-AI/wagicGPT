# Wave-22 review — deck109 (Hellrider, mono-red aggro / RDW) — ROTATED IN, re-validation

**Corpus:** PRIMARY `matchups-20260723-173843`, round-robin, deck109 seat = 6 games.
Binary `/tmp/wagic-1d8ed8194` (wave-22 step-1 batch; THIS corpus is its live validation).
Pool: 27/44/62/102/135/140 + deck109. Guide FROZEN from wave-11 (md5 9b82b500ac675be6936d2a31739ca615),
last reviewed wave-13 (5/6). First deck109 corpus under the MODERN engine (answer-first protocol,
trade annotations, castability/hybrid annotations, parser hardening).

**Record: 3/6** — beat 27, 140, 102; lost to 135, 44, 62. Context only; decompose by decision.
vs140 INVERTED again (wave-13 lost to 140; this wave BEAT it by grinding a flood to a T20 Hellrider
kill). The three losses are race/matchup losses to structural-edge opponents (bigger midrange,
evasive-faerie tempo, green voltron-flier) — the untouchable/evasive class the guide already covers.

| opp | result | end (my/opp, turn) | class |
|---|---|---|---|
| 27  | WIN  | 3 / -4, T13  | earned grind (wide Goblin swarm) |
| 140 | WIN  | 20 / -1, T20 | earned — flooded, ground it out, Hellrider haste T20 |
| 102 | WIN  | 20 / 0, T11  | earned race |
| 135 | LOSS | -8 / 1, T10  | close race — got opp to 1, out-raced by a bigger board a turn later |
| 44  | LOSS | -9 / 2, T13  | evasive faeries (Obyra drain) — untouchable-board class |
| 62  | LOSS | -1 / 9, T12  | green voltron-flier (5/8 Birds w/ Blanchwood+auras) — untouchable class |

**Fallbacks: 0/121 decisions at this seat** (brief confirms 109:0).

---

## MANDATE (1) — Answer-first holds on an aggro seat, incl. the pathological longest replies

Sampled the 8 longest replies (9.2k-13.6k chars). **All 8 emit the coded line (CHOICE:/ATTACK:) as
the FIRST non-empty line — AF=True 8/8.** Robust even when the following reasoning is enormous or
spirals. Two of the eight have the first line diverge from the recorded action — both are the
shipped hardening working, not a regression:

- **vs62 s16 (T7, ask):** first line `CHOICE: 2 (Cast nothing right now)`, recorded `choice=1 (Cast
  Pillar of Flame)`. The model spiraled, self-corrected, and emitted a LATER `CHOICE: 1 (Cast Pillar
  of Flame {r})`; the parser's **last-well-formed-CHOICE-wins** rule took the final (correct,
  aggressive) answer. Working as intended.
- **vs62 s21 (T9, attackers):** first line `ATTACK: A1, A2, A3, A4` (all 4 eligible), recorded only
  2 (`Goblin, Legion Loyalist` = A1+A4; next record's combat log confirms `declared attackers:
  Goblin, Legion Loyalist`). This one IS an anomaly — notes Ledger A. The reply is the corpus's
  pathological case: the model looped "I will declare no attackers … but the guide says attack …
  wait" ~15 times against a confusing board (Briar Patch -1/-0 + an unkillable 5/8 flier) and blew
  max_tokens (truncated mid-sentence). Answer-first still held (A1-A4 first); the under-count is a
  downstream parse/salvage artifact of the truncated spiral.

Verdict: **answer-first PASS.** The spiral (model-behavior) routes to general-suggestions; the
attacker under-count routes to notes Ledger A.

---

## MANDATE (2) — Old-guide re-validation under the modern engine; CRUTCH RETIREMENT

### The hybrid option-line annotation SHIPPED — wave-13 Ledger Item B is CLOSED

Wave-13 recorded "option-line hybrid annotation still UNSHIPPED; card notes the sole carrier." It
has shipped. The engine now renders:
- `Cast Rakdos Cackler {b/r} (1/1) [hybrid: each {b/r} pays with B or R - total 1 mana]`
- `Cast Boros Reckoner {r/w}{r/w}{r/w} (3/3) [hybrid: each {r/w} pays with R or W - total 3 mana] {right now: damage 0}`

Both CLOSED-criteria of the skill's crutch-lifecycle rung (wave21 skill L94-96) are met here:
- **Hybrid confusion -> 0**: grep of every reply for "lacking black" / "no black mana" / "need
  black" / "two mana" = 0 hits.
- **Casts executed**: Rakdos Cackler 2/2 (vs27 s24, vs135 s3), Boros Reckoner 1/1 (vs44 s23), all
  with the annotation present, all off Mountains.
- **Unleash "1/1 Counter" taken 8/8** (vs27 s7/s21/s25, vs140 s18/s22, vs135 s4, vs44 s11, vs102 s15).

-> **RETIRE** the verbose Rakdos Cackler hybrid lecture; demote to a lean line (take the 1/1 counter;
the option line shows a Mountain pays it). Same for Boros Reckoner's "three Mountains cast it".

### Pillar of Flame cost-correction crutch — RETIRE

Every Pillar option renders exact cost `Cast Pillar of Flame {r}`; 0 mis-costings. -> RETIRE the
"{R}, not {1}{R}" corrective, keep "fire at face". Engine carries the cost.

### Attacker-sparing fix (mandate f) — N/A at this seat; attack windows all fire

deck109's creatures have no mana abilities (20 basic Mountains), so no would-be attacker can be
tapped for mana — the attacker-sparing fix is structurally N/A here. The testable half — do attack
windows fire every turn there is an eligible attacker? — PASSES: vs27 T5/7/9/11/13; vs44
T2/4/6/8/10/12 (every turn); vs62 T1/3/5/7/9/11. The one apparent gap (vs140 T8->T20) is NOT a
missed window — the board was wiped and deck109 had 0 creatures T10-T18 (confirmed on the board
line of s21-s27) while flooding, until Hellrider (haste) landed T20 and won.

### Frozen guardrails that HELD (keep)

- **DEPLOY-AND-ATTACK**: every castable creature deployed each turn; every eligible attacker
  declared (except the vs62 s21 spiral under-count — harness artifact, not disobedience).
- **BLOCKING re-anchor**: 2 blocker windows (vs27, vs135), both correct all-declines on the
  NOT-lethal branch (parseable `Bn:none`). LETHAL branch + Obliterator still untested (raced) — KEEP.
- **EVASIVE/UNTOUCHABLE**: LOAD-BEARING — 2 of 3 losses (vs44, vs62) are this class. SHARPENED, not
  demoted.

---

## MANDATE (3) — Burn targeting vs trade annotations: CLEAN

19 burn/target windows. **0 burn at own face/creatures.** The one own-creature target (vs102 s12,
only two own Goblins offered) was a FORCED effect (opponent edict with only own creatures legal), not
burn — correct.
- **Face by default**: 17/19 at "The opponent (player, life N)". Correct every default window.
- **Exception fired**: vs44 s13 Searing Spear killed **Obyra, Dreaming Duelist** (2/2 flash flier
  that drains you 1 each time another Faerie enters) over the face — a recurring drain/value engine
  and an evasive body. Revised guide names the life-drain class explicitly (wave-11 guide only listed
  token-makers/spell-triggers/lifegain).
- **Boros Reckoner {RW} first-strike priority**: 8 windows (vs44 T10-13), all offering only the pump
  with `{right now: damage 0}` (not in combat) — model PASSED 8/8. Guide's "pay only in combat vs a
  3-power creature" held; no mana wasted.

---

## The three losses — matchup/race, no decision error of consequence

- **vs135 (T10, close):** raced opp 20 -> 1 by T9 (all burn face, full swings); deck135's bigger
  board (Abominable Treefolk 7/7 trample) turned the corner a turn before deck109 closed. Correct
  all-in T9 line; opp blocked one and survived at 1, then crashed back for lethal. No un-fired burn
  (no cast window T9 -> no burn in hand). Draw-variance.
- **vs44 (T13):** evasive faeries. Killed the drainer (s13), raced face (opp 20 -> 2), couldn't block
  the fliers or close the last 2. Untouchable-board ceiling. Priority passes all correct.
- **vs62 (T12):** green voltron grew one Birds of Paradise into a 5/8 flying-trample carrier
  (Blanchwood Armor + Web + Fists) unblockable by ground creatures and unkillable by burn (7
  toughness). Only kill window was T3 when Birds was a 2/3 mana dork — but at T3 it read as a dork,
  and burning a dork face-first is the correct standard aggro line; no writable rule reliably fires
  there. The s21 spiral dropped ~3 power but the game was structurally lost to an unraceable flier
  regardless. Do NOT relitigate deck construction at a re-validation — inherent RDW no-reach-vs-
  evasion ceiling the guide documents.

## Guide verdict: REVISED (targeted crutch retirement + one sharpen), spine frozen

Retired two representation crutches now carried by the engine (hybrid mana lecture, Pillar cost
correction); demoted a third (Boros mana explanation). SHARPENED EVASIVE with the "ground creatures
can't block their fliers -> attacking is FREE, swing every turn, don't recalculate" line (targets the
vs62 s21 over-agonizing shape) and added a COMMIT-FAST rung to the attack rule. Named the life-drain
class in the burn exception (Obyra validated). No spine rule changed.
