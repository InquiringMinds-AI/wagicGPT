# wave60 — deck152 (Bant Midrange) per-deck review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-171140/`. Seat logs
`*-ai_baka_deck152-0x*`. Record: **5-1**. 310 decisions across the six seat logs,
**0 fallbacks**, 0 re-asks recorded, 0 `bad_reply`/`empty_reply`/`transport_error`.
All turn/phase references below are read out of the rendered `prompt`, not the
translog `turn`/`phase` fields.

## 1. Game-by-game

| opponent | result | end life | rendered end turn | deciding decisions |
|---|---|---|---|---|
| deck130 | WIN | 35 / -7 | 15 | seq 22, 25 — escalating full swings behind Ranger Class wolves; nothing contested |
| deck162 | WIN | 11 / -18 | 13 | seq 22/30/46 attacks; seq 32-42 an 11-window pass loop (see I2) |
| deck123 | WIN | 24 / -2 | 13 | seq 13/18/26 — Katilda + Spellbinder beats; seq 21/27 blocks traded cleanly |
| deck146 | WIN | 31 / -14 | 24 | seq 29 Teferi lands, seq 30/36 -2 dig; seq 45-55 Intrepid Adversary lifelink swings |
| deck125 | WIN | 25 / -5 | 25 | seq 47/50 — correctly attacked with ONE creature under Lightmine Field; textbook |
| deck126 | **LOSS** | 0 / 112 | 22 | seq 48, 58, 66 — three full swings into a lifegain-wall board (see H1) |

The one loss, in detail (`1788559945-ai_baka_deck152-0x55b02d2e1ff0-vs-ai_baka_deck126.jsonl`):
deck126 held 2x Perimeter Captain + Pride Guardian + 2x Wall of Omens + Overgrown Battlement.
Perimeter Captain (verified against `mtg.txt:85398-85401` AND Scryfall — engine script is
faithful) gains 2 life *per defender that blocks*, so with two Captains out a three-blocker
combat is 12 life, plus 3 from Pride Guardian. deck152 held two unblockable fliers
(Elite Spellbinder x2, opponent had no flier/reach) and added ground attackers anyway on all
three swings. Opponent life went 20 -> 32 -> 55 -> 75 -> 89 -> 95, i.e. **the seat's own
attacks were the opponent's main life engine**. The seat did play the right removal card:
seq 22 it used Elite Spellbinder's ETB to exile Exquisite Blood out of the opponent's hand;
the opponent simply drew the second copy (narration line 185). Death was the
Sanguine Bond + Exquisite Blood loop off a Staff of Nin ping at rendered turn 22 (seq 77
prompt) — deck152 has no maindeck answer to an enchantment (Skyclave Apparition is SB only),
so the loop itself was unwinnable. What was winnable was the life race, and the render is
why it was not (H1).

## 2. Engine / interface items

### H1 — HIGH. `ATTACK TOTAL:` projects a life floor that ignores the blocking-trigger lifegain the SAME prompt lists two lines above.
File `1788559945-ai_baka_deck152-0x55b02d2e1ff0-vs-ai_baka_deck126.jsonl`.

Rendered lines vs. what actually happened:

| seq | rendered `ATTACK TOTAL` floor clause | opp life before | opp life after combat | error |
|---|---|---|---|---|
| 48 | `At least 9 damage lands whatever they block - they would be at 43.` | 52 | **66** (seq 53) | +23 |
| 58 | `At least 9 damage lands whatever they block - they would be at 57.` | 66 | **80** (seq 62) | +23 |
| 66 | `At least 10 damage lands whatever they block - they would be at 71.` | 81 | **94** (seq 67) | +23 |

3 of 3 swings. The same prompt already carries the correct per-attacker fact —
seq 58 A3: `(neither dies (blocking trigger: they may gain 4)); most expensive to attack
into: Pride Guardian (0/3) (you kill it, your attacker lives (blocking trigger: they gain 3
and may gain 4 more))` — but ATTACK TOTAL is a pure damage subtraction and states a
*resulting life total* as fact. Under the trust doctrine that line is an instruction, and it
affirmatively contradicts the deck guide (which teaches exactly this matchup in caps at
lines 133-156). The model followed the render: its seq 58 PLAN reads *"Augur and Katilda will
be blocked by defenders, triggering their lifegain walls (gaining them ~2-3 life each), but
we must attack"* — it saw the tags and under-sized the price by roughly 8x, because nothing
aggregates them and the only aggregate on screen points the other way.

Fix shape (mechanics are mine, not the owner's): the floor clause must either subtract the
worst-case blocking-trigger lifegain, or stop naming a resulting life total when any A-line
carries a `blocking trigger:` tag. A separate summed line — e.g.
`BLOCK PRICE: if all N blockable attackers are blocked, they gain up to X life` — would make
the fliers-only line arithmetically obvious. Note the per-attacker tag is itself scoped to
ONE blocker; Perimeter Captain fires per *blocking defender*, so two blocked attackers can
cost far more than the sum of their two tags.

### I2 — MED. The HOLD row does not compress a repeating life-loss loop, and the decline counter calls a non-identical list "exact".
File `1788559922-ai_baka_deck152-0x56115542fe90-vs-ai_baka_deck162.jsonl`, seq 32-42:
eleven consecutive `priority` windows inside one Draw phase, all answered `CHOICE: 0 (pass)`,
**163.3 s of inference** for zero board effect while the seat's life ticked 15 -> 8. HOLD was
offered every time and never taken. Two causes visible in the render:

- HOLD promises `do not ask me again unless the board changes (any change re-opens this
  window)`. A life-total tick IS a change, so HOLD is structurally useless in exactly the
  loop windows it exists to compress.
- seq 42 renders `[you declined this exact list 10 times already this turn]` while the list
  is not textually identical — the annotation on option 1 reads
  `[DRAW PRICE: ... would be at 6]` at seq 42 vs `... would be at 13` at seq 32. Dedupe keys
  on board+question; the counter's wording ("this exact list") over-claims.

Corpus-wide for this seat HOLD was offered 125 times and taken 16 (13%), and the 15 takes in
the deck126 game skipped 265 windows (`hold_windows_skipped: 265` in that gameend) — the
feature works when taken. It is never taken on the seat's OWN turn, and the likely reason is
on screen: HOLD says "rest of this turn" while the same prompt says
`Still ahead of you this turn: BOTH your main phases`, with nothing stating that the
main-phase casting decision is not skipped. Suggest scoping HOLD to the phase, or saying
explicitly that main-phase casting windows are not covered.

### I3 — MED. Opponent creatures render with no rules text anywhere in `--- CURRENT SITUATION ---`.
26 of 26 distinct opponent creatures across this seat's six logs render as bare
`Perimeter Captain {w} (0/4) [defender]` while opponent enchantments and artifacts on the
same line carry `{effect: "..."}` (seq 58 board line: `Exquisite Blood ... {effect: "Whenever
an opponent loses life, you gain that much life."}` next to a textless Perimeter Captain).
The text is recoverable from the static decklist and from the attacker-declaration tags, but
at any decision that is not an attack declaration (blocks, casts, removal targeting) the
board snapshot alone does not say what an opponent creature does. This is the asymmetry
behind H1's magnitude misread. LOW-risk, token-cheap variant: only annotate opponent
creatures whose abilities are triggered/static and combat-relevant.

### I4 — LOW. Legend-rule annotation is correct and was still mis-planned.
Same file, seq 46. Option text:
`Cast Katilda, Dawnhart Prime {g}{w} (1/1) [legendary: you already control Katilda, Dawnhart
Prime - legend rule: casting this sends one copy to your graveyard (you choose which)]`.
Reply: `CHOICE: 1` with `PLAN: ... I have two Katildas (one 4/4, one new 1/1)`. The seat cast
a spell that could only ever bin itself and then planned around two bodies. The annotation is
accurate but is phrased as a *process* ("sends one copy to your graveyard"); an outcome-first
phrasing (`you will still control exactly ONE Katilda after this resolves`) matches the
restriction-first wording rule that fixed the summoning-sickness misread.

## 3. Guide verdict — **KEEP**

`bin/Res/ai/baka/deck152_strategy.txt` (67114 bytes, inside the 41-71 KB band) is not the
cause of the one loss and needs no edit.

- The exact losing pattern is already taught, by name, in caps, at lines 133-156:
  *"OUTNUMBERING THEM DOES NOT MAKE A LIFEGAIN WALL FREE. PERIMETER CAPTAIN gives them 2 life
  every time ANY defender they control blocks; PRIDE GUARDIAN gives them 3 whenever it
  blocks... Send only what actually gets through (Sigarda and Elite Spellbinder FLY) and
  grind with removal instead of feeding the wall."* It names both cards, names the tag string
  to look for, and names fliers-only as the line. The model read the tags (its own seq 58
  plan quotes them) and decided against the guide with a contradicting aggregate on screen.
- Adding more prose here is the failure mode the doctrine warns about: guide prose against a
  contradicting surface loses, and the surface (H1) is the thing that is wrong. Fix H1 and the
  existing teach becomes arithmetically self-evident instead of a rule to be trusted over a
  number.
- The other five games give the guide positive evidence: seq 47/50 vs deck125 is the guide's
  attack-discipline rule executed perfectly under Lightmine Field (single attacker, 5 damage,
  no self-damage), and the deck146 win used the Teferi -2 dig and Intrepid-Adversary lifelink
  lines the guide prescribes.

## 4. Proposals

None. The one real defect this seat found routes to the engine (H1), not to the general guide
or the strategy-writing skill; the deck guide already encodes the correct teach and the skill
already carries the annotation-wording doctrine that I4 and I2 are instances of. Writing
either file would restate existing amendments.

## What I did NOT check

- The five opponent seat logs beyond the deck126 blocker declarations and the deck162 loop
  context — I did not trace opponent decision quality.
- Any lane L/M/N/O/P/Q prediction adjudication (engine-seat scope); H1 is adjacent to lane L
  B11 but B11's own predicate (`can be blocked at all`, kill-claim soundness) was not tested
  here — no swing in these six games carried a `KILLS them whatever they block` clause.
- Whether the `hold_windows_skipped: 265` figure in the deck126 gameend is accurate; I only
  confirmed 15 HOLD takes in that log.
- Mana rendering, `reveal` two-arm behaviour, and sweeper/venture rows beyond spot checks —
  no anomaly surfaced, but I did not census them.
- The narration `Opponent used: Life with Perimeter Captain #N` trigger COUNT per combat was
  reconciled against the opponent's blocker declaration for turn 7 only (3 defenders blocked,
  2 Captains, 6 triggers — correct); later combats I did not re-derive.
