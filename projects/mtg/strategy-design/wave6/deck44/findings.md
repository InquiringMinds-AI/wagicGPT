# Deck 44 (UB Faerie Tempo) — Wave 6 review

Corpus: `matchups-20260713-190011/`, deck44 = **deck0 in all 6 games**.
**Record 3W-2L-1timeout** (up from wave-5's 2-4). This is the FIRST corpus on the stack-surfaced
engine (ON THE STACK section + "can target on the stack" labels shipped after wave 5).
- WON vs140 (WU control), vs110 (affinity), vs131 (izzet spells)
- LOST vs133 (mono-black), vs109 (mono-red aggro)
- TIMEOUT vs135 (snow/simic) — behind on tempo (L10/opp20 at cap), adjudicated loss.

Citations: `eEEEE sN`, `EEEE` = last 4 of the jsonl start-epoch, `sN` = seq.
`e7213`=vs140 W · `e8143`=vs110 W · `e9289`=vs135 TIMEOUT · `e1689`=vs133 L · `e4322`=vs131 W · `e4416`=vs109 L.

Severity: **P0** = game-losing observed; **P1** = large play-quality cost; **P2** = smaller.

---

## Headline: the counter seam TRANSFORMED — representation fixed the wave-5 P0

Wave 5's deciding P0 (counters fired **2/27** windows; the pilot fabricated "no spell on the stack"
over the log tail) is **fixed by the shipped representation**, exactly as wave 5 routed it. The
engine now renders, in CURRENT SITUATION:

```
ON THE STACK, waiting to resolve (top resolves FIRST - you can respond now):
  1 (top): opponent's Wrath of God [spell]
...
1. Cast Arcane Denial {1}{u} - can target on the stack: Wrath of God
```

Result across all 6 games: a counter option appeared in **14 windows; the pilot fired a counter in
13** (93%, was 7%). The one non-fire (`e9289 s10`, letting Search for Azcanta resolve) was a
defensible pass on a slow enchantment. **Zero** hallucinated-resolution replies this corpus — the
"the spell moved to the battlefield" fabrication that lost wave-5 games did not recur once the stack
was surfaced. This is the second corpus-scale confirmation (after deck110's mana line) that
**representation beats instruction**: the wave-3/4/5 tell was present in core AND guide and lost; one
CURRENT-SITUATION line fixed it.

### Counter-CHOICE quality (central question a) — good, sweeper-first is obeyed

Every fired counter, with the guide priority band it satisfies:

| window | target | band | verdict |
|---|---|---|---|
| e7213 s22 | Pyroclasm | #1 sweeper | CORRECT — saved Archmage + 2 Sprites |
| e7213 s45 | Wrath of God | #1 sweeper | CORRECT — saved the last attacker, won |
| e7213 s30 | Rakdos's Return | #4 (X, 4+ mana) | OK — hand-stripper at opp 9 |
| e8143 s51 | Etched Champion | #2 artifact creature (un-GFTT-able) | CORRECT — won the game |
| e4322 s4 | Young Pyromancer | #3 growing engine | CORRECT |
| e4322 s6 | Guttersnipe | #3 growing engine | CORRECT |
| e9289 s14 | Abominable Treefolk | #4 big creature | OK |
| e4416 s17 | Boros Reckoner | #4 (life 12) | OK — defensive vs aggro |
| e4416 s20 | Gore-House Chainwalker | #4 (life 5) | OK — survival |
| e1689 s5/s17 | Bloodghast x2 | #4 | WEAK — Bloodghast recurs from graveyard; a counter just delays it |
| e9289 s19 | Icehide Golem (2/2) | #4 | WEAK — small body, over-countered |
| **e9289 s20** | **Arcum's Astrolabe** | none | **BAD — see below** |

**The sweeper-first priority is landing exactly as designed**: both Pyroclasm and both Wrath of God
were countered on sight; Etched Champion (the un-GFTT-able artifact finisher) was countered and won
vs110. The counter CHOICES that decided games are correct.

**One genuinely bad counter target (P2):** `e9289 s20` countered **Arcum's Astrolabe** — a
1-mana cantrip artifact — with **Arcane Denial**, which (verified `auto=`: countered spell's
controller gets `draw:2`) **hands the opponent 2 cards** to stop a 1-card cantrip. Net -1 card for a
turn's mana, in a game that then timed out. This is the "reaction wasted on a target that does not
matter" failure — the counter-priority list needs a floor that STOPS the pilot spending premium
counters on cheap value.

### Did held counters win/lose games? — held counters WON the two hard matchups

vs140 and vs110 — the two matchups deck44 LOST in wave 5 to un-countered sweepers/artifacts —
**flipped to wins this wave purely on the counter seam firing** (3 counters in vs140, the decisive
Etched-Champion counter in vs110). No game was lost this corpus to a held/un-fired counter. The
capability that was dead for three waves is now the engine of the two hardest wins.

---

## MANDATORY AUDIT 1 — displacement vs overshoot on the hold-{U}{U} rule

**Verdict: the hold-mana rule is OBEYED, correctly calibrated, and did NOT displace development.**

- **Obeyed:** 13/14 counter windows fired means the pilot reached the opponent's turn with {U}{U}
  payable in those windows — it is leaving counter mana open. And payability held: every window that
  arose was affordable.
- **No under-development (the overshoot the skill warns of):** creature-casts per game — vs140 **8**,
  vs131 **6**, vs110 **5** — alongside the counters and 5/5/5 attacker-decisions. In the three won
  games the pilot deployed a full board AND held counters AND attacked every turn. The classic
  displacement risk ("durdle on holding mana while the board never gets built") did **not** occur
  where creatures were drawn.
- **The one timeout is NOT displacement — it is FLOOD/variance (F2).** vs135 cast only 5 creatures and
  got 1 attacker-decision, but the cause is the DRAW, not the hold rule: it kept a legal 2-creature
  hand, cast Miscreant (T2) and Scion (T4), then drew **nothing but lands and counters for turns
  6-14** (`e9289 s8-s14`: Play Swamp / Play Swamp / cast-nothing with 8 lands and a hand of Go for the
  Throat + Counterspell + 2 Arcane Denial). Bodies (Gatewarden, Sprite, Shadow Puppeteers) arrived
  turns 16-20, far too late. The counters it fired were fired because there was nothing else in hand.

**Conclusion:** freeze the hold-{U}{U} and cast-every-turn lines verbatim; the wave's residual is not
here. Per Step-0 OBEYED-BUT-LOSING / turnaround mode, this is a light-touch revision.

## MANDATORY AUDIT 2 — obeyed-but-losing check (before any loud rewrite)

The record ROSE (2/6 → 3W-2L-1TO) and the obedience metrics are HIGH (counters 13/14; creature-casts
5-8 in every game with bodies; cast-nothing all legitimately reactive-only hands; 0 hallucinated
resolutions; timeouts 1/6, that one flood-bound). The two clean losses attribute to **construction
and matchup, not the guide**:
- **vs133 L** — color screw: kept 3 Islands + Miscreant + counters, drew more Islands, never found
  black for Obyra/Bladecrafter/GFTT (F2). Opp finished 16 — the deck never had a plan running.
- **vs109 L** — fast mono-red aggro race; deck44 drew 3 creatures in 11 turns and lost the race.

This is turnaround mode: **freeze the obeyed lines, spend the wave only on the small model-fixable
levers (blocker floor + GFTT no-target), route the record residual to construction.** A loud rewrite
here would churn a guide that just won its two worst matchups — the deck110 anti-pattern.

## MANDATORY AUDIT 3 — convergence watch: reflexive high-life blocking

**Verdict: YES, present with evidence. This is a SECOND deck reporting it (deck109 was wave-5's single
seat) — per the promotion rule this now qualifies for a core line next wave.**

The guide has NO blocker-seam floor, so silence = block-by-default (the pattern the skill predicts).
At **9+ life** the pilot repeatedly chumped its own **racing flyers / anthem** into small attackers:
- `e9289 s4` (T3, **L20**): Faerie Miscreant (1/1, your clock) blocks Icehide Golem (2/2) — chumps a
  win-condition flyer to stop 2 damage at 20.
- `e9289 s7` (T5, **L20**): **Scion of Oona** (your team anthem: other faeries get +1/+1 and shroud)
  blocks Icehide Golem — throws away the pump engine to stop 2 at 20.
- `e1689 s47` (T11, **L20**): Scion of Oona blocks **Phyrexian Obliterator** — at 20 life, and
  Obliterator's punisher makes you **sacrifice a permanent** per point of damage dealt to it; a
  strictly negative block.
- (`e4416 s7/s12`, L15 vs aggro: more defensible — a real race — but s12 chumps Scion into a
  first-striker for a 2-life save, losing the anthem for nothing.)
- Correct block, for contrast: `e9289 s27` (L10) Oona's Gatewarden (defender, wither) blocks Icehide
  Golem — the designated blocker, named gain (wither shrinks it). The ONLY block the deck should make.

Deck-guide fix this wave (per-seam floor): at 9+ life, your flyers/Scion/Miscreant do NOT block — they
are your clock; only **Oona's Gatewarden** blocks. Core-promotion candidate flagged in
general-suggestions.

---

## F1 (P1, harness+guide) — GFTT offered with NO legal target -> 9 validation-fail decisions

**This is the "model chose Go for the Throat but it fails validation" class in stderr. It is NOT
async target-drift — the hypothesis is REFUTED.** In every instance the board had no legal
nonartifact-creature target at BOTH offer and consumption time; nothing left the battlefield in
between. Two board shapes, both grounded in the engine `auto=`/`type=`:

- **vs140 (4 stderr lines / 2 decisions):** opponent board is all lands + **Lightmine Field**, an
  ENCHANTMENT (`type=Enchantment`; `auto=@each blockers... damage:1 all(creature[attacking])`). There
  are **zero creatures**. GFTT is offered anyway; the model picks it planning to *"destroy Lightmine
  Field"* (`e7213 s27, s42`) — but GFTT is `Destroy target nonartifact **creature**`. Illegal type ->
  validation fails -> engine advances the phase (a pass).
- **vs135 (5 stderr lines / 5 decisions):** the opponent's only creature is **Icehide Golem**, a
  **Snow Artifact Creature** (`type=Snow Artifact Creature`). GFTT cannot target artifact creatures.
  Offered anyway; model picks it planning to hit the Golem (`e9289 s17, s18, s25, s26`), and with only
  GFTT left in hand it re-picks it every window it re-appears — an engine-defect **waste loop**.

**Root cause (engine):** the harness offers `Cast Go for the Throat` whenever GFTT is castable (mana
payable) **without checking a legal target exists** — violating the prompt's own contract ("the game
has already checked your mana, colors, and **targets**"). **Aftermath:** the model's pick is silently
**dropped and replaced by a heuristic pass** (`NextGamePhase`), so the JSONL `chosen_text` reads "Cast
Go for the Throat" but that is NOT what happened — a chosen!=happened artifact of the engine-no-op
family. No game was lost to it (a win and a flood-timeout), but it wastes a decision each time and, in
vs135, tied up the pilot's last card in a loop.

**Routing:** primary fix ENGINE (target-existence check before offering targeted removal). Guide
stopgap this wave: extend the existing "GFTT can't hit artifact creatures" line to name **enchantments
(Lightmine Field)** and give the no-target behavioral fallback. Filed in notes.md with repro.

## F2 (deck construction, persists from wave-5 F2) — 6 reactive cards + manabase skew

Confirmed again: the 2 clean losses are the reactive-clog / color-screw hands. The deck runs **2
Counterspell + 2 Arcane Denial + 2 Go for the Throat** in a "kill by turn 10" flyer deck, on a **16
Island / 10 Swamp** base against black-costed spells (Obyra {u}{b}, Bladecrafter {2}{b}, GFTT {1}{b}).
vs133 flooded on Islands and never cast a black spell; vs135 flooded on lands + counters with no
creatures. The counter seam is now GOOD, which *raises* the value of the counters — they won
vs140/vs110 — so wave-5's "trim the counters" is weaker now. The sharper lever is the **manabase**
(black-source count vs black spell load) and/or 1-2 more cheap creatures over the 3rd/4th reactive
card. Route to the user; not guide-fixable.

## F3 (P2, resolved) — mulligan seam healthy

Wave-5 F3 (mulliganing a keepable hand that miscounted Oona's Gatewarden as "no creature") did **not
recur**: 5 Keeps + 1 Mulligan (vs110, which then WON). vs109, the wave-5 offender, **kept** its opener
this wave (`e4416 s0`). The wave-5 mulligan line (count Gatewarden/Sleep-Cursed as your creature) is
holding. Keep it verbatim; no change.

---

## Harness-defect sweep (per skill, run first)

- **DESYNC (parser mis-grab): 0** across all 6 games — head-int of every reply equals `choice`.
- **ENGINE NO-OP (chosen-many/happened-never): none for casts that resolved.** But see F1 — the GFTT
  validation-drop is a distinct chosen!=happened artifact (pick recorded, heuristic played).
- **NUMERIC-OFFSET / X-menu: N/A** — deck44 has no X-spells; no numeric menus.
- **PLAN/CHOICE MISMATCH (wave-5 mechanism B):** **gone.** No "PLAN says counter, picks Cast nothing"
  this corpus — the labeled counter option ("can target on the stack: Wrath of God") makes the counter
  read far better than "Cast nothing," which is precisely why wave 5 routed the fix to representation.

## Wave-5 status ledger

**FIXED / now winning:** counter seam (2/27 -> 13/14, sweeper-first obeyed, 0 hallucinations, vs140 +
vs110 flipped L->W); development (5-8 creatures/game); mulligan defender-count; Sleep-Cursed untap
compressed line held (0 wasteful untaps).

**PERSISTING -> this wave's small levers:** reflexive high-life blocking (Audit 3, new blocker floor +
core candidate); GFTT no-target offer (F1, engine + guide stopgap); weak counter targets (Astrolabe,
a counter-floor line).

**RESIDUAL (construction, not guide):** 6 reactive cards + 16/10 manabase skew (F2).

## Win/loss narratives (brief)
- **vs140 W (opp -2):** deployed Sleep-Cursed x2 + Archmage + Sprites; countered Pyroclasm (s22),
  Rakdos's Return (s30), Wrath of God (s45); attacked through Lightmine Field once Scion granted
  shroud. The wave-5 P0 loss, converted to a win by the representation fix.
- **vs110 W (opp 0):** all-artifact affinity (GFTT correctly dead); countered the un-GFTT-able Etched
  Champion (s51) and raced with flyers.
- **vs131 W (opp -29):** countered Young Pyromancer (s4) + Guttersnipe (s6); 6 creatures to lethal T12.
- **vs135 TIMEOUT (L10/opp20):** land+counter flood, ~zero creatures until T16; countered the real
  threats but never built a clock. Also the reflexive-block and GFTT-loop game.
- **vs133 L (opp 16):** Island color-screw, never cast a black spell; Phyrexian Obliterator ground it.
- **vs109 L (opp 15):** fast red aggro race, 3 creatures in 11 turns; reflexive blocks didn't stem it.

## One-line handoff
The wave-5 P0 is FIXED by representation (counters 2/27 -> 13/14, sweeper-first obeyed, vs140+vs110
flipped to wins); the hold-mana rule is obeyed without displacing development; the residual is a
convergence-flagged reflexive-high-life-blocking leak, an engine GFTT-no-target offer defect, and the
standing 6-reactive-card / manabase construction skew.
