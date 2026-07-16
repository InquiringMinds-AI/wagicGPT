# Deck-135 wave-17 — development notes (engine / harness / model / construction) + ROTATION VERDICT

Run: PRIMARY `matchups-20260716-151559`. Binary `/tmp/wagic-7cdcf9c73` (wave-16 engine batch:
PLAN-carry stale-intent caveat [319 decisions corpus-wide]; legalCasts normalEntry fix; own-target-
only BAD-effect suppression). deck135 jsonl epochs + opponents in findings.md game->file map.
Doctrine: win column context-only; layer-routing enforced (engine items live HERE with repros,
never in prompt/guide text). Each item self-contained with seq repros.

## ENGINE LEDGER

- **ENGINE #1 — Force of Negation cast/resolution seam.**
  - **#1a — RESOLUTION re-confirmed WORKING (regression watch CLEAN).** deck21 s6 and s9 (free
    exile-pitch) each countered a Volcanic Hammer: `Force: hand->stack` then target `stack->exile`,
    Force `stack->graveyard`. Target went stack->EXILE (countered), not stack->battlefield/graveyard.
    No regression. DETECT REGRESSION: a chosen Force whose target EVENTS show stack->graveyard/
    battlefield instead of stack->exile.
  - **#1b (CARRIED from wave-15 as UNVALIDATED) — NOW VALIDATED. CLOSE THE LOOP.** The 69793930a
    tapped-out free-cast fix got its first clean exercise: **deck21 s6** shows `Mana available:
    (no untapped sources)`, the free exile-pitch offered + chosen, and Force PUT ON THE STACK and
    COUNTERED Volcanic Hammer (no silent fizzle). This is exactly the trigger shape wave-16 lacked.
    The tapped-out free-cast path works end-to-end. Downgrade #1b from open to VALIDATED/CLOSED.
  - **#1c (CARRIED from wave-16, NOT exercised this corpus) — hardcast `{1}{u}{u}` offered while
    unpayable -> silent defer/fizzle.** No trigger this corpus: both Force windows offered ONLY the
    free mode (deck135 held a legal blue card to pitch each time), so the hardcast-while-unpayable
    path did not surface. The wave-16 binary added "own-target-only BAD-effect suppression" +
    legalCasts normalEntry fix, which MAY have closed #1c, but that is unconfirmed here. NOT-EXERCISED
    != FIXED. Carry the watch. DETECT: a "Cast Force of Negation {1}{u}{u}" option chosen, followed
    by a `deferred_to_heuristic` record and ZERO `Force: hand->stack`, with the caster short the cost.

- **ENGINE #2 (CARRIED from wave-13; [attacking] half REPRODUCED benign 3rd+ wave; freeze half
  UNVALIDATED) — stale `[attacking]` tag + inverted/absent Treefolk-freeze annotation.**
  - **Stale `[attacking]` — REPRODUCED, benign.** Own TAPPED creatures rendered `[tapped - untaps
    and can attack next turn] [attacking]` in Main phase 2 (post-combat, your turn, no active
    combat): deck110 s31 (Boreal Druid + Icehide Golem + 2x Ice-Fang Coatl), deck49 s13/s14 (Icehide
    Golem). Verified NON-stale (correct) at all opponent-battlefield / Blockers-step instances
    (deck21 s8, deck17 s18, deck110 s9/s13, deck35 s6/s11). Drove NO wrong decision (every tagged
    creature tapped and inert). FIX (unchanged): clear `[attacking]` on any creature not in the
    current declared-attackers set.
  - **Inverted/absent freeze — NOT REPRODUCED (opportunity absent again, 4 corpora running).** No
    non-terminal Treefolk ETB tap-and-freeze window re-rendered at this seat (deck135 never landed
    Treefolk this corpus — it went to library via Azcanta whiffs / was drawn too late everywhere).
    Carry UNVALIDATED. VALIDATION TODO: a non-terminal Treefolk-freeze window re-rendering the frozen
    creature; confirm `frozen`/no `[attacking]`/no `untaps and can attack next turn`.

- **ENGINE #3 (minor, CARRIED) — fetch chosen-but-unresolved re-ask + duplicate identical option
  lines.** Still present as decision-count inflation (e.g. deck21 s3/s4, s16/s17 the same Prismatic
  Vista crack re-listed across the phase boundary; deck110 Misty Rainforest s7/s8; deck49 Windswept
  Heath s6/s8/s9). Zero resource loss; the guide's fetch-reappearance line covers the model side.
  Carried unchanged, low priority.

- **ENGINE #4 (NEW this corpus, HIGH VALUE, deck135-OWNED) — Azcanta, the Sunken Ruin dig ability
  rendered as an opaque "Choose target N of exactly 4" ORDERING menu; put-to-hand whiffs to ZERO
  across two activations.** REPROS: deck62 s28-s30 (activation 1: revealed Abominable Treefolk
  [creature], Force of Negation, Gelid Shackles, Search for Azcanta; model picked target1=Treefolk;
  s30 events = ALL 4 -> library, nothing to hand) and s36-s39 (activation 2: revealed Diamond Faerie
  [creature], Ohran Viper [creature], Flooded Strand [land], Arcum's Astrolabe; model picked
  target1=Ohran Viper; s39 events = ALL 4 -> library, nothing to hand). The engine exposes
  `{2}{U},{T}: look top 4, put ONE NONCREATURE-NONLAND into hand, rest to bottom in any order` as a
  shrinking-option ordering loop that states NONE of: the effect, the noncreature-nonland
  eligibility restriction, which slot is "to hand", or the running assignment. The model picked
  ineligible CREATURES as the keep in both activations and the put-to-hand silently voided.
  COST: MATERIAL — 6 of the seat's 7 fallbacks (deck62 s21/s22/s23/s29/s31/s38), zero card advantage
  from two `{2}{U}` activations, finisher never deployed, in a NON-screwed game vs a slow deck deck135
  BEAT 19/5 last wave -> adjudicated loss 14/20. FIX DIRECTION (representation + verify engine per
  wagicgpt doctrine): (a) name the effect in plain terms; (b) MARK which revealed cards are eligible
  for hand (noncreature-nonland) and gray out creatures/lands; (c) collapse or clearly label the
  immaterial bottom-ordering steps (targets 2..N); (d) show already-assigned picks; AND verify the
  engine's Azcanta put-to-hand actually fires when an ELIGIBLE card is selected as target 1 — across
  two activations here it never put anything in hand, which may be a scripting bug, not just model
  mis-selection. DETECT: an Azcanta (or any reveal-and-order) activation whose events show all N
  revealed cards -> library with none entering hand. Same FAMILY as deck110's promoted E6 (Mox Opal
  degenerate cast-label): engine internal-mechanic exposed as a bare menu -> see general-suggestions #2.

## HARNESS

- **HARNESS #1 (LATENT, CARRIED) — attackers name->index parse.** The mixed `ATTACK: A1, <Name>,
  <Name>` syntax appeared (deck49 s23, s27) and PARSED CORRECTLY both times: at s23 only the one
  legal (non-summoning-sick) named creature was declared; at s27 all three legal named creatures
  were declared. No misfire, no dropped LEGAL attacker. Stays latent. DETECT (unchanged): an
  `ATTACK:` line naming a LEGAL A2+ attacker that is silently dropped.

- **HARNESS #2 — fallback profile: 5 unparsed_reply + 2 stale_echo, all self-healed, none flipped a
  WON game.** deck62 owns 6 (s21 unparsed = Into-the-North target repeat-loop conflating a prior-turn
  log line with the current menu; s22/s23 stale_echo = CORRECT catches, model echoed unoffered
  "Snow-Covered Forest" when the sub-menu offered Scrying Sheets/Island (s22) / Prismatic Vista
  (s23) — settled, not superstrings, wave-15 fix correctly no-matched; s29/s31/s38 = the Azcanta
  ENGINE #4 cluster). deck110 owns 1 (s31 unparsed = lethal-count ramble at 2 life, dead anyway).
  NO wrongful downgrade this seat. The seat's unparsed count concentrates entirely on ENGINE #4 +
  its downstream spillover (s31) — consistent with the PLAN-caveat corpus-wide halving working and
  the residual timeouts being representation-driven, not plan-drift.

- **HARNESS #3 (CARRIED) — GPU_MEM_UTIL latency -> LIFE-adjudication.** Corpus-wide only 3
  life-adjudicated (best yet). At THIS seat TWO of the six games were adjudicated (vs62 @14, vs110
  @2) — deck135 was alive-but-behind in both, so adjudication was the tiebreak, not the cause
  (vs62 lost to ENGINE #4, vs110 to a too-slow construction clock). Corpus-wide item (brief owns).

## MODEL (route to model-experiments; NOT guide text)

- **Reflexive Force on FACE BURN — 3rd corpus.** deck21 s6 (@19 life, exiled Search for Azcanta),
  s9 (@10 life, exiled Ice-Fang Coatl). Both countered a `[spell] targeting you` Volcanic Hammer =
  the guide's hardest verbatim NEVER line. Model over-application; guide already maximal. -> general-
  suggestions #1.
- **Card-KNOWLEDGE errors (type/cost/color).** Azcanta grabs-a-creature (ENGINE #4 model side);
  Treefolk mana-miscount (deck17 s19/s20: `{g}{u}{u}`=3 asserted to pay `{2}{g}{u}`=4, then PASSED
  three decisions waiting on an unaffordable creature the engine never offered — the exact losing
  thought the guide names). Joins carried wave-16 family (Into the North "blue"; Flooded Strand
  "fetches Forest"). -> general-suggestions #3.
- **IMPROVEMENT — no Rule #1 hard creature-skip this corpus (repeat of wave-16).** Deployment axis
  clean: vs49 deployed 7 bodies and won; every game cast every creature it could. The residual
  losses are construction/screw/evasion + the ENGINE #4 whiff, not skipped creatures.

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- **Slow clock + no evasion answer — STANDS (4th corpus).** vs17 (real, T9) overrun by evasive
  Faeries with only ground bodies; vs110 (adj, T14) raced to 2 but the clock (small bodies, Treefolk
  a turn-late 11/11) couldn't close vs Affinity. Abominable Treefolk remains the only real closer.
- **Force of Negation is DEAD vs all-creature aggro — sharp repro this corpus.** vs35 (real, T9)
  flooded on 2x Force of Negation (counters only NONCREATURE) against a Mighty Slivers deck with no
  noncreature spells to counter — two dead cards while drawing no threat. Direction (user's call):
  the Force count is a hedge that whiffs entirely vs creature decks.
- **Gold / double-pip screw off a thin blue/green base — STANDS.** vs21 (real, T15) green-screwed on
  a `{u}{u}{c}` board, its `{g}`/`{g}{u}` creatures stranded, 0 effective creatures.

## VALIDATION TODO (for the next run)
1. ENGINE #4 (NEW): when the Azcanta representation fix ships, confirm (a) revealed cards are marked
   for hand-eligibility, (b) selecting an ELIGIBLE noncreature-nonland as the keep actually PUTS IT
   IN HAND (not all-to-library), (c) the immaterial bottom-ordering steps are collapsed/labeled.
2. ENGINE #1c: still UNEXERCISED — need a Force window where deck135 is tapped out AND holds no blue
   card, to confirm the hardcast `{1}{u}{u}` is not offered (or no longer silently defers if it is).
3. ENGINE #1b: VALIDATED this corpus (deck21 s6). Downgrade to regression-watch only.
4. ENGINE #2 freeze half: a non-terminal Treefolk-freeze window re-rendering the frozen creature —
   UNVALIDATED 4 corpora running (deck135 keeps failing to land Treefolk).
5. HARNESS #1: an `ATTACK: A1, <Name>` where <Name> IS a legal A2+ attacker that is dropped (still
   no misfire — the mixed syntax parsed correctly both times this corpus).

## PER-DECK ROTATION VERDICT: **DO NOT ROTATE deck135 out. KEEP IN.**

The rotation rule retires a veteran whose guide takes NO modification AND whose seat surfaces NO
new work signal. deck135's guide took no modification (FROZEN, byte-identical, `cmp`-verified) — but
the seat surfaced HIGH-value new work: **ENGINE #4 (NEW)** — the Azcanta multi-target ordering menu
representation defect with a MATERIAL cost (whiffed dig twice, plausibly cost the vs62 game), on the
deck135-owned representation charge the brief assigned; PLUS it CLOSED a carried validation loop
(**ENGINE #1b** tapped-out free-cast now VALIDATED at deck21 s6). deck135 is the designated owner of
the Force seam AND (now) the reveal-and-order representation seam; #1c stays unexercised, ENGINE #2's
freeze half is unvalidated a 4th corpus, and #4 needs its fix confirmed. Rotating it out would orphan
exactly the validation loops it exists to run. KEEP deck135 IN through at least the ENGINE #4 fix +
confirmation and the #1c/#2-freeze windows.
