# Notes for wagicGPT development — deck44 wave 10 (self-contained; full repros)

Run: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-031825/`. deck44 = deck0 in all 6
games (confirmed via `gamestart`). **Record 4W-2L.** First corpus under the ANSWER-AFTER-PLAN reply
protocol (PLAN then CHOICE:/ATTACK:/BLOCKS: last), binary `/tmp/wagic-c8c054be8`, max_tokens 2048.
Corpus health for deck44: 0 desyncs, 0 engine-no-ops, 0 validation-drops, 0 timeouts; 3 benign
`unparsed_reply` fallbacks + 1 truncation-parse self-cast (all game-neutral, detailed below).
Win column is context only (doctrine).

game->file map (by `gamestart` opp_deck + gameend life/turn):
- `1784103510` = vs110 **LOSS** (0/20 T9)   | `1784103950` = vs133 WIN (12/0 T17)
- `1784105766` = vs109 WIN (2/-14 T14)       | `1784108791` = vs135 WIN (2/-3 T16)
- `1784112264` = vs131 WIN (20/-3 T12)       | `1784112898` = vs140 **LOSS** (16/13 T39)

## Guide change this wave: NONE (frozen byte-identical)
The deployed wave-9 guide (md5 `bd097b0c4cf8524fb6cc30b75312cabe`) is re-shipped byte-identical. The
wave-9 model-fixable item (counter-priority inversion) is fixed and VALIDATED this corpus (see below);
no new decision-level failure class emerged. The two losses are construction/variance + an engine
auto-tap defect, neither guide-fixable.

---

## ENGINE-LEDGER ITEMS (layer-routed here, NOT prompt text)

### #1 (NEW) — counter options mis-render "- NO legal target right now" over an OCCUPIED stack
The Counterspell / Arcane Denial cast option renders the clause `- NO legal target right now` even
when a spell IS on the stack, and the SAME option line ALSO contains `can target on the stack:
<name>`. The two clauses contradict: a counterspell whose target is a stack-spell HAS a legal target.
Almost certainly the "no legal target" annotation logic only inspects permanent/creature targets and
does not recognize a stack-spell as a legal target, so it wrongly prints "NO legal target" while a
separate code path correctly lists the counterable spell.

Repros (deck44 seat, `chosen_text` shows the malformed option text):
- vs110 `1784103510` s7:  `Cast Arcane Denial {1}{u} - NO legal target right now - can target on the stack: Cranial Plating`  (Cranial Plating IS `1 (top)` on the stack) — pilot COUNTERED correctly.
- vs110 `1784103510` s12: `... NO legal target right now - can target on the stack: Master of Etherium` — COUNTERED.
- vs140 `1784112898` s24 (Wrath of God), s29 (Lightmine Field), s69 (Staff of Nin) — same malformed clause, all COUNTERED correctly.
- vs109 `1784105766` s17 (Legion Loyalist on stack) — same malformed clause.

Impact THIS corpus: ZERO misplays — the pilot ignored the false "NO legal target" clause every time,
trusting the ON THE STACK section + the "can target on the stack: X" clause, and countered when it
wanted to (15/15 counter windows correct). But it is a LATENT risk: a weaker moment could read "NO
legal target" and decline a correct counter (this is exactly the class the own-targets annotation was
built to PREVENT, now mis-firing on the counter path). FIX: suppress the "NO legal target right now"
clause when the counter option has a legal spell target on the stack (i.e., when `can target on the
stack:` is non-empty, the target set is non-empty by definition). Owner: the option-annotation code
(same area as the wave-9 own-targets / wave-10 named-target GFTT annotations).

### #2 (NEW, GAME-CHANGING) — auto-tap greedily taps COLORED sources for GENERIC costs, stranding held counter mana
Casting a spell with a small colored requirement + large generic requirement, the engine auto-taps
colored (blue) sources for the generic portion when off-color/colorless sources are available —
stranding the pilot's ability to hold {U}{U} for a Counterspell. This cost the vs140 game.

Repro (`1784112898-ai_baka_deck44-...-vs-ai_baka_deck140.jsonl`):
- s61 (T36 MP1): pilot casts `Cast Archmage of Echoes {4}{u}` with `Mana available:
  {u}{u}{u}{u}{u}{u}{b}{b}{b}{b}` (6 Island + 4 Swamp, 10 untapped) and Counterspell {u}{u} in hand.
  Archmage needs 1 blue + 4 generic. Optimal tapping (1 Island for {u}, 4 Swamps for {4}) leaves
  5 Islands = {u}{u}{u}{u}{u}. Instead:
- s64 (T37, opponent's turn): Blightsteel Colossus `1 (top)` ON THE STACK. Pilot's untapped mana is
  `{u}{b}{b}{b}{b}` — 5 Islands were tapped for Archmage, leaving only ONE blue. Counterspell {u}{u}
  is uncastable and NOT OFFERED (options are only GFTT — can't hit an artifact — and Cast nothing).
  Blightsteel resolved; s67/s55 it attacked; deck44 lost by infect poison at 16 life (T39, 16/13).

The guide's "LEAVE two blue sources untapped" is UNEXECUTABLE against this: the pilot cannot choose
which lands tap. Had the auto-tapper preferred Swamps for the {4} generic, the pilot would have held
{U}{U} and countered the game-winning bomb. FIX: auto-tap generic costs from colorless/off-color
sources first, preserving colored sources (esp. when the player holds an instant of that color). This
is the single clearest game-changing mechanical item this corpus.

### #3 (KNOWN, 2048-cap; note the NEW parse consequence) — truncated reply with no CHOICE line -> parser scavenges a stray digit -> spurious mis-cast
Known corpus-wide (60 unparsed cuts at the 2048 cap; 4096 shipped next corpus). NEW observation
specific to the answer-after-plan protocol: because the answer is emitted LAST, a reply that runs out
of budget mid-reasoning has NO `CHOICE:` line, and the parser then scavenges a digit from the reply
body — which can record a valid-looking option that CONTRADICTS the reasoning.

Repro: vs140 `1784112898` s68 (T38 MP2, L16/13). Reply is a 2000+ char Blightsteel infect-rules
spiral that hit the cap with no CHOICE line; `fallback=None` but `choice=1` = `Cast Go for the Throat
- the only legal targets are YOUR OWN right now` (a self-target). The derivation EXPLICITLY obeyed
the annotation ("Go for the Throat: The only legal targets are YOUR OWN right now. So no."). Game
was already lost (Blightsteel unanswerable). So this is a HARNESS artifact, not a representation
failure — it should NOT be counted against the own-targets annotation (which is 36/36 obeyed in
reasoning). 4096-token cap should resolve; a `CHOICE:`-line-absent reply should ideally fall to the
`unparsed_reply` heuristic path (as s65 did) rather than scavenge a body digit.

### Benign fallbacks (game-neutral, logged for completeness)
- vs110 `1784103510` s13 blockers: `unparsed_reply` -> heuristic choice=0 (no blocks). The reply was
  fully reasoned and ended `BLOCKS: (No blocks declared; Oona's Gatewarden stays untapped)` — the
  parenthetical prose failed the block-declaration parse. The heuristic (no blocks) MATCHED the
  pilot's stated intent (correct non-block of a 3/5 with a 2/1 at L12). Did NOT change the game.
  MINOR HARNESS NOTE: the block parser should accept a "no blocks" prose declaration on the BLOCKS:
  line rather than dropping to heuristic.
- vs140 `1784112898` s65: `unparsed_reply` on a land-drop window, immaterial.

---

## Decision-quality confirmations (validation of prior-wave fixes)

### Wave-9 counter-category-#2 tightening — VALIDATED (was the wave-9 loss cause; now obeyed)
vs110 `1784103510` (affinity): s7 COUNTERED Cranial Plating, s12 COUNTERED Master of Etherium — both
un-GFTT-able artifact engines, countered ON SIGHT. Wave-9's inversion ("slow artifact = ignorable,
deal with it later"; and spending the counter on face burn instead) did NOT recur. The pilot's s12
PLAN quotes the guide's "ON SIGHT" verbatim — the tightening propagated. vs110 is still a loss, but
now purely to a fast affinity race + creature-light reactive draw (construction/variance), not the
counter inversion. Full 15/15 counter-window audit in findings.md.

### NEW named-target GFTT annotation — killed the wave-9 s54 fabrication class
The engine now prints `Cast Go for the Throat {1}{b} - legal targets right now: <names>` listing
every legal target (incl. opponent creatures). Repros: vs131 `1784112264` s8/s9 (names Young
Pyromancer — pilot reasons cleanly, no "is it an artifact? Wait..." loop, deliberate hold), vs109
`1784105766` s6 (names Gore-House Chainwalker — pilot cast GFTT, target sub-pick s7 correctly hit the
opponent's Chainwalker). The wave-9 fabricated-no-legal-target self-negotiation appears NOWHERE this
corpus. The complementary own-targets form (`the only legal targets are YOUR OWN right now`) fired 36×,
36/36 obeyed in reasoning.

---

## Deck-construction flags (for the user / decklist owner) — persists waves 5-10
- **16 Island / 10 Swamp + 6 reactive (2 Counterspell, 2 Arcane Denial, 2 Go for the Throat).** The
  counters EARN their slots (protected all 4 wins: Cranial Plating, Master of Etherium, Wrath of God,
  Lightmine Field, Staff of Nin, Guttersnipe, Young Pyromancer, Abominable Treefolk, Ice-Fang Coatl).
  "Trim the counters" is the WRONG lever.
- **The all-reactive/creature-light slow keep is the loss signature.** vs110 (fast affinity out-raced
  a reactive draw, T9) and the vs140 grind both show it. Sharper levers than trimming counters:
  black-source density vs the black spell load (Obyra {u}{b} / Bladecrafter {2}{b} / GFTT {1}{b}),
  and/or 1-2 more cheap flyers over the 3rd/4th reactive card to raise the aggro floor.
- **No answer to an infect ARTIFACT bomb (Blightsteel Colossus).** A UB tempo deck cannot GFTT it
  (artifact), cannot profitably block it (trample+infect), and cannot destroy it (indestructible) —
  the ONLY answer is a counter when it is cast, which the auto-tap defect (#2) denied. This is a
  matchup/construction fact, not guide-fixable; the model-relevant part is engine ledger #2.
- **The loss ROTATES by matchup/draw** (wave 7 lost vs133; wave 8 vs109; wave 9 vs109+vs110; wave 10
  vs110+vs140). A variance signature on a slow reactive half, not a guide regression.
