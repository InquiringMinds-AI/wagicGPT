# Wave-59 synthesis notes

**Inputs.** One proposal file corpus-wide: `wave59/deck123/general-proposals.md` (P1). No
`skill-proposals.md` from any seat. Read: all seven `deck*/review.md`, `engine-seat.md`,
`wave58/general-strategy.md`, `wave58/strategy-writing-skill.md` (amendments 1-331 index +
#313/#314/#315/#322/#330 in full), `wave58/general-strategy.md` CORE-PROMPT sections (R313, R314,
R328-R330), and the two cited translog records.

## P1 — "tell the pilot what to do when LINE 1 depends on arithmetic it may only write on LINE 2" — **REJECT**

One line of reasons: the decisions it cites are real and costly (`123v126` s86, a self-sweep of 31
own creatures that lost a won game; `123v125` s129), but every part of the ask is already carried —
the guide half by skill **#314** ("a guide may not invite a mid-reply change of mind"; every check is
written as a PRE-condition on the single CHOICE line) reinforced by **#322** (a brake runs after the
choice is formed and *before* it is written) and **#330** (a ritual names the window class that owns
it), and the engine half by **R313** (take the LAST legal `CHOICE:` line on a naturally-terminated
reply, stamp `choice_revised`) — so the failure is a deck-guide ritual placed on LINE 2 in violation
of #314, not a general-guide gap.

Three supporting points, recorded so this is not re-proposed:

1. **Wrong file.** `general-strategy.md` is the RENDER / CORE-PROMPT / PROCESS docket; it is not
   served to the model (no reference to it anywhere in `src/`; the pilot-facing reply rules are
   `kReplyProtocol` in `AIPlayerGPT.cpp`). "Add pilot-frame text to the general strategy's reply
   section" has no delivery channel. The proposal's own durable fix (its review's I4) is correctly
   routed to the engine seat, which is where a `kReplyProtocol` change belongs.
2. **The proposed text contradicts an adopted amendment.** #314 closes: *"No guide sentence may
   substitute for it, and this amendment does not authorise one."* P1's text instructs the pilot to
   *"write a SECOND answer line with the choice you now mean"* — the mid-reply change of mind #314
   bans, and one that R313 explicitly declines to protect past two lines (three or more `CHOICE:`
   lines keep the refusal). Teaching it would convert some current one-line answers into refused
   three-line replies.
3. **The seat already fixed it where it can be fixed this wave.** deck123's own guide edit **E4**
   (`wave59/deck123/strategy.txt`) replaces a false claim with what the engine does and closes
   *"Never leave a CHOICE line standing that your own plan line contradicts"* — deck-side, deployed,
   paid by s86. That edit does **not** contradict any adopted general change (nothing was adopted),
   and unlike P1's wording it stops short of instructing a second answer line, so it is consistent
   with #314. The remaining generality — other guides whose ritual output decides the answer line —
   is a #314/#322 conformance check for those guides' own reviewers, not a new item.

Engine-seat cross-check: `engine-seat.md` files no defect in this shape (its HIGH items are the false
`THIS WINS THE GAME` badge, the duplicate-index `PUT:` rejection, and the connect-budget regression),
so nothing there pays for P1 either.

## Outputs

* `wave59/general-strategy.md` — `wave58/general-strategy.md` copied forward, **no adopted edit**
  (`cmp` clean against wave 58).
* `wave59/strategy-writing-skill.md` — `wave58/strategy-writing-skill.md` byte-verbatim, **no
  amendment appended**; edition remains amendments 1-331. Prefix verified with `cmp` (identical whole
  file, 1,304,751 bytes).

## What I did NOT check

No build, no run, no git. I did not re-derive any engine-seat census figure, did not read the
wave-58 corpus, did not read the six non-deck123 `strategy.txt` files for #314/#322 conformance
(only deck123's edit list), and did not read amendments 1-312 in full — only the index line of each
plus the five read whole. I verified the two P1 seqs and `s69` directly in the translogs; I did not
audit the rest of deck123's slice.
