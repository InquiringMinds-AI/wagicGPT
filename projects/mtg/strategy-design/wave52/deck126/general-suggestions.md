# wave-52 core-prompt / render proposals - deck126 seat

Layer-routed; engine items are in `seats/seat-125-126.md`.

## G6 - `{paying this taps: <name> - it cannot attack this turn}` states the wrong consequence for a
## [defender]

**Where.** `AIPlayerGPT.cpp` `paymentTapsClause()` - the tail is `" - " << (one ? "it" : "they") <<
" cannot attack this turn"` unconditionally when the caller sets the flag.

**Repro.** 91 rows across my six deck126 logs;
`1788320097-ai_baka_deck126-0x55b12e55f100-vs-ai_baka_deck125.jsonl` seq 86 (t22),
`1788320110-...deck126...vs-ai_baka_deck130.jsonl` seq 13 (t9, 14 life, one Battlement, Rorix
Bladewing 6/5 flying on their line): `Cast Overgrown Battlement {1}{g} ... {paying this taps:
Overgrown Battlement - it cannot attack this turn}`.

**Proposal.** Every creature in this deck carries `defender` - "cannot attack this turn" is a cost of
zero, and the clause is silent about the one real cost (the wall is tapped through the opponent's
combat and cannot block). When the tapped source has `defender`, print `- it cannot block on their
turn`; when it has neither restriction, keep the current words; when a row taps several and they
differ, name each with its own consequence. This is not a legality change and it does not remove a
choice - it replaces a true-but-empty consequence with the true one.

**Falsifiable.** wave 53: rows naming a [defender] source and reading "cannot attack this turn": 0
(was 91). Deck126 takes of such a row while a flier is on the opponent's line: report.

## G7 - `[second copy: ... both stay on the battlefield - no legend rule]` answers a rules question
## and is read as an endorsement

**Where.** lane K's `secondCopyTag()`.

**Repro.** deck126 met 25 such rows and took 9; five were `Chromatic Lantern` #2 (`vs162` seq 10,
`vs123` seq 41, `vs125` seq 39 and 41, `vs152` seq 40) and two were `Exquisite Blood` #2 (`vs146`
seq 12 at 11 life with a live Tribute row beside it, `vs125` seq 135). Under wave 51's
`[already owned:` wording the same seat declined 9 of 9.

**Proposal.** The tag currently says what happens to the copy. Add what the copy DOES, which the
engine already knows: for a permanent whose only continuous effect is a non-stacking lord or static
(`Chromatic Lantern`'s `lord(land|mybattlefield) transforms(...)`), print
`[second copy: you already control Chromatic Lantern; both stay, but the "lands you control have..."
effect is already on - this copy adds only its own {T}: Add]`. That is a factual completion, not a
recommendation, and it is what the pilot's actual question was.

## G8 - a 900 s empty reply on the mulligan ask

`1788320086-ai_baka_deck126-0x559639b50390-vs-ai_baka_deck146.jsonl` seq 1: `latency_ms 900019`,
`fallback: empty_reply`, `choice -1`, on a two-row mulligan menu. The run's `WAGIC_GPT_TIMEOUT` is
900 s, so the ask consumed the whole budget and the fallback kept the hand - i.e. the *first*
decision of that game was never the model's. Routed here rather than as an engine bug because the
corpus is otherwise 15 fallbacks / 3,384 and the harness behaved as configured; the ask worth noting
is whether the mulligan ask (a 1.5-2 KB prompt) deserves a shorter per-ask deadline than a 30 KB
lategame ask, so a stall there costs seconds rather than a game's opening decision.

## G9 - the PLAN echo is an uncontrolled input channel

`vs146` seq 11 emitted a 1,500-char self-arguing PLAN ending in "Both combo pieces ... are now on
the battlefield" (false), and that exact text was re-served as `YOUR PLAN (as you last stated it)`
at seq 12 and seq 13; the seq-12 decision (a redundant second Exquisite Blood at 11 life) was made
on it. Same shape at `vs125` seq 152-216, where a false plan about Exquisite Blood's trigger
direction survived nine turns. Proposal for the reply rules: bound the PLAN to a short forward
INTENT and reject/trim a plan that asserts board state, since a board claim is exactly what goes
stale between windows. (The guide-side half is deck126 edits D66/D67; a guide sentence did not stop
it, which is why it is routed here.)
