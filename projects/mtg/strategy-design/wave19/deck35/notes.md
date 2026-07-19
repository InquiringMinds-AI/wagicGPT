# deck35 -- development notes (engine / harness / model / construction) + rotation verdict

## Engine / representation ledger items

1. **[ENGINE BUG -- CONFIRMED + 2-wave convergent -- ESCALATE to build/verify] Declare-attackers
   decision is NOT issued to the GPT seat on some own-turns despite eligible untapped, unsick
   attackers.** Wave-18 ledger #2 raised this as a watch (vs110, no blockers record). This corpus
   CONFIRMS it at the attack step, twice independently:
   - **vs135 (adj LOSS):** deck35 dealt ZERO combat damage all game (opp translog has no
     "Damage: N dealt by ...Sliver" event; both opp -1s were its own fetchlands). On **turn 8**
     Horned Sliver (cast turn 6) was unsick + untapped and NO `attackers` decision was issued; the
     game continued to t9/t10 so it is not a turn-cap artifact. On **turn 10** the model's stated
     plan was explicitly aggressive and it still got no attack step before the cap. This is the
     PRIMARY cause of the adjudicated loss (the "0-attacker durdle" instrument fired, but the cause
     is engine, not the model).
   - **vs27 (WIN):** on **turn 4** (Striking t0 + Spinneret t2, both unsick) and **turn 6** (seq11
     prompt renders FOUR slivers, all untapped, plan "attack with all four for lethal") NO `attackers`
     decision was issued -- yet the SAME game issued attack steps normally on t2, t8, t10.
   The non-issuance is **inconsistent** (fires some own-turns, not others), which points at a
   conditional gate rather than a blanket skip. **Confound (why this is escalated as CONFIRMED-repro
   but not root-caused):** the translog cannot show the engine's own eligibility computation, so I
   cannot see WHICH condition suppresses the step. **Repro seqs handed to engine:** vs135 file
   `1784470862-ai_baka_deck35-...-vs-ai_baka_deck135.jsonl` (no `attackers` kind anywhere, board grows
   t6->t10); vs27 file `1784479371-ai_baka_deck35-...-vs-ai_baka_deck27.jsonl` (attackers at seq6/15/18
   only; missing at the t4 and t6 own-turns). **Where to look:** the combat/attacker-declaration gate
   in ActionLayer / GameObserver combat phase and how AIPlayerGPT's attacker step is entered -- what
   distinguishes an own-turn that offers the step (vs27 t2/t8/t10) from one that skips it (vs27 t4/t6,
   vs135 t8/t10). Hypothesis to test first: the step is skipped when the model's LAST main-phase action
   was a cast that left no "Cast nothing right now" prompt AND the newly-cast creatures are all
   summoning-sick, so the phase advances past combat before the pre-existing unsick attackers are
   offered. **This is the seat's #1 engine item and is ready for a targeted engine investigation now
   that a second seat-independent repro exists.**

2. **[REPRESENTATION -- wave-18 ledger #1 SHIPPED + VALIDATED -- CLOSE OUT] Block-seam trade-outcome
   annotations.** Wave-18 proposed annotating each candidate block with the computed result; the
   wave-19 engine batch shipped exactly this ("deal up to N -- you would be at M -- LETHAL/NOT lethal"
   plus per-blocker "you kill it, your blocker lives" / "your blocker dies, attacker lives"). At my
   seat it is PRESENT, ACCURATE (no misleading outcome printed), and it IMPROVED play: vs27 seq13 the
   model took the clean first-strike profitable block guided by the tag; vs49 seq17 (block at 1 life,
   10k chars) PARSED correctly. Block fallbacks went 1 -> 0. The attacker line now prints "deals N" as
   POWER separate from (X/Y), delivering the wave-18 G1 toughness-as-power fix in the representation.
   **Verdict: resolved and confirmed working -- retire this ledger item.** (The reasoning TAX at the
   block seam persists as long replies, but it no longer self-poisons; that residual is now subsumed
   by the equivalent-outcome discipline item in general-suggestions.md G1.)

3. **[MODEL WATCH -- carried from wave-17/18 #3, still unconfirmed, LOWER priority now] Gemhide
   auto-tap suppressing attackers.** Previously hypothesized as a cause of low attack counts in
   Gemhide-heavy games. This wave the low/zero attack counts are better explained by ledger #1
   (the engine not issuing the step at all) than by auto-tap tapping a would-be attacker -- in vs27
   the un-offered attackers were rendered UNTAPPED, so auto-tap was not the cause there. Keep the
   watch open but treat ledger #1 as the leading explanation; only re-open auto-tap if a future corpus
   shows an eligible attacker rendered [tapped] specifically because a colored cost auto-tapped it.

## Deck-construction observations (recorded; not my call to change)
- **Blue availability is the SWING variable for this deck -- 3rd corpus confirming it.** vs62 flipped
  LOSS->WIN this wave precisely because blue came online (Island rendered, Winged cast); vs135's two
  fallbacks AND its 0-damage draw both trace to a hand of three blue Winged Slivers with no Island and
  a late Gemhide. When construction is tuned: raise blue sources or move the evasion off a hard blue
  pip. The guide now adds a Gemhide-fixes-blue line + a mulligan trap-flag for blue-only-no-fixing
  hands, but the guide cannot draw an Island.
- **Shifting Sliver (the premier win-con) has now gone undeployed for THREE full corpora** -- {3}{u},
  same blue-scarcity cause. Never provably a misplay (never castable when relevant), but the deck's
  best card has never hit the board. Construction signal.
- Both non-variance losses were to the pool's fastest (110 Affinity, dead t6) and, last wave, biggest
  (62 aura giant). A base-1/1 ground swarm is structurally soft to a turn-6 clock; not guide-fixable.

## Rotation verdict
**NOT a rotation candidate.** Clear new-work signal on all three axes: (a) the guide produced a
REVISION this wave (land-drop-discipline line + Gemhide-fixes-blue + mulligan blue-trap flag) targeting
the newly-relocated tax, so per Step 0-ter a revised guide carries a new-work signal by construction;
(b) a validated DIED class held (FC1 durdle, 2nd wave) AND a wave-18 revision was validated
(block-seam fallback 1->0); (c) the seat surfaced a now-2-wave-convergent, seat-independent-reproducible
ENGINE BUG (attackers non-issuance, ledger #1) that is the deck's single biggest result-affecting issue
and is ready to hand to engine. Concrete next-corpus work: (1) re-validate the land-drop-discipline
revision (do the land-drop fallbacks disappear? does the tax relocate again, and to where?); (2) confirm
whether the engine attacker-non-issuance fix lands and the vs135-class 0-damage games disappear; (3)
watch the blue-only-no-fixing mulligan flag in action.

## Engine items count: 3 (item 1 ESCALATE-to-build, item 2 CLOSE-OUT/resolved, item 3 lowered WATCH).
