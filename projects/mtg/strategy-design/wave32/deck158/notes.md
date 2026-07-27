# deck158 — development notes (wave 32)

Corpus `matchups-20260727-115847`, binary `7fabd9bd0`, 6 games at this seat. Layer-routing:
everything here is ENGINE / RENDER / HARNESS. Nothing below belongs in guide prose.
Card facts verified against `bin/Res/sets/primitives/borderline.txt` + `_macros.txt`.

---

## N-158j — POISON / INFECT STATE IS RENDERED NOWHERE. **P0, corpus-integrity.**

**Severity: HIGH (decided a game at this seat and plausibly all six of deck105's).
Repro quality: PERFECT (deterministic, every prompt in the matchup).**

**Symptom.** deck158 lost to deck105 on turn 13 at `Your life: 15 | Opponent life: 8`, ahead on
life and on board, with the opponent's only creature tapped. The alt-win fired with zero
warning in the prompt.

**Evidence.** In `1785173974-ai_baka_deck158-...-vs-ai_baka_deck105.jsonl`, the
`--- CURRENT SITUATION ---` block of every record contains `Your life: N | Opponent life: M` and
**no poison line, no counter line, no infect summary**. The narration prints infect damage as an
ordinary damage event with **no consequence at all**:
```
- Phase: Combat damage
- Damage: 5 dealt by Putrefax to you
- Phase: Cleanup
...
- Damage: 2 dealt by Blackcleave Goblin to you
```
Neither line is followed by `- Your life -N` (correct: infect deals poison, not damage) **and
neither is followed by any poison counter narration** (the defect). Life stayed at 15 across both.

**Both seats are blind.** Checked the opponent's own log
(`*deck105-0x*-vs-ai_baka_deck158.jsonl`): its last board render is
`Your life: 3 | Opponent life: 15` with `Blackcleave Goblin [haste, infect]` and
`Ichor Rats [infect]` on its battlefield and **no poison total for either player**. The infect
pilot cannot count its own clock either; its plan line reads *"Cast Ichor Rats to gain an
immediate poison counter on the opponent"* with no way to know the running count.

**Why this is P0 rather than a deck158 item.** deck105 went **6/6** this corpus — the brief's
headline "first ever Step-0 perfecta". That record was won on a state variable absent from every
prompt in the corpus. Per the perception-vs-strategy routing, no guide at any seat can address it.
Any wave-32 conclusion that reads deck105's six games as decision-quality evidence, or reads its
opponents' losses as guide failures, is reading an artefact.

**Fix direction.** Add poison to the board snapshot on BOTH sides whenever either player has a
non-zero count — the same treatment `[counters: 9x +1/+1]` already gets on permanents — and
narrate the counter event (`- Poison counter added to you: +5 (now 7 of 10)`). The lethal
threshold (10) should be printed, since it is a rule the pilot cannot derive from the board.
Engine support is confirmed present (the alt-win fires); this is purely a render omission.

**Adjacent check the fix should cover (same class, not yet witnessed):** any other alternate win
or loss condition in the pool — empty-library/mill, "you win the game" statics, energy/experience
thresholds. See deck158 skill.md P5 for the intake sweep.

---

## N-158m — i14 AMASS PREVIEW IS BLIND TO MACRO-DEFINED CARD SCRIPTS. **HIGH.**

**Severity: HIGH (the annotation was built for this deck and reaches 5.8% of its amass offers).
Repro quality: PERFECT (static, deterministic, mechanism read from source).**

**Measured coverage at this seat**, over every `N. Cast <amass card>` option line in 6 games:

| card | offers | with `{right now: Army X/X -> Y/Y}` |
|---|---|---|
| Foray of Orcs | 3 | **3** |
| Easterling Vanguard | 17 | 0 |
| Mordor Muster | 13 | 0 |
| March from the Black Gate | 9 | 0 |
| Orcish Bowmasters | 7 | 0 |
| Dunland Crebain | 2 | 0 |
| Swarming of Moria | 1 | 0 |
| **total** | **52** | **3** |

**Mechanism (source-read, not hypothesis).**
- `dynamicMagnitudes` -> `amassCounters(card)` -> `amassCountersFromScript(card->magicText)`
  scans for the literal substrings `counter(1/1.` then `counter(1/1)`
  (`projects/mtg/src/AIPlayerGPT.cpp:246-259`).
- **Foray of Orcs writes its amass literally** in both `auto=` branches
  (`transforms((Orc,newability[counter(1/1.2)], ...))`) -> matches -> renders.
- **Every other amass card in the deck uses the `_AMASSORCn_` macro**
  (`bin/Res/sets/primitives/_macros.txt:68-72`), e.g. `Mordor Muster` is
  `auto=ability$!name(Amass orcs 1) _AMASSORC1_!$ controller`.
- Macro expansion happens on a LOCAL copy inside the ability factory:
  `projects/mtg/src/MTGAbility.cpp:6650`, `magicText = AutoLineMacro::Process(magicText);` — the
  result is **never written back to `CardPrimitive::magicText`** (`src/CardPrimitive.cpp:358-360`
  appends raw `auto=` values). The scanner therefore sees the opaque token `_AMASSORC1_`, finds no
  `counter(`, and returns 0.

**Fix directions (either is small).**
1. Call `AutoLineMacro::Process(...)` on the text inside `amassCountersFromScript` before scanning
   — this also fixes every FUTURE macro-defined magnitude for free (`_TREASURE_`, `_FOOD_`, and
   any later `#AUTO_DEFINE` that carries a number).
2. Or map the five `_AMASSORCn_` macro names to their counts directly (n = 1..5) as a special case.

Option 1 is preferred: it closes the class, not the instance. Whichever ships, the validation
metric is the COVERAGE FRACTION above re-measured on the next corpus, not "Foray renders".

**Negative control that must not break:** `Assault on Osgiliath` has nine
`alternative if compare(fullpaid)~equalto~N` branches carrying `counter(1/1)` through
`counter(1/1.8)`, so `values.size() > 1` -> return 0. The multi-branch suppression is correct and
must survive the fix (it is the guard that keeps a confidently-wrong number off an X-spell).

---

## N-158k — Feed the Swarm's per-target life cost renders on the CAST line but not on the TARGET menu. **MEDIUM.**

**Severity: MEDIUM (the decision is committed at the surface that lacks the number).
Repro quality: PERFECT (7 cast renders and 4 target menus in this corpus).**

Cast line (i14, working) — vsk36 s9:
```
2. Cast Feed the Swarm {1}{b} - legal targets right now: Ornithopter (costs you 0 life),
   Salvage Slasher (costs you 2 life), Cathodion (costs you 3 life)
```
Target menu, the seam where the target is actually committed — vsk36 s18:
```
- TARGET CHOICE for Feed the Swarm ... Pick the ONE target it will affect from the list below
  1. Ornithopter (0/2) [flying] [opponent's battlefield] - "Flying"
  2. Cathodion (3/3) [opponent's battlefield] [tapped] - "When Cathodion dies, add {3}."
```
No costs. The pilot survived at vsk36 (it carried the arithmetic across from the cast ask and won
that turn) but answered vs152 s26 and s31 — at 11 and 6 life — with no price visible.

**Fix.** Emit `(costs you N life)` on the target lines too, from the same
`life:-manacost` evaluation. Generalises: any magnitude attached to a cast option whose choice is
re-asked at a later seam should be echoed at that seam.

---

## N-158l — Counter-source attribution emits an EMPTY `[from ]` when the source is a sorcery already in the graveyard. **LOW, latent.**

**Severity: LOW (no misplay traced, no verbalized confusion).
Repro quality: PERFECT (38 distinct prompts).**

The i14 counter narration ships three variants at this seat:

| form | distinct prompts | condition |
|---|---|---|
| `- Counter added to Orc army: +1/+1 (now 1/1) [from March from the Black Gate]` | 90 | amass CREATES the token; source is a permanent still on the battlefield |
| `- Counter added to Orc army: +1/+1 (now 2/2)` (no bracket) | 63 | amass onto an EXISTING Army (the `_AMASSORCn_` `transforms` branch) |
| `- Counter added to Orc army: +1/+1 (now 1/1) [from ]` | 38 | amass CREATES the token; source is a SORCERY already moved to the graveyard |

Traced by preceding narration: named sources are `March from the Black Gate`, `Dunland Crebain`,
`Easterling Vanguard`; the empty bracket always follows
`- Your Mordor Muster: stack -> graveyard`, `- Your Foray of Orcs: stack -> graveyard`, or a
Bowmasters ETB ping (vs139 s18, vs152 s35).

**Fix.** Resolve the source name before the source leaves the stack, or suppress the bracket
entirely when the name is empty (an empty `[from ]` is strictly worse than no bracket). The
COUNT half of the fix is correct and valuable — `(now N/N)` retired N-158b — so this is a
polish item on an otherwise-good change.

**Note the third variant is arguably also a gap:** the existing-Army path (63 prompts) carries no
source at all, so the pilot cannot attribute the majority of its counters. Low priority — the
running total is what it actually reads — but if the fix touches this code, unify the three.

---

## N-158n — Blocks parser rejects a well-formed `BLOCKS:` head line that uses card NAMES instead of `Bn:An` labels. **MEDIUM, harness-lane proposal.**

**Severity: MEDIUM (game-losing at vs152 s35). Repro quality: GOOD (1 instance, exact text).**

vs152 seq35, t16, my_life 6, `fallback: unparsed_reply`, `choice: 0`. Options were:
```
A1. Moonrage Brute (3/3) deals 3 [first strike, nightbound]
A2. Sigarda, Champion of Light (5/5) deals 5 [trample, flying]
B1. Orc army (1/1) - may block A1 (your blocker dies, attacker lives)
```
Reply head, verbatim:
```
BLOCKS: Orc army: Sigarda, Champion of Light; Dunland Crebain: Moonrage Brute
```
Fallback -> Baka -> no blocks -> 3 + 5 = 8 damage at 6 life -> **-2, game over.**

Three faults, only one of which the parser can own:
1. **Parser-ownable:** NAME:NAME instead of `Bn:An`, with `;` as the separator instead of `,`.
   The line is otherwise a compliant answer-first `BLOCKS:` head. A name-tolerant pass (match each
   side against the rendered blocker/attacker names, drop unmatched pairs) would have salvaged
   `Orc army -> Moonrage Brute`... except that pair was not offered either (see 3), so the salvage
   here yields nothing. **The value of the proposal is therefore forward-looking, not retroactive
   — do not credit it with this game.**
2. **Not parser-ownable:** `Dunland Crebain` is not a B-line and the board says
   `Dunland Crebain {2}{b} (1/1) [flying] [tapped - cannot attack or block this turn]`. The
   existing illegal-assignment drop already handles it correctly.
3. **Not parser-ownable:** `Orc army -> Sigarda` is illegal (Sigarda flies; the B-line offers
   `may block A1` only). Correctly droppable.

**Recommendation.** Add the name-form tolerance to the blocks parser (cheap, and the attackers
parser's tolerance already sets the precedent), and ship the regression cases PARSETEST wants: a
positive (`BLOCKS: Orc army: Moonrage Brute` with a matching render), a negative (a name that
matches no rendered label must NOT match), and the mixed form. **This is not a fix for the vs152
loss** — that decision was lost at the perception layer (a tapped creature asserted as available)
and is addressed by the blocker-seam procedure now in strategy.txt.

---

## N-158d — CARRIED, PERSISTS: Foray of Orcs' damage-target menu names the ARMY as the source.

**Severity: LOW. Repro quality: PERFECT (all Foray casts).**
```
- TARGET CHOICE for Orc army - its "damage creature" ability (this spell/ability is already on
  the stack and needs a target ...) ... answer with the chosen TARGET's name (not "Orc army")
```
(vs152 s26-s35.) The ability is `transforms(...)`-ed onto the Army token, so the menu attributes
it to the Army rather than to Foray of Orcs. NEW since wave-31: the ability-name qualifier
(`its "damage creature" ability`) has been added, which materially improves legibility. The
SOURCE is still wrong, and a menu that names your own creature as the thing asking for a target
is an invitation to self-target. No misplay traced in either corpus. Keep open, low priority.

---

## N-158e — CARRIED, KNOWN-OPEN, steady state confirmed: no flash windows.

3 `priority` decisions in 6 games. Exactly ONE on the opponent's turn (vs152 s34, Main phase 1),
and its sole option was `Draw 1 with Clue [cost: {2}, Sacrifice]`. **Orcish Bowmasters was never
offered at instant speed in 6 games** despite being in hand across many opponent turns.
Mechanism located in wave-31 (Baka's `computeActions` builds no casts outside main phases on its
own turn; shared-path, fix deferred). Per the wave-32 brief this is NOT counted against the guide,
and the guide's de-flash teach ("cast it in your main phase like any other creature") remains the
correct response for as long as the item is open.

---

## N-158f — CARRIED, UNEXERCISED this corpus.

Assault on Osgiliath's `emblem transforms((,newability[lord(*[orc;goblin]|mybattlefield) double
strike],newability[lord(...) haste])) ueot` appeared NOT to reach the Army in wave-31 (two
witnesses). **Assault on Osgiliath was never offered in any of the 6 wave-32 games**, so the
trigger condition (cast Assault with an Army on board / creating one) never engaged. Per HL4
rule 2 the verdict is UNEXERCISED, not resolved. A probe deck is the right instrument if this is
to be closed — stack `deck199` with 4x Assault, 4x March, 8 Mountain / 8 Swamp and run one pinned
game; the observable is an Attackers phase in the same turn Assault resolves, and first-strike
damage ordering in the combat log.

Same note applies to wave-31's **N-158i** (X-affordability-slack-0 silent defer): it is an
Assault-only trigger at this seat, it did not recur, and it is **UNEXERCISED**, not fixed by i5.

---

## L-158-1 — LENGTH-ONLY WATCH: whole-turn planning displaces onto the LAND-DROP seam.

**Severity: NONE observed (every land-drop choice was correct). Repro quality: GOOD (2 instances).**

Wave-31's three largest replies were all land drops rehearsing the mana-pool belief (S-158b); i12
killed that belief and p95 reply length fell 46% (9,866 -> 5,287). Two large land-drop replies
survive with entirely different content:
- vsk36 s16, t10: 10,763 chars / 161.8s on `Play Swamp`. The body is a complete lethal
  computation plus a Feed-the-Swarm target comparison — the whole turn planned at a 3-option
  seam, then planned again at the cast ask two records later.
- vs116 s13, t8: 5,587 chars / 86.6s on `Play Swamp`.

Two contributing surfaces, both present in the vsk36 s16 prompt: (a) the land-drop ask carries the
FULL board, hand and plan but only three trivial options, so deliberation has nowhere to go but
the rest of the turn; (b) the stale-plan notice fired
(`"the actions your plan names are no longer among the options available right now - the game
state has advanced past that plan; re-derive your choice from the current board"`), which
correctly invalidates the plan and implicitly invites a full re-derivation at the cheapest seam
available.

**Not a defect and no action proposed** — decision quality at these seams was 100%. Recorded so
the trend is visible if a future corpus shows the tax growing back, and because it is the answer
to "what is the residual tax rehearsing now?" (deck158 skill.md P2).

---

## Corpus-hygiene observations (no action)

- **Crash gate: 0 signatures** across all 6 stderr files at this seat
  (`game-158v105`, `game-158v116`, `game-158v36`, `game-139v158`, `game-146v158`, `game-152v158`),
  grepping `FATAL|Segmentation|Assertion|terminate called|core dumped`. i16 confirmed clean.
- **Clue token card text renders empty with a questionable source attribution:**
  `Draw 1 with Clue [cost: {2}, Sacrifice] {card text: "() source: Mauhur, Uruk-hai Captain"}`
  (vs152 s18/s21) — an empty `()` where the text should be, and the source names Mauhur on a Clue
  that arrived from `Fateful Absence`'s investigate. Cosmetic; no decision affected; noted only
  because i13's card-text work touched this surface.
