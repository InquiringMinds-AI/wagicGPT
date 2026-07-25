# deck137 — Selesnya (GW) Adventures — wave 25 (guided ROUND 3)

Binary 62d8783a5 (wave-25 step-1 batch), corpus matchups-20260725-035605, -T 3000, REPPENALTY=1.05.
My 6 pilot logs: 1784*-ai_baka_deck137-0x*-vs-{131,133,27,109,102,59}.jsonl.

Outcomes: **1/6 + 1 timeout-draw** (down from wave-24's 2/6+draw).
- WIN vs27 (adj, 16/10 t12).
- TIMEOUT-DRAW vs133 (10/10 t14) — **THIS is the corpus's 1 timeout, MINE** (see (4)).
- LOSS vs131 (adj behind 2/6 t12), LOSS vs102 (adj behind 9/13 t10), LOSS vs59 (KILLED -4/18 t11),
  LOSS vs109 (KILLED 0/15 t10).
Loss pattern UNCHANGED and now sharper: TEMPO / late-stabilization. All four losses are aggro RACES
the deck lost by developing/stabilizing too slowly, not by bad card choice. Two kills (vs59, vs109).

Corpus health at my seat: **0 unparsed, 0 empty, 0 retracted_choice, 0 stale_echo.** The only
fallbacks are my 15 by-design convoke defers (6). Longest reply 14,987 chars (deck59 s16, priority
pass) — ends coherent, parsed choice=0; retry correctly fired 0x (no decode garbage in any of my
five 12k-15k replies).

## (1) MY TWO SHIPPED LEDGER ITEMS — b3 echo-qualifier + b4 shockland annotation. Both VALIDATED.

### b4 (shockland ETB annotates consequences) — SHIPPED as CARD-TEXT annotation; decisions correct; standalone menu residual.
The shock consequence now rides Temple Garden's `{text:}` everywhere the card is listed:
`Temple Garden (land) {text: ({T}: Add {G} or {W}.) -- As Temple Garden enters, you may pay 2 life.
If you don't, Temple Garden enters tapped.}` (seen in every reveal/hand list — deck131 s4, deck133
s5, deck27 s10, deck102 s9). So the "enters tapped unless you pay 2" fact is now discoverable at the
source. Live shock DECISIONS this corpus: deck109 s4, deck59 s3, plus the re-plays (deck131 s8,
deck133 s7/s16/s29, deck27 s13/s39, deck109 s14) — **ALL chose "pay 2 life", ALL parsed (choice=1,
fb=None), ZERO stale_echo.** The wave-24 "is this prompt malformed?" confusion did not recur.
RESIDUAL (downgraded to cosmetic): the standalone ETB DECISION MENU still renders
`Choose an option for :` (empty subject) with bare `1. pay 2 life` / `2. tap` and NO per-option
consequence tag — the b4 annotation reached the card text, not the menu options themselves. Because
every live decision was now correct, this is no longer harmful; kept as an open representation nit
(notes.md, R-SHOCKLAND-ETB-UNLABELED downgraded).

### b3 (echo-qualifier containment trust) — CONFIRMED HOLDING; the exact s4-tap shape is ABSENT-by-behavior.
0 stale_echo corpus-wide. The specific wave-24 s4 shape — a `CHOICE: 2 (Tap Temple Garden)` whose
card-name qualifier collided with a stale "Play Temple Garden" sibling — **did NOT recur, because the
triggering behavior did not occur**: every shock this corpus paid 2 life (option 1), never chose
"tap", so no "Tap <card>" qualifier was ever emitted. (The guide's mulligan note steers to pay, which
the model followed.) I therefore cannot positively re-fire the exact s4 collision, but the containment
IS exercised and holding on the qualifier-decorated shapes that DID occur and parsed to the correct
index: deck59 s10 `CHOICE: 1 (Cast Flower // Flourish {g/w} [hybrid: each {g/w} pays with G or W -
total 1 mana])`, deck27 s22 `CHOICE: 1 (Create human with Castle Ardenvale)`, deck133 s26
`CHOICE: 1 (Venerated Loxodon)`, deck133 s25 `ATTACK: A1, A2 (Venerated Loxodon, Giant Killer // Chop
Down)` — every one a heavily-decorated parenthetical that resolved to the right option, fb=None. Net:
b3's trust is proven against qualifier decoration in general; the narrow tap-collision case is
untestable this corpus because the model stopped choosing tap.

## (2) BLOCKERS-SEAM ROUND 2 — the wave-24 13.6k lethal-block spiral is GONE. Guide section LANDED.

Wave-24's worst reply was a 13,642-char BLOCKERS decision (deck59 s39) that opened with an ILLEGAL
one-blocker-onto-four-attackers assignment and re-derived "at most one attacker per blocker" from
scratch. **This corpus every blockers reply is short and legal.** All 7 blockers records:
- deck131 s11 (801 ch, B1:A1), s44 (1052 ch, `B2:A1, B3:A2` two blockers on two Guttersnipes — legal
  gang-avoidance), deck133 s22 (1014 ch, B1:A1), deck109 s12 (1127, B1:A1), s19 (1204, B1:A3),
  **s26 (1328 ch, `B1:A3, B2:none` — a LETHAL-pressure block at life 3)**, deck102 s22 (723, B1:A1).
- Max blockers reply 1,328 chars (vs 13,642 last wave) — a ~10x collapse. Zero illegal first lines,
  zero one-on-many assignments, zero re-derivation spirals. The deck109 s26 block at life 3 is the
  direct successor of the wave-24 spiral situation and it resolved cleanly in 1,328 chars.
The blocking-under-lethal commit rule added to the wave-24 guide did exactly its job. KEEP it.

## (3) THE SPIRAL RELOCATED AGAIN — now onto UNFLOORABLE generic seams (harmless, but the latency driver).
With attackers (wave-23 floor) AND blockers (wave-24 floor) both spiral-free, my five 12k-15k replies
this corpus are all on GENERIC seams that no per-seat rule can floor: land-drops (deck131 s36 13,020
ch "Play Plains vs Forest vs no land"; deck27 s17 14,347 ch "play Castle Ardenvale?"), priority passes
(deck59 s16 14,987 ch -> pass), single-/few-option casts (deck102 s10 14,710 ch; deck27 s29/s43;
deck59 s38/s46), library reveals (deck109 s29 12,787 ch). This CONFIRMS wave-24 skill PROPOSAL 2's
prediction: flooring a named seam relocates the spiral rather than removing it. The new evidence is
that relocation TERMINATES at ubiquitous seams you cannot enumerate-and-floor (every land drop, every
priority window). The over-deliberation is harmless — choices land correct, and b2 last-answer-wins
(see (5)) even rescues snap first-line errors — but it IS the dominant latency/timeout contributor.
Skill implication in skill.md; harness/latency implication in notes.md.

## (4) THE 1 TIMEOUT-DRAW IS MINE (133v137) — decompose: -T 3000 bump HELPED; deck137 was LEAN this time.
Both seats reached turn 14 at 10/10, no gameend record either side (cap-adjudicated draw).
- **deck137 seat (mine): 31 LLM calls, sum 700s, med 16.3s, max 140.8s** — LEAN.
- **deck133 seat: 51 LLM calls, sum 2,146s, med 18.9s, max 202.5s** — the density culprit this game.
- Combined pure-inference wait = 2,846s, just under the 3,000s cap.
Contrast wave-24's 27v137 timeout: capped at TURN 10, combined ~2,352s ~ 2,400s cap, and deck137 was
THEN the density culprit (52 calls). This wave the +600s bump bought +4 turns of depth (t10 -> t14)
and deck137 got its FULL deliberation with no truncation-forced answers (med 16.3s, longest 140.8s
completed). So the -T 3000 bump PREVENTED wave-24-style cap starvation of deck137's decisions — the
game capped only because deck133's 51-call density stacked on top. Both at even life = a genuine
grind correctly adjudicated a draw, NOT a stall or a play failure. deck137 was NOT decision-dense
this game (its go-wide never got going — it was being ground). Harness note in notes.md.

## (5) 1/6 DECISION-QUALITY READ — one real misplay found; b2 last-answer-wins is a POSITIVE.
- **b2 NATURAL-STOP precedence VALIDATED (4 clean rescues).** Four replies where the first coded line
  disagreed with the recorded choice — in every case the model REVISED to a BETTER final answer and
  the parser took the final line: deck27 s19 (first CHOICE:5 -> final CHOICE:1 Flourish for stated
  lethal "opponent takes 10 damage"), deck27 s40 (5->4, cast March normally X=3 for 12 lethal, PLAN
  confirms "Choice 4"), deck59 s46 (2->1 stabilize March), deck131 s37 (2->3 Trostani anthem). b2 is
  actively improving play at this seat by honoring the model's considered conclusion over its snap.
- **THE ONE REAL MISPLAY — deck27 s21, "ATTACK: none" with a 5/5 (adventure-body misread).** Board:
  I control Lovestruck Beast (5/5) AND `Faerie Guidemother // Gift of the Fae (1/1) [flying, adventure]`;
  opponent has only `Oona's Gatewarden (2/1) [flying, defender, wither]`; life 18/20, my turn. The
  engine correctly OFFERED Lovestruck Beast as A1 (its "you control a 1/1" condition is met — Faerie
  Guidemother is a live 1/1). The model DECLINED, reasoning "Faerie Guidemother ... is an adventurer
  face, not a 1/1 body." It **misread the [adventure] tag on a resolved battlefield creature as
  meaning it is not a real 1/1** — so it wrongly believed Lovestruck was locked and passed a
  kills-or-trades-up 5/5 swing while ahead. A tempo leak, and exactly the deck's losing shape (durdle,
  under-press). GUIDE FIX SHIPPED: a new paragraph teaching that a battlefield [adventure] creature IS
  its printed (P/T) body (counts for Lovestruck's 1/1, convoke, blocking) + the ATTACK exclusion's
  INVERSE ("if Lovestruck IS listed, the engine already confirmed a 1/1 — swing, don't re-derive").
  Possible representation nit flagged in notes.md (does the [adventure] tag on a resolved body mislead?).
- **Late-stabilization persists (deck109 kill).** Life trajectory 16->14->11->9->3->1->0 vs mono-R
  aggro: the model kept attacking with its 1/1 Edgewall Innkeeper (s25 t7, s34 t9) and cast Flourish
  (a team PUMP) twice at life 1 rather than making lifelink Soldiers and holding blockers. The ROLE
  CHECK exists in the guide but isn't executed decisively — it doesn't hold its draw engine / small
  bodies back when behind on the race. Guide's ROLE CHECK strengthened this wave (stop feeding small
  bodies into the attack when stabilizing; reserve Flourish/Formation for a defensive turn). Softer
  than the s21 fix; a strengthening of existing text, not a new rule.

## (6) CONVOKE DEFERS ROUND 3 — 15 again, HARMLESS (say so).
All 15 are `kind=defer / fallback=deferred_to_heuristic`, empty prompt+reply, latency=-1 (no LLM
call): the convoke tap-payment sub-choice, routed to baka by design. Distribution deck131 x6,
deck102 x4, deck27 x3, deck59 x2 (none vs133/vs109). They remain the SOLE fallback source corpus-wide
(15 of 15). No harm: the heuristic's tap-selection never stranded a wanted attacker — verified deck131
attacked t7 (Flaxen Intruder) and t11 (Flaxen + Innkeeper) despite convoke defers those very turns.
Third consecutive corpus of harmless defers; no ledger item, flagged only so synthesis knows deck137
is still the lone defer seat (convoke is still a single-deck mechanic).

## repetition_penalty=1.05 — third-corpus verdict at this seat: SAME. No prose harm, no spiral cure.
Reasoning stays coherent across all five 12k-15k replies (adventure mechanics, wither, persist,
convoke, lethal math all sound); the penalty neither degraded prose nor tamed the (now-relocated)
generic-seam spiral. Reconfirms: a GUIDE/representation commit-rule cures a spiral where the sampler
cannot — and once the floorable seams are floored, the residual spiral moves to seams a guide can't
reach, so it becomes a latency-management problem, not a guide problem.
