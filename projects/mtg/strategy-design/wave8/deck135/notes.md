# Deck-135 wave-8 — development notes (engine / harness / model / construction)

Run: PRIMARY control arm `matchups-20260714-044131`. deck135 jsonl epochs + opponents in
findings.md game->file map. Doctrine: win column context-only; items below are decision-level or
platform-level with seq repros.

## ENGINE LEDGER

- **Fetch re-offer tax — REDUCED (item 4 fired) but NOT eliminated; keep on the ledger.**
  Decline-suppression (brief item 4) fired: held-fetch decline windows fell from wave-7's ~62/game
  avg to ~27/game (per game this wave: vs140 54, vs131 38, vs44 22, vs109 19, vs110 2; vs133 wedge
  excluded). No strategy damage — nothing was permanently withdrawn (the crack re-appears each turn
  as the board state changes, which is exactly why the residual 22-54 persists), and no
  declined-twice-then-needed pattern exists (the vs44 non-cracking was piloting paralysis, not
  suppression — see model note). Residual fix (unchanged from wave-7): once a crack activation is
  CHOSEN, consume/withdraw it; and do not re-list a held fetch's crack every priority window (offer
  at most once per main-phase window / suppress on opponent turns while tapped out). Detect via the
  verbatim recurring `Put in Play with <fetch> targeting <land>` / `search basic land with <fetch>
  targeting <land>` strings across consecutive seqs. Still the #1 engine contributor to this deck's
  timeout decision counts (vs140: 54 of ~84 priority windows were held-fetch re-offers).

- **135v133 turn-2 Astrolabe livelock — CONFIRMED present in this corpus, FIX built post-corpus,
  EXCLUDED from play-quality (brief item 7).** Seat file 1784024870 is 16 KB, ends at turn 2,
  20/20 — the Arcum's Astrolabe never-castable-normal-mode no-op + cached-ask replay. Nothing to do
  here beyond confirming it is the known wedge; do not read it as a deck135 play failure.

## MODEL / REPLY-PROTOCOL (route to model-experiments / reply-protocol layer)

- **Answer-before-reasoning intent-collapse (vs131 s48).** Reply head `4` = Cast nothing, PLAN
  reasons in full to "I must cast a creature first... I will choose Option 2 as the standard cast"
  (cast Icehide Golem). The head-first parser committed `4` before the PLAN reasoning completed.
  Self-corrected at s51 (same turn, MP2) — ~1 phase tempo cost. Single instance. Candidate
  reply-protocol A/B (symmetric-core, judge by decision-quality not win-rate): accept the LAST bare
  number in the reply, OR move CHOICE after the PLAN, OR reconcile a trailing named-option index
  against the head. Do NOT change the protocol on one datum — arm a metric (recorded choice ==
  reply's own concluded option?) and watch across waves. Detail in skill.md P3 / general-suggestions.

- **Development paralysis when no creature is castable (vs44 s15/s18/s23/s27) — ADDRESSED IN GUIDE
  this wave, tracked here for validation.** The pilot Cast nothing on 4 own-turn windows while
  `Into the North {1}{g}` (its blue fix) and `Glacial Revelation {2}{g}` were castable, "holding
  mana for a creature" it could not cast. Root: Rule #1's else-clause was a permission, overridden
  by a self-generated "don't waste mana on non-threats" default. Guide fix = Rule #1 point 3
  (imperative develop-branch, scoped to "no creature in cast list"). If the manabase is later
  fixed OR the model reliably develops when creature-less, the point-3 imperative can compress.

- No on-board hallucination (phantom-lethal class did not recur; board-read anchor held). No
  desyncs (0/6). No numeric-offset menu traps. No degenerate blocker replies. No `defer`/`fallback`
  records in any deck135 seat.

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- **Manabase strands the deck's own 2-color win creatures — STANDS, sharpened.** Ice-Fang Coatl
  {G}{U}, Ohran Viper {1}{G}{G}, Abominable Treefolk {2}{G}{U} are gold/double-pip off a
  green-heavy, blue-thin base (blue = 2 Snow-Covered Island + Island-capable fetches + Astrolabe).
  vs109 (opp 12->wait opp 20): 1 creature offered all game. vs110 (opp 17): 2 creatures offered.
  vs131 (opp 18): Treefolk planned all game, never castable (short generic+blue). **Sharpening
  (new this wave): the deck can fix its own blue via fetches / Into the North, and the pilot
  UNDERUSES that fixing (vs44) — so part of the wave-7 "strand" is piloting, now addressed in the
  guide. The construction ceiling is still real; direction (user's call): raise effective blue
  (more Islands / Island-capable fetches / lean on Astrolabe) or shave a pip off the creature
  suite.** Not guide-length-fixable beyond the develop-branch.

- **Slow clock is structural.** Bodies are 1/1-2/2; Abominable Treefolk is the only closer and
  lands T16+ if at all. Feeds every timeout (vs140 to T28). Construction (threat size/count), not
  guide text.

## STOPGAP / DEFUSAL TRIPLES (line, defect + citation, removal condition)

1. RULE #1 board-read anchor ("if a creature is not printed on your battlefield line it is NOT in
   play") — patches on-board hallucination (wave-6 vs131 s52-60). Removal: phantom-lethal class
   resolved model-side, 0 across multiple waves with adequate precondition count. KEEP (held; 0
   recurrences this wave, still modest precondition count).
2. RULE #1 point 3 develop-branch ("no creature in your list? DEVELOP — Into the North for the
   Island, then dig; Cast nothing / hold mana is wrong") — patches development paralysis (wave-8
   vs44 s15/s18/s23/s27). Removal: manabase fixed OR the model reliably develops when creature-less
   across waves. NEW this wave.
3. RULE #1 / KEY-CARD uncastable-Treefolk anchor ("if Treefolk is NOT listed you can't pay yet;
   deploy what IS listed, else fix toward blue") — patches vs44-style stale-plan-around-uncastable
   (manabase strand). Removal: manabase fixed OR the model reliably reconciles a stale plan against
   the legal list. KEEP; extended this wave to route to the develop-branch when no creature is
   listed.
4. Force face-burn redirect ("NEVER Force a burn spell at your own face") — patches the wave-7
   vs110 s45 over-fire. Removal: Force face-burn over-fire stays 0 across waves. KEEP (0 this wave).
5. Fetch re-offer guide line ("don't re-pick a fetch-crack you already took") — INTERIM stopgap for
   the engine re-offer defect. Removal: when the engine consumes/withdraws the crack activation
   (engine ledger item #1). KEEP; clarified this wave that a REAPPEARING crack line does not mean
   you cracked it (you only cracked it if a land actually entered) — the wave-7 wording risked the
   pilot reading a never-cracked re-offer as "already done" (a live interaction with the vs44
   paralysis, though vs44 was primarily the mana-holding instinct).

## FREEZE-CHECK (OBEYED-BUT-LOSING / turnaround-preservation mode)

Revised as a constrained diff off the live deployed guide. Frozen lines and where they survived:
- **RULE #1 block** — FROZEN verbatim through point 1 and the board-read anchor. Point 2 split
  (creature-cast case) + NEW point 3 (develop-when-creatureless imperative, scoped so it cannot
  override point 1). The "develop next turn is the losing thought" and board-read anchor sentences
  are verbatim.
- **RULE #2 block** — FROZEN verbatim (attack seam healthy; Boreal-Druid carve-out held).
- **WHO IS THE BEATDOWN** — FROZEN; added one clause routing the defender case to the develop-branch
  when no creature is castable.
- **MANA / MULLIGAN / LETHAL CHECK** — FROZEN; MANA gained one clause reinforcing "get the first
  blue source down early / Into the North for the Island IS your play when creature-less," and the
  re-pick line clarified per stopgap #5. MULLIGAN/LETHAL verbatim.
- **Force of Negation KEY-CARD line** — FROZEN board-wipe scoping + face-burn redirect; added
  "Aetherize (mass-bounce that clears your board)" to the sanctioned-target list (observed correct
  fire vs131 s34). Obeyed core verbatim.
- **KEY CARDS + DECIDING SITUATIONS** — kept all prior lines; added the develop-when-creatureless
  situation and the Aetherize clause; extended the uncastable-Treefolk situation to name the
  fix-toward-blue fallback. All positive situation->play form.
No frozen line was dropped; new text targets only the develop-when-creatureless leak and the two
clarifications (Aetherize target, re-pick wording).

## VALIDATION TODO (before this guide is credited)
Re-run self-play with this guide and confirm: (a) development-paralysis windows (own turn, Cast
nothing, no creature but fixing/dig castable, non-redundant) drop toward 0 — especially Into the
North taken when short blue with no creature; (b) Rule #1 conflict rate holds (no creature passed
for value); (c) Force face-burn over-fire stays 0; (d) no phantom-board recurrence. The record is
construction-bound — do NOT read a flat/low win-rate as a guide regression until the manabase flag
is addressed. Reply-protocol A/B (vs131 s48 class) is a SEPARATE core experiment, not gated on
this guide.
