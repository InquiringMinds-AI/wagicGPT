# Deck 44 (UB Faerie Tempo) — Wave 7 review

Corpus: `matchups-20260713-222831/`, deck44 = **deck0 in all 6 games**.
**Record 5W-1L — THE TOP DECK OF THE POOL.** Waves 4->7: 3/6 -> 2/6 -> 3/6 -> **5/6**.
- WON vs109 (mono-red aggro), vs131 (izzet spells), vs140 (WU control), vs110 (affinity), vs135 (snow/simic)
- LOST vs133 (mono-black devotion) only.

Epoch->game map (verified by gameend life/turn vs results.tsv — the jsonl start-epoch order does NOT
match the game order, so I mapped by outcome, not filename):
`e9713`=vs131 W (15/-7 T14) . `e9716`=vs109 W (10/-9 T12) . `e0884`=vs140 W (19/-9 T12) .
`e2523`=**vs133 L** (0/13 T13) . `e5194`=vs135 W (18/-10 T18) . `e5681`=vs110 W (20/-3 T14).
Citations: `eEEEE sN` = last-4 of jsonl start-epoch + seq.

Severity: **P0** = game-losing observed; **P1** = large play-quality cost; **P2** = smaller.
Posture: **FROZEN-SPINE / turnaround mode.** A 5/6 top deck gets a tiny diff, not a rewrite. The
wave-6 additions all landed; the loss yields exactly ONE model-fixable lever pair. Everything the
corpus shows obeyed is frozen verbatim.

---

## WHY IT WORKS NOW — the frozen spine (protect this)

The deck's engine is **cast-a-faerie-every-turn + attack-in-the-air**, and the thing that turned it
into the top deck is the **counter seam protecting that clock**. Every win was decided by the pilot
holding {U}{U} and countering the opponent's one haymaker that would have erased the board:

| game | the countered haymaker | band | why it won |
|---|---|---|---|
| vs131 W | Cyclonic Rift (`e9713 s14`) | #1 sweeper (bounce-all) | saved the flyer board, raced to -7 |
| vs140 W | Black Sun's Zenith (`e0884 s11`) | #1 sweeper (X-wrath) | saved the board, won 19/-9 |
| vs110 W | Master of Etherium (`s29`) + Etched Champion (`s46`) | #2 un-GFTT-able artifact finishers | both countered, raced to -3 |
| vs135 W | Glacial Revelation x2 (`s8, s54`) | big card-draw engine | denied refuel, ground them to -10 |
| vs109 W | Searing Spear (`s9`) | #4 defensive vs aggro | + L12-13 race-defense blocks, won 10/-9 |

Three facts make the spine load-bearing and must be **frozen**:
1. **Hold-{U}{U} discipline is obeyed AND non-displacing.** Every counter window that arose was
   payable (the pilot reached the opponent's turn with {U}{U} open), and it still deployed a board:
   creature-casts per WIN were 4-6 alongside the counters and 4-9 attacker-decisions. The wave-5/6
   "does hold-mana starve development?" worry is answered NO for the fourth wave running.
2. **Sweeper-first counter priority is exactly obeyed** — every sweeper and every un-GFTT-able artifact
   finisher was countered on sight; nothing cheaper was countered ahead of them.
3. **The counter-value floor (wave-6) held** — ZERO cheap-cantrip counters this corpus (wave-6's
   Arcum's Astrolabe waste did not recur). See the one borderline (`vs133 s13`) below; it is a
   plan/choice mis-pick, not a floor failure.

---

## WAVE-6 ADDITIONS AUDIT — overshoot / inert / obeyed

### 1. GFTT no-target fallback — WORKED, and the defect MIGRATED exactly as the ledger predicted
Wave-6's headline defect (`Cast Go for the Throat` offered with no legal target -> 9 validation-drops,
chosen!=happened) is **GONE for deck44: 0 `fails validation` in all six deck44 stderr.** The guide
stopgap (name enchantments/artifact-creatures + the no-target behavioral fallback) held: GFTT was
still cast when it had a real target (`vs133 s6`, `vs135 s46`) and was not fired into targetless boards.

The engine-class defect it stopgapped is deck-agnostic and **migrated to other targeted spells in the
opponents' seats**, precisely as wave-6's notes.md predicted:
- **Liliana of the Veil — 4 `fails validation`** (opponents' decisions)
- **Fatal Push — 2 `fails validation`** (opponents' decisions)
These are not deck44 decisions (they are the mono-black opponents), but they confirm the root cause is
engine-side and per-spell, so the guide stopgap can never be a general fix. Re-filed to the engine
ledger with the exact prediction closed. (See notes.md.)

### 2. Counter-value floor — held (no overshoot, no inert)
No premium counter was spent on a mana rock or 1-mana cantrip this corpus. The wave-6 per-deck line
("never spend a counter on a 1-2 mana cantrip or mana rock; Arcane Denial draws THEM two cards") is
obeyed. Freeze it.

### 3. Blocking floor — obeyed at high life, but has TWO gaps (one is the loss)
The wave-6 per-seam floor ("at 9+ life your flyers do NOT block; only Oona's Gatewarden blocks") fired
correctly where it mattered most:
- `vs133 s9` (**L20**): declined to block Phyrexian Obliterator — PLAN cites the sacrifice punisher.
  This is the floor working, and it is the SAME block the pilot later broke (see P0).
- `vs135 s27` (L20): only Oona's Gatewarden blocks Icehide Golem (the designated wither blocker). Clean.

But it is **under-fired, never over-fired** (no overshoot):
- **`vs110 s10/s17` (both L20, won game) — reflexive high-life blocks of win-con flyers.** `s10` Faerie
  Bladecrafter (a growing win-con) blocks Ornithopter (1/2); `s17` Surveilling Sprite blocks Signal
  Pest (**0/1 — deals 0 damage**, so the block gains nothing per core line 29). Both kill the attacker
  and survive/trade, so harm was near-zero and the game was won at 20 life — but the pilot is halting
  Bladecrafter's growth and tapping its clock to block a 0-power creature. P2, low-priority; tighten
  the high-life line to forbid blocking "even to kill a small or 0-power attacker."
- **`vs133 s18` (L11) — the loss. See P0.**

---

## P0 (the vs133 loss) — Obyra blocks Phyrexian Obliterator under life pressure

`vs133 s18` (T11, **L11**): the pilot blocked Phyrexian Obliterator with Obyra (grown to 3/3 by a
counter). Obliterator's `auto=@damaged(this) ... sacrifice that many permanents` (verified in mtg.txt)
means the 3 damage Obyra deals it forces **3 sacrifices** — the pilot then sac'd Scion of Oona +
2 Islands (`s19-s21`), losing its whole board and its lands in one block. The game was over 2 turns later.

The tell: **the pilot DECLINED this exact block correctly at `s9` (L20)** citing the sacrifice, then
broke the rule at `s18` (L11) under life pressure. The PLAN at s18 even wrestles with trample and
talks itself into it ("if I don't block, I take 5... does trample carry over? Yes..."). This is an
**INTRA-GUIDE CONTRADICTION** (skill Step-0): the Obliterator prohibition currently lives as a
sub-note buried INSIDE the "when your life is 8 or less, chump the biggest attacker" paragraph. At
L11 the pilot is in chump-mode and Obliterator IS the biggest attacker, so the loud chump rule
**vetoes** the buried prohibition — concreteness beats prominence.

**Fix (guide, this wave):** promote "NEVER block Phyrexian Obliterator, at ANY life" to a standalone
absolute at the TOP of the blocking block with its positive redirect (take the trample damage), and
name it as the explicit EXCEPTION inside the chump rule and the Gatewarden rule (even wither damage
triggers the sacrifice). Not a louder copy — a REPOSITION so it stops colliding with the chump rule.

## P1 (the vs133 loss, the deeper lever) — GFTT wasted on Bloodghast, none left for Obliterator

Go for the Throat is deck44's **only clean answer to Phyrexian Obliterator** — Obliterator is a
`type=Creature` (nonartifact), so `target=creature[-artifact]` CAN destroy it. The pilot spent GFTT on
**Bloodghast** at `s6` (T4), which `autograveyard=_LANDFALL_may moveTo(mybattlefield)` returns from the
graveyard on every landfall — killing it only delays it one turn. By the time Obliterator arrived
(T7), the answer was gone.

This is the highest-value 133 lever: had GFTT been held for Obliterator, the block at s18 never
happens and the 5/5 trampler that ground the pilot out is dead. **Fix (guide):** against a black
opponent, hold GFTT for Phyrexian Obliterator; do not spend it on Bloodghast (it recurs). This also
completes wave-6's "counter on Bloodghast is WEAK — recurs" observation, extended to removal.

## P2 — PLAN/CHOICE MISMATCH resurfaced once, in the loss

`vs133 s13`: the PLAN reasons correctly — "Let Inquisition of Kozilek resolve; it is a low-impact
discard spell... countering it would waste a valuable counter needed for sweepers or Liliana's -2" —
then the reply is `2` = "Cast Arcane Denial ... Inquisition of Kozilek". The pilot countered a 1-mana
discard AGAINST ITS OWN PLAN, handing the opponent 2 cards (Arcane Denial's `draw:2` to the countered
spell's controller). Head-int `2` == choice `2` (NOT a desync). Wave-6 declared plan/choice mismatch
"gone"; it is back exactly ONCE, on a counter option, in the loss. The labeled counter option is
attractive enough that the pilot mis-picked it over "Cast nothing." Single instance, low frequency —
route to representation-watch (option ordering / a stronger "your plan says let-resolve -> pick the
NON-counter number" binding), not a guide rewrite. Contributed card disadvantage to a grindy loss.

---

## Harness-defect sweep (per skill, run first)
- **DESYNC (parser mis-grab): 0** across all 6 games — head-int of every reply equals `choice`. (Task's
  "0 desyncs expected" — confirmed.)
- **ENGINE NO-OP (chosen-many/happened-never): none.** No cast repeated with mana unspent.
- **VALIDATION-DROP (chosen recorded, heuristic played): 0 for deck44.** The wave-6 GFTT defect is
  gone; migrated to opponents' Liliana/Fatal Push (engine ledger, not deck44 play).
- **NUMERIC-OFFSET / X-menu: N/A** — deck44 has no X-spells.
- **PLAN/CHOICE MISMATCH:** 1 (`vs133 s13`, above).
- **Fallbacks: 1 total** (`vs109 s15`, `unparsed_reply` — degenerate model babble
  "**demonstrate thedemonstrate...", heuristic answered, non-decisive). Latency median ~7.2-9.7s/game,
  isolated 40-48s spikes; no deck44 timeouts (all 6 games resolved in 12-18 turns). Re-attach markers
  are a deck110 concern — N/A here.

## RESIDUAL (construction, not guide) — persists from waves 5-6
vs133 is a genuinely hard matchup, and the loss was threat-starved: only **2 creature-casts** the whole
game (wins had 4-6). The deck runs 2 Counterspell + 2 Arcane Denial + 2 Go for the Throat on a
16 Island / 10 Swamp base against black-costed spells. The counters now clearly EARN their slots (they
won four games), so "trim the counters" is the wrong lever; the sharper one is the **manabase** (raise
black sources vs the black spell load: Obyra {u}{b}, Bladecrafter {2}{b}, GFTT {1}{b}) and/or 1-2 more
cheap flyers over the 3rd/4th reactive card. Route to the user; not guide-fixable. (See notes.md.)

## Wave-6 status ledger
**FIXED / now winning:** counter seam (sweeper-first obeyed, 0 hallucinations, four wins protected);
GFTT no-target fallback (0 validation-drops, down from 9); counter-value floor (no cheap counters);
development (4-6 creatures/win); mulligan defender-count (5 keeps, 1 mull that won).
**THIS WAVE'S TINY DIFF:** Phyrexian Obliterator block prohibition repositioned to standalone
absolute (P0); GFTT-hold-for-Obliterator / don't-GFTT-Bloodghast (P1).
**WATCH (no edit):** high-life reflexive block of win-con flyers (P2, low-harm); plan/choice
mismatch on counter options (P2, 1 instance).
**RESIDUAL (construction):** 6 reactive cards + 16/10 manabase skew.

## One-line handoff
The top deck's spine is counter-seam-protects-the-flyer-clock (hold {U}{U}, sweeper-first, cast a
faerie every turn) and it is FROZEN; the only loss (vs133) came from one buried rule losing a collision
— the pilot blocked Phyrexian Obliterator under life pressure and fed its sac punisher after wasting
its only answer (GFTT) on recursive Bloodghast — so this wave's whole diff is repositioning the
Obliterator prohibition to a standalone absolute and teaching GFTT to wait for Obliterator.
