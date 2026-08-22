# deck105 (Infected Wounds — BG Infect) — Wave-35 guide-review findings

**Rubric**: owner's verbatim ruling — win rate irrelevant; adherence, and on divergence whether
departing was correct at that moment. **Arm weighting per brief**: arm C (FP8, the standing
opponent) is the pilot the guide teaches; arm B marks under-specification; arm A is background
(quant-damaged, never tuned to). Pre-digested evidence: `wave35-review/deck105-tier-review.md`
(arms A+B) and `deck105-fp8-review.md` (arm C) — cited, not re-derived; load-bearing seqs
re-verified against the translogs this pass.

## 1. The numeric ≤13→BLOCK rule: KEPT VERBATIM, now explicitly fenced

The rule is the seat's proven core and this review's protected asset:
- **Arm C: 15/15 exact** across all six games (fp8-review, full-walk section; e.g. no-block at
  LIFE AFTER 17/14 boundaries — 116 seq16/seq21, 36 seq5/10/15, 152 seq5; every LETHAL/<=13
  blocked — 146 seq13, 152 seq19/26/32).
- **Arm B: zero divergences in 118 decisions**; its two hardest calls were solved by literally
  executing the guide's arithmetic (158 seq5 t4: 17-3=14 -> none; 158 seq13 t8: LETHAL -> block,
  survived at 2).
- The standout, re-verified in the translog this pass: **C vs158 seq11 t8** — LIFE AFTER 1 vs a
  counters-grown 8/8 Orc army; trace: *"'NOT lethal…taking damage while ahead' — This is just the
  system prompt's assessment. I will follow the strategy guide's explicit rule: 'YOUR LIFE AFTER
  is 1 -> BLOCK'."* Blocked with Corpse Cur, survived at 1. The FP8 pilot recognized and refused
  the generic-hint seam on its own.

**Revision**: not one word of steps 1–4 changed. Added one scoping sentence naming the generic
suffix (*"block only where the trade favors you; taking damage while ahead is often correct"*) as
the general prompt's advice that this deck's steps REPLACE — turning what the FP8 pilot worked out
for itself at 158 seq11 into stated text, and closing the license the 4.75-bit arm took from that
suffix twice (A-36 seq13, A-139 seq12; tier-review docket item 1). Render-side fix flagged for
core in `general-suggestions.md`; my text is scoped to this deck only.

## 2. "Believe the printed outcome line" — the Ichorclaw hardening (seat-focus item)

The Class-1 misread recurred in arm C exactly as the fp8 review recorded — re-verified in the
translog: **C vs152 seq26 t14** (LETHAL, life 2), reasoning claims *"If B1 blocks A1: B1 becomes
3/3 … kills A1"* and even *"'becomes blocked' triggers in the declare blockers step"*, while the
printed B-line said verbatim **"may block A1 (your blocker dies, attacker lives (infect shrinks it
to 2/2))"**. Decision-neutral this time only because the block was mandated and every legal
assignment gave the same face damage. Same wrong belief as A-152 seq9 (there it cost a body);
never once in the 122B arm -> **tier-intrinsic rules-prior, prompt-resistant but belief-persistent**
(fp8-review verdict section). Expect recurrence at any non-mandatory Ichorclaw block.

**Guide-side hardening shipped, two layers:**
1. **The card note now explains the trigger word instead of only asserting the result**: "becomes
   blocked" is a thing that happens TO AN ATTACKER; when Ichorclaw is the blocker, nothing has
   become blocked, no trigger, it fights as a 1/1. The old note stated the truth ("It gets nothing
   for blocking") and both 35B quants overrode it — an assertion loses to a prior; a mechanism
   explanation gives the prior nothing to argue with.
2. **A printed-outcome-line trust rule in the block section**, anchored per HL2 to the render
   substrings it follows: the parenthesis on every "may block" line ("both die" / "your blocker
   dies, attacker lives" / "you kill it, your blocker lives") is the engine's computed outcome of
   that pairing; when your own combat math disagrees with it, your math has a wrong term. Worked
   example re-derived from the rule in the rule's vocabulary (HL2 pre-ship check): the seq26 line
   comes out right under the rule's binding. Scoped to B-lines only — the surface this seat
   actually sees (I did not verify attacker-side forecasts include triggers, so I claim nothing
   about them).

The truly targeted fix remains render-lane: annotate at the option ("as a BLOCKER it stays 1/1" on
Ichorclaw's B-line) — the pattern that went 5/5 and 4/4 on Relic Putrescence's "only legal targets
are YOUR OWN" annotation (positive control, both 35B arms). Routed to `notes.md` for the wave-36
docket per the fp8 review's recommendation.

## 3. Justified audibles the guide previously forbade -> narrow named exceptions (HL9.4 form)

Owner rubric rewards good divergence; a guide that forbids the correct play is a guide bug
(deck139 precedent, this wave's brief). Two arm-C audibles adjudicated JUSTIFIED got the play
legalized as a **default + named trigger**, never a cost/benefit description:
- **C vs152 seq30 t15**: held Cystbearer home at 1 life (*"Cystbearer attacking would kill me"*)
  against "attack with every creature". Correct — survived the turn. New named exception: at 5
  life or less, if their untapped creatures can kill you back, hold back the smallest set of
  bodies the block rule will need; attack with the rest. Default unchanged: every creature listed.
- **C vs116 seq25 t12** (cast nothing, lethal alpha on board: *"Poison 6/10. Need 4. Attackers
  4+1+1=6. Wins."* — won that turn) and **C vs139 seq17 t11** (deferred a cast past combat at
  opponent 9 poison, lost nothing). New clause: when the printed poison number plus this turn's
  attack power reaches 10, the attack IS the turn — casting first is the default, not a gate.

Arm A's two audibles (A-139 seq12 trample-chump economics; A-36 seq13 race logic) are consistent
with these same two exceptions but were NOT tuned to — both edits are justified by arm B/C
evidence alone per the brief's arm weighting.

## 4. Poison-line render (zero-state fix) — VERIFIED SERVING THE ARITHMETIC

Measured across all six arm-C deck105 seat files this pass: the poison line renders at every
state including zero once poison is live — "Poison counters (you): 0 of 10" **73 renders**,
"(opponent): 0 of 10" 48, plus the full climb (1, 3, 4, 6, 9 of 10) with the "N more end it"
tail. Zero-state present in **6/6 games**. "Poison counters (me)" appears **0 times in prompts**
(10 times in reasoning — model paraphrase, not a render defect). The guide's arithmetic rode it:
C-116 seq25 read "6/10, need 4, attackers 6 — wins" and won t12 with the opponent at **28 life**,
never reconciling poison against life. One guide correction fell out (HL3.2 falsity-fact sweep):
the old clause *"and, once you have taken any, the same line for you"* is now false — your line
renders at 0 of 10 too. Corrected to match the verified render.

## 5. Everything else: audited, KEPT

Standing loop (land / biggest casts, Putrefax cast+attack same turn C-139 seq15–16 / full
attacks 18/19 in C), card rules (Necropede at theirs 3/3 in C; Corpse Cur biggest return 158
seq9–10; Ichor Rats cast despite self-poison; Relic Putrescence declines 4/4 on the annotation),
mulligan section (6/6 C keeps guide-legal, 12/12 across A+B), Skithiryx/Putrefax never-block,
multi-block damage assignment, all SITUATIONS bullets — no divergence traced to any of them; per
HL3's gate none was deleted or compressed (no carrying surface exists for the card facts;
load-bearing, stays). The arm-A mana-recount failure class (A-158 seq14) did **not** recur under
FP8 (0/134) — the guide's "do not recount your lands" guardrail stands unchanged; class assessed
quant-damage, not guide-lane (fp8-review, Class 2).

## Verdict

FP8 seat adherence 133/134 (~99%), zero decision-corrupting divergences; the guide's numeric core
is validated at all three tiers. This revision protects the proven text, converts the two
correct-but-forbidden plays into named exceptions, states the Ichorclaw blocker-side truth as
mechanism + render-trust rather than bare assertion, and scopes the generic-hint collision.
Rotation-relevant: this seat's guide is CONVERGING — the remaining live risk (Ichorclaw
rules-prior) is render-lane, not guide-lane; one clean FP8 corpus on this text with the B-line
annotation landed would argue for freeze.
