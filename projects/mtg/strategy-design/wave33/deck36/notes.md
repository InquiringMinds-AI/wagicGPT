# deck36 — development notes (engine / harness / representation ledger), wave 33

Seat: deck36 (Master of Ether, UB artifacts). Binary `bdb30d8e7`. Corpus
`matchups-20260819-070739`. All seq/turn citations are from the deck36 seat log of the named
matchup. Card facts read from `bin/Res/sets/primitives/mtg.txt` with `/usr/bin/grep` this session.

**Discharged this wave — do not re-open:**
- **N-36a** (poison invisible + false life math on infect) — **FIXED and VALIDATED.** i1: 17/17
  situation-block poison lines from the first counter on; symmetric non-combat narration
  (Ichor Rats) present; infect combat damage narrated distinctly. i2: 3/3 infect blocker windows
  partitioned and priced against 10, `you would be at N - NOT lethal` 0/3 on infect, non-infect
  control unchanged 7/7. The seat's wave-32 loss shape did not recur; it WON the rematch at 20 life.
- **N-36b** ({0} renders as empty string) — **FIXED and VALIDATED.** 508/508 engine cost emitters
  carry `{0}` (55 hand / 26 cast-option / 427 battlefield), plus a new pregame affordability
  emitter that also carries it. 0 confabulations in 201 replies. 0/45 `Cast nothing right now`.
- **N-36h** (i10 attachment probe) — **NO PROBE NEEDED; see N-36h-2 below for the narrowed residual.**

---

## N-36i — A NON-ARTIFACT LAND CARRIES NO TYPE TAG ON THE BATTLEFIELD LINE, AND THE TWO EMITTERS DISAGREE  [MED — latency cost, arrival-traced twice]

**Symptom.** On the battlefield line an artifact land renders `Vault of Whispers [artifact]` and a
plain land renders with **no tag at all**. Verbatim, vs105 s11, against `Artifacts in play: you 5`:

```
Your battlefield (creatures: 2): Tolarian Academy; Ornithopter {0} (0/2) [artifact] [flying]; Mox
Jet {0} [artifact]; Mox Sapphire {0} [artifact]; Glaze Fiend {1}{b} (2/3) (printed 0/1) [artifact]
[flying]; Vault of Whispers [artifact]
```

Six items; five tagged; `Tolarian Academy` bare. Confirmed on a basic-land board, vs158 s23:
`... Mox Sapphire {0} [artifact]; Island; Scarecrone {3} (1/2) [artifact]; ...`

**The engine's count is CORRECT** (`name=Tolarian Academy / type=Legendary Land`); the pilot's
recount is not, because the render gives it no discriminator in a list otherwise composed of
`[artifact]`.

**Arrival-traced cost, 2 windows, both in vs105:**
- s11: *"Total = 6 artifacts. The prompt says 'Artifacts in play: you 5'... Perhaps Tolarian Academy
  is not counted as an artifact in the summary? Or maybe the prompt's summary count is slightly
  off?"* — then deferred: *"I must trust the game state provided in the summary line over my manual
  count."*
- s23: *"That is definitely 9. However, the prompt explicitly states 'Artifacts in play: you 8'.
  This might be a discrepancy in the prompt's count vs the list."*

No wrong decision resulted (the guide's read-don't-count rule held both times); the cost is reply
budget and a rehearsed doubt about the one number this deck's whole plan is denominated in.

**Emitter disagreement — the fix is already written elsewhere in the same file.** The SACRIFICE menu
emitter tags the same permanents correctly: `Swamp [land] [your battlefield] [tapped]`,
`Tolarian Academy [land] [your battlefield]`, `Vault of Whispers [land] [your battlefield] [tapped]`.
So `[land]` exists as a token; the battlefield emitter simply does not print it.

**Fix.** Emit `[land]` on the battlefield line for every land, artifact land or not, so an artifact
land reads `Vault of Whispers [land] [artifact]` and a plain land reads `Tolarian Academy [land]`.
Per HL5, enumerate every emitter of a permanent's type tags before declaring this closed — at least
the battlefield line, the sacrifice/choose-a-card menu, the hand line (`Island (land: taps for {U})`,
a THIRD distinct form) and the attacker/blocker option lines. This is wave-32's N-36d with the
failure mode INVERTED (was: under-reading artifact lands; now: over-reading non-artifact lands), so
it supersedes N-36d rather than sitting beside it.

**N-36i-b, same family, separate call needed.** The situation-block poison pair is suppressed while
both totals are 0 (absent s1–s19 of the vs105 game, present 17/17 from s20). The blockers header does
print `0 of 10`, so the decision-relevant window is covered and this costs nothing — but it is the
same falsy-zero shape N-36b just closed, and for a deck with no infect the OPPONENT's `0 of 10` is
the only view it gets of an alt-win it could theoretically race. Owner call, not a defect claim.

---

## N-36j — THE SEQUENTIAL-SACRIFICE HEADER SAYS "of exactly 6" WITHOUT SAYING SIX OF WHAT  [MED — contributes to the corpus's worst latency sink]

**Symptom.** Annihilator resolves as N sequential one-card asks. Each carries:

```
SACRIFICE ONE OF YOUR OWN CARDS: your own effect (Sacrifice) makes you sacrifice a card from the
list below - each option is a card YOU will LOSE, NOT a target you attack or affect. Pick the card
you can best AFFORD TO LOSE (usually your LEAST valuable: a spare land or a redundant/dead card;
KEEP your best spells, answers, and threats). Choose card 4 of exactly 6 from the list below, and
answer with the chosen card's name.
```

The "6" is the number of SACRIFICES the trigger demands. The list below it holds 9 or 11 permanents.
Nothing on the line says the 6 is a sacrifice count rather than a list length, and the pilot reads
it as a list length and concludes the list is broken.

**Verbatim, vs116 s38** (129 s / 8,793 chars): *"The prompt says 'Choose card 1 of exactly 6'. This
is confusing. Maybe the list is truncated or I should just pick from the 9. The instruction says
'Choose card 1 of exactly 6 from the list below'."*
**Same at vs116 s30** (137 s / 9,127 chars): *"The prompt asks for 'Card 4 of exactly 6'. This is
weird. Maybe the list is truncated or the prompt is flawed."*

**Corpus count: 16 sacrifice windows at this seat, all in the vs116 game (s27–s32, s38–s42).**

**Cost.** Not the whole spiral, but the opening paragraph of every one of them. The six worst
decisions of that game total **831 s = 44% of this seat's 1,910 s budget**, in a game the 3,000 s cap
adjudicated at turn 13 with this seat consuming 74% of the wall clock (opponent: 660 s / 26 asks).

**Fix (cheap, string-only).** State the quantity and the position in the sequence:
`This is sacrifice 4 of 6 that this effect demands. Choose ONE card from the list below; you will be
asked again for the remaining 2.` Optionally state what survives: `After all 6, 5 of your 11
permanents remain.` A stronger fix is to ask ONCE for an ordered set of 6, but that changes the
answer protocol and is not proposed here.

---

## N-36e — THE PLAN SPLITTER ANCHORS ON THE **FIRST** `PLAN:`, NOT THE LAST  [MED-HIGH — parser; root-caused this wave; regressed vs wave-32]

**Measurement.** **107 of 201 prompts (53%)** carry a `YOUR PLAN (as you last stated it):` field
longer than 400 chars; p50 428, max **1,541**. Wave-32 was 51/132 = 39%, max ~800.

**Root cause, exact repro (vs105 s35 -> s36).** s35's reply contains TWO `PLAN:` occurrences:

- char **501**: an abandoned first-pass plan, immediately followed by ~4,850 chars of the model
  talking itself out of it.
- char **5,359**: the real committed plan — *"PLAN: Pass priority in Main Phase 1 to proceed to the
  Declare Attackers step. Attack with Master of Etherium, Glaze Fiend, Cathodion #1, Cathodion #2,
  and Silver Myr to deal lethal damage (approx 10+ damage) and win the game."*

The s36 prompt's plan field is **1,424 chars** starting at the char-501 text and running through the
abandoned deliberation, verbatim: *"Proceed to the Declare Attackers step. Attack with Master of
Etherium, Glaze Fiend, Cathodion #1, Cathodion #2, and Silver Myr (once it is cast, though it is
currently in hand and needs to be cast first, but the prompt implies we are deciding the *next*
action in the sequence. Wait, looking at the options provided in the prompt... Or maybe the prompt is
truncated? ... No, that's a contradiction."*

**The committed plan never reached the next prompt. The retracted reasoning did.**

**Cross-check that the fix is already proven in this codebase.** `parseChoice` gets the same problem
RIGHT: at vs146 s18 the reply emits `CHOICE: 2 (Cast Yotian Soldier {3} (1/4))`, then 4,600 chars,
then `CHOICE: 1 (Cast Akroma's Memorial {7})`, and the engine executed the LAST one (translog
`chosen_text` = Akroma's Memorial) — which won the game. So the last-line-leading-label rule exists,
works, and is decision-critical; `consumePlan` just does not use it.

**Fix.** Anchor `consumePlan` on the LAST line-leading `PLAN:` occurrence (matching `parseChoice`'s
own rule), and cap the extracted field length. Note the same first-occurrence convention is
deliberate and correct in `postPlanOverrun` (`AIPlayerGPT.cpp` ~2219) — that one measures the tail
and should not change; only the CONSUMER should move to last.

---

## N-36k — THE GAME LOG'S TURN-SCOPED ENTRIES READ AS THIS-TURN ENTRIES AGAINST THE PHASE HEADER  [MED — the seat's single most expensive reply]

Distinct from the phase-header contradiction (which is about turn OWNERSHIP; see
`general-suggestions.md`). Here the pilot reconciles the LOG against the PHASE.

**vs158 s16, 227 s / 13,930 chars, chose `pass` — the worst single reply at this seat.** Verbatim:

> *"'Your Scarecrone: hand -> stack' -> Cast Scarecrone. 'Phase: Attackers' -> Attack with Salvage
> Slasher. 'Phase: Combat damage' -> Deal 1 damage. Then 'CURRENT SITUATION' says 'Phase: Main phase
> 1'. **This is contradictory.** If I already attacked and dealt damage, I am in the End Phase or
> Second Main Phase, not Main Phase 1. Unless the log is describing a *hypothetical* or *previous*
> turn? No, the log says 'Turn 6'."*

The log entries it is citing belong to an EARLIER turn under the same `=== Turn N ===` heading
convention; the pilot cannot tell that the current turn's entries have not yet been written. Cheapest
fix: mark the in-progress turn explicitly, e.g. `=== Turn 7 - YOUR turn (IN PROGRESS) ===`, or emit
the current phase as the last log line so the log and the header cannot disagree.

**Related, same seat, cheaper:** several `priority` replies burn budget on *"if playing a land is
legal, it should be listed; if it's not listed, maybe I'm not allowed to? No, that contradicts the
rules"* (vs158 s16, vs105 s7, vs105 s35, vs152 s16). The pilot does not accept that the offered list
is complete for the current step. One line on the priority prompt — `this list is complete for this
step; anything not listed is not available to you right now` — would close a recurring paragraph.
Recorded, not proposed as a core change here.

---

## N-36c — MASTER OF ETHERIUM'S CAST LINE IS STILL ONE SHORT  [MED — carried, NOT fixed]

Unchanged from wave-32; re-verified 3/3 this corpus.

| game | cast line | `Artifacts in play` at cast | arrival |
|---|---|---|---|
| vs105 s23 | `Cast Master of Etherium {2}{u} (8/8)` | `you 8` | blocks at s26 as `Master of Etherium (9/9)` |
| vs152 s24 | `Cast Master of Etherium {2}{u} (6/6)` | `you 6` | plan and board read `(7/7)` |
| vs139 s11 | `Cast Master of Etherium {2}{u} (7/7)` | `you 7` | attacker lines `(10/10)` s18, `(11/11)` s24 |

The option is built while the card is still in hand, so the CDA count excludes Master itself.
**The guide's `+1` stopgap is doing the work perfectly (4/4 plans state the post-resolution number),
so this is now a pure representation debt, not a decision risk.** Fix remains: build the cast-line
CDA against the post-resolution battlefield, as the Gray Merchant `{drains N}` fix did.
**Stopgap removal condition (positive obligation on the wave-34 reviewer): the first corpus in which
`Cast Master of Etherium {2}{u} (N/N)` prints the post-resolution value, the guide's "add 1" block is
DELETED that same wave and the prediction inverts to "the plan matches the printed number".**

---

## N-36f — "coloured mana cannot pay a generic cost": BELIEF ABSENT, REPRESENTATION UNCHANGED  [recorded, steady state]

The wave-32 proposal (make the mana line say what it BUYS, not what colours exist) did **not** ship.
The line is verbatim unchanged: `Mana available: 4 total (four untapped sources, tapped automatically
when you cast; colours you can make: {u}{b})` — 191/201 prompts.

**The belief did not recur: 0/201 replies contain a "cannot pay {1}" / "no generic mana" derivation**
(baseline: 2 replies, 1 costing a combat step). Guide RULE #2 held it. Recorded as steady state; the
representation change stays available but is not escalated on this corpus's evidence.

---

## N-36g — TOKEN AND CLUE CARD TEXT RENDERS AS AN EMPTY PARENTHESIS  [LOW — cosmetic, recurs and spreads]

- vs152 s20: `Draw 1 with Clue [cost: {2}, Sacrifice] {card text: "() source: Glaze Fiend"}`
- vs116 s27–s29 (the token family, same shape): `Pest #1 (0/1) [your battlefield] - "( artifact) source: Nuisance Engine"`

The option LABEL carries the real information and the pilot answered correctly every time. Only the
`{card text: ...}` payload is empty, and the `source:` suffix reads as if the token WERE its creator.
Unchanged from wave-32 except that the token path now shows it too.

---

## N-36h-2 — i10 ATTACHMENTS: PROBE DEBT DISCHARGED, RESIDUAL NARROWED TO TWO ITEMS

**The wave-32 probe spec (build deck198/deck199) is CANCELLED — the pool exercised it.** deck105's
`Relic Putrescence` attached to this seat's Mox Jet and both render directions fired in 11/11
prompts (vs105 s26–s36):

- FORWARD: `Mox Jet {0} [artifact] {attached: Relic Putrescence}`
- REVERSE: `Relic Putrescence {2}{b} [enchantment] [attached to: Mox Jet]`

Correctness check against the primitive — `name=Relic Putrescence / abilities=infect /
target=artifact / auto=@tapped(mytgt):damage:1 targetcontroller / text=Enchant artifact -- Whenever
enchanted artifact becomes tapped, its controller gets a poison counter. / type=Enchantment` — the
host is an artifact, legal for `target=artifact`. The render is correct, not merely present.

**Residual, stated honestly, both still UNEXERCISED:**
1. **Instance handles in the reverse render.** The host here is a singleton (correctly rendered with
   no `#N`). Whether `[attached to: Memnite #2]` binds to the same handle the battlefield line gives
   that creature is still untested. Trigger condition: an Aura or Equipment attached to a permanent
   whose NAME appears 2+ times on the same battlefield line. Corpus count: **0**.
2. **Equipment entirely.** `[equipment]` and `Equip ` are **0/201** at this seat and were 0/42
   translogs in wave-32. No pool deck runs Equipment.

**Narrowed probe spec** (only if the owner wants item 1 closed; item 2 needs a roster change, not a
probe): one deck with 4x a cheap Aura plus 8–10 duplicate-named cheap creatures (Memnite, Ornithopter)
and 20 lands, versus a plain creature deck; 2 one-shot GPT games at `timeout 3000`; grep the
translogs for `[attached to: <name> #` and confirm the handle matches the host's battlefield handle.
Delete the probe decks afterwards; preserve decklists, translogs and report under
`strategy-design/wave33/probe-attachments/`.

---

## N-36l — ATTACHMENT MISATTRIBUTION IN THE PILOT'S READ  [LOW — 1/11, self-corrected]

vs105 s32: *"A1 (Ornithopter) deals 0 damage due to the attached Relic Putrescence"* — the aura is on
Mox Jet, not Ornithopter. The pilot self-corrected mid-reply and the decision (exclude the
`[deals 0]` body) was right for an unrelated and correct reason. Recorded as a rate (1/11), not a
render defect: the render is unambiguous.

---

## THE COMMIT-COUNTER FIELDS, AND WHAT THEY MISS  [instrument — routes to synthesis]

Counted fields at this seat, verbatim per the brief:

- **`post_plan_overrun` > 0: 25/201 (12.4%).** Mean of the nonzero: **2,275**. Max **7,777**.
  Full set: 7777, 7417, 7312, 6716, 5787, 3997, 3653, 3554, 1401, 1338, 1308, 1267, 993, 890, 787,
  637, 428, 401, 327, 299, 262, 178, 66, 59, 17.
- **`commit_retracted`: 0/201. Of the 25 overruns, retracted 0, fatal 0.**
- **`pruned_pairs`: never written.**
- **Replies with no `PLAN:` line: 6/201**, all compliant under `f46dd58ee`, all scoring 0 by design.

**The gap.** 8/201 replies (4.0%) emitted a coded answer, then a DIFFERENT coded answer later in the
same reply. All 8 scored `post_plan_overrun` 0 and `commit_retracted` false — correctly by spec, and
uselessly for the question being asked. `postPlanOverrun` measures the tail past the first `PLAN:`;
`commitRetracted` fires only on the three fallback exits. Since the protocol puts `PLAN:` LAST, a
reply that re-answers and then plans is invisible to both. See `general-suggestions.md` for the
one-line third field.

---

## DECK CONSTRUCTION FLAGS (roster owner, not fixable by guide)

- **Akroma's Memorial {7} is VINDICATED and the wave-32 flag is WITHDRAWN.** It resolved for the
  first time this corpus (vs146 s18, turn 9), granted the whole team seven keywords, and **won that
  game**. Wave-32's "the deck supports at most one, and arguably zero" was written on a corpus in
  which it was never castable. Keep 2.
- **2x Vedalken Archmage {2}{U}{U}** remains the only nonland non-artifact card and the only
  double-blue cost in a five-colour artifact-land manabase. **Never cast in 12 games across two
  corpora.** This is now a two-corpus null; it is the deck's weakest slot.
- **Coiled Tinviper (1 copy)** also never cast in 12 games.
- Shell health is otherwise strong: the seat has now killed or been ahead in every game it was not
  clock-adjudicated out of.

---

## FALSIFIABLE PREDICTIONS FOR THE WAVE-34 REVIEWER

1. **The sacrifice section fires:** at a `SACRIFICE ONE OF YOUR OWN CARDS` window, no reply exceeds
   60 s or 3,000 chars, and no reply asks what "of exactly N" means (baseline: 16 windows, 4 replies
   over 90 s, 2 asking; worst 137 s / 9,127 chars).
2. **RULE #1's retirement did no harm:** `Cast nothing right now` stays at 0 taken, and no reply
   contains a cost-confabulation (baseline 0/45 and 0/201 — this is a NON-REGRESSION prediction and
   any nonzero means the demotion was wrong and the block comes back).
3. **RULE #2 stays unexercised:** 0 replies containing a "cannot pay {1}" derivation.
4. **Master read:** every plan naming a Master cast still states one HIGHER than the printed
   `(N/N)` — **unless** N-36c shipped, in which case the guide's "add 1" block must be DELETED that
   wave and the prediction inverts.
5. **Academy windows:** upkeep activations stay 0; opponent's-turn activations stay 0.
6. **PRESERVE — must NOT regress:** attackers stay 30/30 against the non-`[deals 0]` eligible set;
   infect blocker windows stay at 100% blocked while the poison total is under 8; mulligans stay
   0–2 per game. Any drop here is the guide doing harm and the relevant block comes back out.
7. **Scarecrone draw:** the `{1}, Sacrifice a Scarecrow: Draw a card` line is now in the guide;
   expect its use to stay at or above the unprompted baseline (3 activations in 6 games) and to stop
   appearing at 3 life as the only action of a lost turn.
8. **N-36i:** if `[land]` ships on the battlefield line, zero replies recount the artifact total
   against the battlefield list (baseline 2).
