# Deck-110 (Etched Affinity) — Wave-16 game review (2nd per-deck-rotation cycle)

**Corpus.** PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-084532/`, 6 deck110 seat
translogs (`*ai_baka_deck110-0x*.jsonl`), round-robin over 44,62,133,135,17,21 (deck140 rotated
OUT; deck62 Enchantresses entered GUIDELESS). Binary `/tmp/wagic-69793930a` (wave-15 engine batch:
free alternative casts at zero mana, stale_echo option-subset fallback, hybrid-pip affordability
annotation — none of which fire materially at this mono-cost artifact seat). Wave-11 guide FROZEN;
live `deck110_strategy.txt` md5 `00849aa9` (byte-identical since wave-8; **9th freeze**). Reply
protocol = answer-after-plan + name-echo (`CHOICE: N (name)`). Card facts vs
`bin/Res/sets/primitives/mtg.txt`; representation facts vs live prompts. Spark @ GPU_MEM_UTIL=0.75.
Owned items this wave: **E2** (equip churn A->B->A), **E6** (Mox Opal "Cast Card Normally"), plus
standing validation of **Galvanic** (two `Damage:2` lines = 4, COUNT instances) and **attackers
reconcile**.

## Record: 4 WIN / 2 LOSS (win table 110: 4/6; results.tsv confirmed)

| seat opp | result | end life (my/op) | T | one-line |
|---|---|---|---|---|
| deck62  (Enchantresses)   | **WIN**       | 13 / -7 | 10 | clean aggro; Etched Champion protection carrier walked past green Saprolings |
| deck44  (Faerie)          | **WIN**       | 25 / -10| 11 | flooded bodies, Signal-Pest go-wide, lethal alpha s34; 2 Blast kills on fliers |
| deck135 (Modern Snow)     | **WIN**       | 20 / -3 | 8  | Galvanic reach + Plating on Steel Overseer; **equip MOVE was an upgrade, not churn** |
| deck17  (Faeries)         | **WIN** (adj) | 20 / 14 | 8  | slow flier grind, ahead comfortably; 3 unparsed rambles (all heuristic-answered) |
| deck133 (Ph. Asphodel)    | **LOSS**      | 0 / 16  | 13 | Inquisition+Fatal Push+Geralf recursion; **drew 3 lands + Plating in a row = flood** |
| deck21  (Goblin Gang)     | **LOSS**      | -5 / 16 | 9  | fast-race + land-heavy draw + **self-Galvanic-Blasted its own Steel Overseer (blunder)** |

results.tsv cross-check: `110 62 0 13 -7 10`, `110 133 1 0 16 13`, `110 17 adj0 20 14 8`,
`110 21 1 -5 16 9`, `44 110 1 -10 25 11`, `110 135 0 20 -3 8` — all consistent. The two losses are
NEW (deck110 won both vs133 and vs21 last wave); deck140 (the wave-15 loss) rotated out. This is a
**FREEZE wave**: no guide line earned a change. Substance = two shipped-fix VALIDATIONS (both PASS
again), E2/E6 non-reproduction, and one NEW harness finding with a real outcome cost.

---

## FINDING 1 — VALIDATION / standing: Galvanic Blast metalcraft is CORRECT on both arms across the WHOLE corpus — 11 casts, 0 errors. Method trap re-confirmed.

Counting `Damage:` INSTANCES per Blast cast (NOT deduping the string) against the surface
`Artifacts in play: you N` in the same prompt:

| game | seq | surface N | metalcraft | Damage instances | total | target | correct? |
|---|---|---|---|---|---|---|---|
| deck133 | s16 | **2** | OFF | `2`           | **2** | opponent face          | yes (2 at <3) |
| deck44  | s14 | **2** | OFF | `2`           | **2** | opponent face          | yes (2 at <3) |
| deck62  | s14 | 4 | ON | `2`,`2`           | **4** | opponent face          | yes |
| deck17  | s9  | 3 | ON | `2`,`2`           | **4** | Cloud Sprite (kill)    | yes |
| deck17  | s22 | 4 | ON | `2`,`2`           | **4** | opponent face          | yes |
| deck21  | s8  | 3 | ON | `2`,`2`           | **4** | **own Steel Overseer** | dmg correct; TARGET blunder (Finding 5) |
| deck21  | s15 | 5 | ON | `2`,`2`           | **4** | opponent face          | yes |
| deck44  | s17 | 3 | ON | `2`,`2`           | **4** | Oona's Gatewarden (kill)| yes |
| deck44  | s26 | 6 | ON | `2`,`2`           | **4** | Surveilling Sprite (kill)| yes |
| deck135 | s18 | 5 | ON | `2`,`2`           | **4** | opponent face          | yes |
| deck135 | s26 | 6 | ON | `2`,`2`           | **4** | opponent face          | yes |

BOTH arms correct in every instance: 2 at N<3, 4 at N>=3. The wave-13 flat-2 card-script bug remains
CLOSED. The verification TRAP holds exactly as prior waves warned — a naive string-dedup grep collapses
the two identical `Damage: 2` lines and mis-reports a metalcraft Blast as "2"; you MUST count instances
or read the life delta (I re-hit this: the first dedup pass reported single lines, the instance-count
pass showed `2,2`). Layer: ENGINE/CARD-SCRIPT, RESOLVED — verification only, NO guide item.

## FINDING 2 — VALIDATION / standing: attackers name->index reconcile works — 17 attacker declarations, no eligible attacker dropped; MIXED-form and summoning-sick exclusion both correct.

17 attacker windows audited against each prompt's eligible roster + declared-attacker events:
- MIXED `A1, <name>` and pure-index forms resolve to the correct eligible set (deck62 s9
  `A1, Etched Champion` -> 2; deck62 s16/s21 `A1,A2`; deck44 s30/s34 four-body spreads all declared).
- **Summoning-sick names correctly EXCLUDED** (deck135 s6 emitted `A1, Steel Overseer` but Steel
  Overseer was summoning-sick/tapped -> reconcile kept only Signal Pest; same M1(a) pattern as wave-15).
- Defensive hold-backs correct: deck21 s18 `no attackers` at 6 life (kept blockers); deck135 s20
  `no attackers` (going for Galvanic lethal). No eligible attacker was ever dropped.

Layer: attackers-parser, RESOLVED — verification only. The wave-13 MIXED-form silent-drop stays closed.

## FINDING 3 — E2 (equip churn) did NOT recur this wave. Every equip was legit: first-equip, a genuine UPGRADE move, or a post-death rebuild. Rule #1 obeyed 100%.

Only 1 Cranial Plating was ever attached in every game (deck62 drew 2 but spread onto naked carriers).
All equip sequences classify clean by OUTCOME DELTA (the E2 lever):
- **deck62**: equipped Etched Champion (protection carrier #1), passed every `(ALREADY)` re-offer
  (s20/s22/s24). Correct.
- **deck135**: s10 equip Signal Pest (2/2 w/counter -> 7/2, best available; Steel Overseer tapped),
  then s15 MOVE to Steel Overseer. This is an **UPGRADE, not churn** — Steel Overseer had grown to
  9/3 via its own counters, so the move raises resulting power (9 > 7) onto a tougher body; the E2
  outcome-delta lever would correctly ALLOW it. Then held (s19/s27 pass). NOTE a prose-vs-choice
  divergence at s15 (prose said "I will NOT equip Steel Overseer yet" but emitted CHOICE:2 = equip it)
  — the emitted choice was right; the prose was a stale carry (see Finding 5).
- **deck17**: s14 equip Vault Skirge (6/1 flyer), passed `(ALREADY)` (s15/s18), Vault Skirge DIED in
  combat s20 (traded with Plumeveil), so s22 re-equip onto Steel Overseer = **legit post-death
  rebuild** per Rule #1. Then held.
- **deck133**: s18 equip Vault Skirge (4/1 lifelink flyer), passed `(ALREADY)` (s19/s21). Correct.

**Zero `(ALREADY — change NOTHING)` options were TAKEN** anywhere (all such were passed). E2's
identity-keyed blind spot (the net-zero downgrade MOVE) got no exercise because the model made only
value-positive moves this wave. The outcome-delta lever remains the right unfixed engine fix, now
validated-by-absence (it would ALLOW deck135 s15 and pass everything else). Layer: engine ledger,
UNFIXED but non-biting this wave. MEDIUM/dormant.

## FINDING 4 — E6 (Mox Opal "Cast Card Normally" mislabel) NOT exercised this wave. No new evidence; ledger item stays open.

Every Mox Opal interaction this corpus was a plain **`Cast Mox Opal`** (casting the artifact itself
— deck17 s3, deck133 s4, deck21 s3, deck44 s4/s5), which serializes cleanly and the model handled
correctly every time. The wave-15 confusion shape — a `Cast Card Normally with Mox Opal {Metalcraft
- {T}: Add one mana...}` line echoing the rock's OWN text into the card slot — did NOT appear at this
seat this wave (the wave-15 witnesses were both in one specific deck44 board state that did not recur).
Not-exercised != not-fixed. E6 remains an OPEN representation ledger item (owner: `AIPlayerGPT.cpp`
cast-with-mana-source serialization); this seat contributes no confirm/deny this cycle. LOW-MED.

## FINDING 5 — NEW / HIGH (harness): the "YOUR PLAN (as you last stated it)" carry re-injects the model's own STALE/confused reasoning, and it caused a real outcome-costing blunder — deck21 self-Galvanic-Blasted its own Steel Overseer.

Every decision prompt carries a `YOUR PLAN (as you last stated it): ...` block echoing the model's
previous full PLAN prose. When that carried plan is a clean resolved intent it aids continuity; when
it is STALE relative to the advanced game state (a card was already cast, the phase/menu changed, or
the prior plan ended mid-"Wait..." spiral), re-injecting it actively harms. Three harms observed:

**(a) The clean causal blunder — deck21 s6->s8 (a LOSS).** T2, race vs Goblin Gang.
- s5: cast Steel Overseer (choice 2). Correct.
- s6: menu is now `[Galvanic Blast, nothing]` (Overseer already cast) — but the carried plan still
  reads "I will cast Steel Overseer ... I will NOT cast Galvanic Blast yet." Its plan names an action
  no longer on the menu; unable to reconcile, it emits **CHOICE:1 = Cast Galvanic Blast** (the action
  its prose explicitly rejected).
- s7: now selecting the Blast's TARGET `[you, Steel Overseer(yours), opponent, Raging Goblin]`. The
  carried plan says "cast Steel Overseer," so it picks the option whose NAME matches — its **own Steel
  Overseer** — and the log (s8) resolves `Damage 2 + Damage 2 -> Steel Overseer -> graveyard`: it
  **metalcraft-blasted its own just-cast creature to death**. A self-inflicted 2-for-1 (lost a
  creature AND wasted a Blast) in a race it went on to lose. Neither s6 nor s7 was a fallback
  (fb=None) — these were the model's reasoned mis-choices, and name-echo did NOT catch it because the
  echoed option string ("Steel Overseer (your battlefield)") is a real offered option.

**(b) The unparsed loops — deck21 s13, deck133 s9.** Both giant `Wait...Wait...` spirals (~14k chars ->
ramble-guard -> `unparsed_reply` -> heuristic) were seeded by the carried plan CONTRADICTING the current
prompt: deck21 s13's carry described an Upkeep menu ("the list only shows Counter with Steel Overseer")
while the current menu was Main-phase `[Galvanic, Memnite, nothing]`; deck133 s9's carry asserted "Mana
available: {r} from 1 untapped source" while the current prompt read `{r}{w} from 2 untapped sources`.
The stale carry + the new prompt = the model re-litigates the contradiction until it times out.

**(c) prose-vs-emitted-CHOICE divergences** (deck135 s15, deck21 s6) are the same family — the emitted
CHOICE follows the current menu while the PLAN prose describes the stale carried action.

Mechanism is NEW at this seat (grep of all prior deck110 waves: no "last stated"/plan-carry item).
It is CROSS-DECK (every seat carries a plan) but deck110 produced the cleanest repro WITH an outcome
cost. NOT guide-fixable (fighting a prompt-assembly bug with guide text adds noise). Harness fix:
when the carried plan's named action is not on the current menu (state advanced), DROP or truncate the
carry — or carry only the last resolved `CHOICE:` line, not the full rambly PLAN including its trailing
"Wait..." tail. Owner: `AIPlayerGPT.cpp` prompt assembly. Priority HIGH. Full repro in notes.md (H1).

## FINDING 6 — the two losses are DECK-CONSTRUCTION / matchup (DC1), not obedience failures — plus, in deck21, the Finding-5 self-blast.

- **deck133 (LOSS, flood):** vs a dedicated B/x attrition deck (Inquisition of Kozilek stripped a
  Blast T2; Fatal Push killed Signal Pest; Geralf's Messenger recursion + Bloodghast + a planeswalker).
  The model played the early game fine (equipped Vault Skirge to 4/1 lifelink, attacked to op14), then
  **drew Glimmervoid, Glimmervoid, Island, Cranial Plating across T6-T12 — pure land flood, zero
  creatures** (verified from the hand line each turn). With its one threat removed and no follow-up, it
  got ground out 0/16. No legal better line existed at the end (s25 hand = Cranial Plating only, nothing
  to equip/block). Pure construction/variance.
- **deck21 (LOSS, race):** vs fast red (Raging Goblin haste T1, Scuzzback Scrapper wither, Boartusk
  Liege lord, Lightning Bolt/Volcanic Hammer). Land-heavy draw (3 Darksteel Citadel + Mox, only Steel
  Overseers/Vault Skirge as bodies; its bodies were repeatedly burned off). The **self-blast (Finding
  5)** cost a creature + a Blast early. It later burned its remaining Blast at the opponent's FACE
  (s15, op->16) while BEHIND in the race — dubious (op ended at 16, so the face damage never mattered; a
  defensive Blast on an attacker would have been better), but marginal given the board. It correctly
  held all blockers at s18 (6 life) but still died to the Boartusk-pumped alpha. Loss = land-heavy draw
  + fast matchup + the self-blast; construction-dominant, with one harness-driven blunder.

Both share the standing DC1 precondition (flood / hostile matchup exposure). NOT guide tasks.

## FINDING 7 — mulligan note: ALL SIX opening hands were 1-creature keeps (4 won). The guide's "keep 2+ creatures" rule is universally overridden and the deck wins anyway — do NOT tighten it.

Opening hands (creature count): deck62 Ornithopter(1); deck17 Steel Overseer(1); deck133 Vault
Skirge(1); deck21 Steel Overseer(1); deck44 Signal Pest(1); deck135 Signal Pest(1). Every single hand
had exactly ONE creature + ~4 mana sources + burn, and the deck went 4/2. So the 1-creature keep is
NOT the loss differentiator (it's a universal feature of this corpus's draws), and the mulligan rule's
`keep 2+ creatures` line is being ignored in favor of keeping — correctly, because 0-drops
(Memnite/Ornithopter/Signal Pest) and Cranial Plating/Galvanic reach come off the top. **Tightening the
mulligan rule to ship 1-creature hands would ship 4 winners.** Leave it frozen. (This closes the
tempting-but-wrong hypothesis that the two new losses were seeded by loose mulligans.)

## FINDING 8 — reasoning-tax / ramble rate is elevated but NON-biting: 7 unparsed + 2 defer (~5.6%), all heuristic-answered, none a lethal miss. deck17 is the ramble hotspot (3 unparsed).

Fallbacks: deck62 0; deck17 s1/s5/s30 unparsed (mulligan + 2 develop windows, all ~12-13k-char
spirals, heuristic kept/played fine, game WON adj); deck133 s3 defer + s9 unparsed; deck21 s13
unparsed; deck44 s3 defer + s21 unparsed; deck135 s21/s22 unparsed. Recurrent spiral triggers this
corpus: metalcraft artifact-recount (E3), mana-availability misreads, phase confusion, cast-vs-target,
and the stale plan-carry (Finding 5). None discarded a materially better line than the heuristic; the
only spiral with a downstream cost was deck21's (Finding 5). Big non-fallback rambles also occurred
in the deck62 WIN (s10 protection-blocking 8.5k chars -> correct "no blockers"; s12 metalcraft-recount
12.8k chars -> correct). Layer: MODEL + the Finding-5 harness amplifier. Latency/timeout risk under
the Spark @0.75 cap, but no timeout occurred (0 raw timeouts corpus-wide per brief).

## GUARDRAIL / PROTOCOL VERIFICATION
1. **Name-echo compliance:** every parsed `CHOICE:` carried `(name)`; **0 wrong remaps**. The one
   dangerous mis-selection (deck21 s7 self-target) was a name-echo that CORRECTLY matched an offered
   option string — the fault is the stale plan-carry naming the wrong card, not the echo mechanism.
2. **Fallbacks 7 unparsed + 2 defer (~5.6%):** all benign at the fallback layer (heuristic answered,
   none a lethal miss, all in won/non-critical positions or a game lost for construction reasons).
   No `empty_reply`. The wave-15 engine batch (free-cast-at-zero-mana, stale_echo subset matcher,
   hybrid-pip annotation) had no material effect at this mono-{cost} artifact seat.
3. **Rule #1 (equip-once) obeyed 100%** — 0 `(ALREADY — change NOTHING)` options taken (Finding 3).
4. **E3 artifact-count fold-in — persists, NON-biting.** Surface `Artifacts in play: you N` correct
   in every window; the model still hand-recounts and re-litigates Glimmervoid (deck62 s12/s13) but
   self-corrects; no metalcraft-gated pick flipped on a wrong count.
5. **Blocker over-block (wave-15 Finding 7) — did NOT recur.** Only defensive windows were deck62 s10
   (`no blockers`, correct — took 2 while ahead) and deck21 s18 (`no attackers`, correct hold-back).
   No costly over-block this corpus.
6. **Board-state contradiction witness (wave-15 8.3):** deck21 s13's `[tapped - untaps and can attack
   next turn]` on its own Steel Overseer contributed to the spiral, but co-located with the stale
   plan-carry — folded into Finding 5, not a standalone promotion.

## Adjudicated positions (brief ask: ahead/behind for the right reasons?)
- **deck17 20/14 (WIN adj)** — AHEAD, right: 6/1 lifelink equipped flyer + protection Champion vs a
  slow Faerie board; correct win despite the 3 rambles.
- **deck133 0/16 (LOSS)** — BEHIND, right: one threat removed + drew 4 lands vs an attrition/removal
  deck = no legal damage path. Correct-play flood loss.

## HANDOFF
1. **FREEZE the guide verbatim** (strategy.txt md5 `00849aa9`, byte-identical; **9th freeze**). No
   guide line earned a change; no guardrail earned a demotion. Substance = two shipped-fix VALIDATIONS
   (Galvanic 11/11, attackers 17/17), E2 non-reproduction (validated-by-absence), E6 non-exercise.
2. Route to notes: **H1 NEW/HIGH** — stale plan-carry -> self-blast blunder + unparsed loops
   (Finding 5, clean repro, outcome cost); **E2** dormant (unfixed lever, no exercise); **E6** open
   (no exercise). Route to general-suggestions: the plan-carry harness fix (cross-deck) and the
   reasoning-tax/ramble mitigation. Rotation verdict in notes.md: **HOLD** — the seat surfaced a NEW,
   outcome-costing harness repro this cycle (Finding 5), so the no-new-work-signal condition is not met.
