# Wave-31 rotation reconciliation

## OUT this wave (THREE seats) -- UNSENTIMENTAL

The largest single-wave rotation ties wave-26's record. All three leave with FROZEN guides and a
named hand-off obligation; none of them closes its residual by leaving.

- **deck122 (WB Eternal Life -- lifegain / Felidar alt-win / Sorin).** Exit evidence: *no loss
  traces to a teach* -- 4 of 5 teaches exercised and validated, the 5th (Sorin) unexercised but
  already validated in wave 30 on a shared surface (wave-30 HL5), and the 6/6 -> 3/6 record delta
  decomposes entirely into the pool (kills flat at 2; adj-ahead 4 -> 1; pool adjudications 12/21 ->
  3/21). Guide byte-frozen at `bin/Res/ai/baka/deck122_strategy.txt`. The one piece of new guide
  work -- the "attacking taps your blockers" corollary from vs158 seq30 -- is a GENERAL teach-
  category gap, not a deck122 fact, and shipped to the skill (wave-31 HL3) instead of a rotation
  slot.
  - **HAND-OFF: the N-122a Felidar probe.** The alt-win check is LIVE-UNVALIDATED for a second
    consecutive corpus: the ability is well-formed (`auto=@each my upkeep:this(controllerlife > 39)
    wingame`), the card never reached the battlefield in any of the 6 games (in hand once, bottomed
    at the London mulligan), and the seat crossed 40 life WITHOUT it (vs152: 40+ at seq19, finished
    at 63, five own upkeeps passed at 40+). Two corpora of random draws are sufficient evidence that
    the pool cannot co-present card + threshold. **Carry as a between-wave STACKED PROBE
    (deck199-style: Felidar + gainers, no other win route).** It is not a reason to hold a slot.
  - Also leaving with the seat, all engine-lane: N-122b (missing cast asks with payable spells),
    N-122c (attacker tapped-tag contradiction -- CROSS-SEAT, top of the ledger), N-122d (truncation
    discards a valid answer-first `BLOCKS:` line).

- **deck148 (mono-W Kor Army equipment) -- the CONSTRUCTION-CEILINGED exit, walked as specified.**
  Exit evidence: three corpora (1/6 Step-0, 2/6 first-guided, 4/6 confirming) with the same 60 cards
  and, for the last two, a byte-identical guide; all four teaches fired again under a materially
  different pool, one quoted back verbatim by the pilot (vs146 seq19); **0 fallbacks / 187
  decisions**, 0 parser contradictions, 0 crash signatures, 0 adjudications. Per wave-31 HL7 the
  4/6 is NOT read as the ceiling moving -- the ceiling is confirmed on the LOSS SIGNATURE: in both
  losses the deck got its conqueror to **2 life (vs152)** and **1 life (vs137)** and had no card in
  60 that could finish or survive. Guide freezes.
  - **HAND-OFF: the CONSTRUCTION package to the roster owner** (deck148/notes.md Part B, four
    numbered items, each grounded in a named game): (1) trim the low-impact equipment glut -- 17
    equipment against 20 creatures; cut Argentum Armor x2 (drawn twice, cast **zero** times in 12
    guided games at {6}+{6}), Bladed Pinions x2, 2 Bone Saw, aim ~11-12 equipment / 24-25 creatures;
    (2) the engine must SURVIVE -- Armament Master cast 5 times across 12 guided games and geared
    **once** (the game it won outright); (3) zero interaction / zero reach / zero card advantage is
    the ceiling itself -- 2-4 slots of cheap white interaction is the single highest-value add; (4)
    information only -- Zamriel x2, Kabira Crossroads x2 and Argentum Armor x2 have zero live data
    in 12 games. **If the list is rebuilt per Part B the result is a materially different deck and
    re-enters as a fresh Step-0 -> first-guided arc under a new seat, not as a continuation.**
  - Engine-lane residuals leaving with the seat: N-148a/b (equipment attachment render; N-148b
    caused a WRONG COMMIT and is best validated by a token+equipment PROBE deck -- the same
    reasoning that rotated 136 out), N-148c (granted lifelink never produces life), N-148d (no
    creature marker on hand lines -> a false mulligan).
  - **NOTE for the orchestrator:** deck148's reviewer requested a *separate* "construction-terminal-
    adjacent frozen list" rather than the canary list. This reconciliation places it on the CANARY
    list (hence 23, per the wave's accounting) with an additional `construction-handed-off` tag,
    because its guide is converged and re-runnable exactly like a canary; deck134 remains the only
    entry on the true construction-TERMINAL list. Say the word if you want the seat's own
    formulation instead -- the canary count would then read 22.

- **deck137 (GW convoke + Adventures) -- the longest-serving seat (in-pool since wave-22) leaves on
  GUIDE-LANE exhaustion, with the convoke arc explicitly NOT closed.** The reviewer states plainly
  that the h2 gate as written is NOT met (offered convokes resolved 2/8; failures are 1 defer + 5
  silent no-ops, not exclusively divergence-C) and recommends rotation anyway on the campaign's own
  unsentimental criterion. Synthesis CONCURS, on the wave-30 deck136 precedent: nine guided rounds,
  byte-frozen two consecutive waves, every teach exercised and correct this corpus, zero parser
  events, zero perception misreads, and no guide edit this corpus justifies. The premise that held
  the seat in wave 30 -- an UNCHARACTERIZED residual -- is discharged: the arc now hands wave-32
  three **source-located** defects with file:line and repro seqs, plus a confirmed divergence-C
  shape. And a random-draw seat is now the WORSE instrument: 6 games produced only 8 convoke picks,
  half the March evidence came from ONE game, and two games contributed zero convoke events.
  - **HAND-OFF: the MANDATORY convoke probe.** Full spec in `deck137/findings.md` §8 -- build
    `deck198.txt` (4x March of the Multitudes, 4x Venerated Loxodon, 4x Emmara, **12x Forest**, 4x
    Plains, 4x Edgewall Innkeeper, 4x Lovestruck Beast for the white Human tokens, 4x Flower //
    Flourish), copy `deck137_strategy.txt` to `deck198_strategy.txt`, run pinned one-shot GPT games
    with `timeout 3000`, **DELETE both probe decks afterwards** and verify the highest `deckN` is
    back to the real roster. Post-fix PASS conditions, all three ARRIVAL-TRACED (never counter-read):
    an `ANNOUNCE_X` record on a convoke-route pick plus `March: hand -> stack` and `Soldier (1/1):
    created -> battlefield` xN with N == the announced X; on an all-Forest board with >= 2 untapped
    white Human tokens the convoke cast line **is offered** and the pool balances after payment; the
    `Cast Card Normally` negative control still resolves; and offered-convoke picks producing no
    downstream game-state effect == 0.
  - **This write-up must not be read as closing the convoke arc.** The arc closes when the wave-32
    fix is validated against that observable.

## Canary list

20 -> **23** (+122, +148, +137). deck134 remains on the SEPARATE construction-terminal list (not a
canary). deck148 additionally carries the `construction-handed-off` tag (see the note above).

## HOLDS (4 seats)

- **146 (WB Orzhov Dungeons) -- HOLD; exit projection moved wave-32 -> WAVE-33.** The wave-30
  projection rested on a false premise: it read the seat as converged with only a Kaya draw-variance
  residual. Kaya IS now exercised and discharged (her 9-option loyalty menu, vs122 seq35 t12, is
  clean) -- but the retroactive card-fact audit found the "converged, frozen" guide carrying THREE
  false teaches, one of which cost a game and produced 59 impossible plans, through four corpora and
  three freeze validations. A guide with a game-costing false teach is by definition not converged,
  and the seat now has genuinely NEW, non-speculative guide work that has never been validated.
  Guide REVISED this wave. Wave-32 exit gate (the reviewer's, adopted): (1) the Acererak fix lands --
  **zero windows in which an offered removal spell with a live target is declined in favour of Cast
  Acererak** (this corpus: 4 such windows in one game) and the 59 attack-with-Acererak passages go
  to ~0; (2) the Vanishing Verse take-rate recovers (16 offered / 4 taken = 25% this corpus, the
  weakest number at the seat, against 16 taken in wave 30); (3) the MDFC text is truthful in
  practice; (4) N-146h resolved or the guide's workaround holds -- and if the engine fix ships, the
  "leave Agadeem's Awakening alone" line becomes a crutch to DEMOTE; (5) the three-corpus-stable
  surfaces stay validated. Clean on (1)-(3) => rotate at wave-33 with a hand-off covering N-146h and
  the N-152a MDFC lane.
- **139 (Temur/Sultai Mutants) -- HOLD; freeze DECLINED, gated on the N-139h probe.** Three of five
  wave-32 gate conditions are open. MET: N-139h fixed (0 crash signatures at 6/6 seat stderr; 4/4
  offered-and-chosen mutates arrived; 0 silent no-ops). **NOT MET -- UNEXERCISED:** N-139h
  REVALIDATED (the crashing conjunction never occurred -- Pollywog reached play in 1 of 6 games, its
  one discounted offer was declined, it died two turns later). NOT MET: guide frozen (REVISED this
  wave -- mulligan floor with a numeric terminator, dual-land counting with the actual offenders
  named, mutate-line verification, attack floor). NOT MET: the combat-latency teach (tail WORSENED,
  max 209s -> 224s, >195s count 4 -> 6, and RELOCATED off combat onto casting/mulligan asks; teach
  retargeted, needs re-measurement). PARTIAL: revised-guide validation. Two of the freshly-authored
  teaches have never been tested. **Pair the hold with the scripted N-139h probe so the crash
  validation does not depend on drawing Pollywog again.** Construction note for the roster owner
  (independent of rotation): the five double-pip Apex mutates are unsupported by this mana base.
- **152 (Bant Midrange GWU) -- HOLD; Step-0 seat, first guided outing, guide REVISED.** Step-0 seats
  never rotate on their first guided corpus. h4 and h5 both landed at this seat and are cited
  (48 truthful MDFC-land renders, old text gone from all land lines; priority asks 148 -> 22 with
  display-toggle-only windows 121 -> 0; seat fallbacks 1 -> 0). h4/h5 PASS. Guide revisions this
  wave: a hard DEPLOY FLOOR (the two fatal low-life declines the cast-nothing split exposed), the
  werewolf rarity claim REWRITTEN as the rule plus both outcomes plus the board field that answers
  it, and the T7b Flip-Side teach DELETED as unreachable under h5. Carries N-152c (HIGH, first-
  order), N-152d, N-152e.
- **158 (Orcs of Mordor, RB amass) -- HOLD; Step-0 debut 5/6, initial guide drafted.** The strongest
  Step-0 showing of the campaign, with five clean kills and zero adjudications; the initial guide is
  written against the new PRESERVE-LEDGER rule (a short confirmatory attack line over a 30/30 seam;
  the #1 slot given to the single false belief that traces to the only loss). Its guide's Rule #1
  mana-line stopgap and the Foray / Feed the Swarm magnitude stopgaps all carry explicit removal
  conditions tied to N-158g / N-158c / N-158h.

## WAVE-32 POOL PROPOSAL (7 seats)

Holdovers (4): **146** (WB dungeons/venture) · **139** (Temur/Sultai mutate) · **152** (GWU
werewolf / Class level-up / walkers) · **158** (RB amass).

Surface/colour gap analysis vs those four. Colours are broadly covered (W/U/B/R/G all present), so
COLOUR is the weaker axis this wave and DECISION SURFACE is the stronger one. What the holdovers do
NOT exercise: any **alternate win condition** (deck122 took it out with it), any **artifact-matters**
deck at all, any **cheat-a-permanent-into-play** seam, any **tutor/search** deck (deck136 took the
reveal path out), and any **mono-colour** identity. Three rotate-ins, chosen to hit one of those
each:

- **deck105 "Infected Wounds" (BG infect/poison, 28 lines of decklist).** Opens the **POISON alternate-win
  surface** -- a second, independent instance of the now-ESTABLISHED alt-win teach category that
  deck122 validated and then removed from the pool, on a counter type nothing in the campaign has
  ever rendered. Infect also creates a decision surface with no analogue: combat damage that does
  NOT reduce life, so every race calculation the pilot knows is wrong by default, and the blockers
  annotations ("you would be at M -- NOT lethal") are structurally misleading for it. Non-singleton
  core (Ichorclaw Myr x4, plus 2-ofs of Necropede / Plague Stinger / Putrefax / Hand of the
  Praetors / Skithiryx) so the surface fires most games.
  *Risk to name up front:* if the engine does not implement poison counters, the seat's headline
  surface is UNEXERCISED on arrival -- which is itself a first-order ledger finding, and the Step-0
  read stays useful either way.
  *Runner-up considered:* **deck58 "Bloodhall Ooze"** (BG, +1/+1-counter growth) -- adds no new
  surface over deck158's amass counters.
- **deck36 "Master of Ether" (UB artifacts, 28 lines of decklist).** The pool has **zero artifact-matters** deck
  and has not had one since deck110 (affinity) rotated out many waves ago. It brings four surfaces
  at once: **dynamic artifact-count-scaling P/T** (Master of Etherium x4, Salvage Slasher, Glaze
  Fiend -- a moving lord the render must keep honest), **artifact LANDS** (Seat of the Synod x4,
  Vault of Whispers x4, Great Furnace, Ancient Den, Tree of Tales -- lands that are also artifacts,
  which stresses exactly the type-tagging and `Artifacts in play:` counting that deck148's
  N-148a/b just exposed), **{0}-cost casts** (Ornithopter x4, Mox Jet, Mox Sapphire -- the cheapest
  possible test of the `Mana available:` render N-158g is about), and Tolarian Academy's scaling
  mana. Adds blue weight the pool only has via two multicolour holdovers.
  *Runner-up considered:* **deck121 "Reckoner's Chain"** -- equipment, which deck148 just spent
  three corpora on.
- **deck116 "Time of Need" (GU midrange, 39 lines of decklist).** Opens the **cheat-into-play** seam --
  Elvish Piper x4 (`{G}, T: put a creature from your hand onto the battlefield`) and Quicksilver
  Amulet x4 -- which is a PUT-INTO-PLAY decision shape distinct from casting and lives on the same
  engine lane as the h4/N-152a put-into-play defect. It also brings a **tutor/search** deck back to
  the pool (Time of Need x3, Fabricate x3) at a moment when the reveal/search path carries an open
  ledger item (N-136a family, N-137e), and a **1-of legendary/Eldrazi top end** (Emrakul, Kozilek,
  Ulamog, Sheoldred, Jin-Gitaxias) whose cast/annihilator/shuffle triggers no seat has ever
  rendered. Its own hazard is known and is a feature: 1-of finishers are the absent-engine-card risk
  the wave-20 rung names, and the deck's four Pipers are the redundancy that makes the SURFACE fire
  even when a specific fatty does not.
  *Runner-up considered:* **deck92 "Mermadness" (mono-U merfolk tempo)** -- genuinely new bounce and
  mass-bounce (Unsummon x4, Boomerang, Inundate) plus the pool's only mono-colour identity; it lost
  to deck116 only because bounce is a targeting variation on surfaces already proven, while
  put-into-play and tutoring are new SHAPES. **First alternate if any pick above is vetoed.**

## Guideless-roster accounting

164 total decks. **28** currently carry a `deckN_strategy.txt` on disk (8 14 17 18 21 22 27 35 44 49
59 62 93 102 109 110 122 131 133 134 135 136 137 139 140 146 148 152). Deploying deck158's initial
guide this wave -> **29 guided, 135 guideless**. The three wave-32 rotate-ins (105, 36, 116) stay
GUIDELESS until their initial guides are drafted at the end of wave 32, so the guideless reserve
after this wave is **135**. All three exited seats (122, 148, 137) KEEP their guides, so they remain
counted among the 29. Commander-size decks (16 26 34 46 55 68 81 102 124 138 141 142 151 153 155 156
159 160 161) are excluded from rotate-in consideration, as is deck129 (109 lines) on the same
size ground.
