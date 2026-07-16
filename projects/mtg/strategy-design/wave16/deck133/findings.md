# Wave 16 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: PRIMARY `matchups-20260716-084532` (21 games round-robin over 44,110,133,135,17,21,62).
Binary `/tmp/wagic-69793930a` (wave-15 engine batch: free alternative casts at zero mana +
stale_echo option-subset-of-echo fallback matcher + hybrid-pip affordability annotation). Spark
@0.75. deck133 was the GPT seat in 6 games. **0 segfaults / 0 empty_reply corpus-wide (5th clean
crash-fix corpus).**

**Real record 4W / 2L (up from 2W/4L in wave-15).**

| opp | result | end my/opp | last T | mode | note |
|-----|--------|-----------|--------|------|------|
| 135 | **WON** | 10 / **-7** | 14 | clean kill | beatdown; Fatal Push -> Ice-Fang Coatl, Bloodghast/Geralf beats closed it |
| 110 | **WON** | **16 / 0** | 13 | clean kill | **REVERSAL vs wave-15 loss**: drew Fatal Push, killed Signal Pest, ground affinity out |
| 21  | **WON** | 2 / **-2** | 14 | clean kill | **REVERSAL vs wave-15 loss**: won the Goblin race; ~20 own-target-FP windows all declined |
| 62  | **WON** | **18 / 2** | 12 | clean kill | NEW opp (Enchantresses); Fatal Push + Liliana + Bloodghast/GM-body beats |
| 17  | LOST | **-1 / 20** | 11(adj) | 0 dmg dealt | pure no-reach vs Faeries — opp stayed at 20 ALL game; GM never drawn (structural, identical to wave-15) |
| 44  | LOST | **-4 / 18** | 12 | clean kill vs it | **REGRESSION vs wave-15 win**: UB control grind; self-damage suite + no closer drawn; soft Thoughtseize-past-T3 slips (finding 4) |

**Verdict on the 2/6:** BOTH losses are the deck's known structural face under life-adjudication.
vs17 is pure no-reach (dealt literally ZERO damage all game; opp_life trajectory flat 20 across all
28 logged decisions). vs44 is the self-damage-suite exposed by a no-closer draw: deck133 bled
20->8 largely on repeated Thoughtseize (2 life each) + fetch against a control deck that only fell
to 18, with Obliterator offered just 2x and Gray Merchant never castable. **No hard deciding
decision error in either loss.** The four wins include TWO clean reversals of wave-15's structural
losses (vs110 artifact-flyer aggro, vs21 Goblin swarm) — both flipped purely on drawing removal /
winning the race, not on any guide change. **Guide FROZEN byte-identical (sha1 7558c0ff...,
11th+ consecutive freeze).**

---

## FINDING 1 — [ITEM A: removal cast offered with only friendly legal targets] — NO RECURRENCE. Route: ENGINE/REPRESENTATION lever WEAKENED. PASS.

My primary charge. Wave-15's finding 2 was a single self-Fatal-Push (vs21 s23) via a card-presence
hallucination: an "only legal targets are YOUR OWN" Fatal Push cast that destroyed the pilot's own
Bloodghast (1/11 own-target windows disobeyed). **This wave the annotation held 100%.**

The `Cast Fatal Push {b} - the only legal targets are YOUR OWN right now` option was offered at
**~20 windows** (vs135 s14/s15/s16; vs21 s10/s12/s14/s17/s19/s22/s25/s27/s29/s32/s34/s35/s37/s39/
s40/s41). **The model chose the own-target Fatal Push at ZERO of them** — every window resolved to
"Cast nothing right now" or a real spell (Thoughtseize/Inquisition/Bloodghast/Collective Brutality).
The one wave-15 misfire did NOT recur.

**Consequence for the wave-15 engine-suppression lever (notes #A last wave — "suppress a removal
cast whose only legal targets are friendly"):** this wave gives it a 0/20 recurrence rate. The
annotation-obeyed rate is now 30/31 across the two corpora combined. The durable-suppression lever
is still *architecturally* sound (it would make the failure structurally impossible), but its
empirical support has shrunk to a single non-deciding wave-15 firing — **downgrade it from a
build candidate to a parked WATCH** (notes #A). Do not lengthen the guide for it (present-and-obeyed).

---

## FINDING 2 — [ITEM B: stale_echo superstring fix] — NO WRONGFUL SUPERSTRING DOWNGRADES; Yawgmoth-attack shape got NO direct exercise. Route: HARNESS. PASS (with a scope caveat).

My second charge. Wave-15's finding 3 was a wrongful stale_echo downgrade at vs140 s37: echo
`CHOICE: 1 (Attack with Yawgmoth, Thran Physician)` was a SUPERSTRING of the offered short option
`Attack with Yawgmoth`, so an equality/substring key match failed and the reply was downgraded to
the heuristic. The wave-15 batch shipped the option-subset-of-echo fallback matcher to fix exactly
this.

**Result this corpus:**
- A full scan for the wave-15 shape (echo names an offered option only as a proper superstring, and
  was still downgraded to choice=-1) found **ZERO instances**. No wrongful superstring downgrade
  survived at this seat.
- **BUT the exact Yawgmoth-attack shape got NO direct exercise this corpus.** deck133 had no
  attack-with-a-multiword-named-creature window at all — the only Yawgmoth-string options were land
  drops (`Play/Hold Urborg, Tomb of Yawgmoth`, vs62 s36, vs44 s6), which the model echoed exactly
  and parsed cleanly. So the superstring matcher is "not-exercised for its signature shape, no
  regressions observed" — consistent with the brief's corpus-wide note that several wave-15 fixes
  got little exercise. Not-exercised != not-fixed; the code path had no wrongful firing to catch.
- **The two stale_echo events this seat are BOTH genuine catches, neither a superstring
  false-positive:**
  - **vs17 s11 (T2):** options were two identical `Briarberry Cohort ... [opponent's hand]` entries
    (a Thoughtseize discard-selection sub-menu); echo `CHOICE: 1 (Target opponent reveals their hand
    for Thoughtseize)` names an action absent from the menu — the model was reasoning at the
    cast-Thoughtseize step while the engine had advanced to the discard-pick step. TRUE catch; the
    heuristic completed the discard cleanly.
  - **vs62 s29 (T8):** see finding 3 — a genuine "echo names no offered option" case, but the option
    LABELS were degenerate (`Becomes `), a serialization defect distinct from the superstring class.

**Item B verdict:** the superstring fix caught no wrongful downgrades because none occurred; the
class it targets appears closed. Carry the Yawgmoth-attack shape as still-awaiting a direct live
witness (a future corpus with a Yawgmoth attacker will confirm the *positive* match).

---

## FINDING 3 — [NEW, LOW, non-deciding] stale_echo via DEGENERATE option label `Becomes ` (Liliana +1 target sub-menu). Route: HARNESS/SERIALIZATION (notes #B). NOT the superstring class.

**vs62 seq29 (T8, my13/opp20).** Options serialized as `['Becomes ', 'Decline - do nothing']`. The
reply is a lucid, CORRECT plan: activate Liliana, the Last Hope's `+1` to give Canopy Spider -2/-1
(neutralizing the 1/5 trample-reach attacker), closing `CHOICE: 1 (Activate Liliana's +1 ability on
Canopy Spider)`. The echo names no option because **option 1's label is the truncated/degenerate
string `Becomes ` — the target-selection sub-menu rendered the stat-change with no creature name and
no stat detail.** The echo cannot match `Becomes `, so the reply downgraded to stale_echo ->
heuristic (which added the loyalty counter anyway — events: `Counter added to Liliana, the Last
Hope` — so the intended +1 fired regardless; non-deciding).

This is NOT item B (not a superstring — the option label carries no usable name at all) and NOT item
A. It is a serialization defect: a target sub-menu whose option labels collapse to a bare `Becomes `
prefix. Any deck running Liliana, the Last Hope's +1 (or similarly-worded stat-change target menus)
will hit it. The fix lives at the harness/serialization layer (emit the full option label with the
target creature + resulting stats), not at the guide. Single-seat, LOW, carry as a WATCH (notes #B).

---

## FINDING 4 — [MED, non-deciding-in-isolation, CONSTRUCTION + soft guide-adherence] vs44 loss: repeated post-T3 Thoughtseize into a no-closer draw. Route: CONSTRUCTION (notes #D) + sanctioned-against guide slip.

vs44 is the wave's one record regression (wave-15 WON, wave-16 LOST). The mechanism is entirely the
deck's structural self-damage face, with a soft guide-adherence slip contributing:
- deck133 cast Thoughtseize at **T3 (s10, took Counterspell — fine, on-curve), T5 (s15), and T9
  (s26)** — three casts, -2 life each. Life fell 20->19->16->13->9->8 while opp only reached 18.
- **s15 (T5, my16):** the model's own reasoning acknowledges "Casting Thoughtseize costs 2 life,
  which is risky" but its only castable spells were two Thoughtseize (no creature in hand) — the
  guide's remedy ("Prefer Inquisition or a creature") was unavailable, and the guide-correct play
  was "Cast nothing." It cast Thoughtseize anyway. Soft slip.
- **s26 (T9, my13):** the threat was a 4/4 flying Archmage of Echoes ALREADY on the battlefield —
  Thoughtseize (a hand-strip) cannot answer it. The model correctly diagnosed the flyer but still
  cast Thoughtseize (-2 life) that did nothing about it. This is precisely the guide's anti-pattern
  ("If the opponent's hand shows nothing that threatens you, DO NOT cast Thoughtseize - its 2 life
  is a race you lose").

Neither is a HARD deciding error (Obliterator offered only 2x and never resolved; Gray Merchant
never castable — the game was lost on the no-closer draw against a control deck deck133 can't race).
But the repeated post-T3 self-inflicted Thoughtseize is the self-damage-suite construction flaw
(notes #D) surfacing, and it is a present-and-disobeyed guide line (Thoughtseize thin/late rule) ->
**sanctioned-against escalation; the durable fix is construction (a Thoughtseize->Inquisition shift),
not more guide text.** KEPT verbatim; logged.

---

## VERIFIED-GOOD (no regression; wave-11 termini hold)

- **SELF-FATAL-PUSH WATCH — CLEAN.** All THREE real Fatal Push casts this corpus targeted an enemy
  creature over an available friendly option and resolved as a kill: vs110 s7-11 -> `Opponent's
  Signal Pest: battlefield -> graveyard`; vs135 s17-19 (targets Bloodghast/Ice-Fang Coatl) ->
  correctly picked Ice-Fang Coatl -> `Opponent's Ice-Fang Coatl: battlefield -> graveyard`; vs21
  s8-10 (targets Bloodghast/Raging Goblin) -> correctly picked Raging Goblin -> `Opponent's Raging
  Goblin: battlefield -> graveyard`. **ZERO self-destroys, ZERO fizzles at any FP window this
  corpus** (improvement over wave-15's one vs21 s23 self-FP). The wave-13 stack-targeting self-FP
  also did not recur.
- **Impact-cast / beats-are-the-win line vindicated.** All four wins closed on BEATS, not a Gray
  Merchant cast (GM was never cast as a spell this corpus — vs62 it entered and attacked as a 2/4
  body). Matches the guide's "Half your games you win by BEATS ... the beats ARE the win."
- **Gray Merchant wait-rule obeyed.** The one GM cast-option window (vs62 s32, drains 8, opp18/my12)
  correctly did NOT get force-cast (N<opp, my>10 -> WAIT); the model rambled the devotion count
  (finding 5) and the heuristic held, guide-consistent.
- **Fetch / Liliana / Yawgmoth / Collective Brutality coexistence clean** — no re-ask inflation, no
  no-legal-target loops.

## FINDING 5 — [LOW, non-deciding] GM devotion-ramble triggers the ramble guard. Route: MODEL (guide already forbids). Watch.

**vs62 seq32 (unparsed_reply, choice=-1).** GM offered `{right now: drains 8}`. The guide is
explicit: "do NOT try to count your own devotion by hand, you will miscount; the option already did
the counting for you." The model IGNORED this and wrote a long manual devotion count ("5 Swamps ...
Bloodghast x2 = 4 {B} symbols ... Liliana ...") and never emitted a CHOICE line -> ramble guard ->
heuristic. Non-deciding (waiting on GM at N8<opp18/my12 was guide-correct anyway). The guide line
already forbids the hand-count, so this is present-and-disobeyed -> no new guide text. Carry as a
MODEL watch: the "use the shown number directly" instruction is disobeyed under devotion-math
temptation. If it recurs at a DECIDING GM window (where a miscount would flip a win/stabilize call),
re-escalate.

## MANDATORY AUDITS

- **Fallbacks (5 this seat): 2 stale_echo + 3 unparsed_reply**, all choice=-1, heuristic played,
  NONE deciding. Breakdown — stale_echo: vs17 s11 (TRUE, discard-step confusion), vs62 s29 (TRUE but
  degenerate-label, finding 3). unparsed_reply: vs21 s30 (ramble, no CHOICE), vs62 s32 (GM
  devotion-ramble, finding 5), vs62 s39 priority (2nd-main plan-ramble after combat already
  executed, harmless). **empty_reply = 0** (Spark-death class stays CLOSED, 5th clean corpus).
  **Verified no self-Fatal-Push and no self-destroy resolved at any fallback window.**
- **vs17 LOSS — pure no-reach (structural).** opp_life flat at 20 across all 28 logged decisions;
  deck133 dealt ZERO damage all game, ground the Faerie board but had no flyer answer and never drew
  Gray Merchant. Not a misplay. Carried wave-9..15.
- **vs44 LOSS — self-damage-suite + no-closer (construction).** See finding 4. No hard deciding
  error; the wave-15 win of this matchup was a better draw, not a better guide.

## FREEZE-CHECK (every kept line accounted for; ZERO edits)
Live guide BYTE-IDENTICAL to wave-11 output (`cmp` clean; sha1 7558c0ff...). wave16/deck133/
strategy.txt is a verbatim copy of the live guide.
- **Identity + beats-are-the-win + impact-cast list** — vindicated (all 4 wins on beats). KEPT.
- **own-targets Fatal Push annotation** — obeyed 20/20 this corpus (finding 1). KEPT.
- **GM block (bare-N, use-shown-number-directly, wait-vs-cast)** — wait-rule obeyed (vs62 s32); the
  "don't hand-count devotion" line was disobeyed once, non-deciding (finding 5) -> sanctioned-against,
  KEPT.
- **Thoughtseize thin/late rule** — soft-disobeyed twice vs44 (finding 4) -> sanctioned-against,
  construction fix, KEPT verbatim.
- **DEFENDER/Obliterator-wall, VS RED/BURN, Bloodghast, Collective Brutality, Liliana, fetch,
  Yawgmoth, Geralf's, Arena, mulligan** — obeyed or untested; losses structural. KEPT VERBATIM.
Nothing dropped. **Only change this wave is ZERO.**

## In my words
The two numbers I owned came back clean and, better, improving. Item A — casting removal at your own
creatures — did not recur once across roughly twenty windows where the engine offered the pilot a
Fatal Push whose only legal targets were friendly; the model declined every single one, so the
wave-15 misfire looks like a one-off hallucination rather than a standing failure, and the
engine-suppression lever I proposed last wave can drop from "build" to "parked." Item B — the
superstring stale_echo false-positive — produced zero wrongful downgrades this corpus, but its
signature Yawgmoth-attack shape simply never came up, so the fix is validated by absence-of-
regression rather than by a positive match I can point to. The self-Fatal-Push watch is clean: all
three real Push casts correctly killed an enemy creature over an offered friendly target. The record
climbed to 4/6 with two clean reversals of last wave's structural losses, on better draws, not a
changed guide. The two remaining losses are the deck's face I keep writing down — no reach against
Faeries, and a self-damaging discard suite that bleeds it out of grindy control games with no closer
drawn. The one genuinely new thing is small and harness-shaped: a Liliana +1 target sub-menu whose
options serialize to a bare "Becomes " with no creature or stats, which correctly but needlessly
tripped the stale_echo route. The guide freezes clean again.
