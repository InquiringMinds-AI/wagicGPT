# Deck 44 (UB Faerie Tempo) — Wave 8 review

Corpus: PRIMARY control arm `matchups-20260714-044131/`. deck44 = **deck0 in all 6 games**
(confirmed by the new `gamestart` record: `my_deck=ai_baka_deck44`).
**Record 5W-1L — again the top of the pool.** Waves 4->8: 3/6 -> 2/6 -> 3/6 -> 5/6 -> **5/6**.
- WON vs140 (WU control, 20/0 T15), vs135 (snow/simic, 18/-5 T10), vs131 (izzet, 20/-1 T14),
  vs110 (affinity, 7/-11 T18), **vs133 (mono-black devotion, 16/0 T14)**.
- LOST vs109 (mono-red aggro, 0/8 T9) only.

game->file map (via `gamestart` + gameend life/turn vs results.tsv; start-epoch order still != game order):
`1784023977`=vs109 **L** . `1784026019`=vs140 W . `1784026789`=vs135 W . `1784027271`=vs131 W .
`1784027527`=vs110 W . `1784028941`=vs133 W. Citations: `vsNNN sN`.

Posture: **FROZEN-SPINE / turnaround mode, 5th wave running.** A 5/6 top deck gets a micro-diff, not a
rewrite. Every wave-6/7 addition landed again; the corpus yields ONE small model-fixable decision-level
item plus two watches. **THE LOSS ROTATED MATCHUP** (wave 7 lost vs133 / beat vs109; this wave beat vs133 /
lost vs109) — a variance signature, not a guide regression. Win column is context only (doctrine).

---

## WHY IT WORKS — the frozen spine (protect this; all re-verified)

The engine is **cast-a-faerie-every-turn + attack-in-the-air**, protected by the **counter seam**. Every
win was again decided by holding {U}{U} and countering the opponent's board-erasing haymaker while still
deploying flyers. Re-verified this corpus:

1. **Counter DECISIONS were 15/15 CORRECT** (audit below). Sweeper-first priority and the counter-value
   floor were both obeyed with zero errors.
2. **Hold-{U}{U} is non-displacing** — in the two high cast-nothing games (vs140 12/18, vs133 8/12) EVERY
   `Cast nothing` decision had **zero castable creatures** on the menu (verified per-record): the pilot was
   holding counter mana on a reactive-half hand, not durdling past a body. vs140 was won **20/0 — the pilot
   took no damage all game**, a pure control-the-sweepers win.
3. **Development in the creature-drawing games is healthy** — vs131 cast-nothing 0/10, vs110 1/11,
   vs135 2/7; a faerie hit the board nearly every castable window.

---

## COUNTER AUDIT — the correct metric is DECISION-CORRECTNESS, not raw take-rate

The wave-7 handoff cited "13/14 offer windows" (a raw take-count). This wave the raw take-rate is **9/15** —
which LOOKS like a drop but is **15/15 correct decisions**. The counter-value floor makes correct
let-resolves show up as "declines," so a lower take-rate is BETTER play here, not worse. All 15 ON-THE-STACK
counter windows, classified:

**Correct TAKES (9):** vs140 s21 Damnation (sweeper) . vs131 s12 Young Pyromancer (engine), s20 Hydrolash
(bounce-the-attackers), s21 Cyclonic Rift (sweeper) . vs110 s6 Master of Etherium, s9 Cranial Plating,
s16 Etched Champion (un-GFTT-able artifact finishers), s22 Vault Skirge (artifact creature, pilot at L9 =
the guide's "<=12 life" catch-all) . vs133 s25 Geralf's Messenger (3/2 draining 2 on entry, protecting
lethal at opp 4).

**Correct LET-RESOLVES (6):** vs135 s14 Into the North (land search), s15 Arcum's Astrolabe (mana rock) .
vs110 s13 Ornithopter (0/2, deals 0) . vs133 s13 Bloodghast (recurs), s14 Inquisition of Kozilek (discard),
s18 Thoughtseize (discard). Each PLAN cited the value-floor reason verbatim.

**=> The wave-7 P2 watch (PLAN/CHOICE MISMATCH on labeled counter options) DID NOT RECUR: 0/15.** The
guide's plan-binding line ("If your PLAN says 'let it resolve,' your reply number must be 'Cast nothing'")
is now obeyed perfectly — the exact instrument that closed wave-7's single mismatch (vs133 s13). Consider
this watch **CLOSED for deck44** (0 across 15 windows, the largest counter sample yet).

---

## WAVE-6/7 ADDITIONS AUDIT

### 1. GFTT no-target fallback — OBEYED, and the engine option is STILL rendered targetless
deck44 held Go for the Throat all game vs133 and declined it at EVERY targetless window (s5, s7, s10, s12,
s13 — PLAN each: "no legal target"). **0 validation-drops in all six deck44 stderr.** Corpus-wide the whole
validation-drop class is DEAD: **1 total** (`land Glimmervoid`, the deliberate gate) — down from wave-7's 15
(GFTT 9 + Liliana 4 + Fatal Push 2). BUT the option is **still offered bare** — `1. Cast Go for the Throat
{1}{b}` on an empty board with NO `[no legal target]` marker (verified corpus-wide). The contract violation
persists at the menu; the 0-drop rate is the GUIDE fallback, not option-suppression. **=> NOT double
coverage — KEEP the GFTT no-target fallback, re-cite; do NOT demote.** (notes.md #1, gen-sug #1.)

### 2. Obliterator no-block absolute + GFTT-hold-for-Obliterator (wave-7 P0/P1) — UNTESTED this wave
The mono-black opponent (deck133) did NOT draw/cast Phyrexian Obliterator this game (confirmed: the opp
board never held it — earlier "obliterator" grep hits matched the GUIDE TEXT). vs133 was won as a clean
flyer-tempo + counter game. So the wave-7 fixes are **frozen, untested, KEEP** — the answer to the hard
matchup when Obliterator + a threat-dense draw appears (= the wave-7 loss; its non-appearance = the
construction/variance residual, confirmed).

### 3. Counter-value floor — held, no overshoot, no inert (6/6 correct let-resolves; see audit).

### 4. Blocking floor — obeyed; two low-harm gaps (both non-decisive)
- **`vs110 s19` (L11) — reflexive high-life block of the WIN-CON.** Archmage of Echoes (4/4, primary win
  condition) blocked Ornithopter (**0/2, deals 0**). The guide already forbids it ("at 9+ life... Do NOT
  block even to kill a 0-power attacker"); L11 >= 9, so this is a rule **present-and-disobeyed-at-distance**.
  Harm ~0 (won 7/-11). RECURRENCE of the wave-7 P2 reflexive-block watch, same deck (vs110) -> ROUTE TO
  REPRESENTATION (a `[deals 0]`/`[not lethal]` rider at the block seam), NOT louder prose. Kept per-deck
  (reinforced by naming the win-con card in the no-block line).
- **`vs109 s6` (L20) — Gatewarden's wither NEGATED by first strike (the one model-fixable item).** Gatewarden
  (2/1 wither) blocked **Ash Zealot** ({r}{r} 2/2, **first strike** — verified `abilities=first strike,haste`).
  First strike killed the 1-toughness Gatewarden BEFORE its wither landed -> Gatewarden died for zero, Ash
  Zealot survived. Better legal block: **Stromkirk Noble** ({r} 1/1, `auto=@combatdamaged(player)...counter(1/1)`
  — grows each hit; Gatewarden is a Faerie so `cantbeblockedby(human)` did not stop it): wither would have
  KILLED the growing threat (it reached 4/4, ~half the lethal clock). **Pre-computable, named-card
  opportunity conversion** -> a tight carve-out on the Gatewarden line. Non-decisive here (slow keep vs fast
  aggro), but a real leak.

---

## THE vs109 LOSS — fast mono-red aggro on a slow keep (variance/construction)

Kept hand (per the mulligan rule, correct — Sleep-Cursed counts as the creature): 3 lands + Sleep-Cursed
Faerie + Arcane Denial + 2 Counterspell. The only "creature" **enters tapped and does nothing for 3 turns**,
and the other three cards are reactive — against mono-red (Stromkirk Noble -> 4/4, Ash Zealot first-strike
haste, Gore-House Chainwalker) the deck was on the back foot from turn 2 and never stabilized. The pilot's
play was reasonable throughout (no archetype visibility at the mulligan; racing from L13 was near-hopeless).
The only micro-leak is the s6 wither/first-strike block above. Owner: **construction + variance**. The
mulligan keeping a Sleep-Cursed-only hand vs aggro is a single-instance WATCH, not an edit (tightening it
would over-mulligan good keeps in the other 5 matchups).

---

## HARNESS-DEFECT SWEEP (run first, per skill)
- **DESYNC:** 0 across all 6 games (head-int == choice everywhere).
- **ENGINE NO-OP:** none.
- **VALIDATION-DROP:** 0 for deck44; corpus-wide 1 (Glimmervoid gate). Class effectively resolved.
- **`deferred_to_heuristic` records:** none in deck44 jsonl (never picked a targetless spell).
- **NUMERIC-OFFSET / X-menu:** N/A (no X-spells).
- **PLAN/CHOICE MISMATCH:** 0 (wave-7 watch — closed, 0/15 counter windows).
- **Fallbacks/unparsed:** 0. No deck44 timeouts (all 6 in 9-18 turns).

## LAND-MENU / 2-COLOR CHECK (brief item)
The land-drop enumeration fix FIRED: **12 multi-distinct-land offers** ("Play Island" + "Play Swamp" in one
menu) across the 6 deck44 games. The pilot resolved every one CORRECTLY by reasoning from its hand (plays
Swamp when it needs {b} for Obyra/Bladecrafter/GFTT, Island when it needs {u} for the immediate creature —
e.g. vs135 s3 "Play Swamp to have black mana needed for Obyra"). **deck44's guide has NO land-color-priority
rule and NO hold-to-reach-your-color stopgap** (unlike deck131's Mountain-first) — so there is **nothing to
drop** (brief's removal condition is N/A here). No fumbled black mana observed.

## RESIDUAL (construction, not guide) — persists from waves 5-7
16 Island / 10 Swamp + 6 reactive cards (2 Counterspell, 2 Arcane Denial, 2 GFTT). The counters again
EARNED their slots (protected 4 wins). "Trim the counters" stays the WRONG lever. Sharper: manabase
black-density vs the black spell load, and/or 1-2 more cheap flyers over the 3rd/4th reactive card, to lower
the all-reactive slow-keep rate that lost vs109 and (wave 7) vs133. Route to the user. (notes.md.)

## Wave-8 status ledger
**FROZEN / re-confirmed:** counter seam (15/15 correct decisions, 0 hallucinated resolutions); hold-{U}{U}
non-displacing; counter-value floor (6/6 correct let-resolves); plan-binding (0 mismatches, wave-7 P2
CLOSED); GFTT no-target fallback (0 drops, KEEP); Obliterator no-block + GFTT-hold (untested, kept);
mulligan; blocking floor at high life (obeyed but for the two gaps).
**THIS WAVE'S MICRO-DIFF:** one carve-out on the Gatewarden block line — send it into the growing/killable
attacker (Stromkirk Noble), and wither does nothing through first strike (vs109 s6). Plus additive
observed-pool card names into the counter tables (Damnation, Hydrolash, Geralf's Messenger, Bloodghast) and
Archmage named in the high-life no-block line.
**WATCH (no edit):** reflexive high-life block of a win-con flyer at 0-power (vs110 s19 — REPRESENTATION);
Sleep-Cursed-only keep vs fast aggro (vs109, 1 instance).
**RESIDUAL (construction):** 16/10 manabase + 6 reactive cards.

## FREEZE-CHECK (every kept line accounted for)
All wave-7 load-bearing lines survive VERBATIM: the #1 cast-a-creature rule; the engine (Obyra/Archmage/
Bladecrafter incl. the "life -2 is a bonus" display note); the counter priority list + counter-value floor +
BOTH plan-binding sentences + hold-{U}{U}/leave-two-blue; the GFTT block (Obliterator target) + no-target
fallback; the Obliterator NEVER-block standalone absolute + its chump/Gatewarden exceptions; the mulligan;
all card notes; the SITUATIONS recap. CHANGES (additive only, no reword of an obeyed line): (a) Gatewarden
block-selection carve-out [new bullet]; (b) "Archmage of Echoes" + "Archmage's growth" added to the 9+
no-block list [reinforce a distance-disobeyed line with the named card]; (c) Damnation/Geralf's Messenger/
Hydrolash added to the counter tables and Bloodghast to the let-resolve list [match the observed pool];
(d) one positive "cheap spell -> let resolve -> Cast nothing" line + the first-strike note in the SITUATIONS
recap. Displacement audit: ~+10 lines net, nothing removed, still ~one screen at deck44 reference density.

## One-line handoff
The top deck's spine (hold {U}{U}, sweeper-first counters, cast a faerie every turn) is FROZEN and proved
itself again at 5-1 with 15/15 correct counter decisions and 0 plan/choice mismatches (wave-7 watch
closed); the loss rotated to fast mono-red on a slow reactive keep (variance/construction), and the whole
model-fixable diff is a single Gatewarden block-selection carve-out (block the growing Stromkirk Noble,
never waste wither into a first-striker).
