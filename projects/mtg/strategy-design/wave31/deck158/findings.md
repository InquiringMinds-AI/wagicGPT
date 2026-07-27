# deck158 (Orcs of Mordor) — wave-31 STEP-0 GUIDELESS DEBUT, seat findings

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-025026`, binary cf7cb2e53, 6 games,
no guide (core prompt only). All seq/turn citations are the translog record's own `seq`/`turn`
fields; game-log turn numbers (which run one ahead) are named as "game Turn N" when quoted.

## 0. Corpus shape at this seat

| opponent | result | game end | decisions |
|---|---|---|---|
| 148 | **LOSS** | my_life -4 @ t10 | 21 |
| 139 | WIN | opp 0 @ t11 | 24 |
| 137 | WIN | opp -10 @ t13 | 26 |
| 146 | WIN | opp -1 @ t19 | 36 |
| 152 | WIN | opp -6 @ t19 | 46 |
| 122 | WIN | opp -16 @ t13 | 24 |

**5/6, and all five wins are CLEAN KILLS — zero cap-adjudications.** Per the wave-30 HL6
adjudication-alignment caveat this record is NOT metric-inflated: the deck simply killed people.
Games are short (21-46 decisions vs the pool's usual 60-90); this is the fastest seat in the pool.

Decision mix (165 records ex gamestart/gameend): 117 ask, 30 attackers, 11 blockers, 6 priority,
1 defer. **Fallbacks 1/165 (0.6%)**, the single defer. Reply length p50 1,173 / p95 9,866 /
max 13,319. Latency p50 ~19s, max 208s. Zero crash signatures (N-139h corpus gate: PASS at
this seat).

**EXECUTABILITY AUDIT.** 6 priority windows in 6 games and **only ONE of them on the opponent's
turn** (vs152 seq14, t8). Instant-speed/reactive play is structurally unavailable at this seat.
Any guide rule of the form "hold X for the opponent's turn" would be dead text. Confirmed
directly at vs152 seq27 (below): Orcish Bowmasters in hand, own upkeep, 4 untapped sources
including {b} — and the priority option list did NOT contain a Bowmasters cast at all.

---

## (a) AMASS SURFACE READ — the reason this seat was rotated in

### Card facts, verified against `bin/Res/sets/primitives/borderline.txt` + `_macros.txt`

`_AMASSORC1_` / `_AMASSORC2_` (macros, `_macros.txt:68-69`):
`if type(army|mybattlefield)~morethan~0 then ... notatarget(army|myBattlefield)
transforms((Orc,newability[counter(1/1[.2])])) forever else name(Create Orc Army)
token(Orc Army^Creature Orc Army^0/0^black) and!( counter(1/1[.2]) )!`
— i.e. **amass is NOT a target choice**: it is `notatarget`, always the player's own Army, and
it auto-creates a 0/0 black Orc Army token when none exists. There is no "which Army" decision
and the corpus confirms it: **zero amass-target asks in 165 decisions.**

Playset amass sources (all verified from primitives):

| card | cost | amass | verified line |
|---|---|---|---|
| Foray of Orcs x4 | `{3}{R}` sorcery | 2, then deals damage = **the amassed Army's power** to target opponent creature | `auto=if type(army...)~morethan~0 then ... counter(1/1.2) ... damage:power target(creature|opponentbattlefield)` |
| Swarming of Moria x3 | `{2}{R}` sorcery | 2 + a Treasure | `auto=_TREASURE_` / `auto=_AMASSORC2_` |
| Mordor Muster x2 | `{1}{B}` sorcery | 1, draw 1, **lose 1 life** | `auto=draw:1 controller` / `auto=life:-1 controller` / `_AMASSORC1_` |
| March from the Black Gate x2 | `{1}{B}` enchantment | 1 on ETB **and 1 every time an Army you control ATTACKS** | `auto=_AMASSORC1_` / `auto=@combat(attacking) source(army|mybattlefield): _AMASSORC1_` |
| Dunland Crebain x2 | `{2}{B}` 1/1 flier | 2 on ETB | `auto=_AMASSORC2_` |
| Easterling Vanguard x2 | `{1}{B}` 2/1 | 1 **when it DIES** | `auto=_DIES_ _AMASSORC1_` |
| Gothmog x1 | `{3}{B}` 3/3 | 1 on ETB; **creature tokens you control have deathtouch** | `auto=_AMASSORC1_` / `auto=lord(creature[token]|myBattlefield) deathtouch` |
| Orcish Bowmasters x4 | `{1}{B}` 1/1 flash | 1 + 1 damage any target, on ETB and on each opponent draw after their first per draw step | see (b) |
| Assault on Osgiliath x2 | `{X}{R}{R}{R}` sorcery | **X** | see (c) |
| Grishnakh x1 | `{2}{R}` 1/1 | 2 + steal a creature with power <= Army power | never drawn |

Multiplier: **Mauhur, Uruk-hai Captain** `{B}{R}` 2/2 menace —
`auto=@totalcounteradded(1/1) from(*[army;orc;goblin]|mybattlefield) plus(1)` = every +1/+1
counter EVENT on the Army gets one extra counter.

### How amass RENDERS — verdict: the STATE surfaces are excellent, the OPTION surfaces are not

**WORKING (do not touch).**
- Battlefield line: `Orc army (9/9) (printed 0/0) [deathtouch] [counters: 9x +1/+1]`
  (vs137 seq20). Current P/T, printed P/T, conferred keywords and the counter count are all
  present. The pilot read the computed P/T off this line every single time and never re-derived
  it from base stats + counters — the TRUST-THE-RENDERED-NUMBER class is a non-issue here.
- Attacker line: `A1. Orc army (9/9) [deathtouch]` (vs137 seq21); blocker line carries the trade
  annotation `B1. Orc army (3/3) - may block A1 (you kill it, your blocker lives)` (vs152 seq20).
- Token creation narrates: `Your Orc army (0/0): created -> battlefield` (vs152 seq18).
- The March attack-trigger fires and narrates in the Blockers step every time the Army attacks
  (vs146 seq24, vs152 seq26, vs122 seq22 — 100% of Army attacks with March out).

**GAP 1 — amass magnitude is absent from every cast option line.** `1. Cast Mordor Muster
{1}{b}`, `Cast March from the Black Gate {1}{b}`, `Cast Dunland Crebain {2}{b} (1/1)` carry no
amass amount and no resulting Army size. The pilot has to know the card text and do the
arithmetic; it did so, but that arithmetic is where the reasoning tax lives (vs137 seq18: 13,319
chars / 208s spent on a **land-drop** decision, the whole body being "does Foray make the Army
9/9 before I attack"). Candidate annotation: `{right now: Army 7/7 -> 9/9}`. Routed to notes.md
as N-158a.

**GAP 2 — `- Counter added to Orc army` carries neither a count nor the Mauhur bonus.** One line
per amass EVENT, no magnitude. Arithmetically confirmed at vs122 game Turn 12: four
`Counter added to Orc army` lines took the Army 7/7 -> 15/15 (+8) because Mauhur was out —
so each narration line was worth 2, while at vs137 game Turn 12 (no Mauhur) Foray's two lines
were worth 1 each. The narration is unusable for prediction; only the battlefield `(N/N)` line
is. Notes N-158b. (No misplay traced to it — the pilot correctly used the battlefield line —
so this is a latent item, not an active one.)

**GAP 3 — Foray of Orcs' `{right now: damage 0}` is a static 0 in every instance and is never
the real number.** Both casts:
- vs152 seq17: rendered `Cast Foray of Orcs {3}{r} {right now: damage 0}`, no Army on
  battlefield. Resolved: Army token created, +2 counters, `Damage: 2 dealt by Orc army to
  Briarbridge Tracker` (seq19 narration). Real magnitude **2**, rendered **0**.
- vs137 seq19: rendered `{right now: damage 0}` with a 7/7 Army out; had a legal target existed
  the real magnitude would have been 9.
The pilot survived it BOTH times by reasoning past the annotation ("the damage portion... the
primary effect (Amass 2) happens regardless" — vs137 seq19 reply; "will be 2 after amassing" —
vs152 seq18 reply) but paid 13.1k and 9.8k chars doing it. This is the Gray-Merchant
`{drains N}` offset shape and it is worse (the offset is not constant — it is the whole value).
Notes N-158c.

**GAP 4 — Foray's damage target menu is labelled with the wrong source.** vs152 seq18:
`TARGET CHOICE for Orc army (... answer with the chosen TARGET's name (not "Orc army"))`. The
ability was transformed onto the Army token, so the menu names the Army rather than Foray of
Orcs. The pilot decoded it (it picked correctly), but a menu that names your own creature as the
spell asking for a target is an invitation to self-target. Notes N-158d.

**No render gap found on:** amass target selection (there is none by design), Army
identity/type, counter display, deathtouch conferral, or the Snarling Warg
`(4/4) (printed 3/4)` orc-conditional pump (vs122 seq22 — correct and legible).

---

## (b) ORCISH BOWMASTERS x4 — triggers correct, but the marquee mode never fired

Primitive (`borderline.txt:80463`): `abilities=flash`, `{1}{B}` 1/1;
`auto=ability$!name(Amass orcs 1) _AMASSORC1_!$ controller` + `auto=name(Damage any target)
damage:1 target(anytarget)` (the ETB pair) and the same pair gated on
`@drawfoeof(player) restriction{compare(odrewcount)~morethan~1}`.

- **Cast 5 times across 6 games (22 hand-appearances). All 5 in the pilot's own Main phase 1
  at sorcery speed.** The ETB damage target ask rendered correctly every time
  (`1. The opponent (player, life 20)` / creature options / `[your battlefield]` options), and
  the pilot picked the opponent's FACE 5/5 — correct in all five, since no offered creature had
  toughness 1. This is the CORE prompt's ping-arithmetic rule working ("N damage targets a
  creature only when N >= its toughness, else face"). **Do not re-teach it in the guide.**
- **Amass fired on every ETB** (`Counter added to Orc army` immediately after the damage ask).
- **The extra-draw trigger fired exactly ONCE in 6 games** (vs137 seq6, t4, on the opponent's
  turn — target ask rendered, pilot took the face, opp 20->19). The engine's restriction
  `compare(odrewcount)~morethan~1` means the trigger only fires on a card drawn beyond the first
  each draw step; this pool almost never does that. **Bowmasters' realized value at this seat is
  a 2-mana 1/1 that pings for 1 and grows the Army — not a draw-punisher.** Report the null:
  the punish mode is UNEXERCISED (per the wave-30 seeded-but-unexercised rung, the missing
  trigger condition is "an opponent draws a second card in one draw step").
- **Did the pilot hold it for flash value? No — and it was RIGHT not to, structurally.** With one
  opponent-turn priority window in six games there is nowhere to flash it. Worse, when a window
  did coincide with Bowmasters in hand the cast was **not offered**: vs152 seq27, own Upkeep,
  hand `Orcish Bowmasters {1}{b} (1/1)`, `Mana available: {r}{b} from 4 untapped sources`, and
  the entire legal-action list was `1. Draw 1 with Clue`. The pilot explicitly noticed
  ("the options provided only show Draw 1 with Clue... I must draw"). Flash creature casts are
  missing from the priority-window legal-action list. Notes N-158e.

---

## (c) "SAGA" — Assault on Osgiliath is NOT a Saga in this engine

The brief asked for a Saga chapter read; there is no Saga in deck158. Assault on Osgiliath
(`borderline.txt:5886`) is a **Sorcery** `{X}{R}{R}{R}` with a second castable face:
- `restriction=type(army|mybattlefield)~equalto~0` — the BASE face is legal only with **no**
  Army (it creates one: `ifnot paid(alternative) then name(Create Orc Army) token(...) and!(
  counter(1/1.fullpaid) )!`).
- `otherrestriction=type(army|mybattlefield)~morethan~0`, `other={X}{R}{R}{R}` — the
  **alternative** face is legal only when you DO have an Army (it adds `fullpaid` counters).
- Plus, on both faces, `auto=emblem transforms((,newability[lord(*[orc;goblin]|mybattlefield)
  double strike],newability[lord(*[orc;goblin]|mybattlefield) haste])) ueot`.

Renders and decisions:
- Both faces render legibly and distinctly: `Cast Assault on Osgiliath {r}{r}{r}{x}` (base) vs
  `Cast Assault on Osgiliath with its alternative cost {r}{r}{r}{x}` (alternative). Only the
  legal one is ever offered — checked at vs146 seq20/seq22 (alternative, Army out) and seq24
  (base, Army just died). Correct.
- The **X menu is exemplary**: `You can afford X up to 3 with your current mana - higher values
  are NOT offered... option 1 is the LARGEST X (X = 3). Reply with the OPTION number, not the X
  value: 1. X = 3 / 2. X = 2 / 3. X = 1 / 4. X = 0`. The pilot picked max X **2/2** (vs152
  seq24, vs146 seq28). The h2-family affordability annotation and the largest-first ordering are
  both doing their job here.
- **Counters land correctly**: X=3 -> three `Counter added to Orc army` lines, Army 3/3 -> 6/6
  (vs152 game Turn 12) and 0/0 -> 3/3 (vs146 game Turn 16).
- **The double-strike / haste emblem appears NOT to reach the Army — two independent witnesses**
  (routed to notes as N-158f, engine-lane, NOT written into the guide):
  1. vs146 seq28/29, game Turn 16: Assault (base face, X=3) resolves in Main phase 1, creating
     a fresh 0/0 -> 3/3 Army. The narration then goes straight to `=== Turn 17 - opponent's
     turn ===`: **no Attackers phase, no attackers window** — consistent with the Army being
     summoning-sick, i.e. no haste. The pilot had cast it explicitly for the haste swing
     ("Attack immediately with the Army to deal 6 damage") and got nothing. At 5 life vs opp 11
     this cost six damage and probably the tempo of the game.
  2. vs152 game Turn 12: Assault (alternative, X=3) resolves, the 7/7 Army attacks and is
     blocked by a 3-toughness Elite Spellbinder. Log: a single `Damage: 7 dealt by Orc army to
     Elite Spellbinder` **and** `Damage: 3 dealt by Elite Spellbinder to Orc army`. With double
     strike the 7-power first-strike damage would have killed the blocker before it dealt any.

---

## (e) THE DEFER — vs146 seq25, turn 13: a NEW seam (X-COST ZERO-SLACK), not divergence-C

Full record: `{"choice": -1, "chosen_text": "Assault on Osgiliath", "fallback":
"deferred_to_heuristic", "kind": "defer", "latency_ms": -1, "options": 0, "phase": "Main phase
2", "prompt": "", "reply": "", "seq": 25, "turn": 13}` — empty prompt, empty reply: **the model
was never asked**. The defer is entirely engine-side.

Shape, reconstructed from the adjacent records:
- seq24 (Main phase 2, same turn), board: `Mana available: {r} from 3 untapped sources`,
  battlefield has `Swamp [tapped]; Mountain #1; Mountain #2; Mountain #3; ...; Mountain #4
  [tapped]` and **creatures: 0** (the Army traded with Barrowin of Clan Undurr that combat).
  Options: `1. Cast Assault on Osgiliath {r}{r}{r}{x}` / `2. Cast nothing right now`.
  The pilot chose 1.
- The base face was correctly legal (no Army). Its colored requirement `{R}{R}{R}` consumes
  **all three** untapped Mountains, so the only affordable X is **0**.
- seq25: defer. **Arrival-traced: the cast never happened.** seq26's narration jumps from
  `Your Orc army: graveyard -> zone` straight to `=== Turn 15 - opponent's turn ===` with no
  `Assault on Osgiliath: hand -> stack`, and seq26's hand still lists
  `Assault on Osgiliath {r}{r}{r}{x}`. Card returned to hand, silent abort.
- Two turns later, with **6** untapped sources (X-slack of 3), the identical cast was offered,
  the ANNOUNCE_X menu DID fire ("You can afford X up to 3"), and the cast resolved (seq27/28).

**Classification: NEW seam. Not divergence-C** — deck158 has no convoke and no
alternative-cost-with-creature-tapping anywhere in the list; the seq24 cast was the plain base
face at its printed cost. What the two cases have in common is only the FAILURE MODE
(`FindCardToPlay`'s `aiForcedCandidate` validation rejecting a cast `offerable()` approved ->
silent defer -> silent no-op). Here the discriminator is **X-affordability slack = 0**:
the same card, same face, same seat deferred at slack 0 and resolved at slack 3.

Sub-note on severity: at X=0 the base face resolves to a 0/0 Army token with
`counter(1/1.fullpaid)` = 0 counters, which dies to state-based actions immediately — so the
deferred cast was a null play and the defer cost nothing THIS time. That makes it a low-harm
but clean repro, which is exactly what the ledger wants. Two plausible fix directions for the
ledger: (i) apply the h2 `maxAnnounceableX` credit path to plain X-costs so an X=0-only cast
either announces X=0 or is filtered from the cast menu by the same logic that filters
601.2c-illegal casts; (ii) make the validation reject LOUD (re-ask) rather than deferring.

---

## (d) 5/6 DECOMPOSITION — what to PRESERVE, what to CORRECT

### PRESERVE (guide must do no harm to these — all are guideless-baseline behaviors)

**P1. The attack floor is already perfect: 30/30 attacker windows declared EVERY eligible
creature; zero `ATTACK: none`; zero mixed-form under-declarations; zero unparsed.** Scored
against the engine's `A1..An` eligibility list per the wave-15 rule, not against `chosen_text`.
Includes the 5-attacker alpha at vs152 seq44 and the correct lethal read at vs139 seq22
("3 + 4 = 7... reduces the opponent's life to 0"). This is the strongest attacker record in the
campaign and it is UNGUIDED. The guide's attack section must be short and confirmatory; a loud
new attack floor here is pure overshoot risk (the deck135 pursuit-overshoot shape).

**P2. Amass-then-attack sequencing was discovered unaided at least once and it won the game.**
vs137 seq19, game Turn 12: 7/7 Army, opponent board empty. The pilot explicitly compared
"attack for 7 now, Foray next turn" against "Foray to 9/9 in Main 1, then attack for 9",
chose the second, and the game log confirms `Foray: hand -> stack` -> two counters -> `You
declared attackers: Orc army` -> 9 damage. This is the deck's central sequencing fact and it
must be written down, because it CONTRADICTS the core prompt's default ("cast non-instant cards
in your SECOND main phase by default").

**P3. Attack-as-engine.** The pilot attacked with the Army into blocks it survived and let the
March triggers snowball it: vs122 Army 1/1 (t5) -> 3/3 -> 5/5 -> 11/11 -> **15/15** (t13), a
15-damage swing for the kill. It found the March-plus-Mauhur loop without being told.

**P4. Removal target selection by impact.** Feed the Swarm picks were the correct threat every
time: Serra Ascendant 6/6 lifelinker (vs122 seq13), Nadaar 5/5 (vs146 seq21), Stone Haven
Pilgrim 4/2 (vs148 seq12). Sauron's ETB steal was used aggressively and correctly twice —
steal the biggest body and swing with it (vs139 seq16/17 Migratory Greathorn; vs146 seq17/18
Nadaar, which also removed the blocker from the defence).

**P5. Blocking discipline.** 11 windows, all defensible; the "unblocked total is NOT lethal"
annotation was quoted and obeyed; favorable trades taken (vs152 seq20 Army 3/3 eats a 2/2 Wolf
and lives; vs122 seq16 Easterling Vanguard trades with Kitchen Finks). One malformed line
(vs139 seq18 opened `BLOCKS: B1:A1, B1:A2`, self-corrected in prose; first-wins parser took the
correct single block) — not a fallback, no action.

**P6. Mulligan is a non-issue.** All 6 openers were 3-landers; all 6 kept. Core's land-count
default is doing the job. The guide gets no mulligan section beyond one line.

### CORRECT (the guide's actual job)

**C1 — #1 LEAK, and the cause of the ONLY loss: the pilot reads `Mana available: {r}{b} from N
untapped sources` as a TWO-mana pool.** It treats the colour SET as the pool SIZE.
Offered-and-declined-as-unaffordable instances:
- **vs148 seq19, t9, my_life 1 (GAME-LOSING).** Options: `1. Cast Snarling Warg {3}{b} (3/4)` /
  `2. Cast nothing`. `Mana available: {r}{b} from 5 untapped sources`. Reply: *"I have {R}{B}
  available, but Snarling Warg costs {3}{B} (four mana total). I cannot afford to cast it."*
  Chose Cast nothing. Snarling Warg would have entered as a **4/4** (`aslongas(*[orc;goblin]|
  myBattlefield) 1/0`, and the pilot controlled an Orc Army) — a blocker against the exact
  1+3 = 4 damage that killed it on the opponent's next turn. It died at -4.
- **vs148 seq17, t9**, same board, same offered Warg, same misread, chose Cast nothing.
- **vs146 seq31, t17, my_life 5 vs opp 7.** `from 6 untapped sources`; `Cast Gothmog {3}{b}` and
  `Cast Snarling Warg {3}{b}` both offered; reply "cannot pay {3}{B}"; took the 2-mana March
  from the Black Gate instead.
- **vs152 seq11, t7.** `{r}{b} from 2 untapped sources`, `Cast Feed the Swarm {1}{b}` offered;
  reply *"I only have one {B} and one {R} (which cannot pay for the generic {1})"* — a second,
  distinct false belief (off-colour mana cannot pay generic). Chose Cast nothing.
This class is ALSO the dominant reasoning tax: **8 of the top 10 tax spikes are mana arithmetic**,
including three on LAND-DROP decisions (vs137 seq18 13.3k/208s; vs148 seq2 12.9k/164s; vs152
seq22 12.2k/150s). Core already carries the corrective ("Never decline a listed action because
you believe... you cannot pay for it"; "any color can pay a generic cost"; "declining an offered
action because you wrongly believe you cannot pay for it" is in its common-errors list) and it
is STILL LOSING locally — which is exactly the Inputs-#2 caveat that licenses a deck-guide
reinforcement tied to named cards. **Layer split: the guide gets the interim corrective FACT
anchored to the deck's four-mana cards; the durable fix is the render's WORDING** (colour-set
first reads as pool size) -> notes N-158g.

**C2 — Easterling Vanguard's amass is a DEATH trigger, read as an ETB trigger.** vs137 seq20:
"Casting it immediately triggers its... ability", and the follow-on plan asserted the Army would
be "10/10". The engine says `auto=_DIES_ _AMASSORC1_`. The pilot caught itself one record later
(vs137 seq21: "likely miscounting the Army as 10/10"). Cost: tax plus a wrong attack plan.
The guide gets the fact **plus its positive consequence**: the Vanguard is the deck's free
trade/chump blocker, because dying is how it pays.

**C3 — `[summoning sick - cannot attack this turn]` read as "cannot block".** vs146 seq23,
t13, my_life 5: the pilot attacked its 2/2 Army into a board whose only creature was
`Barrowin of Clan Undurr {2}{b}{w} (3/3) [summoning sick - cannot attack this turn]`, reasoning
*"The opponent has no untapped creatures to block (their lands and Barrowin are tapped or
summoning sick)"*. Barrowin blocked; the March trigger made the Army 3/3; both died. The Army
— the win condition — was traded away for a 3/3 at 5 life. Note the tag itself is correctly
worded (restriction-first, wave-29 HL2 compliant) and the SAME pilot got it right two games
earlier (vs152 seq19: "creatures with summoning sickness cannot attack, but they CAN block").
So this is a STRATEGY/knowledge leak, not an annotation-wording leak — guide layer, one flat
fact.

**C4 — Feed the Swarm's life cost is invisible and the rendered number is wrong.**
`auto=transforms((,newability[destroy],newability[life:-manacost opponent])) oneshot` — you lose
life equal to the destroyed permanent's mana value. Every one of the ~44 option renders in the
corpus reads `{right now: life -2}`, a **constant**, regardless of target. Actual resolutions:
Nadaar `{2}{W}` -> **-3** (vs146: my_life 8 -> 5), Stone Haven Pilgrim `{1}{W}` -> -2 (vs148:
7 -> 5), Serra Ascendant `{W}` -> -1 (vs122), Kor ally token -> **0** (vs148 seq13/14: 5 -> 5).
This deck races at low life (it finished at 3, 5, 7, 13, 15 life in its five wins) so a 1-life
error is real. Durable fix -> notes N-158h; the guide carries an interim read-the-target's-cost
line, tagged with a removal condition.

**C5 — 12/55 casting windows (21.8%) chose "Cast nothing".** Most are legitimate (Feed the
Swarm with no target; genuinely unaffordable). But four of them are C1 (above) and two more
sit at Main phase 2 with mana up (vs148 seq19, vs137 seq23). Not a separate leak — it is C1's
downstream symptom, and fixing C1 is the lever. Do NOT write a second cast-floor rule: core's
act-floor is already explicit and the deck's real problem is a payability misread, not passivity.

### UNEXERCISED (report the null; do not manufacture a read)

- **Swarming of Moria (3-of), Ugluk of the White Hand (1-of), Grishnakh, Brash Instigator (1-of)
  never appeared in a single prompt across 6 games.** Grishnakh in particular carries the only
  amass-gated STEAL in the deck (`target(creature[-legendary&power<=p]|opponentbattlefield)`),
  a genuinely different decision SHAPE — but per the wave-30 validation-transfer rung its
  cousin surface (Sauron's ETB steal-and-swing) exercised twice and correctly, so this is
  draw variance, not a held gap.
- **Witch-king's attack trigger** (defender sacrifices its lowest-power creature): cast once
  (vs139 seq21), the game ended before it attacked. UNTESTED.
- **Sauron's `{1}{B}{R}` pump/drain activation**: offered once (vs139 seq19, own upkeep),
  passed — correctly, it was upkeep. Its Main-phase-2 window never arose. UNTESTED.
- **Bowmasters' extra-draw punish trigger**: 1 firing in 6 games (see (b)). Effectively
  unexercised; missing trigger condition is an opponent drawing a second card in one draw step.

### CONSTRUCTION note (not a guide problem)

Nothing to route. The deck is 60 cards, 26 lands (12 Mountain / 14 Swamp), the curve tops at 6,
and it produced five clean kills. The one loss was a decision loss (C1), not a power loss. This
seat is NOT construction-ceilinged.

## Guide-draft summary

Plan, in my own words: **you have exactly one win condition and it is a token — the Orc Army.
Every amass spell feeds the same body, March from the Black Gate turns each of its attacks into
another counter, Mauhur adds one to every counter event, and Gothmog makes it a deathtouch
attacker nothing profitably blocks. You cast amass spells in MAIN PHASE 1 and then swing, so the
counters land before damage.** The single biggest piloting problem the guide is built to fix:
**the pilot believes it has two mana when it has five, and declines the four-mana threats that
are sitting in its own option list — that misread, and nothing else, lost the only game.**
