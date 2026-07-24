# Deck-135 (Modern Snow) — Wave-22 game review

Corpus: PRIMARY `matchups-20260723-173843` (21-game round-robin; binary `/tmp/wagic-1d8ed8194`
= the wave-22 step-1 batch, THIS corpus is its first live validation). Pool CHANGED from wave-21
(135,62,27,102 continuing + 109/44/140 rotated in; 14/35/49 rotated out) — so cross-wave record
comparison is WEAK evidence this cycle (matchup difficulty shifted for everyone). deck135 guide
FROZEN (byte-identical to live `Res/ai/baka/deck135_strategy.txt`, `cmp`-verified — see
strategy.txt). deck135 OWNS: ENGINE-F1 fetch-resolution validation, R1 reveal-partition regression
watch, Azcanta transform-annotation + activation window, the 1 stale_echo fallback, and the
1/6->4/6 attribution. Doctrine: win column CONTEXT ONLY; unit of analysis is the DECISION with
seq repros.

## RECORD (context only — NOT a verdict) — 4W / 2L  (UP from wave-21's 1/6; wave-20 was 4/6)

| game    | opp              | my/opp final | turn | outcome        | wave-21 (diff pool) |
|---------|------------------|--------------|------|----------------|---------------------|
| 135v27  | 27 Zombies       | 11/-4        | T12  | WIN (opp dead) | LOSS -4/19          |
| 135v102 | 102 Tergrid      | 16/-5        | T12  | WIN (opp dead) | LOSS 5/19           |
| 135v109 | 109 Mono-R Aggro | 1/-8         | T10  | WIN (opp dead) | (new opp)           |
| 135v62  | 62 Enchantresses | 17/7         | T11  | WIN (ahead)    | WIN adj 17/14       |
| 135v140 | 140 RBW grinder  | 17/18        | T10  | LOSS (adj, opp ahead) | (new opp)    |
| 135v44  | 44 Faerie Tempo  | 4/10         | T14  | LOSS           | (new opp)           |

Win table assigns 135 = 4/6. GAME -> FILE MAP (owner seat = first name):
- vs27  `1784846326-ai_baka_deck135-0x55877632e880-vs-ai_baka_deck27.jsonl`   (WIN)
- vs140 `1784847735-ai_baka_deck135-0x564767d538d0-vs-ai_baka_deck140.jsonl` (LOSS adj)
- vs102 `1784852077-ai_baka_deck135-0x55e7c44a9920-vs-ai_baka_deck102.jsonl` (WIN)
- vs44  `1784852308-ai_baka_deck135-0x5651e1e31960-vs-ai_baka_deck44.jsonl`  (LOSS)
- vs109 `1784852536-ai_baka_deck135-0x563384947a90-vs-ai_baka_deck109.jsonl` (WIN)
- vs62  `1784857812-ai_baka_deck135-0x55edde0a38e0-vs-ai_baka_deck62.jsonl`  (WIN)

---

# HEADLINE 1: ENGINE-F1 FETCHLAND-FIZZLE FIX — **CONFIRMED FIXED. 10/10 cracks resolved cleanly, ZERO fizzles, including the exact wave-21 same-turn-land-drop repro shape.**

The detector (per brief item a / wave-21 notes ENGINE-F1): every chosen `Put in Play with <fetch>`
must show `<fetch>: battlefield -> graveyard` (sacrifice completed) + `<land>: library ->
battlefield` (land entered) in the next record's events. Broad sweep over EVERY fetch-crack CHOICE
across all 6 games:

| game  | crack seq(s)   | fetch(es)                          | resolution (life paid + grav + land) |
|-------|----------------|------------------------------------|--------------------------------------|
| vs27  | s3, s7         | Misty Rainforest, Flooded Strand   | BOTH clean (grav=1 land=1 each)       |
| vs140 | s3             | Windswept Heath                    | clean                                 |
| vs102 | (none cracked) | —                                  | n/a                                   |
| vs44  | s6, s11        | Misty Rainforest x2                 | BOTH clean                            |
| vs109 | s3, s11        | Flooded Strand, Misty Rainforest   | BOTH clean                            |
| vs62  | s3, s10, s18   | Misty Rainforest x2, Windswept Heath| ALL THREE clean                      |

**10 cracks, 10 clean resolutions, 0 fizzles.** Contrast wave-21: ~4 resolved / ~20 cracked
(three total-fizzle games). The fizzle mechanism is gone.

## The FIXED CASE (crack after a same-turn land drop) is DIRECTLY confirmed.

The 2016 zero-cost-branch guard refused any land with the land-drop spent — including an in-play
fetchland cracked the same turn its fetchland was played from hand. Two cracks this corpus are the
EXACT wave-21 fizzle signature (fetchland played from hand, then cracked SAME turn = land-drop
spent), and both resolved:

- **vs27 s3** (T0/opening, Main phase 1), verbatim events of the crack record then s4:
  ```
  - Your Misty Rainforest: hand -> battlefield        <- land drop spent (fetchland from hand)
  - You: Put in Play with Misty Rainforest targeting Snow-Covered Forest [cost: Tap, Life, Sacrifice]
  (s4) - Your life -1 (now 19)                          <- cost paid
        - Your Misty Rainforest: battlefield -> graveyard   <- SACRIFICE COMPLETED (was missing in wave-21)
        - Your Snow-Covered Forest: library -> battlefield  <- LAND ENTERED (never happened in wave-21)
  ```
- **vs140 s3** — the wave-21 repro used *Windswept Heath from hand -> crack same turn* (vs35 s6);
  here the SAME card in the SAME shape resolves cleanly:
  ```
  - Your Windswept Heath: hand -> battlefield
  - You: Put in Play with Windswept Heath targeting Snow-Covered Forest [cost: Tap, Life, Sacrifice]
  (s4) - Your life -1 (now 19)
        - Your Windswept Heath: battlefield -> graveyard
        - Your Snow-Covered Forest: library -> battlefield
  ```

No re-arm loop occurred anywhere (a resolved fetch removes its own crack option — the fetchland is
in the graveyard, so the option cannot re-list). The model never faced a "silent fizzle it can't
perceive" (ENGINE-F1b) this corpus because there were no fizzles. CLOSE ENGINE-F1.

---

# HEADLINE 2: ENGINE-R1 (Glacial Revelation reveal partition) — **THIRD independent confirmation. Regression detector NEGATIVE on all 3 true Glacial reveals. Stays CLOSED.**

20 reveal records at this seat. Classified by prompt signature: 16 are library-search / Scrying
Sheets reveals (search-and-shuffle "to choose card" — all cards `goes to library`, no graveyard
partition, R2/R3 territory, non-actionable), leaving **3 true Glacial Revelation ("get snow")
reveals** + 1 Scrying-Sheets single-card (vs44 s38, see below). All three Glacial partitions are
CORRECT (snow -> hand, non-snow -> graveyard):

- **vs102 s16** (reveal 6): PUT Icehide Golem, Ohran Viper, Icehide Golem, Arcum's Astrolabe x2
  (5 snow) -> ALL `goes to hand`; Glacial Revelation (non-snow) -> `goes to graveyard`. Mixed
  partition, correct.
- **vs44 s32** (reveal 6): PUT Treefolk x2, Ohran Viper, Ice-Fang Coatl (4 snow) -> hand;
  Prismatic Vista + Into the North (non-snow) -> graveyard. Correct.
- **vs62 s23** (reveal 6): PUT Boreal Druid, Arcum's Astrolabe, Icehide Golem (3 snow) -> hand;
  Into the North, Search for Azcanta, Glacial Revelation (non-snow) -> graveyard. Correct.

Zero all-to-graveyard behavior. With wave-20 (n=1) and wave-21 (n=2) this is the THIRD corpus and
the partition is correct on both mixed-eligibility and all-eligible shapes. R1 regression-watch:
NEGATIVE.

Minor (NOT R1): **vs44 s38** is a Scrying-Sheets single-card top-reveal (option list = one card,
`revealed 1 and kept them all (put in library)`), not Glacial. The model kept a snow Ice-Fang
Coatl in library ("PUT: none") while its reasoning conflated library/graveyard/hand — it correctly
declined to bin the card but the reasoning shows the same library-vs-graveyard confusion seen on
Scrying-Sheets prompts. No card lost; low-signal. Routed to notes as a representation nit.

---

# HEADLINE 3: AZCANTA — **the multi-wave block is BROKEN. The (d) "[available NOW...]" annotation FLIPPED the wave-21 declines into ACCEPTS, and the ACTIVATION path finally became reachable.**

Wave-21's finding: the Transform option carried no eligibility annotation, the model manually
miscounted the graveyard, and DECLINED a beneficial available transform TWICE — so the R4
activation path never opened (untested for multiple waves). This corpus shipped annotation (d):
`Transform:azcanta, the sunken ruin [available NOW - this transform is only offered because its
condition is already met; do not recount, it is legal this instant]`.

**Three transform windows occurred; ALL THREE transformed (0 declines):**
- **vs140 s24** (T10): CLEAN MODEL ACCEPT (`CHOICE: 1 (Transform...)`). Reply cites the annotation
  logic: "I have 7 or more cards in graveyard... Transforming... is a free value gain... There is
  no reason to decline this free upgrade." The wave-21 miscount-spiral is GONE — the model trusts
  the offer.
- **vs44 s36** (T14, at 4 life): CLEAN MODEL ACCEPT (idx1, Transform).
- **vs27 s27** (T12): the model INTENDED to accept (`CHOICE: 1 (Transform...)`, full transform
  reasoning) but the pick fell to a `stale_echo` fallback (see below) — the heuristic transformed
  it anyway (events confirm transform + activation resolved). Intent = accept.

So annotation (d) decisively fixes the ENGINE-R6 decline problem: 3/3 transformed vs wave-21's 0/2.

**The R4 ACTIVATION path is now REACHABLE and was OFFERED — but the model PASSED every window.**
After transform, the `look top 4 cards with Azcanta, the Sunken Ruin [cost: {2}{u}, Tap]`
activated ability appeared at 6 priority windows (vs27 s28/s31, vs140 s25/s28, vs44 s37/s40). The
model chose `pass` at all 6. These passes are largely DEFENSIBLE, not misplays:
- vs27 s28 (11/6, ahead, going for lethal Treefolk 9/9 trample vs opp at 6),
- vs44 s37 (4 life, T14 — passed to cast Treefolk 10/10 at s41; stabilize/creature-first is correct),
- vs140 s25 (17/18 grind — the one arguably-marginal skip; it re-cast Search for Azcanta at s30
  instead; likely also mana-gated on {2}{U}).

Consequence: the Azcanta card-selections that DID resolve in events were the top-**1** Search-for-
Azcanta UPKEEP looks (labeled with the post-transform name "Azcanta, the Sunken Ruin" — a cosmetic
labeling quirk), which correctly sent revealed CREATURES (Diamond Faerie, Icehide Golem, Ice-Fang
Coatl) to graveyard — eligibility direction correct. But a clean **model-driven top-4 {2}{U},{T}
activation with a noncreature-nonland put to hand** was NOT captured, because the model rationally
deprioritized the activation every time. R4 to-hand-slot resolution: transform barrier removed +
activation offered + engine-side eligibility direction correct, but the model-elected top-4 pick
remains UNOBSERVED (the model keeps having a better play than spending {2}{U} on card-draw).

---

# THE 1 FALLBACK — vs27 s27 `stale_echo`: a NEW echo shape (transform dual-name mismatch), NOT the shipped-(c)-fix class. Harmless here (heuristic completed the intended transform).

vs27 s27 (ask, T12, 11/6, `fallback: stale_echo`, `choice: -1`). Options:
`["Transform:azcanta, the sunken ruin [available NOW...]", "Decline - do nothing"]`.
Reply line 1: `CHOICE: 1 (Transform: Search for Azcanta)`. The INDEX (1 = Transform) is correct and
the reasoning is entirely pro-transform. The echo classifier fired `stale_echo` because the model's
echoed label — **"Transform: Search for Azcanta"** (the PRE-transform source-card name) — does not
match the option string **"Transform:azcanta, the sunken ruin"** (the POST-transform destination
face). This is a transform DUAL-NAME mismatch: the engine labels the option by the destination
face; the model naturally echoes the source card it is transforming. The shipped (c) fixes target
"<spell> targeting <target>" source-prefix and combat-line label-hijack shapes — neither covers
this. So this is a genuinely NEW echo shape the (c) fixes do not cover. Harmless this instance (the
model chose the right index; heuristic transformed; game won 11/-4), but a correct Transform pick
was routed to heuristic on a naming technicality. Route: notes HARNESS-ECHO-Transform.

Net: this fallback is a SIDE-EFFECT of fixing the decline — annotation (d) made the model
confidently accept, which exposed the transform-face echo gap. Fixing it (accept an echo matching
EITHER the source or destination face, or match on the "Transform:" prefix + index) would convert
this last transform residual into a clean accept.

---

# ANSWER-FIRST — WORKING; 100% at this seat, no deliberation-quality regression.
Every decision reply across all 6 games leads with the coded line (CHOICE/ATTACK/PUT/BLOCK):
33/33, 30/30, 35/35, 43/43, 27/27, 38/38 = 206/206. Zero unparsed, zero empty (only the 1
stale_echo). Longest replies are still 12k-15k-char decode spirals (HARNESS-3 carried, model/decode
item — the Azcanta transform reasoning at vs140 s24 = 14.5k chars — but it landed on the RIGHT
answer, so no decision degraded). Answer-first holds.

# OBEDIENCE CHECK — no guide leak.
- Rule #1 (deploy a creature): obeyed. vs44 s41 cast Treefolk (10/10) over the fetch AND the
  Azcanta activation at 4 life = textbook creature-first. vs27 s23 Treefolk over Search/Druid.
  No "durdle on value over a listed creature" seen.
- Rule #2 (attack profitably): obeyed. Wins closed by attacking (vs27/vs102/vs109 all put opp to
  negative life). No 0-attacker durdle at a profitable window.
- The two losses trace to matchup/draw, not guide text: vs140 = ground out by the RBW grinder to
  an adjudicated 17/18 (both alive T10, opp one point ahead); vs44 = raced low by Faerie tempo to
  4 life (finisher landed at s41 but too late). Zero decision-level guide leak. Guide stays FROZEN.

# 1/6 -> 4/6 ATTRIBUTION (honest; pool changed so treat cross-wave record as weak).
The record swing is REAL but the pool swap is a confound (35/14/49 out, 140/44/109 in). What CAN
be attributed at the mechanism level: wave-21's regression was DRIVEN by fetch-fizzle (three
total-fizzle games = permanent green screw = losses). This corpus: **fetch resolution went from
~20% to 100%**. The two SAME-opponent games available as a control both flipped loss->win with
clean fetches: **vs27** (persistent wave-20/21 loss) -> WIN 11/-4, and **vs102** (wave-21 flip-loss
5/19) -> WIN 16/-5. Both wins featured fully-resolved fetches feeding on-curve creatures. No other
deck135-specific variable changed (guide frozen, core prompt symmetric, answer-first already live
in wave-21). So the fetch fix is the best-supported primary driver of the recovery; the pool change
is a real but unquantifiable secondary confound. Honest verdict: **fetch fix >> pool change**, but
not cleanly separable on n=6 with a changed field.

# CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate).
- Fetch-color dependence: the compounding risk is GONE now that ENGINE-F1 is fixed — the deck's
  "fetch a color, cast a {G}{U} threat" plan executed reliably (10/10). The 7-corpus fetch-fragility
  flag downgrades from "compounds with an engine bug" to "ordinary color-screw variance."
- Under-powered small-creature clock — STANDS but softened: Treefolk closed vs27/vs102/vs109 (all
  wins) once mana cooperated. Diamond Faerie (G/W/U) not seen stranding this corpus.
- Force of Negation: NOT cast this corpus (no board-wipe window). Reflexive-off-whitelist watch
  neither cleared nor re-triggered — stays at WATCH.

# GUIDE DISPOSITION: FROZEN (byte-identical to live; `cmp`-verified — CMP reported byte-identical).
strategy.txt is a byte-copy. See notes.md for the layer-routed ledger and rotation verdict.
