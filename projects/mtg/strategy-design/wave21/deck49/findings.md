# deck49 (Dragons) — wave-21 findings (WAVE-20 EDITS VALIDATED; develop-seam oscillation DEAD; record fell on draw/matchup)

Mono-red Dragons aggro, FOURTH guided corpus, first validation of the wave-20 two-hunk revision
(deployed guide md5 479f907edbbc57e76c02bfaab8ba12d9 — key-blocker exception tightened + develop-seam
"either is fine, COMMIT" tiebreak). 6 games, **3/6** (W vs deck135, deck14, deck35; L vs deck27,
deck102, deck62). Binary 3be7f5a67 (wave-21 engine batch: answer-first reply protocol, parser
hardening incl. retracted_choice, prose salvage, etc.). Decisions: ask 99, attackers 16, blockers 9,
gamestart 6, gameend 6. **1 fallback (stale_echo, deck35 seq18) — a HARNESS false-positive on a valid
answer-first reply, zero harm (below).** 0 unparsed, 0 retracted, 0 prose-salvage at this seat.

Step-0-ter posture (rule 1): judge by taught-class survival + tax, not the record. The record fell
4/6->3/6 on a REPEATED pool, entirely to draw/matchup variance — deck49's guide behavior was correct
in every one of the 6 games. Decision quality IMPROVED this corpus (0 develop-seam spirals, 0
unparses vs wave-20's 3). **The record dropped DESPITE better play.**

## HEADLINE — the wave-20 develop-seam edits WORKED; the oscillation SHAPE is genuinely gone there

The wave-20 streak broke on 3 unparsed replies, ALL at the early Main-1 develop seam (seq8, t4-5,
lone-1/1 board, deploy-creature-vs-burn-face fork with no tiebreak), 5000-9000 chars of loop/spiral/
collapse. This corpus the SAME fork type appeared in ALL SIX games and every one committed cleanly:

| game | seq | turn | opp | menu had creature+burn | choice | reply len |
|------|-----|------|-----|------------------------|--------|-----------|
| deck27  | 3 | 0 | 20 | Dragonmaster + Lava/Bolt | Cast Dragonmaster | 905 |
| deck135 | 6 | 3 | 17 | Bloodmark + burn | Cast Bloodmark | 898 |
| deck14  | 3 | 0 | 20 | Dragonmaster + burn | Cast Dragonmaster | 961 |
| deck102 | 6 | 2 | 17 | Bloodmark + burn | Cast Bloodmark | 719 |
| deck35  | 3 | 0 | 20 | Dragonmaster + burn | Cast Dragonmaster | 984 |
| deck62  | 7 | 3 | 17 | Bloodmark + burn | Cast Bloodmark | 1202 |

Every one chose DEPLOY (the guide's soft board-state lean: "if you have no threat on the board yet,
deploy the creature"), in 719-1202 chars — 6-9x SHORTER than the wave-20 spirals at this exact
seam. ZERO oscillation, ZERO "the guide says face / the guide says deploy" re-argument, ZERO
truncation. This is the answer the wave-20 exit question needed: **the guide edit DISSOLVED THE
TRIGGER — it did not merely get absorbed by the answer-first protocol.** The tell is reply LENGTH:
absorption would leave a long-but-parsed spiral; instead the replies are short clean commits. The fork
stopped competing.

### EXIT Q1 — develop-seam fallbacks dead? YES, emphatically. But distinguish two claims:
- **At the develop seam:** the oscillation shape is GONE (short commits, above). Guide edit fixed the
  trigger.
- **Elsewhere (combat/card-value/mana math):** the G-49a reasoning-tax spiral PERSISTS — but now
  PARSES via answer-first instead of truncating. Three big spirals this corpus, all with a coded first
  line: deck35 seq18 War-Paint value 5730c/83.8s; deck102 seq14 War-Paint value 9541c/135s; deck27
  seq22 Shivan-cast 12939c/185s + seq23 blocks 13042c/185s (near the 16.9k max). These are the
  protocol ABSORBING the shape. One of them tripped a false-positive fallback (below) — absorption is
  not fully free.

So: the guide edit genuinely killed the shape at the seam it was written for; the residual tax lives
in OTHER pockets and is now a parse-safe (but not cost-free) verbose spiral. This is the G-49a
relocation, exactly as wave-20 predicted ("the guide can lower the TRIGGER frequency; the loop
tendency itself is CORE/decode").

## The one fallback — deck35 seq18 stale_echo — a HARNESS FALSE-POSITIVE, not a model error

Classification for the exit question: **stale_echo (NOT retracted_choice).** The model emitted a
perfectly valid answer-first `CHOICE: 1 (Cast Goblin War Paint)` — legal option 1, fresh War-Paint-on-
Regent combat math — after an 83.8s / 5730-char deliberation. The reply is NOT a byte-repeat of seq17
(different md5). The harness nonetheless routed it to heuristic (choice=-1, fallback=stale_echo).
**The model did NOT disavow its choice; the routing discarded a good answer.** Zero harm: the heuristic
cast War Paint and seq19 targeted Thunderbreak Regent (exactly the model's stated intent), buffing it
to 6/6; the seq20 swing (Regent 6/6 + Whelp 2/2, both unblocked flyers) was lethal — deck49 WON that
turn (op 8 -> -1). So the fallback routing was benign only by luck (the heuristic happened to agree),
not because the model erred.

Likely trigger (hypothesis, route to harness owner to confirm): the seq18 reply's PLAN block re-states
seq17's PLAN nearly verbatim ("...cast Goblin War Paint on ... next turn I win"), and the stale_echo
guard fingerprints against the prior turn's reply/PLAN. If so, the residual G-49a tax spiral (re-
deriving the same plan) is what tripped the false-positive — a NEW way the tax costs a decision even
under answer-first. See notes.md HARNESS-49a.

## EXIT Q2 — burn-face rate held? key-blocker exception over-generalize? Held 13/13; over-gen UNDER-TESTED (no harm)

All 13 burn TARGET-choice decisions went to the OPPONENT'S FACE. Zero creature-targeted — cleaner than
wave-20's ~16/16. Creatures WERE on several menus and still declined:
- deck27 seq7/8: menu offered Oona's Gatewarden (1/3 wall) — chose face.
- deck62 seq9/10: menu offered Druid of the Cowl + Bloodmark — chose face.
- deck102 seq19/20: menu offered Ravenous Chupacabra (2/2) — chose face.
- deck135 seq8/9: menu offered Ice-Fang Coatl — chose face.
- deck14 seq9-12: menu offered Merfolk of the Pearl Trident (x2 bolts) — chose face both.
- Plus deck35 seq6/8, deck62 seq5/12, deck102 seq4, deck135 seq4 (face-only or face-chosen).

**The tightened exception did NOT over-generalize into visible harm** — but the corpus presented NO
must-kill-blocker spot (no "big attacker about to kill you", no "key wall stopping your main threat"
where the model wrongly went face). So "doesn't over-correct into never-kill-any-blocker" is validated
as HARMLESS but UNDER-TESTED: there was no case where killing a blocker was actually correct. Honest
read: burn-face discipline is rock-solid; the exception's narrow correctness is unexercised.

## EXIT Q3 — "either is fine, COMMIT" prevented blind creature-casts when burn was lethal? UNTESTED (no lethal-burn fork arose)

No ask this corpus had opp_life in burn range (<=3-ish) while deck49 held BOTH burn and a creature on
the menu. The lowest deck49-to-act opp life with a real choice was 8 (deck35, deck102) — and in both,
burn was already spent (deck35 closed via combat; deck102 had only War Paint in hand). So the OPPOSITE
failure mode the tiebreak could cause (a blind creature-cast when burn was the lethal play) never had a
chance to fire. No evidence of harm; simply not exercised. The tiebreak's OBSERVABLE benefit this
corpus was Q1 (dissolving the develop-seam oscillation), not a lethal-burn commit.

## The three losses — all draw/matchup/construction (G-49d family), NONE an edit side-effect

- **deck27 (L->L, me -2 / op 14, t13):** burn-LIGHT draw (only Lava seq5 + Bolt seq7 -> op stuck at 14
  from t2 to gameend), walled by Zombie blockers, and — Ashenmoor Liege lost to a card-script-drift
  block. seq15 block menu rendered "B1. Ashenmoor Liege (4/1) - may block A1 (both die)": the engine
  gives Liege toughness 1 (primitive mtg.txt power=4 toughness=1) but the REAL Oracle is a 4/4. With
  the true 4/4, blocking the 2/2 Zombie Outlander KILLS it and the Liege LIVES (keeps the anthem); the
  4/1 renders "(both die)", so deck49 traded its only anthem for a 2/2 at 18 life under no lethal
  pressure. The block is defensible GIVEN the (wrong) 4/1; the root is the card drift. Primary cause =
  construction (burn-light + walled); the Ashenmoor drift is a real game-material aggravator. See
  notes.md CARD-49-ashenmoor.
- **deck62 (W->L flip, me -9 / op 11, t10):** pure DRAW/MATCHUP variance. deck49 burned op 20->11 by t5
  (Bolt+Bolt+Lava, all face — correct off-case), deployed Bloodmark, then drew a Shivan Dragon (6-drop)
  + 3 Mountains and sat creatureless (board creatures:0) from t7, unable to cast the 6-drop until t9.
  The opponent assembled an explosive Enchantress engine (a 7/9 Yavimaya Enchantress + Druid 1/5 +
  Saproling swarm, trample everywhere) and trampled deck49 20->17->-9 in one cycle. deck49 played
  correctly; it drew the top-heavy flood (G-49d) into the worst matchup. (Minor tax wart: seq14 land-
  drop was a 5588c/95s spiral re-deriving Shivan's cost — harmless, correct land drop; G-49a residual.)
- **deck102 (W->L flip, me -3 / op 31, t21):** got op to 8 by t8 (burn + Bloodmark beats) but had NO
  reach to close the last 8; the opponent (lifegain/value) climbed op 8 -> 31 while deck49 top-decked
  bare Mountains late (seq21/24/25 hand = just "Mountain"). NOT a durdle (nothing castable) and no
  fumbled lethal (no burn in hand when op hit 8). Construction/no-reach vs a lifegain matchup (G-49d).

vs wave-20: deck14 flipped L->W (last wave lost its Thunderbreak to Persuasion theft; this wave curved
out clean — Dragonmaster + War-Paint-own + 2 Bolts face + Thunderbreak, won t10 at 18). deck62 and
deck102 flipped W->L on draw/matchup variance. All four record deltas across the two waves are pool
variance on a repeated control, not prompt effects — the deck49 construction ceiling (top-heavy
curve / no reach, G-49d) is the record's real governor, and it is a DECKLIST property outside every
guide/core surface.

## Taught-class audit (the real signal) — 4/6 -> 3/6 context, all classes HELD

- **BURN-GOES-FACE: 13/13 face, 0 creature-targeted** (see Q2). Perfect. Creatures declined on 5 menus.
- **War-Paint-own: 4/4 own, 0 opponent.** deck135 seq10/11 (-> own Bloodmark over Ice-Fang Coatl),
  deck14 seq5/6 (-> own Dragonmaster over Merfolk), deck102 seq16 (own-only annotation, cast safely),
  deck35 seq18/19 (-> own Thunderbreak). The own-only annotation ("the only legal targets are YOUR OWN
  right now") LIVE and used (deck102 seq10/16).
- **Crucible-needs-dragon: FIRED as a correct CAST-when-eligible.** deck135 seq18: cast Crucible of
  Fire t9 WITH Thunderbreak Regent already in play (dragon down first) — the rule's positive case, not
  just the decline case. Clean.
- **DEPLOY-AND-ATTACK: obeyed everywhere eligible.** Multi-attacker declares in all wins; the late
  "cast nothing"/"play mountain" turns in the losses were genuine empties (bare-land hands or lone
  War-Paint), not durdles.
- **Off-case (curve fails -> burn is your deck): re-validated on deck62 + partially deck102.** deck62
  was the pure off-case (burn-light flood) — every burn went face, none held. Behavior correct; loss =
  construction. Consistent with wave-20's deck27 off-case discharge.
- **Rule #1 mana-trivial: held; one contained re-derivation** (deck62 seq14 land-drop spiral, self-
  corrected, harmless).

## Bottom line
The wave-20 edits are VALIDATED: the develop-seam oscillation that broke the streak is GONE (6/6 clean
commits), burn-face discipline is perfect (13/13), and no over-correction harm appeared. The 4/6->3/6
drop is draw/matchup variance on a repeated pool around the standing G-49d construction ceiling — NOT
an edit side-effect; decision quality rose. The guide is CONVERGED and warrants a BYTE-COPY (no change
evidenced). Residual open work is all cross-layer and routed out: Ashenmoor Liege card-script drift
(now game-material -> card owner), the stale_echo false-positive (harness), and the persistent G-49a
tax spiral in non-develop pockets (core/decode). See notes.md for the rotation verdict.
