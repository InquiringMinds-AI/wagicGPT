# Deck 44 (UB Faerie Tempo) — Wave 22 review (ROTATED IN; old guide re-validation)

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260723-173843/`. Binary `1d8ed8194`.
deck44 = **deck1 in every game except vs140** (`my_deck=ai_baka_deck44`). **Record 4W-2L** (brief 44:4/6).

game (results.tsv) -> result:
- `27 44 0` **L** (killed -4/20, T10) . `135 44 adj1` **W-adj** (10/4, T14 cap)
- `62 44 1` **W** (kill 5/-1, T13) . `102 44 adj1` **W-adj** (17/2, T15 cap)
- `109 44 1` **W** (kill 2/-9, T13) . `44 140 adj1` **L-adj** (18/24, T35 cap)

192 model decisions (ask 131, priority 40, attackers 20, blockers 10). Instrumented over all 6 logs:
**answer-first 192/192** (0 non-coded first lines), **0 fallbacks** (every chosen_text maps to an offered
option; brief confirms per-seat=0), **0 self-GFTT**. Win column is context only (doctrine).

---

## VERDICT / DISPOSITION

**Guide: EDIT (small, justified) — 3 diffs, net +2 lines.** This is the wave the rotated-in old guide
was pulled back to earn: two passages were interim CRUTCHES for engine defects the modern engine now
carries, and the mandate is explicit ("retire stale crutches"). Per the current skill's crutch-lifecycle
rung I DEMOTED the self-GFTT crutch (durable engine warning shipped, confirmed FIRED + OBEYED 100%) and
added one clause where the model visibly muddled. Everything else validated CLEAN and is frozen. The 4/6
is honest pilot execution; both losses are construction/matchup, neither a pilot or guide fault.

---

## F1 (HEADLINE — crutch lifecycle) the wave-16 self-GFTT hole is CLOSED; its guide crutch is now DEMOTED

Wave-16's one genuine misplay (vs135 s21: cast GFTT when its only legal target was its own Faerie
Miscreant, destroying it) drove engine ledger #7 (own-target-only removal). The durable fix has since
SHIPPED: the GFTT option, when only friendly targets exist, now renders

    Cast Go for the Throat {1}{b} - the only legal targets are YOUR OWN right now
    (warning: this would harm your own side - only correct if you are deliberately triggering something)
    [this cannot target the spell on the stack - battlefield permanents only]

Confirmed FIRED + OBEYED: the friendly-only GFTT option appeared on **~28 decisions** this corpus
(deck135 s7/s9/s10/s12/s14/s16/s17/s19/s21/s22; deck140 s18/s20/s21/s25/s27/s28/s31/s32/s33/s37/s38/
s39/s41/s42/s43/s47/s48/s49/s51/s52/s53) and the model chose the friendly-only GFTT **0 times**.
Instrumented `self-GFTT = 0`. The wave-16 misplay did NOT recur.

Two bonus closures in the same annotation:
- `[this cannot target the spell on the stack ...]` closes the wave-16 SECONDARY model-watch (the caster
  believed GFTT could answer a spell on the STACK) — the engine now states it on the option line.
- deck140 s27: model read that annotation, ignored the useless friendly-GFTT, and countered Pyroclasm
  (the actual sweeper) — the annotation not only obeyed but steered to the right line.

Per the DEMOTE rung (rule still TRUE — never GFTT your own — but the engine carries it at the decision
point, fired+obeyed -> guide defense is now double-coverage/attention-tax) I demoted the two passages:
(1) the GO-FOR-THE-THROAT walkthrough (5 lines -> 3, keeping the never-self-destroy ANCHOR + shipped
marker STRING), (2) DELETED the redundant SITUATIONS bullet (duplicated the engine-annotated case,
never decided a game). **This is the crux of the rotation re-validation.**

## F2 (counter / response-window discipline) CLEAN — every fire and decline correct

deck44 is the pool's best instant-speed test (2 Counterspell, 2 Arcane Denial, flash Obyra/Scion). Every
response window rendered real options; every call was right:
- **Correct FIRES:** vs140 s11 Arcane Denial -> Lightmine Field (attacker-hosing enchantment, priority #3);
  vs140 s27 Arcane Denial -> Pyroclasm (board sweeper, priority #1, protecting Faerie Miscreant).
- **Correct DECLINES:** vs102 s18 -> Mox Jet (mana rock); vs135 s10 -> Into the North (land search);
  vs140 s6/s8 -> Cruel Edict / Elixir of Immortality (explicit let-resolve list); vs27 s20/s26 ->
  **Lord of Atlantis** at 14 then 7 life. Looks like a must-counter anthem lord, but the model correctly
  read deck27's board was all **Zombies** (not Merfolk) so the Lord buffed NOTHING and was a vanilla 2/2 —
  the decline was the right read, not a missed answer.

**Held-mana discipline (wave-11 finding) still honored** at the outcome level: counters fired on the
opponents' turns (Lightmine Field T9, Pyroclasm T21), so deck44 left {u}{u} untapped rather than tapping
out — the colored-needs-first payment kept mana available every time a real threat appeared. No
color-payment fizzle in any of the 6 games.

## F3 (combat) CLEAN — attacks every turn, reads the modern trade annotations, correct chumps, sharp lethal

- Attack-every-turn across vs135/vs62/vs102/vs109 (chip lines advance every turn; no passive turns ahead).
- Trade annotations used well: vs109 s14 Archmage(5/5) blocks Ash Zealot(2/2 first strike) — a real body
  eats the first-striker; vs102 s15 Gatewarden blocks Leaden Myr(2/2 tramp lifelink) denying lifegain;
  vs109 s17 correctly DECLINED to block Boros Reckoner with a 4/4 (block triggers the damage-redirect) and
  raced instead.
- Chump at <=8 life: vs62 s30 (L8) Surveilling Sprite chumps a Saproling — per guide.
- Lethal recognition: vs109 s21 alpha-struck a pumped board (Scion 3/3 + Archmage 7/7 + 2x Bladecrafter
  6/6 = 22 power) into 13 life for the -9 kill.

## F4 (the two losses) both construction/matchup, neither pilot nor guide fault

- **vs27 (killed T10) = BLACK color-screw.** Guide-legal reactive keep (Faerie Miscreant + 3 Islands +
  Counterspell + 2 Arcane Denial), never drew a Swamp — Bladecrafter/GFTT dead on an all-Islands board
  (model s24: "I have no black mana to cast Faerie Bladecrafter or Go for the Throat"). Lone Faerie
  Miscreant clock while deck27 built a wide Zombie board and raced 20->14->7->dead. Same two-sided
  color-consistency signature flagged waves 5-16 (wave-16 vs62 = the twin).
- **vs140 (L-adj 18/24, T35) = reactive clock too slow vs lifegain control.** deck44 flooded and could not
  race deck140-the-grinder (24 life behind Elixir + Lightmine Field + Pyroclasm). It played the DEFENSIVE
  half correctly (countered Lightmine Field + Pyroclasm, let Elixir/Cruel Edict resolve) but a 1/1-flyer
  clock cannot beat a deck gaining to 24. Flat 20->18 vs stable 24. Legit loss.

## F5 (adjudication who-was-ahead — timeout-economics) all three adj outcomes correct

- vs135 W-adj 10/4: ahead by 6 AND on trajectory (opp dropping every turn). Correct.
- vs102 W-adj 17/2: way ahead, opp at 2 about to die. Correct.
- vs140 L-adj 18/24: behind by 6 and flat the whole game vs a stable 24 — not winning. Correct award.

## F6 (answer-first — rotated-in re-validation) HOLDS 100%; neutralized a decode-loop at this seat

192/192 replies emit the coded line first. The deck27 ahead-position repetition loop appeared HERE: vs27
s8 (trivial "Play Island") drew a **15,804-char** degenerate ramble that loops/re-derives and is cut
mid-sentence — yet `CHOICE: 1 (Play Island)` is line 1 and the parser locked it before the tail
degenerated. Reaffirms the wave-21 finding (answer-first defuses the decode-loop at the parser layer).
Model-side, not guide-fixable -> notes.

## Minor decision-quality signals (LOW; routed to notes, not guide-authored)

- **Sleep-Cursed Faerie untap paid in Upkeep (vs135 s26/s27/s28, T13):** guide says pay {1}{u} only in
  Main Phase 1; model paid thrice in Upkeep to accelerate the wake. Wasteful but marginal (game won on adj;
  mana unneeded). Present-and-disobeyed, not a guide-authoring fix.
- **Cruel Edict muddle (vs140 s6, s21):** model leaned on the guide's "Cruel Edict with no creature does
  NOTHING" line while it DID have a creature, second-guessing ("Wait, I *do* have a creature"). Reached the
  defensible answer (let a 1/1, then Obyra, die rather than Arcane Denial into their 2 cards), but the
  guide only covered the no-creature case -> one clarifying clause added (guide diff F2). The one place a
  guide edit was earned by an observed muddle.
