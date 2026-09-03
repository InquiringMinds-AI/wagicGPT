# deck123 — core-prompt / render proposals (wave 54)

Layer routing: everything here is a change to text the engine EMITS, or to the reply contract.
Defects in engine behaviour (windows, drops, loops) are in the seat file's engine list, not here.

---

## G1 (HIGH, render) — mark the row that is correct when every cast row is dead
**Why.** This is the fourth corpus at deck123's dead-cast rung and the guide lane is now
demonstrably exhausted: `123v125` s176 is the FIRST reply in four corpora to write the mandated
`PLAN: Damnation: N 0, K 0, M 0;` — and it cast the Damnation on the same line. Wave 53's own
prediction P7 said this is the point at which the ask becomes a row annotation. Five cards went
this way in one game (s147, s176, s192, s211, s269) and the pilot's own prose named the reason:
"cast second Intruder Alarm (dead, but only way to proceed)". The model is not misreading the
annotations — it is treating "Cast nothing right now" as a non-answer.
**Ask.** When EVERY `Cast ...` row on a casting menu carries a dead verdict — `{right now:
destroys 0 of their creatures ... 0 of yours}`, `{right now: they control 0 creatures - at 0 this
does nothing}`, `{kills 0 of the N legal targets ...}`, `[second copy: ... a second copy changes
nothing]`, `{X pricing: ... kills nothing}` — extend the pass row's own text:
`Cast nothing right now (every card you could cast on this menu is priced as changing nothing)`.
One clause, on the row that should be taken, computed from verdicts the emitter already has.
No legal choice is removed and no row is re-ordered.
**Falsifier.** deck123 casts off an all-dead menu: 5 this corpus -> 0.

## G2 (HIGH, reply contract) — `plan_choice_conflict` fires on the PLAN template this pool mandates
**Why.** 7 of my seats' 16 fallbacks are this check, all on the repeat row, all in two games
(`123v126` s29/s58/s62/s90/s95, `123v146` s23/s26). Four of the seven re-asks came back byte-
identical and were executed anyway (`plan_choice_conflict_exhausted` at s30, s59, s24, s27) — a
model call bought nothing. One was a true positive and corrected the play (s90 -> s91). The
trigger is a PLAN that contains "pass" or "stop reached" as *arithmetic commentary* beside a
CHOICE that takes a row, which is the shape the guides ask for ("stop <L+C+3>; M is <M> now").
**Ask.** Scope the prose check the way wave 53's H1 already proposed and narrow it once more:
fire only when the prose names a DIFFERENT ROW (a number, or a row's short name) than the coded
line. A paragraph that reaches a verdict without naming a row should be ignored, not re-asked.
Corpus-wide this would keep s90 and drop s29/s58/s62/s95/s23/s26.
**Falsifier.** `decision_reversed_in_prose` firings whose re-ask returns the identical reply:
4 this corpus -> 0.

## G3 (MED, render) — collapse duplicate rows in library-reveal menus
**Why.** Unchanged from wave-53 L2 and still the two largest non-log payloads at this seat:
`123v125` s9 lists **50** rows with `Intruder Alarm` four times, `Damnation` three times,
`Thraben Doomsayer` four times as separate identical strings; `123v126` s23 lists 45 the same way.
The battlefield line above them already collapses identical permanents with `x4` handles.
**Ask.** The `xN` handle form the battlefield line uses, applied to reveal lists.

## G4 (MED, render) — the fetch row still names a land and never its colours
**Why.** Carried from wave-53 M2, unchanged and still unmarked. `123v130` s8 and `123v146` s20
each offered four or five `Put a card onto the battlefield with Marsh Flats targeting <land>` rows
with `{this land makes no mana - crack it for a land: your untapped mana sources go from N to
N+1}` and **no colour on any row**. Behaviour improved without it (4 of 5 fetches took a dual this
corpus, against 5 of 8 last), so this is now a cheap correctness item rather than a bleeding one.
**Ask.** Append the produced colours to each fetch row, in the same shape the mana line uses:
`targeting Tundra {this land makes no mana - crack it for a land ...; the land it fetches makes
{W}{U}}`.

## G5 (LOW, render) — `- Paid` receipts and the mana line are clean at this seat
15,170 `- Paid` renders corpus-wide, 0 `with no source tapped`; at my seats every forecast
`{leaves N of your M untapped mana sources untapped}` matched the receipt I could check. Recorded
as a PASS, no ask.
