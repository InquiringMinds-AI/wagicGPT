# deck152 -> core (general prompt) proposals, wave 33

## VERDICT: **PASS** (28th consecutive). No change proposed to the general prompt.

The core prompt was quoted back at me correctly by my own pilot this corpus, at the one seam that
has historically broken it. deck116 s20, verbatim from the reply:

> *"the prompt explicitly states: 'If a spell is a numbered "Cast..." option it IS payable... If a
> spell is not in the cast list you CANNOT cast it right now'. Since Teferi is **not** listed in
> the 'Your legal actions' section, you cannot cast him this turn."*

That is the completeness guarantee working: a correct suppression, obeyed in 1,300 characters,
with no contradiction claim and no fallback. Wave-32's review of this seat carried two
kilobyte-scale *"This is a contradiction"* spirals against the same sentence -- and they were the
prompt telling the truth about an engine that was lying. With N-152f closed the guarantee is
honest again, and the seat's behaviour changed accordingly with no prompt edit. **The right
conclusion is that the wave-32 pressure on that sentence was misattributed to the core prompt and
has now been resolved in the engine.** Nothing to widen, nothing to carve.

Two further checks, both clean at this seat:

- **The win-condition sentence** (`AIPlayerGPT.cpp:63`, *"You win by reducing the opponent's life
  from 20 to 0"*) -- deck105's wave-32 **G1**, HELD pending this corpus. I am one of the seats that
  played the poison matchup with the render live. **The conflation did NOT persist at my seat.**
  The pilot correctly treated poison as a losing clock, at a healthy 20 life, and said so:
  *"Not blocking would be a significant risk toward losing via poison"* (s32). It also correctly
  tracked the OPPONENT's poison total as a second route to winning without ever confusing it with
  life. The false core sentence was in front of it in every prompt and the RENDERED number
  overrode it -- which is the routing the loop already believes (a rendered number beats prose).
  **My evidence supports NOT adopting G1.** It is deck105's item to close, but this is a second
  seat reporting no residual conflation.
- **The reply protocol** -- see the owner-directive evidence below. My contribution is evidence
  only, per the directive; I propose nothing about it.

---

## OWNER DIRECTIVE ITEM 1 -- EVIDENCE ONLY (no counter-proposal, no reordering, no terminator)

The directive asks each seat for three specific things. All three, measured:

### (a) Counted overrun / retraction fields

All 211 decisions at this seat, fields quoted verbatim from the translog:

| field | value |
|---|---|
| `post_plan_overrun > 0` | **21 / 211 = 10.0%** |
| mean overrun (of those 21) | **2,173 chars** |
| median overrun | 1,081 chars |
| max overrun | **13,243 chars** (deck116 s39, `kind=reveal`, 15,082-char reply, 187 s) |
| `commit_retracted: true` | **0** |
| fatal overruns | **0** |
| replies with no `PLAN:` line at all | **5 / 211** (protocol-compliant under `f46dd58ee`) |
| `pruned_pairs` non-empty | 1 (the `all_assignments_illegal` at deck36 s8) |

**deck152 contributes ZERO fatal post-PLAN retractions to the HL10 promotion trigger.** The
overrun is real and large in the tail, but at this seat it never cost a commit.

One measurement note that may matter to the A/B design: the overrun and the reply length are only
loosely coupled here. My three largest replies (deck139 s2 at 9,737 chars, deck139 s10 at 10,264,
deck116 s38 at 11,573) carry overruns of 36, 32 and 1,208 -- the bulk of the text is BEFORE the
PLAN line, i.e. it is the reasoning the protocol currently invites between the answer and the
plan, not scratch after the terminator. **A metric that counts only chars past `PLAN:` will
understate the post-answer prose by roughly an order of magnitude at this seat.** If the wave-34
A/B is scored on `post_plan_overrun` alone it will read as a much smaller effect than the owner's
corpus-wide ~1,900-char mean implies for total post-answer output.

### (b) Does the 4.2% post-answer self-correction carry decision VALUE?

**At this seat: 11 / 211 = 5.2% last-label-differs re-commits, and YES, materially.** The engine
took the LAST label in all eleven. Full decomposition:

| window | first label -> last label | assessment |
|---|---|---|
| deck146 s29 `blockers` | `B1:A1, B1:A4` **(ILLEGAL: one blocker on two attackers)** -> `B1:A1` | **legality rescue** |
| deck116 s24 `ask` | `Cast nothing` -> `Cast Fateful Absence`, killing `Thragtusk (5/3)` | high value |
| deck36 s26 `ask` | `Cast nothing` -> `Cast Teferi`, resolved and piloted | high value |
| deck36 s15 `priority` | `pass` -> `level Ranger Class` | rescued a false decline made one decision earlier |
| deck105 s24 `priority` | `pass` -> `level Ranger Class` | free value, guide-sanctioned |
| deck116 s32 `priority` | `pass` -> `level Ranger Class` | free value |
| deck116 s38 `priority` | `pass` -> `-2: reveal top 3` | mild positive |
| deck146 s28 `ask` | `don't add any counter` -> `add 1 counter` | positive (spare mana, guide rule) |
| deck146 s21 `blockers` | `B1:A1` -> `none` (10 life, 1 incoming) | neutral / defensible |
| deck139 s3 `ask` | land swap | mild positive |
| deck139 s10 `ask` | `Play Plains` -> `Play Deserted Beach` | mild positive (enabled the blue later) |

**9 of 11 improved the decision, 1 converted an illegal answer into a legal one, 0 were harmful, 0
retracted a correct answer.** The pattern is legible and not random: the dominant shape is
**`pass`/`Cast nothing` -> a real action**, i.e. the pilot's first instinct at a low-information
seam is to decline, and the second pass finds the value. That is exactly the class of improvement
pre-answer reasoning should deliver, so the directive's target design should absorb it cleanly --
but it is not free, and the number to beat at this seat is 5.2% of decisions.

**The one item I would not assume transfers**: the deck146 s29 legality rescue. Native pre-answer
reasoning reasons about the board; the illegal assignment there was caught by the model *re-reading
its own emitted first line* against the protocol's own constraint sentence
(*"Wait, I cannot assign the same blocker (B1) to two attackers"*). This corpus also contains the
counterfactual: **deck36 s8, the seat's only fallback, is an illegal blocker assignment that was
NOT self-corrected and cost the decision** (`all_assignments_illegal`, `pruned_pairs: "Intrepid
Adversary -> Glaze Fiend"`). So at this seat the post-answer block caught one illegal block and
missed one, 1 of 2. **Recommendation as evidence, not as a counter-proposal: whatever replaces the
block, the wave-34 A/B should report ILLEGAL-ASSIGNMENT RATE as a first-class metric alongside
decision quality and latency**, because it is the one axis where the discarded text is currently
doing safety work rather than value work, and it is already instrumented (`all_assignments_illegal`
+ `pruned_pairs`).

### (c) Was the discarded scratch text load-bearing for REVIEW?

**Yes, decisively, and it is the whole diagnostic surface of this seat's review.** Every named
belief in my findings.md is recoverable ONLY from prose the protocol calls discardable:

- the successor mana belief -- *"Ranger Class is {1}{G}, so I need 1 generic and 1 Green. **Do I
  have generic mana?**"* (deck36 s15). Without it the record shows `pass`, and the false decline one
  decision earlier looks like an ordinary hold.
- both false mulligans -- *"the hand is effectively dead for the first few turns"* (vs158 s1),
  *"a fatal flaw for this specific deck"* (vs105 s1). Without them I would have two mulligans and no
  way to tell they were guide-authored, and the vs158 loss would have read as variance.
- the i2 validation -- the pilot quoting the poison forecast's arithmetic back before chump-blocking
  with a lord (s32). Without it, i2 would be "the string rendered", not "the string changed a
  decision".
- the completeness-guarantee compliance at deck116 s20, which is the core-prompt PASS above.

**This does NOT argue against the directive** -- the dev-loop half of the owner's design
(native reasoning ON, captured to translog as `reasoning_content`) preserves all of it. It argues
that the capture is a **hard prerequisite, not a nice-to-have**: a wave-34 corpus that turns
reasoning on without persisting `reasoning_content` into the translog would leave the seat reviews
with `choice`, `chosen_text` and a terse answer, and every rung in the skill that depends on
naming a belief in the pilot's own words -- HL6's recite-then-exclude tell, HL10's distinct-belief
count, HL7's anti-pattern-verbatim rule, the tax table -- would have nothing to read. I would
gate the A/B on that field existing and being non-empty at both seats before the corpus runs.

---

## Related, routed elsewhere, not a core proposal

- **N-152k** (`{0}` printed for an ABSENT cost -- 166 false renders at this seat, caused by i5) is
  a render-lane item in notes.md, not a core item. Flagged here only because it puts a false card
  fact inside the same prompt as the Flip Side annotation that contradicts it, and the annotation
  is the general-prompt-adjacent surface. No edit proposed.
- **Cap / latency**: two cap adjudications at this seat, both with the board alive, 79 s and 67 s
  per decision, 18 decisions over 100 s, reply lengths FLAT vs wave-32. Numbers are in notes.md
  (N-152m) for the synthesis's cap-3000-to-3600 / `-j 2` weighing. I take no position; the seat's
  own commits were never retracted, so the protocol fix and the cap question are independent here.
