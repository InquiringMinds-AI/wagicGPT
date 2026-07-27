# deck158 (Orcs of Mordor) — wave-32 seat findings (2nd corpus, FIRST GUIDED)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-115847`, binary `7fabd9bd0`, 6 games,
initial guide deployed (`bin/Res/ai/baka/deck158_strategy.txt`, drafted from the wave-31 5/6
Step-0 debut). All seq/turn citations are the translog record's own `seq`/`turn`. Card facts
verified against `bin/Res/sets/primitives/borderline.txt` + `_macros.txt` via `/usr/bin/grep`.

## 0. Corpus shape at this seat

| opponent | result | game end | turn | decisions |
|---|---|---|---|---|
| 146 | **WIN — clean kill** | opp -9 | 9 | 14 |
| 116 | **WIN — clean kill** | opp -3 | 8 | 17 |
| 36 | **WIN — clean kill** | opp -5 | 10 | 22 |
| 105 | LOSS | **my life 15, opp 3 — POISON alt-win** | 13 | 22 |
| 139 | LOSS | my -7, opp 8 | 12 | 20 |
| 152 | LOSS | my -2, opp 11 | 16 | 37 |

**3/6, and all three wins are CLEAN KILLS — zero cap-adjudications at this seat** (pool
adjudication rate 1/21, `results.tsv`). Per HL7 the pool-invariant statistic is the KILL COUNT:
**5 kills (wave-31) -> 3 kills (wave-32)**, against a pool that replaced 122/148/137 with three
debuts including a 6/6 infect perfecta.

**The kill CLOCK collapsed, and that is the headline the win column hides.** Winning turn,
wave-31: 11, 13, 19, 19, 13 (median 13). Wave-32: **9, 8, 10 (median 9)**. Decisions per game
fell 21-46 -> 14-37 (median 21 -> 19.5). This deck now kills four turns earlier than it did
with no guide. The three losses are decomposed in §5 and NONE of them is "the guide taught the
wrong line and it cost the game."

Decision mix (120 records ex gamestart/gameend): 72 ask + 8 mulligan/untap-ask, 21 attackers,
14 blockers, 3 priority, 1 bottom, 1 reveal. **Fallbacks 2/120 (1.7% at this seat; 2 of the
corpus's 3)** — 1 retracted_choice, 1 unparsed_reply (§6). Reply length p50 1,060 / p95 5,287 /
max 12,180. Latency p50 16.7s / p90 48.0s / max 206s. **Crash gate: 0 signatures in all 6
stderr files** (`game-158v*`, `game-*v158*`).

---

## 1. (i12) N-158g COUNT-FIRST MANA LINE — VERDICT: **VALIDATED, CLASS DEAD. Rule #1 DEMOTES.**

### The render shipped on every path

Every `Mana available:` line at this seat is count-first. Verbatim forms observed:

```
Mana available: 0 total (no untapped sources)
Mana available: 1 total (one untapped source, tapped automatically when you cast; colours you can make: {b})
Mana available: 5 total (five untapped sources, tapped automatically when you cast; colours you can make: {r}{b})
Mana available: 2 total (two untapped sources, tapped automatically when you cast; colours you can make: {b}) | Already in pool: {r}{b}
```
(vs146 s1/s3, vsk36 s16, vs105 s12.) **Zero occurrences of the wave-31 form
`Mana available: {r}{b} from N untapped sources` anywhere in 6 games** — old-string grep clean,
so no sibling emitter survived (HL5 sweep).

### The false-decline class: audited exhaustively, ZERO instances

Method: every `ask`/`priority` record whose option list contained at least one `N. Cast <card>`
line, cross-tabbed against `chosen_text`.

**33 casting windows with at least one real cast offered. 32 casts taken. 1 non-answer** — and
that one is the seq16 `retracted_choice` fallback (§6a), not a decline. **Genuine
"Cast nothing right now" against an offered cast: 0/33.**

Wave-31 baseline for the same measure: 12/55 declines, of which **4 were the C1 false-affordability
class** (vs148 s17 and s19 — the game-losing pair, vs146 s31, vs152 s11), all reasoning verbatim
"I have {R}{B} available, but Snarling Warg costs {3}{B} (four mana total). I cannot afford to
cast it."

Every reply containing affordability vocabulary (`cannot afford|cannot pay|only have|not enough
mana|insufficient`) was read. **No instance is a false decline of an offered payable cast.** The
survivors are all TRUE statements about cards NOT in the option list, and the model now quotes the
new render to reason correctly:

- **vs152 s31, t15** — *"the prompt says 'Mana available: 3 total (three untapped sources ...
  colours you can make: {b})' ... So I have 3 Swamps untapped. I have 0 Mountains untapped. I
  cannot pay the {R} cost for Sauron's ability {1}{B}{R} right now."* Correct on both counts, and
  it reasoned FROM the new line by name.
- **vs139 s16, t11** — *"I lack the mana (need 3-4 mana, have only 2 sources)"* for Foray/Swarming.
  True (2 untapped sources; Foray is `{3}{R}` = 4).
- **vs146 s4, t3** — a mid-sentence self-correction on Mauhur `{B}{R}` at 2 sources: *"which I
  cannot pay yet ... and I have the colors, wait... Mauhur is {r}{b}, I have a Mountain (R) and a
  Swamp"* — and it then cast March (Mauhur was not on the list; the engine agrees).

**Positive control that the new line is load-bearing, not decorative:** vsk36 s16, a LAND DROP,
the model writes *"I have 5 untapped lands and need to maximize my mana ... With 5 sources, I can
easily cast a 2-mana sorcery"* — the exact arithmetic wave-31 could not do at the same seam.

### Tax spikes: collapsed, but not to zero, and the RESIDUE IS A DIFFERENT SHAPE

Wave-31's three largest replies were **13,319 chars / 208s, 12,875 / 164s, 12,198 / 150s — ALL
THREE ON LAND DROPS, all rehearsing the mana-pool belief** (S-158b). Wave-32:

| chars | latency | seat | seq | turn | kind | choice |
|---|---|---|---|---|---|---|
| 12,180 | 206.0s | vs139 | 16 | 11 | ask | *(retracted — §6a)* |
| 10,763 | 161.8s | vsk36 | 16 | 10 | ask | Play Swamp |
| 6,519 | 96.6s | vs152 | 17 | 9 | attackers | Orcish Bowmasters |
| 5,646 | 101.8s | vsk36 | 17 | 10 | ask | Cast Feed the Swarm |
| 5,587 | 86.6s | vs116 | 13 | 8 | ask | Play Swamp |

p95 reply length **9,866 -> 5,287 (-46%)**; median latency 19s -> 16.7s. The land-drop rows
survive but their CONTENT changed completely: vsk36 s16 (10,763 chars) contains no mana-pool
doubt at all — it is a full lethal computation plus a Feed-the-Swarm value comparison,
i.e. **the model doing its whole-turn planning at the land-drop seam and then again at the cast
seam.** Two contributing surfaces, both visible in that record:
1. the land-drop ask carries the FULL board + hand + plan but only 3 trivial options, so the
   deliberation has nowhere to go but the rest of the turn;
2. the stale-plan notice fired (*"the actions your plan names are no longer among the options
   available right now"*), which correctly invalidates the plan and implicitly invites a full
   re-derivation at the cheapest possible seam.

Routed as **L-158-1** (notes.md), a length-only watch item; it is not a decision defect (the
choice was correct in every land-drop spike) and it is NOT the wave-31 belief.

### RECOMMENDATION

**Rule #1 DEMOTES.** Three independent reasons, all HL10 / stopgap-retirement grounds:
1. **The class is dead** — 0/33, versus 4/55 fatal instances at the same seat one wave ago.
2. **The rule quotes a render string that no longer exists.** Its second line reads *"The board
   line reads like 'Mana available: {r}{b} from 5 untapped sources'"* — the pilot will never see
   that string again, so the rule now describes a surface that is gone. Guide prose that
   contradicts the live render loses (perception-vs-strategy), and here it does not even contradict
   — it points at nothing.
3. It occupies the guide's #1 slot with ~14 lines and a 5-card enumeration whose whole job the
   render now does inline.

**It does not vanish entirely.** One NEW, distinct arithmetic error appeared in its place and
justifies a single retained line (see §6a): at vs139 s16 the model read **`{2}{R}` as two mana**
(*"Grishnakh cost: {2}{R}. I need 2 generic and 1 Red. I have 2 sources. Can I pay {2}{R}? Yes, 2
sources = 2 mana"*). That is the INVERSE of wave-31's error — it now trusts the count line and
mis-parses the COST notation. The engine was right to withhold the cast; the replacement teach is
a cost-notation fact, not a mana-pool fact, and it goes in the casting block as a falsity-fact
line following its positive rule (negative-framing ruling, exception 1).

---

## 2. (i14) MAGNITUDE CLUSTER — VERDICT: **PARTIAL. Three sub-items PASS outright; the amass preview is PATH-SCOPED and reaches 3 of 52 offers at this seat.**

### 2a. Foray of Orcs damage post-amass — **PASS, exact, 3/3**

Primitive (`borderline.txt`, `name=Foray of Orcs`, `mana={3}{R}`, `type=Sorcery`):
`auto=if type(army|mybattlefield)~morethan~0 then ... transforms((Orc,newability[counter(1/1.2)],
newability[name(Damage creature) damage:power target(creature|opponentbattlefield)])) forever` and
a sibling `~equalto~0` branch that creates the token first. Damage = the Army's power AFTER the
two counters.

All three renders this corpus, verbatim, **both branches covered**:
- `3. Cast Foray of Orcs {3}{r} {right now: Army 8/8 -> 10/10, damage 10}` (vs146 s11)
- `1. Cast Foray of Orcs {3}{r} {right now: Army 7/7 -> 9/9, damage 9}` (vs116 s14)
- `2. Cast Foray of Orcs {3}{r} {right now: Army 0/0 -> 2/2 (a new 0/0 Army token is created
  first), damage 2}` (vs152 s25) — the no-Army branch, with the token-creation clause spelled out.

**Zero `damage 0` renders in the corpus.** Wave-31 had `{right now: damage 0}` on 2/2 casts, a
constant that was never the real number (N-158c). Ground-truth check on the hardest case: vs152
s25 predicted `0/0 -> 2/2, damage 2`; the narration resolves
`Your Orc army (0/0): created -> battlefield` / `+1/+1 (now 1/1)` / `+1/+1 (now 2/2)` /
`Damage: 2 dealt by Orc army to Brutal Cathar`. **Exact match.**

### 2b. Feed the Swarm per-target life costs — **PASS on the cast line, ABSENT on the target menu**

Primitive: `auto=transforms((,newability[destroy],newability[life:-manacost opponent])) oneshot`,
`mana={1}{B}`, `target=*[creature;enchantment]|opponentBattlefield`. Life lost = the destroyed
permanent's mana value.

All 7 Feed the Swarm cast offers carry per-target costs. Verbatim:
- `2. Cast Feed the Swarm {1}{b} - legal targets right now: Ornithopter (costs you 0 life),
  Salvage Slasher (costs you 2 life), Cathodion (costs you 3 life)` (vsk36 s9)
- `... Ornithopter (costs you 0 life), Cathodion (costs you 3 life), Steel Wall (costs you 1
  life), ...` (vsk36 s13)
- `1. Cast Feed the Swarm {1}{b} - legal targets right now: Ichorclaw Myr (costs you 2 life),
  Cystbearer (costs you 3 life)` (vs105 s11)

Numbers verified: Ornithopter `{0}` -> 0, Steel Wall `{1}` -> 1, Cathodion `{3}` -> 3. Wave-31's
constant `{right now: life -2}` on ~44 renders is gone.

**It changed a decision, and won a game.** vsk36 s17/s18, t10, at 12 life with exact lethal on
board: the model wrote *"If I destroy Cathodion, I lose 3 life (12 -> 9). Ornithopter is {0} ... If
I destroy Ornithopter, I lose 0 life ... Destroying Ornithopter is strictly better"*, took
Ornithopter, and killed on the swing (opp -5, t10). Both mana values were read straight off the
render.

**The gap (HL5 sibling path):** the `(costs you N life)` annotation appears on the CAST option line
only. The subsequent `- TARGET CHOICE for Feed the Swarm ...` menu — where the target is actually
committed — carries the target's rules text but **no cost**. Verbatim, vsk36 s18:
`Ornithopter (0/2) [flying] [opponent's battlefield] - "Flying"`. The two-step choreography means
the number is shown at step 1 and the decision is made at step 2. Routed **N-158k**.

### 2c. Counter narration with count + source — **PASS on count, PATH-SCOPED on source**

Wave-31's `- Counter added to Orc army` (bare, no magnitude — N-158b) is gone. **Every** counter
line now carries the resulting P/T. Three source-attribution variants coexist:

| form | distinct prompts | when |
|---|---|---|
| `+1/+1 (now 1/1) [from March from the Black Gate]` | 90 | amass CREATES the token, source is a permanent |
| `+1/+1 (now 2/2)` *(no bracket)* | 63 | amass onto an EXISTING Army (macro `transforms` branch) |
| `+1/+1 (now 1/1) [from ]` **empty** | 38 | amass CREATES the token, source is a SORCERY already in the graveyard |

Traced against surrounding narration: named sources are `March from the Black Gate`,
`Dunland Crebain`, `Easterling Vanguard` (permanents); the empty bracket always follows
`- Your Mordor Muster: stack -> graveyard` / `- Your Foray of Orcs: stack -> graveyard` /
`- Damage: 1 dealt by Orcish Bowmasters to the opponent` (vs139 s18, vs152 s35). **An empty
`[from ]` is a rendered defect on i14's own new surface.** No reply verbalized confusion about it
and no misplay traced to it — latent, low severity, clean repro. Routed **N-158l**.

Mauhur's doubling is now legible through the running total (`(now 4/4)` -> `(now 6/6)` on one
event), which retires the wave-31 unusability complaint.

### 2d. Amass previews on the fixed-count cards — **PARTIAL: 3/52 coverage, and the mechanism is a macro-expansion path-scope**

Brief i14 names "the four fixed-count cards (Foray x2 paths, Grishnakh, Widespread Brutality,
Surrounded by Orcs)". Only Foray and Grishnakh are in deck158, and Grishnakh was never offered.
Measured coverage of `{right now: Army X/X -> Y/Y}` over every amass-card cast offer:

| card | offers | with preview |
|---|---|---|
| Foray of Orcs | 3 | **3** |
| Easterling Vanguard | 17 | 0 |
| Mordor Muster | 13 | 0 |
| March from the Black Gate | 9 | 0 |
| Orcish Bowmasters | 7 | 0 |
| Dunland Crebain | 2 | 0 |
| Swarming of Moria | 1 | 0 |
| **total** | **52** | **3 (5.8%)** |

**Mechanism, located in code (not hypothesised):** `dynamicMagnitudes` ->
`amassCounters(card)` -> `amassCountersFromScript(card->magicText)` scans for the literal
substrings `counter(1/1.` and `counter(1/1)` (`src/AIPlayerGPT.cpp:246-259`). Foray of Orcs writes
its amass LITERALLY in its `auto=` lines, so it matches. Every other amass card in the deck uses
the **`_AMASSORCn_` macro** (`_macros.txt:68-72`), and macro expansion happens on a LOCAL copy
inside the ability factory (`src/MTGAbility.cpp:6650`,
`magicText = AutoLineMacro::Process(magicText);`) — it is **never written back into
`CardPrimitive::magicText`**. So the scanner sees the literal token `_AMASSORC1_`, finds no
`counter(`, and returns 0.

This is exactly the wave-31 HL5 shape applied to the i14 fix itself: the fix landed on the
literal-script path and is blank on the macro path, and the macro path is where 49 of 52 of this
seat's amass offers live. It is also cheap to close (run `AutoLineMacro::Process` on the text
before scanning, or map the five macro names to their counts). Routed **N-158m, HIGH** — this is
the single highest-value open item at this seat because the amass preview is the annotation that
was designed for this deck.

Correctly SUPPRESSED, verified: **Assault on Osgiliath** (`counter(1/1)` through `counter(1/1.8)`
across nine `alternative if compare(fullpaid)` branches -> `values.size() > 1` -> return 0) — the
multi-branch guard works. Never offered this corpus, so this is a code read, not a live witness.

---

## 3. GUIDE FIRST-OUTING VALIDATION

### 3a. Card-fact audit of the DEPLOYED guide (HL1 hard gate) — **PASS on every number**

Every mana cost, P/T, activation cost and trigger condition in the deployed guide, checked line by
line against `borderline.txt`:

| guide claim | primitive | verdict |
|---|---|---|
| March from the Black Gate `{1}{b}` enchantment, amass 1 on ETB **and** on every Army attack | `mana={1}{B}`, `type=Enchantment`, `auto=_AMASSORC1_`, `auto=@combat(attacking) source(army\|mybattlefield):... _AMASSORC1_` | TRUE |
| Mauhur `{r}{b}` 2/2 menace, every +1/+1 counter event on Army/Orc adds one extra | `mana={B}{R}`, 2/2, `abilities=menace`, `@totalcounteradded(1/1) from(*[army;orc;goblin]\|mybattlefield) plus(1)` | TRUE |
| Gothmog `{3}{b}` 3/3, amass 1, **tokens** gain deathtouch | `mana={3}{B}`, 3/3, `_AMASSORC1_`, `lord(creature[token]\|myBattlefield) deathtouch` | TRUE |
| Foray `{3}{r}`, amass 2 then damage = Army power after amass | as §2a | TRUE |
| Swarming of Moria `{2}{r}`, Treasure + amass 2 | `mana={2}{R}`, `_TREASURE_`, `_AMASSORC2_` | TRUE |
| Mordor Muster `{1}{b}`, draw 1, lose 1, amass 1 | `draw:1 controller`, `life:-1 controller`, `_AMASSORC1_` | TRUE |
| Bowmasters `{1}{b}` 1/1 flash, 1 damage any target + amass 1 on entry | `abilities=flash`, 1/1, `damage:1 target(anytarget)`, `_AMASSORC1_` | TRUE |
| Easterling Vanguard `{1}{b}` 2/1, amass on **death** | `auto=_DIES_ _AMASSORC1_`, 2/1 | TRUE |
| Dunland Crebain `{2}{b}` 1/1 flying, amass 2 on entry | `abilities=flying`, 1/1, `_AMASSORC2_` | TRUE |
| Snarling Warg `{3}{b}` 3/4 menace, 4/4 while you control an Orc | 3/4, `menace`, `aslongas(*[orc;goblin]\|myBattlefield) 1/0` | TRUE |
| Olog-hai Crusher `{3}{r}` 4/4 trample, can block only with a Goblin/Orc | 4/4, `trample`, `this(variable{type:*[goblin;orc]:mybattlefield}=0) cantblock` | TRUE |
| Sauron `{3}{r}{b}` 4/4, ETB steal untapped+hasty; `{1}{b}{r}` team +2/+0 **and drains 2** | `mana={3}{B}{R}`, 4/4, `moveto(mybattlefield) and!(... untap],haste)) ueot)!`, `{1}{B}{R}: ... life:-2 opponent ... all(creature\|myBattlefield) 2/0 ueot` | TRUE |
| Witch-king `{4}{b}{b}` 5/3 flying, attack -> defender sacrifices lowest-power creature | `mana={4}{B}{B}`, 5/3, `flying`, `_ATTACKING_ ... notaTarget(creature[power=power:lowest:...]) sacrifice` | TRUE |
| Feed the Swarm `{1}{b}`, creature OR enchantment, lose life = destroyed permanent's mana value | as §2b | TRUE |
| Ugluk `{2}{b}{r}` 3/3, grows when another of your creatures dies | `mana={2}{B}{R}`, 3/3, `counter(1/1)` / `counter(1/1,2)` for Orc/Goblin | TRUE (see nit) |
| Amass has no target choice; a 0/0 Orc Army is created if you control none | `notatarget(army\|myBattlefield)`, `else name(Create Orc Army) token(Orc Army^...^0/0^black)` | TRUE |

**Nit (fix in revision, not a misteach):** the guide writes Ugluk as *"grows +1/+1 (or +2/+2 for
an Orc/Goblin)"*. The primitive puts **two +1/+1 counters**, which is +2/+2 in effect but a
different object for anything that counts counters (Mauhur triggers on the counter EVENT). Reword
to "one +1/+1 counter, or TWO for an Orc/Goblin."

**Two DECKLIST OMISSIONS the audit surfaces (HL1's coverage half):** the deck runs
**2x Assault on Osgiliath** and **1x Grishnakh, Brash Instigator** and the guide names NEITHER.
Assault is a two-faced X-spell whose legality flips on whether you control an Army
(`restriction=type(army)~equalto~0` / `otherrestriction=...~morethan~0`) — wave-31 recorded the pilot
handling its X menu well (max X 2/2) but also recorded **N-158f**: the double-strike/haste emblem
appears not to reach the Army. Grishnakh appeared in hand this corpus (vs139 s16) and drew a
wrong-arithmetic detour. Both get a line in the revision.

### 3b. Teach-by-teach FIRING record

| # | teach | fired? | evidence |
|---|---|---|---|
| T1 | Attack floor: "keep declaring EVERY creature the attackers list offers" | **YES, 21/21** | Every attackers window declared the full `A1..An` set; zero `ATTACK: none`; zero mixed-form drops. Includes the stolen `Sigarda, Champion of Light (5/5) [trample, flying, haste]` at vs152 s32. Cumulative guided+unguided: **51/51**. |
| T2 | Amass in MAIN PHASE 1, then attack (overrides core's main-2 default) | **outcome YES, mechanism UNEXERCISED** | 33/33 casts in Main phase 1, all pre-combat. But the corpus produced **zero Main-phase-2 decisions of any kind** (phase histogram: 72 ask + 2 priority in Main 1, 0 in Main 2), so the pilot was never offered the choice the teach exists to settle. Per HL4 rule 2 the verdict is UNEXERCISED, not validated. |
| T3 | Cast floor / "the game already checked your lands" | **YES, 33/33** | §1. |
| T4 | Easterling Vanguard is a trade/chump, never held back | **YES** | Attacked at vs139 s17, vs152 s22/s27/s32; died in combat and paid off — `Counter added to Orc army: +1/+1 (now 1/1) [from Easterling Vanguard]` (vs152, t16). |
| T5 | Feed the Swarm: spend on what is actually killing you, not the cheapest | **YES, 2/2 real windows** | vs105 s12 took Cystbearer (2/3 infect) over Ichorclaw Myr (1/1) at a 3-vs-2 life cost. vsk36 s18 took Ornithopter (0 life) to open exact lethal. |
| T6 | Sauron: "take their BIGGEST body and attack with it" | **YES on the pick, and T1 rescued the reasoning** | vs152 s31 stole `Sigarda (5/5) [trample, flying]` over Moonrage Brute and a Wolf. But the reply asserted *"she is tapped, I cannot attack with her this turn"* — false (the primitive grants `untap` + `haste`). The attack floor declared her anyway; 5 damage landed. **Evidence FOR keeping the floor loud**, and the "untapped and hasty" clause needs sharpening. |
| T7 | Bowmasters: cast in main phase, never hold for flash | **YES, non-harmful** | 3 priority windows in 6 games, exactly ONE on the opponent's turn (vs152 s34), whose only option was `Draw 1 with Clue`. Bowmasters was never offerable at instant speed — **N-158e steady state confirmed, not counted against the guide.** |
| T8 | Bowmasters ping: "face UNLESS a creature with toughness 1 is on the board" | **NO — MISSED, in the lost game** | vs105 s7, t4. Option list included `Ichorclaw Myr (1/1) [infect] [opponent's battlefield]`. Reply: *"the opponent has no other creatures with low toughness that are immediately threatening ... Killing their Ichorclaw Myr is unnecessary right now as it is summoning sick and cannot attack anyway."* Took the face for 1. The exception was met and unread. Wording defect, §4. |
| T9 | Mulligan: "keep any 7 with 2-5 lands; a hand with a Swamp is a keep" | **HALF-MISTEACH; pilot overrode the wrong half** | vs139 s1: 7-card hand = 3 Mountains + March + Bowmasters + 2x Feed the Swarm — **3 lands, zero black sources, zero castable spells.** The count clause says KEEP. The pilot MULLIGANED. s2: 2 Mountains + 5 black/red spells, again no black source -> mulliganed. s3: 4 lands including a Swamp -> **KEPT** (the Swamp clause fired correctly). The pilot used the colour clause and ignored the count clause, which is the right call and the opposite of what the leading sentence says. §4. |
| T10 | "SUMMONING-SICK OPPONENT CREATURES CAN STILL BLOCK" | **superseded by the render** | The engine now prints `[summoning sick - cannot attack this turn, but CAN block]` — 95 occurrences in my prompts, zero misreads, zero paradox verbalizations. The guide line is TRUE but inert. Trim. |
| T11 | "The `{right now: damage 0}` note on Foray is not the real number" | **NOW FALSE** | §2a. The note is exact. HL10 reconciliation — this sentence must go or it teaches the pilot to distrust a correct annotation. |
| T12 | Rule #1's quoted render string | **NOW FALSE / dead string** | §1. |

**Did any teach MISLEAD?** Three sentences are now wrong or wrong-shaped: T11 (false since i14),
T12 (dead string since i12), T9's leading clause (colour-blind). **None of the three cost a game**
— T11 and T12 were simply not exercised in a harmful direction, and the pilot overrode T9. T8's
wording contributed to a leak in a lost game.

### 3c. PRESERVE ledger — held under the first guide (HL8 do-no-harm check)

The wave-31 initial guide was written under HL8 (short, confirmatory attack section; no
manufactured posture). All five preserve items survived:
- P1 attack floor **21/21** (was 30/30 unguided) — no overshoot, no under-declaration.
- P3 attack-as-engine: vs146 Army 1/1 (t5) -> 6/6 (t7) -> **11/11 (t9)** for the kill.
- P4 removal targeting: 2/2 correct (T5).
- P5 blocking: 13/14 windows defensible; the 14th is the fallback (§6b).
- P6 mulligan: 5/6 openers kept on the first hand; the one exception (vs139) is T9.

---

## 4. THE RESIDUE — what the revision is actually for

**R1 (the loss-tracing leak, #1 slot). The blocker seam has no floor and produced the corpus's
one fatal fallback.** vs152 s35, t16, at 6 life. Offered exactly:
```
A1. Moonrage Brute (3/3) deals 3 [first strike, nightbound]
A2. Sigarda, Champion of Light (5/5) deals 5 [trample, flying]
B1. Orc army (1/1) - may block A1 (your blocker dies, attacker lives)
```
Reply: `BLOCKS: Orc army: Sigarda, Champion of Light; Dunland Crebain: Moonrage Brute`. Three
faults in one line — (a) NAME:NAME form instead of the protocol's `B1:A2`; (b) `Dunland Crebain`
is not on the blocker list and the board line says
`Dunland Crebain {2}{b} (1/1) [flying] [tapped - cannot attack or block this turn]`; (c)
`Orc army` -> `Sigarda` is an illegal block the render already excluded (`may block A1` only —
Sigarda has flying). Unparsed -> Baka -> no blocks -> 3 + 5 = 8 damage at 6 life -> **-2, game
over.** The render was correct and explicit at every point; this is a compliance failure the guide
can address with a per-seam procedure (the render's own `- may block AN` list is the whole answer,
and the guide currently says nothing about how to READ it).

**R2. Mulligan is a colour problem dressed as a count problem.** deck158 is 26 lands, 12 Mountain
/ 14 Swamp, **zero duals**, and the entire amass engine except Foray/Swarming/Olog-hai is black.
A 3-land all-Mountain hand is a mulligan and the guide's first clause calls it a keep. Also needs
the HL10/Step-5.6 numeric floor: the pilot took 2 mulligans in vs139 with no terminator written
anywhere.

**R3. T8's exception is in a trailing "unless" clause.** Step 5's wording doctrine: an exception in
a subordinate tail loses to the default that leads the sentence. Restructure exception-first with
the numeric test spelled out.

**R4. Two decklist cards have no guide line at all** (Assault on Osgiliath x2, Grishnakh x1).

**R5. Reconciliation sweep (HL10)** — delete T11 and T12's dead-string prose, retarget Feed the
Swarm and Foray at the annotations that now carry the number, trim T10.

---

## 5. LOSS DECOMPOSITION — 5/6 -> 3/6, honestly

**Pool delta first (HL7).** Wave-32 rotated out 122 (I killed it t13), 148 (my only wave-31 loss)
and 137 (I killed it t13), and rotated in 105, 36, 116. My record against the two HOLDOVERS I also
faced in wave-31 went 2-0 -> 0-2 (139, 152), and my record against the three DEBUTS was 2-1. Pool
adjudication rate fell 3/21 -> 1/21, so **the adjudication column explains none of the delta** —
this is a real 5-kill -> 3-kill move against a harder field, with the kill clock four turns faster.

**L1 — vs105 (BG infect). NOT a decision loss: the clock was INVISIBLE.**
Final state at my last decision (s20, t13): `Your life: 15 | Opponent life: 8`, my board a 4/4
Army and 9 lands, their board one tapped `Blackcleave Goblin {3}{b} (2/1) [haste, infect]`. I was
ahead on both life and board and I lost on the spot. Cause: **poison counters are rendered
NOWHERE.** The prompt's situation block has `Your life:` / `Opponent life:` and no poison line;
the narration prints `- Damage: 5 dealt by Putrefax to you` and `- Damage: 2 dealt by Blackcleave
Goblin to you` with **no accompanying life change and no counter line at all** — an event with no
visible consequence. Cross-checked at the opponent's seat: deck105's OWN prompts also show no
poison total (its last board render is `Your life: 3 | Opponent life: 15` with three infect
creatures and no count). Neither player can see the alt-win clock. Routed **N-158j, HIGH** — and
note that deck105's 6/6 perfecta was won on this invisible axis, which makes it a corpus-level
integrity item, not just mine.
Secondary, guide-lane: T8's missed Bowmasters ping at s7 (§3b) gave a 1/1 infect body a free turn
in the game where every infect point was 10% of a lethal clock. Attribution: dominant cause
render/engine, contributing cause guide wording.

**L2 — vs139 (Temur/Sultai Mutants). Colour screw, not a play loss.**
Two mulligans (both hands all-Mountain with 4-5 black spells), kept 4 lands at 5 cards, bottomed 2
(i7 correct: 2 mulligans -> exactly 2 bottoms, `PUT: 1, 4`). Stuck on **Mountain + Swamp = 2 lands
from t3 to t11** while holding Foray `{3}{R}`, Swarming `{2}{R}` x2, Ugluk `{2}{B}{R}`. Opponent
assembled Gemrazer (4/4 trample) + Dryad + Illuna (6/6 trample flying). The seq16 fallback (§6a)
occurred at 2 life in a position the model itself scored as lost. The guide's T9 count-clause would
have KEPT the first screw hand; the pilot's override was better than the guide.

**L3 — vs152 (Bant Midrange). Blocker-seam fallback at 6 life.** §4 R1. Contributing: at s32
(t15, 6 life) the pilot attacked with all three bodies including the borrowed Sigarda, leaving only
the summoning-sick `Sauron, the Lidless Eye (4/4) [... but CAN block]` home — a defensible line that
was undone when `Fateful Absence` killed Sauron on the opponent's turn (narration, t17). That is
opponent removal, not a decision defect. The decisive decision is s35.

**Summary: 0 of 3 losses is "the guide taught the wrong line."** One is an unrendered win
condition, one is mana colour screw the guide's mulligan clause would have made WORSE, one is a
reply-format/compliance fallback at a seam the guide never addressed.

---

## 6. THE TWO FALLBACKS (both mine; 2 of the corpus's 3)

### 6a. `retracted_choice` — vs139 seq16, t11, Main phase 1, my_life 2, 12,180 chars / 206.0s

Options: `1. Cast Mordor Muster {1}{b} {card text: ...}` / `2. Cast nothing right now`.
`choice: -1`, `fallback: retracted_choice`.

**The reply OPENS with a correct, complete, protocol-compliant answer:**
```
CHOICE: 1 (Cast Mordor Muster {1}{b})
Reasoning: I am at 2 life and the opponent has two massive attackers ... I need to generate an
immediate threat that can attack this turn. ... PLAN: Cast Mordor Muster to create a 2/2 Orc
Army (boosted by Mauhur) and draw a card.
```
It then continues **past its own PLAN: line** — `"Wait, looking at the opponent's board..."` /
`"Actually, let's re-evaluate the 'win this'..."` — for another ~10,000 characters of survival
search, and never re-commits. The safety route dropped the answer and Baka answered.

Classification: **TRUE POSITIVE for the parser (a genuine post-PLAN retraction spiral), but it
discarded a correct answer.** Trigger shape, for the ledger: a hopeless board state
(life 2, opponent lethal next turn) + a two-option list + a self-audit loop that begins AFTER the
PLAN line with "Wait/Actually". Nothing about this is the wave-26 example-echo class or the
wave-24 hypothetical-prose class; it is a COMMIT failure. Cheapest lever is a guide/prompt commit
rule ("your answer ends at the PLAN line"), not a parser change.

**A second, separable finding inside the same reply — a NEW cost-notation misread:**
```
Grishnakh cost: {2}{R}.
I need 2 generic and 1 Red.
I have 2 sources. Can I pay {2}{R}?
Yes, 2 sources = 2 mana. One is Mountain (R), one is Swamp (B).
... So I can pay {2}{R} with one Mountain and one Swamp.
Why is Grishnakh not in the list?
```
`{2}{R}` is THREE mana; the model treated the leading generic as the total. The **engine was
correct** to withhold the cast (this is NOT an i1/N-152c castable-but-not-offered instance — I
verified payability by hand from `Mana available: 2 total` and `mana={2}{R}`), and the model
reached the right conclusion anyway (*"I cannot cast Grishnakh if it's not in the list"*), citing
the core prompt's own rule. But it burned ~2k chars on it and this is the belief that replaces the
wave-31 mana-pool belief. One guide line, per §1.

### 6b. `unparsed_reply` — vs152 seq35, t16, Blockers, my_life 6, 3,385 chars / 27.7s

Full verbatim head: `BLOCKS: Orc army: Sigarda, Champion of Light; Dunland Crebain: Moonrage
Brute`. `choice: 0`, `fallback: unparsed_reply`. Analysed in §4 R1 — name-form instead of
`Bn:An`, a tapped creature asserted as an available blocker, and an illegal flying block. The
reply's own body shows the model treating its 2-blocker premise as settled fact
(*"Is there a better way? No, I only have 2 blockers"*) and computing trample math for a block the
render never offered. **Game-losing.**

Note for the parser ledger: the reply IS a compliant `BLOCKS:` head line by shape — it just uses
card NAMES on both sides of the colon. i6's truncation salvage does not cover it (nothing was
truncated). A name-tolerant fallback in the blocks parser would have recovered a partly-legal
assignment here (Army -> Moonrage Brute is legal and survives 3 of the 8 damage); routed
**N-158n** as a harness-lane proposal, not a guide item.

---

## 7. Step-1 batch items witnessed at this seat (beyond i12/i14)

- **i8 (N-122c/N-139k combat tags): VALIDATED.** `[tapped - attacking]` x22, `[summoning sick -
  cannot attack this turn, but CAN block]` x95, `[tapped - cannot attack or block this turn]` x64,
  and **zero** occurrences of the old bare `cannot attack this turn]` form. **Zero paradox
  verbalizations** across all 120 replies (grepped `contradict|paradox|inconsistent|conflicting`
  — no hits). The wave-31 C3 leak (summoning-sick read as "cannot block") did not recur.
- **i13 (N-146i broad card text on Cast lines): VALIDATED, 100%.** 103 `N. Cast ...` option lines;
  70 are real casts and **70/70 carry `{card text: "..."}` or a `legal targets right now:` list**;
  the other 33 are `Cast nothing right now`. No length-related degradation at this seat — p95 reply
  length FELL 46% in the same wave the prompt grew.
- **i7 (N-139i London bottoming): VALIDATED at my one mulligan game.** vs139: `KEEPS` after 2
  mulligans -> `bottom` record with 7 constant labels -> `PUT: 1, 4` = exactly 2 bottoms.
- **i5 (loud re-ask):** **zero** `validation_reject_reask` / `_exhausted` markers at this seat, and
  zero defers — consistent with the corpus-level note that no validation reject occurred in this
  pool. Wave-31's N-158i (X-slack-0 silent defer) did NOT recur, but **Assault on Osgiliath was
  never offered this corpus**, so its trigger condition never engaged: **UNEXERCISED**, not fixed.
- **i16 (AIStats FATAL):** crash-grep clean, 0/6.
- **i1, i2, i3, i6, i9, i10, i11, i15:** no witness at this seat (no duals, no convoke, no Agadeem,
  no truncation, no 0-power attackers, no equipment, no mulligan-with-creature misread, no DFC).

## 8. Carried open items re-confirmed at this seat

- **N-158e (flash windows), KNOWN-OPEN, steady state:** 3 priority windows / 6 games, 1 on the
  opponent's turn, Bowmasters never offered at instant speed. Not counted against the guide (per
  brief). The guide's de-flash teach is the correct response and stays.
- **N-158d (Foray target menu names the wrong source), PERSISTS:** `- TARGET CHOICE for Orc army -
  its "damage creature" ability ... answer with the chosen TARGET's name (not "Orc army")`
  (vs152 s26-s35). The ability-name qualifier is new and helps, but the SOURCE is still the Army
  rather than Foray of Orcs. No misplay traced; latent.
- **N-158f (Assault emblem / double strike + haste):** UNEXERCISED — Assault never offered.
- **N-158b (counter narration magnitude):** CLOSED by i14 (§2c).
- **N-158c (Foray `damage 0`):** CLOSED by i14 (§2a).
- **N-158h (Feed the Swarm constant life cost):** CLOSED by i14 on the cast line; sibling gap
  N-158k opened on the target menu.
- **N-158g (mana line):** CLOSED by i12 (§1).

## 9. Guide-draft summary

The engine identity, the attack floor and the cast floor are all working and are FROZEN territory;
the revision must not re-legislate them. The residue is four narrow things: **a blocker-seam
procedure keyed to the render's own `- may block An` list** (the one fatal decision), **a
colour-based mulligan rule with a numeric floor** (the clause the pilot had to override), **an
exception-first Bowmasters ping test** (the missed exception in the lost game), and **lines for the
two decklist cards the guide never named**. Everything cured by i12/i14 comes OUT: Rule #1's
14-line procedure demotes to one cost-notation falsity fact, the Foray "the annotation is wrong"
sentence is deleted and replaced by "the annotation is exact — read it," Feed the Swarm points at
`(costs you N life)`, and the summoning-sick paragraph is retired to the render.
