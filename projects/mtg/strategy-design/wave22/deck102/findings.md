# deck102 (Tergrid, discard/sacrifice) — findings (wave 22, guided round 2 / Step 0-quater)

Corpus: `matchups-20260723-173843`, binary `1d8ed8194`, 6 games round-robin (vs 62/135/44/140/109/27).
Record **2/6** (wave-21 was 3/6 — POOL SWAPPED, so cross-wave record is weak evidence per the brief).
Fallbacks **3/1368 at this seat** (2 retracted_choice, 1 stale_echo). Headline results:

1. **HARNESS-N9 residual — BOTH retracted_choice fires are CORRECT FIRES, zero residual FPs.** The N9
   reference-scoping cut this seat's retraction fires from wave-21's class down to 2, and both remaining
   fires are genuine disavowals where the model wrote a first-line CHOICE and then coherently, repeatedly
   picked a DIFFERENT numbered option and never re-committed. The N9 fix is validated clean at this seat.
2. **R-PAINLAND (item e) — CLOSED / VALIDATED.** CAUTION line AND the per-land battlefield tag both render;
   the wave-21 Ancient-Tomb self-death class did NOT recur. Guide crutch DEMOTED (see strategy edit 1).
3. **NEW engine/representation defect R-DFC-FLIP** — Tergrid is the DFC "God of Fright // Tergrid's Lantern";
   the "Flip Side" face-toggle is offered as a repeatable *priority pseudo-action*, the model thrashed it 11x,
   misread the uncast Lantern as an on-battlefield activatable permanent (that IS the s42 stale_echo), and
   Tergrid was never cast. Routed to notes.md.

Record decomposed:

| vs  | result   | end    | my/opp | read |
|-----|----------|--------|--------|------|
| 140 | WIN      | t12    | 25/0   | killed them — deploy/attack floor working, opp to 0 |
| 27  | WIN(adj) | cap t11| 15/8   | ahead at cap, steady clock |
| 44  | loss(adj)| cap t15| 2/17   | behind at cap; 4 Ancient Tomb taps, NO self-death (survived) |
| 62  | loss     | t12    | -1/10  | fast race; desperation Pox/Smallpox at <=2 life; the s21 retracted_choice |
| 135 | loss     | t12    | -5/16  | raced by Ohran Viper/Icehide Golem; Ancient Tomb 1 of many sources, passed at 4 life (no self-kill) |
| 109 | loss     | t11    | 0/20   | MANA-SCREWED at 3 mana all game; Tergrid DFC stuck in hand; opp untouched at 20 |

---

## Section 1 — HARNESS-N9 RESIDUAL (owned item b): both fires CORRECT, no residual false positive.

Brief item (b) scoped the retraction detector so a phrase must precede a line-leading PLAN: AND reference
the chosen option; wave-21's 4 FPs are suppressed. Both of this corpus's retracted_choice fires are at my
seat. Verdict on each:

### s21 vs62 (kind ask, turn 11, L1/10) — CORRECT FIRE (genuine disavowal).
First line: `CHOICE: 4 (Smallpox)` @pos 0. The reply then spirals for 16,752 chars over a `TARGET CHOICE
for discard cards` prompt it cannot parse, and explicitly, repeatedly abandons option 4:
- @5388 "I will choose the opponent as the target" | @6205 "I will choose the first option, which is Thought
  Vessel" | @6752 "I will choose option 1, Thought Vessel" — then flip-flops "option 4, Smallpox" <-> "option 1,
  Thought Vessel" <-> "the opponent" **dozens** of times, truncating mid-sentence with NO PLAN: line.
The model unambiguously disavowed its committed choice (CHOICE: 4) and never coherently re-committed to
anything. Heuristic fallback is appropriate (game already lost at L1). **Not a false positive** — this is
exactly what the detector should catch. The 16K spiral is driven by a separate representation defect (the
Pox self-discard "TARGET CHOICE" prompt — see notes R-POX-DISCARD-TARGET).

### s29 vs140 (kind ask, turn 12, L22/11) — CORRECT FIRE (genuine disavowal).
First line: `CHOICE: 1 (Cast Arcane Signet)` @pos 0. The 12,247-char reply then argues, repeatedly and
coherently, for a DIFFERENT option — Mox Jet (option 2):
- @8559 "I should cast Mox Jet" | @9494 "I will cast Mox Jet" | @10499 "So I will cast Mox Jet" — interleaved
  with "But wait, maybe I should cast Arcane Signet," never re-committing to Signet, then collapses into an
  identical mana-arithmetic loop ("...Total 2. No, that's wrong. Wait, if I have 3 in the pool..." repeated
  verbatim 6x @11161-12162) and truncates. NO PLAN: line.
The model disavowed the chosen option (Signet) in favour of Mox Jet and never resolved. Correct fire; the
true intent is genuinely unrecoverable from the reply. **Not a false positive.**

**Conclusion for the master ledger:** the N9 fix reduced this seat from a fallback-generating retraction
class to 2 fires, and BOTH are legitimate model self-disavowals — no residual false positive at deck102.
N9 can be considered validated here. (Note the common root: both are the behind/confused *giant-loop
truncation tax* — answer-first still puts a CHOICE: line first, but when the body then coherently commits
to a different option, retraction correctly fires. This is model behaviour, not a detector bug.)

---

## Section 2 — R-PAINLAND (owned item e): CLOSED / VALIDATED. Self-death class stopped.

Ancient Tomb was drawn in 2 games (vs135, vs44). Both surfaces render:
- **CAUTION line** (mana line): `CAUTION - some usable mana sources DAMAGE YOU when tapped for mana (auto-tap
  when you cast may spend them - count this life loss first): Ancient Tomb: 2 damage`  [renders]
- **Battlefield tag**: `Ancient Tomb [tapping for mana deals 2 damage to its controller]`  [renders]

Self-death class (wave-21 vs27: cast a non-essential spell via Ancient Tomb at 3 life -> self-killed) did
**NOT recur**:
- vs135: died in a race (Ohran Viper/Icehide Golem), Ancient Tomb was 1 of many damage sources; at 4 life on
  the opponent's turn the model correctly chose `CHOICE: 0 (pass)` for three consecutive priority windows
  rather than self-tapping. No gratuitous self-kill.
- vs44: 4 Ancient Tomb taps across a 15-turn game and the model SURVIVED to adjudication (my=2) — it priced
  the taps and did not immolate.
Distinct self-taps this corpus: vs135 = 3, vs44 = 4 (not the 205 the naive grep shows — that count is
inflated by the cumulative game-log repeating every event in each later prompt).

**Crutch-lifecycle ruling (per skill Method-headline-3):** the durable fix shipped, is confirmed FIRED
(both surfaces render) and OBEYED (self-death class stopped). Rule is still TRUE (painlands still damage),
so this is a **DEMOTE**, not a retire. Executed in strategy edit 1. Critically, the wave-21 crutch contained
a clause that the fix has made **factually FALSE** — "the 'Mana available' line does NOT warn you of that
cost" — the CAUTION line now DOES warn. A stale crutch is an attention tax; a *false* crutch actively
contradicts what the model sees, so it had to be fixed regardless. (Generalized to skill.md.)

---

## Section 3 — s42 vs109 stale_echo (owned item, priority): NEW shape, NOT covered by the (c) fixes.

`kind priority`, turn 11, L7/20, single option: `Flip Side with Tergrid's Lantern`. The model wrote
`CHOICE: 0 (pass)` (answer-first), reasoned itself into believing it could **activate** Tergrid's Lantern —
then emitted a SECOND block: `PLAN: Activate Tergrid's Lantern...` + `CHOICE: 1 (Activate Tergrid's
Lantern)`. The two contradictory CHOICE lines (pass -> activate) tripped the fallback.

Classified against brief item (c): this is **neither** the source-prefix shape (`<spell> targeting <target>`)
**nor** the combat-math label-hijack (an `Attack:` CoT line stealing a CHOICE). It is a NEW shape: the model
**invents an option name** ("Activate Tergrid's Lantern") that does not match the offered option ("Flip Side
with Tergrid's Lantern"), and re-commits to that phantom option. The root is the DFC-flip representation
defect (R-DFC-FLIP) — the model mistook an uncast, in-hand DFC back-face for an on-battlefield activatable
permanent. The (c) echo fixes do not close this shape. Routed to deck140 (stale_echo owner) + notes.

---

## Section 4 — GUIDE ROUND-2 VALIDATION (the wave-21 edits).

### Edit 1 (retire the stale Thoughtseize crutch): VALIDATED — pick-ONE stayed clean without it.
2 reveal records this corpus, both single clean picks with the crutch gone:
- vs135 s24 (t7): `PUT: 7 (Force of Negation)` — one pick, took the best counter off the top of a revealed hand.
- vs44 s10 (t2): `PUT: 2 (Counterspell)` — one pick from a hand of two Counterspells + Arcane Denial.
0/2 over-picks without the crutch. Retiring it did not regress pick-ONE. Confirmed closed.

### Edit 2 (sharpen the symmetric-edict clause): UNDER-EXERCISED — no clean test.
The only symmetric casts were vs62 s19 `Cast Pox` at L2 and s21 `Cast Smallpox` at L1 — both desperation
Hail-Marys in an already-lost race (died turn 12). At s19 the model reasoned "if I do nothing I die to their
attack" and cast Pox hoping to survive; it did NOT heed the sharpened "won't stop the evasive clock" nuance
(the opponent would sac a Saproling and keep the 4/5 flying Birds that was killing me) — but the spot was
unwinnable regardless, so this is not evidence the edit failed. No ahead-on-board symmetric decision occurred.
KEEP; re-validate when a genuine grind-the-even-board spot appears.

### Edit 3 (painland caution): see Section 2 — DEMOTED (engine now carries it).

---

## Section 5 — TERGRID (owned item 5): DREW again, still NOT exercised — mana screw + DFC.
Tergrid was drawn in vs109 on **turn 4** (L16) — early, for once. It was never cast through turn 11 and ended
the game still in hand. Cause is TWO-fold:
1. **Mana screw**: the deck was stuck at **3 mana the entire game** (3 Swamps; never a productive 4th land).
   The God costs {3}{B}{B}=5 and the Lantern back-face {3}{B}=4 — **neither face was ever affordable.** Every
   main-phase ask offered cheaper spells (Soul Shatter, Liliana's Triumph, Elvish Doomsayer, Night's Whisper);
   Tergrid never appeared as a castable ask option because the model could not pay for it.
2. **DFC representation (R-DFC-FLIP)**: the ONLY Tergrid interaction ever offered was the repeatable priority
   pseudo-action "Flip Side with Tergrid, God of Fright" / "...Tergrid's Lantern". The model selected it 11x
   (each a no-op face toggle), and at s42 misread the uncast Lantern as an activatable battlefield permanent.

**Steal engine remains unexercised: 0/18 games across three corpora.** The variance (1-of in a 100-card
singleton) is now compounded by needing 4-5 mana AND surviving the DFC-flip confusion. A Tergrid+enablers
probe deck remains the only reliable validation path (notes, routed to engine-seam owner). Guide gap closed
(the guide had pretended Tergrid was mono-faced) — see strategy edit 2.

---

## Section 6 — ANSWER-FIRST (every-reviewer validation): HOLDING.
All 6 p95+ replies lead with a complete coded line: `CHOICE: 4 (Smallpox)` (16,752 ch), `CHOICE: 0 (pass)`
(14,715), `CHOICE: 5 (Hymn to Tourach)` (14,154), `CHOICE: 2 (Cast nothing right now)` (14,107), etc. The
giant-reply tax persists (16K-char loops at behind/confused spots) but the answer comes first. The only
conversions to fallback are the 2 retracted_choice fires — where the model committed first, then the body
coherently disavowed for a different option (Section 1) — which answer-first cannot prevent by design.
