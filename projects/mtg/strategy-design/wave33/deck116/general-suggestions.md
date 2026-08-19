# deck116 -> core (general prompt): **PASS**, 28th consecutive.

No core-prompt proposal from this seat. The general prompt was not implicated in any of the six
games. Specifically checked and clean at this seat:

- The win-condition sentence (`AIPlayerGPT.cpp:63`, *"You win by reducing the opponent's life from
  20 to 0"*) that deck105 raised and SELF-HELD in wave-32: at deck116's seat it did NOT mislead the
  pilot in the poison game. With i1 rendering the count, vs105 s11 and s15 both open *"I am at 8
  poison counters and need to win immediately"* and s15 prices the race explicitly (*"You are losing
  the poison race (need 2 more, they need 9)"*). The conflation the clause was proposed against did
  not persist here. My seat's evidence therefore points at NOT adopting the widening — one seat,
  offered as a data point to whoever re-checks deck105's held G1, not as a counter-proposal.
- Answer format, the placeholder example, and the `0 = pass` convention: 131 decisions, 0 fallbacks,
  0 out-of-range answers, 0 example-echoes.
- The plan-staleness note fired correctly and was ignored once (routed as N-116h, render lane, not
  core).

---

# OWNER DIRECTIVE 1 — EVIDENCE ONLY (no reordering, no brevity clause, no terminator proposed)

Contributions in the three shapes the directive asks for.

## (a) Counted overrun / retraction fields — and a measurement gap that will bite the A/B

131 decisions. **4** replies omit the `PLAN:` line (compliant under `f46dd58ee`). **14** replies
(10.7%) carry `post_plan_overrun` > 0, totalling **14,025 chars**, max 3,353. **`commit_retracted`:
0. Fatal overruns: 0.** Every first-line answer stood and was executed. deck116 adds nothing to the
2-seat fatal-retraction promotion trigger.

**But the field is measuring the wrong boundary at this seat, by a factor of nineteen.** Text after
the committed answer line (`CHOICE:` / `BLOCKS:` / `ATTACK:`) totals **263,247 chars — mean 2,009
per reply**, which is the ~1,900 corpus-wide figure the directive cites. `post_plan_overrun` sees
**5.3%** of it, because the destructive pattern here is post-ANSWER and pre-PLAN, and because the
field is format-sensitive:

| decision | post-ANSWER chars | `post_plan_overrun` | why the field missed it |
|---|---|---|---|
| vs152 s3 (land drop, 209 s) | 14,206 | 262 | reply writes its plan line last and lowercase (`Plan:`) |
| vs139 s3 (land drop, 201 s) | 13,326 | **0** | no `PLAN:` line at all — legal under `f46dd58ee` |
| vs158 s7 (cast, 80 s) | 5,707 | **0** | PLAN line is last |
| vs146 s13 (blockers, 82 s) | 5,592 | **0** | no PLAN line |

The four replies that cost this seat the most are the four the counter scores at 0 or near it.
**Evidence-side consequence: the wave-34 A/B needs a `post_answer_overrun` field (chars after the
first committed answer line) or it will report a change it cannot see.** Recorded as an instrument
observation in `notes.md`, not as a protocol proposal.

## (b) Does the 4.2% post-answer self-correction carry decision VALUE that native pre-answer reasoning would have to replace?

**Rate at this seat: 5 / 109 multi-option decisions = 4.6%.** All five read in full:

| seq | committed | reasoning concluded | correct answer | value of the correction | outcome |
|---|---|---|---|---|---|
| vs158 s7 | Cast Fabricate (2nd Amulet, one already in hand) | Cast Time of Need | **Time of Need** | **HIGH — decisive** | discarded; game lost T8 |
| vs146 s13 | `B1:A1, B2:A2` (Elvish Piper blocks) | save the Piper | **save the Piper** | **HIGH** | discarded; Piper died |
| vs152 s3 | Play Hinterland Harbor (enters tapped) | Play Island | **Island** | LOW-MED (tempo) | discarded |
| vs152 s24 | Cast nothing | Cast Kitchen Finks | **Kitchen Finks** | MEDIUM | **honoured** — a second `CHOICE:` line was emitted and the parser took it |
| vs36 s14 | Piper -> Emrakul | Piper -> Thragtusk | **Emrakul** | **NEGATIVE** | the commit protected the better answer |

**Answer: yes, three of five corrections were right and two of those three were material — but the
value is only realised when the model happens to emit a second `CHOICE:` line, which one of five
did.** The mechanism that captures the value today is not the reasoning block; it is an accident of
formatting. Everything native pre-answer reasoning would need to replace is visible in these five
rows, and all three correct conclusions are reached from the same prompt the model already has —
none required information that arrives only after committing.

Free corollary for the dev-vs-user split: at this seat the post-answer text is 263 KB of output
across six games, on decisions whose committed answers it changed once.

## (c) Replies where the discarded scratch text was LOAD-BEARING FOR REVIEW

Two, and the first is the strongest specimen my seat produced in either wave.

1. **vs158 s7.** Scored on the decision alone, this is RULE #2's strict-dominance corollary MISSED —
   the identical wave-32 error, in a lost game. Scored with the discarded text read, it is the
   corollary **FIRED**: the reply quotes it verbatim twice, applies it correctly, and concludes
   *"So I will cast Time of Need."* **Without reading the scratch, this wave's revision would have
   rewritten a rule that is working perfectly.** If the reasoning block is removed and native
   reasoning is not captured to the translog, that distinction becomes unrecoverable at review time.
2. **vs146 s13.** The only RULE #4 violation in the corpus. The scratch says *"Losing Elvish Piper is
   catastrophic for your game plan ... Piper is our win condition engine. We must save Piper."* The
   teach landed; the ordering discarded it. Same inversion of the verdict.

**This is the strongest support my seat can give the directive's own conclusion** — the model is
repeating the instructed pattern rather than disobeying, and it is doing so in exactly the places
where a reviewer would otherwise blame the guide. It also carries a review-side requirement that
the directive already anticipates: **the dev loop must capture native reasoning to the translog
(`reasoning_content`), because seat review currently depends on it and would go blind without it.**
Recorded as evidence for the wave-34 step-1 implementers; no protocol design proposed from here.

---

# Related docket item 3 (consumed-decision narration decoration) — one supporting measurement

The narrated consumed-decision line at this seat carries the full decoration:
`- You: Put in Play with Quicksilver Amulet targeting Emrakul, the Aeons Torn [cost: {4}, Tap]
{card text: "{4}, {T}: You may put a creature card from your hand onto the battlefield."}` — 213
chars where the decision is 62. It appears 11 times across the corpus and, because the event stream
is cumulative within a game, each occurrence is re-sent on every subsequent prompt of that game.
Small, as the docket says; recorded because it is measured rather than estimated, and because
stripping it also removes the surface that made N-116g's silent no-op look like a success.
