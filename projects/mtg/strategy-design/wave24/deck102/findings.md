# deck102 (Tergrid, discard/sacrifice) — findings (wave 24, guided round 4)

Corpus: `matchups-20260724-125739` (PRIMARY, post-N9-hotfix), binary `d7ff8b6c4` (wave-24 step-1
batch + PREGAME MULLIGAN PHASE + N9 hotfix). 6 games round-robin (vs 27/59/109/137/131/133).
Record **2/6**: WIN vs27 (t13), WIN(adj) vs131 (23/17 @ t14), LOSS vs59 (t10), LOSS vs109 (t11),
LOSS(adj) vs137 (12/15 @ t10), **DRAW vs133 (timeout, 18/18 @ t10 — half mine)**. Cross-wave
record is WEAK (pool changed: 140 out, 59 in) — judge decisions. **Fallbacks 1/205 at this seat**
(the s5 priority unparsed_reply the brief assigned me). Pregame phase's first fleet outing validated
at this seat.

Headline: **quiet, correct corpus. Tergrid CAST AGAIN and this time RULES-CORRECT (God front face,
not the wave-23 backface bug); flip-thrash stayed at 0/6; Smallpox discipline held on a well-reasoned
turn-2 fire; the s5 unparsed is a benign out-of-range hallucination (heuristic passed correctly, no
game harm). The one real blemish is a PREGAME misplay: an incorrect mulligan driven by the model
MISREADING its own 2-Swamp hand as "zero Swamps."**

| vs  | result     | end   | my/opp | read |
|-----|------------|-------|--------|------|
| 27  | WIN        | t13   | —      | won without Tergrid (never castable); clean |
| 59  | LOSS       | t10   | —      | **over-mulliganed to a weak 5 on a false hand-read; flooded bottom** |
| 109 | LOSS       | t11   | —      | lost the race to mono-R aggro; **turn-2 Smallpox was DISCIPLINED/correct** |
| 137 | LOSS(adj)  | t10   | 12/15  | behind at cap vs GW Adventures |
| 131 | WIN(adj)   | t14   | 23/17  | ahead at cap; one 198s slow decision but game completed |
| 133 | DRAW(t/o)  | t10   | 18/18  | **Tergrid CAST (God face, correct); slow-grind timeout, dead even** |

---

## Section 1 — s5 PRIORITY unparsed_reply (brief focus 1): CLASSIFIED — benign OUT-OF-RANGE hallucination, commit-lock family, NEW sub-shape. No game harm.

Game: vs133, seq 5, kind=priority, turn 0, Main phase 1, L20/20, latency 52.8s, options=1,
choice=-1, fallback=unparsed_reply. Model qwen35. Reply complete (3556 chars, full PLAN — NOT
truncated, NOT decode-collapse).

**What the parser saw.** Line 1 was a clean coded line: `CHOICE: 2 (Cast Commander's Sphere)`.
Answer-first extracts it. But only options **0 (pass)** and **1 (Flip Side toggle)** were legal —
`CHOICE: 2` is OUT OF RANGE, so the parser correctly rejects it, unparsed, heuristic fallback (which
PASSED, the correct action). Zero game impact.

**Why the model emitted an out-of-range 2.** Two compounding seeds, both visible in the record:
1. **Stale PLAN naming an unaffordable card.** The carried `YOUR PLAN` line read "Play the Swamp,
   tap it for {B}, and cast Commander's Sphere." Commander's Sphere costs `{3}`; the model had
   exactly `{b}` (1 untapped Swamp). The plan was already impossible when it was written, and it
   was fed back in as authoritative intent.
2. **The reply-protocol EXAMPLE seeded the index.** The protocol ends with `e.g. "CHOICE: 2 (Cast
   Fatal Push)"`. The model pattern-filled the example verbatim — `CHOICE: 2 (Cast <planned card>)`
   — committing to index 2 before checking legality. There is no option 2.

**The self-correction.** In the prose that follows, the model reasons *correctly* the whole way:
counts its mana ({b}), confirms it cannot afford Commander's Sphere / Tergrid / Chupacabra,
recognizes the lone listed action is "just a UI toggle... essentially do nothing," and concludes
**"So I will choose 0 (pass)."** But it states 0 only in PROSE and never re-emits a corrected
`CHOICE:` line. So the parser has exactly one coded line — the wrong, out-of-range one.

**Classification (per skill method-3).** COMMIT-LOCK family (there IS a coded line, self-corrected
in prose), but a THIRD sub-shape distinct from the two on record: not truncation-freeze (reply
completed), not decode-collapse (coded line present). It is **example-format + stale-plan seeding an
OUT-OF-RANGE line-1 index, corrected in prose, never re-emitted as a coded line.** A "prefer the
last coded line" parser rule would NOT save it — index 2 is the only coded line present. This is the
wave-23 "1 out-of-range conflation" class, now with a precise mechanism. Isolated: a corpus-wide
scan (all 6 games) found this as the ONLY line-1-coded vs prose-corrected fallback.

**A DFC angle worth naming.** The trigger board was a priority window where the ONLY listed legal
action was the Tergrid **Flip Side DISPLAY TOGGLE** (nothing castable, land already played). The
model was visibly unsettled by a menu offering only a do-nothing toggle ("The only option listed is
1. Flip Side... is it a partial list?"), which pushed it to hallucinate a cast rather than pass.
The demotion HELD anyway — it did NOT consume the toggle (flip-thrash stayed 0). Routing + candidate
fixes in notes.md. This is a harness/representation + core-example item, NOT a guide gap.

---

## Section 2 — DFC round 2 (brief focus 2): Tergrid CAST + RULES-CORRECT; flip-thrash 0/6; NO steal window opened.

- **Tergrid drawn/cast: YES, in vs133 (the timeout game).** The model drew Tergrid, discarded it to
  graveyard, recurred it via **Mortuary Mire** (grave->library top), redrew it, and cast it. Cast
  path: `Choose an option for Tergrid, God of Fright: -> Cast Card Normally`, `hand -> stack`,
  `stack -> battlefield`. It had 5 lands (4 Swamps + Mortuary Mire) and cast the **{3}{B}{B} GOD
  FRONT FACE directly** — the rules-correct 4/5. Then equipped Lightning Greaves (haste+shroud) and
  planned to swing. **R-DFC-BACKFACE-RESOLVES-FRONT did NOT recur** — because the Lantern back-face
  was never cast this corpus (the model reached the God's full 5 mana and cast the God directly). The
  bug remains OPEN/un-re-exercised (notes.md), not resolved and not observed.
- **Flip-thrash: still gone — 0 Flip Side consumed across all 6 games** (wave-22 had 11, wave-23 had
  1). The DISPLAY-TOGGLE annotation renders in full and the demotion holds. Even the s5 confusion did
  not produce a toggle consume.
- **Steal window: 0 across the entire corpus.** No opponent sacrifice/discard of a permanent occurred
  while Tergrid was on the battlefield (Tergrid landed ~t9-10 in vs133, right at the timeout cap, with
  no time to force a steal). So **the wave-23 "take-the-steal" guide line got NO test this wave** —
  neither validated nor invalidated. The deck's core payoff line is now UNVALIDATED across two
  consecutive waves that had Tergrid on the board (wave-23: 1 decline; wave-24: 0 windows). This is
  the seat's outstanding validation debt — see the probe-deck recommendation in notes.md.

---

## Section 3 — Wave-23 guide edits round 2 (brief focus 3): painland HELD, Smallpox exception HELD (well-reasoned fire).

- **Painland demotion (held again).** Ancient Tomb appeared in vs59's kept hand and vs109. No
  self-death, no misprice. The CAUTION line + battlefield damage tag are engine-carried; demotion
  durable.
- **Smallpox gated exception + hard rule.** ONE symmetric cast this corpus: **vs109 seq4, Smallpox
  {b}{b}, turn 2, L20/20, empty board.** The reasoning was EXCELLENT and disciplined: opponent had
  only Rakdos Cackler (1/1) + one Mountain (their ONLY land); the model correctly saw that Smallpox
  forces them to sac their only creature AND their only mana source while 102 (no creatures) loses
  just one of two Swamps and a card — "a massive tempo swing that aligns with our attrition plan."
  This is the guide's case-(c) spirit (own board empty -> lose little) fused with a de-facto
  asymmetric outcome (opp stripped to zero lands). NOT a leak; the symmetric framing held. (102 still
  lost vs109 — mono-R aggro, 5/6 this corpus — but the loss was the race, not the Smallpox, which was
  correct.) No forced-self-discard bomb-pitch surfaced this corpus (no Smallpox discard steps hit a
  hand containing a bomb).

---

## Section 4 — Pregame-phase validation (brief focus 4): phase MECHANICALLY SOUND; ONE mulligan misplay (hand-misread).

Mechanics — all correct:
- **Bottom-N math correct** (London rule N = mulligans taken): vs59 mulled twice -> bottomed 2; vs133
  mulled once -> bottomed 1; the four keep-on-7 games bottomed 0 (no bottom record). No double-asks,
  no wrong-N, no leyline cards in any opening hand (deck102 runs none -> 103.6 window N/A). Game-start
  integrity: turns proceeded normally with sensible life totals; no anomalies.
- The "opening hand narration reflects the pre-mulligan deal" cosmetic did NOT cause harm here — the
  `CURRENT SITUATION` hand line at each mulligan ask showed the CORRECT current 7 (verified at vs59
  seq2 and vs133 seq5).

Decision quality — mostly fine, ONE clear misplay:
- **vs27 / vs109 / vs137 / vs131: KEEP on 7** — all reasonable one-decision keeps.
- **vs133: mull->keep, bottom Sheoldred.** Kept {Chupacabra, Commander's Sphere, 2 Swamp, Tergrid,
  ...}, bottomed the {5}{B}{B} 7-drop Sheoldred. DEFENSIBLE — keep the cheaper Tergrid + a ramp rock,
  ship the redundant top-end bomb.
- **vs59: MISPLAY — over-mulliganed to a weak 5, partly on a FALSE hand-read.**
  - Hand 1 (mull): 3 tapped/utility lands + Sol Ring + Night's Whisper + 1-drop + equipment; no early
    disruption, thin black. Mulligan DEFENSIBLE (awkward mana, no early black play).
  - **Hand 2 (mull — WRONG): the model's reply claimed "zero basic Swamps," but the prompt's hand line
    plainly listed TWO Swamps** (Worn Powerstone; Witch's Cottage; Archon; **Swamp**; Necrogen Mists;
    **Swamp**; Reliquary Tower). That is a KEEPABLE hand — 4 lands incl. 2 Swamps, a ramp rock, a bomb
    (Archon), and disruption (Necrogen Mists). The model shipped it on a factual MISCOUNT of its own
    listed hand, anchored to a stale PLAN ("Mulligan to find a hand with at least one basic Swamp")
    that the hand ALREADY satisfied. Representation was correct (hand rendered clearly) -> this is a
    MODEL attention/miscount error, not a display gap.
  - Bottom (keep 5): PUT Reliquary Tower + Sangromancer. Bottoming Reliquary Tower (redundant
    colorless land) = correct; bottoming Sangromancer (the only midrange body) to keep a 4-land +
    lone-1/1 hand = FLOOD-prone and mediocre — keeping 3 lands + Doomsayer + Sangromancer would have
    been the better 5.
  - Net: two mulligans (one on a misread) into a weak flooded 5, and 102 lost at t10. The clearest
    decision-quality blemish of the corpus, and it lives in the NEW pregame surface. Routed to
    notes.md as a pregame model-attention finding (analogous to method-2's claimed-vs-actual
    board-state diff, now applied to the mulligan HAND).

---

## Section 5 — 2/6 decisions read + the 102vs133 timeout (brief focus 5, coordinate finding).

**Record decomposition (decisions, not win column):**
- WIN vs27 (t13), WIN(adj) vs131 (ahead 23/17 at cap): clean; no seat leaks.
- LOSS vs109 (t11): lost the race to mono-R aggro — a matchup/tempo loss, play was fine (the turn-2
  Smallpox was correct). LOSS vs137(adj) (behind 12/15): GW Adventures, close at cap.
- LOSS vs59 (t10): the pregame misplay above is the identifiable contributor (weak flooded 5).
- The seat's 2/6 is a hard-pool artifact (109 5/6, 133 4/6 are strong opponents) more than a play
  collapse; the only clear in-game decision leak is the vs59 mulligan.

**102vs133 timeout (the half-mine coordinate finding) — GENUINE SLOW GRIND, not an engine/phase
stall.** Decision counts + latency at my seat: 34 latency-bearing decisions, sum 706s, median 16.4s,
max 107.8s, **18 priority + 14 ask + 1 bottom + 1 attackers**. Symmetric opponent ~700s -> ~1400s
pure inference + pregame + stale re-asks + overhead => 2400s cap reached at only **turn 10, dead even
18/18**. Diagnosis:
- **Board stall in a mono-B attrition (102) vs mono-B devotion (133) matchup** = few creatures, lots
  of removal, many pass-priority windows. The tell is the **18 priority decisions** at my seat (vs 6
  in the aggressive vs27 win) — each a full ~16s model round-trip spent mostly to PASS.
- No single decision consumed the cap (max 107.8s); decisions flowed at normal latency. The pregame
  phase added only ~2 extra decisions (one mulligan) — NOT unusual time.
- **Ahead-ness at cap: neither** (18/18, barely damaged) — a true drawish state, correctly adjudicated
  as a draw.
- Verdict: latency-bound slow grind, driven by high priority-window counts on a stalled board — the
  same latency economics the brief flags (median 17.3s / p90 73s). Remedy is throughput (harness -j /
  WAGIC_GPT_TIMEOUT), NOT a play or engine fix. Companion note: the 27vs137 timeout is very likely the
  same class (both ~t10, even life). Cross-referenced in notes.md; coordinate with deck27/deck137.

REPPENALTY=1.05 second corpus: no degradation at this seat — no spiral lengthening, coherent long
replies, 1 benign fallback. Consistent with wave-23's SAME/slightly-better verdict.
