# wagicGPT rules-conformance audit — flagship flagging document

**What this is.** A single entry-point that merges the seven section audits of the Wagic
engine against the *Magic: The Gathering* Comprehensive Rules. It flags every rule the
engine implements incorrectly, and every rule for which the engine has no machinery, for
the game types Wagic actually supports. The seven section files remain the detail; this
document is the map. Each section's own `## HIGHLIGHTS` block is its payload — this summary
consolidates those, it does not re-derive them.

**Rules audited.** Official WotC Comprehensive Rules, effective **2026-02-27**
(`data/MagicCompRules-20260227.txt`, sliced per section; not committed — WotC copyright).

**Scope.** Two-player constructed/limited "classic" Magic — the engine's demo, selfplay,
and duel modes. **Excluded (classified N/A):** CR 8xx multiplayer, CR 9xx casual variants,
Momir, Vanguard, Planechase, Archenemy, Conspiracy, Commander. Rules inside 1xx–7xx that
exist only for those excluded formats are N/A. Physical-play-only rules (shuffling
etiquette, sideboard physicality, negotiated shortcuts) are also N/A.

**Verdict taxonomy** (from `00-frame.md`, classified at the sub-rule level, one verdict per
rule id):

| verdict | meaning |
|---|---|
| **IMPLEMENTED** | Engine honors the rule; cited code evidence. |
| **PARTIAL** | Some sub-behavior honored, some not; the split is explained in the row. |
| **INCORRECT** | Engine actively does something that contradicts the rule; row cites code AND states engine-behavior-vs-rule. |
| **UNIMPLEMENTED** | No engine machinery for it (after a good-faith identifier search). |
| **N/A** | Excluded game type or physical-play-only. |

**Coverage guarantee.** All seven reports passed `check-coverage.py` with **MISSING: 0**.
Every one of the **2,818** rule ids in the audited slices carries exactly one verdict; the
grand totals below sum to 2,818.

**Epistemic note (read this before trusting a verdict).** This audit was performed
**read-only** by code inspection (Codex-assisted, single-model per section, sections audited
independently). Verdicts are grounded in cited `src/File.cpp:line` evidence and, where
relevant, in the project's own known-defect ledger
(`strategy-design/lexicon-triage/engine-bugs.md`) and test fixtures (`bin/Res/test/`).
**Not every PARTIAL or INCORRECT was runtime-probed** — most rest on reading the code path,
not on observing a live misfire (the exceptions are findings tied to a catalogued defect or
a green/known-failing fixture, which are called out). Treat INCORRECT findings as
high-confidence leads, not adjudicated bug reports. See the Known-limits appendix.

---

## 1. Grand totals

| section | CR scope | IMPLEMENTED | PARTIAL | INCORRECT | UNIMPLEMENTED | N/A | total |
|---|---|--:|--:|--:|--:|--:|--:|
| 10 concepts / card parts | 1xx + 2xx | 213 | 161 | 1 | 159 | 89 | 623 |
| 11 types / zones | 3xx + 4xx | 86 | 69 | 11 | 27 | 48 | 241 |
| 12 turns | 5xx | 41 | 39 | 27 | 31 | 10 | 148 |
| 13 casting / abilities | 601–608 | 23 | 77 | 16 | 47 | 5 | 168 |
| 14 effects | 609–616 | 8 | 60 | 22 | 56 | 5 | 151 |
| 15 additional (7xx − 702) | 7xx − 702 | 39 | 420 | 18 | 177 | 79 | 733 |
| 16 keyword abilities | 702 | 260 | 318 | 32 | 97 | 47 | 754 |
| **TOTAL** | | **670** | **1,144** | **127** | **594** | **283** | **2,818** |

Read the shape, not just the totals. The 1,144 PARTIALs are the dominant class: Wagic is a
**card-script engine with per-card approximations**, not a rules engine that derives behavior
from CR primitives. Most PARTIALs say "the common case is scripted and works; the general CR
mechanism (layers, timestamps, atomic transactions, simultaneous SBAs, APNAP ordering) does
not exist." That architectural fact is the through-line under nearly every INCORRECT finding
too, and it is stated most bluntly in sections 12, 13, 14, and 15.

---

## 2. INCORRECT — full consolidated list

**127 rule-ids** are INCORRECT, consolidating into **~55 distinct defects** (many defects
span several adjacent sub-rules — e.g. Gift is 13 ids, one bug). Grouped by CR section.
"[ledger]" marks a finding that matches a catalogued entry in
`strategy-design/lexicon-triage/engine-bugs.md`.

### 1xx — Game concepts
- **119.5** — `reduceto` (set-life) is a **no-op**; the engine does not apply the gain/loss
  needed to reach the target life total (`src/MTGAbility.cpp:5136-5140`). **[ledger]**

### 3xx–4xx — Types & zones
- **304.4 / 307.4 / 400.4a** — Generic zone moves (`AAMover`, `putInZone`) let **instant and
  sorcery cards be moved to the battlefield**; no destination/type guard
  (`src/AllAbilities.cpp:6925-6979`, `src/MTGGameZones.cpp:467-831`).
- **306.7** — The **obsolete planeswalker damage-redirection rule is still implemented**:
  player damage searches for an opposing planeswalker and redirects to it
  (`src/AllAbilities.cpp:2070-2090`), contradicting modern direct-to-planeswalker targeting.
- **309.5b** — Dungeon completion **sideboards the completing card and increments a counter**
  instead of removing the completed dungeon, choosing a new one, and entering its top room
  (`src/AllAbilities.cpp:1717-1736`). **[ledger]**
- **310.4b** — Battle entry defense is card-scripted, and real Battle `counter(0/0.6.defense)`
  lines **parse NULL, leaving the Battle at 0 defense** (`src/GameObserver.cpp:846-856`).
  **[ledger]**
- **310.8b / 310.8c** — No `protector` concept exists, so **attack/block eligibility for
  Battles falls back to ordinary controller/opponent combat logic** and cannot enforce the
  protector-only rules (`src/MTGRules.cpp:2160-2280`, `2470-2590`).
- **400.7** — Ordinary zone changes **preserve forbidden object memory** — X, kicker,
  stored-card links, alternate-cost flags, commander damage, etc. are copied across the
  new-object clone (`src/MTGGameZones.cpp:918-969`), far beyond the CR's enumerated exceptions.
- **400.11b** — Sideboard/command-zone **target moves are inert** (`src/TargetChooser.cpp:1645-1679`).
  **[ledger]**
- **402.2** — When a player is over hand size at cleanup and hasn't discarded, the engine
  **auto-discards `hand->cards[0]` repeatedly** instead of letting the player choose the
  excess (`src/GameObserver.cpp:247-259`). **[ledger]** *(Same defect family as 514.1 — see
  cross-section note in §6.)*

### 5xx — Turn structure & combat (27 ids; the highest-density INCORRECT section)
- **500.1 / 501.1 / 504.1** — A new game **starts at first main**, deleting the starting
  player's **entire beginning phase — untap, upkeep, and draw** — on turn 1, not just the draw
  (`src/Rules.cpp:822-843`, `src/PreGamePhase.cpp:303-311`).
- **500.3 / 502.4 / 514.3** — **Untap and cleanup run as ordinary priority-bearing phases**;
  untap triggers can be acted on too early, and cleanup lacks its no-priority default
  (`src/GameObserver.cpp:360-414`).
- **500.7** — **Extra turns are per-player scalar counters**; this cannot preserve APNAP
  insertion / most-recent-first ordering when both players gain extra turns
  (`src/GameObserver.cpp:145-178`).
- **506.3 / 506.3f / 508.1a / 509.1a** — Attack/block predicates check "is a creature" but
  **omit the "is not also a battle" exception**, so creature-Battles can attack and block
  (`src/MTGCardInstance.cpp:841-865`, `1124-1134`).
- **506.3a / 506.3c / 508.4** — **Put-attacking helpers** clear summoning sickness, call
  `setAttacker`, and **default the destination to the opponent** with no destination/type
  validation (`include/AllAbilities.h:2310-2341`, `src/GuiCombat.cpp:535-536`).
- **506.4c / 510.1b** — An **unblocked attacker whose planeswalker/battle destination vanished
  deals damage to the opponent** as a fallback instead of assigning none (`src/GuiCombat.cpp:535-536`).
- **508.1d** — **MUSTATTACK is greedily forced, not jointly maximized, and is skipped for
  extra combats** (`src/MTGRules.cpp:2139-2148`).
- **508.1h / 509.1d** — Attack and block **costs are paid per-creature, not totaled and locked**
  before payment (`src/MTGRules.cpp:1916-1976`, `1996-2055`). The unpayable alternate
  attack-cost wedge is **[ledger]**.
- **508.3a / 508.3e** — The declare-attackers transition **loops over every currently-marked
  attacker**, so a creature put onto the battlefield attacking can wrongly trigger "attacks";
  triggers can't distinguish attacking a player from their planeswalker/battle
  (`src/MTGRules.cpp:2362-2379`). Attacked-entity primitive is **[ledger]**.
- **508.5** — Combat-damage removal clears `isAttacking` and **retains no former destination**,
  so the last defending player can't be recovered (`src/MTGCardInstance.cpp:760-775`).
- **509.1c** — **MUSTBLOCK greedily picks the first legal target**; no maximum-requirements
  solver (`src/MTGRules.cpp:2498-2619`).
- **509.1h** — `removeBlocker` **resets a blocked attacker to unblocked** when its last blocker
  leaves during the blockers phase (`src/MTGCardInstance.cpp:1703-1713`); CR requires blocked
  status to persist, so this enables damage CR forbids.
- **510.1c / 510.1e** — Combat-damage assignment enforces **legacy sequential lethal ordering**
  rather than arbitrary division among multiple blockers (`src/GuiCombat.cpp:126-169`).
- **510.4** — First/double-strike eligibility is **tested live in each damage pass** rather than
  snapshotted at the start of the first step (`src/GameObserver.cpp:295-321`); gaining/losing
  first strike between steps produces wrong damage.
- **511.3** — Combat roles are **not cleared at end of combat**; attacking/blocking status
  persists into postcombat main (`src/MTGRules.cpp:2380-2394`).
- **514.1 / 514.2** — Cleanup's mandatory actions happen at the wrong time via the wrong
  mechanism: hand-size discard is deferred to `AFTER_EOT`, damage removal is a scripted
  `@each cleanup` trigger, and end-of-turn expiry is separate machinery — none is CR 514's
  single simultaneous non-stack action (`bin/Res/rules/mtg.txt:87-88`). 514.1 is **[ledger]**.

### 601–608 — Casting, abilities, resolution (16 ids)
- **601.2a / 601.2f / 601.2i / 601.5** — Casting is **not an atomic transaction**: the card
  moves to the stack only *after* costs are paid; total cost is not locked as one ordered
  value; cast/put-on-stack triggers fire **before casting completes**; no snapshot/rollback
  (`src/MTGRules.cpp:559-641`).
- **602.2a** — Same defect for **activated abilities**: the stack action is created only after
  targets and costs are committed (`src/MTGAbility.cpp:7556-7562`).
- **603.3 / 603.3b** — **Target, sacrifice, exploit, discard, cycling, and stack-zone triggered
  events resolve immediately** instead of using the stack, eliminating responses and APNAP
  ordering (`src/MTGAbility.cpp:8132-8197`).
- **603.6b** — Static/lord effects mutate an entrant only on a **later `Update()` scan**, so
  ETB triggers can observe the entrant before its continuous modifications exist
  (`src/MTGAbility.cpp:8360-8415`). *(Same machinery as 611.3c / 613.5 in section 14.)*
- **603.7c** — Delayed-trigger wrappers **retain raw card/target pointers** and use current-zone
  checks, so leave-and-return object identity is not tracked (`src/MTGAbility.cpp:8574-8591`).
- **605.5a** — `AManaProducer` is a **targeted immediate ability**; targeted mana producers are
  not demoted to ordinary stack abilities as the CR requires (`src/MTGAbility.cpp:8733-8793`).
- **606.2** — Loyalty handling is **keyed on the source being a planeswalker**, so *every*
  activated ability on a planeswalker gets loyalty timing/use restrictions, not only abilities
  with loyalty symbols (`src/MTGAbility.cpp:7725`).
- **607.3** — Imprint consumers use `imprintedCards.back()` — **only the last exiled card**, not
  every exiled card or the required sum/choice (`src/AllAbilities.cpp:2848-2865`).
- **608.2b** — `TargetAbility::resolve()` **reuses retained targets, rejecting only phased
  permanents**; it neither revalidates chooser legality nor fizzles an all-illegal-target
  spell/ability (`src/MTGAbility.cpp:8052-8098`).
- **608.2e / 608.2f** — Multi-target / multi-player effects **resolve sequentially per target**,
  so actions the CR requires to be simultaneous are observably ordered
  (`src/MTGAbility.cpp:8074-8095`).
- **608.3b** — `Spell::resolve()` **moves every permanent spell to the battlefield without a
  target check** or bestow/mutate fallback; bestow's type is merely changed after stack
  insertion (`src/ActionStack.cpp:462-481`, `src/MTGRules.cpp:1175-1182`).

### 609–616 — Effects (22 ids; CR 613 is the epicenter)
- **610.3a / 610.3b** — `ABlink` **exiles the target even when its return event has already
  occurred** (`src/AllAbilities.cpp:10418-10438`); CR says the object doesn't move.
- **610.4a** — **"Phase out until" is materially wrong**: every `isPhased` permanent phases in at
  its controller's next untap, including until-event phase-outs
  (`src/GameObserver.cpp:1921-1946`).
- **611.2e** — Enters-with characteristics are installed **after** zone entry (the code even has
  a `skipDamageTestOnce` workaround), not as a simultaneous enters-with pass
  (`src/AllAbilities.cpp:3868-3900`).
- **613.1 / 613.1b / 613.1d / 613.1e / 613.1f / 613.3 / 613.5 / 613.6 / 613.10 / 613.11** —
  **CR 613 is not implemented as a layer system.** Characteristics are mutated in place by
  observer callbacks; there are **no layer tags, effect timestamps, dependency ordering, or an
  atomic recomputation pass** (`src/ActionLayer.cpp:146-181`, `src/AllAbilities.cpp:8695-8872`).
  Type/color/ability/control/P-T changes execute in effect order, not layer order; ability
  loss iterates the action vector in reverse and the source itself labels the lord exception a
  "hack" that doesn't handle P/T layers (`src/AllAbilities.cpp:9223-9269`). This is the single
  largest INCORRECT cluster and the root of Humility/Opalescence-class wrong results.
- **614.5** — A replacement can **apply twice to one event**: a returned event recursively
  restarts the modifier list with no "already applied" set (`src/ReplacementEffects.cpp:171-174`).
- **614.7a** — **Zero-damage events consume a one-shot prevention shield**; the registry runs
  before the zero-damage check (`src/Damage.cpp:45-58`, `src/ReplacementEffects.cpp:24-29`).
- **614.12 / 614.17d** — **No prospective-ETB replacement pass**; "can't enter" and ETB
  replacements can't inspect the fully-modified would-be permanent
  (`src/AllAbilities.cpp:8665-8872`).
- **615.7** — Numeric shields subtract per source with **no controller choice allocating a
  limited shield among simultaneous damage** (`src/Damage.cpp:68-78`).
- **616.1 / 616.1e** — Replacement/prevention ordering **applies effects in registry insertion
  order with no affected-player choice or APNAP** (`src/ReplacementEffects.cpp:164-175`); the
  draw-only menu is the sole exception.

### 7xx − 702 — Additional rules (18 ids; SBA correctness is the core)
- **704.3 / 704.7 / 704.8** — The **state-based-action pass is not simultaneous or
  fixed-point**: it mutates each player/card in sequence, dispatches events as it goes, and
  snapshots LKI one card at a time (`src/GameObserver.cpp:833-1060`).
- **704.4 / 704.5h** — **Deathtouch destroys synchronously during the damage event** instead of
  marking damage and destroying at the next SBA check (`src/MTGRules.cpp:4188-4214`).
  *(Corroborated by 702.2b — see §6.)*
- **704.5b** — **Empty-library loss calls `setLoser` immediately** inside `drawFromLibrary()`
  rather than at the next SBA check (`src/MTGGameZones.cpp:322-336`).
- **704.5d** — **Token disappearance is synchronous** on the zone-change event, not an SBA
  (`src/MTGRules.cpp:3659-3683`).
- **704.5f** — A **normal creature at toughness ≤ 0 can regenerate**: the pass calls `destroy()`
  (which honors regeneration) for non-indestructible creatures; only indestructible ones are
  force-moved (`src/GameObserver.cpp:1020-1027`). CR 704.5f is not destruction.
- **704.5j** — The **legend rule is an event-driven one-card mover**, not "choose one, put all
  others away" as one SBA; 3+ duplicates can remain (`src/MTGRules.cpp:3761-3798`).
- **704.5m** — **Unattached Auras can survive**: cleanup is gated on `card->target` being
  non-null (`src/GameObserver.cpp:931-999`).
- **704.5r** — Maximum-counter rules **prevent additions instead of removing excess counters as
  an SBA**, changing trigger/replacement observability (`src/AllAbilities.cpp:3210-3227`).
- **704.5v** — A **zero-defense Battle is marked defeated but not put into the graveyard** by the
  SBA (`src/GameObserver.cpp:846-852`).
- **730.1 / 730.1a / 730.2 / 730.2a / 730.2b / 730.2c** — **Day/night is a real battlefield
  marker card**, so the game designation can be counted/selected/affected as a permanent
  (fixtures document this: `bin/Res/test/lexicon/basic_daybound_flips_after_empty_turn.txt`).
  *(Verdict-altitude conflict with 502.2 — see §6.)*

### 702 — Keyword abilities (32 ids; 6 distinct keyword defects)
- **702.2 / 702.2b — Deathtouch** — lethal assignment and LKI are correct, but destruction
  happens **during the damage event, not the SBA** (`src/MTGRules.cpp:4188-4213`).
  *(Same defect as 704.5h.)*
- **702.59 / 702.59a — Recover** — `_RECOVER_` parses but **returns/exiles nothing**; witness in
  `_known_failures.txt`. **[ledger]**
- **702.61 / 702.61a / 702.61c — Split Second** — the stack **auto-resolves the split-second
  object**, wrongly suppressing permitted mana abilities/special actions and bypassing priority
  (`src/ActionStack.cpp:1144-1151`).
- **702.131 / .131a–d — Ascend** — the city's blessing is a **battlefield token**, not an
  immutable player designation, so it interacts with permanent counts/effects
  (`_macros.txt:133-137`).
- **702.170 / .170a–f — Plot** — reuses foretell without a working plotted-cast permission;
  `macro_plotcast.txt` is a **known failure**, so plotted cards can't reliably be cast later.
  **[ledger]**
- **702.174 / .174a–m — Gift** — Gift cards use equal `mana=`/`other=` costs, so the
  alternative-payment state **never sets and the promised-gift branch is unreachable**; every
  gift effect is wrong when promised. **[ledger]**

---

## 3. UNIMPLEMENTED — tiered

594 ids have no engine machinery. Tiered by whether they can change outcomes in the supported
two-player classic format.

### Tier 1 — gameplay-visible in the supported format (would change real game outcomes)
- **Companion (702.139)** — no pregame reveal, deck-restriction validation, or
  outside-game-to-hand procedure. High impact for deck legality and opening state.
- **Banding (702.22 / 508.1e)** — name parses but attack-grouping and damage-assignment
  behavior are unreachable. **[ledger]**
- **Phasing keyword (702.26)** — the name and one-shot token parse, but there is **no
  untap-step phase-out/phase-in transition** for the keyword. **[ledger]** *(distinct from the
  610.4 "phase out until" INCORRECT finding.)*
- **Poisonous (702.70)** — no combat-damage poison trigger.
- **Champion (702.72)** — macro explicitly marked not implemented; no linked leave/return.
- **Backup (702.165)** — reminder text only; no counter-plus-ability-copy behavior.
- **Retargeting / change-targets (115.3, 115.5–115.9)** — no atomic retarget transaction,
  final-target-set validation, or division preservation. Affects any "choose new targets" card.
- **General draw/loss SBA timing** — empty-library loss and hand-size enforcement exist but at
  the wrong point (these are INCORRECT, not unimplemented); the **mandatory-loop / game-restart
  procedure (104.4 / 104.6 / 726)** is genuinely absent.
- **APNAP simultaneous ordering** — no owner-choice for simultaneous library/graveyard
  placement (401.4, 404.3) and no APNAP trigger-batch ordering (405.3, 603.3b). Rare to notice
  in two-player but real when two triggers stack.
- **Saga lore-counter turn action + final-chapter SBA (505.4 / 703.4f / 704.5s / 714)** — Sagas
  can be script-approximated per card, but there is no intrinsic chapter engine.
- **Face-down exile (406.3–406.4)** — no face-down visibility/piles; affects any "exile face
  down, may play later" card and foretell-adjacent mechanics.

### Tier 2 — edge / rare in the supported format
- **Text-changing effects & splice (612 whole section, 702.47)** — no rules-text transformation
  domain (layer 3); classic color-word/land-word cards and Spy Kit are in missing-card data.
- **Fateseal (701.29), Clash (701.30), Monstrosity (701.37), Bolster (701.39), Detain
  (701.35), Goad (701.15)** — older keyword actions with no named implementation.
- **World rule (704.5k)**, **Role state-based pruning (303.7a / 704.5y)**, **Rad counters
  (727)**, **Amplify (702.38), Gravestorm (702.69), Aura Swap (702.65), Read Ahead (702.155),
  Freerunning (702.173)**.
- **Sticker system (123 entire block, 400.7m, 612.9)** — no sheets/tickets/name/ability/P-T/art
  stickers. (Borderline N/A, but 123 exists in the supported rules for eternal-legal sticker
  cards, so it is flagged UNIMPLEMENTED, not N/A.)
- **Current-speed package (702.178–702.179, 704.5z)** — Max Speed / Start Your Engines! have no
  player-speed state.

### Tier 3 — infrastructural minutiae / newest keyword tail
- **Post-2024 keyword-action wave** — Behold (701.4), Connive (701.50), Incubate (701.53),
  Time Travel (701.56), Collect Evidence (701.59), Suspect (701.60), Endure (701.63),
  Airbend/Earthbend/Waterbend (701.65–67), Blight (701.68) — no named implementation.
- **2025–26 keyword wave (702)** — Harmonize (702.180), Job Select (702.182), Tiered (702.183),
  ∞/Infinity (702.186), Web-slinging (702.188), Firebending (702.189), Sneak (702.190),
  Living Metal (702.161), More Than Meets the Eye (702.162) — no engine behavior.
- **Controlling another player (722), ending the turn/phase (723), restart-the-game (726),
  subgames (728)** — major additional-rule subsystems entirely absent (mostly Karn/Shahrazad
  and mostly edge for two-player classic).

**Compact 702 UNIMPLEMENTED keyword roll-up** (top-level keyword → CR id, one row per keyword):

| keyword | rule | tier |
|---|---|---|
| Banding | 702.22 | 1 |
| Phasing (keyword) | 702.26 | 1 |
| Amplify | 702.38 | 2 |
| Splice | 702.47 | 2 |
| Aura Swap | 702.65 | 2 |
| Gravestorm | 702.69 | 2 |
| Poisonous | 702.70 | 1 |
| Champion | 702.72 | 1 |
| Companion | 702.139 | 1 |
| Read Ahead | 702.155 | 2 |
| Living Metal | 702.161 | 3 |
| More Than Meets the Eye | 702.162 | 3 |
| Backup | 702.165 | 1 |
| Freerunning | 702.173 | 2 |
| Max Speed | 702.178 | 2 |
| Start Your Engines! | 702.179 | 2 |
| Harmonize | 702.180 | 3 |
| Job Select | 702.182 | 3 |
| Tiered | 702.183 | 3 |
| ∞ (Infinity) | 702.186 | 3 |
| Web-slinging | 702.188 | 3 |
| Firebending | 702.189 | 3 |
| Sneak | 702.190 | 3 |

---

## 4. PARTIAL — notable highlights only

The 1,144 PARTIALs are not listed exhaustively (see the section files). These are the ones the
section HIGHLIGHTS flagged as materially incomplete for the supported format:

- **Casting/activation are non-transactional (601.2/602.2 family).** The root under most
  proposal-dependent legality and cost defects — costs paid before the object exists on the
  stack, no locked total, no rollback.
- **Copying is materially incomplete (707, 613.2, 707.x).** Permanent/token copying exists;
  full copiable-value layers, spell/ability copies, DFC copies, and the copy SBA are not
  general concepts.
- **Special card forms are approximations (701.27/708/709/712/729).** Morph/manifest, split
  cards, DFCs, meld, mutate have scripted paths but non-uniform zone characteristics and
  copiable values. The `@mutated` trigger parser failure is **[ledger]**.
- **Prevention has two incompatible pipelines (615).** Registered prevention runs before
  damage; numeric shields and card-specific conversion run later inside `Damage::resolve`.
- **Monarch & Initiative are single flags (724/725), not complete inherent-rule subsystems.**
- **Equipment/Aura/attachment legality is periodic string-driven checks (301.5/303.4/701.3),
  not a general attachment-legality model.** No Reconfigure, no multi-attachment.
- **Dungeon is a counter/script approximation (309/701.49).** No room graph, venture marker, or
  bottom-room state action.
- **Mana subsystem: symbols work; source restrictions, delayed-spend, conversions, X/LKI
  exceptions do not (106–107).**
- **Ordinary characteristics work; multiple/interchangeable names, special-face mana values,
  subtype constraints are incomplete (201/202/205).**
- **Battle combat falls back to controller/opponent logic (310.8 family)** — no protector.
- **Dice/coin (705/706) exist but lack modifiers, reroll ordering, declared results, and
  generalized tables.**

---

## 5. Priority recommendations — "if the engine fixes anything next"

Ranked by game-impact in the supported two-player classic format, weighing frequency and
visibility. "[ledger]" = already tracked in `engine-bugs.md`.

1. **CR 613 layer system (613.1–613.11, §14).** Architectural root of wrong P/T, type, color,
   ability, and control interactions. Largest single INCORRECT cluster; the highest-leverage
   (and hardest) fix.
2. **State-based-action pass: make it simultaneous + fixed-point (704.3/704.7/704.8, §15).**
   Underlies deathtouch, regeneration, legend, token, and LKI defects below.
3. **Deathtouch / zero-toughness destroy synchronously; toughness ≤0 can regenerate
   (704.4/704.5f/704.5h/702.2b).** Extremely common combat outcome; corroborated across two
   sections. Fixing #2 largely fixes this.
4. **Turn 1 deletes the starting player's untap + upkeep + draw (500.1/501.1/504.1, §12).**
   Affects the opening of *every* game and all beginning-of-turn triggers on turn 1.
5. **Combat correctness cluster (509.1h blocked-status-lost, 510.1c-e damage ordering/division,
   506.4c/510.1b null-destination→opponent, 510.4 first-strike snapshot, §12).** Frequently
   hit; produces visibly wrong combat results.
6. **Target legality not rechecked at resolution / permanent spells enter with no target check
   (608.2b/608.3b, §13).** Breaks fizzle, partial-legal resolution, Aura/bestow/mutate
   fallback.
7. **Triggered events that resolve immediately, bypassing the stack (603.3/603.3b, §13).**
   Eliminates responses to sacrifice/discard/cycling/target triggers.
8. **Casting is not atomic — costs paid before stack, cast triggers fire early
   (601.2a/601.2i/601.5, §13).** Root of many downstream defects; large but foundational.
9. **`reduceto` set-life no-op (119.5, §10). [ledger]** Small, self-contained, and any set-life
   card is silently broken.
10. **Battle defense parses NULL → 0 defense (310.4b, §11). [ledger]** Battles are unusable;
    localized parser fix.
11. **Gift keyword unreachable when promised (702.174, §16). [ledger]** Every Gift card is
    behaviorally wrong; equal `mana=`/`other=` cost is the specific bug.
12. **Cleanup discard: wrong cards + wrong timing (402.2/514.1, §11+§12). [ledger]** Two
    sections, one defect family (see §6). Visible whenever a player ends the turn over hand size.
13. **Obsolete planeswalker damage redirection still active (306.7, §11).** Contradicts modern
    Oracle; localized removal in `AllAbilities.cpp:2070-2090`.
14. **Instants/sorceries can be moved to the battlefield (304.4/307.4/400.4a, §11).** Add a
    destination/type guard to `putInZone`/`AAMover`.
15. **Replacement applies twice to one event (614.5) + zero-damage consumes a shield (614.7a),
    §14.** Two small, well-localized `ReplacementEffects`/`Damage` fixes.
16. **Split Second auto-resolves, suppressing permitted mana abilities (702.61, §16).**
17. **Extra-turn sequencing not CR-correct (500.7, §12).** Scalar counters can't order
    APNAP/most-recent-first when both players get extra turns.
18. **Phasing keyword has no phase-out/in transition (702.26, §16, [ledger]) + "phase out until"
    is wrong (610.4a, §14).** Related phasing gaps.
19. **Zone changes preserve forbidden object memory (400.7, §11).** X/kicker/stored-card/
    alt-cost state bleeds across ordinary moves.
20. **Companion (702.139, §16) + Banding (702.22/508.1e, §16/§12, [ledger]).** Deck-legality and
    combat-declaration keywords with no working machinery.

---

## 6. Cross-section notes: conflicts and corroborations

Per audit discipline, where two sections describe the same engine machinery, the relationship
is stated explicitly rather than silently resolved.

**Genuine verdict conflicts (same machinery, different verdict — resolve before acting):**

- **Day/night representation — 502.2 (PARTIAL, §12) vs 730.1/730.2/703.4b (INCORRECT, §15).**
  Both point at the same `Day`/`Night` marker-card mechanism. §12 rates the untap-transition
  *behavior* PARTIAL ("scripted phase-trigger ability, works via fixtures"); §15 rates the
  underlying *representation* INCORRECT ("a marker card is a countable/selectable permanent,
  contradicting the CR game designation"). These are different altitudes on one mechanism — the
  behavior can pass its fixtures while the representation is still CR-wrong. **Treat as INCORRECT
  at the representation level; the PARTIAL is about the transition timing only.**

- **Empty-library-loss timing — §10 (104/121 loss machinery rated IMPLEMENTED) vs 704.5b
  (INCORRECT, §15).** §10 confirms the loss *occurs* (correct outcome via
  `Player.cpp:334-359`); §15 flags that it occurs *immediately inside `drawFromLibrary()`*
  rather than at the next SBA check (wrong timing, can reorder triggers mid-resolution). Not a
  contradiction of outcome, but §10's IMPLEMENTED **under-flags the timing defect** that §15
  catches. The correct combined verdict is: outcome right, SBA timing wrong.

**Corroborations (independent sections reached the same INCORRECT finding — higher
confidence):**

- **Deathtouch synchronous destruction** — 704.5h/704.4 (§15) and 702.2b (§16) independently
  flag the same `MTGRules.cpp:4188-4214` defect. High-confidence.
- **Battle protector absence** — 310.8b/310.8c (§11) and 704.5w/704.5x (§15) agree no
  `protector` machinery exists.
- **Battle zero-defense handling** — 310.4b (§11, entry defense NULL) and 704.5v (§15,
  defeated-but-not-graveyarded) describe two ends of the same broken Battle lifecycle.
- **Cleanup discard** — 402.2 (§11, *wrong cards chosen*) and 514.1 (§12, *wrong timing /
  mechanism*) are two faces of one catalogued defect ("sethand / cleanup discard not
  enforced"). Fix them together.
- **Static/lord effects are frame-late** — 603.6b (§13), 611.3c + 613.5 (§14) all point at the
  `Update()`-scan-after-entry problem in `MTGAbility.cpp:8360-8415`.
- **Simultaneous-resolution is sequential** — 608.2e-f (§13), 613.7m (§14), and 704.3/704.7-8
  (§15) are the same "no simultaneous/atomic pass" architecture surfacing in casting,
  effects, and SBAs respectively.

---

## 7. Known-limits appendix — what this audit did NOT do

- **No runtime probing beyond cited fixtures.** Verdicts are code-inspection-based. Except where
  a finding is tied to a catalogued `engine-bugs.md` entry or a specific green/known-failing
  fixture, no INCORRECT/PARTIAL was confirmed by observing a live game misfire. A code path that
  *looks* wrong on inspection may be masked or corrected by another path not in view.
- **Single-model, per-section audit.** Each section was audited independently by one model pass.
  There was no second-reviewer adversarial pass over the verdicts themselves. Systematic blind
  spots in the search strategy (e.g. a mechanism implemented under an unexpected identifier)
  would produce false UNIMPLEMENTED verdicts; the "good-faith identifier search" bar means
  absence-of-evidence, not proof of absence.
- **Sections audited independently → cross-section interactions are under-flagged.** The §6
  conflicts/corroborations were reconstructed at synthesis time from the HIGHLIGHTS, not
  designed into the section audits. Defects that only manifest at the *seam* between two
  subsystems (e.g. a layer-system fix interacting with the SBA pass) are likely under-represented
  because no single section owned the seam. The Day/night and empty-library-loss items in §6 are
  the conflicts that surfaced; others may exist between sections that were not directly compared.
- **Verdict granularity is the sub-rule, not the card.** "IMPLEMENTED" for a sub-rule does not
  guarantee any *particular card* using it works — card-script correctness is a separate axis
  (the `engine-bugs.md` ledger and the lexicon test suite cover that). Conversely, an
  UNIMPLEMENTED keyword may still have a handful of individual cards faked convincingly in
  script.
- **CR snapshot is 2026-02-27.** Later rules updates are not reflected. The newest-keyword tail
  (Tier 3 UNIMPLEMENTED) is the fastest-moving and most likely to have shifted.
- **No fix was written or attempted.** Per the owner's mandate this is documentation only. Code
  citations are `file:line` at audit time and may drift as the tree changes.
