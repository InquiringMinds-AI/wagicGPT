# Wave 11 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: PRIMARY `matchups-20260715-072254` (21 games, round-robin over 44,109,110,131,133,135,140;
binary `/tmp/wagic-54a5a9d01` = wave-10 engine batch + max_tokens 4096 + parse ramble guards +
stack-target annotation fix + fetch pre-targeting keying; wave-10 guides byte-identical; core 7th PASS;
answer-after-plan reply protocol). deck133 was the GPT seat in 6 games.

**Real record 5W / 1L — BEST-EVER (up from 4W/2L wave-10).** WON vs110, vs131, vs135, vs109, vs140;
LOST vs44. **All five wins are clean KILLS (opponent to 0 or negative), none adjudicated** — a step up
from wave-10's two adj wins. The one flip from wave-10 is **vs109 loss -> WIN**. Per binding doctrine the
win-rate is context only (draw variance landing favorably ON TOP of high obedience); the guide is
byte-identical to the wave-10 output (freeze-check below, `diff -q` clean).

| opp | result | end my/opp | last T | fb | note |
|-----|--------|-----------|--------|----|------|
| 110 | **WON** | 9 / **-2**  | 17 | 0 | affinity; GM drain-8 stabilizer (my9<=10); ONE self-Fatal-Push (stack-target, won anyway) |
| 131 | **WON** | 11 / **-2** | 19 | 1 | Liliana Veil + Last Hope coexist-casts/activations; grind kill |
| 135 | **WON** | 19 / **-2** | 19 | 3 | GM drain-7 LETHAL (N=7>=opp5); ONE self-Fatal-Push (stack-target, won anyway) |
| 109 | **WON** | 6 / **0**   | 11 | 2 | FLIP from wave-10 loss: Geralf's + Yawgmoth -1/-1 grind + Obliterator + beats close |
| 140 | **WON** | 14 / **-2** | 27 | 4 | 27-turn grind; GM drain-4 build-cast (no creature offered) + Obliterator/Yawgmoth beats |
| 44  | LOST | **-7** / 20 | 12 | 2 | Faerie flyer tempo; opp STATIC at 20 all game (0 damage dealt) — no reach/flyer answer |

**Mode: OBEYED / TURNAROUND-PRESERVATION.** The wins execute the guide's own lines (beatdown curve; GM
as finisher vs135 AND stabilizer vs110/vs140; Yawgmoth/Obliterator beats vs109/vs131; Liliana
coexist-casts vs131). The single loss is the recurring construction race the deck cannot answer (no reach
vs Faeries — same flag as wave-9 #9 / wave-10 #9). Disposition: **guide FROZEN, byte-identical.**

## Harness / representation sweep — CLEAN
- **0 fails-validation, 0 DESYNC** in all 6 game stderrs; **0 defer records in the JSONL** (the
  `AIPlayerGPT[phN]: defer to heuristic` stderr lines are the fallback mechanism, logged per-game for BOTH
  GPT seats; deck133's own fallbacks are the 12 below).
- **12 fallbacks = 11 empty_reply + 1 unparsed_reply** (vs131 x1, vs44 x2, vs110 x0, vs135 x3, vs109 x2,
  vs140 x4). Down from wave-10's 14. **Every fallback record has choice=-1 and the heuristic's action —
  ZERO prose-scanned casts** (RAMBLE-GUARD VERIFIED, section A). None changed a game (audited section E/1).

---

## VERIFICATION LIST (the four deck133 focus items)

### (A) FOCUS 1 — RAMBLE-GUARD VERIFICATION: PASS. Zero prose-scanned casts; the wave-10 self-Fatal-Push-via-int-grab class is CLOSED.
Wave-10's signature leak (deck110 seq20: a formatless ~7000-char ramble truncated at the 2048 cap,
int-grabbed by the parser to a self-Fatal-Push) is GONE. Every one of the 12 fallback records this corpus
is a clean `fallback=empty_reply` (11) or `fallback=unparsed_reply` (1), **all with `choice=-1` and the
heuristic playing** — no record carries a prose-scanned integer cast.

- **11 empty_reply**: all `replen=0`, `latency=120004ms` — the known HTTP 120s-timeout tail-generation
  class (harness fix WAGIC_GPT_TIMEOUT 240s ships next corpus). The model returned nothing; the guard fell
  straight to the heuristic (choice=-1). Do not re-diagnose.
- **1 unparsed_reply** (vs44 seq29, replen=621, lat=8853ms): a GENUINE reply, well-formed
  (`PLAN: ... CHOICE: 3`), that named Geralf's Messenger and emitted `CHOICE: 3` — but the option list held
  only 2 options (`1. Cast Thoughtseize / 2. Cast nothing`). The out-of-range index was **rejected** ->
  choice=-1 -> heuristic. This is the wave-10 "unoffered-desired-card" class, now landing OUT OF RANGE and
  safely caught instead of int-grabbing an in-list harmful option. The ramble guard + range check did
  exactly their job.

**Verdict: the ramble guard is verified end-to-end at this seat.** The wave-10 number/parse self-Fatal-Push
mechanism (formatless-truncation int-grab; in-range fallback-digit mapping) did NOT recur.

### (B) FOCUS 2 — Thin-hand / empty-hand Thoughtseize annotation: STEADY (100% obeyed, second wave running).
48 Thoughtseize option windows. The annotation renders `... - the opponent's hand is EMPTY: nothing to
strip` (empty) and `... - the opponent holds only N card(s)` (thin).
- **EMPTY-hand windows: 0 casts** (vs110 seq20/23/26/27/28/30/32/35/39/45; vs140 seq26/32/35/38/41/42/44/47/49).
- **THIN-hand windows: 0 casts** (vs44 seq24/28; vs110 seq13/14/16; vs140 seq17/20/22/51/52/53/58/60/65/67/73).
- **Every real Thoughtseize cast was a FULL/other hand on an early turn** (vs131 seq12/17 T5; vs110 seq3 T1;
  vs135 seq3 T1; vs140 seq13 T5) — all sanctioned early seizes. Zero empty/thin seizes, zero
  fabricated-card-over-empty-hand replies. The wave-9 fabrication class stays CLOSED.

### (C) FOCUS 3 — GM bare-N + Liliana coexist: STEADY.
**Gray Merchant bare-N** — 3 casts, shown N == resolved every time; no N->N+2 regression:
- vs110 seq39 (T15, my9/opp16): `drains 8` -> my9->17 (+8), opp16->8 (-8). Stabilizer (my9<=10). Correct.
- vs135 seq50 (T19, my12/opp5): `drains 7` -> my12->19 (+7), opp5->-2 (-7) = **lethal** (N=7>=opp5). Correct.
- vs140 seq32 (T13, my16/opp24): `drains 4` -> my16->20 (+4), opp24->20 (-4). Build-cast; see section (D).
- GM offered 3x, chosen 3x (no WAIT window occurred — the bare-N wait CLAUSE was not exercised this corpus).

**Liliana coexist** — both Lilianas cast AND activated, all resolved, 0 validation/defer/loop:
- Liliana of the Veil: cast vs131 seq28 & seq34 (+1 seq29/seq35, -2 sac seq37), cast vs110 seq8 (-2 sac seq9),
  cast vs109 seq41.
- Liliana, the Last Hope: cast vs131 seq50 (-2 mill/return seq51) — coexists with a live Veil the same game.
- 0 fails-validation / 0 defer / 0 loops across all 6 stderrs. Fixed class stays dead.

### (D) GM build-cast at my16/opp24 (deck140 seq32) — WITHIN TOLERANCE, not a bare-N leak.
The guide's "N < their life AND your life above 10 -> WAIT, take a creature to build devotion" clause is
scoped to *taking a creature from lower on the list*. At this window **no creature was offered** (options:
Fatal Push own-only / GM drains-4 / Thoughtseize-empty / Cast nothing). With opp at empty hand + empty
board, GM-for-4 (gain 4, chip 4) dominates Cast-nothing. The reply used the shown number directly
("drains 4 life... me to 20") — bare-N obeyed at the number layer even while the model mused (correctly
without overriding) about why devotion 6 shows as 4. Not a leak; a defensible no-creature-available cast.

---

## (E) NEW FINDING — self-Fatal-Push via a STACK-TARGETING rules misconception (2 instances, both WON, non-deciding). A DIFFERENT mechanism from wave-10; routes to REPRESENTATION/MODEL, not guide.

Two non-fallback casts landed on the annotated `Cast Fatal Push {b} - the only legal targets are YOUR OWN
right now` option. Unlike wave-10 (where the reasoning OBEYED the annotation and the NUMBER leaked), here
**the reasoning DISOBEYS the annotation** by fabricating that Fatal Push can destroy an opponent's creature
**spell on the stack**:

- **vs110 seq25 (T10, my9/opp18).** Opponent is casting Steel Overseer (`hand -> stack`). PLAN:
  *"The Steel Overseer is on the stack. It has mana value 2. Therefore, Fatal Push can destroy it
  immediately... I will cast Fatal Push targeting the Steel Overseer."* `CHOICE: 1` -> resolved onto own
  Bloodghast (`Your Bloodghast: battlefield -> graveyard`; returned via landfall). The Overseer resolved
  onto the battlefield untouched. The model conflated an instant's stack-timing with a counterspell's
  ability to remove a spell.
- **vs135 seq45 (T14, my14/opp11).** Opponent is casting Ice-Fang Coatl (`hand -> stack`). PLAN:
  *"I will cast Fatal Push targeting the opponent's Ice-Fang Coatl to destroy it."* `CHOICE: 1` -> resolved
  onto own Bloodghast (enchanted by Gelid Shackles, could not block anyway; returned via landfall). Same
  mechanism.

**Ratio:** across 46 own-target Fatal Push windows (vs110 x6, vs135 x12, vs140 x28), the annotation was
obeyed 39/41 real-choice windows (~95%; 5 fallbacks excluded), with these 2 leaks. This matches wave-10's
27/29 obey rate — the annotation is working; the 2 leaks are a MODEL rules-misconception the annotation's
current wording does not inoculate against (it says WHICH targets are legal, not WHY the on-stack creature
is not one).

Both cast onto Bloodghast, which returns via landfall the same/next turn, so both were self-limiting and
in games that were won — **non-deciding, but a genuine and NEW decision-failure class.** Route:
representation (a low-priority option-line refinement naming the on-stack creature as untargetable) +
model (the stack-vs-battlefield / destroy-vs-counter confusion). See notes.md #1. **NOT a guide line** —
single seat, and the annotation is present-and-obeyed-95%: a louder guide rule is the sanctioned-against
escalation (representation-beats-instruction).

---

## MANDATORY AUDITS

### (1) Fallbacks — none changed a game.
- **vs44 (the only LOSS)** carried 2 fallbacks: seq20 (empty_reply, a fetch-crack priority window T9
  my15/opp20) and seq29 (unparsed_reply, the out-of-range CHOICE:3 above). Neither was deciding — the loss
  is structural (below). The empty_reply clusters in the WON games (vs135 x3, vs140 x4, vs109 x2) all fell
  on Fatal-Push-own-only / fetch / long-grind windows where the heuristic played a SAFE line (fetch crack,
  land, Yawgmoth activation, Bloodghast attack, Cast nothing). **I verified no self-Fatal-Push resolved at
  any empty_reply window** — the heuristic did not cast the annotated-bad option.

### (2) The vs44 loss is CONSTRUCTION, not play.
opp44 (Faeries) sat **STATIC at 20 life the entire game** while deck133 bled 20 -> 4 -> dead (T12).
deck133 dealt ZERO damage: no reach, no flyer answer, Gray Merchant never online against a static 20, and
its own fetch/discard/Yawgmoth self-damage did the rest. Same recurring flag as wave-9/wave-10 #9. Obedience
was high (curve deployed, removal used). NOT a rewrite trigger — route to deck construction (notes #4).

### (3) Convergence watch — reflexive high-life blocking: NEGATIVE for this deck.
0 blocker windows all corpus (structural: Bloodghast can't block, walls draw-dependent, games were
beatdowns/races/grinds). No `[deals 0]` blocker windows. No reflexive-block possible at this seat.

### (4) Fetch behavior — clean.
Fetches cracked when a land was needed; the 2 fetch-window fallbacks were empty_reply timeouts (heuristic
handled). No single-option "Put in Play" re-ask inflation as a decision-count problem — consistent with the
consume-on-choose / pre-targeting-keying engine changes. Not a leak.

---

## FREEZE-CHECK (turnaround-preservation — every kept line accounted for; ZERO edits)
Live guide is BYTE-IDENTICAL to the wave-10 output (`diff -q` clean). Every line stands:
- **Identity + beats-are-the-win paragraph** — vindicated by 5 wins, 4 of them via beats/grind (vs109
  Yawgmoth+Obliterator; vs131 grind; vs140 beats) and GM (vs110 stabilizer, vs135 lethal). KEPT VERBATIM.
- **Impact-ranked cast list + GM #1 ELSE (bare-N)** — OBEYED; bare-N validated on 3 casts (C). KEPT.
- **GRAY MERCHANT block ("use the shown number directly")** — CONFIRMED shown==resolved x3 (C). KEPT.
- **Thoughtseize rule (empty-hand prohibition + DECIDING SITUATIONS line)** — OBSERVED OBEYED via the
  shipped annotation (B). **Demotion caveat (conditional-surface granularity) STILL HOLDS: DO NOT demote** —
  the annotation surfaces only hand=EMPTY / holds-N; the guide clause also covers "holds nothing that
  threatens you" (hidden contents) and alternate menus the annotation may not render. KEEP at full prominence.
- **own-targets / legal-target Fatal Push annotation** — renders and is obeyed 95%; the 2 residual self-casts
  are a MODEL stack-targeting misconception, not guide-writable (E). KEPT VERBATIM.
- **ROLE CHECK + DEFENDER/Obliterator-wall line** — untestable at this seat (0 blocks, structural); the wall
  renders opponent-side. KEPT VERBATIM.
- **VS RED/BURN, Bloodghast, Fatal Push, Collective Brutality, Liliana, fetch, Yawgmoth, Geralf's, Arena,
  attack, mulligan, DECIDING SITUATIONS** — obeyed or untested; KEPT VERBATIM.
Nothing dropped. **The only change this wave is ZERO — the guide is frozen and stands alone.**

## In my words
Best game yet: 5/6, and all five wins were real kills, not adjudications — the deck actually closed. The
engine batch validated cleanly at my seat. The ramble guard is the headline: every one of my 12 fallbacks
fell straight to the heuristic with choice=-1 and not a single prose-scanned cast, and the exact wave-10
leak (a formatless truncated ramble getting int-grabbed into a self-Fatal-Push) did not recur — even the
one genuine unparsed reply, which wanted an unoffered card and emitted an out-of-range "CHOICE: 3," was
just rejected. Thoughtseize discipline held at 100% (zero empty- or thin-hand seizes), Gray Merchant's
bare-N stayed shown==resolved on all three casts, and both Lilianas coexisted and activated with zero
validation noise. The one genuinely new thing is subtle and NOT the wave-10 problem: twice the model cast
Fatal Push believing it could destroy an opponent creature that was still ON THE STACK mid-cast — it read
the "your own targets only" annotation and reasoned straight past it, conflating an instant's timing with a
counterspell. Both hit its own Bloodghast (which came right back via landfall) in games it won, so they
cost nothing, but they mark a model-level rules gap the annotation's wording doesn't close. That belongs to
representation and the model, not another paragraph of guide. So the guide freezes clean.
