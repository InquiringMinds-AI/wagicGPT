# deck14 (Deep Blue) — findings.md — wave-21 revised-guide round 3

Seat: deck14, mono-blue tempo. Corpus `matchups-20260723-084938`, binary 3be7f5a67.
Record **2/6** (down from wave-20's 3/6). Owner-seat games (same 7-of-7 slate as wave-20):
- **WIN vs102** (adj, opp 4 / me 14, cap t13), **WIN vs135** (adj, opp 13 / me 18, cap ~t11) — both
  life-adjudicated at cap, no gameend record.
- **LOSS vs27** (dead -11, t15), **LOSS vs49** (dead 0, t10), **LOSS vs62** (dead -15, t14),
  **LOSS vs35** (dead -2, t12).
- Flips vs wave-20: **vs135 L->W**, **vs49 W->L**, **vs27 W->L**. Net -1.

Fallbacks this seat: **1** (vs27 s29 stale_echo) = 0.55% of 181 decisions. Matches brief's "14:1".
Win column is context only; the unit is the decision.

---

## HEADLINE: the bounce-on-stack class (wave-20's #1 leak — 1 misplay + 3 fallbacks) is KILLED.

**I own wave-21 item (f) validation. The engine batch fixed the class outright, in concert with the
shipped guide trap.** Two shipped surfaces:
- The CAST-option annotation `[this cannot target the spell on the stack - battlefield permanents only]`
  fired on **26 records** across all 6 games where a bounce cast was offered with an opponent spell
  pending (vs27 x9, vs102 x5, vs49 x4, vs135 x5, vs35 x2, vs62 x1).
- The TARGET-CHOICE header `NOTE: these targets are battlefield permanents only - the spell being cast
  on the stack is NOT a legal target...` — the header the brief says fired on **4 records corpus-wide**;
  **all 4 are at my seat (vs62 s19-s22)**. The one live bounce-target MENU this whole corpus is vs62 s19.

**Model behavior on the 26 annotated cast windows: 25 DECLINES ("Cast nothing"), 1 CAST — and the one
cast was CORRECT, not a misfire.** Zero self-bounces. Zero fallbacks in this class.

### The one bounce cast (vs62 s18->s19) — a CORRECT advanced play, the inverse of wave-20's misplay.
- **s18 (cast decision):** opponent casts **Web** targeting their OWN Yavimaya Enchantress (Web -> reach,
  so the 9/9 could block deck14's flyers). The annotation renders on the Boomerang option. The model READS
  it ("The menu explicitly states Boomerang cannot target the spell on the stack... It cannot counter a
  spell") and reasons to the correct line: **bounce the Enchantress itself** — the permanent the aura
  targets — so Web fizzles on resolution. Casts Boomerang (choice 1).
- **s19 (TARGET CHOICE menu, WITH the new header):** model picks **choice 10 = Yavimaya Enchantress (9/9)**
  — the opponent's biggest threat. NOT a land, NOT its own Coral Merfolk (option 15). Returns the 9/9 to
  hand, strips Blanchwood Armor, fizzles Web. High-value tempo bounce.
- Contrast wave-20 vs62 s18: the model bounced its OWN Lord of Atlantis trying to hit an on-stack Canopy
  Spider. **Same matchup, same seat, opposite outcome** — the fix flipped a self-inflicted misplay into a
  correct one. Cleanest possible A/B for item (f).

The class that was the seat's dominant leak, the reason for last wave's hoisted trap, and the deciding
rotation gate is now solved: engine annotation + header, confirmed FIRED and OBEYED on 26 cast windows +
1 menu window. See notes E1 (now CLOSED) and the Revision decision (demotion).

---

## E2 (token-cap truncation-before-line): GONE under answer-first (item b). I own the E2 re-check.

Wave-20 had 3 truncation-before-line fallbacks (2 blocker-decisive) — replies that blew the cap and
emitted NO coded line. Wave-21: **long deliberations still occur but every one parses**, because the
coded line is now emitted FIRST. Sampled the seat's longest replies (all p95+):
- vs135 s29 (15,373 chars, ask) -> first line `CHOICE: 2 (Cast Unsummon)` OK parsed.
- vs27 s28 (14,655, ask) -> `CHOICE: 3 (Cast nothing right now)` OK.
- vs35 s19 (13,005), s23 (11,882), s22 (11,342, all ask) -> `CHOICE: 3 ...` first OK.
- vs49 s6 (13,598, ask) -> `CHOICE: 3 ...` first OK.
- vs62 s21 (10,374, **attackers**) -> `ATTACK: A1` first OK (a wall of block-scenario rambling AFTER the
  answer — exactly the case that truncated-before-line in wave-20, now safe).
- vs27 s42 (9,376, **blockers**) -> `BLOCKS: B1:A2, B1:A5` first OK.

**0 truncation fallbacks, 0 prose salvages needed** at this seat. The E2 class is closed by answer-first.
No answer-quality regression from committing before reasoning: the coded first-lines match the reasoning
that follows (e.g. vs62 s21 `ATTACK: A1` is the only attacker anyway; the ramble is post-hoc).

### The single fallback (vs27 s29) is NOT E2 and NOT bounce-on-stack — classify: format-echo stale_echo.
- Context: a **normal Unsummon TARGET-CHOICE menu, no spell on the stack** (3 opponent creatures:
  Inkfathom Infiltrator / Lord of Atlantis / Zombie Master). The model picks the RIGHT target in prose
  (Inkfathom Infiltrator, option 1 — the unblockable 3/2 clock).
- Why it fell back: the coded line reads `CHOICE: 1 (Unsummon targeting Inkfathom Infiltrator)` — it
  echoed the **SPELL name** ("Unsummon"), which is in NO option, ahead of the target name. The menu
  header explicitly instructs "answer with the chosen TARGET's name (not 'Unsummon')". The parser
  Mechanism-B'd it (echo names a card in no option) -> **stale_echo -> heuristic**.
- Cost: **non-fatal.** s30 events show an Unsummon resolved (opponent returned a card to hand); the model
  cast a SECOND Unsummon at s30->s31 and there picked Inkfathom Infiltrator cleanly. The seat proves it
  CAN format a target menu right — vs62 s19 wrote `CHOICE: 10 (Yavimaya Enchantress)` (target name only).
  This is an INCONSISTENT echo-format, not a truncation and not a strategy error. Owner = harness/parser
  (see notes H1). It did not retract a choice, so `retracted_choice` does not apply; the classification
  the brief asked for is: **stale_echo, caused by spell-name-echo on a target menu, correctly routed,
  heuristic recovered, low cost.** The E2 class is otherwise entirely gone.

There were **0 `retracted_choice` fallbacks** at this seat (the corpus's 5 live elsewhere).

---

## Rule #1B (finisher deployment) — HELD. 3/6 games, but 3/3 when the finisher was DRAWN.

Wave-20 validated Rule #1B at 4/6 finisher-deployed games. Wave-21 shows **3/6 deployed** — but the drop
is entirely fewer finishers DRAWN, not a rule failure. Split per the skill's displacement rule:

| game | finisher ever in hand? | deployed? | timing |
|---|---|---|---|
| vs27 | YES (s21+) | YES | Air Elemental t14 (drawn late; cast when mana allowed) |
| vs35 | YES (s2+) | YES | Mahamoti t11 (6 mana; NOT castable until t11 — mana-gated, not sat-on) |
| vs135| YES (s9+) | YES | Air Elemental t9 -> attacked t11 (opp 17->13) |
| vs102| never drawn | n/a | won anyway via steal + Lord clock |
| vs49 | never drawn | n/a | starved; raced out t10 |
| vs62 | never drawn | n/a | starved; Enchantress snowball |

**Deployment-when-drawn = 3/3 (100%), each the turn it became castable — no "sit on the flyer".**
vs35's Mahamoti sat in hand from t1 but 6 mana wasn't up until t11 (mana development, verified against
the per-ask mana line), so it is NOT a hold-the-finisher regression. The 3 non-deployments are pure DRAW
STARVATION (finisher never in hand), the DISPLACEMENT rung, not the rule failing. Rule #1B is intact and
the wave-19/20 thesis holds: **vs135 is this wave's proof — wave-20 it lost STARVED (no flyer drawn);
wave-21 it drew Air Elemental, deployed it, and won (L->W).**

Attack-floor (Rule #1): declared attackers every game (vs27 t2/4/6/8, vs49 t3/5/7/9, vs62 t5/7/9/11,
vs35 t5/7/9/11, vs135 t3/5/11, vs102 t4/10/11). vs35 t3 "no attackers" was a considered decline. One soft
spot: **vs27 stopped attacking after t8** and slid into a defensive bounce-loop (below).

---

## The regression 3/6 -> 2/6, decomposed: draw variance + opponent improvement, NOT a batch side-effect.

- **vs135 (L->W, +1):** drew Air Elemental this corpus, deployed t9, attacked t11 (opp->13), ground the
  rest with Merfolk + Boomerang/Unsummon on Astrolabe/Ohran Viper; won at cap. Pure favorable draw variance
  — the exact game wave-20 lost to starvation.
- **vs49 (W->L, -1):** **finisher NEVER drawn, no Persuasion/Control Magic drawn** either. deck49 Dragons
  raced it (my life 20->8 by t5 off an early Dragon curve incl. Dragonmaster Outcast). Attack floor held
  (Merfolk t3/5/7, Lord t9), Unsummon bounced the Dragonmaster Outcast (s13), but 1/1-2/2 bodies can't
  pressure and opp sat at 18 all game. Wave-20 vs49 was the textbook WIN precisely because it drew
  Persuasion (stole Thunderbreak Regent) + Mahamoti; this corpus drew neither. **Draw variance.**
- **vs27 (W->L, -1):** **opponent improvement + late draw + hard clock.** deck27 Zombies played its wave-20
  revised guide and jumped 3/6->5/6 corpus-wide — a stronger opponent now. Its Inkfathom Infiltrator
  (unblockable 3/2) + Lord of the Undead recursion + Zombie Master regen ground deck14 out. deck14 held opp
  at 16 from t8->t15 (dealt ~0 damage for 7 turns) while sliding 18->-11: it fell into a DEFENSIVE
  bounce-loop on the unblockable Infiltrator (Unsummon s24/s28/s31) and drew Air Elemental only t14 — far
  too late (cast t14 at 1 life, then chump-blocked with it t15). Wave-20 vs27 was a 23-turn adj WIN; the
  improved Zombie deck closed it in 15. Guide already carries both relevant lines (attack floor +
  "don't re-bounce the same creature every turn on defense"); the Infiltrator being unblockable makes the
  bounces semi-forced. n=1, opponent-improvement-dominated — a WATCH, not a diff (notes M1).
- **vs62 / vs35 (LOSS both waves):** construction-hard matchups. vs62 = Enchantress snowball (bounced the
  9/9 correctly at s19 but the board rebuilt; Air Elemental never drawn). vs35 = Sliver anthem swarm
  (Mahamoti landed t11 but racing a double-strike anthem with 2/2s is dead). Neither is a guide/batch
  regression.

**Batch side-effect check: NONE found.** No cast-abort/stall (item i): all 14 "Already in pool" records
are ordinary mid-cast target menus, no repeat-pick-with-floating-mana loops. Partial attacker-sparing
(item a) is inert here (mono-U, all casts payable off Islands). Answer-first (b) and the bounce annotation
(f) HELPED. The record moved on draws (vs135 +, vs49 -) and one improved opponent (vs27 -), net -1 on n=6
— the rare-event/small-sample regime, not evidence anything broke.

---

## Play-quality positives (guide content re-validated)
- **Steal-their-best held:** vs102 t12 Control Magic -> **Sheoldred, Whispering One (6/6)** (opponent's
  bomb), which closed the win; no 1/1 steals anywhere.
- **Bounce aimed at real threats when not stack-confused:** Yavimaya Enchantress (vs62 s19), Inkfathom
  Infiltrator (vs27), Ohran Viper + Arcum's Astrolabe (vs135), Dragonmaster Outcast (vs49). The wave-19
  re-bounce-a-1/1 anti-pattern did not recur (the vs27 Infiltrator loop is on an unblockable clock, a
  different, semi-forced case).
- **No self-bounce anywhere this corpus** (wave-20 had one costly one).

## Revision decision: REVISE — a single DEMOTION diff (the engine fix landed; the trap is now double coverage).
Turnaround/OBEYED context. Wave-20's hoisted bounce-on-stack TRAP was the interim guide defense for a leak
whose durable fix (item f: CAST annotation + TARGET-menu header) has now SHIPPED and is CONFIRMED FIRED +
OBEYED (26 cast windows, 25 declines; 1 menu window played correctly with the header present; 0 self-bounces).
Per the skill's demotion terminus, a confirmed-fired-and-obeyed representation fix makes the interim guide
trap DOUBLE COVERAGE (attention tax) and TRIGGERS demotion (deck110 wave-7 precedent: 19-line procedure -> 10
once the marker landed). I DEMOTE the ~10-line two-part `#1 MISFIRE` procedure to ~3 lines, keeping the
load-bearing tokens — the corrective FACT ("ALREADY ON THE BATTLEFIELD", "cannot touch a spell being cast"),
the shipped marker STRING (`[this cannot target the spell on the stack]`), and the never-self-bounce anchor —
and dropping the (1)/(2) walkthrough the engine now carries at the point of decision. **Everything else is
frozen byte-for-byte** (Rule #1, #1B(a)(b), #2, YOU RARELY BLOCK, the rest of the card list, OBEY THE OFFERED
OPTION LIST, MULLIGAN).

### FREEZE-CHECK (turnaround-mode requirement)
| frozen line | status in wave-21 strategy.txt |
|---|---|
| RULE #1 (attack every turn) | verbatim, unchanged |
| RULE #1B(a) deploy flyers / race mana | verbatim, unchanged |
| RULE #1B(b) swing whole board | verbatim, unchanged |
| RULE #2 short coded combat answers | verbatim, unchanged |
| YOU RARELY BLOCK paragraph | verbatim, unchanged |
| Lord/Air/Mahamoti/Axeshark/Coral/Counsel/Control-Magic bullets | verbatim, unchanged |
| OBEY THE OFFERED OPTION LIST | verbatim, unchanged |
| MULLIGAN | verbatim, unchanged |
| Unsummon/Boomerang bullet | **DEMOTED** (only changed line): kept "clear a blocker / reset persist / strip aura / remove tempo", kept the don't-re-bounce-a-1/1 line, kept corrective fact + marker string + never-self-bounce; dropped the (1)/(2) procedure now carried by the engine annotation+header |

Byte delta: 7723 -> smaller (one bullet compressed). No new permissive text added (the aura-fizzle line
the model executed correctly at vs62 s19 was deliberately NOT added — the model already does it unprompted;
adding it risks over-generalization).
