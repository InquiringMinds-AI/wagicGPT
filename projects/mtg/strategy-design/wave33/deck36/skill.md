# deck36 → strategy-writing-skill proposals (wave 33)

Three proposals. Each names the rung it attaches to, the seat evidence, and what a reviewer would
have done differently had the rung existed.

---

## P1 — HL3 SHARPENING: **a stopgap's retirement must sweep the RENDER STRINGS it quoted, not only the belief it targeted.** A shipped render fix silently turns a guide's supporting quotation into a lie.

**Attaches to:** HL3 (quoted-render-string sweep) and the Stopgap Retirement checklist item, which
currently asks two questions — is the class dead, and what does the residue rehearse — and does not
ask the third.

**Seat evidence.** deck36's RULE #1 existed because a {0} cost rendered as an empty string. It told
the pilot, in its own voice:

> *"the game does NOT print a cost on a free card - 'Cast Mox Jet' and 'Cast Ornithopter (0/2)' are
> complete lines, not broken ones"* … *"A card with no cost printed on its line costs {0}."*

i5 shipped and now **508/508** engine cost emitters carry `{0}` — `Cast Ornithopter {0} (0/2)`,
`Mox Jet {0} [artifact]`. The rule's BEHAVIOUR is still correct (cast the free thing), but its
JUSTIFICATION is now a false statement about the pilot's own prompt, and its example quotation
`Cast Ornithopter (0/2)` has **zero hits** in 201 prompts. A guide that tells the pilot the render
looks one way when it demonstrably looks another spends its credibility on the wrong side.

The existing two-question retirement test does not catch this: the class IS dead (question a) and
there IS no residue (question b), so a reviewer who answered both correctly would still have shipped
the false quotation. The HL3 string sweep DOES catch it — but only if the reviewer runs the sweep on
the stopgap's supporting prose, which reads like settled background rather than a live claim.

**Proposed rung text.** *When a render fix ships that a guide teach was written AGAINST, the
retirement pass runs a THIRD question before the other two: **what did this teach tell the pilot its
prompt LOOKS like, and does it still look like that?** Grep every such sentence verbatim. A teach
whose behaviour survives but whose render description does not is SUPERSEDED, not FIRED: the
description is deleted in the same revision and the superseding string is quoted in the firing table.*

**Cost:** one extra grep per retired stopgap. **Falsifier:** a wave in which a shipped render fix
leaves a guide's render description true — then the rung is redundant and can be folded back into
HL3's general sweep.

---

## P2 — HL10 / instrument rung: **an instrument that measures a PROXY boundary must state which boundary the phenomenon actually has, and a seat that finds them different must report the divergence as a first-class result.**

**Attaches to:** HL10 (instruments) and the commit-counter reporting obligation in the reviewer brief.

**Seat evidence.** The wave-33 counter was shipped to measure "a reply that answers and then keeps
writing without re-committing". At deck36 it reports **25/201 overruns, 0 retracted, 0 fatal** — a
clean, reassuring result.

It cannot see the class that actually mattered here. **8/201 replies (4.0%) emitted a coded answer
and then a DIFFERENT coded answer later in the same reply.** All 8 scored `post_plan_overrun` 0 and
`commit_retracted` false, correctly per their definitions: `postPlanOverrun` measures the tail past
the first `PLAN:`, and the protocol puts `PLAN:` LAST, so a reply that re-answers and then plans has
nothing after its plan line; `commitRetracted` fires only on three fallback exits, and none of these
fell back — the parser took the last label and executed it.

One of those 8 is **the deciding play of a win**: vs146 s18 answered `CHOICE: 2 (Cast Yotian Soldier
{3} (1/4))`, wrote 4,600 chars, then answered `CHOICE: 1 (Cast Akroma's Memorial {7})`. Akroma's
resolved, gave the team seven keywords, and won the game on turn 13. **`post_plan_overrun` = 0 on
that record.** A synthesis reading only the shipped fields would conclude the post-answer tail is
inert at this seat. It is the opposite of inert.

The general shape: `post_plan_overrun` is a **post-PLAN** boundary standing proxy for a **post-ANSWER**
phenomenon, and the two coincide only in replies that never re-answer — i.e. exactly the replies the
instrument was built to distinguish FROM.

**Proposed rung text.** *When an instrument is shipped to count a behaviour, the seat that reports it
states, in one line, WHICH BOUNDARY the field measures and WHICH BOUNDARY the behaviour has. Where
they differ, the seat reports the divergence with a worked specimen BEFORE reporting the field's
value, and the field's value is reported as a lower bound. A clean instrument reading on a proxy
boundary is not evidence the behaviour is absent.*

This is the seat-review face of the campaign's existing "a green test on the component NEXT TO the
defect proves nothing" discipline; the counters here are the component next to the defect.

**Cost:** one line per shipped instrument. **Falsifier:** a corpus in which the shipped fields and
the last-label-differs count move together across seats — then the proxy is validated and the rung
relaxes to a one-time check.

---

## P3 — HL4 / null-handling: **before writing a probe spec for an unexercised render, grep the OPPONENTS' decklists for a card that triggers it. The pool's other side is the cheapest probe there is.**

**Attaches to:** HL4-2 (state the trigger condition and the corpus count for a null) — which
currently ends at "deliverable is a PROBE SPEC" and does not require checking the opposite seat first.

**Seat evidence.** Wave-32's deck36 review reported i10 attachments UNEXERCISED, swept all 42
translogs of 21 games for `attached to:` / `{attached:` / `[equipment]` / `Equip `, found 0, correctly
concluded deck36 runs no Equipment or Auras, and wrote a full probe spec: build deck198/deck199 in
the numbering gap, 4x Equipment, 4x Aura, duplicate-named creatures, two one-shot games, then delete
the decks.

**No probe was needed.** deck105 — already in the pool, unchanged from wave-32 — runs `Relic
Putrescence`, an Aura with `target=artifact`. It attached to this seat's Mox Jet and **both render
directions fired in 11/11 prompts**: `Mox Jet {0} [artifact] {attached: Relic Putrescence}` and
`Relic Putrescence {2}{b} [enchantment] [attached to: Mox Jet]`. The wave-32 sweep found zero only
because it ran on the wave-32 corpus, where those 11 windows did not happen to occur; the CARD was
in the pool the whole time.

The generalisable error: the wave-32 null was derived from "does MY deck contain a trigger" plus "did
the corpus happen to sample one", and skipped "does ANY pool deck contain a trigger". The first two
are corpus facts and can be zero by luck; the third is a construction fact and cannot.

**Proposed rung text.** *A null on a render whose trigger is a CARD PROPERTY is not final until the
POOL's decklists have been grepped for a card with that property. Report the null in two parts: the
corpus count (how often it fired) and the pool count (how many decks CAN fire it). Pool count > 0
means "unexercised this corpus, re-check next" — a WATCH, not a probe. A probe is only warranted at
pool count 0, and its spec then doubles as a roster request.*

**Cost:** one grep of `Res/ai/baka/deck*.txt` against `primitives/*.txt` per null. **Falsifier:** a
null where the pool-count grep is impractical because the trigger is not a card property (a phase,
a zone transition, a timing window) — the rung explicitly scopes to card-property triggers for that
reason.

---

## NOT proposed (considered, rejected)

- **A rung about sacrifice-seam latency.** Real and expensive here (831 s = 44% of a lost game's
  budget), but it is a deck-and-render finding, not a method one: the guide fixes it and N-36j fixes
  the header. No general reviewer procedure was missing.
- **A rung requiring reviewers to report opponent-seat latency.** I used it (74%/26% split proved the
  vs116 cap was this seat's own clock), and it was cheap — but the reviewer brief already asks for
  spiral characterisation, and one seat is not enough evidence that the split is generally
  load-bearing. Re-propose if a second seat finds the same.
