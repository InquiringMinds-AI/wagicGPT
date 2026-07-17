# Deck-135 wave-18 — development notes (engine / harness / model) + ROTATION VERDICT

Run: PRIMARY `matchups-20260716-185930`. Binary `/tmp/wagic-f664539a3` (wave-17 engine batch:
TARGET CHOICE sub-menu framing; hand-card PUT_INTO_PLAY dead-end filter [Mox E6]; beneficial
opponent-only-target cast suppression [E-49c]; "BLOCKS: none" example; "(player, life N)" echo strip).
deck135 jsonl epochs + opponents in findings.md game->file map. Doctrine: win column context-only;
layer-routing enforced (engine items live HERE with repros, never in guide text). Each item
self-contained with seq repros.

## ENGINE LEDGER

- **ENGINE #1 — Force of Negation cast/resolution seam.**
  - **#1a — RESOLUTION re-confirmed WORKING (regression watch CLEAN).** Both Force casts this corpus
    (vs62 s12 target Primal Rage, vs14 s14 target Unsummon) resolved `hand->stack`, target
    `stack->exile`, Force `stack->graveyard`. Target went stack->EXILE (countered), never
    stack->battlefield/graveyard. No regression. DETECT REGRESSION: a chosen Force whose target
    EVENTS show stack->graveyard/battlefield instead of stack->exile.
  - **#1b — VALIDATED wave-17, regression-watch only. CLEAN.** No tapped-out-free-cast fizzle this
    corpus; both Force windows had untapped context and a blue card to pitch. No regression.
  - **#1c (CARRIED, STILL UNEXERCISED) — hardcast `{1}{u}{u}` offered while unpayable -> silent
    defer/fizzle.** No trigger again: both Force windows offered ONLY the free exile-pitch mode
    (deck135 held a legal blue card each time). NOT-EXERCISED != FIXED. Carry the watch. DETECT: a
    "Cast Force of Negation {1}{u}{u}" option chosen, followed by `deferred_to_heuristic` and ZERO
    `Force: hand->stack`, with the caster short the cost.
  - **#1d (NEW this corpus, deck135-OWNED) — the free "exile a blue card" pitch is AUTO-SELECTED by
    the engine with no card-choice surfaced, and the option line does not name which card will be
    exiled.** REPRO: vs62 s12 — option text `Cast Force of Negation with its exile a blue card from
    hand cost - can target on the stack: Primal Rage` (no pitch card named). The model's PLAN said it
    would pitch "Into the North (a blue card)" — but Into the North is `{1}{g}` (GREEN), so the engine
    pitched the ONLY blue card, **Abominable Treefolk** (the deck's finisher). s13 events:
    `Your Abominable Treefolk: hand -> exile` then `Your Force of Negation: hand -> stack`. The model
    committed to the counter without knowing it would lose its closer. COST: this corpus non-fatal
    (raced with Golems and still won vs62) but it threw away the finisher on a low-value counter. FIX
    DIRECTION (representation): name the card the free pitch will exile on the option line, and warn
    when it is the caster's ONLY pitch-color card or a listed finisher; ideally SURFACE the pitch-card
    selection when the caster holds more than one blue card. This is the same annotation lever the
    owner ruling introduces (warn on the option line) — see general-suggestions #1. DETECT: a Force
    (or any exile-a-card alt-cost) whose events exile a HIGH-VALUE card (finisher / only pitch-color
    card) with no preceding card-selection prompt.

- **ENGINE #2 (CARRIED from wave-13; stale-[attacking] half REPRODUCED benign 5th+ wave; freeze half
  UNVALIDATED) — stale `[attacking]` tag + inverted/absent Treefolk-freeze annotation.**
  - **Stale `[attacking]` — REPRODUCED, benign.** Own TAPPED creatures render `[attacking]` in Main
    phase 2 (post-combat, your turn, no active combat): vs62 s26/s32/s33/s34 (Golems/Viper/Coatl
    post-swing), vs110 s18 (Treefolk+Druid post-swing). Verified NON-stale (correct) at all
    opponent-battlefield / Blockers-step instances (vs21 s4, vs49 s21, vs14 s15). Drove NO wrong
    decision (every tagged creature tapped/inert). FIX (unchanged): clear `[attacking]` on any
    creature not in the current declared-attackers set.
  - **Inverted/absent freeze — NOT cleanly re-rendered (5 corpora running).** deck135 DID land Treefolk
    twice (vs110 s10, vs21 s15) and vs110 s11 rendered its ETB tap correctly as a TARGET CHOICE
    (-> Signal Pest). But the FROZEN opponent creature's subsequent battlefield rendering (frozen /
    no `[attacking]` / no "untaps and can attack next turn") was not captured in a non-terminal
    window this corpus (vs110 was adjudicated at T8, one turn after the tap). Carry UNVALIDATED.
    VALIDATION TODO: a non-terminal Treefolk-freeze window re-rendering the frozen creature.

- **ENGINE #3 (minor, CARRIED) — fetch chosen-but-unresolved re-ask + duplicate identical option
  lines.** Still present as decision-count inflation (e.g. Windswept Heath re-listed across the phase
  boundary vs49 s7/s9/s10; Prismatic Vista re-asks vs21 s7/s8, vs110 s7/s8). Zero resource loss; the
  guide's fetch-reappearance line covers the model side. Carried unchanged, low priority.

- **ENGINE #4 (CARRIED from wave-17, HIGH VALUE, deck135-OWNED) — Azcanta, the Sunken Ruin dig
  ability rendered as an opaque "Choose target N of exactly 4" ORDERING menu; put-to-hand whiffed to
  ZERO across two activations last wave.** NOT EXERCISED this corpus — deck135 never transformed
  Search for Azcanta into Azcanta, the Sunken Ruin (no `{2}{U},{T}` activation), so the shrinking-
  ordering menu had no trigger and the fix remains UNCONFIRMED. `grep 'of exactly'` = 0 hits corpus-
  wide at this seat. Carry the fix + the wave-17 validation TODO (mark hand-eligible cards, actually
  put an eligible noncreature-nonland to hand, collapse/label the bottom-ordering steps). DETECT:
  an Azcanta (or any reveal-and-order) activation whose events show all N revealed cards -> library
  with none entering hand.
  - **#4b (KNOWN-OPEN, NOTES ONLY per brief) — optionone reveal-to-X auto-decline fingerprint
    PRESENT.** Search-for-Azcanta-style upkeep top-look: vs35 s14 events `Your Misty Rainforest is
    revealed (from library) / ...goes to library / Phase: Draw / ...: library -> hand` (a LAND
    revealed, the optional put-to-X auto-declined back to library, then drawn on the normal draw
    step); vs35 s19 same shape with Force of Negation. The engine auto-declines the optional reveal
    chooser before the model can answer — the model never had the choice. Root-caused wave-17; fix
    pending a reveal repro harness. NOT re-diagnosed.

## HARNESS

- **HARNESS #1 (LATENT, CARRIED) — attackers name->index parse.** The mixed `ATTACK: A1, <Name>`
  syntax appeared and PARSED CORRECTLY: vs35 s17 (`Boreal Druid, Ice-Fang Coatl` -> A1, Coatl),
  vs62 s19/s25 (`A1, A2, Ohran Viper` / `...Ice-Fang Coatl`), vs110 s17 (`Boreal Druid, Abominable
  Treefolk`), vs14 s31/s36 (`...Diamond Faerie`). No misfire, no dropped LEGAL attacker. Stays
  latent. DETECT (unchanged): an `ATTACK:` line naming a LEGAL A2+ attacker that is silently dropped.

- **HARNESS #2 — fallback profile: 3 unparsed_reply, 0 stale_echo, 0 defer, 0 empty (BEST profile at
  this seat).** All 3 unparsed are decode-time REPEAT-LOOPS (vs49 s22 ~100x, vs110 s19 ~60x, vs62 s31
  life-accounting confusion on an Ohran Viper Draw/Decline trigger). All in LOW-LIFE / ambiguous
  spots on phase-restricted or binary menus; NONE flipped a winnable game (2 in lost games, 1
  post-adjudication in a won game). A 4th repeat-loop (vs35 s19 ~15x) PARSED. NOT the PLAN-caveat
  stale-plan class (confirmed absent — none carry a stale prior PLAN). The 0 stale_echo confirms the
  wave-17 TARGET CHOICE framing closed the seat's old target-sub-menu stale_echo seam (2 last wave ->
  0 this wave). Route the repeat-loops to a decode-time mitigation (repetition penalty / max-token
  guard with a last-well-formed-CHOICE salvage). See general-suggestions #2.

- **HARNESS #3 (CARRIED) — GPU_MEM_UTIL latency -> LIFE-adjudication.** vs110 adjudicated (14 v 13,
  T8) with deck135 AHEAD — adjudication confirmed a deserved win, not a distortion. Corpus-wide item
  (brief owns).

## MODEL (route to model-experiments; NOT guide text) — see general-suggestions #1/#2/#3.

- Reflexive Force on off-whitelist noncreatures (4th corpus, mutated to pump-enchantment + bounce);
  card-knowledge (Into the North "blue"; Windswept Heath "fetches Island"); repeat-loop spirals.
- **IMPROVEMENT — the CONTROL game executed cleanly.** All 8 target sub-menu decisions correct;
  Treefolk + Gelid Shackles sequence won vs110; Diamond Faerie flying finisher closed vs14; no Rule #1
  hard creature-skip. The residual losses are construction/matchup + variance, not play errors.

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- **Force of Negation DEAD vs all-creature aggro — repro'd AGAIN.** vs35 (real, T9): flooded on Force
  vs Mighty Slivers (no noncreature spells to counter); never castable; ran over by a Sliver army.
  Same flag as wave-17. The Force count is a hedge that whiffs entirely vs creature decks.
- **Slow ground clock + no evasion answer — STANDS (5th corpus).** vs35 (T9) overrun; vs21 (T9)
  outraced by Goblin burn, Treefolk landed T6 too late. Abominable Treefolk / Diamond Faerie (the
  flying finisher, which DID close vs14) remain the only real closers.
- **Thin blue base + fetch-color mismatch — STANDS.** vs49 (real, T9): blue-screwed because the early
  fixing was Windswept Heath (Forest/Plains only, cannot fetch the needed Island); a WIN last wave
  flipped to a LOSS on the draw. Gold/double-pip and blue-source dependence remain the fragility.

## VALIDATION TODO (for the next run)
1. ENGINE #1d (NEW): when the Force option-line names the pitch card (or surfaces the pitch-card
   selection), confirm the model no longer exiles its finisher to a low-value counter.
2. ENGINE #1c: STILL UNEXERCISED — need a Force window where deck135 is tapped out AND holds no blue
   card, to confirm the hardcast `{1}{u}{u}` is not offered / no longer silently defers.
3. ENGINE #4 (Azcanta ordering menu): UNEXERCISED this corpus — need a Search-for-Azcanta transform +
   `{2}{U},{T}` activation to confirm the representation fix (hand-eligibility marks, put-to-hand
   fires, bottom-ordering collapsed).
4. ENGINE #2 freeze half: a non-terminal Treefolk-freeze window re-rendering the frozen creature —
   UNVALIDATED 5 corpora (Treefolk landed vs110/vs21 but no clean non-terminal re-render captured).
5. HARNESS #2: whether a decode-time repetition guard / last-CHOICE salvage lands the repeat-loop
   unparsed count toward 0.

## PER-DECK ROTATION VERDICT: **DO NOT ROTATE deck135 out. KEEP IN.**

Rotation retires a veteran whose guide takes NO modification AND whose seat surfaces NO new work
signal. deck135's guide took no modification (FROZEN, byte-identical, `cmp`-verified) — but the seat
surfaced strong NEW work signal:
- **Charge (a) answered with the seat's best evidence yet:** the wave-17 TARGET CHOICE framing UNLOCKED
  the control game (0 stale_echo vs 2 last wave; 8/8 correct target picks; the vs110 Treefolk+Shackles
  sequence WON on it). This is the validation loop deck135 exists to run, and it paid off — the seat
  is the designated proving ground for the target-sub-menu + Force seams.
- **NEW engine finding (ENGINE #1d):** the free Force pitch auto-exiles the finisher with no
  card-selection surfaced and no card named on the option line — a representation gap with a clear
  fix, feeding the owner's incoming warning-annotation design.
- **Carried loops still OPEN and needing this seat:** #1c (hardcast-while-unpayable) unexercised a 3rd
  corpus; #4 (Azcanta ordering menu) fix unconfirmed (unexercised); #2 freeze-half unvalidated a 5th
  corpus. Rotating deck135 out would orphan exactly these.
KEEP deck135 IN through at least the ENGINE #1d/#4 confirmations and the #1c/#2-freeze windows.
