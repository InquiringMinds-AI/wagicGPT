# deck131 — wave 8 review (UR Guttersnipe spellslinger; "Mind Control / Best Counter Deck Ever")

Corpus: PRIMARY control arm `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260714-044131/`, one
GPT-vs-GPT round-robin, qwen35 both seats, wave-7 guide in play, wave-7 engine ledger shipped in
the binary. **6 deck131 games.** Live guide = the wave-7 revision (`bin/Res/ai/baka/deck131_strategy.txt`:
two-gear identity, Guttersnipe cast-every-turn #1, Mountain-first + interim hold-to-reach stopgap,
literal-string mulligan trigger + keep-side guardrail, attack/no-block, NEVER-block-Guttersnipe,
draw-on-curve, Prism/Elixir lifegain-only).

**Record: 1 W / 4 L / 1 TIMEOUT (1/6).** Fourth straight bottom-tier wave (5:0/6, 6:1/6, 7:1/6).

Seat-file -> game map (from the NEW `gamestart` record — no more epoch arithmetic):

| result | vs | file (epoch) | recs | end life me/opp | one-line cause |
|---|---|---|---|---|---|
| WIN     | 135 | 1784022098 | 55 | 18 / -3  | Young Pyromancer + Elemental **token swarm** (no Guttersnipe) |
| TIMEOUT | 140 | 1784024389 | 93 | 33 / 24  | lifegain+Pyroclasm stall; Guttersnipe swept 3x |
| LOSS    | 109 | 1784025145 | 21 | 0 / 20   | mono-red aggro (Stromkirk+Hellrider+Goblins) outraced it; opp at 20 |
| LOSS    | 110 | 1784026638 | 32 | -8 / 18  | affinity (Master of Etherium) crushed it |
| LOSS    | 133 | 1784027267 | 34 | -2 / 16  | zombies (Geralf's Messenger) outraced it |
| LOSS    | 44  | 1784027271 | 26 | -1 / 20  | **kept a 0-Mountain no-draw hand** -> drew red T7 -> outraced; opp at 20 |

Opponent life at death in the four losses: 20, 18, 16, 20 — all high. The multi-wave signature
(opp near 20 at death = deck131 never ran a clock) recurs. **OBEYED-BUT-LOSING, fourth wave.** But
this wave the headline was different: the wave-7 REPRESENTATION headline (land menu) LANDED and is
confirmed obeyed end-to-end — the one high-leverage lever closed. Freeze the obeyed lines; the guide
changes are three small, low-churn items.

Transport clean: **0 desyncs, 0 defers (`deferred_to_heuristic`), 0 `fails validation`** across all
6 games and both stderr sets.

---

## HEADLINE — the wave-7 land-menu fix FIRED and is OBEYED 12/12 (verify + drop the stopgap)

Wave 7's headline was that "Mountain-first" was UNEXECUTABLE: the engine offered lands one at a time
as `Play <land> / Hold <land>` and NEVER co-offered two lands (0/6 games). The wave-7 engine ledger
shipped land-drop enumeration. **It fired for deck131:**

- **Multi-land offers (both `Play Mountain` and `Play Island` in one option list): 12** across the
  6 games (vs135 2, vs140 5, vs109 2, vs110 2, vs133 1, vs44 0). Wave-7 had ZERO.
- **Mountain-first execution: 12 / 12.** Every decision where both were co-offered, the pilot chose
  `Play Mountain` — including when Mountain was NOT first in the list (vs135 seq2 opts
  `[Play Island, Play Mountain, Play no land]` -> Mountain; vs110 seq5 same shape -> Mountain). The
  pilot picks by NAME, not position. The wave-6/7 guide rule now EXECUTES.
- **First Mountain played, per game:** vs135 T1, vs140 T1, vs109 T0, vs110 T0, vs133 T0 — five of six
  on curve. The one late Mountain (vs44 T7) had **no Mountain in the opening hand** (0 multi-land
  offers there because there was no Mountain to co-offer) — a DRAW/mulligan story, not the interface.

**Verdict: the interim "hold non-Mountain lands to reach the Mountain" stopgap's removal condition is
MET (fix fired AND obeyed).** DROP it. It is now double coverage — pure attention tax — and the "HOW
THE LAND MENU WORKS (read this — it is not obvious)" paragraph describes an interface that no longer
exists (a stale caution that reinforces the wrong posture, Step 0). The LANDS block collapses to one
line: when both are offered, pick Play Mountain.

## RE-WEIGH the manabase flag now that the interface is fixed (softens from #1)

With the interface fixed, actual red-screw is much reduced — color reached the board on curve in 5/6
games. The remaining color problem is narrower:

- **vs44 (first Mountain T7)** was a **mulligan error**, not the manabase and not the interface: a
  0-Mountain, no-draw-spell opener was KEPT (below), then red arrived on natural draw T7.
- **vs109 (Mountain T0), vs110 (T0), vs133 (T0)** all had red on time and lost to FASTER or HARDER
  boards (mono-red aggro, affinity, zombies), not to color. deck131 has no second act after its first
  threat is answered or outpaced.

Dominant construction issue is no longer color — it is **threat density 6/60 (3 Guttersnipe + 3 Young
Pyromancer)** and **no reach vs lifegain**. The 14I/8M manabase still leans slightly wrong for all-red
payoffs; a small shift (11M/11I) is cheap and harmless, but it drops from #1 to secondary — the
interface fix already recovered the color games it was blamed for. Restated in general-suggestions.md.

## MULLIGAN — the wave-7 false-mulligan is GONE, but a NEW false-KEEP appeared (guardrail inversion)

Two-direction tally (literal-string trigger + false-positive guardrail from wave 7):

| vs | opener | Mtn | correct | pilot | verdict |
|---|---|---|---|---|---|
| 135 | Isl, YP, Downsize, Mtn, Elixir, Isl, Elixir | 1 | KEEP | Keep | ok |
| 140 | Isl, Isl, Isl, EScatter, Prism, Downsize, YP | **0** | MULLIGAN | Mulligan | ok |
| 109 | Mtn, Counter, Mtn, Prism, Isl, Downsize, YP | 2 | KEEP | Keep | ok |
| 110 | Mtn, Isl, Prism, EScatter, Mtn, Downsize, YP | 2 | KEEP | Keep | ok |
| 133 | Isl, Elixir, Mtn, Downsize, Isl, EScatter, YP | 1 | KEEP | Keep | ok |
| 44  | Isl, YP, CycRift, Isl, Isl, Prism, CycRift | **0** | MULLIGAN | **Keep** | **FALSE KEEP** |

- **Keep-side guardrail (wave-7's fix for the false-mulligan): HELD.** Keepable Mountain-hands kept =
  **5/6**; the wave-7 false-mulligan (a 2-Mountain hand shipped as "zero Mountains") did NOT recur (0
  this wave). The literal-string trigger works in that direction.
- **NEW failure — no-Mountain mulligan trigger MISFIRED once (vs44, false keep).** True no-Mountain
  openers = 2 (vs140, vs44); correctly mulliganed = **1/2**. The vs44 opener has NO Mountain and NO
  draw spell (Cyclonic Rift is neither Artificer's Epiphany nor Opportunity) -> a clear MULLIGAN. The
  pilot KEPT it. Reply (seq1) READ the guide correctly, then talked itself out of it: *"the guide
  says: 'Mulligan ONLY when NO Mountain appears...'. I have no Mountain. However... 'The ONE
  no-Mountain KEEP: a hand holding Artificer's Epiphany OR Opportunity...'. I do not have those. But I
  have Young Pyromancer. The guide says: 'A hand with a Mo[untain and a Young Pyromancer is ALWAYS a
  keep]...'"* — it grabbed the keep-side guardrail sentence, DROPPED the "Mountain and," and let a lone
  Young Pyromancer authorize the keep. **The keep-side guardrail added in wave 7 to stop false
  mulligans INVERTED into a false keep** — the exact over-generalization the skill warns about (a
  corrective fact pattern-matched past its scope; keyed on the salient "Young Pyromancer ... ALWAYS a
  keep").
  - **Cost:** the kept 0-red hand drew its first Mountain T7, stranded Young Pyromancer, opp reached
    20, loss at T14.

**Fix (guide, low-churn): make the red-source gate STRICT and un-overridable, remove the phrasing a
payoff can hijack.** STEP 1 — is the word "Mountain" in the hand list? If NO, and no Artificer's
Epiphany/Opportunity, MULLIGAN — full stop, no matter what creatures the hand holds. Only a hand that
PASSES the red-source gate reaches keep considerations. Re-phrase the keep-side guardrail so "Young
Pyromancer" never stands alone as a keep trigger (Mountain is the subject: "a hand that HAS a Mountain
is a keep if it also has a payoff or draw spell"). Track BOTH directions next wave: `(no-red openers
mulliganed)/(no-red openers)` = 1/2, `(keepable Mountain-hands kept)/(keepable)` = 5/6.

## GUTTERSNIPE — cast-every-turn obeyed where testable; NEVER-block held; one protect-the-payoff miss

- **Board presence: only vs140** (the lifegain+Pyroclasm stall). Guttersnipe cast 3x there and
  **Pyroclasm swept it each time** (T11, T17, T35) — a 2/2 into a repeated `deal 2 to all creatures`.
  In the other 5 games Guttersnipe was never cast (not drawn / not castable in time). Precondition rare
  AGAIN (1/6 games). The vs135 WIN was pure **Young Pyromancer token swarm** — no Guttersnipe all game.
- **Cast-every-turn while Guttersnipe out: OBEYED.** A spell was cast as fuel each window (events show
  `Damage: 2 dealt by Guttersnipe to the opponent` T17). **0 cast-nothing while Guttersnipe out.** (The
  41 raw cast-nothing decisions corpus-wide were almost all "only castable is Elixir/Prism Ring" —
  declining lifegain, which the guide COMMANDS. Guide-compliant, not leaks.)
- **NEVER-block-with-Guttersnipe: HELD.** The wave-7 violation (vs110, Guttersnipe chump at 14) did
  NOT recur; Guttersnipe never blocked. All actual blocks were sound Elemental chumps (vs Hellrider at
  14 while raced; vs Master of Etherium; vs Geralf's Messenger). No reflexive high-life blocks. Attack
  participation 100% (vs135 grew YP -> YP+Elemental -> YP+4 Elementals against the full `A#.` list).
- **NEW decision failure — declined available protection for the payoff (vs140 seq51 T34).** Pyroclasm
  on the stack about to kill a just-cast Guttersnipe; the pilot had **Dissipate up** (option
  `Cast Dissipate {1}{u}{u} - can target on the stack: Pyroclasm`) and chose **nothing**. Reply:
  *"Pyroclasm will deal 2 damage to all creatures (including my Guttersnipe) and destroy my Guttersnipe
  if it has toughness 2 or less. However, Guttersnipe has 2 tough[ness]..."* — it correctly identified
  the threat, then made a **rules-arithmetic error** (2 damage does not kill 2 toughness) and let its
  only reach die. A clean instance of the frozen line ("point counters at removal aimed at
  Guttersnipe") failing at a RECOGNITION gap: a sweeper is not read as "removal aimed at Guttersnipe,"
  and `damage >= toughness = dead` is a false-world-model belief.
  - **Fix (guide): sharpen the existing counter line** with the flat falsity-fact tied to the visible
    card — "Guttersnipe is a 2/2: any 'deals 2 (or more) to all creatures' spell (Pyroclasm, a board
    wipe) on the stack KILLS it — that IS removal aimed at Guttersnipe; counter it." A clause on a
    frozen line, no new paragraph. The general misconception (N damage kills N toughness) is routed to
    general-suggestions.md as a single-seat core WATCH, not a core edit.

## FROZEN-LINE RECHECK (obeyed-but-losing discipline — verify obedience, do not churn)

- **Guttersnipe cast-every-turn #1** — obeyed where precondition existed (vs140); 0 cast-nothing while
  out. Precondition rare (1/6). FREEZE verbatim.
- **Two-gear identity (race/dig)** — obeyed. FREEZE.
- **Deploy-payoffs / cast-creature-first** — obeyed (YP down on curve every game it was drawn). FREEZE.
- **Attack-every-creature / no-block-while-racing** — 100% attack participation; blocks correct chumps
  only. FREEZE.
- **NEVER block with Guttersnipe** — no violation. FREEZE (precondition rare; keep as the per-deck
  floor / reflexive-block convergence-watch instance).
- **Mountain-first** — EXECUTES 12/12. FREEZE the pick rule; DROP the interim stopgap + stale
  one-at-a-time-menu explanation (removal condition met).
- **Reactive / draw / Prism-Elixir-lifegain / lethal-check** — obeyed (every Prism/Elixir cast a
  "nothing better" window; every cast-nothing-over-Elixir correct). FREEZE; ADD the sweeper clause to
  the counter line only.
- **Winning-line block** — the self-cited turn-by-turn win is now stale (it describes a
  double-Guttersnipe close; this wave's WIN was a Young-Pyromancer token swarm to 18/-3, no
  Guttersnipe). UPDATE to the vs135 line per the skill's "delete self-citation of a game that no longer
  exists"; keep the Guttersnipe grind as the alternative close.

## Harness / engine sweep (required)
- DESYNCS: 0. ENGINE NO-OPS: none. DEFERS: 0. `fails validation`: 0.
- **LAND-MENU representation defect: RESOLVED** for this seat (12 multi-land offers, 12/12 obeyed). The
  wave-7 top ask is CLOSED.
- **TIMEOUT (vs140) now writes life/turn in results.tsv** (adjudicated) — the wave-4->7 ask is
  partially answered; a per-seat `gameend` on timeout is still absent in the jsonl. Minor; noted.

## Routing summary
1. **[GUIDE — done this wave, low-churn]** (a) collapse LANDS block, DROP the interim stopgap + stale
   menu explanation (fix fired 12/12); (b) STRICT red-source mulligan gate + de-hijack the keep-side
   guardrail (vs44 false keep); (c) sweeper-recognition clause on the counter line (vs140 protect-payoff
   miss); (d) update the stale winning-line to the vs135 YP-swarm win.
2. **[DECK construction — softened]** threat density 6/60 and no reach vs lifegain are the dominant
   construction issues; the 14I/8M manabase drops from #1 (interface fix recovered the color games) to
   a cheap secondary tweak (11M/11I). Cut Prism Ring for reach (Spellheart Chimera #SB).
3. **[CORE — WATCH only, single-seat]** `damage >= toughness = dead` false belief (vs140 seq51). Do
   NOT promote on one seat; carry as a rules-fact candidate for the core owner.
4. **[DECK/matchup — hard/unwinnable]** vs140 lifegain+Pyroclasm, vs110 affinity, vs109 mono-red
   aggro, vs133 zombies. n=6 = noise; route the record to construction/tiers, not the guide.
