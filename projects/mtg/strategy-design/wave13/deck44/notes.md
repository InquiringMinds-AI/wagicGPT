# Notes for wagicGPT development — deck44 wave 13b (self-contained; full repros)

Run: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-183228/`. deck44 = deck0 in all 6 games.
Binary `/tmp/wagic-a75e2625e` (Force r2/r3 + auto-tap two-pass d5d7b1f2e + name-echo c3e6d95b2 +
BLOCKS:none 42e8bc180 + 240s timeout). **Record 3W-3L** (W vs135/vs131/vs133, L vs109/vs110/vs140 —
SAME split as wave 11). Corpus health for deck44: 0 empty replies, 0 fallback records, 0 choice=-1,
0 defers, 160 decisions. Win column is context only (doctrine).

game->file map (`gamestart` opp_deck + results.tsv):
- `1784162895`=vs109 L (-1/9 T9)   | `1784165345`=vs135 W-adj (20/8 T14)
- `1784166093`=vs110 L-adj (7/18 T14) | `1784167378`=vs131 W (19/-2 T14)
- `1784169213`=vs140 L-adj (12/16 T32) | `1784171613`=vs133 W-adj (9/4 T15)

## Guide change this wave: NONE (frozen byte-identical)
Live guide md5 `bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines, re-shipped byte-identical. The two NEW
failures (F1 self-destroy, F2 sub-menu confusion) are reply-protocol/representation, NOT guide prose.

---

## ENGINE / REPLY-PROTOCOL LEDGER (layer-routed here, NOT prompt text)

### #1 (NEW, HIGH, TOP MODEL-FIXABLE ITEM THIS WAVE) — reconcile arm: an ABSENT `(name)` echo must NOT fall back to the raw CHOICE index
The name-echo protocol resolves an echo-vs-index conflict by "index wins." When the echo matches NO
current option, that is a STALE-INDEX signal, and index-wins fires an UNWANTED cast:
- **vs135 `1784165345` s24 (T8) — SELF-DESTROY:** GFTT target sub-menu `[1] Sleep-Cursed Faerie [your
  battlefield] / [2] Faerie Bladecrafter [your battlefield] / [3] Boreal Druid [opponent's battlefield]`.
  Reply re-litigates ("I will NOT cast GFTT... pass the turn") and emits `CHOICE: 1 (Cast Go for the
  Throat {1}{b})`. Echo names the parent action (absent from targets) → index 1 → own Sleep-Cursed Faerie.
  s25 events: `Your Go for the Throat: hand->stack | Your Sleep-Cursed Faerie: battlefield->graveyard`.
  Pilot destroyed its own 3/3 flyer.
- **vs133 `1784171613` s14 (T8) — index-drift after same-turn cast:** at s13 cast Surveilling Sprite
  (option 2); at s14 the list SHIFTED to `[1] GFTT / [2] Cast nothing`. Reply intent = HOLD ("I will not
  cast GFTT"), but emits `CHOICE: 1 (Cast Surveilling Sprite)` — echo names the already-cast Sprite
  (absent) → index 1 (now GFTT) wins → GFTT cast against intent → s15 forced target menu → wasted on
  Bloodghast (guide anti-pattern AND the model's own refusal).
- CLEAN control: **vs131 s20** OWN-ONLY GFTT, model emits `CHOICE: 2 (Cast nothing)`, echo matches → correct.
FIX: when the CHOICE echo matches no current option, route to the Cast-nothing/pass option (or re-prompt),
NOT the raw index. Also re-anchor indices after any same-turn cast so a stale index cannot silently
re-map to a different spell. Owner: the reply-protocol reconcile arm (parseChoice / name-echo path).
Directly extends wave-11 skill headline 1 (absent-named-action routing) to the name-echo layer.

### #2 (NEW, MEDIUM, representation) — forced-choice sub-menus are bare/unframed → read as "glitches"
Targeted-spell target menus and forced-sacrifice menus present a bare creature list with no header for
the committing spell/effect and no cancel:
- vs135 s24: "I will pass the turn" (re-litigates a committed GFTT).
- vs133 s15: "The prompt is likely a glitch... There is no active effect requiring... I should not cast."
- vs133 s19 (T11): opponent Liliana of the Veil forced sacrifice; reply "the prompt is likely a glitch...
  if I must pick, the least valuable — Scion of Oona." Sacked the ANTHEM LORD (team +1/+1 & shroud) when
  Surveilling Sprite (draws on death) was the better fodder.
FIX: add a sub-menu header naming the committing spell/effect + "not optional"; annotate `[your
battlefield]` vs `[opponent's battlefield]` and list opponent targets first; consider a safe default.
Owner: the option-rendering / sub-menu prompt builder. Overlaps #1 at vs135 s24 & vs133 s15 (distinct
layer: #1 = wrong option selected, #2 = why the model was confused and trapped).

### #3 (STANDING TOP ENGINE ITEM, ledger #2, STILL UNFIXED) — auto-tap greedily taps COLORED sources for GENERIC costs, stranding held counter mana
The d5d7b1f2e two-pass did NOT change live behavior — greedy-blue-for-generic persists (two-pass landed
in `ManaEngine::planPayment`; live tap path `selectAutoTapProducers` is unpatched). deck44 = 16 Island +
10 Swamp, ALL basics, NO duals (decklist verified) → pip accounting is clean; the stranding is real.
- **Cleanest loss-decoupled repro — vs140 `1784169213` s39 (T26):** `Cast Archmage of Echoes {4}{u}`
  (1 blue + 4 generic) from `{u}{u}{u}{u}{b}{b}{b}{b}` (4 Isl + 4 Swamp) → next mana `{b}{b}{b}` (dU=4,
  dB=1). Tapped ALL 4 Islands for a 1-blue cost; blue-preserving (1 Isl {u} + 4 Swamp {4}) leaves
  `{u}{u}{u}`. Zero blue retained vs three. Best regression fixture in the corpus.
- Other same-turn confirmations: vs140 s19 Scion {2}{u} `u4b1→u1b1` (dU=3), vs140 s25 Sprite {1}{u}
  `u4b4→u2b4` (1 blue stranded), vs133 s13 Sprite {1}{u} `u3b1→u1b1`.
Impact THIS corpus: **ZERO** — all 9 counters cast successfully; every decline had adequate blue (vs135
s14 declined at `{u}{u}` in hand — judgment, not denial); late losses had 8-13 sources (moot). LIVE-AND-
HARMLESS, same as wave 11. FIX (unchanged): pay generic from colorless/off-color sources FIRST, preserving
colored sources, especially when the player holds an instant of that color. The guide's "LEAVE two blue
sources untapped" is UNEXECUTABLE without this. Owner: the mana/auto-tap payment code (selectAutoTapProducers).

### #4 (LOW, harness/parse) — mixed index+name declarations: partial parse takes A-indices, silently drops name-form entries
The brief's `ATTACK: <name>` shape. The attacker parser accepts leading `A#` indices and drops trailing
card-NAME entries. Repros (deck44): vs140 s6 `A1, Obyra`; s14 `A1, A2, Faerie Bladecrafter`; vs131 s16
`A1, Faerie Bladecrafter`; s29 `A1,A2,A3, Faerie Bladecrafter ×2`; vs133 s8, s22. **In EVERY case the
dropped name was NOT in that turn's attacker-slot list (summoning-sick), so the parser attacked with all
ELIGIBLE indexed attackers — 0 cost.** The model habitually over-names ineligible creatures; the slot
list protects it. Latent risk: a name-form reference to an ELIGIBLE attacker would be silently dropped.
FIX (optional, low priority): a name→index reconcile arm for attackers that resolves names against the
slot list AND respects eligibility. Owner: the attacker reply parser.

---

## Decision-quality confirmations (verification of shipped fixes)

- **Name-echo: 152/152 CHOICE lines carry `(name)` (100%).** No wrong echo REMAP (index/chosen 160/160
  consistent). BUT the absent-echo → index-wins default is ledger #1's mechanism.
- **BLOCKS:none (42e8bc180): CLEAN.** 3 bare-none firings (vs109 s10, vs110 s7, vs133 s20) → choice=0,
  all reasonings intended no-block. 0 misparses. The wave-11 bare-none→unparsed bug is closed here.
- **Force r2/r3: N/A** (deck44 has no Force of Negation).
- **Counter spine: 9 cast (all correct/defensible) + 7 declines (correct let-resolves).** Best: vs110 s9
  Cranial Plating, vs140 s17 Staff of Nin (artifact engines, cat #2). Two mild: vs133 s9 Fatal Push
  counter (both counters gone before Liliana T12); vs135 declined FIRST Ice-Fang (s14) then countered the
  SECOND (s26) — backwards, low stakes. Sweeper cat #1 UNTESTED (no Wrath/Pyroclasm hit the stack).
- **Attack-every-turn spine: obeyed** (all eligible flyers swung each combat).
- **Block-adherence blemish (single, no cost):** vs131 s17 Faerie Bladecrafter blocked a 1/1 at L19 —
  soft violation of "9+ life = only Gatewarden blocks." Won 19/-2.

## Deck-construction flags (for the user / decklist owner) — persists waves 5-13
16 Island / 10 Swamp + 6 reactive (2 Counterspell, 2 Arcane Denial, 2 GFTT). Counters EARN their slots
(protected the wins: Cranial Plating, Staff of Nin, Lightmine, Young Pyromancer, Ohran Viper, Ice-Fang,
Geralf's, Cruel Edict). "Trim the counters" is the WRONG lever. The all-reactive/creature-light slow keep
is the loss signature — SAME 3 losses as wave 11 (vs109 out-raced by mono-red swarm; vs110 lost the
affinity clock race; vs140 behind on life adjudication). Sharper levers: black-source density vs the black
spell load ({u}{b}/{2}{b}/{1}{b}), and/or 1-2 more cheap flyers over the 3rd/4th reactive card to raise
the aggro floor. Variance signature on the slow reactive half, not a guide regression.

## One-line handoff
Top deck, guide FROZEN 10th wave, 3W-3L (same matchup split as wave 11; all 3 losses = creature-light
reactive draws out-raced, construction/variance). Shipped fixes verify: name-echo 100% compliant,
BLOCKS:none clean (0 misparses), 240s timeout (0 empties). BUT a NEW HIGH class emerged in the name-echo
era: the reconcile arm's index-wins-over-ABSENT-echo default fired an unwanted removal that SELF-DESTROYED
the pilot's own Sleep-Cursed Faerie (vs135 s24) and wasted GFTT on Bloodghast against the model's own
refusal (vs133 s14/s15) — route to reply-protocol ledger #1 (extends wave-11 headline 1 to the name-echo
layer), NOT guide. Forced sub-menus read as "glitches" (ledger #2, representation). The standing auto-tap
color-stranding (ledger #3, ledger #2 in prior numbering) STILL UNFIXED — two-pass did not reach the live
path — re-verified, outcome-neutral; cleanest fixture vs140 s39 (Archmage {4}{u} tapped 4 blue from
{u4}{b4}). All engine-layer; guide byte-identical FROZEN.
