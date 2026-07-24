# deck133 wave-23 — development notes (engine / harness / model / construction) + ROTATION VERDICT

Corpus PRIMARY `matchups-20260724-013710`, binary `0e6361732`, pilot repetition_penalty=1.05.
Real games: 140(W), 102(W), 109(W), 137(W-adj), 131(W), 27(L) — **5W/1L, corpus TOP seat.**
Layer-routing enforced: engine/harness/model/construction live HERE with seq repros; the guide got
two surgical ADDs (findings.md 3+4); win column is context-only.

## ENGINE — [NEW, MED, non-deciding this game but caused an 85s spiral + a fallback] Yawgmoth ability offered with NO sacrificeable creature.
**vs137 seq26, priority (opponent's attack step), my18/opp18.** The engine offered as a legal action:
`-1/-1 Counter with Yawgmoth, Thran Physician targeting Venerated Loxodon [cost: Life, Sacrifice]`,
card text "Pay 1 life, **Sacrifice another creature**: put a -1/-1 counter ... draw a card." deck133's
serialized battlefield at that moment: `Your battlefield (creatures: 1): ... Yawgmoth, Thran
Physician` (Liliana the Last Hope is a planeswalker, not a creature; Bloodghast died on turn 12 and is
in the graveyard). There is NO "another creature" to sacrifice -> the activation cost is **UNPAYABLE**,
yet it was listed as legal. The model correctly diagnosed the impossibility and spiraled ~85s trying
to reconcile "the game says it's legal" against "I have no creature to sacrifice," ultimately flagged
retracted_choice; the heuristic then just blocked with Yawgmoth (safe). **Repro:** grep the vs137
deck133-seat log for seq26; the prompt's battlefield line reads `(creatures: 1)` while option 1 demands
a "Sacrifice another creature" cost. **Fix candidate:** gate the Yawgmoth (and any "Sacrifice another
creature") activated-ability offer on `count(other controlled creatures) >= 1`. Non-deciding (deck133
won 25/~6) but it is a legality-filter gap that directly produced a fallback and a long spiral.

## HARNESS/PARSER — [NEW, precious post-INDEX-WINS evidence] retracted_choice appears to OVER-FIRE on self-correction PROSE when the coded CHOICE index is never actually changed.
deck133 owns ALL 3 of the corpus's retracted_choice fallbacks. The discriminator = count coded
`CHOICE: N` indices and whether the FINAL differs from the first-line one:
- **vs140 s9 — GENUINE (indices [2,1]):** `CHOICE: 2 (Decline)` ... `So CHOICE: 1.` A real two-index
  contradiction -> retracted_choice CORRECT.
- **vs137 s26 — ENGINE-CAUSED (index [1], sustained):** disavowal prose was a CORRECT refusal of the
  unpayable Yawgmoth option (engine bug above), not a change of intent. Heuristic-block was safe.
- **vs137 s29 — OVER-FIRE (index [1], sustained):** `CHOICE: 1 (Play Polluted Delta)` held throughout;
  "Correction:"/"Wait," revised only a sub-point (Bloodghast haste), never the coded choice; PLAN
  restated option 1; heuristic coincidentally played option 1 = model intent.
**Recommendation for the parser owner:** gate retracted_choice on (a) an actual contradictory second
coded CHOICE index (s9's 2->1), OR (b) an explicit disavowal of the SPECIFIC coded action's
legality/payability (s26). Do NOT fire on generic "Correction:"/"Wait," phrases while the coded index
stands unchanged (s29). Where one in-range coded index is sustained, INDEX-WINS should trust it. LATENT
RISK: in a window where the heuristic's guess did NOT match the sustained coded intent, the s29-style
over-fire would flip a correct decision to the heuristic. (All 3 non-deciding this corpus.)

## HARNESS/SERIALIZATION — [carried class, LOW, cross-seat] nameless option labels `Put in Play` / `Decline - do nothing`.
**vs140 s9, vs27 s17, vs140 s22.** The "cast this permanent" binary serializes with the card name ONLY
in the prompt header (`Choose an option for Bloodghast:`), never in the option label. Same degenerate-
label class as wave-16's `Becomes ` (Liliana +1 target sub-menu). Combined with a stale prior-PLAN it
contributed to the s9 retraction spiral. **Fix:** emit the card name in the option label (`Put
Bloodghast in Play` / `Cast Bloodghast`). Any deck hitting a cast-permanent binary is affected.

## HARNESS — [note-only] 2 empty_reply, uniform 2502ms fast-fail, board impact NONE.
vs27 s6 (reveal, Thoughtseize hand-reveal — flow proceeded via heuristic) and vs131 s28 (Snow-Covered
Swamp land-drop — deferred at worst; deck133 WON 22/-2). Matches the brief's transient/server-side
diagnosis. No board or life impact; neither deciding. Pilot-correlation stays OPEN per the brief but
nothing at this seat implicates repetition_penalty.

## MODEL — [WATCH, no guide change] Gray Merchant strict wait-rule disobeyed on the EAGER side, outcome-positive under cap-adjudication.
GM cast at vs137 s37 (my17/opp18 drains 8) and vs131 s31 (my18/opp14 drains 4) where the guide's
wait-rule (N<opp AND my>10 -> WAIT) says hold. Both banked a life-differential swing that is directly
win-relevant under LIFE-CAP adjudication, and both games were won. The wait-rule was written for a
non-adjudicated world; it may be mildly miscalibrated for the adjudication meta. Two outcome-positive
deviations is too thin to change the guide — WATCH; re-escalate only if eager-GM ever LOSES a game a
wait would have won. No devotion-hand-count ramble flipped a deciding GM call this corpus.

## CONSTRUCTION (upstream decklist flags — RE-CONFIRMED; do NOT fix with guide length)
- **No reach / no flyer answer except Gray Merchant + Liliana -2 edict.** vs27 LOSS: opp went to the
  air with 2x Glen Elendra Liege (4/5 flyers); deck133 dealt ZERO damage all game (opp flat 20), FP
  can't hit the 4/5s, Bloodghast can't block, GM never castable (mana-light). Carried wave-9..16.
- **No early blocker; Bloodghast cannot block.** Bit again vs27 (defender with no wall). A cheap early
  blocker would blunt the go-wide/flyer losses. Carried wave-10..16.
- **Self-damage suite (Thoughtseize 2, Arena 1/turn, fetch 1 each).** Did NOT decide a loss this corpus
  (Thoughtseize discipline mostly held; vs27 lost to no-reach, not self-bleed). Carried, quieter this
  wave.

## GUIDE EDIT LEDGER (this wave — two ADDs, everything else BYTE-FROZEN from wave-11)
- **ADD** (Bloodghast section): "...the BEST card to give up to a forced sacrifice or discard (an
  edict, Cruel Edict, a forced-discard prompt) - it returns from the graveyard on your next land ...".
  Justification: new-pool forced-loss surface (finding 3); deck-specific edge the engine's generic
  "least valuable" framing cannot encode. COEXISTS with the engine framing, does not duplicate it.
- **ADD** (Liliana of the Veil section): a +1 caution paragraph (symmetric discard; use only when
  ahead-on-cards or hand-dead; when defending, prefer -2 or hold). Justification: guide gap on a
  3-copy card's mode-selection, exposed by the vs27 self-discard (finding 4).
- **KEPT (audited, not demoted):** FP revolt/legal-target block (the fetch-crack-to-enable-revolt
  sequencing advice EXCEEDS the engine's "legal targets right now" annotation — not demotion debt);
  GM "use the shown number directly / don't hand-count devotion" (the retracted spirals prove the
  hand-count temptation is still live); identity + impact-cast list + DEFENDER/Obliterator-wall + VS
  RED/BURN + Collective Brutality + fetch + Yawgmoth + Geralf's + Arena + mulligan (obeyed or untested).
- **NO forced-loss RULE added** (deliberately): the engine framing carries it and the model obeyed
  without a guide rule; a rule would be redundant (skill method-headline-1a anti-pattern).

## ROTATION VERDICT — STAY (revised this cycle; validate the two adds next corpus).
The rotation test is "no-guide-mod AND no-new-work-signal = candidate." deck133 FAILS both prongs:
(1) the guide WAS modified (two adds), and (2) there was a genuine new-work signal — the rotated-in
new pool exposed two real seams (forced self-loss + Liliana +1 while defending) the old frozen guide
never covered. deck133 should STAY one more cycle to validate the Bloodghast-fodder and Liliana-+1
adds under the modern engine, and to re-witness the retracted_choice class (which it uniquely
produces). Two engine/harness items are also open at this seat for the ledger: the Yawgmoth
unpayable-cost offer and the retracted_choice over-fire discriminator.
