# deck35 (Mighty Slivers) — FIRST GUIDED VALIDATION (Step 0-ter) — findings

Seat: deck35, GUR Sliver lord-stack swarm-race. **First corpus WITH its wave-17 initial guide.**
Record **4/6 — POOL LEADER**. Binary /tmp/wagic-f664539a3. Corpus matchups-20260716-185930.

## Headline: the guide's #1 job SUCCEEDED. FC1 (the defensive-spiral durdle) is DEAD.

Wave-17 guideless, this deck's win/loss axis was "did it race or did it turtle." Its two clean
losses (49, 62) were **0-attacker durdles** — it cast 5 slivers, chump-blocked them all away, and
left the opponent at 20 life having never declared an attacker. The guide's #1 rule was a direct
countermand of the core's defensive instinct. This corpus validates it.

## VALIDATION TABLE (0-attacker durdle instrument front and center)

| opp | result | my/opp | `attackers` recs | `blockers` recs | **0-attacker game?** | note |
|-----|--------|--------|:---:|:---:|:---:|------|
| 135 | WIN t9  | 17/-7 | 1 | 2 | **NO** | built board vs empty side, Fury double-strike alpha t9 = lethal |
| 21  | WIN t11 | 9/-15 | 2 | 3 | **NO** | Muscle+Bonesplitter stack read correctly, 27-power lethal swing |
| 14  | WIN t10 | 20/-2 | 5 | 0 | **NO** | Heart Sliver t2, clean race, opp never stabilized |
| 62  | LOSS t10| -3/16 | 4 | 2 | **NO** | attacked every turn; BLUE-SCREWED (no Island); lost to a 10/12 aura Spider |
| 110 | LOSS t8 | -1/23 | 2 | 1 | **NO** | attacked; out-raced by turbo-Affinity (Cranial Plating + lifelink) |
| 49  | WIN t11 | 2/-7  | 5 | 0 | **NO** | **FLIPPED from wave-17 durdle-LOSS**; Heart t3/t5, raced from turn 3 |

**Zero 0-attacker games (was 2/6). Every game reached the attack step with live creatures.**
The two wave-17 durdle losses both changed character: **deck49 flipped LOSS→WIN** (0 attackers/opp@20
→ 5 attack records/opp@-7), and **deck62 became a real race** (0 attackers/opp@20 → 4 attack records,
opp taken to 16). The durdle signature the instrument was built to catch **did not fire once.**

## Died / Persisted / Mutated — per taught class

### FC1 — defensive chump-block spiral → **DIED.** (the guide's core countermand worked)
No game turtled. Blocking is now correct and selective: the model takes small hits when healthy
("no blockers" at 135 s8/s13 while at 20+, 110 s6 at 14) and blocks only when facing lethal
(62 s13/s16 — the 10/12 Canopy Spider IS a lethal attack) or on a profitable trade (21 s18
Bonesplitter-buffed blockers eat two attackers). No sliver was thrown away one-at-a-time to shave
2–3 life off a healthy total. The core prompt's "preserve life / trade favorably" instinct that was
actively losing the deck is now overridden. **Validated; do not trim the anti-durdle spine — it is
one corpus old and load-bearing.**

### Multi-lord pump stacking → **EXERCISED and CORRECT** (resolves the wave-17 caveat)
Last wave every buff-math case was a single Muscle; multi-anthem stacking was untested. This corpus
exercised it three ways, all read correctly off the rendered line:
- **21 s16 — Muscle + Bonesplitter (two DIFFERENT pump anthems):** bodies rendered "5/3, 4/2, 4/2,
  5/3" (base 2/2 or 1/1, +1/+1 Muscle, +2/+0 Bonesplitter). The model read them verbatim, summed 18,
  attacked all four. s20: 6 creatures, "27 power," first strike + trample — correct lethal call.
- **110 s12/s13 — two Muscle Slivers stacked:** every sliver rendered 3/3 (1/1 +2/+2). Correct.
- **135 s16 — Muscle + Fury (double strike):** the model recognized the Fury double-strike
  multiplier and held one turn to assemble a lethal alpha rather than dribble in. Correct.
The buff-math VERDICT is now **PASS including multi-anthem stacking** — the model trusts the rendered
[X/Y] and does not recompute its own lord stack.

### FC2 — wrong lord priority (defensive lords over race-closers; never fetch Heart) → **LARGELY DIED**
Heart Sliver now LEADS when drawn: 14 (t2!), 62 (t3), 49 (t3 and t5). Race-closers are prioritized.
Two residues, neither game-costing:
- **14 cast Striking twice** (t4, t6) — the guide's "one Striking is plenty" was disobeyed, but the
  opponent had no board and it won at 20 life. Cosmetic.
- **62 cast Two-Headed (t7) and Spinneret (t9)** — situational lords — but its race-closers
  Winged/Shifting are BLUE and **deck62 drew zero Islands** (final board: only Forests/Mountains).
  Those situational lords were the best CASTABLE plays, not a priority error. Color screw, not FC2.
- **Shifting Sliver was STILL never deployed** across all 6 games (a 2-of needing {3}{u}; blue was
  scarce every game it mattered). Not provably a misplay — never castable when relevant — but the
  premier win-con never hit the board once in two corpora. This is a CONSTRUCTION signal (see notes).

### FC3 — blocking-math comprehension tax → **PERSISTS; now the DOMINANT residual failure.**
With FC1's wrong CHOICES gone, the wasted REASONING at the block seam did not follow it out the door
— it is now the single biggest failure mode in the corpus, and it produced the corpus's ONLY fallback:
- **62 s13 (t8): 11,215 chars.** **62 s16 (t10): 11,169 chars → `unparsed_reply` (Baka answered).**
- The s16 loop is pathological: facing a lethal attack (10/12 aura Canopy Spider + two Saprolings,
  14 power into 9 life = dead regardless), the model **distrusts BOTH representation aids the guide
  tells it to trust** — it re-derives the Spider's power from Ancestral Mask + the enchantment count,
  doubts the rendered "(10/12)", doubts the "unblocked ... deal up to 14" annotation, and cycles
  "I die either way / maybe the Spider is 4/6 / ALIVE!" for ~60 iterations without ever emitting a
  parseable BLOCK line. It self-poisons into an unparsed reply.
- **Root of the loop is a P/T-reading error feeding the distrust:** the model repeatedly read the
  Saproling **"(2/4)" as dealing 4** (reading toughness as power). Its own bad arithmetic (10+4+2=16)
  never reconciled with the CORRECT annotation (10+**2**+2=**14**), so it concluded the numbers must
  be wrong and spiraled. The annotation was RIGHT and would have ended the decision in one line if
  trusted. This is a genuine death (lost anyway), so the fallback did not cost the game — but it is a
  massive tax and a self-poisoning shape that WILL cost a game when the block is actually live.

## Losses were RACES, not durdles (the off-case, per Step 0-ter rule 6)

Both losses are exactly the "the losing corpus lives in the off-case" pattern: the guide fixed the
durdle, and what remains is being out-raced by a faster or larger opponent.
- **62 (LOSS):** BLUE color-screw stranded both evasion lords in hand; a Blanchwood/aura opponent
  assembled a **10/12 trample Canopy Spider** (Ancestral Mask + Fists of Ironwood). A base-1/1 small
  swarm cannot race a single 10/12 trampler when its own evasion is color-locked out. Attacked every
  turn it could (t3/t5/t7/t9). Construction + color variance + matchup, not a play failure.
- **110 (LOSS):** turbo-Affinity — **Cranial Plating on a lifelink Vault Skirge (6/1 flying)** +
  Signal Pest — raced to 29 life and killed deck35 on turn 8. deck35 developed 4× 3/3 flyers (2 Muscle
  stacked) but was a touch slow (Muscle t3, Winged t5, no Heart drawn → no haste) and simply behind
  the fastest deck in the pool. Attacked t5 and t7. Legitimate speed loss.

**Neither loss is a guide failure**; both are construction/variance/matchup. See notes for the
construction observations (evasion count, Shifting never landing) and the possible engine gap at 110 t8.

## What went RIGHT (do not re-teach)
- Attack-every-turn is now the model's default posture; blocking is selective and correct.
- Multi-anthem buff math off the rendered line (Muscle+Bonesplitter, 2×Muscle, Muscle+Fury).
- Heart Sliver led when drawn; Fury double-strike alpha recognized (135 s16).
- Mulligan keeps all reasonable; mana handled by the core.
