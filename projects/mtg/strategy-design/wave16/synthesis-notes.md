# Wave-16 synthesis notes — validation table, ranked engine/harness ledger, rotation table, boundary check

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-084532` (21 games round-robin over decks
44,110,133,135,17,21,62). Binary `/tmp/wagic-69793930a` (wave-15 engine batch: free zero-mana alt-casts, the
stale_echo option-subset-of-echo superstring matcher, the hybrid-pip affordability annotation). Spark @0.75.
Win table: 133:4/6, 17:4/6, 110:4/6, 44:4/6, 62:2/6, 21:2/6, 135:1/6. 0 raw timeouts, 4 life-adjudicated
(down from 9-13 — games completing), 0 segfaults (crash-fix's 2nd clean corpus), 0 empty_reply. Fallbacks ~5.2%.

SECOND per-deck-rotation cycle. deck140 rotated out; deck62 (Enchantresses) entered GUIDELESS; decks 17 and 21
played their FIRST GUIDED corpus. Unit of analysis is the DECISION with seq repros; layer-routing enforced (engine/
harness/card-script items live HERE, never in guide/core prose). Single-seat items cannot add/cut a core line.

Skill merge → `wave16/strategy-writing-skill.md` (on the wave-15 base): adopted the FIRST-GUIDE VALIDATION doctrine
as Step 0-ter (2-seat, 17+21), deck62's four engine-deck Step 0-bis refinements (single-seat, tagged), and the
review-discipline "verify the premise" headline (110 mulligan + 44 corpus-wide-verification); WATCH-flagged the
degenerate-label third stale_echo cause (deck133). Core → `wave16/general-strategy.txt`, byte-identical verbatim
copy of the live core (PASS, 11th consecutive).

---

## (a) VALIDATION TABLE — shipped fixes under their at-scale test this corpus

| fix | owner seat | verdict | evidence |
|---|---|---|---|
| **Galvanic Blast metalcraft** | deck110 | **CLOSED — 11/11 both arms** | 11 casts, both arms correct: 2 at N<3 (deck133 s16, deck44 s14), 4 at N≥3 (9 casts). Read by COUNTING `Damage:` INSTANCES — the +2 ships as a 2nd resolution instance, so a working Blast logs two identical `Damage:2` lines; a string-dedup grep mis-reports it as "2" (trap re-hit and re-confirmed). The wave-13 flat-2 card-script bug stays closed. |
| **Attackers name→index reconcile** | deck110/135/44 | **VALIDATED** | 17 attacker windows audited at deck110: every eligible named creature unioned in (MIXED `A1,<name>` + pure-index), every summoning-sick name EXCLUDED (deck135 s6 `A1, Steel Overseer` → only Signal Pest kept), 0 eligible dropped. **Reconciles the wave-15 deck44 FALSE "combat asks removed" finding: corrected read confirms 30 attacker + 14 blocker asks at deck44** (the LLM declares combat normally). |
| **stale_echo superstring fix** | 133/135/21/44 | **CLEAN — 0 wrongful downgrades; positive Yawgmoth witness STILL PENDING** | 0 wrongful superstring downgrades at any seat. All stale_echos this corpus were GENUINE catches (deck135 s33 & deck44 s18: echo names an UNOFFERED land type "Forest" — not a superstring, correctly not matched; the brief's s33 "vs21" is a mislabel, it is vs133). The exact Yawgmoth-attack shape got NO window (deck133 had no attack-with-multiword-creature ask) → positive-match confirmation carries forward. |
| **Free-Force-at-zero-mana (#1b)** | deck135 | **UNVALIDATED** | No tapped-out exercise. The ONE free alt-cast that fired (deck21 s19, countered Volcanic Hammer, stack→exile — resolution re-confirmed) had `{u}{u}{c}` UP, so it does not test the tapped-out path. The tapped-out Force windows offered the HARDCAST (no blue card to pitch), not the free alt-cast. NOT-EXERCISED ≠ NOT-FIXED — validation TODO carried. |
| **Hybrid-pip affordability annotation** | 21/44/17 | **WORKING (cross-seat positive)** | deck21: 7 hybrid casts, 0 declines (wave-15 had 1 misread). deck44: 6/6 correct + one black-screw rescue (cast {u/b} Gatewarden off all-Islands). deck17: the hybrid off-color CLASS DIED (mutated to stale-within-turn-mana, ledger #4). deck110: no material exercise (mono-cost seat). **Candidate to shrink the per-guide HYBRID paragraph to one line** (17/21 already trimmed). |
| **Crash fix (target-cancel UAF)** | deck133 | **CLOSED (2nd clean corpus)** | 0 segfaults, 0 empty_reply corpus-wide (deck133 5th clean corpus for the Spark-death class). |
| **E2 equip churn (outcome-delta lever)** | deck110 | **validated-by-absence** | 0 A→B→A churn; the one MOVE (deck135 s15 Signal Pest→Steel Overseer) was a genuine UPGRADE (7→9 power, tougher body) the outcome-delta lever would ALLOW; 0 `(ALREADY)` options taken. Rule #1 obeyed 100%. Lever unfixed but correct — dormant. |
| **Own-target-only removal annotation (Item A)** | deck133 / deck44 | **30/31 obeyed BUT the class RECURRED** | deck133: the `only legal targets are YOUR OWN` Fatal Push offered ~20×, declined 20/20. **BUT deck44 re-witnessed a RESOLVED self-destroy** (vs135 s21: chose the own-only Go for the Throat, killed its own Faerie Miscreant; present-and-read-and-disobeyed; low outcome cost — won game). See the adjudication below. |

**ADJUDICATION — own-target-only suppression lever (deck133 says "downgrade to parked WATCH"; deck44 says
"reaffirm").** The lever stays an **ACTIVE engine build-candidate (MEDIUM)**, NOT parked. deck133's 0/20 measures
OBEDIENCE of the Fatal-Push annotation at its own seat; it does not observe deck44's breach. The class produced a
RESOLVED self-destroy in each of the last two corpora, at different seats on different cards (wave-15 deck21 s23
self-Fatal-Push; wave-16 deck44 s135-s21 self-GFTT), each despite a present-and-read annotation — so the annotation
REDUCES but does not ELIMINATE the class, and its residual failure rate is exactly what a structural filter (extend
601.2c to "no legal hostile target for a mandatory-target removal spell") drives to zero. It is MEDIUM (not HIGH)
because the outcome cost has been consistently LOW (won / non-deciding games). Keep it on the ledger as a build
candidate (ledger #3); re-escalate to HIGH only if a self-destroy resolves in a DECIDING window.

---

## (b) RANKED ENGINE / HARNESS / CARD-SCRIPT LEDGER (layer-routed; guide/core-blind items live here)

1. **[NEW · HIGH · outcome-costing, cleanest repro deck110 H1] The `YOUR PLAN (as you last stated it)` carry
   re-injects STALE reasoning.** Every decision prompt echoes the model's PREVIOUS full PLAN prose; when the state
   has advanced past it (card already cast, phase/menu changed, or the prior plan ended mid-"Wait…"), the carry
   actively harms. **Clean causal blunder (deck21 s6→s8, a LOSS):** after casting Steel Overseer (s5), the carried
   plan still read "cast Steel Overseer … NOT cast Galvanic Blast"; s6 the model emitted the Blast (the action its
   prose rejected), s7 picked the NAME-matching target = its OWN Overseer, s8 the metalcraft Blast (2+2=4) killed its
   own just-cast creature — a self-2-for-1 in a race it lost. Name-echo did NOT catch it (the echoed string is a real
   offered option). Also seeded the two ~14k-char `unparsed_reply` spirals (deck21 s13 carry describes an Upkeep menu
   vs the current Main menu; deck133 s9 carry asserts `{r}` vs the current `{r}{w}`) and the prose-vs-CHOICE
   divergences (deck135 s15). Cross-deck (every seat carries a plan). **Fix direction:** when the carried plan's
   named action is NOT on the current menu (state advanced), DROP/truncate the carry — or carry only the last
   resolved `CHOICE:` line, not the rambly PLAN incl. its "Wait…" tail. **Weigh against the plan-continuity design**
   (the carry exists to preserve intent across decisions): a menu-membership check on the carried plan's named action
   is the minimal cure that keeps continuity where the plan is still live and only drops it when stale. Fixing this
   should also cut the corpus unparsed-loop rate. Owner: `AIPlayerGPT.cpp` prompt assembly.

2. **[HIGH · engine, deck135 — the Force seam has TWO payability holes with one cure] Force offer-legality does not
   check payability.** (#1b) The FREE alt-cast (exile a blue card, zero mana) fizzles when tapped out — carried from
   wave-15, UNVALIDATED this corpus (no clean trigger). (#1c NEW) the HARDCAST `{1}{u}{u}` is OFFERED while
   genuinely unpayable, chosen, then silently defers/fizzles: deck110 s8 (mana `{g}`, target Cranial Plating), s16
   (`{g}`, Galvanic Blast), s25 (`{g}{u}{c}` = one U short, Galvanic Blast) — in each, deck135 could make neither
   `{1}{u}{u}` nor the exile alt-cost (no blue card in hand), yet Force was listed; 0 `Force: hand→stack` in either
   log; the targets resolved. **Same root, same cure:** before listing ANY Force cast option, verify payability of
   that mode (mana producible for the hardcast, OR a legal blue card exists for the exile alt-cost); offer neither
   mode if neither is payable. Owner: engine cast/alt-cost offer path.

3. **[MEDIUM · engine, adjudicated — 44+133] Suppress a top-level targeted-removal cast whose ONLY legal targets are
   friendly.** Extend the 601.2c no-legal-target filter to "no legal HOSTILE target for a mandatory-target removal
   spell," making the self-destroy structurally impossible. Recurred as a RESOLVED self-GFTT at deck44 vs135 s21
   despite the present-and-read annotation (30/31 obeyed but the 1 breach RESOLVED; 2 resolved self-destroys across
   the last 2 corpora). Low outcome cost so far → MEDIUM. Owner: cast-option 601.2c-style filter. Secondary
   model-watch: stack-vs-battlefield removal-targeting confusion (the model believed GFTT could hit a creature on
   the stack).

4. **[MEDIUM · representation, deck17 — successor to the now-dead hybrid off-color tax] Stale WITHIN-TURN mana
   tracking.** On a SECOND cast in the same main phase the model recounts mana from its battlefield land total,
   ignores the explicit `Mana available: N from K untapped sources` line, and distrusts the (correct) cast list —
   6-9.6k-char replies (deck17 vs62 s13 rl6732, vs110 s5 rl9648): "why isn't Wasp Lancer listed?" when the card was
   already paid for. The prompt is correct; the model overrides it. **Representation levers:** tag the cast list
   header ("already accounts for mana spent this turn"), or narrate a consumed same-turn cast as a mana delta ("you
   cast Scion this turn; 1 mana remains"). Guide trust-line SHIPPED in the deck17 revision; representation is the
   durable fix. Promotion trigger = a 2nd seat's "why isn't X listed?" oscillation. Owner: option-line / mana
   narration serialization. (Converges with deck62 S4 "trust the printed [X/Y]" into a TRUST-THE-RENDERED-NUMBER
   representation family.)

5. **[MEDIUM · dormant, deck110 E2] Equip re-offer no-op marker is TARGET-IDENTITY-keyed, blind to net-zero MOVES.**
   Validated-by-absence this corpus (no churn; the one move was an upgrade the lever would allow). Unfixed. **Fix:**
   compute the marker from OUTCOME DELTA — suppress "Equip … targeting X" when the move would not raise X's resulting
   power above its current carrier's. Owner: `AIPlayerGPT.cpp` equip-option serialization.

6. **[LOW-MED · representation, open, deck110 E6] "Cast Card Normally with Mox Opal" mislabel.** The cast-with-
   mana-source label templates the Mox's OWN ability text into the card slot. NOT exercised this wave (all Mox
   interactions were plain `Cast Mox Opal`). Ledger stays open. **Fix:** show the CARD/action, not the rock's text.
   Owner: cast-with-mana-source serialization.

7. **[LOW · NEW harness/serialization, deck133 — single-seat WATCH] Degenerate option label `Becomes `.** A Liliana,
   the Last Hope `+1` target sub-menu serialized as `['Becomes ', 'Decline - do nothing']` (no creature, no stats),
   so a correct echo could not match → stale_echo → heuristic (fired the +1 anyway; non-deciding, deck133 vs62 s29).
   The THIRD stale_echo downgrade cause (leak / superstring / degenerate-label) — upstream of the matcher; do not
   loosen the matcher. **Fix:** emit the full target-menu label (creature + resulting P/T). Cross-seat (any
   Liliana-the-Last-Hope +1 / stat-change target menu). Owner: option serialization.

8. **[carried · benign · 2 seats] Stale `[attacking]` board tag.** Reproduced at deck135 (deck133 s36, deck62 s29,
   deck44 s34/s35 — own TAPPED creatures tagged `[attacking]` in Main phase 2, no active combat); deck110 s23 was a
   wave-15 2nd-seat witness. 3rd wave, drove NO wrong decision. **Fix:** clear `[attacking]` on any creature not in
   the current declared-attackers set. Companion INVERTED-FREEZE half (a frozen creature annotated `untaps next
   turn`) UNVALIDATED 3 corpora running (no non-terminal Treefolk-freeze window). Owner: board serialization.

9. **[OPEN · dormant-unfixed — ROTATED OUT with deck140] Rakdos's Return X auto-underpay.** Cast 0 times before
   deck140 left the pool; the underpay was never reproduced or cleared. Stays OPEN as a dormant-unfixed item; needs a
   corpus where a lethal Rakdos actually fires (re-enters the active ledger only if deck140 returns or another X-face
   spell exercises the path). Owner: X-payment path.

10. **[reviewer-facing · harness · 2nd-seat witness] translog `options` serialized as an INT COUNT, not the list.**
    Re-confirmed at deck44 (every record) + deck133 (`options`=3 while `options_text` holds the list) — reviewers had
    to scrape the `prompt` CHOICES block. Also still no explicit `resolved: battlefield|graveyard|fizzled|countered|
    exile` field (deltas scraped from `events`). If cheap: serialize `options` as the list + emit `resolved:`. Owner:
    translog c2 serialization.

**Cross-seat MODEL watches (route to model-experiments, NOT any guide):**
- **Force reflexive-cast drift is WORSENING** (deck135) — from wave-13's perfect decline to casting all offers, now
  onto FACE BURN, the guide's hardest verbatim "never" line: deck21 s19 Forced a Volcanic Hammer aimed at its own
  face (exiled a needed Ice-Fang Coatl, died the same turn anyway) + deck110 x3 Forced Galvanic Blast / Cranial
  Plating (all non-sanctioned). The guide scopes Force correctly → model over-application. If other Force-carrying
  seats show the same drift, it is a model-behavior regression.
- **Card-color / fetch-color hallucination** (deck135) — Into the North (green {1}{g}) called a "blue card" for the
  Force alt-cost; Flooded Strand believed to fetch a Forest (Plains/Island only); same family as the damage-vs-
  toughness gap (the model invents a fixed card property to justify a play).
- **Summoning-sickness untracked in model lethal math** (deck110 M1a, deck135, deck62) — the model names same-turn
  creatures as attackers and folds them into its lethal total; the reconcile excludes them so no wrong attack
  occurs, but the arithmetic is inflated. Latent if it ever gates a lethal / hold-back. Mitigation if it bites:
  board-line tag `[can't attack yet — summoning sick]`.
- **damage-vs-toughness gap** (carried, deck135 s27 / deck110 / deck21 F1) — wasted burn on a 6/6 believing 3 damage
  shrinks/kills it.
- **Uncastable-cast hallucination under color-screw** (deck44 vs62) — repeatedly PLANNED to cast a black spell on an
  all-Islands board; the harness refused every instance (latent only).

**Deck-construction flags (route to the roster/decklist owner — user owns the decklist; restate, don't relitigate):**
- **deck135:** manabase strands its own gold / double-pip green creatures (vs21: 0 creatures cast, green-screwed,
  dead T9); NO evasion answer (vs44, 3rd corpus, -4 with SIX creatures deployed vs an evasive 6/6); slow clock
  (Abominable Treefolk the only real closer). Direction: raise effective double-pip green sources, add an evasion
  answer.
- **deck44:** TWO-SIDED color-consistency problem — 16 Island / 10 Swamp must serve both `{U}{U}` counters AND
  `{b}` creatures/removal, so either color can be the pinch (wave-15 blue-screw; wave-16 black-screw = a LOSS, Obyra/
  GFTT/Bladecrafter dead in hand). Levers: more flexible/hybrid fixing; 1-2 cheap flyers to raise the aggro floor
  (the reactive-heavy no-clock keep is the recurring loss signature, vs110 died with ~1 power of clock).
- **deck133:** self-damage suite (Thoughtseize 2 / Phyrexian Arena / fetch) is disproportionately costly under
  life-adjudication (vs44 loss: 20→8 on 3× Thoughtseize + fetch vs a control deck that reached only 18, no closer
  drawn); no reach / no flyer answer except Gray Merchant (vs17: dealt ZERO damage all game); no early blocker.
  Levers: 1-copy Thoughtseize→Inquisition shift, cut Phyrexian Arena, add a cheap early blocker.
- **deck21:** floods (20 Mountain + top-heavy anthem/lord curve; vs133 lost 2-2 with 0 creatures T5-T13 on an
  all-Mountain draw). Lever: trim a land / smooth the curve toward more 1-2 drops.

---

## (c) ROTATION TABLE (rotate a veteran out only if its guide takes NO modification AND its seat surfaces NO new
work signal)

| deck | guide | new work signal? | verdict | reason |
|---|---|---|---|---|
| **deck44** (UB Faerie Tempo) | FROZEN (12th, byte-identical) | NO (self-GFTT is a REAFFIRMATION of shared ledger #3, not a new seat-only item) | **ROTATE OUT** | Reviewer-recommended; conditions met. The wave-15 corrected-read charge is discharged (30 attacker + 14 blocker asks confirmed; the false "combat asks removed" refuted), 4/6 with clean pilot decisions, losses are construction/variance. The frozen guide stays live; the seat leaves active re-review. |
| **deck133** (Ph. Asphodel) | FROZEN (11th+, byte-identical) | thin — only new item is the degenerate-label stale_echo, which is CROSS-SEAT harness, not seat-specific | **ROTATE OUT** | Adjudicated (reviewer leaned out). Item A recurred 0/20 at its seat (parked-clean), item B validated-by-absence, self-FP watch clean. The Yawgmoth positive-witness argument is a harness-validation reason, not a guide reason, and any future corpus with a Yawgmoth attacker catches it; the degenerate-label nit travels to whatever seat next hits a Liliana +1 menu. No deck-specific new work signal → out. |
| **deck110** (Etched Affinity) | FROZEN (9th, byte-identical) | YES — H1 (NEW/HIGH outcome-costing harness repro) | **HOLD (H1 canary)** | Now a HARNESS-CANARY seat: it keeps surfacing novel prompt-assembly bugs with outcome coupling (H1 self-blast), and is the canonical at-scale validator for Galvanic (11/11) and the equip outcome-delta lever. Clean rotate-out only once H1 + E2/E6 are worked and a subsequent corpus surfaces no new class. |
| **deck135** (Modern Snow) | FROZEN (13th, byte-identical) | YES — ENGINE #1c (NEW hardcast-Force-offered-unpayable) on the Force seam it OWNS; #1b still unvalidated | **KEEP** | The Force seam is the project's most active engine surface with two open items (#1b unvalidated, #1c new) + an unvalidated ENGINE #2 freeze half. Rotating it out would orphan the exact validation loop it exists to run. |
| **deck17** (Faeries) | REVISED (first-guide validation) | YES — revised (Class-6 walls-aren't-attackers root fix, Class-4 steal-early sharpen, race off-case branch, dead-section trims) | **KEEP** | First guide VALIDATED (3/6→4/6) and produced a substantive revision; re-validate next corpus on the two has-flyer matchups (mirror, lifelink) + whether the stale-mana tax dropped. |
| **deck21** (Goblin Gang) | REVISED (first-guide validation) | YES — revised (E1 race-math flyer exception, reconciled with the over-broad matchup section) | **KEEP** | First guided corpus; taught classes DIED (hold-burn, Spark timing, haste roster, hybrid — all dead) and the tax collapsed; the one real signal (E1) earned a revision needing validation vs 17/44 (the two by-4 losses) without over-applying vs non-flyer decks. |
| **deck62** (Enchantresses) | NEW initial guide | YES — newcomer, guide shipped | **KEEP** | Guideless first pass produced its initial guide (capability table + engine-sequencing + aura-targeting + attack floor); a hypothesis to validate — check the hallucinated-removal class, enchantress mis-sequencing, and the 10k-char combat tax all drop next corpus. |

**Replacements (2 seats rotate out → 2 enter guideless first-pass, in queue order):**
- deck44 → **deck49 (Dragons)** — GUIDELESS first pass, Step 0-bis doctrine.
- deck133 → **deck35 (Slivers)** — GUIDELESS first pass, Step 0-bis doctrine.

Remaining queue (unchanged order): 14 Deep Blue, 27 Zombies, 102 Tergrid-1v1.

---

## (d) BOUNDARY CHECK — git status + freeze/identity verification

- **git status:** the ONLY untracked entry is `?? projects/mtg/strategy-design/wave16/`. **No reviewer touched
  `bin/Res/` or any engine/source file.** Boundary CLEAN.
- **Veteran freeze verification** (`cmp` of each `wave16/deckN/strategy.txt` vs live `bin/Res/ai/baka/deckN_strategy.txt`):
  all four BYTE-IDENTICAL — deck44 (12084 B), deck110 (7931 B), deck133 (8960 B), deck135 (13357 B). All freezes hold.
- **Revised guides (vetted change-by-change):**
  - **deck17** (live 6319 B / 85 L → revision 5909 B / 80 L): net TRIM. Every trimmed section corresponds to a
    validated-DEAD class (defender-attack Class 2, Cloud-Sprite-block Class 5, the redundant "SITUATIONS THAT DECIDED
    GAMES" recap). Every ADDITION corresponds to a corpus-shown failure: the "walls are not attackers / biggest flyer
    THAT CAN ATTACK" deployment section (Class 6, root cause of BOTH losses); the "trust the menu, do not recount
    your mana" line (Class-1 mutation / ledger #4); the sharpened anthem-lord steal-early clause with the vs44
    witness (Class 4); the "WHEN THE OPPONENT ALSO FLIES OR GAINS LIFE — YOU ARE IN A RACE" off-case branch (WATCH-4,
    both losses). Softened "almost always the beatdown" → "You are the BEATDOWN" per the off-case. **VET PASS.**
  - **deck21** (live 6735 B / 99 L → revision 8539 B / 119 L): net GROWTH, justified. The core addition reworks the
    kill-the-flyer EXCEPTION into an explicit RACE-MATH exception (the no-blockers cumulative math, V2; the
    early-or-never timing, V3; the "do not over-apply — ONE early burn" scoping, V1) and SPLITS the over-broad
    "matchups you cannot solve" section into BIG-wall vs SMALL-flyer-swarm (V1 — the exact over-broad countermand
    that lost both by-4 races). Minor corpus-observed additions: "never cast an anthem into an empty board" and
    "deploy first" on Bloodmark (the vs133 empty-board Oriflamme waste); a hybrid-annotation note (F5 dead). Every
    change corresponds to the validated E1 signal or a corpus-observed waste; the growth is the earned first
    revision (the race-math needs the mechanical explanation, not a one-line evocation). **VET PASS** (on the longer
    side; next revision should trim once E1 validates).
- **deck62 new guide** (73 lines / ~3.9 KB): rules-correct per its findings (Fists = 2 Saprolings + trample on the
  enchanted creature, no damage; Ancestral Mask +2/+2 per OTHER enchantment; Argothian draws + SHROUD; the enemy-
  enchant carve-out for Fists is nuanced-correct); embodies all four S1-S4 refinements (capability table as #1;
  cast-Argothian-first sequencing; attack floor naming 0-power engine/dork exclusions + the summoning-sick-already-
  dropped note; trust-the-printed-[X/Y]); no core-restating beyond deck-scoped framing; actionable. **VET PASS.**
- **general-strategy.txt:** byte-identical copy of the live core `bin/Res/ai/gpt/system_prompt.txt` (19742 B) — core
  PASS, verbatim (11th consecutive).
