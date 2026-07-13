# Deck 44 (UB Faerie Tempo) — Wave 4 review

Corpus: both wave-4 self-play runs, deck44 seat (deck44 was **deck0 in all 12 games**).
- Run A `matchups-20260712-192832/`: 4W–2L (won vs 131, 140, 133, 135; lost vs 110, 109)
- Run B `matchups-20260712-230636/`: 3W–3L (won vs 135, 133, 140; lost vs 109, 131, 110)
- Combined **7–5, zero timeouts.** Citations are `eEPOCH` (jsonl start-epoch) + `sN` seq + turn.

Severity: **P0** = game-losing, observed; **P1** = large play-quality/eval cost; **P2** = smaller.

The single most important pattern: **all 5 losses are to aggressive decks** (109 = mono-red
aggro, 110 = artifact affinity, 131 = izzet spells/burn). Deck44 beats every midrange/slower
opponent but loses the aggro *races* — and the one lever that would flip those races, its
counterspells, is structurally dead (F1). The losses are a compound of a real bad matchup (F2)
made unwinnable by a broken interaction seam (F1).

---

## Top findings (fix these first)

### F1 (P0, engine-seam + representation) — Counterspell fired 0/12 games: the prompt never tells the model a spell is on the stack, so it passes live counters

`Counterspell` (hard counter, 2 copies) was cast **zero times in all 12 games**. `Arcane Denial`
fired only in the two vs-109 games. Deck44 **died holding both Counterspells** in the games it
most needed them.

Root cause is a representation gap, verified directly:
- The `--- CURRENT SITUATION ---` block **never lists the stack** (0 occurrences of "stack" in
  that block), and `system_prompt.txt` has **0** occurrences of "on the stack". The counter
  option renders as a bare `Cast Counterspell {u}{u}` with **no target and no signal that
  anything is counterable**. The model can only infer a pending spell from the raw GAME-LOG tail,
  and usually infers wrong.
- **e2540 (vs131, LOSS) s8, turn 6:** GAME-LOG tail is `Opponent's Guttersnipe: hand -> stack`
  (live, unresolved). Deck44 has `{u}{u}{u}` + two Counterspells. Model chose "Cast nothing",
  PLAN: *"The opponent has no spells on the stack to counter."* It repeated this for **Young
  Pyromancer, Guttersnipe, and Cyclonic Rift** (s5, s8, s11, s15) — every key threat resolved
  uncontested. Lost **-3 to opp 23** (opp gained life off Prism Ring/Elixir), a blowout it could
  have disrupted at any of four windows.
- **e2880 (vs110, LOSS) s28/s30, turn 9:** by the time deck44 gets a priority window,
  `Master of Etherium: hand -> stack` is already followed by `stack -> battlefield` (resolved).
  The model's "nothing to counter" reasoning is now *correct* — the window arrives **after
  resolution**. Master of Etherium (0/0 that became 9/5 → 11/6) then dealt 9 and won. Deck44 died
  at 8 with both Counterspells + both Arcane Denials in hand, opponent at 7.
- Contrast — it *can* work when the model happens to parse the tail: **e0062 (vs109) s12** the
  model reasoned "Searing Spear is on the stack… I should counter it" and fired Arcane Denial.
  The seam is not always dead; it is **unreliable and un-cued**.

Two sub-defects, both need fixing:
1. **Representation (guide/prompt layer, cheaper):** surface the stack as a first-class line in
   CURRENT SITUATION (e.g. `On the stack (you may respond): Opponent's Guttersnipe {1}{r}{r}`)
   and label the counter option (`Cast Counterspell — counter Guttersnipe`). The deciding fact
   currently lives only in the log tail; move it to the highest-attention surface.
2. **Engine timing (harder):** in the vs110 case the window physically arrives post-resolution.
   Deck44 must receive priority **while the opponent's spell is still on the stack**.

**Re-frames wave-3 F8.** Wave 3 blamed guide *wording* (hoard-y language) for "died with counters
in hand". The deeper cause is this seam: the model largely *cannot* fire counters because it is
never told there is something to counter. Guide wording is secondary here.

### F2 (deck weakness / matchup, not a bug) — deck44 loses the aggro race; 6 interaction cards clog a beatdown deck

Every loss is a race lost by tempo, not by combat misplay:
- **e0062 & e7730 (both vs109 mono-red):** deck44 dealt **< 10 total damage across 13 turns**
  (opp 20→11 and 20→12) while ground 20→-1. It attacked correctly every turn but with only two
  1/1 flyers (Faerie Miscreant + Surveilling Sprite) because it **drew lands + counters instead
  of bodies.** Its live interaction (Go for the Throat ×2, Arcane Denial ×2) was used.
- **e2540 (vs131):** deck44 cast **only 4 Faerie Miscreants all game** — color-screwed on
  Islands (opening: Island×3, Miscreant, Counterspell×2, Arcane Denial), never found black mana
  for Bladecrafter/Obyra. A lone 1/1 chipping while Guttersnipe burned it out.

The deck runs **6 reactive cards** (2 Counterspell + 2 Arcane Denial + 2 Go for the Throat) in a
deck whose guide correctly says "you are the BEATDOWN, get them dead before turn 10." Given F1 the
4 counters are near-dead weight that dilutes the "cast a creature every turn" plan and produces
the trap hands in the losses. **Deck-list note (for whoever owns the list):** the Counterspells
are the weakest cards as currently integrated; consider trimming toward more cheap flyers. The
guide can also state that a counter-heavy, one-threat hand is a *ship* vs unknown decks.

### F3 (P2, persisting from wave-3) — Sleep-Cursed Faerie untap waste, now DOUBLE-paying in one turn

Guide rule: pay the `{1}{u}` untap ONLY in main phase 1 when it will attack THIS combat.
- **e2880 (vs110, LOSS) turn 7 (s28 game-log):** deck44 paid `{1}{u}` to untap in **Upkeep** *and
  again* in **Main phase 1** — **4 mana burned in one turn** in a race it lost with opp at 7. The
  upkeep untap is pure waste (can't attack from upkeep; last stun counter came off naturally that
  same untap step).
- Untap-count by game: e2880 = 2, e...1233 (140W) = 3, e5283 (110L run A) = 2. Others 0.

The misplay recurs and worsened (double-pay). The negative phrasing ("NEVER pay… ONLY pay…") is
partly followed then violated in the upkeep window — consistent with "negative framing bleeds".
Recommend a **positive, window-scoped** rule: "Only ever untap Sleep-Cursed Faerie once, in your
Main Phase 1, and only if it will attack this turn. Ignore every untap prompt in Upkeep or on the
opponent's turn."

---

## Wave-3 status ledger

**FIXED:**
- **Archmage of Echoes now hits the board reliably** (wave-3 headline: "never cast in six
  games"). Deployed in e...3390 (140W), e2519 (131W), e6692 (135W). "Cast a creature every turn"
  is largely obeyed: only **2 missed creature-casts in 12 games**, both in already-won games.
- **Timeouts eliminated:** 0/12 vs wave-3's 13/21. Games close. Wave-3 F6 economics resolved.
- **Blockers seam (wave-3 F2 "nearly silent", P1):** now fires 1–3×/game and is used *well* —
  Oona blocks Ash Zealot (e7730 s10), Obyra blocks Legion Loyalist (e0062 s6), Miscreant
  chump-blocks Legion Loyalist (e0062 s19).
- **Empty-reply fallbacks (wave-3 F3):** down to **1 in 12 games** (e0062 s17, choice=-1).
- **`chosen_text` now populated** in the translog (wave-3 F5 partial) — sped this review.

**PERSISTING:**
- **Counter cards die in hand** (wave-3 F8) — mechanism now understood as F1 (seam), not wording.
- **Sleep-Cursed untap waste** (wave-3 mistake list) — see F3; worse now (double-pay).
- **`{right now: life -2}` on Faerie Bladecrafter still reads as a cost** (wave-3 F4). e2519
  (131W) turn 20 chose "Cast nothing" with Bladecrafter the only castable creature and that
  annotation present — game already won, low harm, but trap is live. Relabel to
  `{if it dies: opponent -2}`.
- **Hybrid-mana refusal** (wave-3 F4) — mostly fixed; **1 residual**: e5283 (110L) turn 0 skipped
  an offered `Cast Oona's Gatewarden {u/b}` then cast it later. Down from four straight refusals.

**REGRESSED:** none observed.

**NOT EXERCISED / UNKNOWN:**
- **Mulligan seam:** deck44 chose "Keep this hand" in **all 12 games (0 mulligans).** Cannot
  confirm the seam works or that the model would ever ship. One marginal keep (e2540's
  three-Island + one-1/1 + three-counter hand) went badly (F2). Worth a probe.

---

## Supporting detail / smaller notes

- **Offense execution is sound where bodies exist.** In wins the model attacks every turn, does
  explicit lethal arithmetic, casts Scion of Oona to pump, deploys Shadow Puppeteers (e...0752,
  135W). Go for the Throat fires promptly on the right targets (e0062 s8, e7730 s8/s19 — killing
  aggro creatures early per guide). The **removal half of the interaction package works well;
  only the counter half is broken.**
- **The losses are not combat misplays.** Deck44 blocks and attacks correctly and spends the
  interaction it can fire. The win/loss delta is (a) matchup speed and (b) draw quality (threats
  vs lands+counters) — amplified by F1.
- **Representation nit (wave-3 F4):** `[counters:]` empty bracket still renders on Sleep-Cursed
  Faerie once stun counters reach 0 (e2880 s28/s30). Cosmetic, wastes tokens, looks like a glitch.

## One-line handoff for the guide reviser
The guide is doing its main job (deploy creatures, cast Archmage, attack every turn — wins vs
midrange prove it). The wins are locked; the **losses are all aggro races lost because the deck's
counterspells never fire (F1, mostly a prompt/engine seam, not guide text)** and because 6
reactive cards clog a beatdown deck (F2, a list issue). Guide-layer levers available now: (1) a
positive, window-scoped Sleep-Cursed untap rule (F3); (2) explicit "a counter-only, one-threat
hand is a SHIP" mulligan guidance; (3) relabel the Bladecrafter `-2` annotation. The
highest-value fix (F1) lives below the guide — flag it to whoever owns the prompt/engine.
