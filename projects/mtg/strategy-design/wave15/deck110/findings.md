# Deck-110 (Etched Affinity) — Wave-15 game review (Galvanic-fix corpus, per-deck-rotation era)

**Corpus.** PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-040138/`, 6 deck110 seat
translogs (`*ai_baka_deck110-0x*.jsonl`), round-robin over 44,110,133,135,140,17,21. Binary
`/tmp/wagic-aad32bc7f`. This corpus is the FIRST with the shipped fixes the brief lists as
wave-14-validated infrastructure: the **Galvanic Blast metalcraft fix** (`3cb901295`), the
**attackers name->index reconcile** + absent-echo staleness route (`d0f88326b`), and the
target-cancel use-after-free crash fix (`aad32bc7f`). Wave-11 guide FROZEN; live
`deck110_strategy.txt` md5 `00849aa9` (byte-identical since wave-8; 8th freeze). Reply protocol =
answer-after-plan + name-echo (`CHOICE: N (name)`). Card facts vs
`bin/Res/sets/primitives/mtg.txt`; representation facts vs live prompts. Spark @ GPU_MEM_UTIL=0.75
(KV-preemption latency -> life-adjudications; per brief, evaluate adjudicated POSITIONS not counts).

## Record: 5 WIN / 1 LOSS (win table 110: 5/6)

| seat opp | result | end life (my/op) | T | one-line |
|---|---|---|---|---|
| deck21  (Goblin Gang)   | **WIN**         | 5 / 0   | 12 | won THE RACE; 2 blocks, both minor over-blocks (non-outcome) |
| deck135 (Modern Snow)   | **WIN**         | 20 / -1 | 8  | Galvanic 4-dmg opener; s20->21 equip A->B->A bounce (2 mana, non-outcome) |
| deck44  (Faerie)        | **WIN**         | 1 / -2  | 11 | flooded bodies, lethal alpha s34; Mox-Opal option confusion x2 (non-outcome) |
| deck17  (Faeries)       | **WIN** (adj)   | 21 / 14 | 13 | slow flier-into-flier grind, ahead comfortably; Etched Champion won it late |
| deck133 (Ph. Asphodel)  | **WIN**         | 18 / -4 | 8  | Galvanic fired at FACE for real 4-reach (wave-13 Obliterator blunder GONE) |
| deck140 (Wipe Them Out!)| **LOSS** (adj)  | 18 / 28 | 20 | Lightmine + wipes + drew 0 Blast -> no reach; correct play, DC1 grind |

results.tsv cross-check: `110 21 0 5 0 12`, `110 135 0 20 -1 8`, `44 110 1 -2 1 11`,
`110 17 adj0 21 14 13`, `110 133 0 18 -4 8`, `110 140 adj1 18 28 20` — all consistent. The single
LOSS is the same DC1 board-wipe/Lightmine matchup deck110 lost in wave-13; the model PLAYED it
correctly (see Finding 5). This is a FREEZE wave: no guide line earned a change; the wave's content
is two SHIPPED-FIX VALIDATIONS plus sub-guide engine/model-layer items.

---

## FINDING 1 — VALIDATION / HIGH: the Galvanic Blast metalcraft fix WORKS — 4 dmg at 3+ artifacts, 2 at <=2. The wave-13 core-wincon bug is CLOSED at this seat.

Wave-13 Finding 1 was a HIGH card-script bug: `auto=aslongas(artifact|myBattlefield) damage:2 >2`
(static form, missing `oneshot`) never resolved, so every Blast dealt a flat 2 even under
metalcraft. The shipped fix (`3cb901295`, wave-14-validated) delivers the metalcraft +2 as a
SECOND resolution instance. Confirmed at this seat by counting `Damage:` events per Blast cast
against the artifact count in the same prompt:

| game | seq | T | artifacts (surface N) | metalcraft | Damage events | total | correct? |
|---|---|---|---|---|---|---|---|
| deck135 | s5  | 1  | **3** | ON  | `2`,`2` (op 20->18->16) | **4** | yes |
| deck44  | —   | —  | >=3  | ON  | `2`,`2` (-> Faerie Miscreant) | **4** | yes |
| deck17  | s15 | 4  | **5** | ON  | `2`,`2` (-> Oona's Gatewarden) | **4** | yes |
| deck133 | s5  | 1  | **2** | OFF | `2` (single)          | **2** | yes (metalcraft off at 2) |
| deck133 | s21 | 6  | **4** | ON  | `2`,`2` (op 10->6)     | **4** | yes |

The fix is CORRECT on BOTH arms: 4 when N>=3, 2 when N<3. (Methodology caution: a naive grep of the
event string dedups the two identical `Damage: 2` lines and mis-reads a metalcraft Blast as "2" —
you must COUNT instances / read the life delta, exactly the "verify RESOLVED effects against the
game log" method-headline the wave-13 deck110 seat added. The corpus-wide `N=2-only` invariant from
wave-13 is GONE.) Layer: ENGINE/CARD-SCRIPT, now RESOLVED — verification only, NO guide item.

## FINDING 2 — VALIDATION / play-improvement: the 4-damage Blast made the LETHAL CHECK / reach math REAL, and it visibly improved deck110's play — most sharply at the exact wave-13 blunder spot.

Wave-13's defining deck110 misplay was deck133 s20->s22: with only a Galvanic Blast in hand vs a
5/5 Phyrexian Obliterator, the model burned the Obliterator on three stacked rules errors
(damage!=toughness, inverted sacrifice trigger, sub-lethal burn), wasted its interaction AND
force-sacrificed its own board, in a game it lost -5. **This corpus, the same deck133 matchup is a
WIN (op -4), and at the structurally-similar s21 (T6, my20/op10, metalcraft ON) the model fired the
Blast at the OPPONENT'S FACE** — "Metalcraft active, Galvanic Blast deals 4 damage ... reduce their
life from 10 to 6" (s21/s22/s23) — then attacked for the close. The reach the guide's LETHAL CHECK
counts ("+4 per Blast with metalcraft") now actually lands, so the model's face-burn reach math
pays off instead of tempting it into a bad creature-burn. No Obliterator/creature-burn blunder
recurred anywhere this corpus. **This is the "did the 4-damage fix improve play/win pattern"
answer: YES** — deck133 flipped loss->win and the wave-13 blunder class did not reappear. (The Blast
also correctly killed evasive blockers at 4: deck44 Faerie Miscreant, deck17 Oona's Gatewarden.)

## FINDING 3 — VALIDATION: the attackers name->index reconcile (`d0f88326b`) works at this seat — NO eligible attacker was dropped across 20 attacker decisions; the wave-13 MIXED-form silent-drop class is CLOSED.

Wave-13 headline-2 flagged the MIXED `ATTACK: A1, <name>` form silently dropping the trailing name
token (under-declaring). This corpus the model emitted the MIXED form repeatedly
(deck140 s9, deck135 s11, deck17 s24/s32, deck133 s17) and the FULL-NAME form once
(deck44 s34 `Ornithopter, Memnite, Signal Pest, Steel Overseer`). Reconstructing each against the
prompt's eligible-attacker roster and the log's `You declared attackers:` events:
- Every MIXED name that was ELIGIBLE unioned in (deck135 s15 `A1,A2`->"Signal Pest, Memnite";
  deck44 s23/s29 spreads all declared in full).
- Every MIXED name ABSENT from the eligible roster was a **summoning-sick creature** the model
  wrongly believed could attack (deck135 s11 Memnite; deck133 s17 Ornithopter [cast+equipped that
  turn]; deck17 s24 Ornithopter, s32 the just-recast Vault Skirge) — the reconcile correctly
  RESPECTED eligibility and excluded it. No eligible attacker was ever dropped.

So the durable fix behaves exactly as designed: union eligible names, exclude ineligible ones. The
wave-13 silent-drop is not reproducible here. Layer: attackers-parser, RESOLVED — verification only.
(But it surfaces a MODEL watch — see Finding 6.)

## FINDING 4 — ENGINE LEDGER E2 (equip churn): a CLEANER, fully-executed A->B->A Plating bounce (deck135 s20->s21), 2 wasted mana, non-outcome. The outcome-delta lever is still the right fix.

**deck135 T6 MP2, my20/op10 (a WIN, op ended -1).** One Cranial Plating, sole equipment. Board had
it on Etched Champion (a 7/2 with protection-from-all — carrier #1).
- **s20:** options `[Equip Signal Pest, Equip Etched Champion (ALREADY)]`. Model MOVES the Plating
  OFF the Champion onto the 0-power Signal Pest ("Champion already equipped ... Signal Pest is
  unattached at 0 power, +5 for next turn"). A genuine DOWNGRADE move — off the best carrier.
- **s21:** options `[Equip Signal Pest (ALREADY), Equip Etched Champion]`. Model catches its own
  error ("the Plating moved from Champion to Signal Pest ... move it back") and MOVES it back to the
  Champion.
- **s22:** correctly PASSES (no further churn).

The full **A->B->A bounce executed** this time (wave-13's bounce was interrupted by the parse guard) —
2 wasted `{1}` in one MP2, net-zero board. Both configs were far from mattering (op at 10, lethal
lined up), so non-outcome. This is the E2 identity-keyed marker's blind spot exactly: at s20 moving
to Signal Pest does NOT raise its resulting power (would-be 5/1) above the current carrier's (7/2),
yet Signal Pest is un-marked (a "change") so the model reads a relocation as a gain. The
**outcome-delta lever** (mark/suppress "Equip ... targeting X" whenever the move would not raise X's
resulting power above its current carrier's) would have suppressed the s20 move and pre-empted the
whole bounce. Unchanged owner: `AIPlayerGPT.cpp` equip-option serialization. NO guide text
(Rule #1 already forbids re-equip; the pilot obeys Rule #1 — see Finding 8 — this is the un-guarded
MOVE shape). MEDIUM.

*Contrast (non-churn, legit multi-Plating spreads, marker obeyed):* deck17 s21/s22 (two Platings
onto two naked fliers), deck44 s23/s29 spreads, deck135 s14 (2nd Plating onto naked Memnite for
lethal). Classify by outcome-delta, not raw same-name count. NOTE also: deck17 spent T6-T10 shuffling
Platings across Vault Skirge/Ornithopter into Faerie flying blockers with op stuck at 19 — slow but
net-legit (the intended two-flier spread; game WON 21/14). Etched Champion (unblockable vs colored
Faeries, carrier #1) was finally equipped at s36 T10 and is what actually closed it — a mild
carrier-priority inefficiency (fliers-into-fliers before the protection creature), not churn.

## FINDING 5 — the single LOSS (deck140) is CORRECT play into a DC1 wall, not obedience failure. Lightmine Field + Cruel Edict handled RULES-CORRECTLY.

deck140 (Wipe Them Out! + Lightmine Field) adjudicated my18/op28 — deck110 behind. The model played
it right: (a) it correctly REFUSED to attack into Lightmine Field across s36-s40 — "Signal Pest has
1 toughness ... 1 damage from Lightmine kills it ... it has no first strike or indestructible, dies
for nothing" — the guide's Lightmine rule obeyed and the damage-vs-toughness reasoning is CORRECT
here; (b) it handled forced Cruel Edict sacrifices correctly (s10 "no creatures to sac"; s28 sacs
its lone Vault Skirge). The loss cause is pure construction/variance: deck110 drew **zero Galvanic
Blast** all game (0 casts), and the Blast is the ONLY reach through Lightmine — so with attacks
suicidal and no burn, it got ground out. Standing DC1 (few-creatures / wipe / Lightmine exposure).
NOT a guide task.

## FINDING 6 — MODEL WATCH / NEW: the model repeatedly names SUMMONING-SICK creatures as attackers and counts them in its lethal/damage math. Harmless here (engine roster + reconcile exclude them), latent risk if it ever gates a lethal/hold-back call.

Across 5 attacker windows (deck140 s9, deck135 s11, deck133 s17, deck17 s24/s32) the model named a
creature it had cast/re-cast THAT turn as an attacker and folded it into its damage total
(e.g. deck133 s17 "attack with both Ornithopter (4/2) and Memnite for 5 damage, opp to 8" — only
Memnite was eligible; opp took 1). The engine's eligible-attacker roster + the name-reconcile
eligibility filter dropped every ineligible name, so no wrong attack occurred and no eligible
attacker was lost. But the model's own lethal/reach arithmetic is inflated by ineligible bodies; in
all instances this corpus the window was non-lethal so nothing rode on it. WATCH: the risk is a
turn where the model believes a summoning-sick creature makes an attack lethal (and e.g. taps out /
declines a Blast / sends a needed blocker). Layer: MODEL (summoning-sickness tracking) — NOT
guide-fixable cleanly (the guide's "ATTACK EVERY TURN with every creature" is fine because the
engine only offers eligible ones; a summoning-sickness caveat would add complexity for zero current
outcome benefit). If it ever bites, the harness-side fix is to annotate freshly-entered creatures in
the board line as `[can't attack yet — summoning sick]`. Cross-seat question for synthesis: do other
aggro seats inflate lethal math with summoning-sick creatures?

## FINDING 7 — MODEL / blocker over-block class: RECURS but only MINOR/defensible this corpus (2 instances, both vs deck21, both non-outcome). Far milder than wave-13's costly deck135 s17.

Only 2 defensive blocks in the whole corpus, both in the vs21 RACE (a WIN):
- **s6 T1, my18/op20:** blocked a 1/1 Raging Goblin with a 1/1 lifelink Vault Skirge. The prompt said
  "up to 1 — NOT lethal ... taking damage while ahead is often correct." Trading an evasive flier
  (Plating carrier) for a vanilla ground 1/1 at 18 is a MILD over-block, and the reasoning carried a
  rules-misread — the model thought its Skirge would survive to "attack next turn" (a 1/1 blocking a
  1/1: BOTH die). Defensible in the race frame (removing a swarm attacker); low stakes.
- **s23 T9, my9/op20:** blocked a 2/1 wither Scuzzback Scrapper with a 12/6 Master of Etherium. Here
  the model's WITHER reasoning was CORRECT ("-1/-1 counters ... Master becomes 10/4, survives") — it
  knowingly took 2 permanent counters off its wincon to remove the opponent's only creature while at
  9 (not under lethal pressure). Mild over-block (guide: take the 2 while your clock is bigger), but
  defensible (clears the last blocker). Non-outcome (won op->0).

The "must remove their creature" instinct persists but did NOT cost a game this wave; no
10-power-clock throwaway like wave-13 s17. Representation was correct in both (the prompt annotated
the right line). Score MODEL/representation-wall, not guide. Keep as the cross-seat synthesis
question (do aggro seats over-block on defense) — deck110's contribution this cycle is
"recurs, low stakes."

## FINDING 8 — VALIDATION: Rule #1 (equip-once) obeyed 100%. NO "ALREADY attached — change NOTHING" option was ever taken across the corpus.

Every equip decision was audited: the model NEVER selected an option tagged
"(ALREADY attached ... change NOTHING)". The one churn shape (Finding 4) is a net-zero MOVE between
carriers, which the ALREADY tag does NOT mark — so Rule #1 is not the leak; the outcome-delta lever
is. The guide's #1 rule is fully effective on its stated shape.

## GUARDRAIL / PROTOCOL VERIFICATION
1. **Name-echo compliance 186/186** `CHOICE:` lines carry `(name)` — 100% at this seat. **Zero wrong
   remaps.**
2. **Fallbacks = 4** (out of 204 decisions): 1 `stale_echo` + 3 `unparsed_reply`; 0 `empty_reply`;
   0 defers. All benign — spot-checked, none discarded a materially better line than the heuristic,
   none a lethal miss.
   - `stale_echo` (deck44 s5): a CORRECT catch, not a wrongful downgrade. The model's FIRST echo
     named the sole offered option, but after a 5.4k-token confusion spiral it emitted a FINAL
     `CHOICE: 1 (Play Glimmervoid)` naming an option NOT in the list -> absent-echo -> heuristic. The
     brief's HIGH bar (an echo that DID name an offered option yet got downgraded) is NOT tripped —
     the reconcile keys on the final echo, which named an absent option. Route: correct.
   - `unparsed_reply` (deck44 s9 non-lethal Blast/Steel-Overseer weigh; deck44 s30 Mox-Opal option
     confusion; deck133 s8 degenerate repetition loop) — long rambles, guard working, heuristic
     answered, all in won/non-critical positions.
3. **Board-state contradiction (wave-13 board-serialization class) — 2nd-seat witness, benign.**
   deck21 s23 serialized the attacking Scuzzback Scrapper as `[tapped - untaps and can attack next
   turn]` while listing it in the Attackers block; the model spent reasoning reconciling
   "tapped but attacking." It did NOT time out (parsed, choice=1, correct block), so it does NOT
   complete the wave-13 timeout-as-terminal-cost promotion (which needs a contradiction co-located
   with `unparsed_reply` on a non-lost decision). Logged as a witness.
4. **E3 artifact-count fold-in — persists, NON-biting.** Surface `Artifacts in play: you N` correct
   in every window; the model still re-derives by hand and re-litigates whether Glimmervoid is an
   artifact (deck44 s11/s18, deck133 s7) but self-corrects to "it's a land, excluded" every time. No
   metalcraft-gated pick flipped on a wrong count.
5. **Auto-tap color-stranding — NOT observed** (Glimmervoid + Mox Opal any-color buffer; every listed
   colored cast — Thoughtcast {U}, Galvanic {R}, attach {B}{B} — fired).

## Adjudicated positions (brief ask: ahead/behind for the right reasons?)
- **deck17 21/14** — AHEAD, right: bigger board + lifelink; slow fliers-into-fliers clock, Etched
  Champion closed it. Correct win.
- **deck140 18/28** — BEHIND, right: Lightmine + wipes + 0 Blast drawn = no legal path to damage;
  correct play into a DC1 wall.

## HANDOFF
1. **FREEZE the guide verbatim** (strategy.txt md5 `00849aa9`, byte-identical; 8th freeze). No guide
   line earned a change; no guardrail earned a demotion. The wave's substance is two shipped-fix
   VALIDATIONS (Galvanic 4-dmg, attackers reconcile), both PASS, both improved play.
2. Route to notes: E2 equip churn (Finding 4, cleaner fully-executed bounce — outcome-delta lever
   unchanged); Mox-Opal "Cast Card Normally with Mox Opal" option mislabel (representation, 2
   witnesses); board-state tapped-but-attacking contradiction (Finding 8.3, witness). Route to
   general-suggestions: summoning-sickness-in-lethal-math model watch (Finding 6), blocker
   over-block cross-seat question (Finding 7). Rotation verdict in notes.md: **HOLD (do not rotate
   this cycle).**
