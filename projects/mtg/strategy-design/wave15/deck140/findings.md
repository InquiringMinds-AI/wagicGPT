# Deck-140 review — wave 15 (6th clean corpus; per-deck rotation cycle)

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus.

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-040138`, 6 deck140 seat games,
binary `/tmp/wagic-aad32bc7f` (target-cancel use-after-free fix + absent-echo staleness route +
attackers-name reconcile + Galvanic metalcraft fix). Wave-11 guide, **FROZEN byte-identical**
(`cmp` clean, 7364 bytes). Spark @0.75 (med 11.4s / p90 93s). Every deck140 seat decision read
decision-by-decision from the raw JSONL. `choice` 1-based; `choice -1` = heuristic fallback;
`CHOICE: 0` = pass action.

**Record: 5 W / 1 L** (140:5/6 — ties deck110 for best of corpus). From `results.tsv`:

| game | opp | outcome | end life | class |
|---|---|---|---|---|
| vs110 | Etched Affinity | **WIN adj1** 28-18 T20 | swept every creature board; stabilized to 28 w/ Blightsteel in hand |
| vs133 | Phyrexian Asphodel (mono-B) | **WIN adj1** 18-13 T12 | Elixir-recurred BSZ; ahead on board and life |
| vs135 | Modern Snow | **LOSS (death) 18 to -2 T16** | STRUCTURAL: Abominable Treefolk (0/0 snow-grown 8/8 trample) outran BSZ's affordable X; Wrath/Damnation arrived too late |
| vs17 | Faeries | **WIN adj0** 64-17 T34 | dominated: sweeps + Staff clock + Blightsteel deployed; textbook guide adherence |
| vs44 | Faerie Archmage | **WIN adj1** 19-15 T22 | Damnation on a 7-wide reload; Staff clock; held Rakdos correctly all game |
| vs21 | Goblin Gang | **WIN adj0** 31-13 T28 | ground a burn/swarm deck out with Cruel Edict + Staff + Lightmine + Venser's |

Note: the single loss (vs135) is a REAL board death (winner=0, life -2, T16), **not** a latency
adjudication — the other five are life-adjudicated at cap. The loss is not a latency artifact.

Tags: [FREEZE] · [ENGINE] representation/auto-tap -> notes ledger · [STRUCTURAL] deck/threat-mismatch.

## Headline
The **SWEEP TRIGGER validates a 6th corpus** (read correctly at every window; guide FREEZES) and
the **#1 Rakdos-hold is obeyed at every window** (Rakdos was never fired below-lethal, and never a
missed lethal). The two owned open items resolve cleanly this cycle:
1. **Rakdos X auto-underpay (owned): ZERO recurrence — no witness.** Rakdos's Return was never cast
   this corpus (held every game). The three X-menus that DID surface were all Black Sun's Zenith,
   each surfaced correctly as its own `ask` and paid correctly (option 1 = biggest affordable, incl.
   one correct deliberate X=1). The vs109 engine defect is untested here, not reproduced.
2. **Mana-available dual-hiding (owned): CONFIRMED + heavily quantified — 48 display instances across
   5 of 6 games — but ZERO game-affecting misplays this corpus.** The model routed around every one.

No NEW guide-fixable decision class. Guide freezes for the 6th corpus; seat is a rotation candidate.

## OWNED ITEM 1 — Rakdos's Return X auto-underpay: NO RECURRENCE (no witness this corpus)
**Quantified with seqs.** I scanned all 6 seat files for (a) any Rakdos's Return resolution
(`Damage: N dealt by Rakdos's Return` in events) and (b) any Rakdos X-menu (`ask` with `X = N`
options after a Rakdos cast).
- **Rakdos casts this corpus: 0.** Rakdos's Return entered hand only in vs21 (seq5, t5), vs133
  (opener, milled by Thoughtseize at seq4), and vs135 (seq10 draw). In vs21 it was held from t5->t28
  at every window (seq11,13,14,16,17,21,29,30,33,34,37,41,46 all show it offered and declined). The
  guide's fire condition (`untapped lands - 2 >= opp life`) was NEVER satisfied — the closest was vs21
  seq46 (12 sources -> X=10, opp at 13) and seq43 (11 sources -> X=9, opp 14): both below lethal.
  **The #1 Rakdos-hold was therefore obeyed at every legitimate window with zero missed lethals** —
  no under-fire, no over-fire.
- **X-menus that DID surface (all Black Sun's Zenith, all correct):**
  - vs135 seq15: X-menu `[3,2,1,0]`, chose **option 1 = X=3** (max affordable: 5 sources - {b}{b} = 3).
    Correct (biggest affordable; the 8/8 Treefolk needs X=8, unaffordable — no better option).
  - vs135 seq18: X-menu `[4,3,2,1,0]` -> stale_echo fallback -> heuristic picked **X=4** (biggest;
    seq19 events show 4 counters applied). Correct.
  - vs17 seq12: X-menu `[3,2,1,0]`, chose **option 3 = X=1** — DELIBERATE and correct: board was a
    single 1/1 Briarberry Cohort, X=1 kills it, and BSZ shuffles back into library (guide: "X at/above
    the toughness you need dead"). Good judgment, not an underpay.
- **Conclusion:** the X-payment machinery that was exercised (BSZ) surfaced as its own decision and
  behaved correctly (option 1 = biggest, model picked appropriately). The vs109 Rakdos-specific
  auto-underpay (X paid below intended lethal with no menu surfaced) got **no test** — Rakdos was never
  cast. The engine item stays OPEN-but-UNTESTED in the ledger; no new evidence for or against the fix.

## OWNED ITEM 2 — "Mana available" dual-hiding: CONFIRMED, 48 instances, ZERO game impact
**Quantified with seqs.** Detector: for every `ask`/`priority` prompt, compare the colored pips in
the "Mana available: {..}" display line against the colored pips required by each OFFERED "Cast …
{cost}" option. Because the offered-options list is authoritative (the engine only offers castable
spells), any offered cast whose required color is ABSENT from the display line is a dual-hiding
instance — the display defaulted a dual land to one color and hid the other. Count by game:

| game | dual-hiding records | dominant hidden color / source |
|---|---|---|
| vs110 | 11 | RED hidden (Badlands B/R, Plateau R/W) — Pyroclasm/Rakdos look uncastable |
| vs133 | 0 | (mono-black draw; no red spells offered) |
| vs135 | 2 | RED hidden (seq20, seq22) |
| vs17  | 18 | RED hidden (17x); **BLACK hidden once** (seq28/29: line `{w}{w}{w}{w}`, Cruel Edict {1}{b} offered) |
| vs44  | 6  | RED hidden (Badlands/Plateau/Akoum/Clifftop all B/R or R/W) |
| vs21  | 11 | RED hidden (10x); **B+R hidden once** (seq42: line `{w}{w}{w}{w}{w}`, Rakdos {r}{b}{x} offered) |
| **TOTAL** | **48** | mostly RED-hiding of B/R + R/W dual lands |

This is the brief's KNOWN corpus-wide representation defect ("the 'Mana available' line HIDES
dual-land second colors … do not trust it … the offered-options list is authoritative"). Wave-13
had a single witness (vs109 s9 black-hiding). **This corpus expands the count to 48 across 5 of 6
games** — the defect is pervasive whenever the deck holds B/R (Badlands, Akoum Refuge) or R/W
(Plateau, Clifftop Retreat) duals.

**Game impact this corpus: ZERO.** The model routed around every instance:
- On empty boards (C=0) the hidden red spells (Pyroclasm/Rakdos) were correctly held anyway, so the
  display error was moot (vs110 s7-13, vs21 s13-14/s29-30/s33, vs44 s8-9, vs17 s14-19/s33-37).
- On real creature boards a VISIBLE non-red sweeper was always the correct play, and the model used
  it: vs17 s7 (C=6, life 17, attacked -> cast Damnation, not the toughness-limited Pyroclasm),
  vs21 s16 (C=1 Scrapper, life 14 -> Cruel Edict), vs44 s23 (C=7 -> Damnation).
- Critically, at **vs135 seq20** (life 8, C=1) the model CAST Pyroclasm despite the display line
  hiding red — i.e. it treated the offered-options list as authoritative and cast the
  "unaffordable-looking" red spell. This confirms the model does NOT rigidly trust the mana line;
  the wave-13 vs109 wrong-decline was not reproduced.
- The one BLACK-hiding case (vs17 s28/s29) landed on a C=0 board (Cast nothing correct regardless),
  so no decline-misplay.

**Conclusion:** a real, high-frequency engine/representation defect — now well-quantified — that did
NOT convert to a single misplay this corpus. Stays in the engine ledger (unchanged priority); it is
not a guide-fixable class and not new work.

## The single loss (vs135) — STRUCTURAL, not a blunder
deck140 kept a mandated opener (Badlands, Urborg, Elixir, Pyroclasm, Plateau, BSZ, BSZ — 3 lands +
2 sweepers). The opponent resolved **Abominable Treefolk**, a 0/0 that snow-permanents pump to an
8/8 trample and that regrows past -1/-1 counters as more snow lands enter. deck140's only early
answer was Black Sun's Zenith, but its affordable X was capped at 3-4 ({b}{b} + limited black
sources) — below the 8 toughness needed. Wrath/Damnation (which kill any toughness) weren't drawn
in time; Lightmine Field arrived at seq22 (life 3, too late). Every BSZ decision was correct
(option 1 = biggest affordable each time; killed the 2/2 Icehide, dented the Treefolk). This is the
guide's own documented failure mode ("one huge creature: Black Sun's Zenith — but only when the
biggest affordable X >= the toughness needed; else if a Wrath/Damnation is offered, cast that") with
no Wrath/Damnation available. **The loss is threat-mismatch / reach-starvation, consistent with the
wave-9/10/11/13 loss column — a deck-construction gap (a big-toughness snow-grown trampler outruns
affordable-X BSZ), not a pilot error.** User owns; flag only.

## Fallbacks (verified per brief — no wrongful downgrade)
- **stale_echo (4): all CORRECT catches, none wrongful.** vs133 s9 (Elixir/nothing menu, empty board
  -> heuristic nothing), vs135 s18 (BSZ X-menu; echo named the spell not "X=N" -> heuristic X=4 =
  biggest, correct), vs17 s32 (echoed "Play Akoum Refuge" — a land absent from the cast menu -> C=0 ->
  heuristic nothing), vs21 s42 (echo stale, C=0 -> heuristic hold). In each the echo named a card NOT
  in the offered set; the absent-echo staleness route fired correctly. No case named an offered
  option yet got downgraded — no wrongful downgrade to flag.
- **unparsed_reply (~9): all long-latency rambles (150-215s) -> heuristic; none cost a game.** The
  only ones near stakes self-resolved or were already-losing positions (vs135 s22 at life 3). No
  unparsed fallback landed on a mandatory-sweep window (C>=1 + life<=16 + payable sweeper) with a bad
  heuristic outcome.

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. SWEEP TRIGGER + belief-kills: read correctly, 6th corpus. Tapped-still-counts honored (vs21 s16
   Scrapper tapped -> Cruel Edict; vs17 s7 tapped board -> Damnation). "16 or lower" boundary honored
   (vs21 s16 life 14; vs133 windows). FREEZE.
2. Displacement: the residual did NOT move to a new guide seam. It sits at (a) the ENGINE
   (mana-annotation dual-hiding — now 48-count; Rakdos X-underpay untested), (b) DECK construction
   (threat-mismatch vs a snow-grown trampler / reach). No displaced guide leak.
3. #1 Rakdos-hold: obeyed at every window; zero below-lethal fires, zero missed lethals. Guide text
   never violated.
4. BSZ-X / X-menu / Staff face-ping / Lightmine / Venser's / Mulligan / Lands: all obeyed where
   exercised (Staff took "a player" every upkeep except correct 1-toughness kills: vs17 s25/s26 Scion
   1/1, s34 Cohort, s38 Faerie Swarm, s42 correct player-ping over Blightsteel/Wasp; vs21 s18/s19
   Bloodmark Mentor then player). Freeze.
5. Attack floor: Blightsteel deployed as an attacker in vs17 (seq41) — the only game reaching 12
   mana. No BLOCKS: lines (deck runs ~no creatures). Nothing to legislate.

## WATCH (do NOT legislate — single seat / not deck140-owned)
- **Snow P/T + -1/-1 counter display confusion (engine representation, opponent-side).** vs135:
  Abominable Treefolk shows as 4/4->5/5->9/9 (printed 0/0) with a static "[counters: 4x -1/-1]" as snow
  permanents change its base P/T. The model spent whole reasoning blocks (s20, s21) unsure whether it
  was dead. Not a deck140 decision defect and not a deck140 card — route to engine ledger as a
  low-priority representation candidate needing a 2nd witness (a creature whose printed base P/T is
  0/0 and whose live P/T is set by a dynamic characteristic-defining ability alongside counters).
