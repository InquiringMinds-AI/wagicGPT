# Wave-19 synthesis notes — validation table, ranked engine ledger, cross-seat classes, rotation, guide deployment

FIFTH per-deck-rotation cycle. Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260719-092058`
— 21 games round-robin over decks 110/135/62/49/35/14/27. Binary `/tmp/wagic-72b05535d` (wave-19 engine
batch `4becc83be`: combat name tolerance / `parseBlockAssignments` name→label second pass + ineligible-only
ATTACK→none; per-pairing TRADE OUTCOME block annotations "(both die)"-class; alt-cast PITCH surfacing;
`salvageLoopedChoice`. Plus the GPT REVEAL SEAM `f4a2ea891` = `Player::decideReveal`, translog kind `reveal`,
18 decisions ALL at deck135. Plus suppression→annotation replacement `ed7d579e4`; bug-grind + Oracle sweep
incl. planeswalker-damage errata.)

Win table: 110:5/6, 49:5/6, 35:3/6, 62:2/6, 27:2/6, 135:2/6, 14:2/6. 0 timeouts, 0 draws, 5 life-adjudicated
at cap, **0 segfaults (6th clean corpus)**. Fallbacks 2.4% (27/1146): 21 unparsed, 6 stale_echo, 0 defer,
0 empty. Per-seat: 135:9, 62:7, 14:4, 27:3, 35:2, 110:2, **49:0 (second consecutive zero)**.

deck21 rotated OUT last cycle (2nd converged exemplar). deck27 (Zombies) entered GUIDELESS (Step 0-bis →
initial guide). deck14 (Deep Blue) played its FIRST GUIDED corpus (Step 0-ter → produced a revision). decks
62/49/35 ran revised guides round N (62 round 3, 49/35 round 2). decks 110/135 froze byte-identical.

**Headline of the wave:** the wave-19 combat name-tolerance batch CLOSED the wave-18 #1 item (block-echo
name-format) at every seat that bled it — confirmed by parse + collapse, with the exact biting shape now
parsing (deck110 deck27 s23; deck62 filing-seat 3 saves). AND the new GPT reveal seam, while exposing a
game-affecting card-script bug (ENGINE-R1), CLOSED two long-carried deck135 items (optionone auto-decline;
Azcanta opaque ordering menu) and validated the wave-18 pitch-card-naming fix. Core → **PASS, 14th
consecutive** (no 2+-seat core-wording candidate; every core-adjacent proposal is single-seat method or an
engine/representation item, and the losing-position spiral they touch stays at the GUIDE off-case layer).

---

## (a) VALIDATION / CLOSURE TABLE — wave-19 batch + carried items under their at-scale test

| item | owner seat(s) | verdict | evidence |
|---|---|---|---|
| **Combat name-tolerance batch → BLOCK-ECHO NAME-FORMAT (wave-18 ledger #1, the biting item)** | deck110 / deck62 / deck35 / deck14 | **RESOLVED — the highest-leverage close of the wave** | deck110 deck27 s23 `BLOCKS: Ornithopter:Zombie Master` → choice=1 (exact shape that BIT at wave-18 deck21 s18); label-form still parses (deck49 s12); block fallbacks at deck110 = 0. deck62 (the N2 FILING seat) 3 saves that would each have unparsed last wave: vs14 s8 `ATTACK: A1, Saproling, Saproling` (sick names dropped → Canopy Spider), vs35 s33 over-named attack → the 3 eligible Argothians, vs27 s23 `BLOCKS: Yavimaya Enchantress: Walking Dead`; ZERO attacker/block-name fallbacks (was 2). deck14: name-format combat fallbacks → 0; deck35: block discards → 0. |
| **TRADE OUTCOME block annotations (wave-19 batch, first corpus)** | deck110 / deck62 / deck49 / deck35 / deck27 | **VALIDATED accurate + quality-positive; NO mislead anywhere** | Audited across 5 seats, ~60+ block decisions: no wrong outcome printed. deck110 deck49 s12 the annotation DROVE the optimal block (Master of Etherium "you kill it, your blocker lives" over chumping a Skirge). deck62 19/19 accurate, enabled value blocks. deck35 vs27 seq13 clean first-strike profitable block, vs49 seq17 survival block. Delivers wave-18 G1 (POWER "deals N" printed separate from (X/Y) → toughness-as-power misread pre-empted). **Bonus (deck49 deck14 s23):** annotation "(both die)" masked a card-script stat drift (Ashenmoor Liege engine 4/1 vs guide/real 4/4) and the model trusted the rendered OUTCOME — a second face of the two-sided-trust rule. |
| **ALT-CAST PITCH surfacing (wave-18 ENGINE-1d fix)** | deck135 | **VALIDATED — the finisher was NOT eaten this corpus** | deck135 d62 s19 Force of Negation option now names the auto-exile pool ("auto-exiles ONE of: Search for Azcanta, Ice-Fang Coatl, Search for Azcanta — the engine picks for you here"); engine auto-picked a dig spell, NOT Abominable Treefolk. Wave-18's finisher-eating (vs62 s12) did NOT recur. Residual (LOW): engine still auto-picks; model can't choose which of >1 pitch card. |
| **AZCANTA optionone AUTO-DECLINE (wave-17/18 known-open #4b)** | deck135 | **CLOSED by the reveal seam** | The old fingerprint (engine auto-declines the optional reveal chooser) is GONE: the model now DRIVES all 8 Search-for-Azcanta upkeep looks, both branches resolve correctly (put-to-graveyard: d14 s11/s29; keep-in-library `PUT: none`: d14 s18/24/34, d49 s24/28). Upkeep script uses the safe `target(<upto:1>*|reveal)` idiom. |
| **AZCANTA opaque "Choose target N of exactly 4" ORDERING menu (wave-17 ledger #4 first half)** | deck135 | **CLOSED** | `grep 'of exactly'` gone; transform+activation now renders as a clean reveal seam (d27 s24). |
| **Non-contiguous / skip-first reveal ORDERING residual (brief's ⚠ UNVERIFIED suspect)** | deck135 | **REFUTED as an ordering bug** | All 3 exercised Glacial Rev picks were non-contiguous (d110 `2,6`; d49 `1,2,3,4,6`; d27 `1,3,4,5,6`) and every chosen card went to the WRONG zone regardless of index — the pre-corpus "chose 2,3 / engine moved 1" glimpse was an early view of the R1 all-sweep (index 1 → graveyard because ALL → graveyard), not off-by-one. The one seam that honors the chosen SET (Into the North) takes the first legal target in index order, correctly. Superseded by ENGINE-R1. |
| **E-49c beneficial enemy-only cast: suppression → ANNOTATION swap (owner ruling `ed7d579e4`) + C4 mask-lift** | deck49 | **CLOSED — C4 proven-solved exactly as scheduled** | The mask lifted: enemy-only Goblin War Paint reappeared (deck135 s4/s5/s7) with "every legal target is the OPPONENT's — this would help their side" warning; the model DECLINED 3/3 (Cast nothing s4/s5; Cast Bloodmark Mentor s7), 0 loops, 0 fallbacks. The fatal wave-17 C4 loop had a live opportunity and did NOT recur. Boundary preserved: annotation fires only on the enemy-ONLY menu; the MIXED menu (deck62 s10/s11) is still carried by the guide's never-aura rule → rule stays load-bearing. |
| **E6 Mox "Cast Card Normally" dead-end** | deck110 | **RESOLVED (2nd-wave confirmed by absence)** | `grep "Cast Card Normally"` = 0 corpus-wide. |
| **E2 equip outcome-delta no-op marker** | deck110 | **UNFIXED, dormant, 5th-wave validated-by-absence** | 6 equips value-positive, 0/24 ALREADY-takes; identity-keyed net-zero-MOVE blind spot got no exercise. Lever unchanged (compute the no-op marker from OUTCOME DELTA). |
| **Blanchwood recount / computed-P/T distrust pocket** | deck62 | **STAYS RETIRED (round 3)** | No recount-of-already-printed-P/T; surviving Forest-counts are legitimate prospective cast-value. The 7 unparses are an ENTIRELY different class (see ledger). |
| **H1 PLAN-carry stale-intent** | deck110 | **RESOLVED-WATCH, canary PASS** | One MILD carry-vs-prompt whiff (deck27 s18 land-drop, non-biting); no self-blast, no contradiction spiral. |
| **Crash / UAF / empty_reply** | all | **CLOSED (6th clean corpus)** | 0 segfaults, 0 empty. |

---

## (b) RANKED ENGINE / HARNESS / CARD-SCRIPT LEDGER for the wave-20 batch (layer-routed; guide/core-blind items live here). Rank = frequency × decision-cost × fix-cost.

1. **[NEW · HIGH · deck135-owned, CONFIRMED, GAME-AFFECTING · CARD-SCRIPT] ENGINE-R1 — Glacial Revelation's
   interactive reveal drops the entire "to-hand" partition; every revealed card is milled to graveyard.**
   3/3 parsed reveals lost 100% of the model's snow picks. **d110 s18** `PUT: 2,6` (Snow-Covered Island +
   Ohran Viper) — at 11 life vs lethal Master of Etherium the model correctly grabbed its ONLY answer, engine
   MILLED it → collapse to 1 life, adj LOSS. **d49 s30** `PUT: 1,2,3,4,6` incl. Abominable Treefolk (the
   finisher) — all milled at 10 life → adj LOSS. **d27 s12** all 6 to graveyard (non-critical). NOT
   eligibility (d110 chose Snow-Covered Island, unambiguously snow). **The reveal-seam routing (GPT →
   interactive `auto=` per the ishuman lens) EXPOSED a latent card-script bug Baka never hit** — the
   non-interactive `aicode` line for the same card is CORRECT. Root hypothesis (VERIFY per wagic-cardscript
   FIRST): `Res/sets/primitives/borderline.txt:45643` optiontwo `all(*|reveal) moveto(graveyard)` re-grabs
   what optionone's `<anyamount>` multi-select just moved to hand; the working Search-for-Azcanta upkeep uses
   `optiontwo target(<1>*|reveal)` (the REMAINDER, not `all`). **Fix-seat question:** change Glacial Rev
   optiontwo to target only the unchosen reveal remainder — scoped to the multi-to-hand `<anyamount>` shape;
   `all(*|reveal)` appears 538× corpus-wide, single-target optionone cards may be fine → do NOT blanket-ban.
   Verify on a probe deck (stack Glacial Rev + snow permanents, read next-record zone moves) before AND after.
   Highest-value item of the wave. Owner: card-script. (Single-seat by EXPOSURE only — decisive + confirmed +
   clear fix; this is an engine-ledger bug with a repro, not a core skill line, so the single-seat gate does
   not apply.)

2. **[NEW · HIGH · 2-game convergent, one BITING · ENGINE combat-issuance] ATTACKERS-DECISION NON-ISSUANCE —
   the declare-attackers step is not offered to the GPT seat on some own-turns despite eligible untapped/unsick
   attackers.** deck35: **vs135 (adj LOSS, the PRIMARY cause)** — deck35 dealt ZERO combat damage all game
   (opp translog has no "Damage: N dealt by …Sliver"); on turn 8 Horned Sliver was unsick+untapped and no
   `attackers` decision issued (game ran to t9/t10 → not a cap artifact); at t10 the model's own plan was
   explicitly aggressive and it still got no attack step. **vs27 (WON)** — no `attackers` decision on the t4
   and t6 own-turns despite four untapped slivers, yet the SAME game issued attack steps t2/t8/t10 — the
   non-issuance is INCONSISTENT (a conditional gate). Cost the game at vs135, cost only tempo at vs27.
   **Distinct from wave-18 ledger #6 (no-BLOCKERS-decision-at-lethal, deck35): #6 was the blocks side, this is
   the ATTACKERS side of the same combat-decision-issuance path.** Repro files handed to engine; deck35's
   hypothesis to test first: the step is skipped when the last main-phase action was a cast that left no "Cast
   nothing" prompt AND the newly-cast creatures are all summoning-sick, so the phase advances past combat
   before the pre-existing unsick attackers are offered. Owner: combat/attacker-declaration gate in
   ActionLayer/GameObserver + how AIPlayerGPT enters the attacker step. **Ready for a targeted engine
   investigation now that a 2nd seat-independent repro exists.** NOT guide-fixable (the model WANTED to attack).

3. **[NEW+CARRIED · HIGH · 4 seats, one DECISIVE · HARNESS combat-reply parsing] COMBAT-REPLY PARSE ROBUSTNESS
   — salvage/partial-valid recovery for well-formed-but-dropped combat lines.** One work item, four convergent
   members: (i) **`salvageLoopedChoice` coverage gaps** — the salvage keys on `CHOICE:`/`ATTACK:` only and
   rescued 0/9 at deck135; it misses BLOCKS lines, reveal `PUT:` lines (d35 s10, d27 s8 stated a tentative
   pick early), and terminal prose-commitment lines (deck110 both unparsed ended "I will play the Mountain" /
   "I will choose Cast Cranial Plating" with no `CHOICE:` token). Extend the scanner to recover the last
   well-formed `BLOCKS:`/`PUT:`/prose-commitment line via the same name-echo the `CHOICE:` matcher uses.
   (ii) **well-formed `B#:A#` block DISCARDED — DECISIVE (deck14 vs49 s31).** A 597-char reply ending in a
   clean `BLOCKS: B1:A2` (block the 5/5 Dragon with the stolen 5/5 Dragon) was unparsed and handed to Baka;
   deck14 died 2 turns later. Diagnosis BLOCKED by the observability gap (item 4) — options_text empty, can't
   confirm the index range. Highest-cost fallback class (a correct decisive block dropped). (iii) **name-
   tolerance "(P/T) #N" suffix residual (deck135 d62 s27).** `BLOCKS: Ice-Fang Coatl: Saproling (1/1) #1` — a
   legal single block — discarded because the model's disambiguation count-suffix defeated the name→label
   second pass; strip trailing `(P/T)`/`#N` before the match. NEW shape, not the old distrust pocket. (iv)
   **out-of-range attacker index → whole-reply discard (deck27 deck14 s9).** Model hallucinated an A2 from an
   earlier turn ("Gravelgill Axeshark 4/4"), answered `BLOCKS: B1:A2`; the whole reply discarded. Proposal:
   drop the illegal-index pairing, keep the rest; fall back only if nothing valid remains (protects the
   mixed-reply case). Owner: AIPlayerGPT combat-reply parsing + salvage scanner. Cross-seat (135, 14, 27, 110),
   one decisive repro.

4. **[NEW · MEDIUM · ENABLING · HARNESS serialization] COMBAT `options_text` OBSERVABILITY GAP (deck14).**
   Every `blockers`/`attackers` record at deck14 logs `options_text == ""` (ask records DO carry full
   options_text). Two costs: (a) it defeated the brief's TRADE-OUTCOME annotation validation at deck14 (the
   annotation surface is not present in this seat's logs), and (b) it BLOCKED diagnosis of the DECISIVE
   dropped block (item 3-ii). Serialize combat `options_text` (with any trade annotations) into the translog so
   combat-decision review is possible — this is the combat side of wave-18 ledger #11 (translog `options`
   serialized as an INT count). Enabling item for the whole combat-parse investigation. Owner: translog
   serialization.

5. **[NEW · MEDIUM · deck135-owned · REPRESENTATION] REVEAL/SEARCH ELIGIBILITY SURFACING.** (a) **ENGINE-R3 —
   Into the North SEARCH mis-rendered as a whole-library "choose subset to hand" menu** (49 cards, no snow-land
   filter, verbs describing a hand-partition not a battlefield search). OUTCOME is CORRECT (engine takes the
   first legal snow land in the chosen set, 4/4) but the misrendering caused a 12k-char confusion spiral at
   every instance + 1 fallback (d35 s10). (b) **ENGINE-R4 — Azcanta activation eligibility not surfaced**
   (d27 s24): the to-hand slot is `-land;-creature` but the option list marked nothing; the model picked two
   creatures → 0 to hand. Same family as wave-18's "name the pitch card": for any reveal/search/tutor chooser,
   offer ONLY the legal targets for that partition, or annotate each card's eligibility. Owner: reveal/search
   option serialization. (R4 VALIDATION TODO: an Azcanta activation picking an ELIGIBLE noncreature-nonland —
   confirm it reaches hand, ruling out an R1-style drop in the activation optiontwo.)

6. **[CARRIED · MEDIUM · 2 seats · MODEL/DECODE watch, NOT the engine batch proper] DECODE-REPETITION LOOP.**
   **Decision (per the brief's ask): a model/decode watch, not a guide lever and not a top engine-batch slot.**
   A short phrase repeated verbatim to truncation, no advancing reasoning. deck135 HARNESS-3 (4 of 9), deck62
   N6 (4 of 7, now a SECOND seat), deck27 (part of its 3). Consensus across all three seats: NOT guide-
   addressable (the model even QUOTES the correct guide line inside the loop) and NOT token-budget-fixable
   (bigger budget → longer spiral). **None flipped a winnable game** — all in already-lost/won contexts. Route
   to a decode-time mitigation (repetition penalty and/or a max-token guard). The reviewer discipline that
   matters (skill): bucket truncation-unparses by MECHANISM (repetition-loop vs advancing-spiral) before
   proposing any fix — only advancing-spirals are guide-addressable. Owner: decode params.

7. **[NEW · LOW · deck62 · HARNESS] N7 — stray `ATTACK:` line arriving at a non-combat CAST ask.** vs27 s27
   (Main-2 "Cast Primal Rage / Cast nothing"): the model emitted an `ATTACK:` line (already attacked this
   turn) and the whole reply is DUPLICATED (sampling artifact) → unparse; correct answer was "Cast nothing."
   Cheap tolerance: map a stray combat line at a non-combat ask to Cast-nothing/pass rather than unparsing.
   Single instance. Owner: reply parsing.

8. **[CARRIED · LOW · deck135] Stale `[attacking]` board tag** (own tapped creatures render `[attacking]` in
   Main-2). Not re-audited this corpus; no wrong decision traced to it. FIX: clear `[attacking]` on any creature
   not in the current declared-attackers set. Owner: board serialization.

9. **[CARRIED · LOW · cheap · deck62] Redundant global enchantment in hand** (Primal Rage in hand while a copy
   is in play) amplifies decode loops (vs14 s21, vs110 s1). Annotate "(already in play — a 2nd copy does
   nothing)". Owner: hand-card serialization.

**Card-script Oracle-verification queue (per wagicgpt-verify-oracle-text — flag for the maintainer, do NOT
block guides; note trade annotations now MASK stat drift at the decision surface so decisions won't look wrong):**
**Glacial Revelation** (ENGINE-R1 — the confirmed bug, top of the queue); Ashenmoor Liege (engine 4/1 vs real
4/4 — reconfirmed deck49 deck135 s13 / deck14 s23); Gravelgill Axeshark (engine `{4}{u/b}` 3/3 islandwalk+persist
vs real `{4}{U}` 4/3 Merfolk with a red-permanent lock — likely mis-mapped, deck14); Counsel of the Soratami
(engine `{2}{u}` vs real `{3}{U}`); carried deck49 list (Bloodmark Mentor 1/1 vs 2/1, War Paint `{1}{R}`+2/+2
vs `{R}`+1/+1, Crucible `{3}{R}` vs `{2}{R}{R}`).

**CLOSED / downgraded this wave:** block-echo name-format (wave-18 #1 → RESOLVED, highest-leverage close);
E6 (RESOLVED by absence, 2nd wave); E-49c/C4 (CLOSED — mask lifted, proven-solved); optionone auto-decline +
Azcanta opaque ordering menu (both CLOSED by the reveal seam); pitch-card-naming (VALIDATED, residual LOW);
non-contiguous reveal-ordering suspect (REFUTED — superseded by R1). **Dormant-unfixed:** E2 equip outcome-delta
(5th validated-by-absence).

**Cross-seat MODEL watches (route to model-experiments, NOT any guide):**
- **Correct-but-IGNORED trade annotation (2 seats, converging).** deck62 chump-blocks its 0/1 shroud engine to
  absorb NON-lethal damage while ahead against the "[deals 0 — only absorbs damage]" tag (vs135 s25, vs14 s36);
  deck27 declined a FREE "(you kill it, your blocker lives)" block that would kill the opponent's team-wide
  first-strike lord (deck49 s9). The annotation is correct and the model verbalizes the right read, then
  disobeys — a model-obedience gap, not a representation defect. deck62 routes its half to a GUIDE defensive
  clause (edit B); deck27 flags a candidate guide block-rule. **Promote to a cross-seat class: score every new
  representation annotation on TWO axes (correct AND obeyed); a correct-but-ignored annotation routes to the
  GUIDE, never to a change in the annotation.**
- **Reflexive off-whitelist counter (5th corpus, deck135).** Force on Gaea's Anthem (a +1/+1 team anthem, off
  the guide whitelist), non-fatal; the finisher-eating half is now fixed by the naming annotation. Model
  over-application of an already-maximal rule — do NOT add a 4th Force line; the incoming off-whitelist-target
  warning annotation is the lever.
- **Model card-KNOWLEDGE / rules-knowledge errors (deck14, deck135).** deck14 vs49 s26 "Boomerang the Ashenmoor
  Liege [a spell on the stack]" (bounce can't target a spell); reveal-time snow-supertype confusion (deck135
  d27 s8, which of 6 are snow). Route to model card-knowledge / few-shot.
- **MENU/INTENT MISMATCH (deck14, 3 instances — a named sub-shape of the list-authority family).** The model
  narrates action X then emits a `CHOICE:` index pointing at a DIFFERENT listed option (vs49 s26 "Boomerang" →
  Island; vs135 s27 "Cast Counsel" → Play Island; vs27 s15 "Cast nothing" → Lord of Atlantis). Durable lever =
  representation (echo the chosen option's TEXT in the re-ask, or a soft "your number points at <option>"
  nudge), not core prose. Distinct from out-of-range over-reach and unpayable-decline over-restriction.

**Deck-construction flags (route to the roster owner — user owns the decklist; restate, don't relitigate):**
- **deck135:** under-powered small-creature clock STANDS (6th corpus; Treefolk/Diamond Faerie the only closers)
  — and ENGINE-R1 makes it WORSE by milling those closers off the top. Force DEAD vs all-creature aggro softened
  (vs35 Slivers stabilized to an adj WIN). Thin blue base + fetch-color mismatch STANDS.
- **deck35:** blue availability is the SWING variable (3rd corpus) — vs62 flipped LOSS→WIN when Island came
  online; Shifting Sliver (`{3}{u}`, the premier win-con) undeployed THREE full corpora. Base-1/1 ground swarm
  is structurally soft to a turn-6 clock (110 Affinity).
- **deck49:** top-heavy Dragon curve STILL LATENT (the sole loss, vs110: a lone 1/1 Dragonmaster Outcast that
  never reached 6 lands, out-raced by a 10/5 Master turn 7-8). A DECKLIST property, not guide/decision-fixable.
- **deck62:** matchup-shape FLOOR (a turn-8-13 ground/go-wide deck that beats the midrange mirrors and floors
  against faster/wider clocks); the guide cannot draw a faster clock.
- **deck27:** NO removal / NO burn — pure creature combat (every loss was a creature-combat loss); Deepchannel
  Mentor (whole blue team unblockable) is a 1-of never drawn. Construction levers, not prompt levers.

---

## (c) CROSS-SEAT BEHAVIORAL CLASSES (for the skill — method/instrument level)

1. **Two-tier aggression countermand (deck14, NEW method).** A first guide's attack-floor rule reliably kills
   the 0-attacker durdle but the durdle MUTATES one level down into a SLOW-CLOCK soft-durdle (the model
   satisfies the literal floor — declares an attacker — while its finishers sit in hand and opp life barely
   moves; flyers deployed 1 of 6 games). Write aggression countermands in TWO tiers: (declare attackers) AND
   (deploy the real threat / race the mana to the finisher / swing the whole board), at the altitude of the
   WIN CONDITION, not the combat step. A model that learns to satisfy the literal floor will satisfy ONLY the
   literal floor. Applies to any deck with a distinct finisher behind a cheap opening.

2. **Combat-answer FORMAT-DRIFT rung PROMOTED from watch to validated (deck14 before/after).** Wave-18 flagged
   it single-seat watch; wave-19's before/after (Rule #2 + the engine name-tolerance batch together) collapsed
   combat replies from 8-13k chars to 300-800, converting the two residual tax spikes (vs35 s26 8307, vs49 s28
   4492) from fallback sources into merely-verbose-but-PARSED decisions. For any derivation-heavy-combat deck,
   ship the short-answer + code-format discipline line as a standard rung; the engine parser tolerance is the
   belt, the guide line the suspenders — they compound.

3. **0-attacker durdle instrument — distinguish MODEL-durdle from ENGINE-NON-ISSUANCE (deck35 S1, the wave's
   sharpest instrument refinement).** When the 0-attacker instrument fires, do NOT score it a durdle until you
   check two disqualifiers: (a) did the seat have an eligible untapped/unsick attacker that turn, and (b) was
   an `attackers` decision actually ISSUED? A 0-attacker game with an eligible board and no attacker decision is
   an ENGINE gap (→ notes.md ledger), not a guide regression. Without this, deck35 would have logged the
   attackers-non-issuance bug as an FC1 durdle relapse.

4. **Off-case branch keyed on the CONDITION, not the STATE (deck62 #2, refines the wave-18 promoted off-case
   rung).** The anti-spiral line authored from behind-at-low-life spirals leaks the moment the SAME
   lethal-hunt behavior appears AHEAD (deck62 vs135 s32 at 18/13 with a winning board → unparse); meanwhile the
   behind-case now resolves correctly. Key the off-case on the DECISION CONDITION the behavior tracks ("you
   cannot reach lethal THIS turn — ahead or behind"), never on the mood-proxy ("behind"/"low life") that
   co-occurred in the first corpus.

5. **Bucket truncation-unparses by MECHANISM before proposing a fix (deck62 #3 / deck135 / deck27).** Read
   whether the reply's tail REPEATS (decode-repetition loop → model/decode watch, NOT guide) or PROGRESSES
   (advancing reasoning-spiral → guide off-case branch). Extends the wave-18 repeat-loop signature with a
   second fork: some truncations are neither behavior nor budget — they are decode pathology below the guide
   layer entirely.

6. **Reveal-seam PARTITION-TRACE reviewer instrument (deck135, found ENGINE-R1).** For any deck that reveals:
   score the ZONE OUTCOME (the next record's `events`), never just "did the model answer." A seam's DECISION-
   SURFACING can be fixed while its RESOLUTION stays broken. Separate three look-alike failure modes:
   (a) RESOLUTION bug (engine acknowledges the pick then moves it to the wrong zone — R1); (b) ELIGIBILITY gap
   (model picked cards the restriction forbids because it wasn't surfaced — R4); (c) RENDERING misdirection
   (the effect is mislabeled — R3). Only (a) is an engine/card-script bug; (b) and (c) are representation.

7. **Annotation SCOPE + mask-lift discipline (deck49).** (a) A class recorded "masked, re-test when the mask
   lifts" MUST be re-tested on the corpus where the mask lifts, and the verdict is only valid THEN (C4 closed
   exactly as scheduled). (b) Before trimming a strategic rule as "redundant with an engine annotation," check
   the annotation's SCOPE — a conditional annotation (enemy-only) leaves the complementary case (mixed menu) to
   the guide rule, so it is NOT a trim candidate. Refines wave-18's concurrent-engine-fix apportionment:
   apportion by the annotation's ACTUAL trigger surface, not the class name. (c) A correct decision under an
   outcome annotation is NOT evidence the card-script stat is right — annotations mask stat drift at the
   decision surface, so verify stats separately (Oracle queue).

8. **Verify the focus surface is LOGGED at your seat before reporting (deck14 Proposal 4, reviewer discipline).**
   When a brief assigns a decision-surface validation, first confirm the surface is logged at your seat; if not,
   say so with the field name — an observability gap is a coverage HOLE, not a clean pass (deck14's empty combat
   `options_text` defeated its TRADE-OUTCOME assignment).

9. **Correct-but-ignored annotation → GUIDE, not the annotation (deck62 #4 / deck27 #4, 2-seat converging).**
   See the model-watch above. Score a new representation annotation on both axes; route the correct-but-ignored
   residual to the guide (a protect-the-engine defensive clause), never to a change in the accurate annotation.

---

## (d) ROTATION DECISION

Rule: rotate a veteran out only if (1) its guide takes NO modification AND (2) its seat surfaces NO new work
signal. One veteran rotates per cycle (measured cadence).

| deck | guide | new-work signal? | recommendation | reason |
|---|---|---|---|---|
| **deck110** (Etched Affinity) | FROZEN (12th, byte-identical) | NO | **ROTATE OUT — 3rd converged exemplar** | Its wave-18 self-set clean-rotation precondition ("block-echo matcher fixed AND a subsequent corpus confirms no new class") is now MET: block-echo is FIXED (deck27 s23 name-format parses) and NO new class surfaced — the 2 unparsed are the pre-existing cross-deck giant-reply/prose-commitment pocket, equip clean (5th wave), E6 closed, Galvanic 6/6, annotations helping. Its block-echo witness role is fully dischargeable elsewhere (deck62/deck135 block more). Retain the FROZEN guide as a regression canary (Galvanic-finisher targeting, equip outcome-delta, block-echo fix, trade-annotation accuracy). |
| **deck49** (Dragons) | FROZEN (byte-identical, md5 `a7d4813a…`) | NO (decision-level) — one OPEN validation thread | **HOLD one more cycle (rotation candidate deferred)** | A genuine rotation candidate: every taught class dead/held across TWO guided corpora, 0 fallbacks twice, C4 CLOSED (the last open item), no guide-mod, no decision-level new work. The ONE caveat: the flood/no-curve OFF-CASE (added wave-18) has never been EXERCISED (the winning corpus never floods), so "the mild burn-hold dies under it" is an OPEN validation thread. Per the one-rotation-per-cycle cadence (deck110 is the cleaner rotate — no caveat) and the standing discipline of not orphaning an unvalidated rule, HOLD deck49 one cycle to discharge the flood off-case; it becomes a clean rotate the cycle after a flood game validates it. (Defensible alternative: rotate now and schedule the flood off-case re-test for deck49's next appearance — but the synthesis recommends the HOLD for parity with the wave-18 deck110 precedent.) |

**Rotation this cycle:** ONE veteran rotates out — **deck110** (3rd converged exemplar, after deck17 and
deck21). **deck49 HOLDs** one cycle to validate its unexercised flood off-case.

**KEEPS (all with guide-mod AND/OR new-work signal):** deck135 (FROZEN but STRONGEST new-work of the wave —
owns + answered the reveal-seam charge, found ENGINE-R1, closed 2 carried items, surfaced R3/R4 + HARNESS-1;
keep IN through the ENGINE-R1/R4 fix confirmations, which only this seat runs); deck62 (REVISED round 3 —
edits A/B/C; decode-loop + channel-confusion need another corpus); deck35 (REVISED — land-drop discipline +
Gemhide-fixes-blue + mulligan blue-trap flag; attackers-non-issuance ledger #1 + land-drop tax-relocation
need re-validation); deck14 (REVISED — Rule #1B two-tier aggression; taught classes actively moving, FC2 died
FC1 mutated, E-NEW decisive-block residual); deck27 (NEWCOMER — first guide written, must run its first GUIDED
corpus for Step-0-ter validation).

**Incoming candidate:** rotating deck110 out opens the slot for the next queue entrant — **deck102
(Tergrid-1v1-guide)**, `deck102.txt` present, entering GUIDELESS (Step-0-bis initial guide) per the wave-18
anchor. **Wave-20 pool (explicit): 135, 62, 49, 35, 14, 27, 102** — seven decks (deck110 rotates out, deck102
enters, deck49 holds).

---

## (e) GUIDE-DEPLOYMENT LIST (byte-identical verification: wave19/deckN/strategy.txt vs live
`bin/Res/ai/baka/deckN_strategy.txt`, `cmp`-verified)

Working dir: `~/Projects/wagicGPT/projects/mtg`.

- **FROZEN — byte-identical to live (NO-OP, freezes hold):**
  - deck110 — 7931 B, byte-identical ✓
  - deck135 — 13357 B, byte-identical ✓
  - deck49 — 5283 B, byte-identical ✓ (freeze + rotation-candidate hold)
- **REVISED — DIFFERS from live, COPY wave19 → live:**
  - deck62 — live 7521 B → wave19 8401 B (round-3 edits: A generalize anti-spiral to "cannot win THIS turn,
    ahead OR behind"; B defensive protect-the-engine clause; C mulligan "decide from what the hand HAS")
  - deck35 — live 6777 B → wave19 8293 B (land-drop-discipline line + Gemhide-fixes-blue + mulligan
    blue-only-no-fixing trap flag; anti-durdle spine kept)
  - deck14 — live 5341 B → wave19 6851 B (Rule #1B two-tier aggression: deploy finishers / race to 6 mana /
    swing the whole board — the mutated slow-clock soft-durdle fix)
- **NEW — live file MISSING, COPY wave19 → live (needs `git add -f`; new files under `bin/` are gitignored):**
  - deck27 — 6496 B (the NEW Zombies initial guide, Step-0-bis; format-matched to a live guide)

```
# From ~/Projects/wagicGPT/projects/mtg
cp strategy-design/wave19/deck62/strategy.txt bin/Res/ai/baka/deck62_strategy.txt   # 7521 -> 8401 B
cp strategy-design/wave19/deck35/strategy.txt bin/Res/ai/baka/deck35_strategy.txt   # 6777 -> 8293 B
cp strategy-design/wave19/deck14/strategy.txt bin/Res/ai/baka/deck14_strategy.txt   # 5341 -> 6851 B
cp strategy-design/wave19/deck27/strategy.txt bin/Res/ai/baka/deck27_strategy.txt   # NEW 6496 B
# post-apply cmp verification:
for d in 62 35 14 27; do cmp strategy-design/wave19/deck$d/strategy.txt bin/Res/ai/baka/deck${d}_strategy.txt \
  && echo "deck$d applied OK"; done
# git staging (explicit files ONLY — never -A; never stage CLAUDE.md or the user's lexicon WIP):
git add    projects/mtg/bin/Res/ai/baka/deck62_strategy.txt \
           projects/mtg/bin/Res/ai/baka/deck35_strategy.txt   # tracked (edits to existing guides)
git add -f projects/mtg/bin/Res/ai/baka/deck14_strategy.txt \
           projects/mtg/bin/Res/ai/baka/deck27_strategy.txt   # deck14 pre-existing but bin/-ignored; deck27 NEW
```
(deck110/135/49 unchanged — nothing to stage. NO AI attribution on the commit — user's own public repo. Verify
deck14/deck27 tracked-status with `git check-ignore` before staging; anything under `bin/` needs `-f` if not
already tracked.)
