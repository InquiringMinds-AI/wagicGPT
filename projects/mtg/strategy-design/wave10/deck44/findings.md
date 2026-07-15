# Deck 44 (UB Faerie Tempo) — Wave 10 review (RESUME wave, answer-after-plan protocol)

Corpus: PRIMARY `matchups-20260715-031825/` (binary `/tmp/wagic-c8c054be8`, wave-9 guides,
ANSWER-AFTER-PLAN reply protocol: scratch reasoning -> PLAN: -> CHOICE:/ATTACK:/BLOCKS: final
line, max_tokens 2048). deck44 = **deck0 in all 6 games** (confirmed by `gamestart`
`my_deck=ai_baka_deck44`). Filenames carry `-vs-<opp>`.

**Record 4W-2L.** WON vs133 (12/0 T17), vs109 (2/-14 T14), vs135 (2/-3 T16), vs131 (20/-3 T12).
LOST vs110 (0/20 T9 — fast affinity), vs140 (16/13 T39 — Blightsteel Colossus infect, alive at 16).
Waves 4->10: 3/6 -> 2/6 -> 3/6 -> 5/6 -> 5/6 -> 4/6 -> **4/6**. Win column is context only (doctrine).

game->file map (via `gamestart` opp_deck):
`1784103510`=vs110 **L** . `1784103950`=vs133 W . `1784105766`=vs109 W . `1784108791`=vs135 W .
`1784112264`=vs131 W . `1784112898`=vs140 **L**. Citations: `vsNNN sN`.

Posture: **FROZEN-SPINE / turnaround, 7th wave running.** The spine (hold {U}{U}, sweeper-first
counters, cast a faerie every turn, own-targets annotation) is intact, obeyed, and won 4. This is
the FIRST corpus under the answer-after-plan protocol AND the first with the wave-9 tightenings
deployed — and BOTH verify clean. **No new decision-level failure class. Guide disposition: FROZEN
(byte-identical).** The two losses are construction/variance (vs110) + an ENGINE auto-tap defect
(vs140), neither guide-fixable. The wave's whole model-fixable content is engine-ledger (notes.md).

---

## HEADLINE 1 — the wave-9 counter-category-#2 TIGHTENING is VALIDATED; the vs110 inversion class is DEAD

The wave-9 single edit sharpened counter category #2 (un-GFTT-able artifacts — Steel Overseer,
Master of Etherium, Cranial Plating) with the WHY-as-imperative "counter ON SIGHT, no 'deal with it
later.'" The wave-9 LOSS that motivated it was vs110, where the pilot LET Steel Overseer and Cranial
Plating resolve ("slow artifact, deal with it later") and spent its counter on face burn instead.

**This wave, vs110 (still a loss, but for a DIFFERENT reason), the pilot obeyed the tightening
cleanly:**
- **s7 (T5, L19):** Cranial Plating ON THE STACK -> **COUNTERED** with Arcane Denial. Plan:
  *"the opponent's artifact engine ... is growing dangerously, I must counter this spell."*
- **s12 (T7, L16):** Master of Etherium ON THE STACK -> **COUNTERED**. Plan cites the guide verbatim:
  *"The strategy guide explicitly states to counter artifact engines like Master of Etherium 'ON
  SIGHT' because they grow the opponent's board every turn and are unanswerable."*

The exact wave-9 inversion ("slow artifact = ignorable") is GONE, and the guide's own phrasing
("ON SIGHT") is echoed in the winning-line PLAN — proof the wording propagated. The vs110 loss this
wave is a fast affinity race the pilot could not out-clock with a creature-light reactive hand (it
countered the two biggest artifact engines but the Ornithopters + Steel Overseer already on board
raced it 20->0 by T9). That is construction/variance, NOT the model-fixable inversion of wave 9.

**Full counter-spine audit (15 stack windows, all correct):**
| window | stack spell | action | guide category |
|---|---|---|---|
| vs110 s7 | Cranial Plating | COUNTER | #2 un-GFTT-able equipment ok |
| vs110 s12 | Master of Etherium | COUNTER | #2 artifact creature ok |
| vs135 s7/s10 | Ice-Fang Coatl | COUNTER | #3 growing value flyer ok |
| vs135 s25 | Abominable Treefolk | COUNTER | #3 growing attacker ok |
| vs131 s17 | Guttersnipe | COUNTER | #3 growing engine ok |
| vs131 s20 | Young Pyromancer | COUNTER | #3 growing engine ok |
| vs140 s24 | Wrath of God | COUNTER | #1 SWEEPER ok |
| vs140 s29 | Lightmine Field | COUNTER | #3 attacker-hosing ench. ok |
| vs140 s69 | Staff of Nin | COUNTER | #2-ish un-GFTT-able artifact engine ok |
| vs135 s14 | Boreal Druid | let resolve | mana dork ok |
| vs135 s21 | Into the North | let resolve | land search (named) ok |
| vs135 s22 | Arcum's Astrolabe | let resolve | cantrip artifact (named) ok |
| vs109 s17 | Legion Loyalist (1/1) | let resolve | small creature ok (see below) |
| vs131 s21 | Prism Ring | COUNTER | minor over-counter (see below) |

Two blemishes, neither a class, neither cost a game:
- **vs131 s21 (L20/4): countered Prism Ring** — a card the guide's LET-RESOLVE list names explicitly
  (a cheap lifegain rock). But opponent was at 4 life in a blowout deck44 won 20/-3 T12; the wasted
  counter was inconsequential. Single low-stakes instance -> not a revision trigger.
- **vs109 s17 (L6/16): let Legion Loyalist (a 1/1) resolve** while at 6 life. Guide category #4
  ("any creature while your life is 12 or less") arguably says counter it, but the pilot's reply
  reasoned correctly — *"it is not a sweeper, an unanswerable artifact engine, or a lethal burn ...
  countering a 1/1 now would leave me no mana for my own threats"* — and WON by racing (2/-14). A 1/1
  is not worth a counter even at low life if you are the beatdown and winning the race. Defensible
  play; category #4 is a slightly-broad safety net, not a bug. No change.

---

## HEADLINE 2 — the NEW "legal targets right now: <names>" annotation KILLED the wave-9 s54 fabrication class

The wave-9 verification item: did the fabricated-no-legal-target class (deck131 s54, the pilot
arguing *"Young Pyromancer is an artifact? No... Wait..."* — talking itself out of legal removal)
die under the new named-target annotation? **Answer: yes, cleanly.** The engine now renders TWO
GFTT annotation forms:
- `- the only legal targets are YOUR OWN right now` (wave-9 own-targets form; 36 windows this corpus)
- `- legal targets right now: <names>` (NEW; NAMES every legal target incl. opponent creatures; 4 windows)

At the exact windows that produced the wave-9 fabrication:
- **vs131 s8 (T4):** `legal targets right now: Faerie Miscreant, Faerie Miscreant, Young Pyromancer`
  — the pilot reasons cleanly and makes a DELIBERATE hold (*"Young Pyromancer is a 2/1 that I can
  block effectively with Gatewarden ... keep my removal for bigger threats"*). No fabrication loop.
  (It later COUNTERED a second Young Pyromancer at s20 and won 20/-3 — the hold was fine in context.)
- **vs109 s6 (T4):** `legal targets right now: Surveilling Sprite, Gore-House Chainwalker` — the
  pilot cast GFTT and the target sub-pick (s7) correctly hit the OPPONENT's Gore-House Chainwalker
  (a 3/2 unleashed can't-block attacker, exactly a guide-named GFTT target), not its own Sprite.

The named-target form is the correct engineering answer to the wave-9 residual: instead of a bare
option the pilot fabricates over, it prints the legal targets and the pilot picks among them. The
"is-it-an-artifact? Wait..." self-negotiation appears NOWHERE in this corpus.

---

## HEADLINE 3 — own-targets annotation: 36/36 CORRECT IN REASONING (the 1 recorded self-cast is a truncation-parse artifact)

The wave-9 own-targets annotation fired **36 times** this corpus. Reasoning-tax stayed low (median
plan 546 chars). **35 clean "Cast nothing"/other; 1 recorded self-cast (vs140 s68) that is NOT a
representation failure** — it is a 2048-token-cap TRUNCATION-PARSE artifact:

- **vs140 s68 (T38 Main phase 2, L16/13):** Blightsteel Colossus has just resolved (unanswerable).
  The reply is a 2000+ char infect-rules death-spiral ("If a player has 10 or more -1/-1 counters,
  they lose ... Go for the Throat: The only legal targets are YOUR OWN right now. **So no.**")
  that hit the token cap and NEVER emitted a `CHOICE:` line. `fallback=None` but the parser grabbed
  a stray `1` from the reply body (it contains "1. Attack", "1. Cast Go for the Throat") -> recorded
  `choice=1` = Cast GFTT (self-target). **The pilot's actual reasoning EXPLICITLY obeyed the
  annotation and rejected self-targeting.** The game was already lost (Blightsteel kills by infect
  next turn regardless). So: the annotation is 36/36 obeyed in the derivation; the recorded self-cast
  changed nothing and is a harness truncation artifact, not a read-then-disobey.

Method note for the tally: a recorded self-cast/mis-cast must be reply-read before it is scored as
an annotation-ignore — a reply that hit the token cap with no `CHOICE:` line and a parser-grabbed
stray digit is a HARNESS defect (known 2048-cap class, 4096 shipped next corpus), not a
representation failure. See notes.md ledger.

**GFTT no-target fallback: KEEP (unchanged from wave 9).** The zero-legal-target case (pilot holds
no creature AND opponent has none, GFTT renders with NO annotation at all) did NOT occur this corpus
— every GFTT window carried one of the two annotations. Untested -> pause-aware freeze, do not
demote.

---

## HEADLINE 4 — PROTOCOL SHIFT: intent-collapse is gone at this seat; label compliance clean

First corpus under the answer-after-plan protocol (PLAN first, then CHOICE:/ATTACK:/BLOCKS: last).
The head-first intent-collapse class (answer committed before the model reasons) is **structurally
absent** at this seat — the CHOICE token is emitted AFTER the PLAN. Findings:

- **Plan-conclusion vs recorded-choice: matches at every counter/GFTT window** except one benign
  wobble. **vs109 s6:** the PLAN prose said *"I will hold my Counterspell and Go for the Throat for
  later"* but `CHOICE: 1` = Cast GFTT. This is a stale-plan-vs-menu mismatch (the plan described an
  uncastable line — Shadow Puppeteers for {6}{u} at 2 mana — that was not on the 2-option menu
  {GFTT, nothing}); faced with the actual menu the model cast GFTT, and the target sub-pick correctly
  hit the opponent's Chainwalker. Outcome was GOOD (removed a real threat, won 2/-14). Not a loss.
- **Attacker declarations use `ATTACK: A1, A2` (attacker-slot indices), parse cleanly**, map to the
  correct engine option every time. (An earlier digest's "MISMATCH" flags were false positives — a
  naive first-int grabbed a stat digit like "1/1"; the ATTACK lines are correct.)
- **No-attack windows: ZERO.** Across vs133/vs135/vs131 the pilot attacked in every attacker window
  (the "attack every turn" spine is fully obeyed). vs135 s31 swung all-6 for lethal (opp 3 -> -3).
- **Label anomalies (3, all benign):**
  - vs110 s13 blockers `unparsed_reply`: the model wrote a well-reasoned decline ending
    `BLOCKS: (No blocks declared; Oona's Gatewarden stays untapped)` — the parenthetical prose failed
    the block-syntax parse -> heuristic fallback choice=0 (no blocks). The fallback MATCHED the pilot's
    stated intent (it reasoned NOT to block a 3/5 with a 2/1). **Did NOT change the game.**
  - vs140 s65 `unparsed_reply`: a land-drop window, immaterial.
  - vs140 s68 truncation self-cast: covered in Headline 3, game already lost.

---

## THE TWO LOSSES — ownership

- **vs110 (L, 0/20 T9):** DECISION-LEVEL play was CORRECT (countered both artifact engines on sight,
  validating the wave-9 fix; deployed a creature at every castable window). Fast affinity + a
  creature-light reactive draw out-raced the clock. Owner: construction/variance.
- **vs140 (L, 16/13 T39):** lost to Blightsteel Colossus (11/11 infect trample artifact) — deck44
  died by POISON at 16 life. The game-losing MECHANICAL factor is an ENGINE AUTO-TAP DEFECT:
  - **s61 (T36):** pilot cast Archmage of Echoes `{4}{u}` with 10 untapped sources (6 Island / 4
    Swamp) and Counterspell in hand. The engine auto-tapped **5 Islands** (all blue) for a spell
    needing only 1 blue, instead of the 4 available Swamps for the {4} generic.
  - **s64 (T37, opp turn):** Blightsteel Colossus ON THE STACK. Pilot's untapped mana `{u}{b}{b}{b}{b}`
    — only ONE blue source. Counterspell `{u}{u}` was therefore UNCASTABLE and NOT OFFERED; the only
    options were GFTT (can't hit an artifact) and Cast nothing. Blightsteel resolved and killed by
    infect. Had the engine tapped Swamps-first, the pilot would have had `{u}{u}{u}{u}{u}` and
    countered the game-winning bomb.

  This is NOT guide-fixable — the guide already says "LEAVE two blue sources untapped," but auto-tap
  makes that rule UNEXECUTABLE (the pilot cannot choose which lands tap). Blightsteel is also
  structurally beyond deck44's other answers (artifact -> no GFTT; trample+infect -> chump doesn't
  save; indestructible). Route the auto-tap greediness to the engine ledger (notes.md #2). The
  residual (no answer to an infect artifact bomb from a UB tempo deck) is construction, not guide.

---

## RE-CONFIRMED / FROZEN (all held; no change)
- **Counter spine on SWEEPERS/ENGINES: correct at 15/15 windows.** Wave-9 category-#2 WHY-imperative
  VALIDATED (vs110 s7/s12 on-sight). Sweeper-first (#1) intact (vs140 s24 Wrath countered).
- **Own-targets annotation: 36 windows, 36/36 obeyed in reasoning** (1 truncation artifact, not a
  representation failure). Reasoning-tax stayed low (median 546). **GFTT no-target fallback: KEEP.**
- **NEW named-target annotation:** killed the wave-9 fabrication class (vs131 s8/s9, vs109 s6). Clean.
- **Cast-a-creature-every-turn spine:** 1 cast-nothing-while-creature-offered window all corpus
  (vs135 s30 — the offered creature was Oona's Gatewarden, a DEFENDER that would not add to the
  lethal attack the pilot then made; correct decline). Otherwise deployed on curve every window.
- **Attack every turn:** 0 declined attacks across the wins. vs135 swung for exact lethal.
- **Face-burn value-floor carve-out (wave-9 add): UNTESTED** — no burn-to-face appeared in any
  counter window this corpus. Pause-aware: keep frozen, do not demote.
- **Obliterator no-block absolute + GFTT-hold: UNTESTED again** — vs133 (mono-black) did not put
  Obliterator on the board as a combat factor. Frozen, keep.
- **Gatewarden first-strike carve-out + <=8 chump + 9+ no-block:** vs110's one blocker window (s13)
  was a correct non-block of a 3/5 with a 2/1 at L12 (>8, and no profitable wither trade); no
  first-striker or chump window arose. Largely UNTESTED, frozen.
- **Mulligan, the engine block, all card notes: verbatim, obeyed** (vs110 s1 keep reasoned correctly).

## HARNESS-DEFECT SWEEP (run first, per skill)
- DESYNC / head-choice mismatch: 0 genuine (attacker "A#" notation parses clean). ENGINE NO-OP: none
  (casts spend mana + show `hand -> stack`). VALIDATION-DROP: none observed in the deck44 seat.
  NUMERIC-OFFSET / X-menu: N/A. Fallbacks: 3 `unparsed_reply` (all benign, above) + 1 truncation
  self-cast (vs140 s68, game already lost). Timeouts: 0. All 6 games 9-39 turns.
- **NEW this corpus — counter-option MIS-ANNOTATION (engine ledger #1):** Counterspell/Arcane Denial
  options render `- NO legal target right now` even when a spell IS on the stack and the SAME option
  separately lists `can target on the stack: <name>` (vs110 s7/s12, vs140 s24/s29/s69, vs109 s17).
  The pilot correctly IGNORED the contradictory "NO legal target" every time (trusting the ON THE
  STACK section + the "can target" clause) and countered when it wanted to — so it caused ZERO
  misplays. But it is a latent risk a weaker moment could obey. Engine fix, NOT guide text.

## RESIDUAL (construction, not guide) — persists waves 5-10
16 Island / 10 Swamp + 6 reactive cards (2 Counterspell, 2 Arcane Denial, 2 GFTT). Counters earn
their slots (protected all 4 wins: Cranial Plating, Master of Etherium, Wrath, Lightmine, Staff of
Nin, Guttersnipe, Young Pyromancer, Abominable Treefolk, Ice-Fang Coatl). "Trim the counters" is the
WRONG lever. The all-reactive/creature-light slow keep loses to fast ground clocks (vs110) and has no
answer to an infect artifact bomb (vs140 Blightsteel). Sharper levers: black-source density vs the
black spell load, and/or 1-2 more cheap flyers over the 3rd/4th reactive card. Route to the user
(notes.md).

## FREEZE-CHECK (every kept line accounted for — guide is BYTE-IDENTICAL to the live wave-9 guide)
Live guide md5 `bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines. Every load-bearing line was OBSERVED
OBEYED or UNTESTED this corpus; NONE was observed-obeyed-AND-double-covered in a way that licenses
demotion:
- **#1 cast-a-creature-every-turn** — obeyed (1 correct defender-decline). VERBATIM.
- **THE ENGINE block** (Obyra/Archmage/Bladecrafter) — Archmage cast on curve (vs135, vs140).
  VERBATIM.
- **ATTACK EVERY TURN + lethal-check** — obeyed 100% (0 declined attacks; vs135 exact lethal).
  VERBATIM.
- **BLOCKING block** (Obliterator never-block absolute; Gatewarden designated-blocker +
  growing/killable + wither-through-first-strike; 9+ no-block; <=8 chump) — largely UNTESTED
  (only vs110 s13, a correct >8 non-block). Pause-aware FREEZE. VERBATIM.
- **COUNTER priority #1/#2/#3/#4 + value floor + face-burn carve-out + both plan-binding sentences +
  hold-{U}{U} + "cast before hold"** — #1/#2/#3 VALIDATED (15/15 windows); #4 and face-burn carve-out
  UNTESTED (freeze). VERBATIM.
- **GFTT block** (target Obliterator; no-target fallback; can't-hit list) — no-target fallback KEEP;
  own+named annotations validated. VERBATIM.
- **Mulligan; all card notes; SITUATIONS recap** — obeyed/untested. VERBATIM.

CHANGES: **NONE.** The guide is frozen byte-identical. No new decision-level failure class emerged;
the wave-9 model-fixable item (counter inversion) is fixed and validated; the two losses are
construction/variance + an engine auto-tap defect (both non-guide). Displacement audit: 0 lines
added, 0 removed.

## One-line handoff
Top deck, spine frozen 7th wave, won 4. The wave-9 counter-#2 tightening is VALIDATED (vs110
countered Cranial Plating + Master of Etherium ON SIGHT — the wave-9 inversion class is dead); the
NEW named-target GFTT annotation killed the wave-9 s54 fabrication class; own-targets is 36/36 obeyed
in reasoning (1 truncation-parse artifact). Guide is byte-identical FROZEN. The only model-relevant
content is engine-ledger: auto-tap stranded {U}{U} and cost the vs140 game (couldn't counter
Blightsteel), and counter options mis-render "NO legal target" over an occupied stack.
