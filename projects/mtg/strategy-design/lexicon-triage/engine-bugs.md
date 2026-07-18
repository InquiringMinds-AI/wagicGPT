# Lexicon test campaign — engine bug catalogue (post fix-pass, 2026-07-18)

Provenance: 832 lexicon fixtures swept per-process → 300 non-passes triaged
one-by-one (9 agents, probe-verified) → all fixture-side defects FIXED and
re-verified → what remains below is the residue that survived BOTH passes. Every
"CONFIRMED" entry has a clean fixture in `Res/test/lexicon/` that fails ONLY on the
defect (those fixtures are parked in `_known_failures.txt` as regression witnesses —
when a bug is fixed, its fixture returns to `_tests.txt`).

The fix pass FALSIFIED eleven of the original triage's engine-bug verdicts (epic,
scry-drain, the deplete/ingest repeat family, tap(noevent), flip, extort, dredge,
completedungeon, affinity-dead-undercount, nolifegainopponent, player-poison
proliferate) — those turned out to be fixture/synthetic-card defects and are now
green. Details of each falsification live in the fix reports; the authoring guide
(docs/testsuite-fixture-authoring.md) encodes the lessons.

## Crashes / parser failures (CONFIRMED)

1. **FIXED 2026-07-18 — `{chosencolor}` OOB crash** (was "cost-catalog
   vector<short> crash"). Root cause: `parseManaCost`'s `{chosencolor}` branch
   called `add(c->chooseacolor, 1)` unguarded, and `chooseacolor` is `-1` until
   an AASetColorChosen resolves → `cost[(size_t)-1]` OOB (assert-abort in debug;
   heap-corrupting 2-byte store in release — CONFIRMED as the multithreaded
   full-registry segfault, same defect). Every other consumer already guarded the
   sentinel; ManaCost.cpp:334 now does too. The original "accumulation across the
   full load" characterization was wrong — deterministic, single-token, fires at
   ability RESOLVE not primitive load. Real cards always wrap `{chosencolor}` in
   `chooseacolor ... chooseend`, so none crashed in play. Witness
   `cost_dynamic_tokens_parse` back in `_tests.txt`, green.
2. **`@mutated` trigger parser NULL** — Archipelagore's
   `@mutated(this):may ... target(<upto:mutations>creature|battlefield) freeze`
   → ABILITYFACTORY NULL; mutate merges fine, the trigger never registers.
   Witnesses: `basic_mutate_triggers_mutation_event`, `effect_mutateover_*`.
3. **Gift mechanic unreachable — RESCOPED to 23 cards, gift branch only
   (2026-07-18 probe).** NOT a parser bug: `alternative`/`ifnot paid(alternative)`
   parse and execute correctly (~220 Kicker/Cleave/Adventure/etc. cards verified
   working), and the "Parser returned NULL" trace is BY-DESIGN output on the
   unpaid branch of every alt-cost card (MTGAbility.cpp:2742-46) — misleading
   ERROR-shaped noise worth silencing/renaming as part of the loud-validation
   work. The real defect: all 23 Gift cards set `other=` equal to `mana=` (gifts
   cost nothing extra), and the engine only sets MANA_PAID_WITH_ALTERNATIVE for a
   DISTINCT payment — so `paid(alternative)` can never become true and every gift
   branch is dead ("gift declined" permanently; each card's default effect works).
   Fix seat: engine — a promise/yes-no additional-cost primitive that sets the
   flag without a mana difference. Secondary (recheck after): `tap(noevent)`
   inside the `and!(...)!` of `_FISHTOKEN_` appeared dropped in the trace.
   Witness: `macro_fish_into_the_flood_maw`.
4. **`altermutationcounter` unusable** — factory case does its side-effects but
   returns no ability ("falls through") → Parser NULL. Witness:
   `effect_altermutationcounter_emits_mutated_event`.
5. **Battle defense-counter line parses NULL on the real card** — Invasion of
   Zendikar's `counter(0/0.6.defense)` ETB → ABILITYFACTORY NULL (synthetic
   `counter(0/0.1.Defense)` parses fine), so the Battle sits at 0 defense and
   `nodefensedamage` has no baseline. Witness: `basic_nodefensedamage_*`.

## Unimplemented mechanics (source-confirmed)

6. **`noentertrg`** — the suppression gate in AllAbilities.h:201 is literally
   commented `//NOT WORKING`; Hushbringer does not stop ETB triggers. Witness:
   `basic_noentertrg_suppresses_etb`.
7. **BANDING** — `MTGCardInstance::banding` only ever assigned NULL;
   `getNextPartner()` never called; band-sharing branches unreachable. Witness:
   `basic_banding_attacks_as_group`.
8. **PHASING** — `GameObserver::phasingPhase()` declared, never defined or called;
   untap never toggles `isPhased`; the `phaseout` effect leaves the card in-zone
   (no observable). Witness: `basic_phasing_leaves_and_returns`.
9. **`leyline`** — Constants::LEYLINE parsed but zero game-logic references; the
   harness also has no opening-hand step. Witness: `basic_leyline_*`.
10. **Day/Night machinery** — the Day emblem's "becomes night" trigger never fires,
    `castcard(Day)` parses NULL, and DFC face-names are indistinguishable in zone
    asserts; blocks daybound/nightbound/nonight. Witnesses: `basic_daybound_*`,
    `basic_nightbound_*`, `basic_nonight_*`.
11. **`primitive=` aliasing in a primitives file** — never copies the referenced
    card's data (even aliasing Grizzly Bears yields an empty card); the field is
    only functional for set `_cards.dat`. Witness: `card_field_primitive_alias_*`.

## Wrong results (CONFIRMED, clean witnesses)

12. **`reduceto` no-op** — all four forms (activated, static, controller, the
    real-card `transforms((,newability[reduceto:N]))` idiom) leave life unchanged.
    Witness: `effect_reduceto_sets_life_total`.
13. **`combattoughness` ignored for damage to players** — Doran-style: unblocked
    0/4 deals 0 (expected 4); has(COMBATTOUGHNESS) is set. Witness:
    `basic_combattoughness_assigns_toughness_damage`.
14. **`totemarmor` never consumes the aura** — creature saved, Umbra stays on the
    battlefield (permanent shield). Witness: `basic_totemarmor_*`.
15. **`noloyaltydamage` carrier blocks the cast entirely** — with Spark Rupture in
    play a Shock aimed at a planeswalker never reaches the stack (works without
    it). Witness: `basic_noloyaltydamage_preserves_planeswalker`.
16. **`alterdevoffset` asymmetric drain** — Gray Merchant + Altar: opponent loses
    devotion (correct) but controller gains devotion+1. Witness:
    `effect_alterdevoffset_changes_devotion`.
17. **`donothing` swallows its `and!` follow-up** — control shape with `life:1`
    gains; donothing's follow-up never fires (AAFakeAbility). Witness:
    `effect_donothing_preserves_followup`.
18. **`count`/`countb` values not passed to `draw:counted*amount`** — DrawAction
    resolves with amount 0 despite matches. Witnesses: `effect_count_*`,
    `effect_countb_*`.
19. **`&&`/`and!()` INTERACTIVE-PAYLOAD FIZZLE (family)** — a combined ability
    whose non-token leg is interactive/forced never executes that leg:
    Devastating Summons' forced land sacrifice, `_AMASS_` (whole line dies, no
    army, no rider), `_MONARCH_OPPONENT_`/`_INITIATIVE_OPPONENT_` riders,
    Finality's exiledeath payload. Non-interactive `transforms(...)` macros with
    the same `&& life:1` shape work. Witnesses: `macro_red_elemental_*`,
    `macro_amass_variants`, `macro_conditionals_and_markers`,
    `macro_suspect_and_finality`.
20. **`exploits` over-sacrifices and `@exploited` never fires** — sacrifices the
    target AND a bystander. Witness: `effect_exploits_fires_exploited_trigger`.
21. **`doforetell` doesn't emit `@foretold`** — exiles correctly, event missing;
    also grants no FORETELL keyword, so a plotted/foretold card can never be cast
    from exile (blocks `macro_plotcast` too). Witnesses: `effect_doforetell_*`,
    `macro_plotcast`.
22. **`reveal ... afterrevealed`: RESCOPED TO 0 REAL CARDS (2026-07-18 probe).**
    `afterrevealed` only fires once the reveal zone is EMPTIED, and only the
    `optionone`/`optiontwo` abilities empty it (AllAbilities.cpp
    MTGRevealingCards::Update). All 410 real usage lines carry an option block
    and all four real structural variants probe WORKS (whole-library tutor,
    fixed-N, revealuntil, reveal-by-type). The witness invented an option-less
    form no card uses → bad-synthetic. Residual engine action: reveal WITHOUT a
    drain block should be REJECTED AT PARSE TIME (silent card-stranding today) —
    a loud-validation item, not a behavior fix. Witness to be reauthored on the
    real form. Fixture: `effect_reveal_resolves_followup`.
23. **interactive surveil strands revealed cards in the `temp` zone** — the
    "put into graveyard" selection never lands them (all `_SURVEIL1/2/3_` cards,
    every driver variant). Witnesses: `macro_surveil1/2/3_*`.
24. **`manifest dread` battlefield leg missing** — mills one, never puts the other
    face-down onto the battlefield. Witness: `macro_manifest_dread`.
25. **`echo` pay-to-keep fails** — paying the echo cost at the right upkeep still
    sacrifices. Witness: `macro_echo`.
26. **`_RECOVER_` no-op** — the dead creature is neither returned to hand nor
    exiled. Witness: `macro_recover`.
27. **`sethand` / cleanup discard not enforced** — no discard at cleanup over max
    hand size (may be a wider engine gap than sethand). Witness:
    `effect_sethand_reduces_cleanup_hand_limit`.
28. **`cantbetargetof` granted-to-another never applies** — the grant never
    reaches the stack. Related: **a `{0}`-cost targeted `grant` fails to bind its
    target** (the same grant with `{T}` works) — zero-cost targeted-grant
    activation looks broken. Witnesses: `effect_cantbetargetof_*` (+ fix-report
    evidence on the grant witness).
29. **`{myevictcost}` always charges {0}** — imprinted-card state not applied at
    cost-parse time. Witness: `cost_dynamic_eval_mycost_myevictcost`.
30. **`ward` counter/fizzle unreliable** — a lone ward wedges the spell on the
    stack; chained wards let it resolve. Witness: `macro_ward_variants`.
31. **`crewbonus` not applied to the crewed vehicle** — Gearshift Ace's first
    strike doesn't reach the Copter. Witness: `field_crewbonus_gearshift_ace`.
32. **sideboard/command-zone targeting inert** — clicks register,
    `target(...|sideboard)`/`(...|commandzone)` moves never happen (all 6 pairs).
    Witness: `target_sideboard_commandzone_matrix`.
33. **`@discarded` never fires on a forced discard from hand** — only the
    cycle-event path reaches it (MTGAbility receiveEvent cycleCheck); rule 603.10a
    says any discard should. Worked around (fixture tests the cycling path); latent
    gap.
34. **`oppnomaxhand` protection collapses when the beneficiary controls no
    permanents** — GameObserver.cpp:1151 clears `nomaxhandsize` by scanning the
    player's own battlefield, ignoring grants from the opponent's permanents.
    Worked around in the fixture (beneficiary given a land); latent gap.
35. **Anafenza, the Foremost: `oppgcreatureexiler` inert on the real card only** —
    synthetic carrier and a byte-identical clone both work; card-data/loading
    discrepancy. (Keyword fixture now uses the synthetic; Anafenza herself is the
    residual bug.)

## Card-DATA bugs (fix in tracked Res, no C++)

36. **RESOLVED AS NOT-BUGS (2026-07-18 step-3 pass).** `_SCAVENGE_`/`_ETERNALIZE_`
    do NOT omit the exile cost — every real card carries it as the `{E}` token in
    its own activation line (`autograveyard={4}{G}{E}:_SCAVENGE_(...)`); the test
    witnesses had omitted `{E}`. Witnesses fixed, both fixtures back in `_tests.txt`
    and green. `_METALCRAFT_`'s missing trailing space after `then` is real but has
    ZERO concatenated users; ruling: macros are invoked with a separating space
    (as all real invocations do) — encoded in the authoring doc rather than as a
    fragile trailing-whitespace edit that any editor would silently strip.

## Engine robustness / harness-boundary items

37. **Unpayable alternative-attack-cost prompt wedges the phase machine** —
    clicking Floodtide Serpent (alt cost `{H(enchantment)}`, nothing to return)
    during attackers leaves the phase stuck; the prompt isn't cancelable by
    default handling.
38. **Menu/chooser-based alternative-cost casts are undrivable by the scripted
    suite seat** (subtype offering, suspend) — the scripted seat is a heuristic AI
    (`ishuman`=0, and several such cards are gated `compare(ishuman)`); mana-cost
    alternatives (emerge) drive fine. Needs an interactive test seat or harness
    support; their fixtures sit in `_known_failures.txt`.
39. **`enlist`'s granted `{T(creature)}` ability is undrivable** — a second click
    on the attacker un-declares it; effectively untestable (and possibly
    unplayable) under the click model. Witness: `macro_enlist_argivian_cavalier`.
40. **`doubleside`** — battlefield-forbidden by design AND unreachable from hand
    via the harness; no test path exists.
41. **Watch item (unconfirmed)**: a `ueot`-wrapped TRIGGERED ability may not tear
    down at cleanup (fired on two consecutive end steps in a strengthened probe).

## Doc-note surprises (engine is "right", the name misleads)

- `explores` only emits the `@explored` event — it never reveals/moves cards.
- `flip(X/Y)` is not a P/T setter — `flip(<CardName>)` transforms into that card
  (`flip(4/4)` found a card literally named "Black Vise" via lookup fallback).
- deplete/ingest color/name "repeat" compares WITHIN a batch of N (N=1 can never
  repeat), and the ingest flags are separate words (`coloringest ingest:N`).
- `mygraveexiler` exiles ALL the controller's dying cards, not just creatures.
- lord-family keywords (`foreach`, `this()`, lords) must be CONTINUOUS — the
  activated `{0}:` form breaks attack declaration for the carrier.
- `completedungeon` sideboards the completing card and emits `@dungeoncompleted`
  with the card in the sideboard — triggers must scope `*|mysideboard`.
