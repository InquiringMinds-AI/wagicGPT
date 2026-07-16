# deck131 — wave 13 review (UR Guttersnipe spellslinger; "Mind Control / Best Counter Deck Ever")

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-183228/` (wave-13b, first CLEAN
post-crash validation). Binary `/tmp/wagic-a75e2625e` (Force r2/r3 + auto-tap two-pass +
**c3e6d95b2 name-echo protocol** + **42e8bc180 BLOCKS:none** + 240s timeout). Spark ran at
GPU_MEM_UTIL=0.75 → high decision latency (med 11.2s / p90 87s); 13/21 corpus games life-adjudicated
at the 2400s cap. Wave-11 guide FROZEN (byte-identical, 9036 bytes) and remains so. 6 deck131 games.

**Record: 2 W / 4 L (2/6).** BOTH wins are LIFE-ADJUDICATION wins (ahead on life at the latency
cap), NOT kills. Seat→game map (results.tsv; deck131 = deck0 or deck1 by row):

| result | vs (deck) | file epoch | end life me/opp | turn | cause |
|---|---|---|---|---|---|
| **WIN (adj)** | 135 (Modern Snow)    | 1784162578 | 25 / 8  | 16 | YP token swarm (guide 2nd-win-path) + lifegain pad; **NO Guttersnipe** |
| **WIN (adj)** | 140 (lifegain)       | 1784168494 | 31 / 29 | 34 | grind-stall; 2x Guttersnipe, countered Wrath of God, lifegain edge |
| LOSS (adj) | 133 (Phyr. Asphodel) | 1784163693 | 18 / 22 | 6  | latency-adj at T6 caught a slow dig hand w/ no payoff drawn; behind on life |
| LOSS (death -6) | 109 (mono-red aggro) | 1784160756 | -6 / 14 | 15 | aggro race; YP killed T6, Guttersnipe drawn T15 (too late) |
| LOSS (death -2) | 110 (Etched Affinity)| 1784160752 | -2 / 16 | 9  | affinity outraced a lone YP; Guttersnipe never seen |
| LOSS (death 0/-2) | 44 (Faerie Archmage) | 1784167378 | -2 / 19 | 14 | recurring Faerie Bladecrafter flyer (3/3→5/5) beat down; no evasion answer |

**Bottom line: the frozen guide is again VALIDATED on every axis it targets and the two engine-fix
wins this corpus were BOTH confirmed at this seat — bare `BLOCKS: none` now PARSES CLEAN (wave-11 H1
CLOSED), and name-echo is 100% with ZERO wrong-remaps. The 4 losses are the standing
construction-bound race/tempo terminus (7th wave), no guide-addressable decision class. Two NEW
decision/representation items surfaced: (F1) a FIRST mulligan STEP-1 gate MISFIRE in 7 waves
(kept a no-red/no-dig hand, won on variance) — a model-ADHERENCE miss on an already-maximally-explicit
line, NOT a guide gap; (F2) the attacker name→index parse gap the brief flagged is PRESENT here with
5 instances and a measured cost (silent partial attacker-drop + heuristic fallback on the win-game
alpha strike). strategy.txt = byte-identical freeze (9036 bytes, `cmp` clean).**

---

## ENGINE-FIX VERIFICATION (brief's confirmed-fired doctrine) — both wins confirmed at this seat

### VF1 — bare `BLOCKS: none` now PARSES CLEAN. Wave-11 H1 (the fatal Guttersnipe-trade bug) CLOSED.
The wave-11 headline defect was that bare `BLOCKS: none` was systematically UNPARSED → fell to the
baka blocker heuristic, which at vs109 blocked Guttersnipe into a fatal trade. **This wave the fix
(42e8bc180) is confirmed: `BLOCKS: none` and `BLOCKS: None` both parse to `chosen_text='no
blockers'` with `fallback=None`.** Repros: deck109 s7 (`BLOCKS: none` → "no blockers", clean),
deck109 s12 (`BLOCKS: None` → "no blockers", clean). The natural all-decline reply is now honored;
the baka heuristic is no longer consulted on a cleanly-reasoned decline. CLOSED.

### VF2 — name-echo protocol: 163/163 CHOICE lines carry "(echo)"; ZERO wrong-remaps (the HIGH case).
100% echo compliance (163/163). The brief's HIGH concern — an echo naming a DIFFERENT LISTED option
than the index, remapped WRONG — **did not occur at this seat: 0 instances.** Classification of all
163 CHOICE lines: 122 echo-matches-index (clean); the remainder are target-selection/descriptive
echoes that match on inspection. The reconcile arm was never invoked to remap because the only
mis-echoes named UNLISTED cards (see F3), so index-wins-on-conflict fired as designed and prevented
all damage. Name-echo is working.

### VF3 — Force of Negation: N/A at this seat (deck131 runs no Force of Negation). No dead-end picks.

### VF4 — auto-tap stranding: LOW exposure at this seat, no observed misfire. deck131's mana is
14 Island / 8 Mountain (basics only, no duals) so pip accounting is unambiguous. All generic+blue
casts (Artificer's Epiphany {2}{u}, Opportunity {4}{u}{u}, Hydrolash {2}{u}, Cyclonic Rift {1}{u},
Dissipate {1}{u}{u}) resolved when the model attempted them; no "affordable cast declined for lack
of mana" or "attempted an unaffordable cast" event traced to stranding. deck44's stranding class is
not reproduced here (different land set). No contribution to the batch item from this seat.

### VF5 — Mountain-first land execution: 15/15 co-offers. FREEZE (5th corpus).
15 windows offered BOTH "Play Mountain" and "Play Island"; chose a Mountain option 15/15, including
11 windows where "Play Island" was listed FIRST (picks by NAME, not index). Now clean across
wave-8/9/10/11/13. CLOSED.

---

## F1 — [DECISION / MODEL-ADHERENCE, NEW, single instance in 7 waves] mulligan STEP-1 gate MISFIRE: kept a no-red/no-dig hand by misciting the guide (vs135 s1). Won on variance.

**First mulligan-gate deviation in 7 waves** (wave-11 was 7/7). deck135 s1 opening hand:
`Prism Ring; Island; Island; Counterspell; Essence Scatter; Island; Young Pyromancer` — **no
Mountain, no Artificer's Epiphany, no Opportunity.** The guide STEP-1 gate is absolute and names this
exact hand: *"If the word 'Mountain' does NOT appear anywhere in it AND the hand has no Artificer's
Epiphany and no Opportunity, MULLIGAN — full stop. It does not matter that the hand has a Young
Pyromancer... The thought 'I'll keep this and cast my creature once I draw a Mountain' is a TRAP."*

The model **KEPT** it, and its reasoning (s1 reply) both misquoted the guide and invoked the named
trap verbatim:
> "While it lacks a Mountain, the presence of Young Pyromancer combined with multiple Islands... is a
> strong keep; we can cast our counters if needed and **hope to draw a Mountain** or a red spell...
> The strategy guide **allows keeping hands with draw spells or payoffs** even without immediate red."

Two errors: (a) it conflated "payoff" (Young Pyromancer) and "counters" with the STEP-1 exception,
which covers ONLY the two named DRAW spells; (b) it used "hope to draw a Mountain" as justification —
the precise thought the guide flags as a TRAP. The hand then DREW a Mountain on T3 (s5) and won the
game (the YP-swarm win, VF-relevant below) — pure variance, the outcome the guide warns is unreliable
("off 8 Mountains in 60 you often never draw the red").

**Attribution: MODEL-ADHERENCE, not a guide gap.** The guide is already maximally explicit — it
states the rule "full stop," names the YP counter-example, and names the trap. Adding louder text is
the sanctioned-against escalation. Route: WATCH + a reviewer method note (skill.md). Single instance
in 7 waves; do NOT touch the frozen guide off one variance-rewarded misfire. Flagged prominently
because the mulligan gate has been the seat's most-cited "validated/frozen" line and this is its
first crack (possible name-echo-era model-behavior drift; re-check next corpus).

## F2 — [ENGINE/HARNESS, GAME-RELEVANT] attacker name→index parse gap (the brief's `ATTACK: Hellrider` shape) is PRESENT here: 5/18 attacker declarations leak names. Root fix = attackers name→index reconcile arm.

The attacker protocol wants A-indices (`ATTACK: A1, A2`). 18 attacker declarations this seat split:
- **13 PURE A-index** → clean.
- **3 MIXED (`ATTACK: A1, <name>`)** → only the A-index parses; the NAME token is **silently
  DROPPED** → fewer attackers than intended. Repros (all resolve to just the A1 creature):
  - deck110 s13 `ATTACK: A1, Elemental` → chosen "Young Pyromancer" only (Elemental left home)
  - deck109 s11 `ATTACK: A1, Elemental (1/1)` → "Young Pyromancer" only
  - deck135 s9 `ATTACK: A1, Elemental` → "Young Pyromancer" only
  In each, a live Elemental token (spawned by a YP trigger that turn) existed and was omitted from
  the swing — a silent MISSED ATTACKER (small, ~1 power each, but a real cost and exactly the
  "attack with EVERY creature" the guide mandates).
- **2 PURE NAME (no A-index)** → whole reply `unparsed_reply` → baka heuristic. Repros:
  - deck109 s28 `ATTACK: Elemental`
  - **deck135 s29 `ATTACK: Elemental (1/1), Young Pyromancer, Elemental (1/1), Elemental (1/1), ...`
    — the WIN game's final 5-attacker alpha strike, unparsed → fell to the heuristic.** deck131 still
    won by adjudication, but the intended lethal-shaped swing was not executed as authored.

This is the same root as the brief's `ATTACK: Hellrider` candidate (a NAME where the protocol wants
A-indices). Fix: an attackers name→index reconcile arm mirroring the name-echo/parseChoice reconcile
(map each name token to its unique battlefield A-index; drop only on ambiguity). HIGH ledger priority
— it is the only GAME-RELEVANT engine gap at this seat and it degrades the guide's core
attack-with-everyone line. Route: notes.md ledger, NOT a guide change.

## F3 — [REPLY-PROTOCOL / MODEL-COMPREHENSION, WATCH] model mis-echoes CHOICE index with a card NOT in the option list (wave-11 H3 continuation) — now CONTAINED by index-wins, benign this wave.

6 clean instances of `CHOICE: N (<card-not-in-options>)`: the model labels an in-range index with a
card that isn't listed, usually a payoff/instant it wishes it could cast:
- deck110 s3, deck44 s3, deck44 s4, deck140 s3: `CHOICE: N (Cast Young Pyromancer)` at T1 — YP not in
  options (uncastable, 1 land) → index landed on "Cast nothing" (correct outcome).
- deck135 s24: `CHOICE: 1 (Cast Hydrolash)` — Hydrolash not listed → index 1 = Elixir cast (benign;
  padded life for the adjudication win).
- deck140 s41: `CHOICE: 2 (Cast Artificer's Epiphany)` — not listed → index 2 = "nothing".

This is exactly wave-11 H3 ("plan fixates on an UNLISTED card and emits a poorly-mapped index"), but
the name-echo era changed its FAILURE MODE: in wave-11 it produced an OUT-OF-RANGE index → unparsed;
now the index is in-range and index-wins-on-conflict contains it to a benign result (the echoed card
is unlisted → no unique remap → index wins). **No game impact this wave** (every mis-echoed card was
uncastable and the index landed on nothing/lifegain). WATCH; the durable root is model comprehension
(believing an unlisted/uncastable card is available at index N). NOT a guide item; below the bar.

## F4 — [ENGINE/REPRESENTATION, single-seat, VERIFY] blocker legend omitted a legal untapped blocker (the Elemental) at vs109 s29 → model forced B1 and the engine blocked GUTTERSNIPE (lethal spot, 0 game impact).

vs109 s29 (T16, deck131 at 4 life facing a lethal 7-attacker swing — dead regardless). The blocker
legend in the prompt listed ONLY `B1. Guttersnipe (2/2)`, yet the battlefield line showed an
**untapped Elemental (1/1)** present (`creatures: 2`, neither tapped). The model reasoned the guide's
absolute correctly and at length ("I cannot block with Guttersnipe... I have an Elemental (1/1) that
can block"), but with no Elemental offered it forced `BLOCKS: B1:A4` believing B1 was the Elemental —
and the engine executed B1 = Guttersnipe (`chosen_text='Guttersnipe blocks Goblin'`). deck131 died
-6 this turn either way, so **0 game impact**, but it is a representation-integrity defect: a legal
untapped blocker was absent from the blocker menu, and the model's determination to block with the
(unoffered) Elemental drove it onto the protected creature. VERIFY caveat: confirm whether the engine
had a reason to exclude the Elemental (it had attacked T13, should have untapped by T16 — no visible
reason). Route: notes.md ledger (engine blocker-enumeration), with the verify caveat. Do NOT touch
the guide (the decision reasoning was correct; the menu was wrong).

---

## Loss failure class — coherent, construction-bound, 7th wave. NO guide-addressable decision defect.

All 4 losses share the standing terminus: the pilot plays the guide correctly and loses to
construction limits, plus a latency-adjudication artifact in one:
- **vs109 / vs110 (aggro/affinity races):** deck131 fielded a lone YP that was removed (Searing Spear
  vs109 T6) or outraced (Memnite 6/1 + Signal Pest affinity vs110); reach (Guttersnipe) either
  arrived T15 into a dead board or never appeared. No early defense/blockers vs faster decks —
  construction.
- **vs44 (faerie tempo):** deck131 bounced the recurring Faerie Bladecrafter three times
  (Downsize/Cyclonic Rift ×2) but it kept returning bigger (3/3→4/4→5/5 flyer); no permanent answer
  to an evasive recurring threat — construction.
- **vs133 (latency-adj at T6):** the 2400s cap fell at T6 while deck131 was in a slow dig hand
  (2× Elixir + lands + one Artificer's Epiphany, no payoff creature in the opener) and behind on life
  18–22. The "Cast nothing" turns were correct (Artificer's needs blue it hadn't yet played; only
  lifegain was castable and correctly declined). This is a draw/latency artifact + a marginal-but-
  guide-legal keep, not a decision defect.

Per the pause-aware demotion doctrine and the win-is-context doctrine, none of this reaches the HIGH
adopt-a-revision bar. The two wins came from CONSTRUCTION/variance (vs135 drew into red off a hand it
should have mulliganed and swarmed; vs140 out-lifegained a lifegain deck to a 31-29 cap edge), not
from guide prose.

## Routing summary
1. **[GUIDE — FREEZE, byte-identical]** strategy.txt = live wave-11 frozen guide, 9036 bytes, `cmp`
   clean. No guide-addressable defect; the two new items (F1 adherence, F2 engine) route below/around
   the guide.
2. **[ENGINE/HARNESS — GAME-RELEVANT]** F2: attacker name→index parse gap, 5 instances (3 silent
   partial-drop + 2 unparsed incl. the win-game alpha strike). Root fix = attackers name→index
   reconcile arm. notes.md.
3. **[MODEL-ADHERENCE — WATCH, prominent]** F1: mulligan STEP-1 misfire (kept no-red/no-dig, miscited
   guide, won on variance). Guide already maximally explicit. notes.md + skill.md method note.
4. **[REPLY-PROTOCOL — WATCH]** F3: mis-echo of an unlisted card at an in-range index; contained by
   index-wins, benign. wave-11 H3 continuation. notes.md.
5. **[ENGINE/REPRESENTATION — VERIFY, single-seat]** F4: blocker legend omitted a legal untapped
   blocker (Elemental) at vs109 s29; lethal spot, 0 impact. notes.md.
6. **[DECK construction — RESTATED, not relitigated]** notes.md.
7. **[CORE — PASS]** no 2+-seat core candidate at this seat. general-suggestions.md = PASS.
