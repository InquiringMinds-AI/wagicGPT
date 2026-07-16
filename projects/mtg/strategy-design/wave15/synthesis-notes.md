# Wave-15 synthesis notes — validation table, ranked engine/harness ledger, rotation table, boundary check

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-040138` (21 games round-robin over decks
44,110,133,135,140,17,21). Binary `/tmp/wagic-aad32bc7f`. Spark @0.75 (med 11.4s / p90 93s; adjudications are a
latency artifact — score POSITIONS). First cycle under the PER-DECK rotation rule; decks 17 (Faeries) and 21
(Goblin Gang) entered GUIDELESS. Five veteran guides FROZEN byte-identical. Core PASS (10th consecutive).

Unit of analysis is the DECISION with seq repros; layer-routing enforced (engine/card-script items live here, never
in guide/core prose). Single-seat items cannot add/cut a core line.

---

## (a) VALIDATION TABLE — the wave-14 fixes under their first at-scale test

| fix (commit) | owner seat | verdict | evidence |
|---|---|---|---|
| **Crash-fix — target-cancel use-after-free `aad32bc7f`** | deck133 | **STRONG PASS** | The wave-14 segfault came from deck133's own Fatal-Push target-decline. This corpus drove the same freed-object family (self-target auto-resolve + heuristic-resolved Fatal Push) **11 times, 0 segfaults**; all 6 games ran to natural/adjudicated end; no `cancelCurrentAction` anomaly. Corpus-wide 0 segfaults. **CLOSE the crash item.** |
| **Galvanic Blast metalcraft `3cb901295`** | deck110 | **PASS (both arms) + play improvement** | Read off engine `Damage:` events: 4 dmg at N≥3 artifacts (deck135 s5 N=3, deck44, deck17 s15 N=5, deck133 s21 N=4), 2 dmg at N<3 (deck133 s5 N=2). Play improved: deck133 flipped loss→win; at the exact wave-13 Obliterator creature-burn blunder spot the model instead fired the Blast at the FACE for real 4-reach; no toughness/creature-burn blunder recurred anywhere. **VERIFICATION TRAP:** the +2 ships as a SECOND resolution instance → a working Blast logs TWO identical `Damage: 2` lines; a grep that dedupes the string mis-reads it as "2" — **count instances / read the life delta.** |
| **Attackers name→index reconcile `d0f88326b`** | deck110 | **PASS** | 20 attacker windows audited vs the eligible roster + `You declared attackers:` events: every eligible named creature unioned in, every ineligible (summoning-sick) name correctly EXCLUDED, **0 eligible attackers dropped**. Wave-13 MIXED-form silent-drop not reproducible. Corroborated deck135 (mixed `A1,<name>` lines, dropped names all ineligible), deck21 s29 (`A1,A2,Scuzzback`, Scuzzback ineligible). deck109 home-seat was wave-14. |
| **Absent-echo staleness route `d0f88326b`** | deck133 | **PASS + 1 false-positive MECHANISM found** | deck133: the wave-13 game-losing shape (vs44 s25 `CHOICE: 1 (Cast Gray Merchant)` at a `[Put in Play/Decline]` menu) routed harmlessly to heuristic; **full Mechanism-B scan = 0 index leaks.** deck44: 2 correct absent-echo refusals (vs140 s22 own-only GFTT, vs135 s48 stale land-echo), **0 self-destroys**, 4/4 removal sub-menus resolved to the opponent. deck110/deck140: all stale_echo events correct catches. **ONE confirmed false-positive mechanism (WATCH):** deck133 vs140 s37 — the route OVER-fired when the echo named an offered option as a SUPERSTRING (`Attack with Yawgmoth, Thran Physician` ⊇ option `Attack with Yawgmoth`); non-deciding; fix = harness prefix/substring match. |
| **Force of Negation resolution — FIRST LIVE VALIDATION (rounds 1/2/3 end-to-end)** | deck135 | **FIRST-EVER PASS + NEW sub-bug** | deck140 **seq18: the free exile alt-cast Force drove Black Sun's Zenith `stack → EXILE` (COUNTERED) — the FIRST completed live alt-cast counter ever captured, and the load-bearing play of a WIN** (the countered wipe would have reset Treefolk+Golem). seq30: hardcast Force ({1}{u}{u}) countered Lightmine Field. Through wave-13 this path had 0 casts / was unvalidated. ENGINE #1 (Force never counters) is FIXED — regression watch: any Force whose target resolves `stack→graveyard/battlefield` instead of `stack→exile`. NEW sub-bug: see ledger #1. |

---

## (b) RANKED ENGINE / HARNESS / CARD-SCRIPT LEDGER

1. **[NEW · HIGH · mechanistic, 3 repros] Force free alt-cast execution wrongly gated on untapped mana.** (deck135
   ledger #4, reframed — SUPERSEDES the async-race theory.) The exile-a-blue alt-cost needs ZERO mana, yet the cast
   silently fails to reach the stack when the caster controls no untapped sources: `(no untapped sources)` correlates
   **3/3** with fizzled answered casts (deck21 s27, s29; deck110 s4), while the one execution (deck135 deck140 seq18)
   had `{w}` up. NOT latency (a 93,000 ms cast executed, an 8,127 ms cast fizzled). Matches the old computeActions
   `ipotential>0` gate note (AIPlayerBaka.cpp ~3114). Bites hardest in the alt-cast's entire reason to exist (tapped
   out on the opponent's turn) — cost deck135 its Treefolk finisher and contributed to the vs21 loss. **Fix:** the
   alternative-cost cast path must put Force on the stack without routing through / gating on the mana-payment or
   untapped-source machinery; an empty cost must be payable on a fully tapped board. Owner: engine cast/alt-cost path.
2. **[PROMOTED · representation] Hybrid-pip / affordability OPTION-LINE annotation — now the TOP representation
   item.** The colored-pays-generic promotion trigger fired cross-seat (deck135 s3 game-losing offered-payable
   decline hallucinating `{1}{g}`; deck17 2 under-deployments; deck21 F5) — but the durable fix is REPRESENTATION,
   not a core line (core already states both facts verbatim; the failure is present-and-disobeyed). Render an
   `offered=payable` breakdown / a "(pay with any color)" / "(payable now)" tag on Cast options so the model cannot
   misread a hybrid or generic cost. Cuts both the wrong declines AND the thousand-token oscillation tax (deck17's
   p90-latency-driven adjudication losses). Cross-seat (17 + 44 + 21). Owner: option-line serialization.
3. **[HIGH · quantified · engine/representation] "Mana available" line HIDES dual-land second colors.** deck140
   quantified **48 instances across 5 of 6 games** (mostly RED-hiding of B/R + R/W duals; 1 black-hide, 1 B+R hide),
   **0 misplays this corpus** (the model treated the offered list as authoritative — even cast an "unaffordable-
   looking" Pyroclasm at vs135 s20). Latent: a wrong decline the day the hidden color is the sole path (the wave-13
   vs109 shape). Same fix family as #2. **Fix:** generate the affordability annotation from the same color-reachability
   engine as the legal-move set. Report count AND impact separately. Owner: mana-annotation serialization.
4. **[confirmed fix-needed · harness] stale_echo SUPERSTRING false-positive matcher.** (deck133 #B.) The absent-echo
   route downgrades a legal, present intent when the echoed FULL card name is a superstring of the option's SHORT
   name (deck133 vs140 s37). **Fix:** match when the offered option name is a PREFIX/substring of the echoed name
   (symmetric with the parseChoice name reconcile). Non-deciding here; single-seat → WATCH for the method, but the
   harness fix itself is cheap and correct. Owner: name-echo/absent-echo reconcile.
5. **[recurred · engine] Equip churn A→B→A net-zero move.** (deck110 E2.) A fully-executed A→B→A Cranial-Plating
   bounce (deck135 s20→s21, 2 wasted `{1}`, net-zero board) — the no-op marker is TARGET-IDENTITY-keyed and blind to
   net-zero MOVES; Rule #1 (equip-once) obeyed 100%, so the leak is the un-marked MOVE. **Fix:** compute the marker
   from OUTCOME DELTA — suppress "Equip … targeting X" when moving the equipment to X would not raise X's resulting
   power above its current carrier's. MEDIUM. Owner: `AIPlayerGPT.cpp` equip-option serialization.
6. **[new · representation, 2 witnesses] Mox Opal "Cast Card Normally with Mox Opal" option shows the Mox's OWN
   text.** (deck110 E6, both deck44 same game — 5.4k-token spiral + an unparsed_reply.) The cast-with-mana-source
   label templates Mox Opal's ability text into the `{card text}` slot instead of the card being cast. **Fix:** show
   the CARD/action, not the mana-rock's text. LOW-MED, cross-deck (any Mox/mana-rock). Owner: option serialization.
7. **[new · engine/representation] Suppress a targeted-removal top-level cast whose ONLY legal targets are
   friendly.** (deck133 #A.) A self-Fatal-Push via a card-PRESENCE hallucination (model thought a battlefield
   Bloodghast was a hand card; echo+index agreed on Fatal Push, so no harness arm could catch it) destroyed its own
   creature. The "only legal targets are YOUR OWN" annotation is present and 10/11 obeyed, but one hallucination
   punched through. **Fix:** suppress / hard-deprioritize such casts (as the 601.2c filter already suppresses
   no-legal-target spells) — makes the failure structurally impossible, not instruction-dependent. MED. Non-deciding.
8. **[card-script · Oracle-verify FIRST] deck21 engine-vs-real divergences.** Per wagicgpt-verify-oracle-text:
   check `Res/sets/primitives/*.txt` against Gatherer/Scryfall. **Scuzzback Scrapper** engine = `{R/G}` 1/1 wither,
   no haste/persist; real = `{2}{R/G}` 3/1 haste-persist (both cost AND body differ; the missing haste is
   behaviorally load-bearing). **Boartusk Liege** engine = 3/4 WITH trample; real = vanilla 4/4 (anthem matches). If
   these are intentional custom cards, fine; if meant to be the real cards, they are wrong. The deck21 guide is
   written to the ENGINE version; do not "fix" the guide to Oracle until the primitives are adjudicated.
9. **[carried · representation, benign] Stale `[attacking]` board tag.** deck135: ~20+ battlefield lines render a
   tapped post-combat creature as `[tapped - untaps and can attack next turn] [attacking]` in non-combat windows;
   deck110 s23 is a 2nd-seat witness (tapped-but-attacking). Did not drive a wrong decision (near-caused a blocker
   misread at deck133 s9). **Fix:** clear `[attacking]` on any creature not in the current declared-attackers set.
   Companion inverted-freeze half (a frozen creature annotated `untaps next turn`) is **UNVALIDATED this corpus**
   (the one Treefolk-freeze window ended the game) — carry OPEN, not fixed.
10. **[OPEN · NOT EXERCISED] Rakdos's Return X auto-underpay.** (deck140.) Rakdos's Return cast 0 times this corpus
    (held every window, correctly) → the underpay could be neither reproduced nor cleared. Adjacent X-payment path
    (3 Black Sun's Zenith X-menus) surfaced correctly (option 1 = biggest affordable, incl. a deliberate X=1) —
    partial mechanism-adjacent evidence only. **"No witness" ≠ "resolved."** Stays OPEN; needs a corpus where a
    lethal Rakdos actually fires.
11. **[model watch] Summoning-sickness untracked in model lethal math.** (deck110 M1a, n=5.) The model names
    same-turn creatures as attackers and folds them into its lethal/damage total; the engine roster + reconcile
    exclude them so no wrong attack occurs, but the arithmetic is inflated. Latent risk if it ever gates a lethal /
    hold-back call. Harness mitigation if it bites: board-line tag `[can't attack yet — summoning sick]`. Cross-seat
    (deck135 same tic). Model-side; not cleanly guide-fixable.
12. **[reviewer-facing · harness] translog serialization nits.** (deck133 E.) The `options` field is intermittently
    an INT COUNT rather than the option list (vs21 s7/s23/s24 — options had to be recovered from the `prompt` CHOICES
    block); and no explicit `resolved: battlefield|graveyard|fizzled|countered|exile` field (deltas scraped from
    `events` narration, carried wave-9..15). If cheap, always serialize `options` as the list and emit `resolved:`.

**Cross-seat MODEL watches (route to model-experiments, not any guide):** Force reflexive-cast swing (deck135: went
from wave-13's perfect decline to casting all 6 offers; got the 2 wipes right, over-Forced a Cranial Plating on an
empty board + a Force-on-Volcanic-Hammer on a false 3-kills-a-6/6 premise); damage-vs-toughness rules gap (deck135
s27, deck110, deck21 F1 all wasted burn on a 6/6 believing 3 damage shrinks/kills it); board-state contradiction
timeout-as-terminal-cost promotion still WATCH (deck110 s23, deck17 s21/s23 witnesses did NOT time out — needs a
contradiction co-located with `unparsed_reply` on a non-lost decision).

---

## (c) ROTATION TABLE (per-deck rotation rule: rotate out only if guide takes NO modification AND seat surfaces NO
new work signal this cycle)

| deck | guide | new work signal? | verdict | reason |
|---|---|---|---|---|
| **deck140** (Wipe Them Out!) | FROZEN (6th) | NO (Rakdos-X untested; dual-hiding is a known corpus-wide item only QUANTIFIED, not new) | **ROTATE OUT** | Reviewer-recommended. Guide is load-bearing (cited verbatim, wins 5/6) — rotation = stop re-reviewing a proven seat, NOT remove the guide. Engine items #3/#10 stay in the shared ledger. |
| **deck44** (UB Faerie Tempo) | FROZEN (11th) | its central finding REJECTED (see below) | **HOLD 1 cycle** | Its "combat asks removed" narrative + loss attribution are rejected (orchestrator-verified); needs a corrected loss read next cycle before a rotation decision. |
| **deck110** (Etched Affinity) | FROZEN (8th) | YES — E2 equip churn recurred, E6 Mox Opal (new, 2 witnesses), M1a summoning-sick (new); canonical validator for Galvanic + attackers-reconcile | **HOLD** | Near-rotation candidate — once E2/E6 are worked and confirmed with no new decision class, it becomes a clean rotate-out. |
| **deck133** (Phyrexian Asphodel) | FROZEN (10th+) | YES — new engine lever #A (suppress own-target-only removal), new harness nit #B (superstring false-positive); PRIMARY validator for crash-fix + absent-echo route | **KEEP** | Fails the second rotation condition; re-evaluate once #A/#B are worked. |
| **deck135** (Modern Snow) | FROZEN (13th) | YES — the corpus's highest-value signal: FIRST live Force counter (ENGINE #1a validated) + the new no-untapped-sources fizzle (#1) on the seam it OWNS; fired the core promotion trigger | **KEEP** | The Force seam is the most active engine surface in the project; rotating it out would orphan the ENGINE-#1b fix→confirm loop that just opened. |
| **deck17** (Faeries) | NEW initial guide | N/A — newcomer | **KEEP** | Guideless first pass; its first guide (hybrid #1-rule, defender facts, attack floor, steal priority) must be validated next corpus. |
| **deck21** (Goblin Gang) | NEW initial guide | N/A — newcomer | **KEEP** | Guideless first pass; its first guide (F1 burn-as-clock / burn-can't-kill-X, F2 Spark timing, F7 no-durdle) must be validated against the opponents that exposed them (vs140 lifegain, vs135/vs17 walls). |

**Replacement for deck140:** next queue item **deck62 (Enchantresses)** enters GUIDELESS first-pass next cycle
(same doctrine as deck17/deck21 this cycle — see Step 0-bis).

---

## (d) BOUNDARY CHECK — git status + freeze verification

- **git status:** `?? projects/mtg/strategy-design/wave15/` is the ONLY untracked entry. **No reviewer touched
  `bin/Res/` or any engine/source file.** Boundary clean.
- **Veteran freeze verification (`cmp` vs live `bin/Res/ai/baka/deckN_strategy.txt`):** all 5 BYTE-IDENTICAL —
  deck44 (12084 B), deck110 (7931 B), deck133 (8960 B), deck135 (13357 B), deck140 (7364 B). All freezes hold.
- **general-strategy.txt:** byte-identical copy of the live core `bin/Res/ai/gpt/system_prompt.txt` (19742 B) — core
  PASS, verbatim.
- **deck44 finding correction (binding):** deck44's F2 "combat asks removed corpus-wide / 0 attacker-blocker asks at
  any seat" is FALSE — per-seat counts are attackers/blockers 29/12 (44), 20/2 (110), 16/0 (133), 18/4 (135),
  20/17 (17), 33/2 (21), 0/0 (140, creatureless). deck110 audited 20 attacker windows; deck135/deck21 show live
  `ATTACK:` declarations this corpus. deck44's auto-combat premise, the surface-audit/trim-block method built on it,
  and its loss attribution ("all-in auto-attacker left no blockers") are REJECTED and NOT adopted anywhere. deck44's
  byte-identical FREEZE stands (independently verified); the seat is HELD one cycle for a corrected read.
