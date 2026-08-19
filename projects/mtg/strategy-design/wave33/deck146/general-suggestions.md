# deck146 -> core (general prompt) proposals, wave 33

**Core prompt verdict: PASS (28th consecutive).** Nothing this seat observed is curable in the
general prompt: the mulligan cluster died to a render change plus a deck-guide floor, the Verse
class is card-script/engine, the spirals are a render ambiguity plus the reply protocol (already
under an owner ruling), and the one fatal decision is a parser resolution rule. Routing all of
them into core text would be the exact layer error the loop exists to prevent.

---

## Owner directive item 1 -- EVIDENCE ONLY (no counter-proposal offered, per the directive)

### (a) Counted overrun / retraction fields at this seat

| Field | Value |
|---|---|
| Decisions with a model call | 227 |
| Replies carrying a `PLAN:` line | 207 (91.2%) |
| Replies with NO `PLAN:` line (compliant per `f46dd58ee`) | 20 (8.8%) |
| `post_plan_overrun` nonzero | **22 / 227 = 9.7%** |
| Total post-answer chars | **68,556**; mean per overrunning reply **3,116**; max **15,163** |
| `commit_retracted` | **0** |
| `pruned_pairs` populated | **0** |
| Fatal overruns (overrun produced a wrong committed decision) | **0** |
| Replies over 60s | 24 / 227; max latency **228.3s** |
| Seat inference in the cap-adjudicated game | **30.4 min** (p90 199.7s) |

The four heaviest overruns in the whole corpus are at this seat (15,163 / 12,155 / 9,091 / 8,638
chars, at 201.0s / 213.8s / 214.1s / 228.3s). Correctness survived all four; the clock did not.

### (b) Does the post-answer self-correction carry decision VALUE?

**At this seat, yes -- 4 of 227 decisions (1.8%) were changed after the first-line answer, and all
four changes were improvements that the parser correctly committed:**

| Repro | First line | Final | Value |
|---|---|---|---|
| vs139 s11 | `CHOICE: 1 (Cast Vanishing Verse)` | `CHOICE: 2 (Cast Pelakka Predation)` | traded a removal spell for a hand-strip against a deck whose threats were already on board |
| vs152 s36 | `CHOICE: 2 (Cast nothing right now)` | `CHOICE: 1 (Cast Emeria's Call)` | cast the seven-mana haymaker that won the game (two 4/4 fliers, opponent dead t16) |
| vs36 s33 | `CHOICE: 4 (Cast nothing)` | `CHOICE: 2 (Cast Acererak)` | took the free venture in the guide's sanctioned dead-turn condition |
| vs158 s24 | `BLOCKS: B1:A3, B1:A4` | `BLOCKS: B1:A1` | **legality self-repair** -- one blocker cannot block two attackers |

So the value is real but it is of one specific kind: **the model's first line is a fast answer and
the reasoning that follows sometimes finds a better one.** Nothing in these four required the
reasoning to be AFTER the answer -- each is a deliberation the model would have performed anyway
if asked to reason first. The vs158 s24 case is the sharpest: a legality constraint the model knew
and violated on its first line, then repaired. Native pre-answer reasoning would have to reproduce
exactly this class; there is no evidence here that it could not, and the retraction machinery
exists only because the answer currently comes first.

### (c) Was the discarded scratch text load-bearing for review?

**Once, weakly.** Every decision-relevant fact this seat reported is recoverable from
`options_text` + `choice` + `chosen_text` + the prompt. The single exception is a card-fact
MISBELIEF that appears nowhere else in the record: vs116 s37, post-answer, verbatim *"So Vanishing
Verse is useful for hitting lands"* -- the inverse of the guide's crutch, invisible in the chosen
option because the model chose something else. Diagnostic value, not decision value.

Against that, the scratch text is **actively harmful to the record in the other direction**: the
`PLAN:` line is the only state carried between decisions, 14 replies emitted a PLAN tail over 800
chars, and at vs116 the 9,091-char s35 spiral was re-injected VERBATIM as `YOUR PLAN (as you last
stated it)` into the s36 AND s37 prompts, where s37 then re-ran the same failed arithmetic. Two
decisions were made on a prompt whose carried plan was three pages of the model's own uncertainty.
If reasoning moves to a native channel, the dev loop keeps every diagnostic the scratch provided
(captured to `reasoning_content`) and the prompt stops carrying it -- which on this evidence is
not a side benefit but the larger of the two wins.

### (d) One measurement the A/B should carry

The three worst spirals at this seat are re-derivations of state the prompt does not state
(N-146n, the dungeon room position). If the wave-34 A/B moves reasoning native without also
closing that ambiguity, the reasoning will simply spiral in the native channel and the latency
finding will be confounded. Recommend the A/B either ship N-146n first or record room-choice asks
as a separate stratum.

---

## Held item re-check (deck105 G1 alt-win clause)

Not this seat's clause, but this seat is an independent second witness and the brief asked for a
re-check with poison rendered: **the conflation does NOT persist here.** With i1 live, deck146
read its own poison total correctly in 15/15 records, priced a 3-poison swing against the
threshold from the blocker forecast, and won the rematch of the game it lost at 21 life. No core
clause is needed on this seat's evidence.
