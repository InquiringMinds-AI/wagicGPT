# Deck-110 wave-18 — cross-deck / engine suggestions (layer-routed, not deck110 guide items)

## 1. [NEW, HIGH-LEVERAGE] Block-echo matcher must resolve creature NAMES to B#/A# labels — mirror the working CHOICE name-echo.
**Cross-deck (every deck that ever blocks).** The `blockers` reply matcher currently accepts ONLY the
exact `BLOCKS: B#:A#` label form. When the qwen pilot declares a legal, correctly-shaped block using
creature names — `BLOCKS: Memnite: Boggart Ram-Gang` (deck21 s18) or `BLOCKS: Ornithopter blocks
Dragonmaster Outcast` (deck49 s12) — the whole reply goes `unparsed_reply` and the decision falls to the
heuristic, which can make a DIFFERENT, worse block. At deck21 s18 this was **biting**: the heuristic
chumped a 9/1 Memnite into a 1/1 wither (Memnite died) and let a 3/3 hit face, in a game lost by 2 life
(the wave-17 rematch deck110 had WON clean). The fix is the exact analogue of the `CHOICE:` name-echo
that already works: parse the B#/A# roster the prompt just printed, then resolve any creature name in the
`BLOCKS:` line to its label (B# for blockers, A# for attackers), tolerating `Bname: Aname`, `Bname blocks
Aname`, and comma-separated lists. The wave-17 batch added a "paraphrase catch" for the priority menu /
"BLOCKS: none" but did NOT extend it to name->label block ASSIGNMENTS. Proof it's a pure matcher gap, not
a model defect: `BLOCKS: B1:A1` parsed cleanly at deck49 s19, deck21 s24, deck21 s30. Priority
MEDIUM-HIGH (biting repro). Owner: block-reply parsing in the AIPlayerGPT harness. Repros: deck21 s18
(biting), deck49 s12 (non-biting), contrast deck49 s19 / deck21 s24 / deck21 s30.

## 2. [STANDING] Compute the equip / mana-ability no-op marker from OUTCOME DELTA, and stop surfacing behavior-neutral no-op actions on the GPT priority menu (E2 lever, now subsuming the sole-option ALREADY menus).
Still the correct standing cross-deck representation fix. E2 (equip churn) is validated-by-absence a 4th
wave (33/33 equips value-positive, ZERO ALREADY taken), so it is dormant — but the SAME lever now also
covers a residual E6-adjacent shape the wave-17 fix left behind: `options=1` priority menus whose sole
option is an "(ALREADY attached ... change NOTHING)" equip re-offer (deck21 s16/s17/s29, deck62 s28/s31,
and the `{B}{B}` re-attach variant deck135 s10/s11). The model navigates these cleanly by passing (zero
fallbacks), so it is LOW priority — but a menu whose only action is a documented no-op carries zero
decision value and ideally should not be surfaced at all. General principle (from the E6 close): do not
place behavior-neutral no-op actions on the GPT priority menu, and NEVER as the sole option. Suppress an
"Equip ... targeting X" whenever moving would not raise X's resulting power above its current carrier
(outcome-delta, not target-identity).

## 3. [CONFIRMED WIN, note for the ledger] The E6 hand-card PUT_INTO_PLAY dead-end filter shipped and works.
"Cast Card Normally with Mox Opal {rules text}" — last wave the seat's #1 fallback driver (5/7) — appears
0 times corpus-wide this wave. Confirming a shipped representation fix by ABSENCE of the old label is the
cheap, reliable validation; worth keeping as the default close-out check for any menu-relabel/suppression
fix.

## 4. [METHOD, cross-reviewer] Partition ramble fallbacks by POSITION before attributing them to a decision defect.
All 4 of this seat's long unparsed rambles fell in the 3 LOST games and in desperate (2 life) or confused
(post-combat / phase-ambiguous) positions; the 3 WINS carried at most 1 non-biting fallback. Giant-reply
overthinks track POSITION (losing/confused), not decision quality — the same lesson deck62 carries under
the "computed-P/T distrust" tax pocket. A reviewer should split ramble fallbacks won-vs-lost before
reading them as a regression. (Reviewer instrument only, no behavioral doctrine.)
