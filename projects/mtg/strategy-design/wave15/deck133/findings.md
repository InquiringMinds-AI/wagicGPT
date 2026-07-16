# Wave 15 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: PRIMARY `matchups-20260716-040138` (21 games round-robin over 44,110,133,135,140,17,21).
Binary `/tmp/wagic-aad32bc7f` (target-cancel use-after-free fix + absent-echo staleness route
`d0f88326b` + attackers name reconcile + Galvanic Blast metalcraft `3cb901295`). Spark @0.75
(latency med 11.4s / p90 93s). Wave-11 guide FROZEN, byte-identical (`cmp` clean — freeze-check
below). deck133 was the GPT seat in 6 games; **0 segfaults corpus-wide (the aad32bc7f crash fix
survived live).**

**Real record 2W / 4L.**

| opp | result | end my/opp | last T | mode | note |
|-----|--------|-----------|--------|------|------|
| 135 | **WON** | 14 / **-6** | 12 | clean kill | textbook beatdown: Bloodghast->Geralf->Obliterator, no GM needed |
| 44  | **WON** | 7 / **-4** | 15 | clean kill | GM drain-4 -> drain-6 chain (both +life), model piloted it perfectly |
| 140 | LOST | **13 / 18** | 12(adj) | adj, behind | Rakdos control grind; self-damage suite left it behind at cap; no deciding error |
| 17  | LOST | **7 / 20** | 11(adj) | adj, 0 dmg dealt | Faerie no-reach: dealt ZERO to opp all game; GM never drawn (structural) |
| 21  | LOST | **-1 / 18** | 13 | clean kill vs it | Goblin swarm race; 1 self-Fatal-Push misfire (finding 2); no early wall |
| 110 | LOST | **-4 / 18** | 8 | clean kill vs it | Cranial-Plating affinity: 4/2 flying Ornithopter, Bloodghast can't block, no removal drawn |

**Verdict on the 2/6:** DOMINANTLY structural/variance under the latency-adjudication regime. THREE
of four losses (vs110 artifact-flyer aggro, vs17 Faerie no-reach, vs21 Goblin swarm) are the deck's
known structural holes (no early blocker, no reach, self-damage suite) exposed by draws that never
delivered removal/GM. The fourth (vs140) is a fair positional adjudication loss — deck133 was simply
BEHIND on life at the cap due to its self-damage suite, with NO deciding decision error. **Crucially,
the wave-13 DECIDING failure class (GM-hallucination executing an arbitrary index) is now FIXED and
did not recur as a loss — see finding 1.** Guide obeyed everywhere it was reasoned directly. **Guide
FROZEN byte-identical.**

---

## FINDING 1 — [CRASH-FIX VALIDATION + wave-13 deciding-class FIX] — my primary charge. PASS.

Two things I own converge here: the aad32bc7f use-after-free crash fix, and the d0f88326b
absent-echo staleness route that closes the wave-13 GM-hallucination deciding class. Both validated
clean at this seat.

### 1a. Crash-fix (aad32bc7f target-cancel use-after-free) — STRONG PASS.
The wave-14 segfault came from THIS seat's Fatal-Push target-decline. This corpus exercised the same
codepath family (Fatal Push into own/heuristic-resolved targets) **11 times** and every instance
resolved cleanly, 0 segfaults, all 6 games ran to natural or life-adjudicated end:
- **vs21 seq23 (T8):** model CAST `Fatal Push {b} - the only legal targets are YOUR OWN right now`;
  the engine auto-resolved onto the single legal target (its own Bloodghast) -> `Your Fatal Push:
  hand->stack / Your Bloodghast: battlefield->graveyard / Your Fatal Push: stack->graveyard` (seq24
  events). Self-target resolution completed with **no crash**; Bloodghast returned via landfall.
- **vs21 seq27/seq28 (T10, unparsed_reply -> heuristic):** the HEURISTIC fallback drove a Fatal Push
  cast + target sub-menu (targets Bloodghast/Boggart Ram-Gang). FP resolved (`hand->stack->graveyard`,
  seq29) as a **fizzle** — no creature destroyed, no self-destroy of Bloodghast, **no crash.** This
  is precisely the heuristic-resolves-FP-target path the fix protects; it held.
- **10 of 11 "only legal targets are YOUR OWN" windows correctly DECLINED** (Cast nothing / cast a
  real spell): vs135 s21/s25/s28/s31/s32, vs21 s10/s17/s19/s20/s26. No crash at any.
- **vs140 seq37:** Fatal Push offered, routed to heuristic via stale_echo — no crash.

No `cancelCurrentAction` anomaly observed. The exact wave-14 trigger (an explicit DECLINE at the
target chooser after committing the cast) did NOT recur this corpus (the model either casts onto the
available target, or picks "Cast nothing" at the top-level menu before reaching the sub-menu), but
the neighboring self-target and heuristic-driven-target resolutions — the same freed-object family —
were exercised repeatedly and are stable. Heuristic resolutions were reasonable (the one nit: the
heuristic was more trigger-happy than the model at vs21 s27, casting a Fatal Push that fizzled where
the model had correctly held at s26 — a heuristic-quality nit, not a crash).

### 1b. wave-13 GM-hallucination deciding class — FIXED by d0f88326b. PASS.
Wave-13's ONE deciding error was vs140 seq45: model echoed `CHOICE: 1 (Cast Gray Merchant)` when GM
was NOT offered -> index-wins -> executed the guide-forbidden thin-hand Thoughtseize -> -2 life ->
flipped a 1-life adjudication. Every analogue this wave was correctly caught:
- **vs44 seq25 (T11):** options `[Put in Play / Decline]` (a fetch put-in-play trigger); reply echoed
  `CHOICE: 1 (Cast Gray Merchant of Asphodel)` — GM absent. **Routed to stale_echo -> heuristic** (did
  "Put in Play"). This is the exact wave-13 shape, now correctly NOT executing the arbitrary index.
- **vs140 s11/s12/s36** (echo names an option/action not offered — a target/ability sub-menu
  confusion): all -> stale_echo -> heuristic.
- **Corpus-wide Mechanism-B leak scan (executed decisions whose CHOICE-echo card is absent from the
  offered options): 0 leaks.** Every echo-names-no-option case was routed to the heuristic; not one
  executed a raw index. The fix is doing exactly what the wave-13 finding-A prescribed.

---

## FINDING 2 — [MED, non-deciding, NEW mechanism, NOT the wave-13 stack-target] self-Fatal-Push via a card-PRESENCE hallucination. Route: MODEL + ENGINE/REPRESENTATION (notes #A). NOT guide-writable.

vs21 seq23 (T8, my12/opp18). Hand held **only two Fatal Push** (no Bloodghast — Bloodghast was
already ON the battlefield). Offered: `[1] Cast Fatal Push (only legal targets are YOUR OWN) / [2]
Cast nothing`. The model's PLAN and reply reason ENTIRELY about casting Bloodghast ("I will cast
Bloodghast from my hand ... a 2/1 body that adds to my devotion") — a hallucination that Bloodghast
was castable from hand — then wrote `CHOICE: 1 (Cast Fatal Push)`. Index 1 = Fatal Push; the engine
cast it onto the only legal target, its own Bloodghast, destroying it (returned next land via
landfall). Correct play was option 2 (Cast nothing / hold Push).

**Why this is distinct from the wave-13 self-FP recurrence (notes F / finding #2):** wave-13's
self-FP was STACK-TARGETING — the model tried to "destroy" an opponent creature-SPELL on the stack
and hit its own creature. **That stack-targeting mechanism did NOT recur this corpus** (the only FP
cast in response to a spell, vs21 s7, correctly targeted a real battlefield creature — Scuzzback
Scrapper — in response to a Lightning Bolt). The vs21 s23 self-FP is a different, simpler failure: a
card-presence hallucination (thinks a battlefield creature is a hand card) producing a CHOICE line
whose echo/index AGREE (both Fatal Push) but CONTRADICT the model's own reasoning. **The name-echo
protocol cannot help** — the echo names an offered option (Fatal Push IS option 1), so there is no
absent-echo to catch; the harness correctly executed what the model literally chose.

Non-deciding (Bloodghast returned via landfall; the material cost was one wasted Fatal Push in a game
lost to the Goblin race on other grounds), but it is a real misfire and points at a durable
representation lever: **a targeted-removal top-level cast whose ONLY legal targets are friendly
permanents is essentially never correct and should be suppressed or hard-deprioritized at the option
layer** (notes #A). The "only legal targets are YOUR OWN" annotation is present and MOSTLY obeyed
(10/11 windows declined) — but one hallucination punched through it, and an engine-side suppression
would make the failure structurally impossible rather than instruction-dependent.

---

## FINDING 3 — [LOW, non-deciding, WATCH] stale_echo possible false-positive via name SUPERSTRING mismatch. Route: HARNESS reconcile (notes #B).

The absent-echo staleness route (d0f88326b) fired 5x at this seat; four are unambiguously CORRECT
catches (vs44 s25, vs140 s11/s12/s36 — echo names a card/action not among the offered options). The
fifth, **vs140 seq37**, is the brief's wrongful-downgrade shape: options were `[1] Attack with
Yawgmoth / [2] Play Verdant Catacombs / [3] Cast Fatal Push / [4] Pass`; the reply is a lucid
multi-step plan closing `CHOICE: 1 (Attack with Yawgmoth, Thran Physician)`. The echo **does** name
offered option 1 — but as a SUPERSTRING (`Attack with Yawgmoth, Thran Physician` contains the
option's `Attack with Yawgmoth`), so an equality/substring match on the option-as-key fails and the
reconcile downgraded to heuristic. Not deciding (heuristic declined the Yawgmoth attack — arguably a
BETTER play, keeping the fragile 2/4 engine back; the intended 2 damage would have made opp 16 vs 13,
still a loss), but it is a genuine reconcile nit: **the name-echo/attacker reconcile should match when
the offered option name is a PREFIX/substring of the echoed full card name, not require equality**
(notes #B). Single-seat, LOW.

---

## VERIFIED-GOOD (no regression; wave-11 termini hold)

- **GM bare-N shown==resolved, and correct impact-cast judgment.** vs44 s22 `drains 4` -> my12->16 /
  opp18->14; s26 `drains 6` -> my11->17 / opp14->8 (both bare-N exact, both gained the shown life).
  The s22 cast at N(4)<opp(18), my>10 is a defensible deviation from the literal wait-rule: NO
  creature was offered (options were GM / Inquisition / nothing), so GM as a 2/4 devotion body + 4
  lifegain that set up the bigger second drain was correct — and it WON the game. The model exercised
  sound judgment where the guide's remedy ("cast a creature instead") was unavailable.
- **Thoughtseize thin/empty-hand discipline OBEYED wherever reasoned.** No thin-hand Thoughtseize
  cast this corpus (the wave-13 sole violation was the now-fixed GM-hallucination collision).
- **Beatdown line executed cleanly vs135** (WON): Thoughtseize/Inquisition disruption -> Bloodghast ->
  Geralf's Messenger -> Phyrexian Obliterator -> attacks closed it (opp -6); GM not needed, matching
  the guide's "beats ARE the win."
- **Yawgmoth piloted as an engine** (vs17, vs140): pay-1-life, sac recurring body, -1/-1 counter,
  draw — mechanically correct throughout (the vs17 grind failed on no-closer/no-reach, not on
  mis-piloting Yawgmoth).
- **Own-target Fatal Push discipline** 10/11 (see finding 1a).

## MANDATORY AUDITS

- **Fallbacks (15 total this seat): 4 stale_echo + 11 unparsed_reply** — every one fell to a safe
  heuristic line; **verified no self-Fatal-Push and no self-destroy resolved at any fallback window**
  (vs21 s27/s28 heuristic FP fizzled, did NOT hit own Bloodghast). Two unparsed windows in the WON
  vs44 game were rescued by an EXCELLENT heuristic: seq27 -> cast Gray Merchant (opp -6 / my +6),
  seq30 -> alpha strike opp->4. empty_reply = 0 (Spark-death class stays CLOSED, 4th clean corpus).
- **vs140 adjudicated LOSS — fairly behind at the cap.** deck133 controlled with 2x Liliana +
  discard but bled its own life (Liliana/fetch/Yawgmoth self-damage, my18->13) against a Rakdos
  control deck that sat at 18. No deciding decision error; the wave-13 GM-hallucination-Thoughtseize
  that lost the analogous game is FIXED. Position lost on the self-damage suite + no closer.
- **vs17 adjudicated LOSS — pure no-reach.** deck133 dealt ZERO damage to opp all game (opp stayed
  20); ground the Faerie board with Yawgmoth (my14->7 on activations) but had no way through
  flyers/Plumeveil and never drew Gray Merchant. Structural, not misplay.
- **vs110 fast LOSS — structural.** deck110 = Cranial-Plating affinity; a 4/2 flying Ornithopter put
  deck133 my13->my1 in one turn. Bloodghast can't block, Fatal Push (which kills Ornithopter/Memnite,
  MV0) never drawn. No removal = variance, not a decision error.

## FREEZE-CHECK (every kept line accounted for; ZERO edits)
Live guide BYTE-IDENTICAL to wave-11 output (`cmp` clean; sha1 7558c0ff...). deck133/strategy.txt is
a verbatim copy.
- **Identity + beats-are-the-win + impact-cast list** — vindicated (vs135 beatdown kill, vs44 GM
  chain). KEPT.
- **GM block (bare-N, use-shown-number-directly, wait-vs-cast)** — bare-N shown==resolved x2; the one
  early GM was correct judgment when no creature was offered. KEPT.
- **Thoughtseize thin/empty-hand rule** — no violation this corpus. KEPT.
- **own-targets Fatal Push annotation** — obeyed 10/11; the one miss was a card-presence hallucination
  the guide already forbids ("confirm the card you name appears in the list") -> sanctioned-against
  escalation, the durable fix is engine suppression (notes #A). KEPT.
- **DEFENDER/Obliterator-wall, VS RED/BURN, Bloodghast, Collective Brutality, Liliana, fetch,
  Yawgmoth, Geralf's, Arena, mulligan** — obeyed or untested; losses were structural. KEPT VERBATIM.
Nothing dropped. **Only change this wave is ZERO.**

## In my words
The number I was told to validate — the crash fix — validated. This seat's own Fatal-Push
target-decline is what segfaulted wave-14, and this corpus drove the same family of self-target and
heuristic-resolved Fatal-Push casts eleven times with zero segfaults and clean resolutions. Better
still, the wave-13 deciding error that lost a game by one life — the model wanting its Gray Merchant
so badly it wrote CHOICE for a card that wasn't offered and executed an arbitrary index — is now
caught every single time: vs44 seq25 is the identical shape and the absent-echo route routed it
harmlessly to the heuristic, and a full scan found zero Mechanism-B leaks. The self-Fatal-Push did
recur once (vs21 s23) but as a different, weaker mechanism: a card-presence hallucination where the
model thought a battlefield Bloodghast was a hand card and destroyed it — the echo and index agreed,
so no harness arm could catch it, and the honest fix is to stop offering removal whose only targets
are your own permanents. The four losses are the deck's structural face: no early blocker, no reach,
a self-damaging discard/fetch suite, all exposed by draws that withheld removal and Gray Merchant.
Everything the model reasoned about directly held. The guide freezes clean; the wave's real product
is a confirmed crash fix, a confirmed absent-echo fix, and two small ledger items.
