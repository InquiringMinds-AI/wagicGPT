# Wave 26 review -- deck133 (mono-black devotion midrange, "Phyrexian Asphodel")

Corpus: PRIMARY `matchups-20260725-115216` (21 games, binary `77e91ef4e` = wave-26 step-1
batch [commits 79e39511f + 77e91ef4e]). deck133 was the GPT seat in 6 games. Pilot:
repetition_penalty=1.05 (standing). deck133 STAYED per the wave-25 verdict (guide freshly edited
last wave -- the Arena sharpening; this corpus is that edit's first at-scale test, and the wave-25
notes teed up a rotate-out if it held).

**Real record 6W / 0L / 0D** (reconciled from results.tsv + gameend records) -- UP from 1/6:

| opp | deck | result | end my/opp | note |
|-----|------|--------|-----------|------|
| 22 | Giants (Step-0) | WON | 4 / -2 (t14) | beatdown RACE, no payoff drawn -- won on beats per the guide |
| 131 | UR Guttersnipe | WON | 13 / -7 (t25) | long grind; Obliterator t15 + Yawgmoth engine t19 |
| 102 | mono-B Tergrid | WON | 14 / -5 (t13) | Obliterator t11 + Yawgmoth engine t7; holds the ONE fallback (finding 2) |
| 27 | UB Wither | WON | 11 / -7 (t17) | Obliterator t15; clean kill (was an adj LOSS in wave-25) |
| 137 | GW Adventures | WON (adj) | 18 / 4 (t14) | adjudicated at cap, comfortably ahead; Gray Merchant t8 drain 6 |
| 18 | Kithkin (Step-0) | WON | 10 / 0 (t14) | Obliterator t8; opp to exactly 0 |

5 clean kills + 1 adjudicated win (well ahead 18/4). Decomposition (finding 1): the 1/6 -> 6/6
flip is THREAT-DELIVERY (drought broke) + POOL-SHIFT, NOT a decision-quality leap -- but decision
quality is solid and clean on every axis audited. Applying the deck59 6/6-hides-taxes rung: the
perfect record hides NO deciding decision tax. The one non-clean event corpus-wide at this seat is
a SAFE-DIRECTION parser false-fallback (finding 2), non-deciding. Guide FROZEN (byte-identical to
deployed `74efc610...`; cmp-verified) -- no new guide-layer work this wave.

---

## FINDING 1 -- [THE 1/6 -> 6/6 DECOMPOSITION] -- threat-delivery (drought broke) + pool-shift dominate; decision quality is clean, not the driver. Route: CONSTRUCTION (context) + no action.

Decomposed per the brief's three hypotheses (threat delivery / pool shift / decision improvements).
Ranked by contribution:

**1. THREAT DELIVERY ARRIVED (dominant).** In wave-25, Gray Merchant AND Phyrexian Obliterator
were offered 0 times in ALL THREE flipped losses -- the deck flooded on discard + Bloodghast
(cannot block) and lost. This corpus the drought BROKE:

| game | Obliterator deployed | Gray Merchant deployed | Yawgmoth engine |
|------|---------------------|------------------------|-----------------|
| vs131 | t15 | -- | t19 (5-act engine) |
| vs102 | t11 | -- | t7 |
| vs27  | t15 | -- | -- |
| vs137 | -- | t8 (drain 6) | t10 |
| vs18  | t8  | -- | -- |
| vs22  | -- | -- | -- (won on beats) |

Phyrexian Obliterator was deployed in 5 of 6 games (all but vs22); Gray Merchant closed vs137. The
deck's structural signature is unchanged (wins when it draws a payoff, loses when it floods) -- this
corpus it drew the payoff. This is DRAW VARIANCE reversing in deck133's favor, the mirror image of
wave-25's variance against it. Not a decision improvement; the deck simply drew its threats.

**2. POOL SHIFT (major).** deck133's two hardest wave-25 matchups BOTH rotated out:
- deck109 (fast mono-R aggro) killed deck133 at turn 8 in wave-25 (threat-drought + no early blocker
  + Bloodghast-can't-block = the deck's worst nightmare). GONE.
- deck59 (WB persistence grind) ground deck133 out on value. GONE.
Replaced by two GUIDELESS Step-0 debuts -- deck18 (Kithkin, 4/6) and deck22 (Giants, 2/6) -- weaker
pilots on fresh decks. deck133 beat both (18: 10/0; 22: 4/-2 on a race). The matchups that punished
deck133's structural holes left the pool; the incoming decks did not exploit them.

**3. DECISION QUALITY: solid and CLEAN, but NOT the flip driver.** Every audited axis held (findings
3-5 + audits): Arena declined correctly (finding 3), Gray Merchant cast in the correct win/save mode
(vs137 s25, drain 6 at opp20, then recurred), Obliterator used as threat+wall, Yawgmoth engine
exercised cleanly 5x (finding 4), Liliana discipline round 4 clean (finding 5). The quality was
ALREADY there in wave-25 (the 1/6 was "not a decision-quality collapse" per that review). Quality
is a floor here, not the lever that moved.

**Verdict:** 1/6 -> 6/6 is variance (threat delivery) + pool composition, on top of an already-sound
pilot. NO decision improvement caused the record; NO decision tax is hidden under it. The
construction flags (no early blocker, no reach except GM+Liliana-2, Bloodghast can't block,
top-heavy payoff base) are UNCHANGED -- they simply were not punished this corpus. They remain
ledgered upstream/construction-lane, not guide-fixable.

---

## FINDING 2 -- [THE CORPUS'S ONE NON-DEFER FALLBACK: retracted_choice at vs102 s22] -- a SAFE-DIRECTION parser false-fallback on a NEW sub-shape (premature-answer-then-self-correct); the reply's clear final answer WAS parseable (option 3, 3-way corroborated). Route: HARNESS/PARSER (notes.md, c4 corpus-gate finding). NON-DECIDING.

The brief flagged this as the only non-defer fallback corpus-wide (kind=priority) and the c4
corpus-gate check. kind=priority, type=retracted_choice, vs102 game, seq 22, turn 9, Main phase 1,
my15/opp16, latency 84,675 ms, choice=-1 (heuristic answered). It is a retracted_choice -- the exact
class deck133 uniquely produced in wave-24, which the b1 de-fang + c4 line-anchored hardening were
meant to close. It RECURRED once here, in a NEW sub-shape.

**The reply verbatim (head + the decisive lines):**
```
CHOICE: 0 (pass)

The board state shows that all my lands ... are tapped, and I have no untapped mana sources.
[... long deliberation ...]
So I can do Option 3.
Why did I think "pass"? Because I assumed I needed mana. I don't.
So, CHOICE: 3 (-1/-1 Counter with Yawgmoth, Thran Physician targeting Geralf's Messenger).
[...]
So the choice is 3.

PLAN: Sacrifice Bloodghast to Yawgmoth to put a -1/-1 counter on Geralf's Messenger and
draw a card. [...]
```

**Decomposition against the precedence spec (natural-stop last-coded-line; truncated first; ATTACK
answer-first):**
- The reply naturally STOPPED (ends with a complete PLAN, not truncated). Precedence for a
  natural-stop reply = LAST-CODED-LINE / the model's committed final answer.
- The model's TRUE final answer is unambiguously option 3, corroborated THREE ways: (a) the
  mid-prose `So, CHOICE: 3 (...)`; (b) the natural-language `So the choice is 3.`; (c) the `PLAN:`
  action (Sacrifice Bloodghast to Yawgmoth -> -1/-1 on Geralf) which is option 3's action, NOT
  option 0's (pass). The model EXPLICITLY retracts its own line-1 answer: "Why did I think 'pass'?
  Because I assumed I needed mana. I don't."
- **Why the detector fired (mechanism, `choiceRetractedNoReplacement`, AIPlayerGPT.cpp ~2510):**
  the ONLY line-leading `CHOICE:` line is line 1 (`CHOICE: 0 (pass)`), so chosenNum=0 and
  lastChoiceEnd=end-of-line-1. The corrected recode `So, CHOICE: 3` is prefixed "So, " -- NOT
  line-leading (the line-anchoring strips only spaces/tabs/`*`/`#`/`-`, not "So, "), so it can never
  be promoted to the answer; it registers as a contradictory mid-prose second coded index (n=3, in
  range, != chosenNum 0) -> condition (a) returns true -> retracted_choice -> heuristic. This is the
  c4 line-anchored hardening's DESIGNED behavior: it correctly refuses to salvage a non-line-leading
  coded index as the answer.

**Is it a c4 hardening MISFIRE?** By the c4 gate's letter (a legitimate reply whose clear final
answer was dropped to fallback), YES this is a c4 finding: the reply's 3-way-corroborated answer
(option 3) fell to the heuristic. But it is the SAFE direction, and BY DESIGN -- the detector's own
comment states: "A false fallback here only routes to the (safe) heuristic; a false take (a used
retracted digit) is the harmful case we still catch." This reply is a genuine
"premature-answer-then-self-correct" shape that the detector CANNOT structurally distinguish from
the harmful "answer-then-drift" shape (wave-24 vs140 s9: line-1 `CHOICE: 2 (Decline)` then buried
`So CHOICE: 1`, where routing to the heuristic was the safe call). Both look identical to condition
(a): a line-leading answer + a contradictory mid-prose recode. It is NOT a wrong TAKE.

**A candidate discriminator (routed to notes.md, LOW priority):** the two shapes DIFFER in that here
the `PLAN:` action corroborates the RECODE (option 3), not the line-1 index (option 0/pass). When a
natural-stop reply's line-leading coded index contradicts its own PLAN action while a non-line-leading
recode matches the PLAN, the recode is a genuine self-correction, not a drift. That signal could let
the parser TAKE the recode instead of falling to the heuristic. It is subtle (requires matching PLAN
prose to an option action -- the fragile name-matching the parser deliberately avoids) and LOW value
(safe-direction, non-deciding, rare: 1/1364 decisions corpus-wide). Offered for the ledger, not urged.

**Deciding?** NON-DECIDING. Turn 9, my15/opp16, the heuristic answered, game WON 14/-5. The
heuristic's action at a low-stakes value-engine activation cost nothing.

**Update to the wave-25 conclusion:** wave-25 recorded "retracted_choice 0 corpus-wide, quoting shape
vanished, de-fang WEAK-confirmed, ceiling reached." That is now REFINED: the class RECURRED once in
wave-26 -- but in a DIFFERENT trigger (genuine self-correction with a non-line-leading recode), NOT
the example-echo trigger the b1 de-fang addressed. The b1 de-fang and the example-echo exclusion are
NOT implicated here; this is the c4 line-anchoring interacting with a premature-then-corrected reply.
The two triggers are independent.

---

## FINDING 3 -- [PHYREXIAN ARENA EDIT -- FIRST CORPUS] -- low-life Arena casts STOPPED (0 casts); the edit's TWO facts are being actively cited; but WEAK-by-construction (the deciding low-life trigger never occurred). Route: guide KEPT (frozen).

The wave-25 edit rewrote the Arena line to state (a) Arena draws at your NEXT upkeep, not on cast
(engine `auto=@each my upkeep:draw:1`); (b) 1 life every upkeep; (c) "not a play, it is a countdown
-- at low life with no other play, cast NOTHING." Deployed guide sha256 `74efc610...`.

**Acceptance this corpus:**
- Phyrexian Arena CAST 0 times across all 6 games (exact chosen_text startswith "Cast Phyrexian
  Arena" = 0). The wave-25 low-life-Arena misplay (vs109 s19 @5 life, vs102 s28 @7 life) did NOT
  recur.
- Arena was OFFERED only twice, BOTH at 16 life (SAFE): vs131 s11 (t5) and s13 (t7). In BOTH the
  model DECLINED Arena and cast Bloodghast, and the reasoning cites the edit's exact facts:
  - s11 verbatim: "Casting it now is superior to casting Phyrexian Arena (which costs life and draws
    slowly)..." -- "draws slowly" is the TIMING correction the edit added (wave-25 the model believed
    Arena "draws immediately").
  - s13 verbatim: "I will not cast Phyrexian Arena yet because I am at 16 life and the opponent has
    no immediate board presence; the 1 life per turn cost is unnecessary..." -- the "1 life per turn"
    + safe-life framing is the countdown logic the edit added.

**Verdict:** The edit's two facts (slow-draw timing + per-upkeep life cost) demonstrably LANDED and
are being cited to decline Arena. But this is WEAK-by-construction validation, ceiling reached -- the
SAME shape as the b1 de-fang (finding 2 lineage). The edit targets the DECIDING case (Arena offered
at LOW life under lethal); this corpus Arena was offered only at 16 life, so the deciding case was
never presented and cannot be strong-confirmed. What IS confirmed: 0 Arena casts, the timing-fact
correction internalized, no counter-evidence. Consistent with the edit working; no misplay recurred.
Guide KEPT, no further edit -- sharpening a rule the model now follows would be a crutch for a
non-problem.

---

## FINDING 4 -- [YAWGMOTH SacrificeCost -- ROUND 2 ACCEPTANCE, EXERCISED AT LAST] -- 5 activations, ALL payable, engine used correctly; the wave-24 unpayable-cost class did NOT recur. Route: guide KEPT; ledger item DISCHARGED.

Yawgmoth (Thran Physician) was CAST in 3 games (vs131 t19, vs102 t7, vs137 t10) -- the wave-25
"Yawgmoth UNEXERCISED (Thran Physician never drawn)" open item is now DISCHARGED. Five ability
activations rendered, all fallback=None, cost `[cost: Life, Sacrifice]` rendered correctly:
- vs131 s37 (t19): -1/-1 on Young Pyromancer (opp creature) -- correct removal.
- vs102 s13 (t7): -1/-1 on own Geralf's Messenger -- opp had no creature; value engine (sac+draw).
- vs137 s29 (t10): -1/-1 on own Bloodghast -- verified opp had 0 creatures (board: "creatures: 0");
  the -1/-1 target was incidental, the value is the sac(Bloodghast)+draw engine. Correct.
- vs137 s45/s46 (t13): -1/-1 on Flaxen Intruder // Welcome Home (opp creature) -- correct.

3 of 5 hit opponent creatures (correct); the 2 own-creature targets both occurred when the opponent
had NO creature (up-to-one target, value-engine use). The guide's "-1/-1 counter on THEIR creature"
is applied where a THEIR creature exists, and Yawgmoth is correctly activated for the sac+draw value
where none does. No unpayable-cost retraction (the wave-24 vs137 s26 class: engine offering
"Sacrifice another creature" with no other creature) -- did NOT recur. Round-2 acceptance: CLEAN.

Note: the one retracted_choice (finding 2) was ALSO a Yawgmoth-targeting decision -- but that is a
parser false-fallback (finding 2), not a Yawgmoth-logic defect; the model's reasoning there was
CORRECT (sac Bloodghast, -1/-1 Geralf, draw), it just premature-answered "pass" and corrected.

---

## FINDING 5 -- [LILIANA DISCIPLINE ROUND 4] -- CLEAN. The symmetric-Veil-+1-while-behind anti-pattern did NOT recur across 9 activations; two-walker distinction applied correctly. Route: guide KEPT.

Nine Liliana loyalty activations rendered; the wave-23 anti-pattern (symmetric Liliana-of-the-Veil
+1 self-discard while defending/behind) did NOT recur:
- Liliana of the Veil activated twice: vs137 s21 (my15/opp20) -2 edict (correct removal); vs137 s26
  (my21/opp14, AHEAD) +1 discard -- the one Veil +1, taken while ahead, which the guide permits.
  Acceptable.
- Liliana, the Last Hope activated seven times (vs22 s14/s17/s22, vs102 s42, vs18 s22/s28/s34): a mix
  of -2 (mill+return, pure value) and +1 (-2/-1 shrink, pure value). Last Hope's +1 is NOT symmetric
  discard -- it is removal/value -- so every one was correct, including the even-life ones (vs18 s34
  my10/opp10 +1 shrinks an attacker = defensive value).

**Verdict:** discipline holds round 4; no edit. The two-walker distinction (Veil-symmetric-+1 vs
Last-Hope-value-+1) is applied correctly -- the only Veil +1 was while ahead; every Last Hope +1 is
legitimate value.

---

## FINDING 6 -- [c1-c5 BEHAVIOR SHIFTS AT THIS SEAT] -- c1 clean (cap gone); c2/c3/c5 UNTESTABLE (deck runs no shockland/pump/may-trigger); c4 = finding 2. Route: notes.md (watch).

- **c1 (full target enumeration, "(+N more)" cap removed):** the "(+N more)" truncation marker
  appears 0 times across all 6 deck133 prompts. 7 targeted-removal decisions (Fatal Push /
  Collective -2/-2) chosen, all resolved clean. Consistent with the fix; deck133's removal is
  single-target vs boards small enough that truncation was not at risk anyway, so this is a
  no-regression confirmation, not a stress test. No prompt-length side effect (reply p50 1093,
  unchanged).
- **c2 (ETB pay-or-tap annotations):** "enters UNTAPPED"/"enters TAPPED" appears 0 times -- deck133
  runs fetches only, no shocklands/pathways. UNTESTABLE at this seat (as wave-25).
- **c3 (PT-pump delta render):** "until EOT (X -> Y)" appears 0 times -- deck133 has no activated
  pump ability. UNTESTABLE at this seat.
- **c4 (parser hardening):** the corpus-gate finding = finding 2 (the retracted_choice
  false-fallback). Reported verbatim against the precedence spec above.
- **c5 (may-ask render):** "you MAY act on" appears 0 times -- no may-triggers with a resolvable
  object surfaced at this seat. UNTESTABLE.

---

## MANDATORY AUDITS (summary)

- **The 6/6 (deck59 6/6-hides-taxes rung):** NO deciding decision tax hidden. The vs22 win at 4 life
  is a clean BEATDOWN RACE (no payoff drawn, attacked with Geralf + 2 Bloodghasts to close per the
  guide's "half your games you win by beats"), not a misplay-induced close call. Every audited axis
  (Arena, GM, Yawgmoth, Liliana, removal) is clean.
- **Arena edit round 1:** 0 low-life casts; edit's facts cited to decline at safe life; WEAK-by-
  construction (deciding trigger absent) -- finding 3.
- **retracted_choice / c4 corpus gate:** 1 (vs102 s22), safe-direction false-fallback on a NEW
  sub-shape, non-deciding, reported verbatim -- finding 2.
- **Yawgmoth SacrificeCost round 2:** EXERCISED (5 activations), all payable, engine correct; wave-24
  unpayable class did not recur -- finding 4. Open item DISCHARGED.
- **Liliana discipline round 4:** CLEAN, 9 activations, 0 anti-pattern -- finding 5.
- **Bloodghast forced-sac fodder round 3:** UNEXERCISED. No edict/forced-sac/forced-discard window
  rendered at this seat (opponents 22/131/102/27/137/18 presented none). Carried, cheap.
- **Gray Merchant:** 1 cast (vs137 s25, drain 6 at opp20, correct win/save mode; recurred). No
  wait-rule failure. Offered/drawn less this corpus because Obliterator+beats carried the wins.
- **Self-Fatal-Push watch:** CLEAN. Targeted-removal all hit opponent creatures; no self-targets, no
  fizzles.
- **Fallbacks at this seat: 1** (the retracted_choice, finding 2). No empty_reply, no unparsed, no
  stale_echo. Retry fired 0x corpus-wide.

## PILOT VERDICT (repetition_penalty=1.05): SAME -- no degradation.
278 non-gameend replies: p50 1093 (wave-25 1128, wave-24 1129 -- stable), p90 3705, p95 5811, max
14922. Latency p50 17.2s, p90 57.8s, max 192.5s. No degenerate n-gram repetition, no prose
degradation. The one long deliberation (the 84.7s retracted_choice, finding 2) is content-rich
self-correction, not decode-collapse. Consistent with wave-24/25. KEEP.

## FREEZE / EDIT CHECK
Guide FROZEN -- byte-identical to deployed `74efc610...` (cmp-verified in wave26/deck133/strategy.txt).
NO edit this cycle: the Arena edit held (finding 3), Yawgmoth exercised clean (finding 4), Liliana
clean (finding 5), and the one new signal (finding 2) is a HARNESS/PARSER-lane item, not a
guide-layer gap. See notes.md for the routed ledger + ROTATION VERDICT (ROTATE OUT).
