# Wave-17 synthesis notes — validation table, ranked engine/harness ledger, rotation table, boundary check, guide-application list

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-151559` (21 games round-robin over decks
110/135/17/21/62/49/35). Binary `/tmp/wagic-7cdcf9c73` (wave-16 engine batch: PLAN-carry stale-intent caveat
[319 decisions corpus-wide]; legalCasts normalEntry fix; own-target-only BAD-effect suppression).
Win table: 17:5/6, 110:4/6, 62:4/6, 49:3/6, 21:2/6, 35:2/6, 135:1/6. 0 timeouts, only 3 life-adjudicated
(best yet), 0 segfaults (4th clean corpus). **Fallbacks 3.3% (34/1021) — BEST EVER**: 18 unparsed (HALVED
from wave-16's 40 — the PLAN caveat is the confirmed cause), 13 stale_echo, 3 defer, 0 empty_reply.

THIRD per-deck-rotation cycle. deck44/deck133 rotated out; **deck49 (Dragons)** + **deck35 (Slivers)** entered
GUIDELESS; **decks 17/21** ran REVISED guides (17 froze, 21 revised again); **deck62** ran its FIRST GUIDED
corpus. Unit of analysis is the DECISION with seq repros; layer-routing enforced (engine/harness/card-script
items live HERE, never in guide/core prose). Single-seat items cannot add/cut a core line.

Skill merge → `wave17/strategy-writing-skill.md` (on the wave-16 base): PROMOTED trust-the-rendered-number to a
confirmed Step-0-bis rule (4 seats: 17/49/62/35); ADOPTED the aura opponent-only off-ramp corollary (2 seats:
62/49) and deck35's 0-attacker durdle-signature reviewer instrument (single-seat, justified as a measurement);
WATCH-flagged deck49's target-sub-menu teaching, deck21's countable-cost-quantity scoping, and deck21's auto-no-
block ground generalization. Core → unchanged (12th consecutive PASS).

---

## (a) VALIDATION TABLE — wave-16 shipped fixes / carried items under their at-scale test this corpus

| item | owner seat | verdict | evidence |
|---|---|---|---|
| **PLAN-carry stale-intent caveat** (wave-16 ledger #1, HIGH) | deck110 H1 canary | **RESOLVED — canary PASS; downgrade HIGH→RESOLVED-WATCH** | The self-Galvanic-Blast + 14k carry-vs-prompt spirals DID NOT RECUR. All 4 Galvanic Blasts hit the opponent; **deck21 s8→s9 is the exact-spot REMATCH of the wave-16 blunder** — reasoned the Scuzzback-Scrapper kill, targeted it, fb=None, WON 14/-9. Caveat present on 59/~181 seat decisions, 55 re-derived to a valid on-menu option (~93%). Corpus-wide unparsed HALVED 40→18 consistent. Residual: 4 caveat-present fallbacks, all E6-driven or position rambles (caveat can't rescue a menu bug). |
| **Force #1b free tapped-out alt-cast** (wave-16 UNVALIDATED) | deck135 | **VALIDATED / CLOSED** | deck21 s6: `Mana available: (no untapped sources)`, free exile-pitch offered+chosen, Force put on stack and COUNTERED Volcanic Hammer (target stack→EXILE), no silent fizzle. Exactly the trigger shape wave-16 lacked. |
| **Force #1c hardcast `{1}{u}{u}` offered-while-unpayable** | deck135 | **NOT EXERCISED — carry watch** | Both Force windows offered ONLY the free mode (deck135 held a blue card each time). The wave-16 batch MAY have closed it but unconfirmed. Detect: a chosen `Cast Force {1}{u}{u}` → `deferred_to_heuristic` + zero `Force: hand→stack` with the caster short the cost. |
| **own-target-only BAD-effect suppression** (batch c) | deck110 | **WORKING (validated-by-absence)** | Largely didn't need to fire (opponent targets legal alongside own; model chose opponent, e.g. deck135 s15). No RESOLVED self-destroy corpus-wide. The MIRROR gap (beneficial aura, all-opponent targets) is now ledger E-49c. |
| **legalCasts unpayable-hardcast fix** (batch b) | deck110 | **WORKING** | deck17 s28/s29 Master of Etherium `{2}{U}` non-offers were the fix correctly withholding an unpayable cost. |
| **Hybrid-pip affordability annotation** | deck17 (35 exposures) / deck21 (8) | **WORKING — corpus-heaviest confirmation** | deck17: 0 mis-renders, 13/13 hybrid casts at the correct total across 35 prompts. deck21: 8 casts, 0 declines. Strongest single annotation-effect tally to date. |
| **Attackers name→index reconcile** | deck135 / deck49 | **LATENT-clean** | Mixed `ATTACK: A1, <Name>` parsed correctly both times (deck49 s23/s27): only legal named creatures declared, none dropped. No misfire. |
| **E2 equip outcome-delta marker** | deck110 | **validated-by-absence, 3rd wave** | 10 equip decisions all value-positive by outcome; every `(ALREADY — change NOTHING)` re-offer PASSED; 0 ALREADY taken (Rule #1 100%). Net-zero-DOWNGRADE blind spot got no exercise. Unfixed, dormant. |
| **Crash / target-cancel UAF** | deck21 / all | **CLOSED (4th clean corpus)** | 0 segfaults, 0 target-cancel crashes, 0 empty_reply corpus-wide. |
| **stale_echo superstring matcher** | deck135 / deck110 / deck21 | **CLEAN — 0 wrongful superstring downgrades** | deck135 s22/s23: echoed unoffered "Snow-Covered Forest" correctly NOT matched (not superstrings). The 13 stale_echoes are genuine catches OR the two new matcher/representation items below (E-49a, sub-menu). |

---

## (b) RANKED ENGINE / HARNESS / CARD-SCRIPT LEDGER (layer-routed; guide/core-blind items live here). Rank = frequency x decision-cost x fix-cost.

1. **[NEW · HIGH · 3+ seats, owns most of the corpus's fallbacks] TARGET / REVEAL SUB-MENU represented as a bare
   menu with no effect statement, eligibility marks, or running state.** One engine theme with several members:
   (i) **deck135 ENGINE#4 — Azcanta, the Sunken Ruin** "Choose target N of exactly 4" ordering menu: put-to-hand
   WHIFFED to ZERO across BOTH activations (deck62 s28-30, s36-39 — model picked ineligible CREATURES as the keep;
   all 4 revealed cards → library, nothing to hand). Cost MATERIAL: 6 of deck135's 7 fallbacks + a plausibly-
   decisive vs62 adjudicated loss (beat that deck 19/5 last wave). **⚠ Azcanta may ALSO be a card-script BUG —
   nothing entered hand across two activations even given the model's mis-selection; Oracle-verify the put-to-hand
   scripting BEFORE engine work, per wagicgpt-verify-oracle-text.** (ii) **deck49 E-49b** — the model echoes the
   SPELL name on a target menu instead of the target (deck35 s11, deck17 s13, deck110 s9) or invents "Cast nothing"
   on a mandatory target menu (deck135 s4); drives ALL 7 of deck49's stale_echoes + both unparsed loops. (iii)
   **deck62 N1** — single-target "Choose the target for Web" read as a phase decision (vs135 s18 → out-of-range
   `CHOICE: 0`). (iv) **wave-16 degenerate-label member** (Liliana +1 `Becomes ` stub) is the same contentless-
   target-menu shape. FIX (representation): name the EFFECT in plain terms; MARK which options are ELIGIBLE for the
   beneficial slot (gray out ineligible); collapse/label immaterial ordering steps; show already-assigned picks;
   make target sub-menus visually distinct from phase menus. Owner: `AIPlayerGPT.cpp` option serialization + verify
   Azcanta card-script. This is the highest-leverage remaining representation surface.

2. **[PROMOTED · MEDIUM-HIGH · cross-deck, cheap] E6 — bare mana-ability activation ("Cast Card Normally with Mox
   Opal {Mox's own rules text}") surfaced as a top-level, often SOLE, GPT action.** Fully characterized by deck110:
   a `priority` decision offering to TAP an in-play Mox for 1 mana as a standalone no-op (no cast in progress);
   two label faults (says "Cast Card Normally" for a mana-source tap; dumps the source's reminder text into the
   card-being-cast slot). Seat's #1 fallback driver — 5 fallbacks (deck17 s24/s25/s28/s29, deck49 s34), incl. a
   13.6k ramble where the model correctly DIAGNOSES the mislabel but cannot act. Outcome cost this wave = none
   (staleness caught every one) but each is a wasted round trip. FIX: relabel `Tap Mox Opal: add one mana of any
   color`, OR (stronger, behavior-neutral) SUPPRESS bare mana-ability activations from the GPT priority menu when
   no cast is in progress (auto-tap already covers payment) — never as the SOLE option. Cross-deck (any mana rock).
   NOT guide-fixable. Owner: `AIPlayerGPT.cpp` option serialization.

3. **[NEW · MEDIUM · 2 fatal loops, cheap mirror] E-49c — beneficial aura offered on OPPONENT-ONLY targets should
   be suppressed.** deck17 s18 (12.4k tax) + deck135 s22 (13.8k tax), both fallbacks: Goblin War Paint (+2/+2+haste)
   offered while its only legal targets were opponent creatures → the model looped hunting for a use. The shipped
   own-target-only BAD-effect suppression has a mirror gap: a beneficial aura/pump/haste cast whose legal targets
   are ALL opponent-controlled is never taken and only produces loops. FIX: extend the cast filter symmetrically.
   **Verify Oracle intent first** — War Paint CAN legally target an enemy; this is a "never-useful, don't offer"
   heuristic, same justification as the bad-effect side. Owner: cast-option filter. (Guide off-ramp shipped as the
   interim in deck49/deck62 guides.)

4. **[NEW · MEDIUM (cheap-win) · cross-seat] deck62 N2 — blockers prompt has NO all-none token/example.** vs110 s5:
   a correct no-block decision written "BLOCKS: (no assignments)" was rejected → unparsed → heuristic. The attackers
   prompt has an explicit "ATTACK: none"; the blockers prompt only shows per-blocker `B2:none`. FIX: add an explicit
   `BLOCKS: none` token+example AND accept common paraphrases. Removes a whole unparsed class; cross-seat value at
   every declines-blocks seat (deck21 auto-declines, deck35). Owner: blockers-prompt serialization + parser.

5. **[NEW · LOW-MED · cheap, repro-first] E-49a — stale_echo matcher: player-target "(player, life N)" suffix
   defeats the echo match (WRONGFUL downgrade root).** deck21 s4: option "The opponent (player, life 20)"; reply
   `CHOICE: 1 (Cast Lightning Bolt targeting The opponent)` carries the correct index AND token "The opponent" yet
   routed to stale_echo→heuristic (the "(player, life 20)" suffix breaks the exact/substring match). Non-fatal here
   (index also correct; heuristic bolted face; game won) but a real wrongful downgrade of a correct answer. FIX:
   when matching an echo against a PLAYER-target option, strip/ignore the volatile "(player, life N)" suffix, or
   match on a stable target token. **Reproduce on 7cdcf9c73 before changing the matcher.** Owner: stale_echo matcher.

6. **[CARRIED · MEDIUM · representation] Computed-P/T distrust — the surviving face of the wave-16 "stale within-
   turn mana" family.** deck17's stale within-turn mana LARGELY RESOLVED (the revised guide's trust-menu line held;
   no "why isn't Wasp Lancer listed?" spirals recurred). The surviving instance is deck62 N3: the model re-derives
   Blanchwood Armor's "+1/+1 per Forest" from land count even though the battlefield shows "Saproling (6/6)
   (printed 1/1)" (vs49 s8/s16/s17, the corpus's single largest reply 12,710 chars). Guide trust-line tightened in
   the deck62 revision; the durable lever is to annotate computed P/T as final, e.g. "(6/6 — Forests/auras already
   counted)". Held pending wave-18 re-measure. Owner: option/board serialization.

7. **[CANDIDATE · MEDIUM · needs 2-seat convergence] Blockers/combat prompt combat-OUTCOME annotation.** The block
   seam is a heavy recurring reasoning-tax sink: deck35 (8717/5720/2531/2271/2254-char replies re-deriving first-
   strike/trample/reach, sometimes wrongly), deck17 #3 (block-restriction re-derivation, `[can block: flyers only]`
   candidate), deck110 s5 (reach/flying re-derivation). FIX (if it converges): extend the block/attack option
   annotation with engine-computed OUTCOME per assignment (blocker dies Y/N, damage to defender, trampled-over,
   first-strike ordering). Groups with N2 (no-block token) and deck17's block-restriction annotation into a
   "blockers/combat prompt representation" family. Layer: engine/representation, NOT core. Owner: block/attack
   serialization.

8. **[CARRIED · benign · deck135, 3rd+ wave] Stale `[attacking]` board tag.** Own TAPPED creatures rendered
   `[attacking]` in Main-2 (deck110 s31, deck49 s13/s14); verified NON-stale at all opponent/Blockers instances.
   Drove NO wrong decision. FIX: clear `[attacking]` on any creature not in the current declared-attackers set.
   Companion inverted/absent Treefolk-freeze half UNVALIDATED 4 corpora running (deck135 keeps failing to land
   Treefolk). Owner: board serialization.

9. **[CARRIED · LOW · deck21] Spark Elemental `[treason]` opaque keyword tag** = its end-of-turn sacrifice. Rename
   to a self-describing `[sacrificed at end of turn]` so the model reads the mechanic off the string. Removes a
   standing guide teach-around. Owner: keyword-tag serialization.

10. **[CARRIED · LOW · deck135 ENGINE#3] Fetch chosen-but-unresolved re-ask + duplicate identical option lines.**
    Decision-count inflation (deck21 s3/s4, deck110 s7/s8, deck49 s6/s8/s9). Zero resource loss; guide covers the
    model side. Low priority. Owner: fetch resolution / option de-dup.

11. **[CARRIED · reviewer-facing · harness] translog `options` serialized as an INT COUNT, not the list;** no
    explicit `resolved: battlefield|graveyard|fizzled|countered|exile` field (reviewers scrape the CHOICES block +
    `events`). If cheap: serialize the list + emit `resolved:`. Owner: translog serialization.

**CLOSED / downgraded this wave:** wave-16 ledger #1 PLAN-carry (→ RESOLVED-WATCH, canary pass); Force #1b (→
VALIDATED/CLOSED); own-target-only self-destroy #3 (shipped filter working, no resolved self-destroy — the live
residual is the beneficial-side mirror E-49c). **Dormant-unfixed:** Rakdos's Return X auto-underpay (rotated out
with deck140; re-enters only if an X-face spell exercises the path).

**Cross-seat MODEL watches (route to model-experiments, NOT any guide — guides already maximal):**
- **Reflexive Force on FACE BURN — 3rd corpus (deck135).** deck21 s6 (@19 life) + s9 (@10 life) both countered a
  Volcanic Hammer `[spell] targeting you` = the guide's hardest verbatim NEVER line. Model over-application; a 4th
  restatement would be sanctioned-against escalation. Any seat with a free/cheap counter facing burn is exposed.
- **Card-KNOWLEDGE hallucination (type/cost/color) — cross-seat (deck135).** Azcanta "grabs a creature" (tried to
  put creatures into a noncreature-nonland slot); Treefolk mana-miscount (`{g}{u}{u}`=3 asserted to pay `{2}{g}{u}`
  =4, PASSED three decisions on an unaffordable creature the engine never offered); carried Into-the-North "blue" /
  Flooded-Strand "fetches Forest". Root: reasons from a hallucinated card property instead of the option list.
- **Gemhide auto-tap eating attackers? (deck35, NOT confirmed).** Hypothesis only — deck35's zero-attack losses were
  demonstrably CHUMP-BLOCKING, not mana-tapping. Recorded so it isn't re-derived cold; worth a one-game probe.

**Deck-construction flags (route to the roster/decklist owner — user owns the decklist; restate, don't relitigate):**
- **deck21:** TWO-wave flood pattern (w16 vs133, w17 vs35) — close/winnable games lost on creature-light draws
  (vs35: no creature offered after seq 3, raced my14/op5, got alpha'd). Lever: trim a land / smooth the curve toward
  more 1-2-drop bodies. Escalated from a single witness to a two-wave pattern.
- **deck135:** slow clock + no evasion answer STANDS (4th corpus, Abominable Treefolk the only closer); Force of
  Negation DEAD vs all-creature aggro (vs35: flooded on 2x Force with no noncreature spells to counter); gold/
  double-pip screw off a thin blue/green base (vs21 green-screwed, 0 effective creatures).
- **deck49:** top-heavy Dragon curve — only two {6} finishers, thin 3-cost slot; mana-screwed holding uncastable
  dragons in 2 of 3 losses. Judge the guide by taught-class death + tax collapse, not win-rate.
- **deck35:** heavy on SITUATIONAL lords (3x Striking, 3x Spinneret) vs race-closers (only 2x Shifting, the flat
  win-con); more evasion/pump + fewer redundant Striking/Spinneret would raise the race ceiling. 62 cards / 27 lands.

**Card-script Oracle-verification queue (per wagicgpt-verify-oracle-text — guide uses ENGINE reality; flag for a
maintainer, do NOT block guides):** Azcanta put-to-hand (possible scripting bug — see ledger #1); deck49 —
Bloodmark Mentor (engine 1/1 vs real 2/1), Goblin War Paint (engine {1}{R} +2/+2 vs real {R} +1/+1), Crucible of
Fire (engine {3}{R} vs real {2}{R}{R}), Rorix toughness unconfirmed, Ashenmoor Liege (engine turned the self-harm
drawback into a punisher-on-target); deck21 — Scuzzback Scrapper (engine {R/G} 1/1 no-haste vs real {2}{R/G} 3/1
haste-persist), Boartusk Liege (engine 3/4 trample vs real vanilla 4/4); deck35 — sliver `lord(sliver)` buffs all
slivers incl. opponent's (matches historical Oracle), Striking correctly `mybattlefield`-scoped — no fix needed.

---

## (c) ROTATION TABLE (rotate a veteran out only if its guide takes NO modification AND its seat surfaces NO new work signal)

| deck | guide | new work signal? | verdict | reason |
|---|---|---|---|---|
| **deck17** (Faeries) | FROZEN (byte-identical, cmp-verified) | NO — every taught class DIED or persists-and-obeyed; all residuals are corpus-wide model/representation items shared across decks | **ROTATE OUT (candidate)** | Guide VALIDATED across two consecutive waves (3/6→4/6→5/6); the wave-16 root fixes (walls-aren't-flyers, race branch) fired and REVERSED their causal matchup (vs110 loss→win); hybrid annotation fully load-bearing at the corpus's heaviest exposure (35). No deck-specific new work. Converged exemplar; if kept, next-corpus watch is purely confirmatory. |
| **deck110** (Etched Affinity) | FROZEN (10th, byte-identical) | YES — E6 (fully characterized, OPEN) | **HOLD (near-rotation)** | H1 canary PASS (self-blast class resolved), E2 validated-by-absence 3rd wave. The only live signal is E6, now fully diagnosed with repros — the work remaining is the `AIPlayerGPT.cpp` fix, not seat analysis. deck110 is also the canonical Galvanic + equip-outcome-delta regression validator. CLEAN rotation once E6 is fixed AND a post-fix corpus confirms the Mox menu is gone with no new class. |
| **deck135** (Modern Snow) | FROZEN (13th, byte-identical) | YES — ENGINE#4 (NEW, HIGH) + closed #1b + open #1c/#2-freeze | **KEEP** | Surfaced the HIGH-value Azcanta reveal-and-order representation defect (material cost) on the charge it was assigned; CLOSED the carried #1b tapped-out free-cast validation. Designated owner of both the Force seam and the reveal-and-order seam; #1c unexercised, #2 freeze-half unvalidated a 4th corpus, #4 needs its fix confirmed. Rotating it out orphans those loops. |
| **deck21** (Goblin Gang) | REVISED (POWER floor + kill-engine sharpen) | YES — guide modified | **KEEP** | Real evidence-backed revision (the POWER FLOOR closing the w16 exception's 0-power-flyer over-application that cost the vs62 loss by 1 life). Needs next-corpus validation: does the floor stop the misfire WITHOUT blunting a legit power-1+ flyer pick, and does the kill-on-sight-engine sharpening make the model kill a legibly-rendered token-maker (re-run vs 49/62)? Record floor is structural (flood variance) with clean core execution. |
| **deck62** (Enchantresses) | REVISED (surgical, first-guide) | YES — guide modified + engine items | **KEEP** | First-guide validation: tax collapse UNAMBIGUOUS (17→3 fallbacks, 14→1 giant replies). Surgical revision (Blanchwood printed-P/T trust-line; "never name a creature absent from the A-list"; aura-section trim). New-work signal: the MUTATED eligibility class + engine items N1 (sub-menu), N2 (no-block token). Wave-18 must confirm the Blanchwood tax + summoning-sick over-attack drop. |
| **deck49** (Dragons) | NEW initial guide | YES — newcomer | **KEEP** | Guideless first pass produced its initial guide (mana-trivial rule-1 + target-sub-menu rule-2 + capability table + burn-to-face + aura off-ramp + attack floor). Hypothesis to validate next corpus (Step 0-ter): do C1 mana-belief / C2 target-sub-menu / C4 War-Paint loops / C5 dead-Crucible die, and does the cast/target tax collapse? |
| **deck35** (Slivers) | NEW initial guide | YES — newcomer | **KEEP** | Guideless first pass produced its initial guide (rule-#1 anti-durdle countermand + capability table with race-closer/situational split + haste/evasion sequencing + trust-the-rendered-lord-stack + rarely-block). Validate next corpus: does the defensive-spiral class die and the 0-attacker-game signature disappear? |

**Rotation this cycle:** ONE veteran rotates out — **deck17** (converged exemplar; guide stands live, seat leaves
active re-review). deck110 HOLDs one more cycle as the E6 regression witness. Per the anchor, the next entrant from
the queue is **14 Deep Blue**; remaining queue after it: **27 Zombies, 102 Tergrid-1v1-guide**. (If E6 lands before
wave-18, deck110 becomes a clean rotate-out after the post-fix validation corpus, which would open a second slot.)

---

## (d) BOUNDARY CHECK — git status + freeze/identity verification

- **git status (`git status --porcelain` in `~/Projects/wagicGPT`):**
  ```
   M projects/mtg/bin/Res/test/lexicon/_tests.txt
   M projects/mtg/bin/Res/test/lexicon/test_primitives.txt
  ?? HANDOFF_CARD_SCRIPT_LEXICON_TESTS.md
  ?? projects/mtg/strategy-design/wave17/
  ```
  The wave-17 review touched ONLY `projects/mtg/strategy-design/wave17/**` (untracked). **The two modified lexicon
  test files + the `HANDOFF_CARD_SCRIPT_LEXICON_TESTS.md` are PRE-EXISTING, UNRELATED card-script-lexicon work — NOT
  part of this wave; the guide-application step must NOT stage or touch them.** No reviewer touched `bin/Res/ai/baka/`
  or any engine/source file. Boundary CLEAN for the review.
- **⚠ CORRECTION to the wave brief's assumption:** `bin/Res` is **NOT** gitignored in this repo — the strategy
  guides are TRACKED (`git ls-files` lists all `deckN_strategy.txt`). Only `CLAUDE.md` is excluded (`.git/info/
  exclude`). So guide application is a NORMAL edit of tracked files; the two NEW guides (deck49/deck35) become new
  tracked files on `git add`. **NO `git add -f` is needed.** CLAUDE.md must NEVER be staged (unchanged rule).
- **Veteran freeze verification** (`cmp` of each frozen `wave17/deckN/strategy.txt` vs live `bin/Res/ai/baka/
  deckN_strategy.txt`): all three **BYTE-IDENTICAL** — deck110 (7931 B), deck135 (13357 B), deck17 (5909 B). Freezes hold.
- **Revised guides (vetted change-by-change vs live):**
  - **deck62** (live 5750 B → revision 5997 B): matches its stated rationale exactly. (1) Blanchwood recount-ban —
    the printed-[X/Y] section now cites the "Saproling (6/6) (printed 1/1)" annotation and forbids recounting Forests/
    anthems. (2) A-list-only attacks — the "creatures that can attack (A1, A2, ...)" line is named the EXACT eligible
    set with "NEVER name a creature absent from the list" + "ATTACK: none" when empty/0-power. (3) Aura-section trim —
    the own-vs-opponent targeting reworded to lean on the shipped "only legal targets are YOUR OWN" annotation, and the
    Fists enemy-enchant nuance dropped. **VET PASS.**
  - **deck21** (live 8539 B → revision 10259 B): matches its stated rationale. (1) POWER FLOOR on the race-math
    exception ("do NOT burn a 0-POWER flyer … Birds of Paradise deals 0"). (2) kill-on-sight ENGINE sharpening ("an
    ENGINE that makes a body/token every turn — kill it EARLY even at 1/1, 'no immediate advantage' is a trap").
    Supporting corpus-observed additions, all consistent with the seat's findings (NOT unexplained): a toughness-
    reading note ("6/1 is 6 POWER but 1 TOUGHNESS" — the damage-vs-toughness watch), an auto-attack/auto-decline-
    blocks note (the seat's 1-block-decision structural fact), a trample gang-block note, and a Cranial Plating wall
    example (opponent-side observed). **VET PASS**, with a NOTE: the guide GREW again (w16 was already flagged "on the
    longer side; next revision should trim"); it is still in the 4-14 KB band but is now the longest revised guide —
    flag for a trim once the POWER-floor exception validates.
- **New guides (format-vetted — band 4-14 KB, no heuristic-score refs, no engine-item prose leaking):**
  - **deck49 Dragons** (5407 B / 64 L): mono-red aggro; rules-correct per findings (Dragonmaster Outcast engine at
    6+ lands, Crucible dead with no Dragon, burn-to-face default, War Paint own-creature + opponent-only off-ramp,
    Ashenmoor {B/R} pays red). Embodies the promoted trust-mana rule-1, the target-sub-menu WATCH rule-2, and the
    aura off-ramp. Token scan CLEAN. **VET PASS.**
  - **deck35 Slivers** (5733 B / 75 L): sliver swarm race; capability table with race-closer/situational SPLIT
    (Heart/Shifting/Winged/pump/Fury vs Striking/Spinneret), haste+evasion sequencing, drop-pump-before-combat,
    trust-the-rendered-lord-stack, rarely-block. Embodies the anti-durdle rule-#1 countermand + the promoted trust-
    number rule. Token scan CLEAN. **VET PASS.**
- **general-strategy / core:** UNCHANGED — 12th consecutive PASS. No 2+-seat core-wording candidate; every core-
  adjacent proposal is single-seat and/or a representation/engine item (routed to the ledger). No `general-strategy.txt`
  copy is produced this wave (no change to verify); the live core `bin/Res/ai/gpt/system_prompt.txt` stands.

---

## (e) GUIDE-APPLICATION LIST (exact file operations for the orchestrator)

Working dir: `~/Projects/wagicGPT/projects/mtg`. Source = `strategy-design/wave17/deckN/strategy.txt`;
destination = `bin/Res/ai/baka/deckN_strategy.txt`. All destinations are TRACKED files (no `-f`); the two new
guides become new tracked files on `git add`. Do NOT stage the pre-existing lexicon test changes or CLAUDE.md.

**COPY (2 revisions):**
```
cp strategy-design/wave17/deck62/strategy.txt bin/Res/ai/baka/deck62_strategy.txt   # 5750 -> 5997 B
cp strategy-design/wave17/deck21/strategy.txt bin/Res/ai/baka/deck21_strategy.txt   # 8539 -> 10259 B
```
**CREATE (2 new guides):**
```
cp strategy-design/wave17/deck49/strategy.txt bin/Res/ai/baka/deck49_strategy.txt   # NEW, 5407 B
cp strategy-design/wave17/deck35/strategy.txt bin/Res/ai/baka/deck35_strategy.txt   # NEW, 5733 B
```
**NO-OP (3 freezes — already byte-identical, do NOT copy):** deck110, deck135, deck17.

Post-apply verification:
```
for d in 62 21 49 35; do cmp strategy-design/wave17/deck$d/strategy.txt bin/Res/ai/baka/deck${d}_strategy.txt \
  && echo "deck$d applied OK"; done
```
