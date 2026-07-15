# deck133 wave-10 — development notes (engine / harness / model / deck-construction)

Corpus: PRIMARY `matchups-20260715-031825`, binary c8c054be8, answer-after-plan protocol, max_tokens
2048. Real games: 110(W), 131(W adj1), 135(W adj1), 140(W), 44(L), 109(L). Layer-routing enforced:
engine/harness items live HERE with seq repros, never in prompt text. Self-contained repros.

## HARNESS / REPLY-PROTOCOL

1. **Self-Fatal-Push-at-own-creature via the number/parse layer — TWO distinct mechanisms, both leaking
   PAST a correct plan (the own-targets annotation was obeyed in reasoning; the recorded number was
   not).** Both in WON games this corpus (non-deciding), but a real class. Route: reply-protocol/parser.
   - **(a) Unoffered-desired-card numeric mismatch.** deck140 (file `1784107845-...deck133-...-vs-...deck140`)
     seq19 T7 my19/opp17. Option list literally `1. Cast Fatal Push {b} - the only legal targets are YOUR
     OWN right now` / `2. Cast nothing right now`. PLAN: *"Fatal Push useless... dangerous if it targets
     my own creatures. I will cast Phyrexian Obliterator"* (Obliterator NOT offered), then `CHOICE: 1` ->
     self-Fatal-Push; seq20 target-picked own Bloodghast; seq21 events `Your Fatal Push: hand->stack /
     Your Bloodghast: battlefield->graveyard / Your Fatal Push: stack->graveyard` (Bloodghast returned via
     landfall same turn). The model mapped its fallback digit onto the annotated-bad option instead of the
     safe pass. Candidate engine fix: when the recorded choice lands on an option flagged self-targeting/
     own-only (the annotation string is already present) AND the plan text names a card NOT in the option
     list, prefer the passive "Cast nothing" option over the harmful cast (or re-ask). Do NOT fix in
     prompt text — the plan already reasoned correctly.
   - **(b) Formatless + truncated reply int-grabbed to a harmful option.** deck110 (file
     `1784112983-...deck133-...-vs-...deck110`) seq20 T9 my17/opp5. Reply DROPPED the `PLAN:`/`CHOICE:`
     format entirely, rambled ~7000 chars of mana confusion to the 2048-token cap, concluded in prose
     *"the correct choice is 4 / Cast nothing"* repeatedly, cut before an answer line. Parser int-grabbed
     `2` = Cast Fatal Push (no leading `PLAN:`, so the truncation-FAIL guard did not fire); seq21 target
     = own Geralf's Messenger (4/3, +1/+1 counter), seq22 events `Your Geralf's Messenger:
     battlefield->graveyard` (undying did NOT return it — it had a counter): a self-2-for-1 that erased 3
     of 5 lethal-next-turn power. deck133 won at T13 anyway (CB drain + Gray Merchant). **Engine fix:** the
     truncation-FAIL heuristic-safe path keys on a reply LEADING with `PLAN:`; extend it to fire whenever a
     reply lacks BOTH a `PLAN:` prefix AND a valid trailing answer label — a formatless truncated reply
     should NOT be int-grabbed from prose into a cast. (This also guards other seats; cross-cutting.)

2. **Attacker-label name-vs-A# parse miss (single instance).** deck135 seq32 attackers T11: reply emitted
   `ATTACK: Geralf's Messenger, Bloodghast` (card NAMES) rather than `A#` indices -> flagged
   `unparsed_reply` -> heuristic fallback (which declared attackers; intent matched, game won). 17/20
   attacker decisions used correct `A#` format. Minor: the attackers parser wants `A#` tokens; a
   name-based `ATTACK:` line falls to heuristic. Note for a future wave — not a leak here.

3. **`resolved:` field still not emitted (reviewer-facing, carried from wave-9 notes #5).** The `events`
   field carries the life/zone deltas needed to verify resolution (used here for the 2 GM casts and the
   self-Fatal-Push resolutions), but an explicit `resolved: battlefield|countered|fizzled|graveyard` would
   remove the manual narration-scrape. Harness/evidence ledger.

## ENGINE / REPRESENTATION — inherited items CONFIRMED CLOSED

4. **Thoughtseize empty-hand annotation SHIPPED and OBEYED — closes the wave-9 fabrication class.** The
   option renders `... - the opponent's hand is EMPTY: nothing to strip` (and `- the opponent holds only
   N card(s)` for thin hands). At every empty-hand window with a real choice the pilot declined
   Thoughtseize (deck109 seq16/18/20; deck110 seq13/15/19/22/25/27; the seq29 fb chose CB not TS). Zero
   empty-hand seizes, zero fabricated-card replies. The wave-9-proposed representation fix (general-
   suggestions #1) landed. No further engine work.

5. **Gray Merchant bare-N — CONFIRMED again (2 casts, shown==resolved).** deck44 seq32 drains6 ->
   +6/-6; deck110 seq35 drains6 -> +6/-6 (lethal win). The "+2" workaround stays DELETED; no regression.

6. **Fetch re-ask multiplier tamed.** 20 fetch-crack choices / 6 games (~3.3/game); the wave-9 single-
   option "Put in Play" re-offer inflation did not recur as decision-count noise — consistent with the
   consume-on-choose engine change. No further work.

7. **Liliana no-legal-target loop — STAYS FIXED.** 0 fails-validation, 0 defer, 0 loops across all 6
   stderrs; Liliana of the Veil + Liliana the Last Hope both cast and activated (vs131), activations vs135.

## MODEL (route to model-experiments / representation, NOT guide lines)

8. **Liliana-the-Last-Hope +1 as removal — not re-tested (no diagnostic window; dormant watch carried).**
   vs131 seq33/34: the +1 (-2/-1 to target creature) had NO opponent creature target; the pilot correctly
   DECLINED. No window that tests whether the pilot sees the +1 as removal when a creature IS present.
   Keep as a dormant watch (unchanged from wave-9 notes #8).

## DECK CONSTRUCTION (upstream decklist flags — RE-CONFIRMED; do NOT fix with guide length)

9. **No reach / no flyer answer except Gray Merchant.** vs44 (LOSS): died to unblockable Faerie beats
   (Surveilling Sprite / Scion of Oona / Faerie Miscreant); GM never drawn; the pilot ground Yawgmoth
   -1/-1 counters killing Faeries but bled its own life to death while opp sat static at 12 with no clock
   from deck133. GM density 4/60, no selection. Consider +1 GM / cheap selection / a flyer-defense source.

10. **No early blocker; Bloodghast cannot block.** vs109 (LOSS): Hellrider aggro T8 blowout, threat-light
    draw, no board assembled, 0 blocker windows. Consider a cheap early blocker. Same as wave-9 notes #9.

11. **Discard-suite + fetch self-damage = close-race margin.** deck44 died at 0 after Thoughtseize (seq16,
    2 life) + 2 fetch cracks + Yawgmoth activations. A 1-copy Thoughtseize -> Inquisition shift would cut
    self-damage. Construction, not a guide gap. Carried from wave-9 notes #11.

## WHAT WORKED (freeze — do not churn)
Impact-ranked act-floor + attack floor OBEYED in all 4 wins. Bare-N GM correct both casts (stabilizer
vs44/vs110-panic, lethal vs110). Liliana coexist-casts steady. Empty-hand Thoughtseize discipline now
enforced by the shipped annotation (the wave-9 live leak is CLOSED). Guide is a ZERO-edit frozen diff.
