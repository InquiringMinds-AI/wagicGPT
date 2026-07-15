# deck133 wave-11 — development notes (engine / harness / model / deck-construction)

Corpus: PRIMARY `matchups-20260715-072254`, binary `/tmp/wagic-54a5a9d01` (wave-10 batch + 4096 cap + parse
ramble guards + stack-target annotation fix + fetch pre-targeting keying), answer-after-plan protocol. Real
games: 110(W), 131(W), 135(W), 109(W), 140(W), 44(L) — 5W/1L, all wins clean kills. Layer-routing enforced:
engine/harness/model items live HERE with seq repros, never in prompt text. Self-contained repros.

## MODEL / REPRESENTATION (route to model-experiments / representation, NOT guide lines)

1. **Self-Fatal-Push via a STACK-TARGETING rules misconception (NEW class this wave; 2 instances, both WON,
   non-deciding).** Distinct from wave-10's number/parse leak: here the REASONING disobeys the own-targets
   annotation by fabricating that Fatal Push can destroy an opponent creature SPELL still on the stack.
   - **vs110** (file `1784120188-ai_baka_deck133-0x563838458030-vs-ai_baka_deck110.jsonl`) seq25 T10
     my9/opp18. Options: `1. Cast Fatal Push {b} - the only legal targets are YOUR OWN right now` / `2. Cast
     nothing`. Events preceding: `Opponent's Steel Overseer: hand -> stack`. PLAN: *"The Steel Overseer is
     on the stack. It has mana value 2. Therefore, Fatal Push can destroy it immediately... I will cast Fatal
     Push targeting the Steel Overseer."* `CHOICE: 1`. seq26 events: `Your Fatal Push: hand -> stack / Your
     Bloodghast: battlefield -> graveyard / Your Fatal Push: stack -> graveyard / Opponent's Steel Overseer:
     stack -> battlefield` (Overseer resolved untouched; Bloodghast returned via landfall next turn).
   - **vs135** (file `1784120578-ai_baka_deck133-0x56549aedc860-vs-ai_baka_deck135.jsonl`) seq45 T14
     my14/opp11. Same option shape. Events preceding: `Opponent's Ice-Fang Coatl: hand -> stack`. PLAN:
     *"I will cast Fatal Push targeting the opponent's Ice-Fang Coatl to destroy it."* `CHOICE: 1`. seq46
     events: `Your Fatal Push: hand -> stack / Your Bloodghast: battlefield -> graveyard / ... / Opponent's
     Ice-Fang Coatl: stack -> battlefield`.
   - Obey-ratio context: across 46 own-target Fatal Push windows the annotation was obeyed 39/41 real-choice
     windows (~95%; 5 fallbacks excluded) — same rate as wave-10 (27/29). The annotation works; this is a
     residual MODEL belief (stack-vs-battlefield; instant-destroy vs counterspell).
   - **Candidate representation fix (engine ledger, low priority):** on a removal option whose only legal
     targets are the pilot's own permanents WHILE an opponent creature-spell is on the stack, name WHY the
     on-stack creature is excluded — e.g. append `(the opponent's <name> is a SPELL on the stack — Fatal
     Push destroys a creature on the battlefield, it cannot target a spell)`. Do NOT fix in prompt text; the
     guide already forbids the behavior and the annotation is present-and-obeyed-95% (representation-beats-
     instruction). Alternatively (more aggressive) the engine could suppress a removal option that can only
     hit the pilot's own creatures during an opponent's creature-cast, since it is never correct — but that
     is heavier filtering; the annotation refinement is the cheaper first move.

2. **The wave-10 number/parse self-Fatal-Push mechanism did NOT recur — the ramble guard + range check
   closed it.** (a) The formatless-truncation int-grab (wave-10 deck110 seq20) is gone: all 11 empty_reply
   fallbacks this wave are `replen=0` 120s timeouts falling straight to the heuristic with choice=-1 — no
   prose scan. (b) The unoffered-desired-card case now emits an out-of-range index that is rejected rather
   than mapped onto an in-list harmful option: vs44 (file `1784119227-ai_baka_deck133-...-vs-...deck44`)
   seq29, options were 2 (`Thoughtseize / Cast nothing`), reply named Geralf's Messenger and emitted
   `CHOICE: 3` -> `unparsed_reply`, choice=-1, heuristic. Verified: wave-10 notes #1 (a) and (b) are both
   closed at this seat.

## HARNESS / REPLY-PROTOCOL

3. **empty_reply timeout class (11 records; carried, known corpus-wide).** All `latency ~= 120004ms`,
   `replen=0` — HTTP 120s timeouts on tail generations. Cluster on Fatal-Push-own-only / fetch / long-grind
   windows (vs135 seq32/37/42, vs140 seq28/37/54/71, vs109 seq30/31, vs131 seq38). Heuristic fallback played
   a SAFE line at every one (verified: no self-Fatal-Push resolved at any empty_reply window). The harness
   fix (WAGIC_GPT_TIMEOUT default 240s, commit 95cf9f5f9) ships next corpus. Do not re-diagnose the mechanism.

4. **Attacker labels clean this wave.** No name-vs-`A#` attacker parse miss recurred at this seat (the lone
   attacker fallback, vs140 seq37, was an empty_reply timeout, not a name-format parse). The wave-10 note #2
   single instance did not recur. Carry as resolved-for-now.

5. **`resolved:` field still not emitted (reviewer-facing, carried from wave-9/10 notes).** The `events`
   field carries the life/zone deltas used here to verify GM resolution (x3), the self-Fatal-Push resolutions
   (x2), and Liliana activations. An explicit `resolved: battlefield|countered|fizzled|graveyard` would
   remove the manual narration-scrape. Harness/evidence ledger, low priority.

## ENGINE / REPRESENTATION — inherited items CONFIRMED CLOSED / STEADY

6. **Thoughtseize empty/thin-hand annotation — STEADY (100% obeyed, second wave).** 48 windows; 0 casts into
   EMPTY hands (vs110 x10, vs140 x9) and 0 into THIN hands (vs44 x2, vs110 x3, vs140 x15). All real casts on
   full early-turn hands (vs131 seq12/17, vs110 seq3, vs135 seq3, vs140 seq13). No further engine work.

7. **Gray Merchant bare-N — CONFIRMED again (3 casts, shown==resolved).** vs110 seq39 drains8 -> +8/-8
   (stabilizer); vs135 seq50 drains7 -> +7/-7 (lethal win); vs140 seq32 drains4 -> +4/-4 (no-creature build
   cast, defensible). The "+2" workaround stays DELETED; no regression.

8. **Liliana no-legal-target loop / coexist — STAYS FIXED.** 0 fails-validation, 0 defer, 0 loops across all
   6 stderrs. Liliana of the Veil cast+activated (vs131 seq28/34 + activations, vs110 seq8/9, vs109 seq41);
   Liliana the Last Hope cast+activated (vs131 seq50/51), coexisting with a live Veil the same game.

9. **Fetch pre-targeting keying — no re-ask class.** Fetches cracked when a land was needed; the 2
   fetch-window fallbacks (vs44 seq20, vs140 seq71) were empty_reply timeouts, not re-ask inflation.
   Consistent with the consume-on-choose / pre-targeting engine changes. No further work.

## DECK CONSTRUCTION (upstream decklist flags — RE-CONFIRMED; do NOT fix with guide length)

10. **No reach / no flyer answer except Gray Merchant.** vs44 (the only LOSS): opp44 (Faeries) sat STATIC at
    20 life the entire game; deck133 dealt ZERO damage and bled 20 -> -7 (T12) from Faerie beats + its own
    fetch/discard/Yawgmoth self-damage. GM never online against a static 20; GM density 4/60, no selection.
    Same as wave-9 #9 / wave-10 #9. Consider +1 GM / cheap selection / a flyer-defense source.

11. **No early blocker; Bloodghast cannot block.** Structural weakness vs fast aggro (vs109 was a T8 blowout
    loss last wave; won this wave only because the draw supplied Geralf's + Yawgmoth + Obliterator on curve —
    variance, not a construction change). Consider a cheap early blocker. Same as wave-10 #10.

12. **Discard-suite + fetch self-damage = close-race margin.** vs44 died at -7 with Thoughtseize/fetch/Yawgmoth
    self-damage compounding a race it could not win. A 1-copy Thoughtseize -> Inquisition shift would cut
    self-damage. Construction, not a guide gap. Carried from wave-10 #11.

## WHAT WORKED (freeze — do not churn)
Best corpus yet (5/6, all clean kills). Impact-ranked act-floor + attack floor OBEYED in all 5 wins. Bare-N
GM correct all 3 casts (stabilizer vs110, lethal vs135, build vs140). Both Lilianas coexist + activate.
Empty/thin-hand Thoughtseize discipline at 100%. The beats-are-the-win identity vindicated (vs109 flip:
Geralf's + Yawgmoth -1/-1 grind + Obliterator + beats close at opp0, no GM needed; vs131 grind kill). The
ramble guard verified end-to-end (12 fallbacks, all choice=-1, zero prose casts). Guide is a ZERO-edit
frozen diff.
