# deck102 (Tergrid, discard/sacrifice) — findings (wave 23, guided round 3 / Step 0-quinquies)

Corpus: `matchups-20260724-013710`, binary `0e6361732`, 6 games round-robin (vs 27/133/140/131/137/109).
Record **3/6** (won 27, 140, 137-adj; lost 133, 109; 131 cap-adj). Cross-wave record is WEAK (pool
changed again) — judge decisions. **Fallbacks 0/205 at this seat** — the INDEX-WINS/N9 clean streak holds
(brief 102:0 confirmed). Headline: **THIS IS THE MILESTONE CORPUS — Tergrid got CAST, the flip-thrash is
essentially gone, AND the steal engine fired for the first time in project history (0/18 -> exercised).**

| vs  | result   | end     | my/opp | read |
|-----|----------|---------|--------|------|
| 27  | WIN      | t13     | 11/0   | **Tergrid cast t9, steal seam fired t11, won** — the keystone game |
| 133 | loss     | t9      | -1/16  | fast loss, no Tergrid; giant-reply tax mild |
| 140 | WIN      | t26     | 24/-4  | long grind win; Smallpox line disciplined but pitched a bomb (below) |
| 131 | cap-adj  | (no ge) | ~9/18  | Smallpox-while-behind engine-kill (disciplined, see S3) |
| 137 | WIN(adj) | (no ge) | ~16/15 | Smallpox even-board, held |
| 109 | loss     | t11     | 0/20   | raced; no Tergrid |

---

## Section 1 — R-DFC-FLIP (OWNED item b): flip-thrash ESSENTIALLY GONE + Tergrid CAST + steal FIRED.

Three separate wins on this item, all from the wave-22->23 batch (b) DISPLAY-TOGGLE annotation + castable
faces + 2-flip cap.

### 1a. Flip-thrash cut 11 -> 1. The annotation works.
Tergrid was drawn in exactly ONE game (vs27; not drawn in the other five — pure 1-of-100 variance). Across
the entire corpus the model consumed the "Flip Side" pseudo-action **once** (vs27 seq7, turn 1, Main 2) vs
**wave-22's 11**. The deployed annotation is explicit and did its job:
> `Flip Side with Tergrid, God of Fright -> DISPLAY TOGGLE only: switches this hand card to show its other
>  face "tergrid's lantern" ({3}{b}). It does NOT cast anything and uses no stack. You usually do NOT need
>  it: the Cast menu is where you cast, and it lists every face you can afford (the other face appears
>  there as an alternative-cost cast). This only changes which face is displayed.`
After one early exploratory toggle the model went to the Cast menu and cast Tergrid (seq34). The wave-22
phantom-"Activate Tergrid's Lantern" invented-name shape (the s42 stale_echo) did NOT recur — 0 fallbacks,
no ANNOUNCE_X/menu/mode decisions, no invented-name CHOICE. The annotation closed the thrash class.

### 1b. Tergrid CAST — first time ever (0/18 -> cast).
vs27 seq34 (turn 9, Main 1, L13/16, mana `{b}{b}{b}{b}` = 4 black): the model chose option 3
`Cast Tergrid's Lantern {3}{b}` (the God needs 5, it had 4 — it correctly reached for the cheaper face it
could afford). Narration:
> `Choose an option for Tergrid's Lantern: -> Cast Card Normally`
> `Your Tergrid's Lantern: hand -> stack`
> `Your Tergrid, God of Fright: stack -> battlefield`
Tergrid landed as a 4/5 menace, blocked (t10, killed Zombie Outlander), got Lightning Greaves equipped, and
attacked for 4 on the way to the win (opp -> 0, t13). **The keystone finally hit the battlefield in a real
game.** WARNING note the face mismatch: the model cast the {3}{B} **Lantern** back-face but the {3}{B}{B}
**God** front-face 4/4 creature is what resolved onto the battlefield (not the Lantern artifact). Rules-wrong
for a modal DFC, but benefit-side here (a 4/4 God for 4 mana, and it enabled the steal). Routed to notes as
R-DFC-BACKFACE-RESOLVES-FRONT.

### 1c. STEAL ENGINE EXERCISED AT LAST (0/18 -> 1 clean fire).
vs27 seq38-39, turn 11. The model cast Liliana's Triumph (edict) with Tergrid out; the opponent sacrificed
Oona's Gatewarden (their only creature). Tergrid's trigger fired and the engine rendered the steal cleanly:
> `Choose an option for Oona's Gatewarden:  1. put on battlefield   2. don't put on battlefield`
The model answered `CHOICE: 2 (don't put on battlefield)` — **it DECLINED the steal.** Seam verdict: the
steal RENDER and PARSE are clean (named the sacrificed card, two clear options, coherent reply, no
fallback) — the seam WORKS. Decision verdict: declining is defensible (Oona's Gatewarden is a 2/1 flying
DEFENDER; the model was the aggressor wanting the opponent creatureless to swing), but taking a free
permanent is almost always right (a flyer to block a counterattack + a devotion pip). This is the ONE
guide-worthy misplay the keystone finally surfaced -> strategy edit 1 adds an explicit "take the steal"
line. Mechanically the keystone is VALIDATED.

---

## Section 2 — FORCED-LOSS framing (deck140's item a) at THIS seat: render CORRECT, one efficacy MISS.

The forced-loss template fired at my seat on my OWN Smallpox self-discards (and would on opponent edicts).
Render is exact and correct — sampled vs133 s18, vs140 s14-16, vs131 s20-21, vs137 s17-18:
> `FORCED discard OF YOUR OWN CARD: ... each option is a card YOU will LOSE, not something you affect or
>  attack. Pick the card you can best AFFORD TO LOSE (usually your LEAST valuable: pitch a spare land or a
>  redundant/dead card, and KEEP your best spells...)`
**Pick quality mostly good, ONE clean bomb-pitch:**
- vs133 s18: pitched a Swamp (land) — correct.
- vs131 s20/s21: pitched Swamps — correct.
- vs137 s18: pitched Temple of the False God (land) — correct.
- vs140 s16: pitched a Swamp (land sac) — correct. s15: sacked Tinybones (only creature, forced).
- **vs140 s14 — MISS: with hand {Necrogen Mists, No Mercy, Phyrexian Obliterator}, the model discarded
  Phyrexian Obliterator ({B}{B}{B}{B} 5/5 trample BOMB) and kept two situational enchantments.** Despite
  the "KEEP your best spells" framing, it pitched its best card. This is the wave-22 deck140 class
  (Damnation+Pyroclasm) recurring — the framing RENDER did not prevent the bomb-pitch here. Cross-seat data
  point routed to notes for the deck140/synthesis efficacy verdict; I also reinforced it in this guide
  (strategy edit 3) since deck102 casts its own Smallpox and suffers its own forced discards.

---

## Section 3 — POX / SYMMETRIC-TRAP discipline, round 3: HELD, with one nuance.

Four symmetric casts this corpus (3 Smallpox + 1 Necrogen Mists). No Pox. Discipline verdict per cast:
- vs140 s13 Smallpox @ L18/15 — AHEAD (case b). Disciplined. (The follow-on bomb-pitch was the discard
  step, not the cast — Section 2.)
- vs140 s18 Necrogen Mists @ L17/14 — ahead. Disciplined.
- vs137 s16 Smallpox @ L17/16 — even board. Disciplined.
- vs131 s19 Smallpox @ **L10/19 (behind on life)** — the interesting one. The guide's absolute "NEVER
  while behind on life" would forbid it, BUT the model's reply shows real discipline: it priced its own
  cost (lose 1 life, discard, sac Tinybones 1/2, sac a land — "Ancient Tomb... deals damage to me anyway")
  and correctly identified that the opponent MUST sacrifice their Young Pyromancer (a 2/1 token ENGINE
  against a 6-card grip). Trading a lone spare 1/2 + a spare land for their engine creature + a discard is
  a defensible board-reset against a flooded spellslinger, and the model was NOT on a fast clock (opp had a
  single 2/1). This is a spot where the guide's blanket "NEVER while behind on life" is slightly too
  absolute for the CHEAP one-of-each Smallpox. Strategy edit 2 adds a narrow, gated exception and re-scopes
  the hard rule to "never while being RACED and low on life" (which is what the rule actually protects
  against). The reasoning was coherent, not a panic Hail-Mary — this is discipline, not a leak.

---

## Section 4 — GUIDE ROUND-3 VALIDATION (the wave-22 edits).

- **Painland demotion (wave-22 edit 1):** HELD. Ancient Tomb drawn in vs131 (CAUTION line + battlefield tag
  both render, sampled at s19). No self-death class; the model priced Tomb's damage in its Smallpox
  reasoning. Engine now carries it — demotion confirmed durable.
- **Symmetric-edict clause (wave-22 edit 2):** finally got real tests this corpus (4 casts) and held (see
  Section 3), with the one nuance now folded in.
- **Tergrid DFC awareness (wave-22 edit 2, the added paragraph):** the crutch it was written for is now
  ENGINE-CARRIED (the DISPLAY TOGGLE annotation), so it is DEMOTED this wave — see strategy edit 1. The
  strategic content (Tergrid = steal engine, cast from the menu) is kept; the mechanical "don't flip"
  warning is trimmed to a pointer.

---

## Section 5 — PILOT (item e, repetition_penalty=1.05): SAME-to-slightly-BETTER at deck102, no degradation.

Seat reply-length percentiles (n=205): **p50 1106 (== corpus), p95 5749 (BELOW corpus p95 7784), max 14514
(DOWN from wave-22 seat's 16752).** No spiral lengthening; if anything the tail is shorter at this seat.
Prose quality is coherent in the long replies (the vs131 s19 Smallpox cost/benefit analysis and the vs27
s39 steal reasoning are both structured and correct-in-form; no verbatim-loop degradation observed in
sampled p95+ replies — a contrast with wave-22's 16K flip-flop loops). Answer-first held (0 unparsed, 0
empty at this seat; all p95+ replies lead with a complete CHOICE line). **Per-seat pilot verdict: SAME /
slightly BETTER — spirals contained, no prose degradation, decision quality intact.**

---

## Section 6 — ANSWER-FIRST / echo cleanliness: HOLDING. 0 fallbacks, 0 unparsed, 0 empty at this seat.
The wave-22 retracted_choice pair did NOT recur; no INDEX-WINS mis-trust, no invented-name re-commit, no
empty_reply (the corpus's 10 empties were at other seats — none at deck102). N9/echo/INDEX-WINS clean
streak continues at deck102.
