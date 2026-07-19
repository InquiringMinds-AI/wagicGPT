# deck14 (Deep Blue) — findings.md — wave-20 revised-guide round 2

Seat: deck14, mono-blue tempo. Corpus `matchups-20260719-151849`, binary 95e89c204.
Record **3/6** (up from 2/6). Owner-seat games:
- **WIN vs102** (adj 8-2, t18), **WIN vs49** (opp -1, t13), **WIN vs27** (adj 6-3, t23).
- **LOSS vs35** (dead 0, t12), **LOSS vs62** (dead -5, t14), **LOSS vs135** (adj 3-15, t15).
I played 3 of the corpus's 5 adjudicated games: vs102 (W), vs135 (L), vs27 (W, 23 turns).
Fallbacks this seat: **6** = 4 ask/target-side (vs102 s9 unparsed, vs27 s11 unparsed, vs27 s42
unparsed, vs35 s27 stale_echo) + 2 blockers-class unparsed (vs62 s28, vs27 s47). Matches the
brief's "6 fallbacks incl 2 blockers."

Unit of analysis is the DECISION with seq repros. Win-rate is context; the slate differs.

## 1. WHICH WAVE-19 REVISIONS FIRED — Rule #1B (deploy the finisher / whole-board swing): VALIDATED

The wave-19 revision escalated Rule #1 -> Rule #1B ("ATTACKING IS NOT ENOUGH; BE A REAL CLOCK":
(a) deploy your flyers, (b) swing the whole board), aimed at the mutated slow-clock soft-durdle
(wave-19: flyers deployed in only 1 of 6 games). Wave-20:

- **Finisher deployment: 1/6 -> 4/6.** Air Elemental / Mahamoti Djinn reached the battlefield in
  vs102 (Mahamoti t18), vs62 (Air Elemental t13), vs49 (Mahamoti t11), vs27 (Mahamoti t21). Not
  deployed only in vs35 and vs135 (both with a real cause, section 4).
- **All THREE wins are finisher games** (vs102/vs49/vs27 all rode a landed Mahamoti). The wave-19
  diagnosis ("wins when it lands a real threat, loses when it pokes with a lone body") is confirmed:
  it landed the threat 3x more often and the record moved with it.
- **Finisher deployed the turn it became available** (no "sit on the flyer" behavior): vs49 s26
  casts Mahamoti at t11 the moment 6 mana is up; vs27 DRAWS Mahamoti t21 (s42 events `library ->
  hand`) and casts it t21, attacks t22 for 5 (opp 8->3).
- **Whole-board swing (Rule #1B-b) fired where the board supported it:** vs49 t13 (s30) swings Lord
  + stolen Thunderbreak Regent + Mahamoti (3-attacker kill); vs102 t18 (s36) Lord + Axeshark.
- **vs49 is the textbook execution and a clean L->W flip** (wave-19 it died at -2): Persuasion steals
  Thunderbreak Regent (4/4 flyer, s22/s23), swings it t11+t13, deploys Mahamoti, alpha-strikes t13
  for the win. Steal-the-best + deploy-the-finisher + whole-board swing, all three levers in one game.

**Attack-floor (Rule #1) still 6/6:** every game declared attackers. Literal-floor obedience held
AND the tier-2 real-clock intent is now landing. The slow-clock durdle is substantially cured;
residual only where the deck was starved of material (section 4).

## 2. BOUNCE-ON-THE-STACK misconception — DOMINANT model error this corpus (ESCALATED, not died)

Wave-19 flagged ONE instance (vs49 s26 "Boomerang the Ashenmoor Liege [spell on stack]"). Wave-20
it is the seat's single biggest model-fixable leak — **1 costly misplay + 3 of 6 fallbacks**, all
the same shape: the model thinks Unsummon/Boomerang can target a card while it is a SPELL on the stack.

- **vs62 s18 (COSTLY MISPLAY, parsed — the one with bite):** model narrates "cast Unsummon targeting
  the Canopy Spider ON THE STACK to return it before it enters." The effectBadOrGood warning FIRED on
  the option (`the only legal targets are YOUR OWN right now (warning: this would harm your own side)`).
  Model cast it anyway. Result (s19 events): `Your Unsummon: hand->stack; Your Lord of Atlantis:
  battlefield->hand; Unsummon->graveyard; Opponent's Canopy Spider: stack->battlefield`. It **bounced
  its OWN Lord of Atlantis**, the Spider resolved anyway, it recast the Lord (s19). A wasted tempo turn
  in a game it LOST — annotation correct+rendered+DISOBEYED by the on-stack false belief overriding it.
- **vs35 s27 (stale_echo):** Boomerang TARGET CHOICE, opponent's Fury Sliver on the stack; model wants
  to bounce the Fury Sliver spell, not in the (battlefield-permanent) list; 15.1k-char thrash, no clean
  CHOICE -> stale_echo.
- **vs27 s11 (unparsed):** Boomerang TARGET CHOICE, opponent's Lord of the Undead on the stack; same
  thrash; 15.6k truncated.
- **vs102 s9 (unparsed):** Boomerang TARGET CHOICE, opponent's Shadowspear (equipment) on the stack;
  same thrash; 16.3k truncated ("I'll assume the prompt is a test of the rule that you can't target a
  spell").

In every case the model KNOWS the rule when it slows down ("you can't target a spell") yet still cast
the bounce to answer a stack spell and then thrashed at the forced target menu. The guide already
carried a passive one-liner ("cannot answer a spell on the stack") — present-and-disobeyed. This is
the constrained-diff target (section on Revision) AND the top representation item (notes E1).

## 3. HALLUCINATION WATCH — invented board object RECURRED (outcome-neutral)

Wave-19's "dropped block" was the model inventing a 5/5 Dragon. **Recurrence confirmed:**
- **vs27 s47 (blockers, t23):** the attacker menu lists **6** attackers (A1-A6, max body 5/5 Sanguine
  Guard). The reply invents "**Vodalian Zombie (7/7) deals 7**", asserts "**8 attackers**", "a 7/7 with
  protection." The Vodalian Zombie was NOT on the battlefield — it entered the NEXT turn (s47 events
  `Turn 24 ... Opponent's Vodalian Zombie: hand -> battlefield`). Two extra attackers incl a phantom 7/7.
- Context: fires INSIDE a 12.2k-char truncated combat ramble on a hopeless board (section 5). The
  hallucination co-occurs with the reasoning-tax truncation environment, not in short clean replies.
  Outcome-neutral (the real 6-attacker board was already lethal; game adjudicated me a win 6-3 before
  the swing resolved). Live class, low current cost, WATCH.

Adjacent invented-CAPABILITY (not an object): vs62 s26/s27 planned "attack with Air Elemental + Axeshark
for 7" the same turn it cast Air Elemental (summoning-sick, not in the attacker menu — only A1 offered),
typed `ATTACK: A1, A2` where A2 did not exist. Engine correctly took A1 only; harmless, same
over-optimistic board-reading family.

## 4. THE THREE LOSSES — none is a Rule-#1B regression

- **vs35 (hard matchup, not a durdle):** go-wide Sliver anthem swarm (+1/+1, flying, first strike,
  double strike via Fury Sliver). deck14 attacked t5/7/9/11 and used Persuasion+Boomerang, but racing a
  double-strike anthem swarm with 2/2 bodies is genuinely dead. The lone fallback (s27) is a
  bounce-on-stack thrash, not combat passivity.
- **vs62 (self-inflicted + strong deck):** lost partly to the s18 self-Unsummon misplay (section 2, a
  wasted turn) plus a large Enchantress board (7/9 and 7/7 Yavimaya under Gaea's Anthem). Air Elemental
  WAS deployed (t13) — Rule #1B fired — but too late against a snowballed board. The s28 blocker fallback
  was on a mathematically-dead board (section 5).
- **vs135 (DISPLACEMENT — starved, not durdling):** attack floor held (t3/5/7/9 with Merfolk of the
  Pearl Trident) but a flyer was NEVER offered (never castable) and Mahamoti/Air Elemental were NOT in
  the final prompt at all — the deck never drew/reached its finishers; attacks stopped after t9 because
  the lone 1/1 clock was outclassed. Per the skill's DISPLACEMENT rung: fixed seam (attack) healthy but
  STARVED; leak is the CAST/draw seam (no material), not the aggression rule. n=1 draw variance.

## 5. COMBAT REASONING-TAX TRUNCATION — wave-19's "class DIED" does NOT hold; mechanism shifted

Wave-19 declared the combat format-drift class dead (replies collapsed to 300-800 chars). Wave-20:
normal combats still parse clean and short (many 400-600-char attackers), BUT under two triggers the
reply blows the token cap and never reaches ANY final line:
- **vs62 s28 (blockers):** 8k+ chars, exhaustively re-derives block scenarios A-F, PROGRESSING (not a
  decode loop), truncates mid-computation. Model correctly concludes the board is mathematically lethal
  (16 trample power vs 9 life, 2 blockers).
- **vs27 s47 (blockers):** 12.2k chars, truncated; reaches a stated intent in the prose tail ("the best
  play is to block A3 (Lord of Atlantis)") but emits no `BLOCKS:` line.
- **vs27 s42 (ask):** 12.9k chars over a TWO-option menu (`Cast Mahamoti Djinn / Cast nothing`) — a
  finisher-cast durdle that truncates. (Mahamoti still landed t21 via the next turn's draw+cast.)

Distinct from wave-19's format-DRIFT (emit card-names but terminate): these are TOKEN-CAP TRUNCATIONS
(never terminate). Route to decode-side (max-token / answer-first), not guide (notes E2).

## 6. PERCEPTION-BUNDLE LIVE VALIDATION (brief's engine batch)

- **`options_text` on combat records: FIXED.** Wave-19 E3 gap (every deck14 combat logged empty
  `options_text`) CLOSED — 40/40 combat records this wave carry full options_text WITH trade annotations
  ("your blocker dies, attacker lives, 3 tramples to your face"). Combat review now possible at this seat.
- **salvageLoopedChoice extended to BLOCKS: did NOT fire on either blocker fallback** — correctly,
  because both (vs62 s28, vs27 s47) were TRUNCATED before any `BLOCKS:` line existed. Salvage needs a
  coded line; there was none. The extension is not invalidated (orthogonal to this failure mode) but the
  GAP is exposed: no salvage for truncated-before-line replies, and a PROSE-intent salvage could have
  recovered vs27 s47's stated "block A3" (notes E2).
- **#N ordinal disambiguation: NOT observed firing at this seat.** No `#N` token rendered on ANY option
  list across all 6 games, including the many duplicate-name TARGET menus (vs62 Control-Magic targets
  "Yavimaya Enchantress, Yavimaya Enchantress"; vs35 Persuasion "Gemhide Sliver, Gemhide Sliver"; vs27
  Unsummon "Lord of the Undead"; vs49 duplicate lands). No duplicate-name pick was fumbled this corpus —
  where the model selected a steal target among functionally-identical duplicates (vs35 s30) it picked the
  right BODY (Fury Sliver, the double-strike lord), and target menus render per-line P/T which
  disambiguates functionally-DIFFERENT duplicates (7/9 vs 7/7 Yavimaya) without needing #N. Verdict: the
  fix's would-matter surface (an actual selection between two same-named creatures in DIFFERENT states) did
  not occur; cannot confirm fired, no harm seen (notes E3).

## 7. PLAY-QUALITY POSITIVES (guide content validated)

- **Steal-their-best held:** Persuasion -> Thunderbreak Regent (vs49, then swung it), Persuasion -> Fury
  Sliver / Control-Magic at anthem lords (vs35). No 1/1 steals. Rule holds.
- **Bounce aimed at real threats** when NOT confused by the stack: -> Lord of the Undead (vs27 s25/s35,
  strip the zombie anthem), -> Sanguine Guard (vs27 s37), -> Ohran Viper / Icehide Golem (vs135). The
  wave-19 "re-bounce a cheap 1/1 every turn" anti-pattern did not meaningfully recur; only defect is the
  on-stack misfire (section 2).

## 8. TEMPO countermand doctrine + the 23-turn vs27 grind — SETTLED, NOT durdling

The vs27 game (23 turns, adj WIN 6-3) is NOT durdling: attack floor held every available turn
(t4/10/12/14/16/18/22); it ground opp 20->8 by t18; Mahamoti was drawn t21 and deployed+attacked
immediately (t22, opp 8->3); the single-attacker turns were forced (Gravelgill was often its only live
untapped body against a recursive Zombie lord board). The 23 turns reflect the Zombie deck's resilience
(lords + regeneration + Vodalian Zombie), not deck14 passivity. TEMPO countermand doctrine is SETTLED
(validated), not contested — Rule #1B moved finisher deployment 1/6->4/6 and every win is a finisher game.

## Revision decision: REVISE (constrained turnaround-mode diff)

Previous revision demonstrably WORKED (2/6->3/6, finisher 1/6->4/6, all wins finisher games) => TURNAROUND
mode: freeze the obeyed lines (Rule #1, #1B, #2, steal-the-best, mana-trivial), edit ONLY the one new leak.
The single warranted diff sharpens the buried, passive bounce-on-stack line into a concrete #1-MISFIRE trap
(both the UPSTREAM "do not cast bounce to answer a stack spell" and the DOWNSTREAM "the target menu is
battlefield-only; do not thrash; never self-bounce to reach the stack"). Nothing else changes byte-wise. The
durable fix remains representation/engine (notes E1); the guide trap is the interim per turnaround doctrine.
