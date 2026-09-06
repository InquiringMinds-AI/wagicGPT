# Wave-63 synthesis — proposals adjudicated

Inputs: `wave63/deck123/general-proposals.md` (the only proposals file this wave; no seat filed
skill proposals), the seven `wave63/deck*/review.md`, `wave63/engine-seat.md`.
Corpus: `matchups-20260905-191148` (19 natural + 2 hung) and the two reruns
`matchups-20260905-214946` / `-215345`. Counts below re-derived from the rendered `prompt`.

**Result: 0 of 1 adopted.** No `wave63/general-strategy.md` is written; wave-62's R331 edition
remains the current general file and the amendment numbering stays at R331. No
`wave63/strategy-writing-skill.md` is written; the wave-59 skill edition (amendments 1-331)
stands unchanged.

---

## G1 (deck123) — "a lethal blockers header whose AT-BEST figure is above 0 is a BLOCK, never a decline"

**REJECT.**

One line of reasons: the file it asks to be written into is not seat-facing, the surface it asks
to compensate for is TRUE rather than false, a second seat read the identical clause correctly in
the same corpus, and the standing wave-62 ruling forbids exactly this kind of guide hedge against
a printed figure — the lever is the render fix the proposal itself prefers.

Expanded, with the checks that produced it:

1. **Wrong file for a teach.** `general-strategy.md` is not loaded by the engine at runtime —
   `/usr/bin/grep -rn "general-strategy" src/ include/ bin/Res/ai/` returns exactly one hit, a
   code comment at `src/AIPlayerGPT.cpp:28125`. Every R-item in the wave-62 edition is an ask on
   the render or the core prompt. A seat-facing teach adopted there would reach no model. The
   seat-facing surfaces are the deck guide and the core prompt; deck123 has already put this teach
   in its own deck guide (its Edit 2, `wave63/deck123/strategy.txt`), where it is legitimately
   deck-scoped and costs the general layer nothing.

2. **The surface is true, and the corpus shows it is readable.** Five blockers windows in the
   corpus carry `this KILLS you` (of 31 blockers windows). Three of those also print a
   `you would be at N AT BEST` figure; two have N above 0:
   - `1788653513-ai_baka_deck123-…-vs-ai_baka_deck152.jsonl` seq 31, turn 13, life 5, `at 1 AT BEST`
     → `BLOCKS: none`, dead at -3. The proposal's window.
   - `1788653548-ai_baka_deck146-…-vs-ai_baka_deck152.jsonl` seq 22, turn 13, life 8, `at 3 AT BEST`
     → `BLOCKS: B2:A3, B1:A2`, reply "taking 5 damage, surviving at 3 life". Correct, unprompted,
     off the same clause in the same left-to-right order.
   So the misread is 1 of 2, not 1 of 1, and the ordering the proposal blames did not stop the
   other seat. Nothing in G1's text is knowledge the deck146 seat needed.

3. **The instruction is already on the same screen, and was overridden, not missed.** The deck123
   seq-31 prompt prints, below the header, `Your life: 5. Unblocked, these attackers deal up to 8
   - you would be at -3 - LETHAL if it all connects (at 0 life you LOSE - 0 is not survival):
   block enough to survive.` and `Trust it over your own arithmetic; never re-derive it.` The
   reply re-derived it anyway ("We take 8 damage, go to -3, and lose"). Adding a fourth statement
   of a rule the seat overrode twice on one screen is the "more prose the model is overriding"
   that deck123's own guide verdict correctly calls noise.

4. **Standing ruling.** wave62/general-strategy.md, the wave-62 rejection list: *"A guide
   compensation for R293, R294 or R303. REJECTED as doctrine, ACCEPTED as debt… No guide may
   hedge against a printed bound (#324, and R303 is exactly a printed bound that lied); the fix is
   the render's."* R303 was a printed bound that LIED and a guide hedge was still refused. Here
   the printed figure is correct (26 1/1 Humans gang-blocking the two 2/2s absorbs 4 of the 8; the
   4 from the flier lands; 5 - 4 = 1). A hedge against a bound that is merely awkwardly ordered is
   a weaker case than the one already rejected.

5. **The proposal's own release condition is the right route, and it is unshipped, not adopted.**
   G1 says: *"If that ships, this proposal becomes a restatement of a true, well-ordered surface
   and should be REJECTED rather than adopted — the render fix is the better lever."* That render
   fix is `wave63/deck123/review.md` HIGH-2 (state survival affirmatively and before the
   `this KILLS you` clause, or suppress that clause when the AT-BEST figure is above 0). It is
   filed, it is a mechanism proposed by that reviewer — not the owner's position — and it belongs
   on the engine docket, not in a general-guide amendment. **Recorded here as debt, in the wave-62
   shape: REJECTED as doctrine, ACCEPTED as debt.** The engine seat did not file this item
   independently (`engine-seat.md` has no `AT BEST` occurrence; its E3b verdict is UNTESTED on the
   different `no block saves you` clause, 0 occurrences), so deck123's HIGH-2 is the sole filing
   and should not be lost when this wave closes.

**Skill proposals:** none filed. deck123 states its two HIGH items are instances of existing
perception-routing and trust doctrine already in the wave-59 skill; that reading is correct and no
amendment is warranted.

---

## Deck-guide edits vs adopted general changes

Nothing was adopted, so no deck-guide edit can contradict an adopted general change. Recorded for
the next wave: deck123's guide **Edit 2** is the deck-scoped form of the rejected G1 (it names the
`AT BEST` figure as the decider and removes a DIY-subtraction instruction), and deck123's guide
**Edit 3** (ignore the header's named assignment on a `NOT lethal` header) is the deck-scoped
counterpart of its own HIGH-1 render item. Both remain deck-scoped and permitted; if the HIGH-1 or
HIGH-2 render fixes ship, both edits become restatements of a true surface and should be re-read
then. No deck guide was edited by this seat.

## What I did NOT check

I did not re-run the engine seat's census or re-adjudicate any lane prediction; I took
`engine-seat.md` as given except for the two `AT BEST` figures I re-derived on disk. I did not read
the six deck guides in `bin/Res/ai/baka/` or any `wave63/deck*/strategy.txt` beyond the edit lists
in the reviews. I did not read the hung games' partial translogs, `corpus-hang.md`, or
`softlock-blockers-1788650768.md` — no proposal turned on them. I did not verify any card
primitive this wave (no adopted item depends on one); deck123's LOW-5 Lightmine Field timing claim
is unchecked by me. No git, no build, no wagic run, no edits under `bin/Res` or `src/`.
