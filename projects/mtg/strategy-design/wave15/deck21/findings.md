# deck21 (Goblin Gang) — guideless first-pass findings (wave-15)

Corpus: `matchups-20260716-040138`, binary `/tmp/wagic-aad32bc7f`. deck21 played with
NO strategy guide (core prompt only). Record: **3/6** — WON vs44, WON vs133,
adjudicated-AHEAD vs135 (17 vs 8); adjudicated-BEHIND vs140 (13 vs 31 = loss), LOST vs17
(faeries), LOST vs110 (affinity). All card rules text below is derived from the translog
option/board annotation strings, not from real-MTG memory (the engine's cards diverge — see
notes.md).

## Deck identity (engine reality)

Mono-Mountain goblin **swarm aggro** with anthems + burn. No defensive plan; win by racing
20 fast. Card set (cost / stats / text as the ENGINE prints them):

- **Raging Goblin** `{R}` 1/1 **haste** — Goblin.
- **Spark Elemental** `{R}` 3/1 **trample, haste, [treason]** — the `[treason]` tag = "sacrifice
  at end of turn." A one-shot 3-damage haste body. NOT a Goblin (Elemental).
- **Boggart Ram-Gang** `{R/G}{R/G}{R/G}` 3/3 **haste, wither** — Goblin. Cost = 3 Mountains.
- **Scuzzback Scrapper** `{R/G}` 1/1 **wither** — Goblin (gets Goblin King's mountainwalk).
  **NO haste** (confirmed: named as attacker the turn it was cast, engine dropped it as
  ineligible — vs135 s29). Cost = 1 Mountain.
- **Goblin Gang Leader** `{2}{R}{R}` 2/2 — ETB: create two 1/1 red Goblin tokens. No haste.
- **Bloodmark Mentor** `{1}{R}` 1/1 — your OTHER red creatures have **first strike**. Goblin.
- **Goblin King** `{1}{R}{R}` 2/2 — other **Goblins** get +1/+1 and mountainwalk.
- **Boartusk Liege** `{1}{R/G}{R/G}{R/G}` 3/4 **trample** — other RED and other GREEN creatures
  get +1/+1 (engine shows it WITH trample; real card is a vanilla 4/4 — see notes). Cost = 4 Mtn.
- **Orcish Oriflamme** `{3}{R}` enchantment — your ATTACKING creatures get +1/+0.
- **Lightning Bolt** `{R}` instant — 3 damage any target.
- **Volcanic Hammer** `{1}{R}` sorcery — 3 damage any target.

Goblins (get Goblin King buff): Raging Goblin, Boggart Ram-Gang, Scuzzback Scrapper, Goblin
Gang Leader + tokens, Bloodmark Mentor, Goblin King. NOT goblins: Spark Elemental, Boartusk
Liege.

## What the CORE PROMPT already got RIGHT (do NOT re-teach)

1. **Beatdown identity & attack-every-turn.** Every game opened Mountain->1-drop->swing;
   kept a fast clock; correctly declined to hold back attackers to block (vs135 s17 kept the
   Liege home only when the block was a bad trade). "I am the beatdown/aggressor" framed
   nearly every reply.
2. **Kill lifegain / engine creatures with burn on sight.** Bolted Steel Overseer (vs110 s8),
   the 7/1 lifelink Vault Skirge (vs110 s18), the growing Faerie Bladecrafter (vs44 s10),
   Oona's Gatewarden / Cloud Sprite flyer-blockers (vs17). The "burn the engine/lifegain, not
   the face, when it earns it" instinct is present without a guide.
3. **Mulligan.** Kept every reasonable 3-4-land hand; no misfires.
4. **Hybrid affordability, MOSTLY.** Cast Boggart Ram-Gang / Boartusk Liege / Scuzzback off
   Mountains without complaint in most games — the `{R/G}` = "castable off red" reading
   usually held (one exception, F5 below).
5. **Blocker/attacker legality**, name-echo, `BLOCKS: None`: all clean at this seat. The two
   `stale_echo` fallbacks (vs135 s10, vs133 s14) are CORRECT staleness catches (echo named a
   no-longer-offered "Play Mountain" and a parent-action "Cast Volcanic Hammer" at a target
   sub-menu, respectively) — no wrongful downgrade.

## Where DECK KNOWLEDGE was LOAD-BEARING (the guide must teach exactly this)

### F1 - BURN IS A CLOCK: never hold it, and it only KILLS toughness <= 3. (game-losing)
Two distinct failures, both deck-specific:
- **Held burn as "interaction" and lost the race (vs140, the adjudicated LOSS).** At s16 the
  reply literally says *"My strategy guide explicitly states: 'hold instant-speed interaction
  for the opponent's turn'"* and holds Lightning Bolt with an empty board while a lifegain
  deck (Venser's Journal + Staff of Nin) stabilized. deck21 then durdled to a naked board and
  chucked single burn spells at a face that gained 4/turn — opponent ran to 31. qwen is
  quoting the CORE prompt's generic hold-instants line; for this deck burn is the CLOCK, not
  interaction. This is the single most important teaching point.
- **Pointed burn at creatures it cannot kill.** vs135: cast BOTH Volcanic Hammers at a 6/6
  Abominable Treefolk (thinking 3 damage "reduces it to a 3/3" — it does not; damage doesn't
  shrink toughness and won't kill a 6/6). vs135 s24: a 2,000-word loop ("maybe Flooded Strand
  is a snow land? No..." ~40x) -> unparsed_reply, all over a burn spell that could never kill
  the target. vs17 wasted burn on 1/1 flyer chump-blockers. Rule needed: burn a creature ONLY
  if its toughness <= 3 AND it's worth killing (lifegain / token-maker / evasive racer); else
  FACE.

### F2 - SPARK ELEMENTAL must be cast in MAIN PHASE 1 (pre-combat). (massive tax; near-waste)
vs135 s3-s6: qwen agonized for **2,773 + 1,695 words** over whether to cast Spark Elemental in
Main phase 2, eventually (correctly) reasoning that casting it post-combat wastes it entirely
(`[treason]` sacrifices it at end step before it ever attacks). The core prompt has no Spark
concept, so it re-derives this every time. Teach it flatly: Spark Elemental = a 3-damage haste
trampler that dies end of turn; cast it in Main phase 1 and swing, or don't cast it.

### F3 - HASTE ROSTER + summoning-sick tokens. (huge tax; corrected-in-reasoning)
vs135 s20: qwen planned a lethal alpha strike counting a just-cast Goblin Gang Leader and its
two tokens, spent **1,266 words** unwinding it after realizing they have summoning sickness,
attacked with 1 creature. Only **Raging Goblin, Boggart Ram-Gang, Spark Elemental** have
haste. Everything else (incl. Goblin Gang Leader's tokens) must be deployed a turn AHEAD.
Teaching the roster kills the recompute.

### F4 - FIRST STRIKE changes combat math. (silent value left on table)
With Bloodmark Mentor out, your red attackers have first strike — a first-striking 3/1 kills a
2/2 blocker before taking damage and SURVIVES. qwen repeatedly did "both die / it's a trade"
math without applying first strike when the Mentor was live. The Bloodmark / Boartusk / Goblin
King / Orcish anthem effects must be named so qwen folds them into combat.

### F5 - HYBRID {R/G} affordability (one recurrence). (turn wasted)
vs133 s3: declined Scuzzback Scrapper reasoning *"I lack the necessary green mana... my only
land is a Mountain"* — then self-corrected next turn. `{R/G}` is one mana a Mountain pays;
`{1}{R/G}{R/G}{R/G}` (Boartusk) = 4 Mountains. Same class as deck109's Rakdos-Cackler note;
belongs in this guide because deck21 has four hybrid cards.

### F6 - GO WIDE and RACE THE FACE; you can't break a wall. (both flyer losses)
vs17 and vs135 stalled against evasive/defensive walls (Plumeveil 4/4 flying defenders,
faerie flyers, Ohran-Viper/deathtouch). deck21 has NO removal for a big blocker and no
evasion of its own (mountainwalk is dead vs non-Mountain opponents). qwen kept trying to
"solve" the wall — chipping burn at 4/4s, holding attackers — instead of deploying every turn
and pushing life. The guide must say: you cannot answer a wall; keep flooding goblins, all
burn to the face, win the life race or accept the loss (interacting is a slower loss).

### F7 - DON'T DURDLE: deploy a body every turn. (vs140 loss shape)
Burn alone (~28 total damage in the deck) cannot outrace repeated lifegain. Your engine is
CREATURES first, burn as reach. An empty board = no clock = the control/lifegain deck wins.

## Reasoning-tax note
This seat is the tax epicenter: replies of 800-3,000 words are common, 5 unparsed_reply
fallbacks, several catastrophic single-sentence loops (vs135 s24, vs133 s8, vs140 s35). The
loop-guard (unparsed->heuristic) worked every time — no scavenged casts. Every tax spike
co-located with a deck fact the guide now pre-answers (F1-F5). Latency-regime artifact, not a
play regression, but the guide's second job (after correctness) is to SHORT-CIRCUIT these.
