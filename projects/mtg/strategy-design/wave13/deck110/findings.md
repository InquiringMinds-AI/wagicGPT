# Deck-110 (Etched Affinity) — Wave-13b game review (name-echo era, clean corpus)

**Corpus.** PRIMARY `matchups-20260715-183228/`, 6 deck110 seat translogs (round-robin over
44,109,110,131,133,135,140). Binary `/tmp/wagic-a75e2625e` (Force r2/r3 + auto-tap two-pass
+ c3e6d95b2 name-echo + 42e8bc180 BLOCKS:none + 240s timeout). Wave-11 guide FROZEN, live
`deck110_strategy.txt` md5 `00849aa9` (unchanged since wave-8). Reply protocol =
answer-after-plan with the NEW **name-echo** (`CHOICE: N (name)`). Choices 1-indexed. Card
facts vs `bin/Res/sets/primitives/mtg.txt`; representation facts vs live prompts. Citations =
seat opp + seq (+turn). Spark ran GPU_MEM_UTIL=0.75 (KV-preemption latency → adjudications;
per brief, NOT a play-quality signal).

## Record: 3 WIN / 3 LOSS (win table 110: 3/6)

| seat opp | result | end life (my/op) | T | one-line |
|---|---|---|---|---|
| deck44  (Faerie)    | **WIN**  (adj1) | 18 / 7  | 14 | flooded bodies + chump D, ahead comfortably |
| deck135 (Modern Snow)| **WIN** (adj1) | 10 / 9  | 12 | NAIL-BITER; s17 over-block slowed own clock |
| deck131 (Mind Control)| **WIN**       | 16 / -2 | 9  | Pest/Ornithopter alpha, dominant |
| deck109 (Hellrider) | **LOSS** (adj1) | 4 / 15  | 8  | raced, correctly behind at cap |
| deck133 (Ph. Asphodel)| **LOSS**      | -5 / 26 | 13 | few creatures + s21 Blast blunder (below) |
| deck140 (Wipe Them Out!)| **LOSS** (adj0)| 14 / 18 | 24 | board-wiped repeatedly, DC1 grind |

results.tsv cross-check: `110 133 1 -5 26 13`, `110 109 adj1 4 15 8`, `131 110 1 -2 16 9`,
`135 110 adj1 9 10 12`, `44 110 adj1 7 18 14`, `140 110 adj0 18 14 24` — all consistent.
Win/loss = variance + construction (DC1, user doctrine, context only). This is NOT a pure
freeze wave: a HIGH engine/card-script bug and two model decision-quality failures surfaced.

---

## FINDING 1 — HIGH / NEW / ENGINE (CARD-SCRIPT): Galvanic Blast metalcraft NEVER applies — it deals a FLAT 2 corpus-wide, not 4.

**The deck's core finisher and removal is delivering HALF its intended metalcraft damage in
every game.** Across all 6 deck110 seat game logs, EVERY resolved Galvanic Blast dealt
`Damage: 2` — it never once dealt 4 — despite metalcraft being active (the affinity board
floods past 3 artifacts by T2-3).

**CLEAN REPRO — deck133 s21/s23, T10, my19/op16:** the s21 prompt surface reads
`Artifacts in play: you 4` (2 Darksteel Citadel + 2 Cranial Plating, both `[artifact]`-tagged;
Glimmervoids correctly untagged) → metalcraft ON → Oracle says Blast = 4. The s23 game log
records `- Damage: 2 dealt by Galvanic Blast to Phyrexian Obliterator`. Metalcraft active,
bonus not applied.

**Root cause (card script, data-fixable — no recompile).** `mtg.txt` Galvanic Blast:
```
auto=damage:2
auto=aslongas(artifact|myBattlefield) damage:2 >2
```
The metalcraft +2 line uses the `aslongas(...) EFFECT >N` STATIC-effect construct to try to
deliver a ONE-SHOT extra 2 damage on resolution. Every other `aslongas(...) >2` card in the
file is a CONTINUOUS modification (Ardent Recruit `2/2`, Etched Champion `protection from
red`, Auriok Edgewright `double strike`) where the static form is correct. A one-shot effect
under `aslongas` never resolves. The engine's own metalcraft ONE-SHOT idiom carries the
`oneshot` keyword (Blade-Tribe Berserkers `aslongas(artifact|myBattlefield) 3/3 >2 ueot
oneshot`; Bleak Coven Vampires `aslongas(...) ...life:-4... >2 oneshot`). Galvanic Blast's
line has NO `oneshot` (and no explicit target inheritance for the second damage), so the
conditional +2 is instantiated as a dead continuous effect and no extra damage lands.

**Impact.** OUTCOME-BEARING, cross-deck. The guide's LETHAL CHECK instructs the model to add
"+4 per Galvanic Blast in hand (metalcraft)"; the engine delivers 2. Every lethal-reach
calculation involving a Blast is systematically over-counted by 2×(Blasts). The guide is
Oracle-CORRECT; the bug is entirely in the card and fixing the card restores guide
correctness. **Layer: ENGINE/CARD-SCRIPT. NOT a guide item.** Route → notes ledger (top item),
owner `bin/Res/sets/primitives/mtg.txt`, verify+fix via the wagic-cardscript skill.

**Methodology note:** wave-11 reported these casts as "genuine metalcraft, correct 4 dmg" by
trusting the MODEL's asserted "4 damage" reasoning — the actual `Damage:` game-log events were
never checked. Verifying resolved effects against the game log (not the model's claim) is what
surfaced this. (Skill sharpening — see skill.md.)

---

## FINDING 2 — MODEL (rules gap), outcome-CONTRIBUTING in the deck133 LOSS: Blast fired at a 5/5 Phyrexian Obliterator on three stacked rules errors.

**deck133 s20→s22, T10 Main phase 2, my19/op16, empty board vs a fresh 5/5 trample
Obliterator + Liliana.** Only card in hand = Galvanic Blast.
- **s20 (MP1):** model reasons correctly — "4 damage is not lethal ... Blast better saved for a
  creature that can actually block" → `CHOICE: 2 (Cast nothing)`. CORRECT.
- **s21 (MP2):** reverses — "I must use Galvanic Blast to DESTROY the Phyrexian Obliterator.
  This clears the board" → `CHOICE: 1 (Cast Galvanic Blast)`. A Blast cannot destroy a 5/5
  (even the intended 4 < 5; the real 2 < 5).
- **s22 (target):** "this does not kill the 5/5 ... significantly reduces its toughness to
  1/5 ... removes ... the 'sacrifice permanents' trigger" → targets the Obliterator.

Three errors: (a) **damage does not reduce toughness** — a 5/5 dealt 4 (or 2) is still a 5/5
next turn, marked damage wears off; there is no "1/5". (b) **Phyrexian Obliterator's trigger
is INVERTED** — Wagic's text (s23) is "Whenever a source deals damage to Phyrexian
Obliterator, that source's controller sacrifices that many permanents"; blasting it makes
DECK110 sacrifice, it does not "remove" a downside. (c) firing sub-lethal burn at a 5/5 with
no follow-up. **Confirmed outcome:** s23 log shows the Blast dealt 2 (Finding 1), the
Obliterator lived at full 5/5, and deck110 was FORCED to sacrifice its own permanents
("Choose target 1 of exactly 2 for Sacrifice 1: Island / Darksteel Citadel"). Wasted its only
interaction AND self-nuked its board, in a game it lost -5. **Layer: MODEL rules knowledge**
(representation gave a truncated Obliterator card text in the option list, but even full text
would not fix the toughness misconception). Not guide-fixable (cannot enumerate every card's
toughness); cross-deck model gap → general-suggestions.

---

## FINDING 3 — MODEL / NEW CLASS (blocker seam): over-blocks against explicit prompt guidance; recurring "blocking is mandatory" misconception.

Wave-11's blocker seam was CLEAN only because deck110 was the aggressor in all 6 games with 0
defensive blocks. This corpus the seat was on defense (5 blocker decisions), exposing a new
class.

**deck135 s17, T6 Blockers, my20/op19 (a WIN, but only adj1 10/9):** facing a lone
Icehide Golem (2/2). The prompt annotates the correct line verbatim: *"Unblocked, these
attackers deal up to 2 — you would be at 18 — NOT lethal: block only where the trade favors
you; taking damage while ahead is often correct,"* with the chump option B1 marked *"[deals 0
— this block kills nothing, it only absorbs damage]"* and B2 the equipped attacker. The model
blocked the 2/2 with its **10/2 equipped evasive Ornithopter (B2)** — both die (2 toughness
takes 2) — throwing away a 10-power flying clock to kill a vanilla 2/2 while at 20 life and
under no pressure. Its reasoning: *"I must block the listed attacker."* Blocking is never
mandatory. **Layer: MODEL** (representation was excellent and was ignored — the
representation-beats-instruction wall, now on the block seam). Plausibly outcome-relevant:
deck135 is aggro-vs-control that should be a comfortable win, and it dropped to a 10-vs-9
adjudication nail-biter — the discarded 10/2 clock is a credible cause.

Same "must-block" framing recurs benignly: deck135 s10 (Signal Pest trades with a 2/2 Golem,
even) and deck44 s44 (chump 2 of 3 Faerie rogues to prevent 8, at 20 life — defensible). The
one costly instance is s17. Single-seat this wave → notes/general-suggestions WATCH; elevate
to synthesis as a cross-seat question (do the other aggro seats over-block now that blocker
decisions fire at scale?). NOT a guide edit (prompt already states the rule; adopt bar HIGH;
single-seat cannot add a core line).

---

## FINDING 4 — ENGINE LEDGER E2 (multi-copy equip churn, ledger #4): RECURRED, non-outcome-affecting; milder than wave-11.

**deck131 s29, T9 MP1, a WIN, opp at 5:** with Plating-A on Signal Pest and Plating-B on
Ornithopter (both 6-power attackers), the model MOVED Plating-B off Ornithopter onto Signal
Pest — a NET-ZERO relocation ("move it to Signal Pest makes it a 6/1"). Then s30 it began
weighing moving it BACK ("Case A [on Ornithopter]: total 7. Case B [on Signal Pest]: total 7 —
identical"): the classic A→B→A bounce, INTERRUPTED by the parse guard (s30 = unparsed,
choice -1, heuristic fallback). Both configs are lethal (opp at 5), so 1 wasted `{1}`, no
outcome change. This is the identity-keyed marker's structural blind spot: at s29 the engine
correctly marks Ornithopter ALREADY and leaves Signal Pest unmarked (a "change"), but the
change adds 0 net board power, so the model reads a relocation as a gain. The E2 outcome-delta
lever (mark/suppress a move that does not raise resulting power above the current carrier's)
still catches it. **Non-outcome-affecting this wave.**

**Contrast — deck109 T6 s21/s22/s23 (3 equips, correct):** three DIFFERENT Cranial Platings
onto three DIFFERENT previously-naked carriers (Ornithopter flier first, then Steel Overseer,
then Signal Pest), each ALREADY-marked re-pick explicitly DECLINED ("I will not equip
Ornithopter again as the game explicitly warns this changes nothing"). Legitimate multi-Plating
deployment, marker obeyed — NOT churn. (Also correct: deck135 s14/s15 two-Plating stack,
deck131 s17/s18/s21 lethal alpha spread.) Classify by outcome-delta, not raw same-name count.

---

## FINDING 5 — MODEL WATCH (target sub-prompt disorientation); name-echo index-fallback correctly neutralized it.

**deck131 s6→s7, T1:** s6 the model commits "Cast Galvanic Blast targeting the opponent"
(`CHOICE: 1`). s7 the engine asks the Blast's TARGET (prompt clearly headed *"Choose the
target for Galvanic Blast: 1. The opponent ... 2. Yourself ... 3. Ornithopter ... 4. Signal
Pest"*). The model loses the thread, reasons as if in a fresh main phase ("I will cast Cranial
Plating first ... hold the Blast"), and emits `CHOICE: 1 (Cast Cranial Plating)` — a name-echo
that names NO option in the list. **The name-echo reconcile found no match for the echo and
fell back to the index (index wins by design); index 1 = "The opponent" = the target s6
actually wanted.** Right target, wrong reasoning — a genuine POSITIVE for the index-wins design
(it absorbed a model confusion). FRAGILE: had the intended target been a non-index-1 slot
(e.g. blast a blocker), the index-1 fallback to "opponent" would have MISFIRED. Related to
wave-10 F2 (unlisted-card fixation) and M1. Layer: MODEL (representation was correct). WATCH,
notes. **No wrong echo-remap occurred (brief's HIGH bar not tripped).**

---

## GUARDRAIL / PROTOCOL VERIFICATION

1. **Name-echo compliance & remaps.** 177/177 `CHOICE:` lines carry `(name)` — 100.0% at this
   seat. **Zero wrong remaps.** One echo-vs-option conflict (deck131 s7, Finding 5) resolved
   correctly to index by design.
2. **`ATTACK: <name>` shape (brief ledger candidate) — OBSERVED.** deck135 s9 T3
   `ATTACK: Ornithopter, Signal Pest` (names, not A-indices) → unparsed → heuristic fallback
   (deck135 WON, non-outcome). deck131 s20 T5 `ATTACK: A1, A2, Memnite` — MIXED index+name,
   PARSED (choice 2). Confirms the brief's proposed attackers name→index reconcile arm
   (mirroring parseChoice). ENGINE ledger, not guide.
3. **Defers = 4, all benign.** Every one is seq3 `Glimmervoid` land auto-play (reply_len 0,
   choice -1) — the defer fix firing on the opening land window (deck133/135/44/140). Not
   decisions.
4. **Unparsed rambles = 12** (choice -1, reply_len 11k–16k) — long reasoning correctly
   discarded by the guard, heuristic answered. Spot-checked (deck109 s26, deck44 s17/18/40,
   deck131 s24/30): all in decided/won/grind positions; **none discarded a materially better
   line than the aggressive heuristic fallback.** empty_reply = 0 (Spark healthy this corpus).
5. **BLOCKS: None** — deck109 s24 (T7, at 8→4 vs Hellrider): a DELIBERATE all-decline race
   call, not a misparse (explicit "I should NOT block ... attack for lethal next turn"). The
   race gamble was defensible (17 lethal lined up) but the game was adjudicated behind at 4/15.
6. **Auto-tap color-stranding (brief #2/#5) — NOT OBSERVED at this seat.** deck110 casts
   colored spells (Thoughtcast {U}, Galvanic Blast {R}, attach {B}{B}) but runs Glimmervoid +
   Mox Opal (any-color) as a buffer; every listed cast fired, no cast declined/failed for lack
   of a color, no reasoning from a mana line that then didn't hold.
7. **E3 artifact-count fold-in — persists, NON-biting.** Surface `Artifacts in play: you N` was
   correct in every window; the model still hand-recomputes and folds Glimmervoids in
   (deck133 s21 "2 Citadels, 2 Glimmervoids, 2 Platings? ... recount") but self-corrects to the
   right metalcraft verdict. No metalcraft-gated PICK flipped on a wrong count. (The deck133 s21
   Blast shortfall is Finding 1's engine bug, NOT an E3 miscount — surface N=4 was right.)

## Adjudicated positions (brief ask: ahead for the right reasons?)
- **deck44 18/7** — clean, comfortably ahead. Right.
- **deck135 10/9** — ahead by 1, should have been more; Finding 3 over-block is the credible tax.
- **deck109 4/15** — correctly BEHIND (a race it lost), right adjudication.
- **deck140 14/18** — behind in a board-wipe grind (only 2 attack windows in 24 turns; DC1).

## HANDOFF
1. **FREEZE the guide verbatim** (strategy.txt md5 `00849aa9`, byte-identical; 7th freeze
   wave). No guide line earned a change and no guardrail earned a demotion. The wave's biggest
   item is an ENGINE/CARD-SCRIPT bug the guide cannot and should not encode.
2. Route to notes: Finding 1 Galvanic Blast metalcraft (HIGH card-script fix); Finding 4 E2
   equip churn (unchanged outcome-delta lever); Finding 6 ATTACK-by-name arm; Finding 5 target
   sub-prompt disorientation (watch). Route to general-suggestions: Finding 2 model rules gap
   (damage≠toughness; read the target's toughness before burning), Finding 3 blocker over-block
   (cross-seat synthesis question), the verify-effect-against-game-log methodology.
