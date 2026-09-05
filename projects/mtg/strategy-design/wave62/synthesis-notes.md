# Wave-62 synthesis — 2 proposals filed, 1 ADOPTED, 1 REJECTED

Input: `wave62/deck123/general-proposals.md` (the only proposals file this wave — no other deck
seat filed general proposals, and **no deck filed skill proposals**, so
`wave62/strategy-writing-skill.md` was not created and the wave-59 edition stands unchanged at
amendments 1-331). Also read: all seven `wave62/deck*/review.md` and `wave62/engine-seat.md`.
Adopted edition: `wave62/general-strategy.md` — `wave59/general-strategy.md` byte-verbatim for its
first 57,114 bytes (verified with `cmp -n 57114`), with `# WAVE-62 ADOPTED — R331` appended.

Standing bar applied: wave 58 adopted 14 of 14 and the owner named that the busywork pattern; waves
59-61 adopted 0. REJECT unless the proposal is paid for by a decision the corpus shows going wrong.

---

## P1 — blockers emitters unpriced against an opposing life LOOP → **ADOPT as R331**

**Paid for, and the payment is a lost game.** Re-derived on disk, not taken from the proposal:
`1788633085-ai_baka_deck123-…-vs-ai_baka_deck126.jsonl` **s76** (turn 15, `kind: blockers`, seat 16
life, 83 blockers available, one 1/1 lifelink attacker, Sanguine Bond + Exquisite Blood on the
opponent's battlefield) prints `best case with every blocker assigned: you would be at 16 - one
legal assignment that reaches it: Thraben Doomsayer #1 blocks Vampire #1` and, on every B row,
`{their attacker's lifelink, this block: they gain 1 from this block only, …}` beside `(you kill
it, your blocker lives)`. The seat replied `BLOCKS: B1:A1` — the header's own assignment. **s77**
events: `Opponent gained 1 life (now 25)`. **s78** events: eleven `You lost 1 life` / `Opponent
gained 1 life` pairs, **16 → 0**; **s79** `gameend`, 0 vs 40, from a board of 83 creatures.
`BLOCKS: none` ends that combat at 15.

Three things carried it over the bar where a normal render nicety would fail:
1. It is a **false** surface, not a missing one — the word "only" and a `best case` of 16 are both
   untrue on that screen, and the same prompt's converter banner says the opposite
   (*"ANY nonzero payment on a tag above is fatal, not merely expensive"*). Narration that lies and
   contradicts itself on one page is the class the engine seat ranks HIGH (its HIGH-1/HIGH-3/HIGH-4
   are the same family).
2. It is the **exact mirror of a fix this wave already shipped**: lane X's D2 withdrew this same
   bounded `blockGain` quantity from `attackTotalLine` under `oppLifeLoop`. The ask is to feed the
   existing `playerHasLifeLoop()` result to the two emitters that were missed, not to invent a
   mechanism. (D2 itself adjudicated UNTESTED — engine-seat §2, lane X — so the attack-side branch
   has not yet been seen to fire; that is an argument for finishing the family, not against it.)
3. The deck-side patch is **already written and is strictly worse**: deck123's guide edits 1-3 teach
   the pilot to distrust a line the engine could simply stop printing. Guide bytes are scarce
   (that guide is at 70,995 B of a 71 KB band); the emitter fix is not.

Scope is stated honestly in R331 and is thin — 1 of 21 blockers windows, 1 of 1 with an opposing
loop. Adopted on decisiveness and falsity, not volume.

**Scope condition written into the adoption (this is the adoption's limit, not a note).** The
withdrawal is gated on `oppLifeLoop`, never on lifelink generally. `wave62/deck162/review.md`
(vs152, **seq 9**) shows the bounded clause `{their attacker's lifelink, this block: they gain 5
from this block only}` on a board with **no** loop, where the figure is correct and the seat's
error was strategic; deck162's guide edit reasons from that line. A version of R331 that suppressed
the clause on every lifelink block would contradict that deck guide and remove a true number.

**No adopted change contradicts any deck agent's guide edit.** deck123 edits 1-3 are consistent with
R331 (they take a `perception status: CLOSED` clause when it ships, as R331 records); deck162's is
protected by the scope condition above; deck125/126/130/146 edits and deck152's KEEP touch nothing
in this seam. No deck guide was edited by this seat.

## P2 — emit the generic-cost clarifier on `Mana available: 0 total` lines → **REJECT**

The **counts are true and I reproduced them**: over all 44 seat files, `mana of ANY colour pays a
generic cost like {2}` renders on **1,922 of 1,922** nonzero `Mana available:` lines and **0 of
177** `Mana available: 0 total (no untapped sources)` lines. The proposal's *causal* claim is what
the corpus refutes.

* **The one zero-mana instance did not produce a wrong decision.** At
  `1788633083-…deck123-…-vs-ai_baka_deck130.jsonl` **s27** the reply does spend 7,680 bytes deriving
  that coloured mana cannot pay generic and that "this deck is effectively dead" — and then takes
  `Put a card onto the battlefield with Marsh Flats targeting Underground Sea`, i.e. **the correct
  play**, the one that fixes the mana. The cost was latency (624 s) and a truncated plan, not a
  misplay. deck123's own review attributes that game to Ponza stripping the lands, not to this.
* **The proposal's corroborating windows argue against it.** All three other cited replies
  (`126v125` s73, `126v146` s58, `152v162` s7) are on screens where the clause **was** present —
  their `Mana available:` lines read 3, 7 and 2 total. Each model raised the misconception and
  **self-corrected within the same reply** ("So I CAN make {B}"; "{2}{G} is 2 generic + 1 green …
  Yes"), then chose correctly (pass, cast nothing, play Barkchannel Pathway). So the clause neither
  prevents the confusion where it renders nor was the missing cause where it does not.
* **Nothing else in the wave supports it.** No other deck review mentions the rule at all
  (`grep -i generic|colou?red mana` over the six other reviews: 0 hits), and the engine seat filed
  no item in this family.

That leaves a ~50-byte emitter change paid for by zero wrong decisions and one expensive reply —
which is the bar waves 59-61 rejected on. Re-file it when a zero-mana window is shown to produce a
wrong choice, or fold it into a lane that is already touching that emitter for another reason.

*(Recorded as filed, not adopted: deck123 deliberately did **not** file its HIGH-2, the answer-line-
before-reasoning seam at `146v123` s18 — ledger D9 / wave-59 deck123 P1 — and named re-proposal as
the busywork pattern. That judgement is endorsed here; D9 stayed UNTESTED this corpus, so the seam
is still open on the docket without a new item.)*

## Files written by this seat
* `wave62/general-strategy.md` — wave-59 edition carried byte-verbatim, `WAVE-62 ADOPTED — R331`
  appended (61,905 B; prefix verified with `cmp -n 57114`).
* `wave62/synthesis-notes.md` — this file.
* Nothing else. No deck guide edited, no skill edition written, no file outside `wave62/` touched,
  no git, no wagic, no build.

## What I did NOT check
* I did not re-adjudicate any lane prediction; engine-seat.md's PASS/FAIL/UNTESTED verdicts are
  taken as given, including D2's UNTESTED, which R331's rationale leans on.
* I did not read the source; whether `blockGain`'s two emitters can in fact reach the
  `playerHasLifeLoop()` result is the proposal's claim from the rendered page, not a traced call
  path (same caveat the engine seat records for its own mechanism sentences).
* I verified card facts only against the primitives (`mtg.txt:99005` Sanguine Bond,
  `mtg.txt:37967` Exquisite Blood) — no Scryfall call was needed, as no verdict here turned on a
  disputed Oracle text.
* I did not audit the six non-deck123 guide rewrites for internal quality — only for contradiction
  with the one adopted change.
