# Wave-18 synthesis notes — validation table, ranked engine/harness ledger, rotation table, boundary check, guide-application list

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-185930` — 21 games round-robin over decks
110/135/21/62/49/35/14. Binary `/tmp/wagic-f664539a3` (wave-17 engine batch: (a) TARGET CHOICE sub-menu framing;
(b) hand-card PUT_INTO_PLAY dead-end filter = Mox E6 fix; (c) beneficial opponent-only-target cast suppression =
E-49c; (d) explicit "BLOCKS: none" example + paraphrase catch; (e) "(player, life N)" echo suffix strip).
Win table: 35:4/6, 135:3/6, 21:3/6, 110:3/6, 14:3/6, 49:2/6, 62:2/6 — flattest table yet. 1 draw/timeout,
2 life-adjudicated, 0 segfaults (5th clean corpus). **Fallbacks 2.2% (25/1146) — NEW BEST**: 22 unparsed,
2 stale_echo, 1 defer, 0 empty.

FOURTH per-deck-rotation cycle. deck17 rotated out last cycle (first converged exemplar). **deck14 (Deep Blue)**
entered GUIDELESS (Step 0-bis → initial guide); **decks 49/35** played their FIRST GUIDED corpus (Step 0-ter, each
produced a revision); **deck62** ran its REVISED guide round 2 (surgical revision); **decks 110/135/21** FROZE
byte-identical. Unit of analysis is the DECISION with seq repros; layer-routing enforced (engine/harness/card-
script items live HERE, never in guide/core prose). Single-seat items cannot add/cut a core line.

Skill merge → `wave18/strategy-writing-skill.md` (on the wave-17 base): PROMOTED the LOSING-POSITION / OFF-CASE
SPIRAL doctrine to a Step-0-ter rung (3 seats — deck62 rule-6 off-case + deck35 forced-lethal escape + deck49
flood off-case, each authored the fix in-guide); PROMOTED the GIANT-REPLY-COUNT tax gauge (4 seats — 62/49/21/35);
SHARPENED "trust the rendered number" to TWO-SIDED (deck35 + deck14 combat face); ADOPTED three method sharpenings
(deck49 concurrent-engine-fix apportionment → rule 5; deck62 name-the-trigger → rule 3; deck21 trim-is-a-separate-
pass) and two measurement instruments (deck49 burn-target-menu face-rate; deck135 repeat-loop signature). WATCH-
flagged deck14's combat FORMAT-DISCIPLINE guide line (single-seat, self-flagged promote-on-2nd-seat; the durable
fix is engine-side). Core → **PASS, 13th consecutive** (see §B).

---

## (a) VALIDATION TABLE — wave-17 shipped fixes / carried items under their at-scale test this corpus

| item | owner seat | verdict | evidence |
|---|---|---|---|
| **TARGET CHOICE sub-menu framing** (wave-17 batch a; ledger #1 last wave) | deck135 / deck49 / deck62 | **RESOLVED — the highest-leverage close of the wave** | The dominant wave-17 fallback surface is GONE. deck135: 8 target decisions, 0 stale_echo (was 2), 8/8 right picks that WON games (Treefolk-ETB→Signal Pest, Gelid Shackles→Master of Etherium). deck49: 0 stale_echo (was 7, ALL at this seam). deck62: 20/20 aura sub-menus parsed to a target name, N1 sub-menu-as-phase-menu misread did NOT recur. Corpus-wide stale_echo 13→2. |
| **Mox E6 — hand-card PUT_INTO_PLAY dead-end filter** (batch b) | deck110 | **RESOLVED (confirmed by absence)** | "Cast Card Normally with Mox Opal {rules text}" = **0 occurrences corpus-wide** (was deck110's #1 fallback driver, 5/7 last wave). The stronger of the two proposed levers shipped and works. No new dead-end shape drives a fallback. |
| **E-49c — beneficial opponent-only-target cast suppression** (batch c) | deck49 / deck135 | **WORKED as a loop-preventer, but MASKS the taught class — and is now REPLACED** | Enemy-only Goblin War Paint was never offered → the fatal wave-17 C4 loops (deck17 s18, deck135 s22) had ZERO recurrence. BUT the class is MASKED, not proven-solved, and the suppression also removed a legal play — both halves of the owner ruling confirmed at one seat. **Owner ruling shipped post-corpus (commit `ed7d579e4`, "Replace strategy-judgment cast suppressions with warning annotations")** → both suppressions become option-line warnings; C4 / War-Paint re-enters and re-validates next corpus under the annotation. |
| **"(player, life N)" suffix strip** (batch e; E-49a) | deck49 / deck110 | **CLOSED (validated)** | deck49: 17/17 burn/player target menus parsed with 0 wrongful downgrades (was the deck21 s4 wrongful stale_echo last wave). The player-target label no longer defeats the matcher. |
| **"BLOCKS: none" example + paraphrase catch** (batch d) | deck62 | **WORKING for the all-decline token; DID NOT extend to name→label ASSIGNMENTS** | Explicit no-block declarations parse. But the batch covered the priority menu / "BLOCKS: none" only — a name-format block ASSIGNMENT still unparsed (ledger #1). |
| **PLAN-carry stale-intent caveat** (wave-16 ledger #1, RESOLVED-WATCH) | deck110 H1 canary | **STAYS RESOLVED-WATCH — canary PASS** | No self-blast, no carry-vs-prompt contradiction loop anywhere. deck110's 6 unparsed = 2 block-echo parse failures + 4 position/state rambles (all in the 3 LOST games). Re-open only on a self-blast or a genuine carry-vs-prompt loop. |
| **Blanchwood recount-ban** (wave-17 deck62 revision; ledger #6) | deck62 N3 | **VALIDATED / RETIRED as a distinct watch** | Recount of already-printed P/T on a body already wearing Blanchwood is GONE; surviving Forest-counts are legitimate PROSPECTIVE cast-value math (body not yet wearing it). The recount tax dropped as planned. |
| **deck21 POWER FLOOR** (wave-17 revision) | deck21 | **VALIDATED on its motivating matchup (LOST −1 → WON +12)** | The identical vs62 / Birds-of-Paradise pairing recurred 4×; the model sent every burn face quoting the new clause verbatim. 0-power-flyer misfire class DEAD, no legit power-1+ flyer pick blunted. |
| **deck21 kill-on-sight ENGINE sharpening** (wave-17 revision) | deck21 | **VALIDATED (fired inside a LOST game)** | Killed the Dragonmaster Outcast on sight vs49 (the exact wave-17 miss) + two engine/lifelink kills vs110 (structural LOSS→WIN). Attribute the DECISION: vs49 lost to an unrelated flood, the kill fired correctly. |
| **deck35 anti-durdle rule-#1 (FC1)** (wave-17 new guide) | deck35 | **VALIDATED DEAD — the 0-attacker instrument's clean win** | 0-attacker games 2/6 → 0/6; both former durdle-losses changed character (49 LOSS→WIN, 62 durdle→race). Multi-anthem stacking caveat RESOLVED (buff-math clean). Residual losses are speed/size construction, not durdle. |
| **deck49 BURN-GOES-FACE + mana-trivial Rule #1 (C1)** (wave-17 new guide) | deck49 | **VALIDATED DEAD (guide-owned, engine-untouched)** | 17/17 face; C1 mana-belief locks 0 (engine never changed mana representation, so Rule #1 owns that kill). Fallbacks 9 → 0 (bulk ENGINE-attributable; apportionment discipline). |
| **Hybrid-pip affordability annotation** | deck21 (~12 exposures) | **WORKING — 4th corpus of confirmation** | ~12 hybrid-annotated cast prompts, 0 declines, 0 wrongful annotation / double-count. Ship-confirmed. |
| **Attackers name→index reconcile** (`d0f88326b`) | deck135 HARNESS#1 | **LATENT-clean** | Mixed `ATTACK: A1, <Name>` parsed correctly at deck135 s17, deck62 s19/s25, deck110 s17, deck14 s31/s36 — no dropped LEGAL attacker. (The BLOCKS side is the gap — ledger #1.) |
| **E2 equip outcome-delta marker** | deck110 | **validated-by-absence, 4th wave (dormant)** | 33 equip decisions, `took_ALREADY=False` on all; every real equip value-positive; sole-option ALREADY menus passed cleanly. Identity-keyed net-zero-MOVE blind spot got no exercise. Unfixed, dormant. |
| **Crash / target-cancel UAF / empty_reply** | all | **CLOSED (5th clean corpus)** | 0 segfaults, 0 target-cancel crashes, 0 empty_reply. |

---

## (b) RANKED ENGINE / HARNESS / CARD-SCRIPT LEDGER (layer-routed; guide/core-blind items live here). Rank = frequency x decision-cost x fix-cost.

1. **[NEW · HIGH · 3 seats, one BITING repro that flipped a winnable game] COMBAT-REPLY NAME TOLERANCE + ELIGIBILITY
   FILTERING across the ATTACK/BLOCKS parsers — one work item.** The blockers/attackers reply matcher accepts only
   the exact `B#:A#` / eligible-A# label form; a legal, correctly-shaped combat declaration written with creature
   NAMES (or naming an ineligible attacker) is rejected WHOLE → `unparsed_reply` → the heuristic makes a different,
   often worse combat decision. Three convergent members:
   (i) **block-echo name→label — BITING (deck110, deck21 s18).** `BLOCKS: Memnite: Boggart Ram-Gang` → unparsed →
   heuristic chumped the 9/1 Memnite into a 1/1 wither (Memnite DIED) and let a 3/3 hit face; deck110 lost −2/16 —
   the wave-17 rematch it had WON clean. Non-biting twin: deck49 s12 `BLOCKS: Ornithopter blocks Dragonmaster
   Outcast`. (ii) **combat FORMAT DRIFT (deck14 E1, 3 discards).** vs62 s34/s37 (8.6k/10.2k-char SENSIBLE blocks
   written with card names) + vs135 s26 (13.1k, decisive LOSS turn, never emitted a clean `ATTACK:`). (iii)
   **attacker-eligibility filter (deck62 N2).** vs14 s21 / vs21 s12 named summoning-sick creatures ABSENT from the
   A-list; the parser rejected the WHOLE line — correct answer both times was `ATTACK: none`. FIX (one item): the
   block/attacker labels already print the NAME beside each B#/A# code, so (a) resolve any unambiguous creature
   name in a `BLOCKS:`/`ATTACK:` line back to its label (the exact analogue of the shipped `CHOICE:` name-echo and
   the attackers name→index reconcile `d0f88326b`), falling back to code-only when a name is ambiguous (duplicate
   names — the known gang-block disambiguation guard), AND (b) DROP named-but-ineligible attackers and attack with
   the eligible SUBSET (empty subset = `ATTACK: none`) instead of unparsing. Proof it is a pure matcher gap: `B1:A1`
   parsed fine at deck49 s19, deck21 s24/s30. Owner: block/attacker reply parsing in the AIPlayerGPT harness.
   Cross-deck (every combat-heavy seat). NOT guide-fixable (reply-protocol layer). **Highest-leverage remaining
   harness item; the guide-side FORMAT-DISCIPLINE line (deck14) is the interim.**

2. **[PROMOTED · HIGH · 2-wave convergent, now CAUSING a fallback] BLOCK-SEAM combat-OUTCOME annotation.** Wave-17
   flagged the block-math tax and proposed annotating each candidate block with a computed OUTCOME; wave-18 gives
   it teeth — deck35 62 s16 produced the corpus's ONLY block fallback: an 11,169-char loop that DISTRUSTED both the
   rendered opponent [X/Y] and the existing "unblocked, these attackers deal up to N" annotation and re-derived a
   10/12 aura creature's power until it emitted an unparseable reply. The annotation the engine ALREADY provides was
   correct and would have closed the decision in one line — the model would not trust it (this is the two-sided-
   trust failure at the representation layer). Corroborating tax: deck21's 4 giant-reply fallbacks + deck62's
   giant-reply spirals share the computed-P/T-distrust shape; deck14 E2. FIX: at the BLOCK seam, annotate each
   candidate block ASSIGNMENT with the computed result — post-block life delta, "you survive / you die," blocker
   dies Y/N, trample-through amount, first-strike ordering — so there is nothing left to re-derive. **Sub-item
   (deck35 G1, representation not core):** the model read a Saproling "(2/4)" as dealing 4 (toughness-for-power) —
   EMPHASIZE/label POWER (the first number) as the damage figure in combat option lines; pick the representation
   layer, not a core line. Owner: block/attack option serialization. Removes the giant-reply block-tax at its root.

3. **[CARRIED · HIGH · UNEXERCISED this corpus] Azcanta / reveal-and-order sub-menu represented as an opaque
   "Choose target N of exactly 4" ORDERING menu; put-to-hand whiffed to ZERO last wave.** deck135-owned. No trigger
   this corpus — deck135 never transformed Search for Azcanta (no `{2}{U},{T}` activation), `grep 'of exactly'` = 0
   hits. Fix + wave-17 validation TODO stand (mark hand-eligible cards, actually put an eligible noncreature-
   nonland to hand, collapse/label the bottom-ordering steps; verify the put-to-hand card-script per
   wagicgpt-verify-oracle-text). **#3b — optionone reveal-to-X auto-decline fingerprint PRESENT** (deck135 vs35
   s14/s19, deck35 #2-adjacent): the engine auto-declines the optional reveal chooser before the model can answer;
   root-caused wave-17, fix pending a reveal repro harness — NOT re-diagnosed. Owner: reveal/order option
   serialization + reveal repro harness. Detect: a reveal-and-order activation whose events show all N revealed
   cards → library with none entering hand.

4. **[NEW · MEDIUM-HIGH · deck135-owned; feeds the shipped annotation redesign] Free alt-cost PITCH card
   auto-selected and NOT named on the option line.** deck135 ENGINE#1d: the free "exile a blue card" Force pitch is
   auto-selected by the engine with no card-choice surfaced and the option line does not name which card it exiles.
   vs62 s12: the model planned to pitch "Into the North (a blue card)" — but Into the North is `{1}{g}` (GREEN), so
   the engine pitched the ONLY blue card, **Abominable Treefolk (the deck's finisher)**; the model committed to a
   low-value counter without knowing it lost its closer (non-fatal this corpus, won anyway). FIX (representation):
   name the card the free pitch will exile on the option line; WARN when it is the caster's only pitch-color card or
   a finisher; ideally surface the pitch-card selection when the caster holds >1 pitch-color card. This is the SAME
   annotation lever as the owner ruling (`ed7d579e4`) — extend it to pitch costs. Cross-deck (any exile-a-card
   alt-cost). Owner: cast-option serialization / the warning-annotation surface.

5. **[NEW · MEDIUM · cross-seat; model/decode + harness salvage] REPEAT-LOOP decode guard + last-well-formed-CHOICE
   salvage.** The residual unparsed tax at several seats is decode-time REPETITION spirals (deck135 HARNESS#2: vs49
   s22 ~100×, vs110 s19 ~60×, vs62 s31; deck62 N4 losing-spiral truncations) that blow the token budget and
   truncate with no CHOICE line. NOT the PLAN-caveat class (no stale prior PLAN). NONE flipped a winnable game.
   TWO-part fix, both engine/model-layer: (a) a decode-time mitigation (repetition penalty and/or a max-token
   guard) — note a bigger budget does NOT help (the model spirals longer, deck62 #5); (b) a truncation-time HARNESS
   salvage that scans the (looping) reply for the LAST well-formed `CHOICE:`/`ATTACK:`/`BLOCKS:` line and emits it —
   the model usually states a tentative choice early in the spiral before looping. Owner: reply parsing + decode
   params. (Behavioral half of the losing-position spiral is handled at the GUIDE off-case layer — see the skill.)

6. **[NEW · MEDIUM-LOW · VERIFY-first] No-blockers-decision-at-lethal — confirm a blockers decision is ISSUED to the
   AI seat whenever it holds ≥1 untapped eligible blocker facing lethal.** deck35 #2: vs110 ended T8 (deck35 −1)
   with NO `blockers` record on the opponent's lethal turn despite deck35 holding 3/3 flyers at 3 life (some fresh-
   cast = untapped, LEGAL blockers; summoning sickness does not stop blocking). Either the engine auto-resolved
   (cousin of the Azcanta / optionone auto-decline, #3b) or it correctly found no result-changing block. Probe, do
   NOT assert a misplay or score it as a durdle. Owner: combat-decision issuance path.

7. **[CARRIED · LOW/benign · deck135, 5th+ wave] Stale `[attacking]` board tag.** Own TAPPED creatures render
   `[attacking]` in Main-2 (deck135 vs62 s26/s32/s33/s34, vs110 s18; also deck110 s31, deck49 s13/s14). Verified
   NON-stale at every opponent/Blockers instance; drove NO wrong decision. FIX: clear `[attacking]` on any creature
   not in the current declared-attackers set. Companion inverted/absent Treefolk-freeze half UNVALIDATED 5 corpora
   running (deck135 landed Treefolk vs110/vs21 but no clean non-terminal re-render captured). Owner: board serialization.

8. **[CARRIED · LOW · deck21] Spark Elemental `[treason]` opaque keyword tag** = its end-of-turn sacrifice. Rename to
   a self-describing `[sacrificed at end of turn]`. Owner: keyword-tag serialization.

9. **[CARRIED · LOW · cheap] Redundant global enchantment in hand generates wasted reasoning.** deck62 N5 / deck21
   s14/s17: Primal Rage in hand while a copy is already on the battlefield amplified the N4 spiral ("why is it in my
   hand?"). Cheap nudge: annotate a hand card whose global effect is already active ("(already in play — a 2nd copy
   does nothing)"). Guide carries a stopgap note. Owner: hand-card serialization.

10. **[CARRIED · LOW · deck135 ENGINE#3] Fetch chosen-but-unresolved re-ask + duplicate identical option lines.**
    Decision-count inflation (deck135 vs49 s7/s9/s10, vs21 s7/s8, vs110 s7/s8). Zero resource loss; guide covers the
    model side. Owner: fetch resolution / option de-dup.

11. **[CARRIED · reviewer-facing · harness] translog `options` serialized as an INT COUNT, not the list;** no explicit
    `resolved:` field (reviewers scrape the CHOICES block + `events`). If cheap: serialize the list + emit `resolved:`.
    Owner: translog serialization.

**CLOSED / downgraded this wave:** TARGET CHOICE sub-menu (wave-17 ledger #1 → RESOLVED, highest-leverage close);
Mox E6 (→ RESOLVED by absence); E-49a "(player, life N)" strip (→ CLOSED); N1 sub-menu-as-phase-menu (→ CLOSED);
Blanchwood recount / computed-P/T-distrust FLAVOR-a (→ RETIRED as a distinct watch; the surviving distrust is the
BLOCK-seam re-derivation now in ledger #2); E-49c suppression (→ REPLACED by warning annotation, commit `ed7d579e4`;
C4 re-validates next corpus). **Dormant-unfixed:** E2 equip outcome-delta (4th validated-by-absence).

**Cross-seat MODEL watches (route to model-experiments, NOT any guide — guides already maximal):**
- **Reflexive counter on an off-whitelist noncreature — 4th corpus, MUTATED (deck135).** Face-burn did not recur;
  the class mutated to a pump/trample ENCHANTMENT (Primal Rage) and a single-target BOUNCE (Unsummon), both clear
  whitelist violations, both NON-FATAL. Model over-application of an already-maximal rule; DO NOT add a 4th Force
  line. Best fix = the shipped option-line WARNING ANNOTATION (ledger #4 + `ed7d579e4`): flag off-whitelist counter
  targets (pump/anthem/trample enchantment, single-target bounce, face burn, discard = "let it resolve").
- **Card-KNOWLEDGE errors (color / fetch-target) — cross-seat (deck135).** Into the North called "blue" (it is
  `{1}{g}`, the mistake that let the Force pitch eat Treefolk); Windswept Heath "fetches Island" (Forest/Plains
  only, stranded the blue half); carried Flooded-Strand "fetches Forest." Root: reasons from a hallucinated card
  property instead of the option list. Route to model card-knowledge watch / few-shot.
- **Gemhide auto-tap suppressing attackers? (deck35 #3, NOT confirmed).** vs135 won at opp −7 but issued only ONE
  `attackers` decision across a 5-body board; reconciles against the eligible set (summoning-sick + 2 Gemhides
  tapped for mana) and did NOT cost the game, but keeps the wave-17 hypothesis alive (autoTapForCost may prefer a
  Gemhide any-color producer over a land, tapping a would-be attacker). Worth a one-game pinned probe.

**Deck-construction flags (route to the roster/decklist owner — user owns the decklist; restate, don't relitigate):**
- **deck21:** THREE-wave flood pattern (w16 vs133, w17 vs35, w18 vs35 + vs49) — both losses this wave were
  0-creature back-halves. Guide is CONVERGED; construction (trim 1-2 Mountains / add cheap 1-2-drop redundancy) is
  the last lever on this seat's record.
- **deck135:** Force of Negation DEAD vs all-creature aggro (vs35 flooded on 2× Force, no noncreature to counter);
  slow ground clock + no evasion answer STANDS (5th corpus, Abominable Treefolk / Diamond Faerie the only closers);
  thin blue base + fetch-color mismatch (Windswept Heath can't fetch the needed Island) flipped a wave-17 win to a loss.
- **deck49:** top-heavy Dragon curve — early clock is a 1/1 needing 6 lands, real threats at 4-6, only two {6}
  finishers, thin 3-cost slot; all four losses trace to it. Lever: a 3-drop flyer / second cheap evasive threat.
- **deck35:** premier win-con Shifting Sliver (`{3}{u}`) undeployed TWO full corpora on a ~5-Island blue base;
  both losses to the pool's fastest/biggest (110 turbo-Affinity lifelink flyer, 62 10/12 aura giant). Lever: raise
  blue sources or move evasion off a hard blue pip; trim redundant 3× Striking / 3× Spinneret for more evasion/pump.
- **deck62:** matchup-shape FLOOR, not a play problem — a turn-8-13 ground/go-wide deck that beats the midrange
  mirrors (49, 35) and floors against faster clocks (Goblin aggro, affinity+lifegain). Do not chase with guide prose.

**Card-script Oracle-verification queue (per wagicgpt-verify-oracle-text — guide uses ENGINE reality; flag for a
maintainer, do NOT block guides):** Azcanta put-to-hand (possible scripting bug — ledger #3); deck14 —
**Gravelgill Axeshark** (engine `{4}{u/b}` 3/3 islandwalk+persist vs paper `{4}{U}` 4/3 Merfolk with a red-permanent
attack/block restriction — likely a mis-mapped primitive), Counsel of the Soratami (engine `{2}{u}` vs paper
`{3}{U}`); deck49 — Bloodmark Mentor (engine 1/1 vs real 2/1), Goblin War Paint (engine `{1}{R}` +2/+2 vs real
`{R}` +1/+1), Crucible of Fire (engine `{3}{R}` vs real `{2}{R}{R}`), Rorix toughness (renders 6/5, likely fine),
Ashenmoor Liege (engine punisher-on-target vs real self-harm trigger); deck21 — Scuzzback Scrapper (engine `{R/G}`
1/1 no-haste vs real `{2}{R/G}` 3/1 haste-persist), Boartusk Liege (engine 3/4 trample vs real vanilla 4/4);
deck35 — sliver `lord(sliver)` buffs all slivers incl. opponent's (matches historical Oracle — no fix).

---

## (c) ROTATION TABLE (rotate a veteran out only if its guide takes NO modification AND its seat surfaces NO new work signal)

| deck | guide | new work signal? | verdict | reason |
|---|---|---|---|---|
| **deck21** (Goblin Gang) | FROZEN (byte-identical, cmp-verified) | NO — both open revisions VALIDATED on their motivating matchups; every residual is a cross-seat engine pocket or construction | **ROTATE OUT (converged — 2nd exemplar after deck17)** | POWER FLOOR closed the 0-power-flyer misfire on the SAME vs62/Birds pairing (LOST −1 → WON +12, model quoting the clause); kill-on-sight ENGINE sharpening killed the Dragonmaster Outcast on sight vs49 + two engine/lifelink kills vs110 (structural LOSS→WIN). No taught class regressed or mutated. Record floor is STRUCTURAL (flood variance). Remaining levers are construction (flood) + the cross-seat giant-reply pocket — both off the guide layer. If a length trim is wanted, schedule it as a DEDICATED, separately-validated pass. |
| **deck110** (Etched Affinity) | FROZEN (11th, byte-identical) | YES — a NEW cross-deck class surfaced with a BITING repro (block-echo name-format, deck21 s18) | **HOLD one more cycle** | E6 CLOSED (its prior rotation blocker); H1 canary PASS; E2 validated-by-absence 4th wave. But the wave-17 rotation precondition ("E6 fixed AND a subsequent corpus confirms NO new class") — the "no new class" half FAILED: deck110 holds the block-echo BITING repro that flipped a winnable game. By parity with the E6-hold discipline, hold the biting-repro witness until the fix ships AND a post-fix corpus confirms. (The reviewer sanctions the ALTERNATIVE — rotate now, reassigning the block-echo witness to a block-heavier seat since deck14 [E1, 3 combat-format-drift repros] + deck62 [N2 attacker-eligibility] already own the cross-deck item — a defensible call that would open a 2nd slot for 102 Tergrid. Synthesis recommends the HOLD for parity + measured one-rotation-per-cycle cadence; deck110's binary stays the Galvanic/equip/E6-absence/block-echo-fix regression canary regardless.) |
| **deck135** (Modern Snow) | FROZEN (13th, byte-identical) | YES — ENGINE#1d (NEW) + carried #1c/#3(Azcanta)/#2-freeze | **KEEP** | Answered its charge with the seat's best evidence yet (TARGET CHOICE framing UNLOCKED the control game: 0 stale_echo, 8/8 right picks, vs110 Treefolk+Shackles WON on it). Surfaced NEW engine #1d (free-pitch auto-exiles the finisher, feeds the annotation redesign). Designated owner of the Force + reveal-and-order seams: #1c unexercised a 3rd corpus, #3 Azcanta fix unconfirmed (unexercised), #2 freeze-half unvalidated a 5th corpus. |
| **deck62** (Enchantresses) | REVISED (surgical, round 2) | YES — guide modified + N2/N4 engine items | **KEEP** | Flipped 4/6→2/6 on a pool swap with ZERO taught-class regression (the losing off-case finally ENGAGED — rule 6). Two evidence-driven edits: rule-6 off-case anti-spiral branch (dominant new tax: 5 giants / 3 unparses) + A-list rule bound to the list under "must-attack" pressure. New-work: the N2 attacker-eligibility filter (→ ledger #1) and the N4 losing-spiral both need another corpus. |
| **deck49** (Dragons) | REVISED (first-guide validation) | YES — guide modified (Step 0-ter revision) | **KEEP** | Validated first-guided: EVERY taught class died, all 9 fallbacks vanished (bulk ENGINE-attributable — apportionment). Revision: compressed Rule #1, trimmed Rule #2 to a pointer (engine now carries the representation), ADDED a flood off-case line. Re-validate: trimmed Rule #2 keeps stale_echo at 0; flood off-case kills the mild burn-hold; **C4 enemy-only War Paint re-appears under the annotation redesign (masked class).** |
| **deck35** (Slivers) | REVISED (first-guide validation) | YES — guide modified (Step 0-ter revision) | **KEEP** | Validated first-guided: FC1 durdle DEAD (0-attacker 2/6→0/6, the instrument's clean win); multi-anthem caveat RESOLVED. Revision: two-sided-trust + forced-lethal escape clause targeting FC3 (now the dominant residual). Re-validate FC3 on the has-giant / has-fast-flyer matchups (62, 110); the block-outcome annotation (ledger #2) is a 2-wave convergent build awaiting a second seat's block-tax — deck14 now provides it. |
| **deck14** (Deep Blue) | NEW initial guide | YES — newcomer | **KEEP** | Guideless first pass → initial guide (mono-blue tempo/control countermand "you look like control, you are tempo — attack" + combat FORMAT-DISCIPLINE line + trust-the-rendered-[X/Y]). FC1 durdle (0-attacker loss vs35) + FC2 combat format-drift tax (3 fallbacks, → ledger #1) are load-bearing. Re-evaluate after its first GUIDED corpus (Step 0-ter). |

**Rotation this cycle:** ONE veteran rotates out — **deck21** (2nd converged exemplar; guide stands live, seat
leaves active re-review). **deck110 HOLDs** one more cycle as the block-echo biting-repro witness (E6-hold parity);
it becomes a clean rotate the cycle after the combat-name-tolerance fix (ledger #1) ships AND a post-fix corpus
confirms no new class — which would then open the 2nd slot for 102 Tergrid-1v1. Per the anchor, the next entrant
from the queue is **27 Zombies** (guideless; `deck27.txt` present, no strategy guide yet); remaining queue after it:
**102 Tergrid-1v1-guide** (`deck102.txt` present).

**WAVE-19 POOL (explicit): 110, 135, 62, 49, 35, 14, 27** (27 Zombies enters GUIDELESS → Step-0-bis initial guide;
49/35/62 run their revised guides for validation; 14 runs its first GUIDED corpus; 110/135 freeze-watch). Seven decks.

---

## (d) BOUNDARY CHECK — git status + what the loop commit may stage

- **git status (`git status --porcelain` in `~/Projects/wagicGPT`, observed):**
  ```
   M projects/mtg/bin/Res/ai/baka/deck49_strategy.txt        <- reviewer-applied (deck49 revision), TRACKED
   M projects/mtg/bin/Res/test/lexicon/_known_failures.txt   <- USER WIP (lexicon), DO NOT STAGE
   M projects/mtg/bin/Res/test/lexicon/_tests.txt            <- USER WIP, DO NOT STAGE
   M projects/mtg/bin/Res/test/lexicon/basic_islandwalk.txt          <- USER WIP (8 landwalk suite files)
   M projects/mtg/bin/Res/test/lexicon/basic_legendarylandwalk.txt   <- USER WIP
   M projects/mtg/bin/Res/test/lexicon/basic_mountainwalk.txt        <- USER WIP
   M projects/mtg/bin/Res/test/lexicon/basic_nonbasiclandwalk.txt    <- USER WIP
   M projects/mtg/bin/Res/test/lexicon/basic_plainswalk.txt          <- USER WIP
   M projects/mtg/bin/Res/test/lexicon/basic_snowforestlandwalk.txt  <- USER WIP
   M projects/mtg/bin/Res/test/lexicon/basic_snowswamplandwalk.txt   <- USER WIP
   M projects/mtg/bin/Res/test/lexicon/basic_swampwalk.txt           <- USER WIP
   M projects/mtg/bin/Res/test/lexicon/test_primitives.txt   <- USER WIP, DO NOT STAGE
   M projects/mtg/include/TestSuiteAI.h                      <- USER WIP, DO NOT STAGE
   M projects/mtg/src/TestSuiteAI.cpp                        <- USER WIP, DO NOT STAGE
  ?? HANDOFF_CARD_SCRIPT_LEXICON_TESTS.md                    <- USER WIP handoff doc, DO NOT STAGE
  ?? docs/card-script-lexicon-coverage.tsv                   <- USER WIP, DO NOT STAGE
  ?? projects/mtg/strategy-design/wave18/                    <- THIS wave's review artifacts (untracked)
  ?? tools/audit_lexicon_coverage.py                         <- USER WIP, DO NOT STAGE
  ```
  The user's WIP = the card-script-lexicon-tests campaign (TestSuiteAI.cpp/.h, Res/test/lexicon/* including the 8
  failing landwalk suite files + _tests.txt + _known_failures.txt + test_primitives.txt, HANDOFF_CARD_SCRIPT_LEXICON_TESTS.md,
  docs/card-script-lexicon-coverage.tsv, tools/audit_lexicon_coverage.py). **NONE of it is part of this wave; the
  loop commit must NEVER stage it.** Only the guide-application changes below are this wave's product.
- **gitignore reality (CORRECTED — the wagicgpt skill is right; the wave-17 note "no `-f` needed" was WRONG for NEW
  files):** `.gitignore:50:[Bb]in/` ignores everything under `bin/`. The strategy guides that were ADDED in prior
  waves are already TRACKED (they show as ` M` on edit and stage with a plain `git add`); a NEW guide under `bin/`
  is IGNORED and needs `git add -f`. Verified: deck49_strategy.txt = not-ignored/TRACKED; **deck14_strategy.txt =
  IGNORED/untracked (needs `-f`)**. CLAUDE.md is separately excluded — NEVER stage it.
- **Freeze/identity verification (mechanical `cmp`, wave18 `deckN/strategy.txt` vs live `bin/Res/ai/baka/deckN_strategy.txt`):**
  - **FROZEN — byte-identical (freezes hold):** deck110 (7931 B), deck135 (13357 B), deck21 (10259 B). ✓
  - **REVIEWER-APPLIED (already live, byte-identical — confirmed):** deck49 (5283 B, tracked, shows ` M`),
    deck14 (5341 B, NEW/ignored under `bin/`). ✓ Both live files MATCH their wave18 copies.
  - **REVISED — NOT yet applied (differ, as expected):** deck35 (wave18 6777 B vs live 5733 B),
    deck62 (wave18 7521 B vs live 5997 B). These are the ONLY remaining copy operations.
- **general-strategy / core:** UNCHANGED — 13th consecutive PASS. No 2+-seat core-wording candidate; every
  core-adjacent proposal is single-seat (deck49 reasoning-length brake, deck35 forced-outcome brake, deck35
  POWER-is-first-number) and/or a representation/engine item routed to the ledger, and the losing-position spiral
  they touch is handled at the GUIDE off-case layer this wave (do not double-layer). CORE WATCH recorded:
  reasoning-length / forced-outcome brake (promote a single flat core rung only if a 3rd/dedicated seat shows a
  forced-outcome spiral the guide off-case branch does NOT cover). No `general-strategy.txt` produced.

---

## (e) GUIDE-APPLICATION LIST (exact remaining file operations for the orchestrator)

Working dir: `~/Projects/wagicGPT/projects/mtg`. Source = `strategy-design/wave18/deckN/strategy.txt`;
destination = `bin/Res/ai/baka/deckN_strategy.txt`. The deck49 + deck14 revisions were ALREADY reviewer-applied and
`cmp`-verified byte-identical to their wave18 copies — do NOT re-copy them. Do NOT stage the user's lexicon WIP or
CLAUDE.md.

**COPY (2 revisions still to apply):**
```
cp strategy-design/wave18/deck35/strategy.txt bin/Res/ai/baka/deck35_strategy.txt   # 5733 -> 6777 B  (two-sided trust + forced-lethal escape; anti-durdle spine byte-for-byte kept)
cp strategy-design/wave18/deck62/strategy.txt bin/Res/ai/baka/deck62_strategy.txt   # 5997 -> 7521 B  (rule-6 off-case anti-spiral branch + A-list-under-must-attack sharpen + redundant-global card-note)
```
**NO-OP (already applied by reviewers — verified byte-identical, do NOT copy):** deck49 (tracked, ` M`), deck14
(new, `bin/`-ignored).
**NO-OP (3 freezes — byte-identical, do NOT copy):** deck110, deck135, deck21.

**Post-apply `cmp` verification:**
```
for d in 35 62; do cmp strategy-design/wave18/deck$d/strategy.txt bin/Res/ai/baka/deck${d}_strategy.txt \
  && echo "deck$d applied OK"; done
```

**git staging for the loop commit (explicit files ONLY — never `-A`):**
```
git add   projects/mtg/bin/Res/ai/baka/deck35_strategy.txt \
          projects/mtg/bin/Res/ai/baka/deck62_strategy.txt \
          projects/mtg/bin/Res/ai/baka/deck49_strategy.txt      # tracked; carries the reviewer-applied revision
git add -f projects/mtg/bin/Res/ai/baka/deck14_strategy.txt     # NEW under bin/ (gitignored) -> needs -f
```
(deck110/135/21 unchanged — nothing to stage. `strategy-design/wave18/**` is the review record; stage it with the
commit if the loop tracks review artifacts, or leave untracked per the wave-17 precedent — it is NOT under `bin/`,
so a plain `git add` suffices if desired. NO AI attribution on the commit — user's own public repo.)
