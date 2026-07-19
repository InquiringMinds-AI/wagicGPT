# deck14 (Deep Blue) — notes.md — engine/harness/model items + rotation verdict

## ENGINE / HARNESS LEDGER (layer-routed here, never core prose)

### E1 (wave-18 item) — block-assignment CARD-NAME tolerance — VALIDATED / CLASS COLLAPSED. Close.
Wave-18 deck14 bled 3 combat fallbacks to card-name format drift in block replies (vs62 s34/s37,
vs135 s26, all 8-13k chars). This wave, with the wave-19 combat name-tolerance batch (parseBlock-
Assignments name->label second pass) live: **ZERO name-format block fallbacks at this seat.** Combat
replies collapsed to 300-800 chars and the two residual 4-8k spikes both parsed to clean coded lines
(vs35 s26 -> `BLOCKS: B1:A1`; vs49 s28 -> `ATTACK: A1, A2`). The engine batch + guide Rule #2
together closed the class deck14 opened. Recommend closing E1 as validated.

### E-NEW (MEDIUM; 1-repro, DECIDING) — a SHORT, well-formed coded block reply was still unparsed.
**vs49 s31 (blockers, turn 14, me=3 — the decisive defensive turn).** Reply (597 chars) ends in a
perfectly-formed `BLOCKS: B1:A2` (block the opponent's attacking 5/5 Dragon with deck14's stolen 5/5
Dragon, leave the 1/1 unblocked — a correct survival block). It was UNPARSED and handed to Baka;
deck14 died two turns later. This is NOT the wave-18 format-drift class (short, coded, sensible) and
NOT a card-name case (it used the B#/A# codes). It is a NEW shape: a clean `B#:A#` block discarded.
The seat's combat records log `options_text` EMPTY, so I cannot confirm from the log whether A2/B1
were in range. Hypotheses for the owner/engine: (a) block-index range validation rejecting a valid
pair; (b) salvageLoopedChoice (new this wave) covers CHOICE but not the blockers path, so a block
reply that trips whatever guard fired here has no salvage net; (c) the `B1:A2` mapping expected a
different token shape. Worth a targeted engine look — a correct decisive block being dropped is the
highest-cost fallback class. seq=31, file 1784474304-ai_baka_deck14-...-vs-ai_baka_deck49.jsonl.

### E2 (LOW-MED; representation) — MENU/INTENT MISMATCH stale_echos (3 this wave).
The model narrates action X in prose then emits a `CHOICE:` index pointing at a DIFFERENT listed
option:
- vs49 s26: "Boomerang the Ashenmoor Liege [spell on stack]" -> `CHOICE: 14`, but option 14 was
  "Island". (Compound: also a rules error — Boomerang cannot target a spell on the stack, only a
  permanent. A model rules-knowledge gap, not just an index slip.)
- vs135 s27: "Cast Counsel of the Soratami" -> `CHOICE: 1`, but option 1 was "Play Island". Events
  show Counsel resolved anyway, so harmless here.
- vs27 s15: "Cast nothing right now" -> `CHOICE: 3`, but option 3 was "Lord of Atlantis"; "Cast
  nothing" was not on that menu.
This is the corpus-wide list-authority family in the over-reach/mismatch direction (wave-18 E3
lineage). Present-and-disobeyed -> durable lever is representation/engine (e.g. echo back the chosen
option's TEXT in the re-ask, or a soft "your number points at <option>, is that your intent?"
nudge), not a core restatement. Guide off-ramp (strategy.txt "match your answer to the list") is the
interim. LOW-MED.

### E3 (LOW; observability) — combat records log EMPTY options_text at this seat.
Every `blockers`/`attackers` record for deck14 has `options_text == ""`. This defeated the brief's
per-seat TRADE-OUTCOME annotation validation (I could not see whether "(both die)"-class annotations
rendered on this seat's block options). If the annotations ARE being emitted engine-side, they are
not reaching this seat's translog options field — either the annotations attach only at some seats
(deck135 owns the reveal seam; maybe block annotations are similarly gated) or combat options simply
aren't serialized into the log the way `ask` options are (ask records DO carry full options_text).
Flag for the harness: serialize combat options_text (with any trade annotations) so combat-decision
review is possible. Observability item, LOW.

## MODEL-BEHAVIOR notes (guide-side, tracked)
- **The guide is provably being read** — vs49 s28 reply quotes Rule #1 verbatim before attacking.
  Strong evidence the initial-guide channel changes behavior; the attack-floor is load-bearing.
- **Durdle MUTATED, did not die**: 0-attacker flip -> slow-clock soft-durdle (attack a lone small
  body while the finisher sits in hand and opp life barely moves). Flyers deployed in only 1/6
  games (Air Elemental vs62, t15). Addressed by the strategy.txt Rule #1B revision (deploy finishers
  / race the mana / swing the whole board). Re-validate next guided round.
- **Bounce still misused defensively** on a cheap recastable creature (vs135 s6/s16 re-Unsummon the
  same 1/1 Ice-Fang Coatl). Guide already warns this; the revision sharpens it. Watch next wave.
- **Steal-their-best now firing correctly** (Dragonmaster Outcast, Yavimaya Enchantress; no 1/1
  steals) — wave-18 Control-Magic-on-a-1/1 misplay is gone. Positive; no action.

## CARD-SCRIPT DIVERGENCES (carried from wave-18, still to Oracle-verify by owner — unchanged)
- **Gravelgill Axeshark**: engine = {4}{u/b} 3/3 [islandwalk, persist]; paper is {4}{U} 4/3 Merfolk
  Soldier with a red-permanent lock, NO islandwalk/persist. Likely a mis-mapped primitive — flag for
  the owner to verify against Scryfall. Guide teaches engine reality.
- **Counsel of the Soratami**: engine {2}{u} (3 mana); paper 10E printing {3}{U} (4 mana). Minor.
- Air Elemental 4/4 flying, Mahamoti Djinn 5/6 flying, Lord of Atlantis 2/2 +1/+1/islandwalk lord,
  Coral Merfolk 2/1 — all match paper. Islandwalk near-dead in this pool (defenders rarely run
  Islands); not a bug, noted so the guide does not over-teach it.

## ENGINE ITEMS COUNT
**3 open** (E-NEW medium/deciding coded-block unparsed; E2 low-med menu/intent mismatch; E3 low
observability) + **1 closed/validated** (E1 block name-tolerance, class collapsed).

## ROTATION VERDICT
**NOT a rotation candidate — deck14 STAYS for a second guided round.** Per the per-deck rotation
test (no-guide-mod AND no-new-work-signal = rotation candidate), deck14 fails BOTH exit conditions:
there IS a warranted guide-mod (Rule #1 escalated to Rule #1B targeting the mutated slow-clock /
finisher-deployment failure) AND there IS a new-work signal (E-NEW: a correct decisive block
dropped unparsed). The taught classes are actively moving under the guide (FC2 died, FC1 mutated),
which is exactly the mid-iteration state that warrants continuation, not rotation. Re-evaluate after
wave-20's corpus: if Rule #1B lands the finishers and the slow-clock losses convert while no new
class emerges, deck14 becomes a rotation candidate then.
