# Wave 24 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: PRIMARY `matchups-20260724-125739` (21 games, binary `/tmp/wagic-d7ff8b6c4` =
step-1 batch `5f78e2934` + pregame phase `86df3b482` + N9 hotfix `d7ff8b6c4`). deck133 was
the GPT seat in 6 games. Pilot: repetition_penalty=1.05 (second corpus). deck133 STAYED per
the wave-23 rotation verdict to validate the two guide adds and re-witness the retracted_choice
class (which it uniquely produces).

**Real record 4W / 1L / 1D** (reconciled from `WAGIC_SELFPLAY_RESULT` lines + life-cap adjudication):

| opp | deck | result | end my/opp | mode | note |
|-----|------|--------|-----------|------|------|
| 131 | Mind Control (Rx tempo) | **WON** | 7 / -4 | clean kill (t12) | GM never needed; Bloodghast+Geralf's beats + FP on Guttersnipe/Young Pyromancer |
| 109 | Mono-R Aggro | **WON** | 11 / 0 | clean kill (t13) | GM stabilize at 3 life (drain 8 -> 11), Yawgmoth grind, FP on Gore-House/Stromkirk |
| 137 | GW Adventures | **WON (adj)** | 14 / 4 | cap-adj, +10 lead | Liliana -2 edicts, GM stabilize at 6, FP killed Faerie Guidemother front face |
| 59  | WB Persistence | **WON (adj)** | 5 / 2 | cap-adj, +3 lead | tight race; FP on Safehold Elite, held Liliana +1 at 5 life (correct) |
| 27  | Zombie/Undead | **LOST** | 0 / 20 | 0 dmg dealt (t12) | **structural no-reach again** — opp flat 20 ALL game; land-light greedy keep |
| 102 | Tergrid control | **DRAW** | ~18 / 18 | timeout at 2400s cap (t11) | slow grind, LLM-latency-bound; even life -> fair draw (decompose in notes.md) |

TRUE model-fallbacks at this seat: **1** (vs131 s21 retracted_choice — the corpus's ONLY
retracted_choice; verdict below). All owned play-tasks pass clean. Guide **BYTE-FROZEN**
(sha256 identical to wave-23/deployed) — no new leak; the two wave-23 adds validated/insurance.

---

## FINDING 1 — [YAWGMOTH ACCEPTANCE TEST — the SacrificeCost candidate-existence fix] — PASS. ZERO unpayable offers; the fix is SURGICAL. Route: ENGINE item CLOSED (findings.md).

This is deck133's flagship owned acceptance test. Wave-23's engine bug (vs137 s26: the
`-1/-1 Counter [cost: Life, Sacrifice]` offered with only Yawgmoth controlled = "Sacrifice
**another** creature" unpayable, which produced an 85s spiral + a fallback) is **GONE**.

Mechanical proof across all 6 deck133-seat logs:
- **37 sacrifice-cost `-1/-1 Counter with Yawgmoth [cost: Life, Sacrifice]` offers** rendered
  this corpus. Controlled-creature counts at those offers: **{2, 3, 4} — never 1.**
  **Unpayable offers (creatures<2): ZERO.** The SacrificeCost::canPay candidate-existence gate
  holds at every window.
- **The fix is SURGICAL, not a blanket Yawgmoth suppression.** Yawgmoth's OTHER activated
  ability — `proliferate [cost: {b}{b}, Discard]` (Oracle: "{B}{B}, Discard a card:
  Proliferate") — does NOT require sacrificing a creature, and it was **correctly still
  offered at creatures:1** (vs27 s21, s25). The gate keys on the SacrificeCost specifically,
  leaving the discard-cost proliferate mode legal. This is exactly the behavior the fix should
  produce and rules out a coarse "don't offer Yawgmoth with 1 creature" hack.

Legitimate windows where Yawgmoth's -1/-1 WAS offered and used (2+ creatures, cost payable):
- **vs27 s20 (creatures:2, first Yawgmoth online):** offered 3 targets, model chose it (killed
  a zombie by sacrificing Bloodghast — recurs on next land, the guide's fodder logic).
- **vs27 s23 (creatures:2):** chosen again.
- **vs109 s19 (creatures:4, full board):** 5 targets offered; s20 (creatures:3) and s21
  (creatures:2, exactly the minimum) both offered and chosen — a sacrifice-candidate present
  each time, the counter+draw engine ran cleanly through the aggro race.
- **vs102 s37/s39 (creatures:2):** offered targeting own Bloodghast; declined (grind, held).

**Verdict: PASS. The Yawgmoth unpayable-offer class is eliminated. The engine item from
wave-23 notes.md is CLOSED.** No guide/harness action; recorded here as the validation.

---

## FINDING 2 — [N9-HOTFIX ACCEPTANCE TEST — s21 retracted_choice] — RESIDUAL FALSE POSITIVE. The hotfix's second-coded-index detector is tripped by the model QUOTING THE PROMPT'S OWN EXAMPLE. Route: HARNESS/PARSER (notes.md).

**vs131 s21 (ask, my17/opp16, turn 7, latency 225,068ms).** deck133 owns the corpus's ONLY
retracted_choice, and it is the N9 hotfix's live acceptance test. Per the brief the hotfix
retired branch (b) so retraction now = "second-contradictory-coded-index ONLY." **Verdict:
this is a RESIDUAL FALSE POSITIVE**, with exact evidence.

The decision: opponent cast Downsize (-4/-0) at Geralf's Messenger; deck133's options were
`1. Cast Fatal Push` / `2. Cast nothing`. The reply's **first line is a clean coded answer:**
`CHOICE: 1 (Cast Fatal Push)`, and the model **re-affirms CHOICE 1 repeatedly** ("So I must
say CHOICE: 1", "I will stick to the format CHOICE: 1 (Cast Fatal Push)"). The model NEVER
changed its DECISION — it agonized for 225s over the OUTPUT FORMAT (whether to answer with the
CHOICE number or the target's name) and which target to pick.

Why the fallback fired (stderr `game-133v131` line 638):
`AIPlayerGPT: ask CHOICE 1 retracted with no replacement; deferring`.
The reply contains ~30 `CHOICE:` occurrences; **every one after the first line is inside
quotation marks as prose** — the model quoting the format string it is debating. The ONLY
contradicting *numeric* coded index is the prompt's OWN worked example, quoted verbatim four
times: `"CHOICE: 2 (Cast Fatal Push)"` (the prompt literally ends `...e.g. "CHOICE: 2 (Cast
Fatal Push)"`). The hotfix's coded-index scan matched that quoted `CHOICE: 2` as a
contradictory second index vs the first-line `CHOICE: 1`, then — because the reply truncated
mid-spiral (15,383 chars, no clean closing CHOICE line, hit the token cap) — found "no
replacement" and deferred.

- **Aggravating detail:** the prompt's example uses the SAME card name as option 1 ("Cast
  Fatal Push"), so the model quoted it obsessively while reconciling format — maximizing the
  chance the scan latches onto it. `"CHOICE: Guttersnipe"` (also quoted repeatedly) is
  non-numeric and would not parse as an index; the numeric trigger is exclusively the quoted
  example.
- **Non-deciding:** the heuristic cast Fatal Push (shouldPlay=90) and the model was then
  handed the SEPARATE target sub-prompt, where it picked **Guttersnipe** (chose 3 of 3) — its
  exact intent. deck133 won 7/-4. The model's actual play executed anyway.

**Discriminator refinement for the parser owner (notes.md):** the second-coded-index scan must
count only **line-anchored** coded indices (a real answer is emitted at line start), and must
**exclude `CHOICE:` tokens inside quotation marks or that are verbatim echoes of the prompt's
`e.g.` example string.** A first-line coded index that is re-affirmed and never contradicted by
another *line-start* index should win under INDEX-WINS. The deeper root — the cast-with-target
format ambiguity that caused the 225s spiral — is a harness/prompt item, also in notes.md.

---

## FINDING 3 — [WAVE-23 ADD ROUND 2: Liliana of the Veil +1 caution] — STRONGLY VALIDATED across 10 Liliana decisions. ZERO anti-pattern. Route: guide KEPT (validated).

The wave-23 finding-4 add: "+1 is symmetric — use it only when the opponent holds MORE cards
than you or your own hand is dead; when defending/behind, prefer -2 or hold." This corpus
exposed **10 Liliana activation windows**; deck133 **never once** repeated the wave-23
anti-pattern (symmetric +1 self-discard while defending, which in wave-23 emptied its own
answers vs27). Every case:

- **vs137 s30 (my14/opp12, hand=[Gray Merchant, Inquisition], oppHand=0):** offered +1 OR -2 —
  chose **-2 (edict)**. Correct: opp has 0 cards (nothing for +1 to strip) and my hand is live;
  +1 would pitch my GM/removal for nothing. Exactly the finding-4 scenario, played right.
- **vs102 s24 (my18/opp20, hand=[Liliana the Last Hope], oppHand=2):** +1 OR -2 — chose **-2**,
  refusing to discard its own planeswalker.
- **vs102 s30 (hand empty, oppHand=1):** chose Liliana **the Last Hope** +1 (-2/-1 on a
  creature = pure value), NOT Veil's symmetric +1. Correctly distinguished the two walkers.
- **vs102 s33 (hand=[Thoughtseize], oppHand=0):** chose Last Hope -2 (return a creature), not
  Veil +1 which would pitch Thoughtseize for nothing.
- **vs102 s37 (hand=[Thoughtseize], oppHand=0):** **held/passed** rather than +1 into an empty
  opp hand (pure downside). Correct.
- **vs59 s28 (my12/opp12, hand empty, oppHand=1):** +1 OR -2 — chose **-2** (remove a board
  threat in a race), defensible.
- **vs59 s32 (my5/opp4) and s34 (my5/opp2), only +1 offered, both hands empty:** **held both
  times.** Critically, at 5 life in a tight race the caution says "when defending/behind prefer
  -2 or hold" — it held. Textbook.
- **vs137 s38 (my14/opp4, ahead, hand=[Inquisition, land], oppHand=1):** the ONE +1 taken —
  while AHEAD and closing, with a spare land to pitch, stripping opp's last card. The caution
  permits +1 when ahead. Acceptable.
- **vs102 s28 (hand EMPTY, oppHand=1):** passed. Minor: with an empty hand the caution's own
  "hand is dead" clause would have ENDORSED Veil's +1 (free strip of opp's last card). The
  model was slightly *too conservative* here — but the error direction is the SAFE one
  (under-using +1), not the wave-23 anti-pattern (over-using it while behind). Non-deciding
  (this is the timeout draw).

**Verdict:** the +1 caution is working (behavior is now clean; the modern engine also helps by
serializing +1 vs -2 as clearly-labeled distinct options with card text). KEEP the add.

---

## FINDING 4 — [WAVE-23 ADD ROUND 2: Bloodghast forced-sacrifice fodder] — UNEXERCISED this corpus. Route: guide KEPT (cheap correct insurance).

No forced-sacrifice / edict / Cruel-Edict window rendered at deck133's seat this corpus. The
wave-23 forced-loss surface was carried mostly by **deck140 (RBW Control, Cruel Edict), which
ROTATED OUT last cycle**; the current opponents (131/59/102/27/109/137) presented no edict-style
forced sacrifice or forced discard at this seat. The Bloodghast-fodder clause therefore could
not be re-validated round 2. It remains correct, deck-specific, and cheap — **KEEP**; it will
validate only in a corpus with an edict-carrying opponent.

(Note: Yawgmoth's OWN sacrifice IS a forced-affordability choice and the model picked Bloodghast
to sacrifice at vs27 s20 — recurs on next land — the same fodder logic operating on a non-edict
surface. Weak corroboration.)

---

## FINDING 5 — [PREGAME MULLIGAN PHASE — first outing] — PASS at this seat, one cosmetic. Route: notes.md.

Every game opened with exactly ONE Keep/Mulligan flow, no double-asks, correct London structure:
- **5/6 kept the turn-0 seven** (vs131/137/109/102/59), all reasonable keeps (3 lands + a
  creature + interaction each).
- **vs27 correctly ran the full London path:** s1 Mulligan -> new 7 -> s2 Keep -> s3 Bottom-1.
  **Bottom-N=1 (one mulligan) correct.** It bottomed the **redundant, hard-to-cast duplicate
  Phyrexian Obliterator** (kept Yawgmoth, one Obliterator, both fetches, Inquisition, Fatal
  Push) — a sensible "pitch the least-castable duplicate" bottom.
- **vs27 mulligan reasoning had minor miscounts** ("5 lands and 4 spells" for a hand that is 3
  lands + 4 spells; "no creatures to cast on turn 1 or 2" when Bloodghast is a 2-drop). The
  *decision* (mulligan a clunky triple-discard, one-threat hand) is defensible, but the counting
  errors are noted. It then kept a **land-light 2-fetch 6-card hand** — a greedy keep that may
  have compounded the vs27 loss (finding 6).
- **Game-start integrity:** starting player skips the turn-1 draw (verified vs131: "Turn 1 -
  YOUR turn" with a 7-card opening hand, no draw). CR-conformant.
- **Leyline (103.6) window:** untestable at this seat — deck133 runs no leyline card and none
  appeared in an opening hand.
- **Known cosmetic (brief item 5):** at vs27 s2 the model spent paragraphs re-deriving its
  post-mulligan hand from the game log ("Let's re-read the log carefully... Initial hand...
  Mulligan... New cards drawn..."), consistent with the brief's note that the "opening hand"
  narration can reflect the pre-mulligan deal. It reached the correct hand; cost only reasoning
  tokens. Cosmetic, note-only.

---

## FINDING 6 — [vs27 LOSS] — structural no-reach AGAIN (opp flat 20 the entire game, 0 damage dealt). No deciding decision error. Route: CONSTRUCTION (notes.md, re-confirmed).

Same face as wave-23's vs27 loss: **opponent life stayed at 20 across the ENTIRE game** —
deck133 dealt literally zero damage — while deck133 fell 20 -> 6 -> 0 (turn 12). Mono-black had
no reach into the Zombie/Undead board; no castable Gray Merchant assembled, no wall held. The
land-light greedy mulligan keep (finding 5) plausibly compounded it (top-heavy 4-drops on 2
lands), but the deeper cause is the carried construction flag (no reach / no early blocker /
Bloodghast can't block). No hard deciding decision error; deck133's known structural matchup.

---

## MANDATORY AUDITS

- **Yawgmoth SacrificeCost acceptance — PASS** (finding 1): 37 sac-cost offers, all creatures>=2,
  0 unpayable; proliferate mode correctly still offered at creatures:1 (surgical).
- **retracted_choice acceptance — RESIDUAL FP** (finding 2): 1 fire, quoted-example trigger,
  non-deciding, discriminator refinement routed to parser.
- **Self-Fatal-Push watch — CLEAN.** 7 self-only-FP annotation windows (vs131 s12/14/15, vs102
  s16/17/18 declined "Cast nothing"; vs109 s28 cast Gray Merchant instead). 8 real FP casts, ALL
  hit **opponent** creatures (Young Pyromancer, Guttersnipe, Faerie Guidemother front face,
  Gore-House Chainwalker, Stromkirk Noble, Inkfathom Infiltrator, Ravenous Chupacabra, Safehold
  Elite). ZERO self-targets, ZERO fizzles.
- **Gray Merchant — 2 casts, both guide-perfect stabilize-mode.** vs137 s37 (my6, life<=10) and
  vs109 s28 (my3, drain 8 -> 11, life<=10). No wait-rule/eager-side issue this corpus.
- **Liliana +1 caution — VALIDATED** (finding 3): 10 windows, 0 anti-pattern.
- **Fallbacks: 1** (vs131 s21 retracted_choice, non-deciding). No empty_reply, no unparsed_reply
  at this seat this corpus.
- **DFC/adventure face:** FP killed Faerie Guidemother // Gift of the Fae front face cleanly
  (vs137). deck133 casts no DFCs itself.

## PILOT VERDICT (repetition_penalty=1.05, 2nd corpus): SAME — NO degradation.
206 non-empty replies: p50 **1129** (wave-23 1103 — identical), p90 4964, p95 **6206** (wave-23
8276 — LEANER tail this corpus), max 15383 (the s21 format spiral). **ZERO degenerate n-gram
repetition** (no 8-gram repeated >=6x anywhere). The long replies are content-rich
(legality/format reconciliation, mana/devotion math), not repetition-driven. No prose
degradation; consistent with the wave-23 finding.

## FREEZE / EDIT CHECK
Guide **BYTE-FROZEN** — sha256 `6ed73f00...39ce` identical across deployed / wave-23 / wave-24.
No new leak this corpus: the two wave-23 adds are validated (Liliana +1) / unexercised-but-correct
(Bloodghast fodder); Yawgmoth acceptance PASSED (engine item closed); self-FP, GM, mulligan all
clean; the only fallback is a harness-parser FP, not a deck/guide issue. strategy.txt byte-copied
and cmp-verified. See notes.md for the full routed ledger + rotation verdict.
