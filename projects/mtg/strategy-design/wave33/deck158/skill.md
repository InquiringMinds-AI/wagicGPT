# deck158 — rung proposals for the strategy-writing skill (wave 33)

Six proposals. S1 and S2 are the ones I would fight for; S2 corrects a statistic the
skill's own HL7 invited and that my wave-32 predecessor (me) reported as a headline.

---

## S1 — ANNOTATION-TRUTH RUNG: a magnitude must be suppressed or qualified when the effect it quantifies CANNOT OCCUR. (proposed HL rung, render-lane doctrine)

**Evidence: N-158s** (findings §5 R2, notes.md). Foray of Orcs' cast line rendered
`{right now: Army 6/6 -> 8/8, damage 8}` against an opponent board with zero creatures.
The card text **on the same line** said "target creature an opponent controls". The
pilot wrote: *"Since there are no opponent creatures to target, the spell deals 8 damage
directly to the opponent (as per the 'right now' text...)"* — it chose the annotation
over the card text and planted a false term in a lethal computation.

**Why it needs a rung rather than a ticket.** The skill already has the
perception-vs-strategy routing (a guide cannot fix what the render asserts). This is the
inverse case and it is not covered: **the annotation programme is now good enough that
the pilot trusts it ABOVE the card text**, which is the behaviour we asked for. That
trust makes a wrong annotation strictly worse than an absent one, and "wrong" now
includes *right number, impossible effect*. Three waves of magnitude work at this seat
(N-158b, N-158c, N-158h, N-158m) built that trust; this rung is what protects it.

**Proposed wording for the skill's render-lane criteria:** *an emitted magnitude asserts
that the effect will happen. Before emitting, check that the effect CAN happen — that a
targeted rider has a legal target, that a conditional branch is the branch that will be
taken. Where it cannot, suppress the clause or name the reason inline. A confident number
attached to an effect that will not occur is a worse defect than no annotation, because
the pilot has been taught to prefer the annotation to the card text.*

---

## S2 — NEVER REPORT A SEAT'S CLOCK CONDITIONED ON WINS. (correction to an HL7 practice)

**Evidence: my own wave-32 findings.** They led with *"The kill CLOCK collapsed, and that
is the headline the win column hides — winning turn wave-31: 11,13,19,19,13 (median 13);
wave-32: 9,8,10 (median 9)"*. Both figures are medians over **wins only**. Over all six
games the medians are wave-32 **11** and wave-33 **11.5** — unchanged. The "collapse to
9" was a survivorship artifact: fast games are the games you win, so a win-conditioned
clock reports a deck's best games as its typical game. It then reads as a *regression*
the moment the seat wins more games, which is exactly backwards.

HL7 correctly tells us the win column is pool-dependent and directs us to the kill count.
It does not say that the tempo statistics computed alongside it inherit the same
selection bias. **Proposed rung:** *any per-game statistic (turn count, decision count,
Army size at kill, damage clock) must be reported over ALL games at the seat. If a
win-conditioned figure is genuinely wanted, print both and label which is which. A
statistic whose denominator moves with the result cannot be compared across waves.*

---

## S3 — PERFECTA / STRONG-RECORD DECOMPOSITION: decompose against each OPPONENT's corpus-wide record and opener quality, not only the pool adjudication rate.

**Evidence: this seat, 6/6.** The wave-32 doctrine (HL1's winning-seat audit) got me to
ask the right question; the instruments it names — adjudication rate, kill count — were
not enough to answer it. What answered it was two cheap tables the skill does not
currently ask for:

- **Per-opponent corpus record.** deck139 went **0-6** across all 21 games. My win over
  it carries near-zero information about deck158. Without this table the win reads
  identically to the win over deck146 (4-2, the pool's #2).
- **Opponent opener quality in MY games.** deck146 and deck152 each took **two
  mulligans to five cards** against me — visible in one line of their own translogs
  (`mulls: Mulligan, Mulligan, Keep this hand`). Two of six wins are against
  five-card openers.

Result: 6/6 decomposes into two clean wins, two discounted, one re-measurement, one
near-void. That is a materially different report from "perfecta".

**Proposed rung:** *a seat at 5/6 or better must publish two tables before any
attribution: (a) each opponent's record across the WHOLE corpus, (b) each opponent's
mulligan count in the games against this seat. Wins against a 0-6 seat and wins against
five-card openers are named as such.* Cheap — both are one pass over `results.tsv` and
the opponents' own logs, which reviewers already have.

---

## S4 — VERIFY THE BRIEF'S KNOWN-OPEN LIST AGAINST THE CORPUS BEFORE CITING IT.

**Evidence: N-158k.** The wave-33 brief lists it under KNOWN-OPEN ("cite, don't
re-diagnose") — *"Feed-the-Swarm costs present on the cast line, still ABSENT on the
target menu (the COMMIT seat)"*. It was **closed in the very step-1 batch the brief
describes** (`948578887`, `AIPlayerGPT.cpp:6363-6376`, PARSETEST `[W33-N158k]` at
`:10825`), and it is live in this corpus at 3/3 menus and 11/11 target lines.

The KNOWN-OPEN list exists to stop re-diagnosis, and it does that job. But "cite, don't
re-diagnose" is easy to read as "cite, don't measure", and a stale entry then propagates
into a guide that teaches around a gap that no longer exists — my deployed guide's
sentence *"the target menu that comes next does not repeat the prices"* is now a
falsehood the pilot is being told.

**Proposed rung:** *KNOWN-OPEN items owned by your seat get a one-line MEASUREMENT, not
a citation: the count of occurrences this corpus. Zero occurrences on an item the brief
says is open is a finding (either fixed, or unexercised), and either way the guide prose
written around it must be re-checked.* This is the HL10 reconciliation sweep keyed to
the ledger instead of to the guide.

---

## S5 — A FIX GATED ON A PROXY VARIABLE MUST BE VALIDATED AT BOTH PARITIES.

**Evidence: N-158r.** i9's pregame hand-only serialization is gated on
`observer->turn == 0` (`AIPlayerGPT.cpp:2544`). For the player on the DRAW that is a
correct proxy for "pregame". For the player on the PLAY, their first main phase is still
turn 0, so their first land drop — an ordinary in-game decision — is served a prompt with
no battlefield, no opponent board and no `Mana available:` line. 3/3 of my on-the-play
games; 0/3 of my on-the-draw games. One of them cost **214.5 s and 14,570 characters** on
a three-option land drop.

A reviewer checking i9 by sampling mulligan prompts would report a clean PASS. So would
a reviewer who happened to sit on the draw in every game.

**Proposed rung:** *when a shipped fix is gated on a variable that stands in for the
condition rather than naming it (a turn counter for "pregame", a phase for "my turn", a
zone for "castable"), the validation must cover both settings of whatever the proxy
diverges on — both parities, both seats, both phases. Name the proxy in the validation
metric.* Generalises past this fix: the skill already prizes ARRIVAL-TRACING over
fallback-counting (HL5); this is the same instinct applied to the fix's precondition
rather than to its effect.

---

## S6 — THREE UNEXERCISED CORPORA MEANS IT IS NOT A CORPUS ITEM.

**Evidence: N-158f and N-158i (Assault on Osgiliath).** Both are Assault-only triggers.
Assault has now gone **unoffered across three consecutive corpora and three binaries**
despite two copies in the 60-card list. Each wave they are re-listed, re-checked and
re-reported as UNEXERCISED. That is three reviews' worth of attention producing the same
null.

**Proposed rung:** *an item that reaches its third consecutive UNEXERCISED verdict is
retired from the corpus ledger and either promoted to a PROBE with a written spec, or
closed as unreachable. The reviewer who records the third null writes the probe spec or
proposes the closure — it does not carry to a fourth wave as a corpus item.* (The probe
spec for these two already exists in wave-32 deck158 notes: `deck199` with 4× Assault,
4× March, 8 Mountain / 8 Swamp, one pinned game; observables are an Attackers phase in
the same turn Assault resolves and the first-strike ordering in the combat log.)
