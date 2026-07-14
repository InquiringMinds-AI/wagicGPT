# deck133 wave-9 — development notes (engine / harness / model / deck-construction)

Corpus: PRIMARY `matchups-20260714-094942`, binary a40671057. Real games: 140(W), 135(W), 131(W),
110(L), 44(L), 109(L). **135v133 livelock FIXED — that game COMPLETED (T11 win) and is INCLUDED.**
Self-contained repros for the project pause.

## ENGINE / REPRESENTATION — inherited items now CLOSED
1. **Gray Merchant bare-N drain display — CONFIRMED on a 3-cast sample (was verified on 1 cast in
   wave-8).** All three GM casts showed `{right now: drains N}` resolving to exactly N (incl. GM's own
   +2 devotion). Repros (game / seq / life delta):
   - vs110 (0x55b901946740) seq28 T9: drains 6 -> my8->14, opp18->12. Cast under the <=10 stabilizer.
   - vs140 (0x564326a78680) seq41 T11: drains 8 -> my18->26, opp22->14.
   - vs135 (0x56090901d770) seq28 T11: drains 10 -> my17->27, opp14->4 (combined-lethal, see #5).
   The "+2" workaround stays DELETED; no regression. No further engine work.

2. **Obliterator broadened punisher-rider — FIRES FOR THE DEFENDER, and opponents respected it (closes
   the wave-8 "no block window" open item).** deck133 attacks with Obliterator; the rider is consumed in
   the OPPONENT seat. Repros:
   - OPP135 (file 1784046413-...deck135-0x560909816f90) blockers seq38 T11: incoming attacker rendered
     `A2. Phyrexian Obliterator (5/5) [trample] {text: Trample -- Whenever a source deals damage to
     Phyrexian Obliterator, that source's controller sacrifices that many permanents.}`; defender chose
     "Boreal Druid blocks Bloodghast" (declined the Obliterator block).
   - OPP131 (file 1784047264-...deck131-0x560201005ff0) blockers seq56 T23: same rider on `A3.`;
     defender chose "Young Pyromancer blocks Geralf's Messenger" (declined the Obliterator block).
   Note for reviewers: verify this from the OPPONENT's file, not deck133's — deck133 had 0 blocker
   windows all corpus.

3. **Liliana no-legal-target loop — STAYS FIXED.** 0 fails-validation in all 6 stderr; 0 defer in the
   JSONL. Liliana of the Veil cast twice (vs131 seq18 T7, seq54 T19), Liliana the Last Hope once
   (vs110 seq19 T7); none looped.

## HARNESS
4. **Priority re-ask multiplier still present** (single-option "Put in Play with <fetch>" re-offers,
   mana unspent — re-offers, not real fetches). Decision-count inflation, not a leak. Noting so a future
   wave doesn't read the count as durdle.

5. **`resolved:` field still not emitted (reviewer-facing).** The `events` field carries the life/zone
   deltas needed to verify resolution (used here for the 3 GM casts), but an explicit
   `resolved: battlefield|countered|fizzled|graveyard` would remove the manual narration-scrape.
   Harness/evidence ledger.

## MODEL (route to model-experiments / representation, NOT guide lines)
6. **Thoughtseize into a HALLUCINATED-EMPTY-HAND (the live piloting leak this wave; escalation now
   warranted -> REPRESENTATION).** vs131 seq58 T19 my7/opp8: prompt showed `Opponent hand size: 0`;
   options were only {Cast Thoughtseize, Cast nothing}; the pilot cast Thoughtseize and its reply
   fabricated "the opponent may have a removal spell or a counterspell this turn" — inventing cards in a
   0-card hand, paying ~2 life for zero in a game won at just my5. Three more late-seizes into <=2-card
   hands: vs131 seq19 (hand 1), vs44 seq21 (hand 2, a loss), vs140 seq46 (hand 2, blowout). The rule is
   present in TWO guide spots and the fact is surfaced -> route to an OPTION-LINE annotation
   (`Cast Thoughtseize {b} - opponent's hand is EMPTY; this only costs you 2 life`), mirror of the
   own-targets annotation. See general-suggestions.md #1. Guide FROZEN; no louder prose.

7. **Gray Merchant combined-lethal recognition — WORKS (positive), latent guide gap self-covered.**
   vs135 seq28: N=10 < opp14 and my17>10 (guide's rule-1 says WAIT), but the pilot cast GM then swung
   Bloodghast+Obliterator (7 power) for exact lethal (drain 10 + 7 = 17 > 14). The guide's win condition
   ignores the attack step; the pilot bridged it correctly. WATCH — do NOT add a combined-lethal clause
   (risks premature small-GM casts the guide warns against as counter-bait). Litmus that the pilot can
   combine drain + beats without being told.

8. **Liliana-the-Last-Hope +1 as removal — not re-tested (no diagnostic window this corpus).** Carried
   from wave-8 as a single-instance watch (the pilot doesn't see the loyalty +1 as removal). No new
   evidence; keep as a dormant watch.

## DECK CONSTRUCTION (upstream decklist flags — RE-CONFIRMED; do NOT fix with guide length)
9. **No early blocker; Bloodghast cannot block.** vs110 (affinity Memnite+Plating to T12) and vs109
   (Hellrider aggro, both attack windows forced-empty, no board assembled) lost as races with no wall
   drawn. 0 blocker windows all corpus. Consider a cheap early blocker.

10. **No reach / no flyer answer except Gray Merchant; GM density 4/60, no selection.** vs44 died to
    unblockable Faerie/Archmage beats, GM never drawn. Consider +1 GM or cheap selection, and/or a
    flyer-defense source.

11. **Discard-suite self-damage = close-race margin.** Thoughtseize 2 + fetch 1s supplied the life
    deck133 died/nearly-died by (vs44 at 0; vs131 won at 5 after the empty-hand seize). A 1-copy
    Thoughtseize -> Inquisition shift would cut self-damage. Construction, not a guide gap.

## WHAT WORKED (freeze — do not churn)
- Impact-ranked act-floor + attack floor: OBEYED in all 3 wins (attacked with everything each turn once
  a board existed). Bare-N GM: all 3 roles exercised (stabilizer vs110, tempo drain vs140, combined-
  lethal finisher vs135), display correct every time. Liliana coexist-casts steady. Guide is a ZERO-edit
  frozen diff this wave and stands alone for the pause.
