# Deck-110 (Etched Affinity) — Wave-17 game review (3rd per-deck-rotation cycle; H1 canary seat)

**Corpus.** PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-151559/`, 6 deck110 seat
translogs (`*ai_baka_deck110-0x*.jsonl`), round-robin over 17,135,49,21,35,62 (deck44/deck133
rotated OUT; deck49 Dragons + deck35 Slivers entered GUIDELESS). Binary `/tmp/wagic-7cdcf9c73`
(wave-16 engine batch: (a) PLAN-carry stale-intent caveat; (b) legalCasts normalEntry unpayable-
hardcast fix; (c) own-target-only BAD-effect suppression). Wave-11 guide FROZEN; live
`deck110_strategy.txt` md5 `00849aa9` — byte-identical since wave-8, **10th freeze** if held.
Reply protocol = answer-after-plan + name-echo (`CHOICE: N (name)`). Card facts vs
`bin/Res/sets/primitives/mtg.txt`; representation facts vs live prompts.
Owned charges: **H1 canary** (did the wave-16 stale-plan/self-Blast spiral class die?), **E6**
(promoted Mox Opal "Cast Card Normally" item — characterize the menu + drive the engine fix),
**E2** (equip churn watch), the **3 deferred_to_heuristic** decisions.

## Record: 4 WIN / 2 LOSS (win table 110: 4/6; results.tsv confirmed)

| seat opp | result | end life (my/op) | T | one-line |
|---|---|---|---|---|
| deck21  (Goblin Gang)   | **WIN**       | 14 / -9 | 6  | REMATCH of the wave-16 self-blast loss — this wave Galvanic correctly killed Scuzzback Scrapper (s8/s9), clean |
| deck35  (Slivers)       | **WIN**       | 12 / -2 | 10 | Signal-Pest/Ornithopter + double-Plating go-wide; guideless Sliver opp out-tempo'd |
| deck62  (Enchantresses) | **WIN** (adj) | 18 / 13 | 8  | Memnite 4/1 Plating beatdown; one sequencing ramble (s14), non-biting |
| deck135 (Modern Snow)   | **WIN** (adj) | 6 / 2   | 14 | close race, Galvanic to face at op-8 (s16) closed it |
| deck17  (Faeries)       | **LOSS**      | -8 / 18 | 13 | Control Magic stole the Ornithopter + creature-flood (hand stuck on Mox/Thoughtcast/Master); DC1 |
| deck49  (Dragons)       | **LOSS**      | -4 / 16 | 13 | pure flood — 1-creature keep, drew lands + one Lightning-Bolted 0/2 all game; DC1 |

results.tsv: `110 17 1 -8 18 13`, `110 135 adj0 6 2 14`, `110 49 1 -4 16 13`, `110 21 0 14 -9 6`,
`110 35 0 12 -2 10`, `110 62 adj0 18 13 8` — all consistent. The two losses are BOTH the
matchup deck110 struggles against structurally (Faeries flood/steal, fast burn) — no obedience
failure caused either. This is a **FREEZE wave**: no decision-level class earned a guide change.
Substance = the H1 canary RESULT (positive), the promoted E6 characterization + engine repros,
E2 non-reproduction (validated-by-absence again), and the 3 defers cleared.

---

## FINDING 1 — H1 CANARY / RESULT: the wave-16 stale-plan spiral class (self-Galvanic-Blast + carry-vs-prompt 14k-char loops) DIED. The PLAN-carry caveat fires broadly and the model re-derives correctly ~93% of the time.

Wave-16's headline harness bug (notes H1) had two repro shapes, both driven by the prompt
re-injecting the model's full prior PLAN without reconciling it to the advanced menu:
- **(a) the causal blunder** — deck21 s6→s8: stale plan "cast Steel Overseer" + a changed menu →
  the model emitted the action its own prose REJECTED (Galvanic Blast) and then self-targeted its
  own Steel Overseer, a game-costing 2-for-1 in a LOSS.
- **(b) the contradiction spirals** — deck21 s13 / deck133 s9: the carried plan described a
  different menu/mana-state than the current prompt; the model re-litigated the mismatch into
  ~14k-char `Wait...Wait...` loops → `unparsed_reply`.

The wave-16 engine batch added the caveat that appends, when the plan's named actions are no
longer on the menu: *"(note: the actions your plan names are no longer among the options available
right now - the game state has advanced past that plan; re-derive your choice from the current
board and the options below.)"* — plus (c) own-target-only BAD-effect suppression, the direct
counter to the self-blast.

**Canary result — the class did not recur:**

- **No self-blast anywhere this corpus.** Every Galvanic Blast targeted the OPPONENT: deck17 s9
  (face), deck135 s16 (face @op-8), deck49 s15 (face), **deck21 s9 → Scuzzback Scrapper (opponent's
  wither creature)**. The deck21 game is the REMATCH of the exact wave-16 blunder — and the
  analogous spot is now a clean, REASONED removal: s8 reply "Scuzzback Scrapper has Wither, which is
  dangerous. Removing it is high value. Therefore, I will cast Galvanic Blast targeting the Scuzzback
  Scrapper." → s9 target = Scuzzback Scrapper. deck110 WON this rematch 14/-9 (fb=None both). The
  model never emitted a plan-rejected action.
- **Caveat fires broadly, model re-derives.** The caveat string appears on **59 of the seat's ~181
  decisions** (~33%, consistent with the corpus-wide 319/1021). Of those 59, **55 re-derived to a
  valid on-menu option** and only 4 fell back (~93% success WITH the caveat present). Examples where
  a stale plan was cleanly abandoned: deck17 s6 (plan named a cast now gone → "Cast nothing right
  now"), deck17 s10/s21 ("pass"), deck49 s16/s17 ("pass"), deck62 s24 (re-derived to Thoughtcast).
- **The (b) contradiction-spiral subclass did NOT recur.** This wave has 3 `unparsed_reply` total,
  and NONE is a carry-vs-prompt contradiction loop: deck17 s24 is an E6 Mox-menu puzzle (Finding 2),
  deck17 s30 is a hopeless-position ramble at 3 life, deck62 s14 is a mana-sequencing over-think in
  a WON game. The wave-16 14k-char stale-plan loops are gone.

**Residual (bounded, NOT the old class):** 4 caveat-present decisions still fell back — deck17 s29
(stale_echo), s30 (unparsed), deck49 s34 (stale_echo), deck62 s14 (unparsed). s29 and s34 are
E6-driven (the degenerate single-option Mox menu, Finding 2) — the caveat neither caused nor cured
them; the model echoed a genuinely-off-menu card because the ONLY real option was an unrecognizable
Mox label. s30/s14 are position-ramble / sequencing over-think, not stale-plan. So the caveat is
working as designed; its one visible failure mode is that it cannot rescue a decision whose sole
option is itself unreadable (that is E6's job to fix, not the caveat's).

**Verdict: H1 (wave-16 HIGH) is effectively CLOSED at this seat** — self-blast suppressed,
spiral subclass gone, caveat validated at 93% re-derive. Layer: HARNESS, RESOLVED (canary pass).
No guide item; no new HIGH harness item. Route a downgrade note to notes.md (H1 → resolved/watch).

## FINDING 2 — E6 (OWNED, PROMOTED): the "Cast Card Normally with Mox Opal {Mox's own rules text}" option is a MISLABELED mana-ability activation surfaced as a top-level GPT action; when it is the SOLE option it is the seat's dominant fallback driver (5 of 7 non-defer fallbacks). Engine fix, with repros.

**What the engine was actually showing.** Every failure at s24/s25/s28/s29 (game vs deck17) — plus
deck49 s34 — is a `priority` decision with **options=1**, the single option being:
`Cast Card Normally with Mox Opal {card text: "Metalcraft - {T}: Add one mana of any color.
Activate this ability only if you control three or more artifacts."}`.

This is NOT a card cast. Tracing the game log, the same line appears every time the model picked it
(deck17 s7/s12/s14/s17/s18/s23/s32; deck49 s9/s21/s22/s25/s26/s29/s31/s32) and resolves to a bare
`- You: Cast Card Normally with Mox Opal {...}` with **no card entering, no spell on the stack** —
it is the engine offering to **tap the already-in-play Mox Opal for one mana of any color** as a
standalone priority action (a mana-ability activation with no spell to pay for = a no-op mana
float). The label is doubly wrong:
1. It says **"Cast Card Normally"** (implies casting a card from hand) when it is a **mana-source
   tap**. The Mox was cast on Turn 1 — casting it again is impossible, which is exactly the trap the
   model falls into ("Mox Opal is already on the battlefield ... Casting it again is impossible").
2. It dumps **the Mox's OWN rules text** into the `{card text: ...}` slot — the slot meant to
   describe the card being cast — so the model sees the reminder text of a mana rock where it
   expects a spell name.

**Why it fails only sometimes.** When the model has a real spell to cast, this Mox line is one of
several options and the model ignores it (or picks it harmlessly). The FAILURES are where the Mox
tap is the **only** legal action AND the model's plan names something that is genuinely NOT castable:
- **deck17 s28/s29** — plan "Cast Master of Etherium {2}{U}"; board shows `Mana available: {1} from 1
  untapped source | Already in pool: {w}` → {2}{U} is unpayable, so Master is correctly not offered,
  leaving only the Mox tap. Model can't map "cast Master" onto "the only option is this Mox thing" →
  echoes "Cast Master of Etherium" (off-menu) → **stale_echo** (correct downgrade). s29 even carried
  the PLAN-caveat and still failed — proving the caveat can't rescue an unreadable sole option.
- **deck17 s24** — same shape earlier, model rambles 13.6k chars ("Maybe the option ... is actually
  'Activate Mox Opal' but the game is mislabeling it? ... This is a puzzle.") → **unparsed_reply**.
  Note: the model DIAGNOSES the mislabel correctly and still can't act on it.
- **deck17 s25** — echoes "Cast Thoughtcast" (off-menu) → **stale_echo**.
- **deck49 s34** — at 1 life, sole option is the Mox tap; model echoes "Cast Cranial Plating"
  (already cast at s33) → **stale_echo**.

**Outcome cost this wave: none** (all 5 occurred in an already-lost deck17 line or the dead deck49
endgame — the staleness route caught every one, no lethal miss), but each is a wasted round trip and
this is measurably the seat's #1 fallback source. Same profile the brief promoted E6 on.

**Engine fix (owner `AIPlayerGPT.cpp` option serialization), two levers — the second is stronger:**
1. **Relabel**: a mana-ability activation must not read "Cast Card Normally with Mox Opal {rules
   text}". Serialize it as what it is — e.g. `Tap Mox Opal: add one mana of any color` — and never
   place the source's own reminder text in the `{card text:}` (card-being-cast) slot.
2. **Suppress the no-op** (recommended): a standalone mana-ability activation with **no spell being
   paid for** is a no-op; the prompt already promises "tapped automatically when you cast", so this
   action carries zero decision value. Do not surface bare mana-source taps on the GPT priority menu
   — **especially never as the SOLE option**, which forces the model to either mis-echo or ramble.
   Removing it would eliminate all 5 fallbacks above and is behavior-neutral (choosing it does
   nothing the auto-tap doesn't already do). Cross-deck: any mana rock (Mox/Lotus/etc.) at any seat.

Repros to drive the fix: **deck17 s24 (unparsed "This is a puzzle"), s25 / s28 / s29 (stale_echo),
deck49 s34 (stale_echo)**; harmless-but-noise chosen instances for contrast: deck17 s7/s12/s14/s17/
s18/s23/s32. Layer: ENGINE/REPRESENTATION, OPEN, priority raised MEDIUM→MEDIUM-HIGH (now has
repeated measured cost). NO guide text (guide already says the Mox line "just makes mana ... your
creatures and spells come first" — the model still can't parse it when it's the sole option; this
is a representation bug, not a knowledge gap).

## FINDING 3 — E2 (equip churn): did NOT recur. Rule #1 obeyed 100% — every "(ALREADY — change NOTHING)" re-offer was PASSED, every equip was value-positive. Validated-by-absence, 3rd wave running.

10 equip decisions across 4 games, all clean by OUTCOME DELTA:
- **First-equips of separate Platings** (this pool drew 2 Platings in several games, spread onto
  distinct carriers): deck135 s10 (Plating#1→Memnite) + s25 (Plating#2→Vault Skirge); deck62 s11
  (→Memnite) + s19 (→Steel Overseer); deck35 s7 (→Signal Pest) + s13 (→Ornithopter).
- **Value-positive stacking / spread where an `(ALREADY)` re-offer was ALSO on the menu** — the E2
  test cases — the model took the REAL equip, never the no-op:
  - deck21 s17 (opts=2, 1 `ALREADY` offered): board had Plating `[attached to: Memnite]` (Memnite
    7/1) + a 2nd unattached Plating; model equipped the 2nd Plating (→Memnite, stacks to 14/1), NOT
    the ALREADY re-offer. `chose_ALREADY=False`.
  - deck35 s13 (opts=6, 2 `ALREADY`): equipped the 2nd Plating→Ornithopter (fresh flyer carrier),
    passed both ALREADY re-offers.
  - deck35 s28 (opts=2, 1 `ALREADY`): equipped→Ornithopter (the non-ALREADY option), passed the
    re-offer.

**Zero `(ALREADY — change NOTHING)` options TAKEN corpus-wide.** The identity-keyed net-zero-MOVE
blind spot (E2's unfixed lever) got no exercise because every move raised resulting power. The
outcome-delta lever remains the correct unfixed engine fix, validated-by-absence a 3rd time. Layer:
engine ledger, UNFIXED but non-biting. MEDIUM/dormant.

## FINDING 4 — the 3 deferred_to_heuristic decisions were all BENIGN forced land-drops (correct defers). No LLM call was made and none was needed.

The corpus's only 3 defers are all at this seat: deck17 s3, deck49 s3, deck49 s4. All have
`prompt=""`, `latency=-1`, `choice=-1`, `chosen_text="Glimmervoid"` — the harness deferred a
land-play to the heuristic without an LLM round-trip, and the heuristic played a Glimmervoid land.
Each sits between LLM-answered land-play asks in the same opening turn (e.g. deck49 s2 "Play
Glimmervoid" → s3/s4 defer → s5 "Cast Ornithopter"). Playing a Glimmervoid is never wrong for this
deck (free untapped any-color land + artifact-count fodder), so **all 3 defers were correct** and
cost nothing — they save round-trips on a forced/trivial choice. Layer: harness, working as
intended. No item.

## FINDING 5 — both losses are DECK-CONSTRUCTION / matchup (DC1), decision-clean. No biting misplay; the E6 round-trips were wasted only in already-lost lines (no lethal miss).

- **deck17 (LOSS -8/18):** Faeries. Opponent's **Control Magic stole the Ornithopter** (T8), then a
  flying beatdown (Wasp Lancer grown to 5/4 via counters, Glen Elendra Liege, 2× Oona's Gatewarden,
  Scion of Oona). deck110 **flooded**: after its early creatures traded/died, its hand was stuck on
  Mox Opal + Thoughtcast + Master of Etherium, and Master was repeatedly **unpayable** ({2}{U} vs
  ≤{1}+pool) at the exact turns it mattered (s28/s29). No legal damage path existed by mid-game. The
  E6 confusion (Finding 2) wasted round-trips here but did not change the result.
- **deck49 (LOSS -4/16):** Dragons/fast-red. Textbook **flood**: 1-creature opening keep
  (Ornithopter 0/2), then drew Vault Skirge (T4), Darksteel Citadel (T6), Darksteel Citadel (T8),
  Island (T10), Cranial Plating (T12) — the mid/late hand line was literally "Mox Opal" every turn.
  Ornithopter was **Lightning-Bolted** (s13); the lone Vault Skirge chump/traded into Bloodmark
  Mentor (s23, defensible at 15 life). Slowly raced 20→1. No better line existed.

Both share the standing DC1 precondition (flood / hostile matchup). NOT guide tasks. Consistent with
wave-16's mulligan note: 1-creature keeps are universal for this deck's draws and it still went 4/2
this wave — do NOT tighten the mulligan rule.

## FINDING 6 — standing validations (verification only, no items):
- **Galvanic Blast targeting** — 4 casts, all correctly aimed at the opponent (face ×3, opponent's
  Scuzzback Scrapper ×1). No self-target. The own-target suppression (batch c) did not even need to
  fire on most (opponent targets were legal alongside own ones, e.g. deck135 s15 listed Memnite/you
  AND opponent creatures; the model chose opponent). Metalcraft damage not re-audited this wave (no
  Blast landed in a contested count window that flipped a pick).
- **Attackers name→index reconcile** — all attacker windows resolved to eligible sets; `no
  attackers` / single-attacker holds sane (deck21 held blockers appropriately in the won race).
- **Name-echo** — every parsed `CHOICE:` carried `(name)`; 0 wrong remaps.
- **legalCasts unpayable-hardcast fix (batch b)** — no unpayable hardcast was offered; the deck17
  s28/s29 Master-of-Etherium non-offers (correctly withheld as unpayable) are this fix working.

---

## CHARGE VERDICTS
- **H1 canary (PLAN-caveat)**: **PASS / class died.** No self-blast; contradiction-spiral subclass
  gone; caveat fires on 59 seat decisions, 93% re-derive; the deck21 rematch of the wave-16 blunder
  is clean and WON. Downgrade H1 to resolved-watch.
- **E6 (Mox "Cast Card Normally")**: **characterized + repros delivered.** Mislabeled mana-ability
  activation surfaced as a top-level (often SOLE) GPT action; seat's #1 fallback driver (5/7
  non-defer fallbacks). Two-lever engine fix (relabel; better: suppress the no-op). Non-biting this
  wave but repeated measured cost → priority MEDIUM-HIGH. Repros: deck17 s24/s25/s28/s29, deck49 s34.
- **E2 (equip churn)**: **no recurrence**, Rule #1 100%, ALREADY re-offers all passed, all equips
  value-positive. Validated-by-absence (3rd wave). Dormant lever unchanged.
- **3 defers**: **all correct** — benign forced Glimmervoid land-drops, no LLM needed.

## HANDOFF
1. **FREEZE the guide verbatim** (md5 `00849aa9`, byte-identical; **10th freeze**). No decision-level
   class earned a change. The E6/E2 items are ENGINE-layer and cannot be fixed with guide text
   (the guide already correctly describes the Mox line and equip-once).
2. Route to notes: **H1 → RESOLVED/WATCH** (canary pass, downgrade from HIGH); **E6** OPEN, priority
   MEDIUM-HIGH, full menu characterization + repros; **E2** dormant/unfixed (validated-by-absence).
3. Route to general-suggestions: the E6 no-op-mana-ability suppression is CROSS-DECK (any mana rock,
   any seat) and the highest-leverage remaining representation fix. The PLAN-caveat is a cross-deck
   WIN worth noting as validated.
4. Rotation verdict in notes.md.
