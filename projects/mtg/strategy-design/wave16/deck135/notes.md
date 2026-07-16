# Deck-135 wave-16 — development notes (engine / harness / model / construction) + ROTATION VERDICT

Run: PRIMARY `matchups-20260716-084532`. Binary `/tmp/wagic-69793930a` (wave-15 engine batch: free
alternative casts at zero mana; stale_echo superstring matcher; hybrid-pip affordability annotation).
deck135 jsonl epochs + opponents in findings.md game->file map. Doctrine: win column context-only;
layer-routing enforced (engine items live HERE with repros, never in prompt/guide text). Each item
self-contained with seq repros.

## ENGINE LEDGER

- **ENGINE #1 — Force of Negation cast/resolution seam. Three sub-items this corpus.**

  - **#1a — RESOLUTION re-confirmed WORKING (regression watch clean).** deck21 s19 (free exile
    alt-cast) countered Volcanic Hammer: deck21 OPPONENT log s17 `Opponent's Force of Negation: hand
    -> stack`, s20 `Your Volcanic Hammer: stack -> exile | Opponent's Force of Negation: stack ->
    graveyard`. Target went stack->EXILE (countered), not stack->battlefield. Consistent with wave-15
    deck140 seq18/seq30. NOTE this fired with `{u}{u}{c}` from 3 UNTAPPED sources — mana was up. No
    regression. DETECT REGRESSION: a chosen Force whose target EVENTS show stack->graveyard/battlefield
    instead of stack->exile.

  - **#1b (CARRIED from wave-15, UNVALIDATED this corpus) — free alt-cast fizzle when tapped out /
    "(no untapped sources)".** The 69793930a fix targets exactly this: admit the exile-pitch cast at
    zero mana. **It got NO clean exercise this corpus.** The only free alt-cast (deck21 s19) had 3
    untapped sources, so it does not test the tapped-out path; the tapped-out Force windows (deck110)
    offered the HARDCAST, not the free alt-cast (no blue card in hand to pitch), so #1b's trigger shape
    (free alt-cast chosen WITH "(no untapped sources)") did not occur. NOT-EXERCISED != NOT-FIXED.
    VALIDATION TODO carried: a free alt-cast chosen with "(no untapped sources)" that PUTS Force on the
    stack and counters (no silent fizzle). DETECT (unchanged): free-alt-cast option chosen + target
    resolves + zero `Force: hand -> stack` + "(no untapped sources)" on the caster's board that window.

  - **#1c (NEW this corpus, HIGH VALUE) — the HARDCAST Force `{1}{u}{u}` is OFFERED while unpayable,
    then silently fizzles via defer.** REPROS: deck110 s8 (target Cranial Plating, mana `{g}`), s16
    (Galvanic Blast, mana `{g}`), s25 (Galvanic Blast, mana `{g}{u}{c}` = one U short). In every case
    deck135 could NOT make `{1}{u}{u}` and had NO blue card in hand for the alt-cost, yet the engine
    listed `Cast Force of Negation {1}{u}{u}`. The model chose it (choice=1); the follow-up
    `kind=defer, fallback=deferred_to_heuristic, latency=-1` step could not complete the unpayable cast;
    **zero `Force: hand -> stack` in either seat log**; the target spells resolved. This is the SAME
    ROOT as #1b (offer legality not checking payability) but on the HARDCAST mode instead of the
    alt-cost. FIX DIRECTION: gate the Force offer on actual payability of the listed mode — verify
    `{1}{u}{u}` is producible for the hardcast, or a legal blue card exists for the exile alt-cost;
    offer neither mode if neither is payable. DETECT: a Force cast chosen (`ask` window, choice selects
    a "Cast Force of Negation" option) followed by a `deferred_to_heuristic` defer record and ZERO
    `Force of Negation: hand -> stack` events, with the caster short the option's cost.

- **ENGINE #2 (CARRIED from wave-13; [attacking] half REPRODUCED benign, freeze half UNVALIDATED) —
  stale `[attacking]` tag + inverted/absent Treefolk-freeze annotation.**
  - **Stale `[attacking]` — REPRODUCED, benign.** Own TAPPED creatures rendered `[tapped - untaps and
    can attack next turn] [attacking]` in Main phase 2 (post-combat, no active combat): deck133 s36
    (Ice-Fang Coatl), deck62 s29 (Icehide Golem x2), deck44 s34/s35 (Icehide Golem). Verified NON-stale
    (correct) at deck133 s35 (Blockers, your turn), deck44 s28 (Combat damage), deck44 s37 (Blockers,
    opponent's turn). Did NOT drive a wrong decision (all stale windows had the tagged creature tapped
    and inert). FIX (unchanged): clear `[attacking]` on any creature not in the current declared-
    attackers set.
  - **Inverted freeze — NOT REPRODUCED (opportunity absent again).** No non-terminal Treefolk ETB
    tap-and-freeze window occurred at this seat this corpus (vs44 cast Diamond Faerie, not Treefolk,
    late; vs62's Treefolk ETB froze nothing re-rendered). Carry UNVALIDATED. VALIDATION TODO: a
    non-terminal Treefolk-freeze window where the frozen creature is re-rendered; confirm `frozen`/no
    `[attacking]`/no `untaps and can attack next turn`.

- **ENGINE #3 (minor, CARRIED) — fetch chosen-but-unresolved re-ask + duplicate identical option
  lines.** Not re-audited in depth. Persists as decision-count inflation only, zero resource loss.
  Carried unchanged; low priority.

## HARNESS

- **HARNESS #1 (LATENT, CARRIED) — attackers name->index parse gap.** Not re-surfaced with an
  outcome-coupled instance this corpus (no legal A2+ named attacker dropped). Stays latent. FIX
  (unchanged): a name->index reconcile arm mirroring parseChoice's name-echo. DETECT: an `ATTACK:` line
  mixing `A<n>` indices with bare creature names.

- **HARNESS #2 — fallback profile: `unparsed_reply` (ramble guard) dominant + 2 `stale_echo` + Force
  defers.** This seat: 5 unparsed_reply (deck133 s30/s31, deck21 s4, deck62 s29, deck44 s36) all
  latency-ramble timeouts, self-healed, none flipped a game; 2 stale_echo (deck133 s33, deck44 s18)
  BOTH CORRECT catches (echo names an unoffered land type — Forest — where the menu offered
  Plains/Island; not a superstring, so the wave-15 superstring fix correctly did not match). 3
  `deferred_to_heuristic` records (deck110 s9/s17/s26) are the Force-fizzle follow-ups from ENGINE #1c,
  not a text-parse fallback. NO wrongful downgrade this seat; the superstring fix caused no false
  negatives.

- **HARNESS #3 (CARRIED) — GPU_MEM_UTIL=0.75 latency -> LIFE-adjudication.** Brief: only 4
  life-adjudicated corpus-wide (down from 9-13; games completing). At THIS seat all 5 losses were REAL
  deaths — adjudication was not the story; deck135 was beaten on board. Corpus-wide item (brief owns).

## MODEL (route to model-experiments; NOT guide text)

- **Force reflexive-cast drift WORSENED — now on FACE BURN.** All 4 Force casts this corpus were bad
  targets: deck21 s19 Forced Volcanic Hammer at its own face (exiled Ice-Fang Coatl, died same turn) —
  the guide's hardest verbatim "never" line; deck110 s8/s16/s25 Forced Galvanic Blast (face burn) /
  Cranial Plating (equipment), all non-sanctioned. The guide scopes Force correctly; this is model
  over-application. Routed to general-suggestions #2.

- **Card-color / fetch-color hallucination.** deck110 s8/s16/s25: model calls Into the North (green
  {1}{g} sorcery) a "blue card" for the Force alt-cost. deck44 s18: model thinks Flooded Strand fetches
  a Forest (Plains/Island only). Same family as the carried damage-vs-toughness gap. Model-side,
  cross-seat. Routed to general-suggestions #3.

- **IMPROVEMENT — no Rule #1 creature-skip this corpus.** Wave-15 had two hard creature-skips
  (deck110 s3, deck133 s18). This corpus none: the two "creature offered, non-creature chosen" grep
  hits (deck133 s18, deck17 s22) are search-target-resolution windows, not skips. The deployment axis is
  clean; deck135 cast every creature it could (vs44 deployed six).

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- **Manabase strands the deck's own gold / double-pip win creatures — STANDS (unchanged from
  wave-13/15).** vs21 is the sharpest repro yet: **0 creatures cast the whole game**, dead T9, its
  Ice-Fang Coatl x2 ({g}{u}) stranded with a `{u}{u}{c}` board (no green). Into the North (the green
  fixer) itself costs {1}{g} — chicken-and-egg — and the fetches drawn (Flooded Strand) can't make
  green. Direction (user's call): raise effective double-pip green sources, or shave a pip.
- **No evasion answer — STANDS, 3rd corpus.** vs44 (real loss, -4, T16) with SIX creatures deployed:
  no answer to an evasive snowballing 6/6 Faerie Bladecrafter — a near-identical repeat of wave-13 and
  wave-15's vs44. Small ground bodies can't block a flyer.
- **Slow clock is structural.** Abominable Treefolk is the only real closer (lands T6-T16); small
  bodies can't race Affinity (vs110, dead T8) or Goblins (vs21, dead T9). The ONE win (vs62) is the
  ceiling working: creatures stuck vs a SLOW deck (Enchantresses), life moved, 19/5.

## VALIDATION TODO (for the next run)
1. ENGINE #1b: the tapped-out free-cast fix is STILL UNVALIDATED (no trigger window occurred). Need a
   free alt-cast chosen with "(no untapped sources)" that puts Force on the stack and counters.
2. ENGINE #1c (NEW): when the offer-legality payability fix ships, confirm the hardcast Force is NOT
   offered while unpayable (and if it is offered+chosen, it no longer silently defers/fizzles).
3. ENGINE #1a regression watch: any Force cast whose target resolves (stack->graveyard/battlefield)
   instead of stack->exile.
4. ENGINE #2 freeze half: a non-terminal Treefolk-freeze window re-rendering the frozen creature —
   confirm no `[attacking]`/no `untaps and can attack next turn`. UNVALIDATED 3 corpora running.
5. HARNESS #1: an `ATTACK: A1, <Name>` where <Name> IS a legal A2+ attacker (first outcome-coupled
   instance).

## PER-DECK ROTATION VERDICT: **DO NOT ROTATE deck135 out. KEEP IN.**

The rotation rule retires a veteran whose guide takes NO modification AND whose seat surfaces NO new
work signal. deck135's guide took no modification (FROZEN, byte-identical) — but the seat again
surfaced high-value new Force-seam work signal: a NEW engine mechanism (ENGINE #1c, hardcast Force
offered-while-unpayable -> silent defer/fizzle, 3 repros) on the ledger item deck135 OWNS, AND the
wave-15 fix (69793930a, ENGINE #1b tapped-out free-cast) shipped but got NO clean exercise this corpus,
leaving its validation loop OPEN. deck135 is the designated owner of the Force seam; that seam still has
two open engine items (#1b unvalidated, #1c new) plus an unvalidated ENGINE #2 freeze half. Rotating it
out would orphan the exact validation loop it exists to run. KEEP deck135 in through at least the
ENGINE #1b/#1c fixes + confirmation.
