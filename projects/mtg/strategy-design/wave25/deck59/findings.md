# deck59 (WB Persistence) — FIRST-GUIDED validation (wave 25, Step-0-ter)

Corpus `matchups-20260725-035605` (binary 62d8783a5, -T 3000, REPPENALTY=1.05). 6
deck59-perspective translogs mined. **Record 6/6 — from 3/6 guideless Step-0.** ALL SIX
games carry a `gameend` in the deck59 log (wave-24 had 3 missing): WIN vs131 (24 v-8, T15),
vs102 (15 v0, T13), **vs109 (13 v-5, T13 — the wave-24 LOSS deck)**, vs133 (13 v-4, T13),
vs27 (4 v0, T23 grind), vs137 (18 v-4, T11). Zero fallbacks at this seat.

**Headline: the initial guide LANDED. Both idiosyncrasy levers fire correctly, the 6/6 is
earned (not a hidden-tax record), and the two wave-24 representation gaps shipped as the b6
FACT and are cited at the decision seam. Guide verdict = BYTE-COPY (validated as-is).
Rotation verdict = ROTATE OUT (converged; only open item is engine-lane).**

---

## Finding 1 — the hybrid/green-mana tax COLLAPSED (rule #1 validated)

Wave-24: 33 giant replies >4k on mana re-derivation, a temporarily-VETOED Safehold/Finks
cast (deck27 s2), and a game-losing color-anxiety MULLIGAN (deck109, kept-able 3-Plains 7
thrown for "no black"). Wave-25:

- **No castable spell was falsely vetoed anywhere in the corpus.** Every green/hybrid
  mention that survived into a reply now SELF-RESOLVES correctly as an aside, not a veto —
  e.g. deck137 s10: "{B/G} means I need a Black OR Green source. I have Swamp" -> cast
  Kitchen Finks correctly. The `no green / need green` regex fired in exactly ONE reply
  (deck137 s10) and it resolved right.
- **No color-anxiety mulligan.** All 6 opening 7s were KEPT (2-3 land hands with castable
  spells) — correct keeps, zero mulligans taken. The wave-24 loss-class mulligan did not
  recur.
- **The mana giants relocated, they did not persist.** Ask-only giants >4k fell 33->8 and
  >6k fell 16->2. The surviving >6k spirals (10 across all decision kinds) are NOT
  mana-derivation — they are tight-game combat math (vs27 grind at myL4), Restless
  Apparition pump-timing (Finding 4), and sweep-timing. Reasoning length on *mana*
  decisions dropped to guide-fact-trusting one-liners; the length that remains is on
  genuinely hard combat/pump judgment in the two long games (vs27 T23, vs133).

Rule #1 ("your mana is simple — every spell casts off Plains+Swamp; if it's listed it's
payable, trust the annotation, don't re-derive") did exactly its job.

## Finding 2 — the persist + symmetric-sweeper ASYMMETRY internalized; the wave-24 loss lever, closed from BOTH directions

**(a) Sweep-when-behind — the exact wave-24 loss spot, now correct and game-winning.**
vs27 s14 (T9, **myL2** oppL16, facing a board of 5 including two 4/3 unblockables = lethal
next turn — the precise "behind, low life, facing lethal" shape deck59 LOST on in wave-24):
it cast **Day of Judgment**, reasoning verbatim the recursion WHY the skill predicted the
go-gate must carry —

  > "my persist creatures (Kitchen Finks, Safehold Elite, Restless Apparition) return to
  > the battlefield... this trades my single card for their entire board, stabilizing me
  > immediately... Casting Day of Judgment is the only move that prevents immediate death."

It ground back from 2 life and WON. It swept twice more when behind/even (s27, s30) and at
s26 (myL4) actively WANTED Damnation but it was not in hand ("survival seems unlikely
without a board wipe"). deck137 s10's PLAN, unprompted: "save Day of Judgment for when the
opponent floods the board or threatens lethal." The go-gate fires on the board-readable
trigger.

**(b) Hold-when-ahead — the wave-24 loss matchup (vs109 Hellrider) is now a dominant win
via the OTHER half of the rule.** deck59 held life flat at **13 from T7 to game end**,
stabilizing EARLY by blocking with persist creatures every combat (s5/s9/s13/s16 — each a
favorable `(you kill it, your blocker lives)` block, see Finding 3), wither-shrinking the
growing Hellrider threats (Rendclaw blocks Stromkirk Noble s16), and correctly NEVER
casting the sweeper it was offered — it was winning the board/race, so the guide's "NEVER
sweep when ahead" held. Opp went 20->19->16->11->4->-5; deck59 took no damage after T7. Won
13 v-5. The loss did not recur because it never reached the losing spot — and where it DID
reach it (vs27 s14), it swept. Rule #2 validated both ways.

## Finding 3 — b6 (persist narration + trade-annotation return clauses) is LIVE and DECISION-EFFECTIVE

The two wave-24 representation gaps (notes.md #1 return-not-narrated, #2 "both die"
ignores persist) shipped as the b6 FACT and both render:

- **Return narration:** `- Your Safehold Elite returns to the battlefield with a -1/-1
  counter (persist)` (deck131 s6; Kitchen Finks s31). Death->return now linked.
- **Trade annotations carry the return clause:** favorable `(you kill it, your blocker
  lives)`; even trade `(both die (yours returns with a -1/-1 counter (persist)))`;
  unfavorable-but-recoverable `(your blocker dies, attacker lives (yours returns with a
  -1/-1 counter (persist)))`.

**Effect on decisions: 11 of 12 block decisions cited the return in their reasoning**, and
the citation drove the block:
- vs109 s13 — "Block the Goblin with Safehold Elite to trade and **trigger its Persist**"
  (chump-trade MADE because the return was visible).
- vs27 s33 (myL4) — chumped a first-striker (Heartmender into Sanguine Guard) reading the
  annotation "your blocker dies, attacker lives (yours returns...)" — a trade it takes only
  because the body comes back.
- vs109 s5/s16, deck131 s5/s9 — favorable persist blocks confirmed via the annotation.

The wave-24 gaps are **CLOSED-BY-FACT** — the guide narrated the recursion value in rules
#2/#3, and the engine now carries it at the combat seam. This is the crutch-lifecycle
matched pair completed at the deck59 seat (annotate-to-teach -> ship the FACT -> the model
reads it at the decision, not just the guide).

## Finding 4 — NEW: Restless Apparition pump-option rendering is ambiguous (engine-ledger, NOT guide)

The pump-activation option renders `3/3 with Restless Apparition [cost:
{w/b}{w/b}{w/b}] {card text: "...gets +3/+3 until end of turn..."}`. The "3/3" prefix
contradicts the "+3/+3" card text and the model cannot reconcile them — it burns thousands
of chars puzzling ("Why does the option say 3/3? ... the card text says +3/+3 ... maybe the
option text is just poorly phrased"). This drives **4 of the corpus's 5 largest priority
spirals** (deck133 s16 6.8k, s19 13k; deck27 s18 7.5k, s21 7.6k). Every pump decision
still resolved CORRECTLY (the deck won all six), so this is a latency/verbosity tax, not a
misplay — but it is the single largest remaining spiral locus at this seat. Route to
notes.md engine/representation ledger; the guide already covers the pump ("your mana-sink
finisher when you have spare mana"), so no guide change.

## Finding 5 — minor items (watch/notes, not levers)

- **Inverse hand-read (deck131 mulligan):** it KEPT an all-Swamp / 0-Plains hand
  (Safehold/Finks need white, uncastable early) while its rationale hallucinated "3 Plains
  and 3 Swamps." The KEEP was correct (Restless castable off Swamps, 3 lands, drew a Plains
  T3) and it won — so this is outcome-neutral and possibly a mild OVER-correction of the
  guide's "don't fear no black" emphasis into "I always have both colors." NOT a guide
  lever: adding a "count your white sources" caution risks re-seeding the exact
  color-anxiety the guide just cured, and the outcome was right. Watch only.
- **First-strike sub-confabulation (vs27 s33):** the reply floated "if Heartmender dies and
  returns before the normal damage step, it can deal damage" — a rules-confused aside
  (a returned persist creature does not deal the prevented combat damage) that did NOT
  drive a wrong choice (the chump-block was correct). Minor; not guide-fixable.

## Finding 6 — rules-shift (batch A): no impact at this seat

deck59 has no planeswalkers (a4 redirect-menu removal — no records), no battles (a5), no
day/night or city's-blessing cards (a6). The only relevant change is a1 (turn-1 upkeep):
deck59's sole upkeep trigger is Heartmender's -1/-1 removal, a no-op on turn 1 with no
creatures. No turn-1 narration anomaly, no expectation of the old redirect menu observed.
Clean.

## Ranked outcome
1. Rule #1 (hybrid mana simplification) — veto GONE, mulligan-anxiety GONE, tax collapsed. **VALIDATED.**
2. Rule #2 (persist + one-sided sweeper) — fired at myL2 with the recursion WHY, held while ahead. **VALIDATED both directions; it was the wave-24 loss lever, now closed.**
3. b6 persist FACT — live, cited at 11/12 blocks, wave-24 gaps CLOSED-BY-FACT.
4. Pump-option rendering ambiguity — new engine-ledger item (largest residual spiral).
