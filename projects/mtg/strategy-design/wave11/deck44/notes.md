# Notes for wagicGPT development — deck44 wave 11 (self-contained; full repros)

Run: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-072254/`. deck44 = deck0 in all 6 games
(confirmed via `gamestart`). **Record 3W-3L.** 2nd corpus under the ANSWER-AFTER-PLAN reply protocol,
binary `/tmp/wagic-54a5a9d01` (wave-10 engine batch: max_tokens 4096 + parse ramble guards +
stack-target annotation fix + fetch pre-targeting keying). Corpus health for deck44: 0 desyncs, 0
engine-no-ops, 0 validation-drops, 0 raw timeouts; 3 benign `unparsed_reply` + 1 `empty_reply`
fallback (all game-neutral, detailed below); 0 truncation-scavenge self-casts. Win column is context
only (doctrine).

game->file map (by `gamestart` opp_deck + results.tsv):
- `1784119227` = vs133 WIN (20/-7 T12)      | `1784122411` = vs110 **LOSS(adj1)** (1/17 T16)
- `1784122965` = vs109 **LOSS** (-5/13 T11)  | `1784124541` = vs135 WIN (18/-7 T18)
- `1784124811` = vs140 **LOSS(adj1)** (17/25 T32) | `1784126729` = vs131 WIN (20/0 T14)

## Guide change this wave: NONE (frozen byte-identical)
Live wave-9/10 guide (md5 `bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines) re-shipped byte-identical.
No new decision-level failure class. The three losses are construction/variance (creature-light
reactive draws out-raced). The wave's model-fixable content is engine-ledger below.

---

## ENGINE-LEDGER ITEMS (layer-routed here, NOT prompt text)

### #2 (STANDING TOP ITEM, UNFIXED) — auto-tap greedily taps COLORED sources for GENERIC costs, stranding held counter mana
Carried from wave 10 (where it cost the vs140 Blightsteel game). STILL UNFIXED this corpus (ledger #2
not yet addressed). The engine auto-taps colored (blue) sources for the GENERIC portion of a cost when
off-color (black) sources are available, stranding the pilot's ability to hold {U}{U} for a
Counterspell. The mana line is `Mana available: <symbols> from N untapped sources (tapped
automatically when you cast)` — the pilot cannot choose WHICH sources tap.

RECURRED this corpus (mechanically confirmed; OUTCOME-NEUTRAL — no counter was denied):
- **vs131 `1784126729` s30 (T12 MP1):** cast `Cast Archmage of Echoes {4}{u}` from
  `{u}{u}{u}{b}{b}{b}` (3 Island + 3 Swamp). Archmage = 1 blue + 4 generic. NEXT record (s31) shows
  `Mana available: {b} from 1 untapped source` — **all 3 Islands tapped** (1 for {u}, 2 for the {4}
  generic) + 2 Swamps, leaving a single Swamp. Blue-preserving tapping (1 Island for {u}, then Swamps
  for the generic) would have left `{u}` untapped.
- **vs131 `1784126729` s21 (T8):** cast `Cast Surveilling Sprite {1}{u}` from `{u}{u}{u}{b}{b}` ->
  next (s22) `{u}{b}{b}`. Tapped 2 blue (1 for {u}, 1 for the {1} generic) when a Swamp was available
  for the generic — a blue source stranded needlessly.

Impact THIS corpus: ZERO games lost to it (unlike wave-10 vs140). At both events the following turn had
no counter window needing the stranded blue (vs131 s30: opp had 0 creatures / no stack; the pilot
untapped and won T14). All 8 counter windows this corpus had sufficient blue and were cast successfully.
So the defect is LIVE-AND-HARMLESS this corpus — a clean, loss-decoupled repro (vs131 s30) that makes a
better regression fixture than the wave-10 loss-tangled one.

FIX (unchanged): auto-tap generic costs from colorless/off-color sources FIRST, preserving colored
sources — especially when the player holds an instant of that color in hand. The guide's "LEAVE two
blue sources untapped" is UNEXECUTABLE without this. Owner: the mana/auto-tap payment code. This remains
the single clearest game-changing mechanical item across waves 10-11.

### #1 (wave-10) — RESOLVED: counter option no longer mis-renders "NO legal target" over an occupied stack
The wave-10 defect (Counterspell/Arcane Denial options printing `- NO legal target right now` even
with a spell on the stack, contradicting the same option's `can target on the stack: <name>` clause)
is **FIXED and VALIDATED**: grep of all 6 deck44 seat files for `NO legal target` = 0 hits. Counter
options now render cleanly `Cast Counterspell {u}{u} - can target on the stack: <name>`. No further action.

### #3 (NEW, LOW PRIORITY, harness/parse) — strict free-form block/attacker parser rejects NAME-based / out-of-range declarations
The block/attacker declaration is free-form (no numbered menu). Expected format (from the prompt):
`BLOCKS: B1:A2, B2:none` and `ATTACK: <slot indices>`. The parser is strict and rejects reasonable
model outputs, dropping to the heuristic. Repros (deck44 seat):
- vs110 `1784122411` s10 (blockers): reply emitted `BLOCKS: B1:A2` but A2 is out of range (only
  `A1. Memnite (1/1)` attacked; the reply hallucinated "Signal Pest is 4/1" attacking). Rejected ->
  `fallback=unparsed_reply, choice=0` (no blocks) = the guide-correct play at L19 anyway.
- vs110 `1784122411` s15 (blockers): reply emitted `BLOCKS: Scion of Oona:none` (card NAME instead of
  `B#`). Rejected -> heuristic no-block = matched the reply's stated intent.
- vs135 `1784124541` s37 (attackers): reply emitted `ATTACK: Scion of Oona, Sleep-Cursed Faerie`
  (card NAMES, not slot indices). Rejected -> `choice=-1` (heuristic). The lethal swing landed one
  window later (s41). Game won.

Impact THIS corpus: ZERO — every heuristic fallback matched the guide-correct or game-neutral action.
It is a LATENT robustness gap: a strict-parse reject on a CORRECT stated declaration could flip to a
game-costing heuristic. Distinct from the truncation-scavenge class (these replies REACHED a
declaration line; the parser rejected its FORM). FIX (optional, low priority): accept card-name block
assignments and a bare `ATTACK: <names>` by resolving names against the attacker/blocker slot list;
and reject out-of-range indices to the `unparsed_reply` heuristic (already happening) rather than
mis-mapping. Owner: the block/attacker reply parser.

### Truncation-scavenge class (wave-10 vs140 s68) — RESOLVED
The wave-10 class (a truncated reply with no CHOICE line whose parser scavenged a stray body digit and
recorded a spurious self-target GFTT) did NOT recur under the 4096 cap + ramble guards. Evidence:
own-targets GFTT fired 29 windows, 0 recorded self-casts (wave-10 was 36/1); no fallback record carries
a scavenged cast integer. The 4 fallbacks all show a proper `fallback=` field + heuristic action. Closed.

### Benign fallbacks (game-neutral, logged for completeness)
- vs110 `1784122411` s29 (blockers): `empty_reply` -> heuristic choice=0 (no blocks). The known 120s
  HTTP-timeout class (WAGIC_GPT_TIMEOUT 240s shipped next corpus, commit 95cf9f5f9). Game-neutral.
- vs110 s10 / s15 and vs135 s37: covered under #3 above.

---

## Decision-quality confirmations (validation of wave-10 fixes)

### Counter spine — STEADY (8 counter windows, 6 correct/cat-aligned + 2 minor blemishes)
CORRECT (cat #3 growers/hosers): vs135 s14 Ohran Viper, vs135 s17 Ice-Fang Coatl, vs140 s21 Lightmine
Field, vs131 s29 Young Pyromancer. DEFENSIBLE: vs140 s6 Cruel Edict #1 (would sac the pilot's only
creature), vs109 s15 Boros Reckoner (3/3 combat-punisher vs mono-red swarm at L16). BLEMISHES (minor,
low-stakes, no game cost): vs133 s12 Counterspell -> Bloodghast (on the guide's explicit let-resolve
list; recurs on landfall; won 20/-7 with counter surplus); vs140 s12 Arcane Denial -> 2nd Cruel Edict
(pilot held 2 creatures, edict would only cost the defender; hands opp 2 cards to save a 2/1). Correct
let-resolves: vs110 s11 Galvanic Blast (face burn, L19), s23 Mox Opal (rock), s9 Signal Pest (1/1),
s28 Vault Skirge (1/1, defensible cat-#4 override). No new failure class; no revision trigger.

### GFTT / target annotations — VALIDATED
- own-targets (`the only legal targets are YOUR OWN right now`): 29 windows, 29/29 obeyed in reasoning,
  0 recorded self-casts.
- named-target (`legal targets right now: <names>`): 2 windows, both hit the OPPONENT (vs135 s25 Ohran
  Viper, vs131 s14 Young Pyromancer). The wave-9 fabricated-no-legal-target self-negotiation appears nowhere.

---

## Deck-construction flags (for the user / decklist owner) — persists waves 5-11
- **16 Island / 10 Swamp + 6 reactive (2 Counterspell, 2 Arcane Denial, 2 Go for the Throat).** The
  counters EARN their slots (protected all 3 wins: Ohran Viper, Ice-Fang Coatl, Lightmine Field, Young
  Pyromancer, Cruel Edict x2). "Trim the counters" is the WRONG lever.
- **The all-reactive/creature-light slow keep is the loss signature — ALL THREE losses this wave.**
  vs109 (2 creatures vs a 5-wide mono-red swarm), vs110 (only Surveilling Sprite ever attacked, 1
  power/turn, out-raced by affinity), vs140 (slow grind, behind on life adjudication). Sharper levers
  than trimming counters: black-source density vs the black spell load (Obyra {u}{b} / Bladecrafter
  {2}{b} / GFTT {1}{b}), and/or 1-2 more cheap flyers over the 3rd/4th reactive card to raise the aggro
  floor.
- **The loss ROTATES by matchup/draw** (wave 7 vs133; 8 vs109; 9 vs109+vs110; 10 vs110+vs140; 11
  vs109+vs110+vs140). A variance signature on the slow reactive half, not a guide regression. The win
  rate dipped 4/6 -> 3/6 purely on draw variance (the same spine, obeyed the same way, won 3 and lost 3
  slow keeps).
