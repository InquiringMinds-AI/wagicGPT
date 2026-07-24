# Wave-23 review — deck109 (Hellrider, mono-red aggro / RDW) — OWNER of batch item (d)

**Corpus:** PRIMARY `matchups-20260724-013710`, round-robin, deck109 seat = 6 games.
Binary `/tmp/wagic-0e6361732` (wave-23 batch; THIS corpus validates it live).
Pool: 27/133/131/137/102/140 + deck109. Guide FROZEN from wave-22
(md5 eef8ebc190fbda22c9e5f057e791ba7d, deployed == wave22/deck109/strategy.txt, cmp-verified).
Second corpus on the wave-22 trimmed guide (the "one confirming corpus before rotate" the
predecessor named).

**Record: 4/6** — beat 131, 137, 102, 140; lost to 27, 133. Context only; decompose by decision.
Pool changed again vs wave-22 (was 27/44/62/102/135/140), so cross-wave record is WEAK.

| opp | result | end (my/opp, turn) | class |
|---|---|---|---|
| 27  | LOSS | -1 / 20, T8  | merfolk evasion — opp stayed at 20 ALL game (Oona's Gatewarden 2/1 flying-defender walled my ground; Inkfathom Infiltrator unblockable + Lord of Atlantis raced me). Untouchable/evasive class. |
| 133 | LOSS | -4 / 11, T13 | raced opp 20->7 by T8, then Gray Merchant of Asphodel devotion-drain swung the race (opp 9->14, drained me). Lifegain/drain race-loss. |
| 131 | WIN  | 20 / 0, T8   | clean race; killed Young Pyromancer with Pillar on sight (exception fired). |
| 137 | WIN  | 12 / 0, T8   | GW Adventures; face-raced through it. |
| 102 | WIN  | 10 / 0, T11  | mono-B devotion; out-raced Gray Merchant. |
| 140 | WIN  | 20 / 0, T8   | RBW control; wide Goblin swarm + Hellrider, opp never stabilized. |

Both losses = evasive/drain matchup ceilings the guide already documents. No decision error of
consequence in either (see loss decomposition at bottom). **Fallbacks: 2/119 at this seat**
(brief per-seat: 109:2 — confirmed): vs27 s13 unparsed_reply, vs140 s19 empty_reply.

---

## OWNED ITEM (d) — ATTACK-line CoT hijack fix: VALIDATED. Wave-22 Ledger A2 NOT reproduced.

Applied the wave-22 A2 scoring rung (ADOPTED as skill Method-headline-4): for EVERY attackers
window, compare the reply's FIRST parseable coded `ATTACK:` line against the eligible-option set
AND the recorded/declared set. **26 attacker decisions across 6 games. First-parseable line honored
in ALL 26; recorded set == the first line's in-eligible-range tokens in ALL 26. ZERO eligible
attacker silently dropped.**

**7 windows where the coded line named MORE tokens than were eligible** — in every one, the extra
tokens were OUT of the eligible set (over-range indices, or named summoning-sick creatures), so
dropping them was correct and outcome-neutral (the wave-22 A1 pattern):

| seq | first coded line | eligible (options) | recorded | dropped tokens eligible? |
|---|---|---|---|---|
| vs27 s10  | `A1, A2, A3, A4`                       | opts=1 (Gore-House)          | Gore-House          | NO (A2-A4 don't exist) |
| vs133 s17 | `A1, Stromkirk Noble`                 | opts=1 (Ash Zealot)          | Ash Zealot          | NO (Stromkirk ineligible) |
| vs133 s21 | `A1, A2, A3`                          | opts=2 (Ash Zealot, Stromkirk)| Ash Zealot, Stromkirk| NO (A3 out of range) |
| vs131 s7  | `A1 (Rakdos Cackler), Goblin, Goblin` | opts=1 (Rakdos Cackler)      | Rakdos Cackler      | NO (Goblins summoning-sick T2) |
| vs137 s11 | `A1 (Stromkirk), Goblin, Goblin`      | opts=1 (Stromkirk)           | Stromkirk           | NO (Goblins just cast) |
| vs102 s16 | `A1, A2`                             | opts=1 (Hellrider)           | Hellrider           | NO (A2 out of range) |
| vs102 s19 | `A1, A2, Goblin x4`                   | opts=2 (Hellrider, Gore-House)| Hellrider, Gore-House| NO (Goblins ineligible) |

**The direct fix-target validation** — first-parseable beats last-wins on SPIRALED attack replies:
- **vs133 s23 (T12, at 2 life, lat 103s, 6969 ch): the reply emits `ATTACK: A1, A2` THREE times**
  (a mild repeat-spiral), first-parseable took the first, recorded `Stromkirk Noble, Gore-House
  Chainwalker` (both eligible) — EXACT. Under the retired last-wins rule the terminal repeat would
  also have been A1,A2 here, but any later waffled/prose "Attack:" line could have hijacked it; the
  first-wins rule removes that exposure.
- **vs102 s19 (T11, lat 56s, 3842 ch)** and **vs137 s7 (T2, lat 157s, 9841 ch)** — both long
  spirals; first coded line honored, recorded set correct.
- **0 combat-math "Attack: Deal N..." prose lines appeared in any reply this corpus** (grep of every
  attackers reply = 0), so the exact prose-hijack shape the fix names did not occur to be caught in
  the act — but the deeper defect (a valid answer-first coded line being overridden by a
  differently-counted salvage subset, wave-22 vs62 s21) is what mattered, and it is **NOT
  REPRODUCED**: no prose-salvage override observed in 26 windows. **Ledger A2 = CLOSED, fix (d)
  validated at this seat.**

Note on the intentional asymmetry (routes to notes/skill, not a defect): ATTACK = first-parseable-
wins; CHOICE = last-well-formed-wins (harness's deliberate mind-change affordance). Correct by
shape — a CHOICE picks one of mutually-exclusive options and benefits from mid-reply self-correction
(validated repeatedly, e.g. wave-22 vs62 s16); an ATTACK declares a SET and rarely benefits from
reversal, while it is uniquely exposed to prose "Attack:" hijacks. First-wins trades a low-value
affordance to kill a high-value hijack. No first-wins downside observed (all 26 first lines correct
or a superset).

---

## TRIMMED-GUIDE ROUND 2 (crutch-retirement validation) — PARTIAL REGRESSION on Pillar cost

The wave-22 revision retired three representation crutches (engine now carries the fact): the hybrid-
mana lecture (Rakdos Cackler), the Pillar cost-correction ("{R}, not {1}{R}"), and demoted the Boros
mana explanation. Round-2 result is SPLIT:

**HYBRID-MANA retirement HOLDS (confirmed).** Zero hybrid-confusion markers ("lacking black" / "no
black" / "need black" / "black mana") in any reply. Rakdos Cackler cast 3x (vs27 s8, vs131 s3/s9),
all off Mountains with the `[hybrid: each {b/r} pays with B or R - total 1 mana]` annotation present.
Unleash "1/1 Counter" taken 5/5 (vs27 s5/s9, vs133 s20, vs131 s4, vs102 s13). The engine annotation
fully substitutes for the retired lecture. -> keep retired.

**PILLAR COST-CORRECTION retirement REGRESSED — restore a lean line (guide revision applied).**
- **vs137 s12 (T4, Main phase 2):** Mana available `{r}` (1 red). Hand: 2x Searing Spear {1}{r},
  Pillar of Flame {r}. Option list shows `Cast Pillar of Flame {r}` (correct cost). The model chose
  **"Cast nothing right now"**, reasoning verbatim: *"I have only {R} available, but both Searing
  Spear and Pillar of Flame require {1}{R} (two mana total). I cannot afford to cast them."* It
  **conflated Pillar's {R} with Searing Spear's {1}{R}, ignored the correct option-line cost, and
  declined an affordable Pillar** — a missed 2 damage at the opponent's face (14->12) or a kill on
  their Edgewall Innkeeper card-draw engine.
- The mis-costing is **stochastic, not gone**: in vs131 s11/s13 the SAME model costed Pillar
  CORRECTLY ("cost is {R}, yes"). So the wave-22 "0 mis-costings" was a one-corpus artifact; under a
  different draw the conflation recurred. This is the precise failure the retired "{R}, not {1}{R}"
  crutch existed to prevent.
- Non-decisive (vs137 won convincingly) but a clean decision ERROR attributable to the retirement.
  **Route: GUIDE.** strategy.txt revised to restore a LEAN, SHARPENED cost line naming the specific
  conflation observed (Pillar {R}=one vs Spear {1}{R}=two; trust the option-line cost). Not a verbose
  re-add — it targets the exact log-witnessed error shape.
- Skill consequence (-> skill.md): the crutch-lifecycle CLOSED-criteria ("failure -> 0 in one corpus
  AND durable annotation renders") are too weak for a COST/COMPREHENSION crutch — the model can
  ignore a correctly-rendered cost from prior belief, and the miss is intermittent. One clean corpus
  is insufficient evidence of redundancy for this crutch class.

---

## BURN DISCIPLINE ROUND 3 — CLEAN (face-by-default + exception + restraint all correct)

10 burn-TARGET windows (model picks a resolving Pillar/Spear's target). **0 self-face burn.** Face
9/10; exception fired correctly once:
- **Face by default 9/10:** vs133 s10, vs137 s6/s17/s19/s23, vs102 s5/s15, vs140 s11/s17 — all at
  "The opponent (player, life N)". vs137 s23 was the lethal face (opp 5->0).
- **Exception fired (1):** vs131 s10 — Searing Spear killed **Young Pyromancer** (token-maker engine)
  over the face at opp life 19. Exactly the guide's named exception. Correct.
- **Restraint, not a miss:** vs102 s15 — did NOT waste Searing Spear on a **spent** Gray Merchant of
  Asphodel (its ETB drain had already resolved; killing it now does nothing) and faced instead.
  Correct discernment that the "drain/lifegain engine" exception is about RECURRING triggers.
- The two forced own-creature target picks (vs140 s13 chose a Goblin; vs140 s19 = the empty_reply)
  were opponent's-turn FORCED effects (edict/target-your-own), NOT burn self-targeting — correct/NA.
- **Borderline note (no action):** vs137's Edgewall Innkeeper (GW Adventures card-draw engine) was
  faced-over every window, never killed. Defensible — Edgewall draws only on Adventure creature casts
  (card advantage, not the guide's token/spell-trigger/lifegain classes) and the face-race won
  convincingly. Not promoted to the exception list on one seat.

---

## FALLBACKS (2) — both non-decisive; classifications

**(1) vs140 s19 empty_reply (transient endpoint outage).** latency 2501ms — the uniform fast-fail
HTTP-level error the brief describes (the deck109 member of the later cluster). Decision: opponent's-
turn forced pick of one of my own 5 creatures (targeted removal/edict), reply = `''`, heuristic took
over. **Board impact: NIL** — my life 20, opp life 4, opp board empty (3 lands), I won at T8. A
dominant winning position; which creature the opponent's effect touched was irrelevant. No pilot
correlation inferable from one benign outage.

**(2) vs27 s13 unparsed_reply — CLASSIFIED: out-of-range index from parent-menu conflation (NOT an
INDEX-WINS-echo shape).** The decision was a 2-option Unleash sub-prompt (`1/1 Counter` / `Decline`).
The reply's first AND last coded line were both `CHOICE: 3 (Cast Gore-House Chainwalker)` — index 3
does not exist (only 2 options) and the echoed label "Cast Gore-House Chainwalker" matches NEITHER
option. The model spiraled 7453 ch about tapped-Mountain mana and answered against the IMAGINED
parent casting-menu ("1. Searing Spear 2. Krenko 3. Gore-House") instead of the current Unleash sub-
prompt. **Why no rule recovered it:** there was no in-range CHOICE line anywhere, and the label
matched no option -> INDEX-WINS correctly refused (out-of-range index + out-of-context label) ->
unparsed_reply. The parser did the right thing; the fault is model-side (sub-prompt/parent-menu
conflation under a mana-spiral). **Board impact: BENIGN** — the heuristic added the 1/1 counter (next
record: "Counter added to Gore-House Chainwalker"), i.e. EXACTLY the choice the guide prescribes. The
vs27 game was a merfolk-evasion loss regardless. NOT an engine/parser regression; a model-
comprehension miss with a self-correcting heuristic. (Minor observation only: a sub-prompt could echo
its parent card context to reduce this conflation — noted, not proposed.)

---

## PER-SEAT PILOT VERDICT (e) — repetition_penalty=1.05: BETTER on the tight loop, FLAT on length

Wave-22's signature pathology at THIS seat was vs62 s21: a ~15x repeated identical waffle sentence
("I will declare no attackers ... but the guide says attack ... wait") that blew max_tokens.

- **Tight identical-line loop: SUPPRESSED (better).** This corpus's WORST intra-reply exact-line
  repetition is **6**, and that 6 is a QUOTED game-log line ("Damage: 1 dealt by Goblin to the
  opponent"), not a decision-waffle loop. Next-worst are 4 and 3s. **The ~15x identical-sentence loop
  did not recur.** The penalty appears to have killed the verbatim-repeat spiral it was aimed at.
- **Overall spiral LENGTH/latency: FLAT.** My seat still produces long EXPLORATORY spirals: replylen
  p50 1024 / p95 11890 / max 14919; latency p50 16.0s / p90 123s / max 213s (vs137 s7 = 157s / 9841
  ch re-deriving Stromkirk's summoning-sickness for a 1-creature T2 attack). The penalty suppresses
  verbatim repetition but not divergent re-reasoning — consistent with the brief's corpus-wide char-
  length "essentially FLAT".
- **Decision quality / prose: NO degradation.** Answer-first held **89/89** ask decisions and 26/26
  attacker windows; prose in the long spirals is grammatical and on-topic (spot-checked vs27 s12 @
  213s/13.3k ch — coherent, correct CHOICE: 3 deploy line). No token-level degradation at 1.05.
- **Verdict line: BETTER (tight-loop pathology suppressed) + SAME (length/latency, prose quality).
  No regression. Pilot is a net positive at this seat; keep it.** Caveat for synthesis: the two
  fallbacks (1 empty, 1 unparsed) are the FIRST at this seat in two corpora, but neither correlates
  to the pilot — the empty is the known transient outage, the unparsed is a model conflation unrelated
  to sampling.

---

## Loss decomposition (matchup/race, no consequential decision error)

- **vs27 (T8, -1):** opp stayed at **20 life the entire game** — Oona's Gatewarden (2/1 flying
  defender, wither) walled every ground attacker while Inkfathom Infiltrator (unblockable) + Lord of
  Atlantis raced me 20->17->9->dead. Textbook RDW-vs-evasion ceiling (guide's EVASIVE/UNTOUCHABLE
  section). Deploy-and-attack ran correctly; the s13 unparsed was benign. No writable fix.
- **vs133 (T13, -4):** raced perfectly (opp 20->7 by T8, me at 20) then Gray Merchant of Asphodel's
  devotion-drain turned the race (opp 9->14, drained me 18->2). The drain is an ETB one-shot, not a
  preemptable recurring engine; the model correctly faced rather than wasting burn on the spent body.
  Lifegain/drain race-loss; no misplay. The s23 spiral (2 life) resolved to the correct all-in.
