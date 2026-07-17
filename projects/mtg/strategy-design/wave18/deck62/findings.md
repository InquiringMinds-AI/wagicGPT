# deck62 (Enchantresses) — REVISED-GUIDE ROUND 2 validation findings (wave 18)

Corpus: `matchups-20260716-185930`, binary `/tmp/wagic-f664539a3`. deck62's SECOND guided corpus
(the wave-17 surgical revision — Blanchwood recount-ban + A-list-only attacks — is byte-identical
live at `Res/ai/baka/deck62_strategy.txt`). 6 seat translogs, 182 decisions.

Record **2/6, down from wave-17's 4/6.** Per Step 0-ter the record is context; read the taught
classes and the tax first. Bottom line: **the drop is STRUCTURAL, not a taught-class regression.**
Targeting is perfect, the enchant-opponent gift stays dead, N1 (sub-menu-as-phase-menu) is dead —
but a NEW dominant failure shape (a losing-position lethal-hunt spiral) appeared, and the wave-17
A-list-only rule DISOBEYED twice under losing pressure.

## Record: 2/6 — the drop is the pool, not the play

| vs | result | life (me/opp) | turn | read |
|---|---|---|---|---|
| deck49 | **WIN** | 8 / -13 | 12 | clean go-wide (Spider + Saprolings, full alpha strike s31 A1-A6). Taught win line fired. |
| deck35 | **WIN** | 16 / -3 | 10 | clean go-wide vs Slivers; Ancestral-Mask/Fists on Spider, alpha struck. |
| deck135 | LOSS | -2 / 3 | 14 | **RAZOR** race loss (opp at 3). Voltron-Birds air clock vs snow/deathtouch ground. Was an adj-WIN wave-17. |
| deck14 | LOSS (adj1) | 11 / 13 | 18 | **RAZOR** adjudication, 2 life behind, vs NEW Deep Blue opponent. Went the distance. |
| deck21 | LOSS | -1 / 11 | 10 | outraced by Goblin aggro; at 2 life by T7, only 0/1s + 1/1 Saprolings, never assembled a threat. Was a WIN wave-17; deck21 also got a power-floor guide sharpen this wave. |
| deck110 | LOSS | -2 / 30 | 10 | affinity + lifegain blowout (opp climbed to 30). Same matchup floor as wave-17. |

Four losses; **two are razor-close (vs135 -2 v 3, vs14 11 v 13)** and two are matchup-floor
blowouts (goblins, affinity-lifegain). The wins held from wave-17. The brief's "flattest table yet"
+ the deck17->deck14 pool swap explain the record without any taught-class failure. deck62 is a
turn-8-13 ground/go-wide deck; it will floor against faster clocks (21/110) and grind razor races
vs 135/14 — those are the deck, not the guide.

## Tax: giant replies exploded 1 -> 8, and the reason IS the record

- **Decisions 182, fallbacks 5 (2.7%)** — up from wave-17's 3/143 (2.1%). All 5 unparsed_reply.
- **Giant replies (>8k chars) went 1 -> 8.** This is the headline and it is NOT the old Blanchwood
  pocket. Breakdown:
  - **5 giants = LOSING-POSITION LETHAL-HUNT SPIRAL (NEW):** vs21 s14 (12429), s15 (11500), s17
    (13182); vs110 s21 (12620); vs135 s28 (11802). All at 2 life, behind, hunting for lethal that
    does not exist. 3 of these truncated mid-reply -> unparsed.
  - **3 giants = prospective aura-cast math (PARSED, correct choice):** vs49 s19 (10450, Birds vs
    Web race calc), vs35 s13 (8508, Ancestral-Mask-on-Spider value), vs135 s24 (8340, a blockers
    lethal/survival check). Legitimate math, just verbose; not a wrong decision.
- The giant count tracks the number of LOSING games. Wave-17's corpus was mostly winning (4/6), so
  the off-case rarely engaged. This corpus put deck62 behind in 4 games, and the off-case (behind,
  low life) is exactly where the spiral lives — a textbook Step 0-ter rule-6 confirmation: **the
  losing corpus lives in the off-case, and the off-case was unguided.**

## The 5 fallbacks — all NEW shapes, none the old computed-P/T pocket

Answer to the brief's question: the 5 unparsed are **NOT** the old giant-reply/Blanchwood-recount
pocket. Two distinct new shapes:

1. **LOSING-SPIRAL TRUNCATION (3):** vs21 s14 (land-drop ask), vs21 s17 (cast ask), vs110 s21
   (land-drop ask). At 2 life vs an opponent at 20/32, the model re-derives its whole board over
   and over hunting for impossible lethal ("10 is not 32. I must have made a mistake."), blows the
   full token budget, truncates mid-sentence with no CHOICE line -> unparsed -> heuristic. The
   decision it was being asked (play a land) was trivial; the spiral was over the *attack that
   wasn't lethal*. Amplifier: **duplicate-card confusion** — Primal Rage sat in hand AND on the
   battlefield (deck21 runs 3x; a 2nd copy is a redundant global), and the model burned hundreds of
   tokens on "why is it in my hand?" (see notes N5).
2. **A-LIST-ONLY DISOBEYED (2):** vs14 s21 and vs21 s12 — the A-list held ONLY the 0/1 Argothian
   Enchantress; the model attacked with "Birds of Paradise" / "Birds, Saproling, Saproling"
   (all summoning-sick, ABSENT from the list) -> unparsed. Correct answer both times was
   ATTACK: none. This is the SAME mutated eligibility class as wave-17 vs135 s19 — the wave-17
   tightening ("NEVER name a creature that is absent from the list") did NOT kill it; it recurs
   specifically when the model is behind and in "I MUST attack to win" mode and overrides the list
   with its own-board read.

## Per-taught-class: died / persisted / mutated

| Taught class | Verdict | Evidence |
|---|---|---|
| Auras DRAW/PUMP, not removal | **DIED (held)** | 0 recurrences. Every aura framed as pump/token/draw. |
| Enchant-opponent GIFT (targeting) | **DIED (held, perfect)** | 20/20 target sub-menus that OFFERED an opponent creature picked an OWN creature. Zero gifts. The "only legal targets are YOUR OWN" / "legal targets right now: <names>" engine labels are load-bearing here. |
| N1 sub-menu-as-phase-menu | **DEAD (confirmed)** | The TARGET CHOICE engine framing worked: every target sub-menu parsed to a target name (none misread as an attack/phase step). The wave-17 vs135 s18 shape did not recur. |
| Argothian-first sequencing | **DIED (held)** | Cast promptly on curve where drawn (vs135 s3/s5, vs110 early). No mis-sequencing. |
| Attack every turn / no durdle | **PERSISTED (working, w/ exception)** | Full alpha strikes in the wins (vs49 s31 A1-A6, vs35 s24 A1-A3). Correctly answered ATTACK: none when only the 0/1 engine was eligible (vs135 s14/s26, vs14 s8/s14/s29, vs110 s12/s19). No passive-at-low-life durdle. |
| Defensive reach / block flyers | **PERSISTED (working)** | Web/Spider reach blocks used; survival blocks vs 135's ground beaters. |
| TRUST printed [X/Y], Blanchwood recount-ban | **LARGELY RESOLVED** | The wave-17 target — recounting Forests on a creature ALREADY showing "(printed X/Y)" with Blanchwood — is gone. The remaining Forest-counts (vs49 s26, vs14 s31/s42) are PROSPECTIVE cast-value math (Blanchwood not yet on the creature; the model legitimately must compute what it will do). N3 can be downgraded (see notes). |
| Attack from the A-list (eligibility) | **PERSISTED-and-DISOBEYED (2 recurrences)** | vs14 s21, vs21 s12 named absent (summoning-sick) creatures under losing pressure. The rule is present and correct but overridden in "must-attack" mode. Guide sharpened + harness item filed (notes N2). |
| Off-case: behind / cannot win this turn | **UNGUIDED -> NEW TAX (the losing-spiral)** | No line told the model what to do when it cannot kill this turn. It hunts impossible lethal, spirals, truncates. Guide revised (rule-6 off-case branch added). |

## Guide disposition: SURGICAL REVISION (round 2), not a freeze

Two evidence-driven edits, both Step-0-ter-sanctioned:
1. **(rule 6, off-case) Add an anti-spiral / can't-win-this-turn line.** This is the dominant new
   tax (5 giants, 3 of the 5 unparses) and it is genuine decision-hygiene, not just token budget:
   when behind and unable to kill this turn, make the best board play and pass instead of
   re-deriving the board hunting for damage that isn't there.
2. **(A-list sharpen) Name the override failure mode.** The existing "never name a creature absent
   from the list" was insufficient; the recurrences are the model in "I must attack to win" mode.
   The revised line ties eligibility to the *list*, not the *wish*, explicitly for the behind case.

Everything else is byte-frozen (the validated-dead sections are already tight). This is NOT a
rotation candidate: guide-mod = yes, new-work-signal = yes (losing-spiral shape, A-list recurrence,
3 engine/harness items).
