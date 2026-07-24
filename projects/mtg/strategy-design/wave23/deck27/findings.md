# deck27 (Zombies UB tribal) — wave-23 findings

**Seat:** guided round 4. Binary /tmp/wagic-0e6361732. Corpus matchups-20260724-013710, 6 games
round-robin. **Record 4/6 (up from 1/6 across another pool swap).** Guide byte-frozen (= the wave-22
free-kills-first revision, deployed last cycle; deployed == wave22 strategy.txt, 7497 bytes).
**136 decisions across 6 games, 3 fallbacks (all at vs137, all unparsed_reply decode-collapse).**

## Record — 4/6 (W vs109/133/131/137-adj; L vs102, vs140-adj)

| vs  | new? | result | life (me/opp) | turn | shape |
|-----|------|--------|---------------|------|-------|
| 109 | cont | **WIN**       | 18 / -1  | 8  | Dominant evasion win vs mono-R aggro — Inkfathom unblockable clock + lord-buffed team, opp to -1 by t8 |
| 133 | NEW  | **WIN**       | 20 / -2  | 12 | Life UNTOUCHED (20). Flew over/under with Glen Elendra + evasion; clean |
| 131 | NEW  | **WIN**       | 20 / 0   | 14 | Life UNTOUCHED (20). Ground the Pyromancer/Guttersnipe deck out with tribal beatdown |
| 137 | NEW  | **WIN (adj)** | 14 / 1   | 9  | Ahead 19/1 by t9 vs GW Adventures; **3 consecutive decode-collapse fallbacks s12-14** but heuristic held the ahead board; won cap-adjudication |
| 102 | cont | **LOSS**      | 0 / 11   | 13 | Tourach deck ground it out — the un-interactable-threat archetype flagged since wave-21. Variance/matchup |
| 140 | cont | **LOSS (adj)**| 6 / 7    | 27 | Lifegain-control again — but MUCH closer than wave-22 (6/7 vs 17/28). Lost adjudication by ONE life at t27 |

**4/6 read (decisions-based, per brief — cross-wave record is WEAK, pool changed again):** three of the
four wins are clean, decisive, life-barely-touched aggro wins (two at 20 life untouched). The wins were
EARNED on the deck's win-path (evasion + lords), not gifted. The two losses are its two known worst
archetypes — Tourach (un-interactable) and lifegain-control — the SAME structural gap flagged since
wave-21/22. Notably the lifegain-control game (vs140) closed from a wave-22 blowout (17/28) to a
one-life adjudication (6/7): the clock is closer, still short. This is the favorable tail of the same
high-variance, no-B-plan surface — do NOT read 1/6->4/6 as the free-kills-first revision "working";
attribute per game. The revision touched exactly ONE decision this corpus (see focus 1).

---

## FOCUS 1 — free-kills-first block rule: FIRED CORRECTLY at the snap layer; no 2nd commit-lock

The wave-22 revision front-loaded a "WHEN SEVERAL ATTACKERS COME AT ONCE ... ALLOCATE by FREE KILLS
FIRST ... never chump with a lord" allocation rule, aimed at the wave-22 vs109 s15 misplay (a lord
chumped into an unkillable 3/3 while two free Goblin kills were available, frozen by truncation).

**Eight blocker decisions this corpus (7 model-decided + 1 heuristic-covered collapse). Every model
snap line was a clean, correct coded FIRST line:**

- **vs109 s13 (t7, 20/9) — THE multi-attacker case, near-identical to the wave-22 misplay shape.**
  Attackers A1 Goblin 1/1, A2 Goblin 1/1, A3 Rakdos Cackler 2/2; ONE blocker = Glen Elendra Liege (2/3,
  a LORD). Snap line = **`BLOCKS: B1:A3`** — blocks the biggest KILLABLE attacker (Cackler 2/2: Liege
  deals 3 -> kills; takes 2 -> **lord SURVIVES** at 2/3), takes 2 from the goblins (->18, not lethal at
  20). This is the exact scenario that misplayed in wave-22 — and it resolved CORRECTLY. The
  distinction the rule needed the model to draw (block the biggest attacker you can kill-and-survive;
  do NOT chump the lord into one you can't kill) was drawn on the first line. The wave-22 misplay had
  the big attacker UNKILLABLE (3/3 Hellrider vs a 2/2); here it was killable (2/2 vs a 2/3 lord), so
  blocking it is right, not a chump. **The behavior the rule targets is now correct.**
- **vs109 s9 (t5, 20/17):** single attacker Chainwalker 3/2; blockers Oona's Gatewarden (wall) and Lord
  of Atlantis (lord). Snap = **`BLOCKS: B1:A1`** — used the WALL (Oona, wither kills the 3/2), explicitly
  reasoned it would NOT trade the lord. Correct free-kills-first + protect-the-lord behavior.
- **vs102 s7/s10, vs133 s10, vs131 s8/s12:** all single-attacker; snap lines all clean and correct
  (favorable trades / free kills taken; no lord-chumps). vs131 s12 traded Lord of the Undead in a
  "both die" for a Guttersnipe (ping engine) — mildly debatable (keeping the lord + racing was an
  option) but it is a fair "both die" trade removing a burn engine, NOT a chump, and the guide
  sanctions "both die" trades; not a misplay of the allocation class.

**Caveat on strength of validation:** only ONE true multi-attacker allocation arose (vs109 s13), so the
positive validation is thin (N=1 for the exact scenario). But it is the RIGHT sign, on the exact shape,
and every single-attacker snap was also clean.

**Commit-lock watch (any 2nd HARMFUL instance? promotion threshold):** **NONE.** No case this corpus of
a snap misplay committed on line 1 then frozen by truncation before self-correction. The commit-lock
stays at 1 harmful instance (wave-22, single-seat) — the >=2-seat promotion threshold is NOT met by
deck27 this corpus. The 3 vs137 unparsed fallbacks are NOT commit-locks (see focus 2): they emit NO
coded line at all, so there is nothing "committed" to freeze.

---

## FOCUS 2 — the 3 fallbacks: DECODE-COLLAPSE unparsed_reply (decode-owner, NOT parser-owner)

All 3 fallbacks are at vs137, consecutive (seq 12->13->14), turns 8-9, while deck27 was decisively AHEAD
(19/10 -> 19/1). All classified `unparsed_reply`, latencies 122.7s / 98.7s / 82.2s (at/over the HTTP
timeout — the reply ran the full generation window as garbage).

**Exact reply shapes (parser-owner classification per brief):**
- **s12 (ask, 122714ms):** a trivial 2-option casting decision (`1. Cast Zombie Outlander / 2. Cast
  nothing`) on a simple ahead board. Reply = `-` then endless `**` / blank lines (10,814 chars). NO
  `CHOICE:` line anywhere.
- **s13 (attackers, 98693ms):** reply = `>` then endless `**` and `(U+FFFD)**` (replacement bytes =
  broken UTF-8 mid-collapse), 7,784 chars, **1354x verbatim-line repetition**. NO `ATTACK:` line.
- **s14 (blockers, 82229ms):** reply = triple-backtick `### 1.` ... endless `### 0.` then endless `C.`,
  6,470 chars. NO `BLOCKS:` line.

**What did the parser see, why did no rule recover it:** the parser saw pure degenerate token-repetition
with ZERO coded lines — no in-range CHOICE index, no ATTACK/BLOCKS token, no echoed label, no sibling
CHOICE line. The Echo INDEX-WINS recovery (item c) operates on a **well-formed in-range choice index**;
absent-card bookend recovery needs a **clean sibling CHOICE line**. Neither exists in a token-collapse —
there is literally nothing to recover. **The parser is CORRECT to classify these unparsed_reply; this is
NOT a parser gap.** This is a **MODEL / DECODE-layer** failure (repetition collapse to the token cap).
Routing them to a parser change would be a misroute — there is no coded content for any parser rule to
grab. (Routed to notes.md as decode-owner, cross-seat/pilot watch.)

**Board impact (per brief, each unparsed owner notes board impact only):** NEGLIGIBLE. deck27 was
ahead 19/10 when the collapse began; the heuristic (Baka) answered all 3 (cast/attack/block), the opp
fell 10->1, and deck27 won the cap-adjudication 14/1. The collapse cost nothing this game — but only
because it happened while already winning and the heuristic could coast. On a contested board a
3-decision blackout could lose a game.

**Why this is the AHEAD-DEGENERATION watch recurring — HARDER and HARMFULLY:** the prior ahead-loop
watch (3 instances: wave-19 d110 s20, wave-20 d27 vs49 s16, wave-21 d27 vs135 s18) was verbatim-LINE
repetition of a reasoning sentence, always truncation-SAVED by answer-first (a coded line 1 survived).
This is the same trigger (decisively ahead, trivial/won decision) but a WORSE manifestation:
token-level collapse that emits NO coded line, so answer-first CANNOT save it -> unparsed fallback. The
watch is now HARMFUL (produced fallbacks) and CLUSTERED (3 consecutive), and it is deck27's FIRST-EVER
fallback cluster (0 fallbacks in wave-21 and wave-22, 135 decisions each). See focus 4 for the pilot tie.

---

## FOCUS 3 — wither watch (round 3): CONFIRMED, zero false positives, stays CLOSED

All wither annotations rendered this corpus, verified against real math:
- **vs102 s7 & s10:** Oona 2/1 wither vs Tourach 4/3 -> "(your blocker dies, attacker lives (wither
  shrinks it to 2/1))". CORRECT: Oona power 2 -> 2 wither counters -> Tourach 4/3 - 2/2 = **2/1**; Oona
  (1 toughness) dies to Tourach's 4.
- **vs109 s9:** Oona 2/1 wither vs Gore-House Chainwalker 3/2 -> "(both die)". CORRECT: 2 wither -> 3/2 -
  2/2 = **1/0 -> dies**; Oona takes 3 -> dies. "both die" is exactly right.

Zero false positives, third clean corpus. NOTE: at vs102 s7 the model's own prose miscounted ("four
-1/-1 counters" — Oona power is 2, not 4), but the ANNOTATION was correct (2/1) and the CHOICE was
correct (wall Tourach with the defender). The internal miscount did not change the decision. Item CLOSED.

---

## FOCUS 4 — pilot verdict (e): repetition_penalty=1.05 — WORSE at this seat for decode stability

Per-seat pilot line: **WORSE (decode stability) / SAME (clean-reply prose).**

Evidence:
- **Clean-reply prose: SAME.** Across the 5 non-collapsed games, block/attack/cast reasoning is coherent
  and correct; reply-char p50 839-1001, max clean 7322 (vs102 s7). No prose degradation, no structured-
  output corruption on clean replies. Corpus-wide spirals were reported FLAT; this seat matches.
- **Decode stability: WORSE, and specifically at the failure mode the pilot was theorized to FIX.** This
  seat went from **0 fallbacks (wave-21, wave-22) to 3** — all decode-collapses, all in one game, all
  while AHEAD. The ahead-degeneration watch's proposed "cheap fix" was *repetition_penalty*. This is the
  FIRST corpus with rep_penalty=1.05 active, and the ahead-degeneration **recurred WITH the penalty on,
  and harder** (token collapse -> unparsed, vs prior benign verbatim-line loops). The 1354x verbatim-line
  collapse at s13 is the single most extreme repetition event ever seen at this seat. Whatever else the
  penalty did corpus-wide, at 1.05 it did **not** prevent — and coincided with — this seat's worst-ever
  decode event.

**Honest hedges:** N is small (3 fallbacks, one game, a NEW opponent — GW Adventures/vs137 never seen
before), so variance and opponent-novelty are live confounds; correlation != causation. But the direction
is clear enough to state: at deck27's seat the pilot did not help the known failure and the seat's
only-ever fallback cluster appeared under it. **The durable takeaway for synthesis: rep_penalty=1.05 is
INSUFFICIENT for the ahead-degeneration collapse — the "cheap fix" hypothesis is not validated at this
strength.**

---

## Durdle late-stall revision (focus): STILL UNEXERCISED — 4th corpus, trigger did not fire

vs140 (t27 grind, the best grind window again) had deck27 **attacking every turn it had an attacker**
(s16 t14, s19 t16, s24 t24, s27 t26 — always ATTACK A1,A2[,A3]), NEVER holding buffed attackers back.
The revision's TRIGGER (walled ground stall + holding eligible attackers) again did not occur; deck27
followed the *prescription* (swing every turn while grinding) and lost the adjudication by ONE life
(6/7). This is a construction clock gap, not a durdle — consistent with wave-22's read. The off-case is
now 4 corpora unexercised; per the skill's own note, hold and re-flag, do not cut for lack of validation.
No guide edit on this axis.
