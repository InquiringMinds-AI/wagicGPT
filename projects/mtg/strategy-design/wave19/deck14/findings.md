# deck14 (Deep Blue) — findings.md — wave-19 FIRST GUIDED corpus

Seat: deck14, mono-blue tempo. Corpus `matchups-20260719-092058`, binary 72b05535d.
Record **2/6**: WIN-adj vs62, WIN vs27; LOSS vs35, LOSS vs49, LOSS vs135, LOSS vs110.
This is the **validation of wave-18's initial guide** (Step 0-ter: did each taught class die,
persist, or mutate?). Win-rate is context — the round-robin is harder and win-rate fell 3/6→2/6,
but the unit of analysis is the taught-class disposition, not the record.

Fallbacks this seat: **4** (matches brief) = 1 unparsed (vs49 s31) + 3 stale_echo (vs49 s26,
vs135 s27, vs27 s15). Down from wave-18's 5, and the SHAPE changed completely (see FC-B).

## 0. Taught-class disposition (the core of this review)

### TAUGHT CLASS A — FC1 durdle-into-defense / 0-attacker flip -> COUNTERMAND FIRED, class MUTATED
The wave-18 loss signature was ZERO `attackers` decisions all game (vs35, flip to full reactive
control). This wave that literal signature is **DEAD**: every one of the 6 games declares attackers,
INCLUDING the vs35 rematch (attacked t5, t7). The guide is provably being **read and obeyed** —
at **vs49 s28 (attackers, t11)** the model's reply verbatim: `The guide says: "A turn with a live
untapped creature and no attack is a misplay." So I must attack. ... ATTACK: A1, A2`. Rule #1's
attack-floor is load-bearing and firing.

BUT the durdle mutated into a weaker, subtler form — a **SLOW-CLOCK soft-durdle**. Rule #1 is
satisfied on its literal terms (an attacker is declared) while its INTENT (be a real clock) is
evaded:
- **The flyer finishers — the guide's stated primary win condition — were deployed in only 1 of 6
  games** (Air Elemental, vs62, and not until t15). In the two slow-clock losses (vs110, vs135)
  deck14 deployed ZERO flyers. At **vs135 s27** the model states it holds a Mahamoti Djinn in hand
  but "lack[s] the mana" — it durdled its development and never raced to 6 mana for the finisher.
- **Opponent life barely moves.** vs110 LOSS: opp 20->14 over 5 attacking turns while deck14's own
  life fell 20->-2 (out-raced by a thin 1-creature clock — it attacked with a lone Lord of Atlantis
  each turn). vs135 LOSS: opp 20->13 across 14 turns; deck14 attacked with a single Coral Merfolk
  exactly twice (t5, t11). These are **losses to an inadequate clock**, not to passivity.
- **Re-bouncing a cheap recastable creature** (the wave-18 anti-pattern) recurs: vs135 s6 and s16
  both Unsummon the same 1/1 Ice-Fang Coatl — exactly the "do not re-bounce a 1-2 mana creature
  every turn on defense" the guide warns against.

**vs35 LOSS is NOT a durdle relapse** — reclassify it as matchup + a known grind failure. deck14
attacked t5/t7 (countermand held); by t9/t11 it faced a 6-Sliver board with anthem + Trample +
Flying + First Strike and correctly reasoned that attacking was suicide (s21/s25 replies are sound:
racing a go-wide anthem swarm with 2-3 small bodies is genuinely dead). The upstream failure was
the same "bounce one recastable sliver at a time while the swarm rebuilds" grind. Mono-blue tempo
vs go-wide Slivers is a bad matchup; the "no attackers" at t9/t11 was a reasoned hold, not a flip.

**vs49 LOSS is a near-win, not a durdle.** Attacked every turn t5->t15, used steals well (Control
Magic -> the opponent's Dragonmaster Outcast, then swung with the 5/5 Dragon it produced), ground
the opponent 20->7, and died at -2 on turn 16 holding at 3 life for 7 turns. Clean, close play.

**Net:** the guide's #1 rule works but must now escalate one order — from "attack every turn"
(achieved) to "attack with your WHOLE board and DEPLOY YOUR FLYERS; a single-creature poke that
leaves the opponent near 20 is a soft durdle; race to 6 mana and land the finisher." This is the
warranted guide revision (see strategy.txt).

### TAUGHT CLASS B — FC2 combat format-drift reasoning-tax -> DIED
Wave-18's dominant fallback source (3 of 5 fallbacks) was 8.6k-13.1k-char combat replies that
drifted out of the coded format (card names / prose / never emitting a clean line). This wave:
- **Combat replies collapsed to 300-800 chars** across the board (see the length table in notes).
- **Two residual tax spikes** — vs35 s26 (blockers, 8,307 chars) and vs49 s28 (attackers, 4,492
  chars) — but **BOTH now PARSE**: each ends in a clean coded line (`BLOCKS: B1:A1` /
  `ATTACK: A1, A2`) and was accepted. The tax spike no longer costs a fallback. Rule #2's
  format-discipline line + the engine's combat name-tolerance batch together closed the class.

### TAUGHT CLASS C — FC3 invented-option / obey-the-list -> PERSISTS (representation wall)
All 3 stale_echos are a single coherent class: **menu/intent MISMATCH** — the model narrates an
action and appends a `CHOICE:` index that maps to a DIFFERENT option in the actually-rendered list:
- **vs49 s26** — narrates "Boomerang the Ashenmoor Liege" and answers `CHOICE: 14 (Ashenmoor
  Liege)`, but option 14 was "Island". (Also a rules error: the Liege was a spell on the stack;
  Boomerang cannot target a spell — bounce only hits permanents.)
- **vs135 s27** — narrates "Cast Counsel of the Soratami" and answers `CHOICE: 1`, but option 1
  was "Play Island". Events show Counsel resolved anyway, so this was harmless drift, not a lost
  decision.
- **vs27 s15** — narrates "Cast nothing right now" and answers `CHOICE: 3`, but option 3 was "Lord
  of Atlantis"; "Cast nothing" was not on that menu at all.
This is the wave-18 E3 face (present-and-disobeyed list authority) in the over-reach direction. The
guide off-ramp already covers it; the durable lever is representation/engine, not more core prose.

## 1. New engine residual (the one unparsed this wave)
**vs49 s31 (blockers, t14, me=3, the decisive defensive turn):** a SHORT (597-char) reply that
ends in a perfectly-formed coded line — `BLOCKS: B1:A2` (block the opponent's 5/5 Dragon with the
stolen 5/5 Dragon; leave the 1/1 unblocked) — and was **still unparsed**. This is NOT the wave-18
format-drift class (short, coded, sensible) and NOT a card-name case (used codes). It is a new
shape: a well-formed `B#:A#` block discarded. Combat records log empty `options_text` at this seat,
so I cannot see the block-menu indices to confirm whether A2/B1 were in range; flagging for an
engine look (block-index validation / salvageLoopedChoice appears not to cover the blockers path).
deck14 died 2 turns later — this fallback handed the decisive block to Baka. seq for the ledger.

## 2. Play-quality (positive validations of guide content)
- **Steal-their-best FIRED**: Persuasion (vs35), Control Magic -> Dragonmaster Outcast (vs49) and ->
  Yavimaya Enchantress (vs62, then attacked with it two turns). No 1/1 steals this wave (wave-18's
  Control-Magic-on-a-1/1 misplay is gone). The "aim the steal at a bomb" rule holds.
- **Mulligan**: kept all six hands; no keep looks punished by the game trajectories (vs135's
  mana-lightness is the only soft flag, and it kept a plausible hand). No mulligan issue to raise.
- **The wins are the finisher/big-threat games**: vs27 (WIN) rode Lord + Gravelgill Axeshark and a
  growing multi-attacker board to opp -5; vs62 (WIN-adj) rode a stolen Yavimaya Enchantress + a
  late Air Elemental. The pattern confirms the diagnosis: deck14 wins when it lands a real threat,
  loses when it pokes with a lone small body — sharpening the guide toward finisher-deployment.

## 3. Brief validation-focus items I could NOT verify from this seat
- **TRADE OUTCOME annotations ("(both die)" etc.)**: NOT OBSERVABLE at deck14 — every `blockers`/
  `attackers` record logs `options_text` as empty, so I cannot confirm whether the annotations
  rendered on this seat's block options, whether block quality moved, or whether any annotation
  misled. Routed to notes as an observability gap; no misled-annotation instance found (none seen).

## 4. Rotation verdict
**NOT a rotation candidate — stays for a second guided round.** Clear guide-mod (rule #1 escalation
to target the mutated slow-clock/finisher-deployment class) AND new-work signal (the vs49 s31
coded-block engine residual). The taught classes are moving under the guide (FC2 died, FC1 mutated
and needs the next-order rule), which is exactly the state that warrants continued iteration.
