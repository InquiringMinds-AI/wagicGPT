# deck35 (Mighty Slivers) — REVISED-GUIDE ROUND 3 (Step 0-ter) — findings

Seat: deck35, GUR Sliver lord-stack swarm-race. AGGRO. **Third corpus with a guide; second with the
wave-19 revision** (land-drop-discipline paragraph + Gemhide-fixes-blue + mulligan blue-trap flag).
Record **3/6** (flat). Binary 95e89c204. Corpus matchups-20260719-151849.
**ZERO fallbacks (the new zero seat).**

WON: 102, 14, 62.  LOST: 27, 135, 49.  All six games ended by LETHAL (a life <=0); **none of my games
was turn-cap-adjudicated** — no adj-durdle concern at this seat.

I OWN the ATTACKER-SPARING CHECK. Verdict up front: **the attacker-sparing payment fix is REAL and
WORKS — but it did NOT collapse the 0-attacker games, because the 0-attacker signature at this seat
was never payment; it is the attackers-decision NON-ISSUANCE engine bug (my wave-19 ledger #1), which
is UNFIXED and reproduced a THIRD wave.** Detail below.

## HEADLINE 1 — ATTACKER-SPARING PAYMENT: VALIDATED WORKING, ZERO counter-examples.

The mandate: find any would-be attacker still tapped for mana with lands available. **None found in
6 games.** Positive repro that the fix fired correctly:

- **vs49 seq8 (T6):** the model cast Winged Sliver {1}{u}. The post-cast board renders
  `Forest [tapped]; ... Island [tapped]; ... Gemhide Sliver {1}{g} (1/1) [flying]` — the engine paid
  the {1}{u} with **two LANDS (Forest + Island) and left the Gemhide UNTAPPED**, preserving it as an
  eligible attacker. This is exactly the swing-neutral preference the fix promises. Pre-fix behavior
  (my wave-19 ledger #3 watch) would have auto-tapped the Gemhide.
- Corpus-wide scan of every `Your battlefield` render across all 6 games: **every** Sliver shown
  `[tapped]` was tapped by ATTACKING (rendered `[tapped - untaps and can attack next turn]`, appearing
  right after an `attackers` decision) or was an edict/sacrifice TARGET (vs102 Tergrid seq12/19). **No
  Sliver was rendered tapped-for-mana with untapped lands available.** vs49 (the 0-attacker game) had
  **zero** tapped Slivers at any point.

So the auto-tap-the-attacker hazard is closed. That is a good engine change and it is confirmed live.

## HEADLINE 2 — 0-ATTACKER TURNS DID **NOT** COLLAPSE. Brief item-(b)'s payment attribution is REFUTED at this seat.

Brief item (b): *"Root cause of the wave-19 '0-attacker durdle' at creature-mana seats was PAYMENT,
not the combat gate."* **This corpus falsifies that for deck35.** Payment was fixed and demonstrably
spared the attackers (Headline 1), yet the 0-attacker signature recurs unchanged:

**vs49 (LOSS, my -2 / opp 20) — a COMPLETE 0-attacker, 0-combat-damage game.** `attackers` decisions
issued all game: **ZERO** (jq count `.kind=="attackers"` = 0). Opponent life never moved off 20; I
died at -2 to a Dragons clock (Bloodmark Mentor / Dragonmaster Outcast / Thunderbreak Regent). This is
NOT a slow-draw excuse and NOT a payment artifact:
- **T8 (seq10/11):** board = `Gemhide (1/1) [flying, first strike]` (cast T4, unsick), `Winged (1/1)
  [flying, first strike]` (cast T6, unsick), `Striking (1/1) [flying, first strike]` (cast T6, unsick)
  — **three eligible untapped unsick attackers** — plus Might (just cast, sick). Every attacker was
  rendered UNTAPPED (Headline 1 confirms nothing was mana-tapped). The model's stated plan (seq10/11):
  *"attack with my full flying swarm next turn to close the game."* **No `attackers` decision was
  issued.** I cast Might and the turn ended; opp's T9 killed me.
- **T6 (seq6-8):** Gemhide (cast T4) was unsick + untapped + flying — an eligible attacker — and again
  **no `attackers` decision**.

The would-be attackers were UNTAPPED, so payment cannot be the cause. The cause is the engine not
ISSUING the declare-attackers decision — my wave-19 ledger #1, now with a **third independent repro**.

**vs14 (WIN) and vs62 (WIN) show the same non-issuance as SKIPPED WINDOWS, not full games:**
- vs14: `Striking` (cast T1, unsick) sat as an eligible attacker on **T3, T5, T7** with **no attack
  step** on any of them; first `attackers` decision was **T9 (seq17)**. Won anyway (Deep Blue is slow;
  the board snowballed to a 7-wide double-strike alpha strike at seq24).
- vs62: `Gemhide` (cast T4, unsick) eligible on **T6 and T8** with **no attack step**; first attack
  **T10 (seq16)**. Won anyway (Enchantress is slow; Bonesplitter+Muscle made a 7-wide 4/2-flyer team
  that overkilled for 22).

The non-issuance cost only tempo in the two slow matchups (won) but was the PRIMARY cause of the vs49
loss (a fast matchup where lost tempo = lost game).

## HEADLINE 3 — sharpened (but still partial) hypothesis for the non-issuance.

The step is **inconsistent** — it fires on some own-turns and skips others in the same game. Best
correlate this corpus (hand to engine; do NOT treat as proven):

- **Reliably ISSUED when a HASTE creature is present.** Every game where Heart Sliver was on the board
  attacked every own-turn thereafter: vs102 (Heart T3 -> attacks T3/T5/T7/T9/T11), vs135 (Heart T3 ->
  attacks T3/T4/T5), vs27 (attacks once Gemhide unsick, then Heart T7). With haste, a freshly-cast
  creature is a legal attacker at the moment the engine checks, and the step fires.
- **Reliably ISSUED when the model explicitly passes the main phase** — vs102 seq22 chose "Cast
  nothing right now" -> seq23 the attack step was offered.
- **SKIPPED when the model casts its LAST affordable spell (taps out / auto-ends the main phase) and
  all attackers are non-haste** — even with pre-existing unsick eligible attackers: vs49 (never chose
  "Cast nothing," no Heart all game -> 0 attack steps), vs14 T3/T5/T7, vs62 T6/T8.
- **Anomalies** that keep this partial: vs27 T5 got an attack step after a cast with an unsick Gemhide
  and no haste; vs14 T9 attacked after casting two sick creatures. So the correlate is loose. The
  translog cannot show the engine's own eligibility/phase computation — the confound is unchanged from
  wave-19. Escalated as a CONFIRMED-repro, NOT root-caused. See notes.md #1 (now 3-wave convergent).

## HEADLINE 4 — FC1 chump-durdle: STILL DEAD (3rd consecutive validation).

No game shows the model CHOOSING to turtle, flip to full defense, or chump its swarm away. Every own-
turn the engine offered an attack step, the model attacked with everything eligible (vs102 5 alpha
strikes; vs14/vs62 full-board swings). Blocking was correctly SELECTIVE: **"no blockers"** in every
healthy-life spot (vs49 seq5/9/12, vs135 seq5, vs62 seq8/12/17), and it took the **one FREE profitable
block** offered — vs14 seq22, `Striking Sliver blocks Lord of Atlantis`, annotation "(you kill it,
your blocker lives)" (first-strike + double-strike 2/2 kills the 2/2, takes nothing). That is the
guide's block-exception (b) fired exactly right, not a chump. **Do not trim the anti-durdle spine.**

## HEADLINE 5 — ZERO-FALLBACK ATTRIBUTION: revision earned part of it; the rest is a FRAGILE parse.

Wave-19 had 2 fallbacks, BOTH land-drop spirals under blue-mana anxiety (vs135 seq3 14,958-char
unparsed; seq6 stale_echo). This wave: 0. Attribution:

1. **The land-drop-discipline revision KILLED the exact wave-19 seam.** vs135's land drops this wave
   are all **390-544 chars, committed** (seq2 390, seq3 438, seq6 544, seq11 429, seq14 494) — the
   pure-oscillation, never-emit-a-CHOICE failure mode is GONE. That is a directly validated revision.
2. **But the long-ramble tax did NOT leave — it persists to 14k chars and RELOCATED** (as my wave-19
   S2 predicted): the longest replies this wave are **vs14 seq19 (14,078 chars, a "Play Island" land
   drop) and vs62 seq7 (12,809 chars, "Cast Striking")**. These are **decode-repetition spirals** —
   the tail degenerates into short repeated lines ("So I should play the Island now. And cast Striking
   Sliver. But wait...") — that happened to still terminate in a parseable CHOICE this corpus.
3. So the zero is **partly earned (revision) and partly FRAGILE**: a 14k repetition-loop is one
   unlucky decode from truncating unparsed. It is NOT that rambling stopped. Combat options_text /
   salvage did NOT contribute (no combat fallbacks existed at my seat to salvage). Route the residual
   to a **decode-time repetition-penalty / max-token guard**, NOT the guide (per wave-19 skill.md
   bucket-by-mechanism: repetition != reasoning-spiral, and a guide off-case cannot reach a decode loop
   — the model even quotes correct guide lines mid-loop).

## Record 3/6 flat — losses decompose to ENGINE + RACE, none to model durdle.

- **vs49 LOSS = ENGINE** (0-attacker non-issuance; Headline 2). The result-determining game.
- **vs27 LOSS = RACE** (attacked T5/T7/T9 every offered turn, Heart T7; lost the damage race to
  Zombies, my 20->0 vs opp 20->11). Variance/matchup, not a durdle.
- **vs135 LOSS = RACE** (Heart T3 -> attacked T3/T4/T5; alpha-struck opp 18->4 at seq16 but was already
  at 3 life and died -8). A base-1/1 swarm losing a coin-flip race to a faster deck. Not guide-fixable.

## What went RIGHT (do not re-teach)
- Attack-every-turn is the default wherever an attack step is offered (validated 3rd wave).
- Multi-anthem [X/Y] buff math off the rendered numbers stays clean (vs62 4/2-flyer team lethal read;
  vs14 double-strike stack; vs135 14-power alpha read).
- Block quality remains GOOD and cheap thanks to the trade annotations (vs14 seq22 free profitable
  block; selective no-blocks everywhere else).
- Heart Sliver led/curved when drawn and — a NEW observation this wave — its haste also DODGES the
  non-issuance bug, so Heart-present games are the ones that attack reliably. Construction signal.
