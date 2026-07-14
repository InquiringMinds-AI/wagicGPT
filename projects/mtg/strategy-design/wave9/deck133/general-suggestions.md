# deck133 wave-9 — core-prompt / representation / engine proposals (layer-routed)

The burden of proof is ON core: every deck pays for each line. **deck133 produced NO core-prompt
proposal this wave.** The corpus was clean (0 desyncs, 0 fails-validation, 0 defer, no no-op decided a
game), the guide is frozen, and the one live leak routes to REPRESENTATION, not the prompt body. Items
below are ENGINE/REPRESENTATION and evidence-tooling, plus the standing construction flags. Because of
the pause, each is self-contained.

## 1. [REPRESENTATION -> ENGINE — NEW, the wave-8-anticipated escalation now has its recurrence] Annotate the Thoughtseize option when the opponent's hand is EMPTY.
- **Repro:** deck133 vs131 (game 0x56020080c660) seq58, T19, my7/opp8. Prompt showed `Opponent hand
  size: 0`. Options were exactly {`Cast Thoughtseize {b}`, `Cast nothing right now`}. The pilot cast
  Thoughtseize and its reply fabricated a target ("the opponent may have a removal spell or a
  counterspell this turn"), paying ~2 life for zero effect in a game it went on to win at just my5
  after the opponent rebounded to 19. Three more late-seizes into <=2-card hands corpus-wide
  (vs131 seq19 hand=1; vs44 seq21 hand=2 in a loss; vs140 seq46 hand=2 in a blowout).
- **Why representation, not guide prose:** the prohibition is present in TWO guide locations
  (impact-list #5 and DECIDING SITUATIONS) and `Opponent hand size: 0` is surfaced — yet the pilot
  fabricates cards over it. This is the read-then-disobey wall in its hallucinated-empty-hand shape;
  the skill forbids escalating wording at that wall.
- **Proposed fix (mirror of the shipped own-targets annotation):** when Thoughtseize (or any
  "target player reveals hand, you choose a card to discard" spell) is offered and the target's hand is
  empty of discardable cards, annotate the OPTION LINE:
  `Cast Thoughtseize {b} - opponent's hand is EMPTY; this only costs you 2 life`.
  This is the same option-line-annotation family as `Cast Fatal Push {b} - the only legal targets are
  YOUR OWN right now`. Generic across seats (any hand-attack discard spell), not deck133-specific.
- Guide FROZEN pending this; no louder prose. If it recurs after the annotation ships, it is a genuine
  world-model defect for model-experiments, not a representation gap.

## 2. [EVIDENCE-TOOLING — reviewer-facing, still open, carried from wave-7/8] Surface the RESOLUTION outcome of a consumed cast in the translog.
A reviewer still scrapes the next record's `events` narration to tell whether a cast RESOLVED vs was
countered/fizzled. The `events` field DID carry the GM life/zone deltas this wave (how the 3 bare-N GM
casts were verified: `Opponent's life -N / Your life +N`), but an explicit per-record
`resolved: battlefield|countered|fizzled|graveyard` would remove the manual scrape and prevent
win-mechanism mis-attribution. Harness/evidence ledger — not player-facing, not the prompt.

## 3. [NON-ITEM — explicitly NOT core] Gray Merchant combined-lethal is a WATCH at the guide layer, not core.
vs135 seq28 showed the pilot correctly firing GM off-condition (N=10 < opp14) because drain 10 + a
7-power swing = combined lethal. The guide's win condition ("N >= opponent's life") ignores the attack
step, yet the pilot bridged it. Recorded as a WATCH (notes.md #1); NOT a core edit and NOT a guide edit
(a combined-lethal clause risks premature small-GM casts the guide already warns against). Route: none.

## Standing DECK-CONSTRUCTION flags (RE-CONFIRMED from wave-7/8; user owns the decklist)
All three losses re-confirm the structural residuals — set as expectations, do NOT fix with guide length:
- **No early blocker; Bloodghast (the T2 body) CANNOT block** -> vs110 (affinity Memnite+Plating to T12)
  and vs109 (Hellrider aggro, both attack windows forced-empty, no board) were races lost with no wall
  drawn early. 0 blocker windows for deck133 all corpus. Consider a cheap early blocker.
- **No reach / no flyer answer except Gray Merchant** (density 4/60, no selection) -> vs44 died to
  unblockable Faerie/Archmage beats with GM never drawn. Consider +1 GM or cheap card selection, and/or
  a flyer-defense source.
- **Discard-suite self-damage (Thoughtseize 2 + fetch 1s)** is the -1/-0 margin in the close races
  (vs44 died at 0; vs131 won at 5 after a self-damaging empty-hand Thoughtseize). A 1-copy Thoughtseize
  -> Inquisition shift would cut self-damage.
