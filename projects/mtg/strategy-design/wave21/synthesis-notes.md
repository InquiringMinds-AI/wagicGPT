# Wave-21 synthesis notes — SEVENTH per-deck-rotation cycle

Unified from seven independent deck-agent reviews mining the wave-21 PRIMARY corpus
`matchups-20260723-084938` (21 games round-robin over 135/62/49/35/14/27/102, binary `/tmp/wagic-3be7f5a67`). Pool
UNCHANGED from wave-20 — a clean **7-of-7 repeated-opponent control**, so every record delta isolates to draw/matchup
variance. **deck102 (Tergrid)** played its FIRST GUIDED corpus (Step 0-ter → three surgical edits); **deck14** round 3
(one DEMOTION diff); **deck49** round 2 (byte-frozen — edits validated); **decks 135/35/62/27 FROZE** byte-identical.
Binary 3be7f5a67 carried the wave-21 engine batch (answer-first + parser hardening + reveal framing + wither
annotations + attacker-sparing + mana-count integrity); THIS CORPUS is that batch's live validation.

Corpus health: fallbacks **0.7% (8/1119)** — a 4x drop from wave-20's 3.0% (32/1067), **composition completely
changed: 0 unparsed_reply, 0 empty_reply** (wave-20 had 29 unparsed), **5 retracted_choice** (NEW deliberate safety
route), **3 stale_echo**; 0 prose salvages fired. 8th consecutive clean corpus, 0 segfaults, 0 timeouts, 0 draws, 4
life-adjudicated at cap. Per-seat fallbacks: 62:4, 135:1, 14:1, 35:1, 49:1, 27:0, 102:0. Latency median 15.9s, p90 70s;
reply length p50 1.0k / p95 8.2k / max 16.9k chars.

**Core (general prompt): PASS, 16th consecutive.** No `general-strategy.txt` produced (no change to verify — see
"Core prompt" below). Every wave-21 finding routes BELOW core (per-deck guide) or to the engine/harness/decode ledger.
The one recurring cross-seat item (the >12k-char decode-repetition spiral) is explicitly a decode-sampler concern
(repetition_penalty), NOT a core line; do not double-layer. Skill METHOD gained rungs (parser-safety-route over-fire
audit; answer-first validation discipline; the crutch-lifecycle second half of the demotion terminus; silent-failure
state-delta scoring) — guide-authoring/reviewer-instrument method, which does not reset the core PASS count.

## Per-seat verdict table

| deck | archetype / status | record (Δ) | fallbacks | guide disposition | rotation |
|------|--------------------|-----------|-----------|-------------------|----------|
| 135 | Modern Snow · frozen veteran | 1/6 (↓ from 4/6) | 1 (retracted_choice false-pos) | **FROZEN** (byte-identical, `cmp`-verified) | KEEP — ENGINE-F1 witness; exit conditions NOT met |
| 62 | Enchantresses · revised round 5 | 4/6 (↑ from 3/6) | 4 (3 retracted false-pos + 1 stale_echo) | **FROZEN** (byte-identical; edit D validated) | KEEP one more — held only as N9 harness-witness |
| 49 | Dragons · revised round 2 | 3/6 (↓ from 4/6) | 1 (stale_echo false-pos) | **FROZEN** (byte-copy; edits validated) | **ROTATE OUT → canary** (guide converged) |
| 35 | Mighty Slivers · frozen round 4 | 3/6 (flat) | 1 (retracted_choice — CORRECT fire) | **FROZEN** (byte-identical) | **ROTATE OUT → canary** (keep-reason discharged) |
| 14 | Deep Blue · revised round 3 | 2/6 (↓ from 3/6) | 1 (stale_echo — spell-name echo) | **REVISE** — one DEMOTION diff (−617 bytes) | **ROTATE OUT → canary** (all 3 conditions met) |
| 27 | Zombies · frozen (wave-20 revision live) | 5/6 (↑ from 3/6) | **0** | **FROZEN** (byte-identical; revision UNVALIDATED) | STAY — late-stall off-case unexercised + 3rd-instance decode |
| 102 | Tergrid · FIRST GUIDED | 3/6 (↑ from 1/6) | **0** | **REVISE** — three surgical edits (+713 bytes) | STAY — first-guided, Tergrid unexercised, 3 new edits |

Record note (context only, win column is NOT a verdict): the pool is a repeated control, so flips isolate to draws. The
big swings decompose to non-guide causes — **deck135 4/6→1/6 is ENGINE-F1 fetchland-fizzle incidence** (its only win is
the only game with resolving fetches; the three flips are its three worst fetch-fizzle games), **deck27 3/6→5/6 is
mostly draw/opponent variance** (3 favorable flips + 1 unfavorable on a repeated control; the durable signal is the
0-fallback tax collapse), **deck14 3/6→2/6 and deck49 4/6→3/6 are draw/matchup variance** (decision quality rose at
both). No record delta traces to a guide line.

## Validation-focus outcomes (the wave-21 engine batch, items a–j)

- **(a) Partial attacker-sparing payment — VALIDATED CLEAN. CLOSE.** (deck35 owns.) 0-attacker games GONE (vs49 now
  issues an attack T5; wave-20's complete 0-attacker loss does not recur); no Sliver ever tapped for mana with a land
  available across 6 games; the max-mana-pressure positive case (vs14 T8: `{4}{g}`=5-mana 5-drop paid from all 5 lands,
  THREE non-haste attackers spared, all three attacked) confirms the reframe. **This SUPERSEDES deck35's own wave-20
  "combat-gate non-issuance" root-cause hypothesis** — the gate was always correct; wave-20's all-or-nothing sparing
  tapped the attackers on the turn's last cast. No new pathology from over-sparing.
- **(b) ANSWER-FIRST reply protocol — VALIDATED at the parse layer, cross-seat. CLOSE (parse symptom); the decode
  disease stays OPEN.** 0 unparsed / 0 empty corpus-wide (29 unparsed in wave-20); the coded line is FIRST in 100% of
  sampled p95+ replies at every seat (deck35 87/87, deck27 135/135, deck14/62/49/102/135 all clean). No answer-QUALITY
  regression from committing-first (coded first line matches the following reasoning). BUT the underlying >12k-char
  decode-repetition spiral is undiminished — it now surfaces as retracted_choice false-positives (HARNESS-N9), a
  stale_echo false-positive (HARNESS-49a), and a hidden-inside-a-clean-parse miscount (deck135 Azcanta). Answer-first
  fixed the SYMPTOM (parse), not the DISEASE (the spiral). Two latent exposures flagged (see reconciliation d).
- **(c) Parser hardening — SPLIT verdict.** (i) **retracted_choice route: OVER-FIRES — OPEN (→ HARNESS-N9).** 4 of the
  5 corpus retracted_choice are FALSE POSITIVES (deck62 owns 3, deck135 1); only deck35 vs62 s18 was a CORRECT fire.
  (ii) **last-well-formed-CHOICE-wins: VALIDATED. CLOSE.** Took the reasoned later line at deck62 (vs14 s26, vs135
  s21), deck27 (vs35 s18), deck135 (vs49 s26, vs62 s9) — quality-preserving, not just parse convenience. (iii)
  **template-placeholder-CHOICE drop: UNTESTED** (0 `[Number]`/`[Name]` parrots this corpus; answer-first suppressed
  the wave-20 N7-template class — nothing to drop). OPEN/untested.
- **(d) Prose-intent salvage (combat) — 0 fired. UNTESTED (consistent with answer-first pre-empting the need).** Two
  seats (deck35 G2, deck49) request EXTENDING it to the priority/casting menu for the commit-then-change-mind case
  (deck35 vs62 s18: line-1 CHOICE went stale, the PLAN stated "Cast Fury" — a prose-intent read would recover the
  intent deterministically instead of relying on the heuristic argmax). OPEN as a low-priority hardening.
- **(e) Reveal framing (whose-hand + pick-EXACTLY-ONE) — VALIDATED CLEAN. CLOSE.** (deck102 owns.) Both reveal records
  render "The opponent revealed their hand … choose the ONE card"; 0/2 over-picks (wave-20 was 2/3; vs14 s7 is the SAME
  hand that over-picked both Boomerangs in wave-20, now a single clean pick); named card left the hand exactly once in
  `events`; the "top N of your library" mislabel is gone. Also closed the deck102 guide crutch (retired, edit 1).
- **(f) Bounce-on-stack annotation + header — VALIDATED. CLOSE.** (deck14 owns.) CAST annotation `[this cannot target
  the spell on the stack]` fired on 26 cast windows; model chose "Cast nothing" on 25/26; the 1 cast (vs62 s18) was a
  CORRECT aura-fizzle bounce; the 1 live target menu (vs62 s19, header present) picked the opponent's 9/9, not its own
  creature. 0 self-bounces, 0 in-class fallbacks. Clean A/B vs wave-20's same-seat same-matchup self-bounce misplay.
  TRIGGERED deck14's demotion of the interim guide trap.
- **(g) Wither trade annotations — VALIDATED, zero false positives. CLOSE.** (deck27 owns.) All 3 Oona's Gatewarden
  pairings correct, including the strictly-harder vs35 s20 (six 9-10/7-8 tramplers: "attacker lives" + shrink math
  9/7→7/5, 10/8→8/6 + trample power−1, all six correct). The wave-20 "(both die)" false-positive class — which directly
  caused the wave-20 vs62 s6 fallback — is gone; 0 wither-induced fallbacks this corpus.
- **(h) Baka blocker fallback (0-power skip / no 2nd-blocker pile) — UNEXERCISED. OPEN (→ N8).** (deck62 owns.) Zero
  blocker fallbacks this corpus (answer-first made block fallbacks rare — every block committed a first-line `BLOCKS:`),
  so the heuristic block path never ran. N8 remains code-shipped-but-unvalidated; needs a seat where a block decision
  actually falls back with a valuable 0-power body present.
- **(i) Mana-count integrity — item-8 (a)/(b) CONFIRMED; residual (c) reclassified. CLOSE (a)/(b), OPEN as ENGINE-F1.**
  (deck102 owns secondary.) No cast-stall loop anywhere (the wave-20 Witch-of-the-Moors / Dreadhorde-Invasion x3 loops
  did not recur; vs62 casts Dreadhorde ONCE); "Already in pool" never coincides with a repeat cast. The item-8
  counting fix + plan-abort are working. The OPEN item-8(c) residual surfaced instead as the fetchland-fizzle
  (ENGINE-F1, deck135) — see reconciliation c.
- **(j) max_reply_tokens / repetition_penalty — repetition_penalty was OFF this corpus. OPEN.** Decode behavior
  unchanged by config. Multi-seat convergence recommends a **modest repetition_penalty pilot next corpus** (see
  reconciliation e).

## Cross-seat reconciliations

**(a) retracted_choice detector OVER-FIRES — reconciled; discriminating shape specified.** deck62's 3 (vs27 s10,
vs135 s5, vs102 s3) and deck135's 1 (vs27 s26) are all FALSE POSITIVES; deck35's 1 (vs62 s18) is a CORRECT fire. The
discriminator: in every over-fire the reply has exactly ONE well-formed, in-range `^CHOICE:` line (the correct
answer-first first line), NO contradictory second CHOICE, and the retraction phrase ("Wait"/"Correction:") lives in the
DOWNSTREAM PLAN body about an unrelated point (a future cast's shroud target, mana arithmetic, Islandwalk mechanics).
The one CORRECT fire (deck35 vs62 s18): the model emitted `CHOICE: 3 (Cast Heart)` first, then reasoned to a DIFFERENT
card and its PLAN concluded "Cast Fury" — a genuine disavowal of the choice itself with no clean re-emitted CHOICE.
**Fix (HARNESS-N9): fire retracted_choice ONLY when (i) the retraction targets the CHOICE/decision itself AND no valid
CHOICE remains, OR (ii) a CONTRADICTORY second coded line is emitted (already handled by last-well-formed-wins). A
single un-superseded, in-range CHOICE with no contradictory successor must be TAKEN regardless of prose "Wait/Correction"
in the reasoning body.** This must preserve the genuine revise-and-replace case (a SECOND coded line → last-wins),
distinguishable by the second coded line. deck62 is the ideal fix-validator (owns 3 of 5).

**(b) Stale-echo family — TWO distinct parser items, not one.** (1) **Spell-name-prefix on a target menu (deck14 vs27
s29 "Unsummon targeting Inkfathom Infiltrator" + deck62 vs14 s29 "Web targeting Yavimaya Enchantress"), 2-seat
convergent.** `CHOICE: N (<spell> targeting <target>)` where N is the correct in-range target and the target name is a
SUBSTRING of the parenthetical, but the leading spell-name qualifier breaks the option-N name match → stale_echo →
heuristic. Fix: when a `CHOICE: N (...)` has an in-range N whose option-N name appears as a substring of the
parenthetical, prefer N (also, the in-range index itself pointed at the right option in both cases — an index-wins
tiebreak would recover it). (2) **Near-duplicate-PLAN fingerprint (deck49 vs35 s18), distinct mechanism.** A valid
answer-first `CHOICE: 1 (Cast Goblin War Paint)` was routed to stale_echo because its PLAN block re-stated the prior
turn's PLAN nearly verbatim (a re-deriving reasoning-tax spiral) and the stale_echo guard fingerprints the reply/PLAN
against the prior turn. Fix: do NOT fire stale_echo when the current reply carries a well-formed, LEGAL, order-agnostic
CHOICE line that DIFFERS from the prior turn's CHOICE (here seq17 was "Play Mountain", seq18 "Cast War Paint" —
different decisions, same PLAN narrative). Both are HARNESS-lane, low harm this corpus (all benign), latent-consequential.

**(c) ENGINE-F1 fetchland fizzle (deck135) — the corpus's top new engine finding; a clean NEW repro of the OPEN
item-8(c) class, on fetchlands.** A fetchland `Put in Play` / `search basic land` activated ability (cost
Tap+Life+Sacrifice) intermittently pays Tap+Life then ABORTS before the Sacrifice+search: no land enters, the fetchland
stays tapped, the option re-arms (repro vs35 s6 verbatim: `Your life -1` with NO `Windswept Heath: battlefield ->
graveyard` and NO `Snow-Covered Forest: -> battlefield`). It is intermittent (proven by contrast: vs62's Flooded
Strand resolved fully, the only win). **Relation to item-8(c):** the wave-21 `item8-diagnosis.md` explicitly left
layer (c) OPEN pending a discriminating probe; ENGINE-F1 is exactly that class caught on the fetchland Sacrifice
sub-click, and both item-8(c) candidate mechanisms fit — (i) `ActivatedAbility::isReactingToClick`'s
`source->controller() == game->currentlyActing()` requirement failing if the acting player shifted between clickstream
ticks (the fetch cracks at a PRIORITY window), or (ii) stale `MTGAbility*` producer/ability pointers between plan tick
and click tick. **Relation to the wave-21 clickstream-abort change (item-8 fix (b)) — FLAG FOR THE FIX INVESTIGATION,
DO NOT ASSERT:** wave-21 (b) ("verify the click landed; on refusal abort the remaining plan") IS in this code path, and
the "pay-life-then-fizzle" signature is consistent with (b) having converted item-8's original "float mana / loop" into
"pay a partial cost then abort." This cannot be discriminated without wave-20 fetch-level data or a code probe. Either
way the fetch does not resolve. Companion **ENGINE-F1b:** the fizzle is SILENT to the model (no negative event; its own
plan narrates success), so it never re-cracks — an engine event on an aborted activation would let it recover. FIX
direction: verify the Sacrifice sub-click lands; on refusal, roll back the already-paid Tap+Life (never leave a partial
cost paid) or re-resolve the ability by source card; emit an aborted-activation event. VALIDATION DETECTOR next corpus:
a fetch CHOICE followed by a life payment with NO fetchland `-> graveyard` and NO `Snow-Covered X: -> battlefield`.

**(d) Answer-first VALIDATED cross-seat (0 unparsed) — two latent exposures named.** (1) **snap-then-revise truncation
exposure (deck62):** for a reasons-then-revises model the snap first-line answer is often the WORSE option and the model
reasons to a better one (vs14 s26 snap `CHOICE: 4` → reasoned `CHOICE: 2`; vs135 s21 snap `BLOCKS: B2:A1` → reasoned
`BLOCKS: B2:A2` saving 8 life). "Last-well-formed-wins" rescued both, so no regression THIS corpus (0 truncations) —
but a reply that truncates AFTER the snap answer and BEFORE the correction would execute the worse snap answer with no
rescue. WATCH: flag any truncated reply whose parsed (snap) answer diverges from where its cut-off reasoning was
heading. (2) **judge co-firing fixes by seam reply-LENGTH, not fallback count (deck49 method point):** answer-first
would make a still-oscillating spiral PARSE, masking a failed guide fix behind a 0-unparse count. deck49's develop-seam
fork resolved in 719–1202 chars across 6/6 games (SHORT = the guide edit genuinely dissolved the trigger, not merely
parse-absorbed); the residual tax in OTHER pockets stays LONG-but-parsed. Always length-check the seam.

**(e) Decode-repetition spirals persist (parse clean now) — RECOMMEND a repetition_penalty pilot next corpus.** Four
seats converge that the >12k-char verbatim-repetition spiral is the undiminished DISEASE that answer-first only masked
at the parse layer: deck135 (Azcanta miscount inside a 15k spiral; HARNESS-N9 over-fire rode a 13k spiral), deck35
(12.5k chars on a LAND DROP; "I die." ×4), deck27 (vs135 s18 the 3rd-instance ahead-position repetition loop, crossing
the wave-20 hold-for-3 threshold — but harm DEFUSED by answer-first), deck49 (the spiral now trips HARNESS-49a stale_echo
instead of truncating). The config key `repetition_penalty` shipped but is OFF. **Decision: enable a MODEST
repetition_penalty pilot next corpus (plus keep the max_reply_tokens guard).** Rationale: it no longer buys correctness
(answer-first did that) but it buys LATENCY (p90 70s) and token cost at every seam, AND it pre-empts the new
false-positive exits (a shorter committed reply would not near-duplicate the prior PLAN → no HARNESS-49a; would not
spiral 13k chars → less HARNESS-N9 fuel). deck27's read: enabling it "would likely kill the loop." Keep it at the
decode-sampler layer, never the guide (the model quotes correct guide lines mid-loop). This is a PILOT, not a fixed
value — pick a conservative penalty and re-check the ahead-position priority giants; back off if it degrades legitimate
long combat-math replies.

## Core prompt: PASS (16th consecutive) — no change

Every seat routed core to PASS; no proposal is both 2+-seat convergent AND properly a core-PROMPT line. Detail: the
decode-repetition spiral (the one strong cross-seat item) is a decode-sampler concern, not core prose (deck135 #4,
deck35 G1, deck49 G-49a explicitly NOT promoted — single-seat, and the guide layer already dissolves its develop-seam
trigger; do not double-layer). Answer-first is a shipped reply-protocol/harness change validated as a net positive, not
a core-body edit. All engine/harness/card items (ENGINE-F1, HARNESS-N9, R-PAINLAND, ENGINE-R6, Ashenmoor drift, N8)
live in the ledger, never in prompt text. The core has now passed 15 waves; nothing this wave clears the bar to touch
it. No `general-strategy.txt` produced.

## WAVE-22 ENGINE LEDGER (ranked; seat notes.md carry the seqs)

1. **ENGINE-F1 — fetchland activations intermittently FIZZLE (deck135, GAME-DECIDING, HIGHEST).** A Tap+Life+Sacrifice
   fetch pays Tap+Life then aborts before Sacrifice+search; land never enters, option re-arms, model can't perceive it.
   Primary driver of deck135's 4/6→1/6. Clean NEW repro of the OPEN item-8(c) class. Companion **ENGINE-F1b** (silent
   to the model → needs an aborted-activation event). Fix: verify the Sacrifice sub-click lands; roll back partial cost
   on refusal / re-resolve by source card; emit the abort event. FLAG: wave-21 item-8(b) clickstream-abort may have
   changed the failure signature (float→partial-cost-abort) — investigate, do not assert. Affects every fetch-based deck.
2. **HARNESS-N9 — retracted_choice route OVER-FIRES (deck62 witness, HIGH).** 4 of 5 corpus retracted_choice are false
   positives (discards a correct, single, un-superseded CHOICE over downstream prose "Wait/Correction"). Fix: require
   the retraction to reference the CHOICE, or a contradictory second coded line. Latent-consequential on CAST/TARGET
   asks; deck62 is the ready validator. (Discriminating shape settled — see reconciliation a.)
3. **R-PAINLAND — self-damaging mana sources hide their life cost (deck102, MEDIUM, cross-deck).** Ancient Tomb / City
   of Traitors deal 2 to the controller per tap, invisible in the "Mana available" line and cast-cost render; the model
   self-killed (deck102 vs27 cast Damnation via Ancient Tomb at 3 life → died). Fix: annotate self-damaging producers
   in the mana line / cost render ("Ancient Tomb deals 2 to you when tapped"). Generalizes to all painful mana sources.
   Interim guide crutch shipped (deck102 edit 3).
4. **ENGINE-R6 — conditional-transform options lack an eligibility annotation (deck135, MEDIUM; also unblocks R4).** The
   Azcanta `Transform:` option is offered only when the 7-cards-in-graveyard threshold is met but carries no annotation
   saying so; the model manually recounts, gets it wrong, and DECLINES a free beneficial transform twice (vs14 s29/s40).
   Fix: extend the reveal eligibility-surfacing pattern — `Transform:Azcanta [available now — 7+ cards in graveyard]`.
   Generalizes to any threshold-gated ability menu (delirium/threshold/metalcraft/ferocious). Blocks **ENGINE-R4**
   (Azcanta ACTIVATION path still UNTESTED — the transform is never accepted, so no activation window is created).
5. **Stale-echo family (2 parser items, LOW–MEDIUM).** (a) spell-name-prefix on a target menu (deck14 + deck62,
   2-seat): prefer the in-range N when the option name is a substring of the parenthetical. (b) near-duplicate-PLAN
   fingerprint (deck49): don't fire stale_echo when a well-formed LEGAL CHOICE differs from the prior turn's CHOICE.
6. **CARD Ashenmoor Liege 4/1 vs real 4/4 (deck49, MEDIUM — Oracle-verify FIRST per project rule).** Primitive
   `mtg.txt` has power=4 toughness=1; real Shadowmoor Oracle is a 4/4 ("Other black/red creatures you control get
   +1/+1; whenever a source deals damage to Ashenmoor Liege, that source's controller loses that much life"). Now
   game-material: deck49 lost its only anthem to a "(both die)" block a true 4/4 would have won (deck27 s15). FAITHFUL
   data-only fix: set `toughness=4` (and separately correct the damage-punisher text drift). Roster/card-owner decision
   (a shared primitive change affects deck power level + other seats' renders) — NOT a per-deck-review edit. Escalated
   from "cosmetic/queued" through wave-20 to game-costing in wave-21.
7. **Decode-time repetition-penalty / max-token guard (cross-seat, MEDIUM-by-frequency, harm DEFUSED).** The disease
   behind every >12k spiral; `repetition_penalty` shipped-but-OFF. RECOMMEND a modest pilot next corpus (see
   reconciliation e). Priority dropped because answer-first defused the correctness harm; it now buys latency/token cost
   and pre-empts HARNESS-N9/HARNESS-49a fuel.
8. **N8 — baka blocker-fallback gang-chumps a 0/1 engine piece (deck62, MEDIUM, code-shipped item (h), UNEXERCISED).**
   The 0-power-skip / no-2nd-blocker-pile fix could not be validated (0 block fallbacks this corpus). Keep OPEN; needs a
   seat where a block decision falls back with a valuable 0-power body present. If answer-first keeps block fallbacks
   rare, validation must come from a probe deck.

**Carries (unchanged/low):** ENGINE-R4 Azcanta activation path (UNTESTED — blocked on R6); prose-intent salvage
extension to the priority/casting menu (deck35 G2, deck49 — the commit-then-change-mind analogue of the combat
salvage); `#N` non-combat duplicate-name target-menu disambiguation (parse-side real but UNEXERCISED end-to-end);
Gravelgill Axeshark ({4}{u/b} 3/3 islandwalk/persist vs paper {4}{U} 4/3) + Counsel of the Soratami mana-cost
divergences (deck14, Oracle-verify, roster owner); deck49 lower-priority Oracle queue (Bloodmark 1/1-vs-2/1, War Paint
{1}{R}/+2+2 vs {R}/+1+1, Crucible {3}{R} vs {2}{R}{R}); Force of Negation reflexive-off-whitelist watch (deck135,
single low-signal firing, stays at WATCH).

**CLOSED this wave:** ENGINE-R1 reveal-partition finalize (n=2 confirmed — deck135 vs14 s23: 6 picks → 6 to-hand, 0
graveyard; regression-watch only); wither trade-annotation (item g, deck27); attacker-sparing PAYMENT fix (item a,
deck35, validated clean); bounce-on-stack representation (item f, deck14 — annotation + header fired + obeyed); reveal
framing whose-hand + pick-ONE (item e, deck102); item-8 counting + plan-abort (a)/(b) (deck102, residual (c) → ENGINE-F1);
last-well-formed-CHOICE-wins (parser, item c-iii); combat `options_text` (carried closed).

## Rotation decisions

Standing test: a seat is a rotation candidate when its guide takes NO modification (this wave's byte-freeze) AND no
seat-specific, GUIDE-LANE new-work signal survives. Where the two gates disagree because the new-work is HARNESS/ENGINE
lane rather than guide-refinement, name which gate holds the seat (wave-21 rotation-discipline rung). Applying honestly:

- **135 — KEEP (in-pool frozen).** Guide frozen (byte-identical, `cmp`-verified), but the new-work signal is
  overwhelming and seat-specific: ENGINE-F1 fetchland fizzle (the corpus's top new engine finding, orphaned if it
  rotates), ENGINE-R6 Azcanta transform, HARNESS-1/N9 witness, and it is the ONLY seat exercising the reveal / fetch /
  Azcanta seams. Its wave-20 exit condition (Azcanta activation resolving correctly) is NOT met (the transform was
  declined twice). Far from a rotation exemplar this wave.
- **62 — KEEP one more (in-pool frozen), held ONLY as the N9 harness-witness.** The GUIDE is CONVERGED (edit D
  validated, its target spiral class dead; every taught class held; byte-freeze earned) → on guide grounds it would
  rotate. The sole hold is that it is the corpus's prime witness for the HARNESS-N9 over-fire (owns 3 of 5) and the
  ideal fix-validator. State it plainly: guide = rotation-eligible; held only as harness-witness for N9. Becomes a clean
  rotation candidate the moment the N9 fix ships and is confirmed at this seat.
- **49 — ROTATE OUT → canary.** Guide CONVERGED (byte-copy; the wave-20 two-hunk revision validated — develop-seam
  oscillation dead 6/6, burn-face 13/13). All residual work is off-guide (HARNESS-49a → harness; CARD-49-ashenmoor →
  card owner; G-49a decode tax → sampler). No deck49-guide-lane signal remains. Guide stays deployed byte-identical as
  a control.
- **35 — ROTATE OUT → canary.** Guide FROZEN. Its NAMED wave-20 keep-reason (the open attacker-non-issuance work its
  shape uniquely surfaced) DISCHARGED — the payment fix validated clean and its own prior root-cause hypothesis is
  settled/superseded, with the max-mana-pressure positive case (vs14 T8) providing the strongest-possible proof. The
  residuals (answer-first stale-line watch; decode tax) are non-seat/cross-deck. Per the discharge rung, do not
  manufacture a fresh hold. Guide stays deployed as a canary. (Payment-fix second-corpus confirmation carries as a
  ledger watch that any aggro replacement can discharge.)
- **14 — ROTATE OUT → canary.** All three wave-20 rotation conditions met: (1) the bounce-on-stack trap LANDED (25/26
  declines, 1 menu correct, 0 self-bounces); (2) E1/E2 engine fixes shipped AND confirmed (items f + b); (3) no new
  STRATEGY class emerged (the only new item, the spell-name-echo stale_echo, is parser-owned; M1 defensive-slide is a
  WATCH entangled with opponent improvement, guide already covers it). The single wave-21 diff is a terminal-lifecycle
  DEMOTION (compressing a solved leak), not new leak-work. Guide (with the demotion) stays deployed as the canary.
- **27 — STAY.** Guide frozen, but its deployed late-stall durdle revision is UNVALIDATED (its walled-ground-stall
  trigger did not occur — vs14 was won by an evasion draw) → not stable-proven; AND the ahead-position degeneration
  loop reached its 3rd instance (crossing the hold-for-3 threshold). Re-validate next corpus with focus on a
  walled-ground grind (bias opponents toward ground-wall decks if the harness allows).
- **102 — STAY.** First guided corpus (Step 0-ter), never a first-pass rotation candidate; produced three real guide
  edits (new-work by construction); and its keystone engine payoff (the Tergrid steal) has NEVER been exercised (0/12
  games across two corpora — a probe deck is the validation path). Re-validate the three edits + attempt the Tergrid
  steal via a probe next wave.

**Three seats rotate OUT to canary (14, 35, 49); guides stay deployed byte-identical.** This retires three
independently-converged seats and, with deck62 held only as the N9 witness, expends most of the banked value of the
7-of-7 repeated-opponent control (it ran twice, waves 20–21 — attribution is banked). The four stayers (135/62/27/102)
preserve partial wave-21 continuity, so wave-22 is not a full pool reset.

**Replacements — bring IN three older-guided decks for re-validation under the current engine batch** (whole-deck-set
campaign scope; these were reviewed in earlier waves and rotated out, their guides deployed but never re-tested against
the heavily-changed 3be7f5a67 engine). Chosen for archetype coverage of the seams the rotated-out seats exercised:
- **deck109 (Hellrider, Mono-Red Aggro/RDW)** — the natural deck49 successor: an aggro-with-reach deck that states BOTH
  "deploy every turn" and "burn goes face," so it directly re-validates the co-firing-fork tiebreak template on a FRESH
  deck (deck49 G-49b flagged exactly this). Keeps burn-face + develop-seam coverage.
- **deck44 (Faerie Archmage, UB Faerie Tempo, all-flying)** — keeps an aggro/evasion/attack-every-turn seat to exercise
  the attacker-sparing/combat-issuance seam and the payment-fix second-corpus confirmation (the deck35 watch), plus the
  evasion win-path.
- **deck140 ("Wipe Them Out", RBW Control, 14 board wipes, almost no creatures)** — the strategically-highest-value
  pick: it is the SECOND symmetric-mass-effect deck, which the wave-20 changelog explicitly flagged as the trigger to
  consider the symmetric-trap fire-gate for CORE-PROMPT promotion (deck102's Pox/Smallpox gate is currently a
  single-seat guide line). Also adds a control/wrath axis absent from the pool and stresses the 601.2c creatureless
  cast filter.

All three already have deployed guides (`deck{44,109,140}_strategy.txt` present in `bin/Res/ai/baka/`), so they enter
as REVISED seats re-validating older guides, not guideless newcomers (lower authoring cost than three fresh Step-0-bis
initial guides, and re-validation of aging guides against a new engine batch is high-value). **Owner degree of freedom:**
if pure coverage-expansion is preferred over old-guide re-validation, any of these may be swapped for a fresh guideless
deck from the ~148 unreviewed roster (Step-0-bis newcomer) — that is a roster-owner call.

**WAVE-22 POOL: 135, 62, 27, 102, 109, 44, 140** (4 stayers + 3 rotated-in). Per-seat wave-22 focus:
- 135: confirm the ENGINE-F1 fetchland-fizzle fix (detector: a fetch CHOICE + life payment with NO fetchland
  `-> graveyard` and NO `Snow-Covered X: -> battlefield`); confirm ENGINE-R6 Azcanta transform annotation and an
  Azcanta activation finally resolving (retires the reveal/fetch/Azcanta charge → rotation candidate); confirm the N9
  fix stopped the retracted_choice false-positive on its single-CHOICE spirals.
- 62: confirm the N9 fix stopped the retracted_choice over-fire on deck62's settled-decision replies WITHOUT breaking
  the genuine revise-and-replace last-wins case; any block fallback for N8; any answer-first
  truncation-between-snap-and-correction. (→ rotation candidate once N9 confirmed here.)
- 27: re-validate the late-stall durdle revision in a genuine walled-ground grind (evasion drought); re-check the
  ahead-position decode loop under a repetition_penalty pilot.
- 102: re-validate the three edits (does edit 1 keep pick-ONE clean now the guide no longer describes the mislabel;
  does edit 2 cut the marginal symmetric-edict-into-evasion cast; does edit 3 reduce painland self-death); attempt the
  Tergrid steal via a probe deck.
- 109/44/140: baseline the older guides against the current engine batch; 109 = co-firing-fork template re-validation +
  burn-face; 44 = attacker-sparing/evasion second-corpus confirmation; 140 = the second-symmetric-deck test that gates
  the symmetric-trap fire-gate core-promotion question.
