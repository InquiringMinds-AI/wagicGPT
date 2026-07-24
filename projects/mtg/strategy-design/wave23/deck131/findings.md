# deck131 — wave 23 review (UR Guttersnipe spellslinger; "Mind Control / Best Counter Deck Ever"). ROTATED IN.

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260724-013710/` (21 games round-robin,
binary `/tmp/wagic-0e6361732`). deck131 rotated back in (last cycled wave-13, vs 110/109/135/133/44/140).
Live guide = wave-11 FROZEN revision (9036 bytes; 12 waves frozen). Pilot live this corpus:
repetition_penalty=1.05.

**Record: 1 W / 5 L (1/6).** Seat->game map (results.tsv; deck131 = deck1 in every row):

| result | vs (deck) | end life me/opp | turn | cause |
|---|---|---|---|---|
| **WIN (adj)** | 102 (Tergrid) | 12 / 3 | 15 | clean YP-swarm race, opp ground 18->1, near-kill; best-played game |
| LOSS (adj) | 140 (RBW lifegain) | 20 / 27 | 26 | **THE OUTAGE GAME** — lifegain stall, life never moved, Guttersnipe arrived T27 |
| LOSS (death) | 137 (GW Adventures) | -2 / 27 | 12 | go-wide race + lifegain-in-race adherence slip; no Guttersnipe, single YP |
| LOSS (death) | 109 (Mono-R Hellrider) | 0 / 20 | 8 | aggro out-raced; one bad chump-block of YP into Hellrider at 12 life |
| LOSS (death) | 133 (Phyr. Asphodel) | -2 / 22 | 16 | MANA SCREW — no-red no-dig hand, **no mulligan was offered**, never cast a payoff |
| LOSS (death) | 27 (Zombies) | 0 / 20 | 14 | **Guttersnipe attack-traded via a TAPPED-STATE MISREAD**; opp life never moved |

**Bottom line:** the 12-wave-frozen guide is obeyed on every axis it already covers (Mountain-first
11/11, mulligan keeps 5/5 correct where offered, attacker declarations 4/4 clean pure-index, no
name-leak, no CoT-hijack). The 1/6 record is dominated by the standing 8-wave construction terminus
(threat density 6/60, no reach vs lifegain, no early defense vs aggro) plus variance (a screw hand
with no mulligan offered). BUT the rotated-in new-pool hunt paid off: the Zombies matchup exposed a
**game-deciding, guide-addressable NEW leak the old pool never surfaced — the model attack-traded its
only Guttersnipe into an UNTAPPED blocker it MISREAD as tapped** (vs27). The guide protected
Guttersnipe on defense (never block) but had a GAP on offense (it told the pilot to attack with
"EVERY creature: Guttersnipe, ..."). One surgical edit fills that gap. The outage (4 empty_reply at
the corpus's first-started game) did NOT decide vs140. Pilot verdict: SAME (no degradation; spirals
shorter than corpus/wave-22; empties are a corpus-start transient, NOT pilot-caused).

---

## MANDATE (1) — THE OUTAGE: did the empty_reply cluster decide vs140? NO.

All 4 of deck131's fallbacks are the s3-s6 `empty_reply` cluster, in the vs140 game (seq3,4,5,6; all
latency ~=2502ms — the uniform HTTP-level fast-fail the brief describes). They landed on turns 1-3, on
MY main-phase priority/cast asks.

**What the heuristic did during the outage:** seq2 (model) played Mountain T1; then during the outage
the baka heuristic cast **Prism Ring T1** (a lifegain artifact — a card the guide de-prioritizes) and
across seq3-6 developed lands and eventually cast **Young Pyromancer**. On T1 with a single land, YP
({1}{R}) was not yet castable, so the heuristic's Prism Ring was not a clearly worse play than the
alternatives available that turn — it cost tempo, not a threat window.

**Did it decide the game? NO — cleanly separable from the loss.** The game ran to turn 26 and was
adjudicated 20/27. **deck131's life NEVER moved the entire game (20 -> 21 at the very end from a Prism
Ring lifegain trigger).** The opponent (RBW lifegain/control) climbed to 27 purely on its own lifegain
and 131 could not deal damage: YP cast twice, both removed (Cruel Edict s5, Pyroclasm s9); Guttersnipe
did not resolve until **seq65, turn 27** — after the adjudication turn. 131 held Counterspell correctly
(used it on Elixir of Immortality, s62) and cast Aetherize as late Guttersnipe fuel. This is the
standing **no-reach-vs-lifegain construction terminus**, identical in shape to wave-13's vs140 game
(out-lifegained to a razor cap edge) — the 4-turn heuristic opening is noise against a 26-turn stall
where the deck's problem was inability to close, not early tempo.

**Pilot-correlation of the empties (OPEN item, my read):** deck131's vs140 game has the EARLIEST
start_epoch of all 21 games (1784875030, rank 1/21). Every empty at every seat clustered in the
earliest-started games (mine + deck140 s5-7, deck133). Later requests **within the same games**
succeeded with rp=1.05. That pattern is a **corpus-start endpoint transient (warmup/outage), NOT a
per-request repetition_penalty rejection** — the pilot is not implicated in the empties. (Routed to
notes.md for the synthesis.)

---

## MANDATE (2) — DECISIONS DECOMPOSITION OF THE LOSSES (excluding the outage game vs140)

### vs27 (Zombies) — L 0/20 T14 — **the corpus's headline decision leak.** GUIDE-ADDRESSABLE.
Both payoffs LANDED (YP s7 T4, Guttersnipe s13 T6) — the good half. Then:
- YP died attacking into a Zombie Outlander trade (s11 T5) — an even trade, defensible.
- **Guttersnipe died T7 attacking into Lord of the Undead.** At s15 the model declared `ATTACK: A1`
  (Guttersnipe) reasoning verbatim: *"the opponent has no untapped creatures to block (Vodalian
  Zombie and Lord of the Undead are both tapped)."* **This is a board misread.** The seq15 prompt's
  opponent battlefield line showed `Vodalian Zombie (3/3) [tapped - untaps and can attack next turn]`
  and `Lord of the Undead {1}{b}{b} (2/2)` — **Lord of the Undead had NO [tapped] tag; it was
  UNTAPPED.** Lord blocked and traded, killing 131's only reach (`Lord of the Undead -> Guttersnipe ->
  graveyard`, s16). The representation was CORRECT and clear (tag present on one, absent on the other);
  the model conflated "one is tapped" into "both are tapped."
- After both payoffs died, 131's hand was reactive/lifegain-only (Essence Scatter x2 with no legal
  target on its own turn, Aetherize, Elixir x2, Prism Ring). No way to rebuild. **Opponent's life
  stayed at 20 the ENTIRE game** (131 dealt zero damage after the payoffs died); it durdled (cast
  nothing + Prism Ring + Elixir) while Zombies beat it 20 -> 0.
- **This is the wave-13 "Guttersnipe attack-trade" WATCH graduating to a HARMFUL, game-deciding
  instance.** It is guide-addressable and it fills a GAP (see the strategy.txt change below).

Note on the guide's #1-RULE "cast Aetherize even with no attackers to bounce" claim: at s14 (Guttersnipe
alive, own MP1) the engine offered ONLY `Cast Elixir / Cast Prism Ring / Cast nothing` — Essence Scatter
(no stack target) and Aetherize (needs attackers AND was unaffordable at 3 mana) were NOT offered. There
was genuinely no Guttersnipe fuel available, so "cast nothing" at s14 was correct. The guide overpromises
target-gated reaction spells as no-target fuel; NO harm this corpus (the model never tried to cast an
unoffered spell). Latent inaccuracy -> notes.md WATCH, not a fix.

### vs137 (GW Adventures) — L -2/27 T12 — construction + adherence slip.
Kept correctly (Mountain + YP + 2 Prism Ring). YP landed s7 T3. No Guttersnipe drawn, single payoff.
Decision leak: cast **Prism Ring again (s14 T7) and Elixir (s16 T7)** — lifegain — while being actively
raced (life 17 and falling to -2). This is the guide's explicit anti-pattern ("Prism Ring / Elixir ...
never in the first turns of a race"). Under a REAL-damage race (not wave-13's latency-adjudication
artifact) the lifegain casting was genuinely bad. Attribution: **model-adherence** (the guide already
forbids it), same class as the vs27 misread and the vs109 block — the model relaxes its protect/race
discipline under pressure. Construction (no reach, single payoff) is the co-cause.

### vs109 (Mono-R Hellrider aggro) — L 0/20 T8 — construction + one bad block.
Kept correctly. YP x2 landed, Downsize/Cyclonic Rift used as removal. Aggro simply faster (131 has no
early blockers/interaction; low threat density). One decision leak: **s12 T6 chump-blocked Young
Pyromancer into Hellrider** at 12 life when the incoming was NOT lethal — the engine's own helper text
said *"NOT lethal ... taking damage while ahead is often correct"* and the option legend flagged
`B1 may block A4 (your blocker dies, attacker lives)`. Blocking a payoff into a 3/3 it can't kill, for
zero, violates the guide's "no blockers unless incoming puts you at 0." Died T8 regardless (didn't
decide it), but a clear guide-covered misplay — model-adherence, like the above.

### vs133 (Phyrexian Asphodel) — L -2/22 T16 — MANA SCREW; **no mulligan offered**.
Opening hand `Downsize; Essence Scatter; Young Pyromancer; Island; Island; Island; Downsize` = a
clean STEP-1 MULLIGAN by the guide (no Mountain, no Artificer's Epiphany, no Opportunity). **But no
mulligan ask was ever logged for this game** — the first record (s1) is already a land-drop; 131 was
never presented the mulligan decision (auto-kept / on-the-play with no mull step surfaced). It flooded
on Islands, drew no red, and **never cast a payoff (0 YP, 0 Guttersnipe)**. The model played the screw
correctly (recognized the failed red gate in its s1 reply, held counters, cast nothing rather than
waste blue). NOT a decision leak — variance + a possible harness gap (see notes.md I-1: was the
mulligan step denied?).

---

## MANDATE (3) — ROTATED-IN RE-VALIDATION: new-leak hunt + demotion-debt audit

**New-leak hunt (the rotation doctrine's point):** the new pool this cycle added 137 (GW Adventures),
102 (Tergrid), 27 (Zombies) — mechanics absent from 131's wave-13 corpus. The hunt found ONE
game-deciding guide-addressable leak the old pool never exercised: **the Guttersnipe offensive
attack-trade** (vs27, Zombies board with a mix of tapped/untapped blockers). The old pool's opponents
never presented Guttersnipe a "swing looks safe but an untapped 2-power creature is hiding on a
partly-tapped board" spot; the frozen guide told the pilot to attack with "EVERY creature: Guttersnipe"
and had no offensive protection for its only reach. **Fixed** (strategy.txt).

**Demotion-debt audit (representation fixes shipped while frozen 12 waves):** LOW / effectively none.
This guide's crutches are STRATEGY, not representation-defect crutches — it never carried the
cost/mana/hybrid/reveal/trade annotation crutches that other decks demoted:
- Mountain-first land rule -> a strategic priority (red = both win conditions), not a demotable
  representation crutch. Fired 11/11 this corpus; KEEP.
- Mulligan "read the list, do not misread as zero Mountains" -> a comprehension guard, still live-relevant
  (the vs27 tapped-misread proves the model DOES misread board state); KEEP.
- Board-wipe-counter rule (2 dmg kills the 2/2) -> strategic; KEEP.
There is nothing to demote. The one representation-shaped MOVE this corpus is the OPPOSITE of demotion:
ADD a per-creature `[tapped]`-tag reading instruction (the new leak needs it), not remove one.

---

## MANDATE (4) — PER-SEAT PILOT VERDICT (e): **SAME** (no degradation).

- **Spiral length: shorter at this seat.** Reply-length p50 1165 / **p95 6078** / max 10938 chars
  (164 replies). Corpus p95 was 7784, wave-22 p95 was 8186; corpus max 15901, wave-22 max 16882. My
  seat sits below both. No spiral lengthening from rp=1.05.
- **Prose quality: coherent, not degraded.** The longest replies (the Inquisition-of-Kozilek and
  target-choice spirals) show the model's normal answer-first CHOICE line + verbose "Wait, let me
  re-check..." self-correction reasoning (the HARNESS-N9 family), NOT token-level repetition loops or
  degenerate output. Every longest reply opens with a clean, in-range CHOICE first line. No structured-
  output degradation observed.
- **Empties: not pilot-caused** (corpus-start transient; earliest-started games only; later same-game
  rp=1.05 requests succeeded — see Mandate 1).

**Verdict line:** rep_penalty=1.05 at deck131 — **SAME**; spirals shorter than corpus and wave-22
(p95 6078 vs 7784/8186), prose coherent, no degradation, empties are a corpus-start transient not the
pilot.

---

## strategy.txt change (justified)

ONE surgical edit; everything else byte-identical to the frozen wave-11 guide. Justification: the
rotation new-leak hunt found a **game-deciding, guide-addressable GAP** — the guide protected Guttersnipe
on defense ("NEVER block with Guttersnipe ... it is your only reach") but the DECLARE-ATTACKERS rule
actively told the pilot to attack with "EVERY creature: Guttersnipe," and vs27 lost its only Guttersnipe
to an offensive trade caused by a `[tapped]`-tag misread. The edit (a) removes Guttersnipe from the
auto-attack list, (b) adds a Guttersnipe-only attack guard that instructs a PER-CREATURE `[tapped]`-tag
read ("a creature printed WITHOUT [tapped] CAN block — even if a different creature on the same line is
tapped; do not read 'one is tapped' as 'all are tapped'"), and (c) adds a matching DECIDING SITUATIONS
line. This is faithful to the guide's own thesis (Guttersnipe = only reach, protect it in every case),
extends it to the offensive side that was open, and directly targets the observed misread mechanism. It
is NOT the sanctioned-against "shout louder at a covered line" — the attack-side protection did not exist.
