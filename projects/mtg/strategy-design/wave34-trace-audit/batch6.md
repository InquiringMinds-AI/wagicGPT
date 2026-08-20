# Wave-34 thinking-trace audit — batch 6

Auditor 6 of 6. Log-reading only; no engine runs.

## 0. Corpus

7 games in `~/.Wagic/ai/gpt/logs/`, **146 reasoning traces** (not 101 — the `ask` kind is
only 69% of the population; `attackers`/`priority`/`blockers` also carry `reasoning` and
were audited alongside).

| file (prefix) | decks | traces | ask / attackers / priority / blockers |
|---|---|---|---|
| 1787165715 | deck158 (Orcs of Mordor) vs deck105 | 17 | 13/3/0/1 |
| 1787172915 | deck36 (Master of Etherium) vs deck158 | 13 | 11/3/0/0 |
| 1787178826 | deck105 (Infect) vs deck146 | 22 | 19/3/0/0 |
| 1787186026 | deck158 vs deck152 | 17 | 11/1/2/3 |
| 1787193226 | deck146 (Dungeon/Acererak) vs deck116 | 20 | 17/3/0/0 |
| 1787200425 | deck36 vs deck116 | 30 | 10/2/18/0 |
| 1787207626 | deck139 (Mutate) vs deck116 | 27 | 20/7/0/0 |

Raw reasoning length: **p50 13,890 · p90 21,451 · max 29,938 chars** (mean 14,189).
`reasoning_budget_hit` fired **twice** (186026 seq2 @12,058 chars; 207626 seq5 @29,027 chars).
One retry in the batch (207626 seq5). Zero `answer_replaced`, zero `commit_retracted`,
zero `dropped_assignments`. `post_answer_overrun` > 0 on 53/146 (mean 51, max 284 chars);
`post_plan_overrun` 0 on all 146.

Method: full close-read of 22 traces (both budget hits, the 10 longest, the high-signal
keyword hits, plus randoms spanning all 7 files and all 4 decision kinds); regex sweep of
all 146 for the diagnostic emitters; hand-scored depth on the close-read set with an
automated category-dedupe proxy run over all 146 as a cross-check.

---

## 1. JOB 1 — diagnostic-depth measurement

**Definition applied.** Depth = char offset of the last sentence that introduces a *new*
development-diagnostic element (false render belief in the model's own words; invented
card fact; verbalized prompt confusion; first plan derivation; annotation misparse).
Re-verified arithmetic, recounted lands, board restatement, and re-assertion of an
already-stated belief were scored as churn.

### Headline (hand-scored, N=22 close-read)

| metric | chars | ≈ tokens (/4) |
|---|---|---|
| **depth p50** | **7,050** | **1,760** |
| **depth p90** | **~15,000** | **~3,750** |
| **depth max** | **18,500** | **4,600** |

**Mean post-diagnostic churn fraction: 0.41** (median 0.45). I.e. on a typical trace the
last ~41% of the reasoning channel adds nothing a developer can use.

### Cross-check (automated, N=146)

Category-first-occurrence proxy over all 146 traces (12 diagnostic categories; only the
first occurrence of each category counted, all later restatements treated as churn):
depth **p50 7,603 · p90 15,305 · max 24,941 chars**; churn fraction **mean 0.400,
p50 0.386, p90 0.696**; median 3 distinct diagnostic categories per trace; 1/146 traces
with zero diagnostic content. The proxy lands within 8% of the hand-scored p50, so the
hand number is not an artefact of my sample.

### By decision kind (proxy)

| kind | n | len p50 | depth p50 | churn |
|---|---|---|---|---|
| ask | 101 | 13,771 | 7,989 | 0.41 |
| attackers | 21 | 13,127 | 6,775 | 0.42 |
| blockers | 4 | 11,969 | 9,748 | 0.30 |
| priority | 20 | 14,741 | 8,570 | 0.37 |

`priority` is the worst value-for-budget: in 1787200425 the model spent **18 priority
decisions × ~14.7k chars each (~264k chars, ~66k tokens)** re-deciding the *same*
Scarecrone activation, because the engine kept re-offering it (see F1).

### Budget implication

- Diagnostic yield is essentially exhausted by **~15,000 chars (~3,750 tokens)**; the
  observed max was 18,500 chars (~4,600 tokens).
- **A ~4,500-token reasoning budget captures ≥90% of development-diagnostic content**
  while removing ~40% of the volume. ~5,000 tokens if you want headroom for the
  attackers/priority tail.
- The budget is currently **non-binding**: only 2/146 traces hit it, so today's 30k-char
  ceiling is not what's producing long traces — the churn is self-generated (see P1–P3)
  and will not shrink by raising the cap.
- ⚠ `reasoning_budget_hit` fired at **12,058** chars in one trace and **29,027** in
  another. Whatever the cap is measured against, it is not `reasoning_chars`. Worth
  confirming the accounting before tuning the number.

### First-diagnostic offset

p50 ~815 chars. The model reaches its first useful observation almost immediately; the
cost is entirely in the tail, not in the ramp-up.

---

## 2. JOB 2 — development findings

Ranked by severity. Every item is quoted verbatim from the trace or the rendered prompt.

### ENGINE / RULES DEFECTS

**F1 — Scarecrone's sacrifice cost is never paid: repeatable free draw.** `1787200425`,
throughout. The engine offered `Draw 1 with Scarecrone [cost: {1}, Sacrifice]` **12+
times** across turns 3–7 while Scarecrone stayed on the battlefield and even attacked:

```
- You: Draw 1 with Scarecrone
- You: Draw 1 with Scarecrone
- Phase: Attackers
- You declared attackers: Glaze Fiend, Scarecrone
```

Model, `priority` seq12: *"If I sacrificed it to draw, it shouldn't be on the battlefield
anymore. ... There is a contradiction in the provided log vs the card text."*
This single defect generated 18 of the file's 30 decisions.

**F1b — Scarecrone's card script is wrong.** Rendered text: `"{1}, Sacrifice a Scarecrow:
Draw a card."` Real Oracle text is *"{1}, Sacrifice an artifact creature: Draw a card."*
Model, seq12: *"The prompt text says '{1}, Sacrifice a Scarecrow: Draw a card.' This is
weird. Usually Scarecrone sacrifices itself."* Two separate faults — wrong sacrifice
class in the script, and no sacrifice performed at all by the engine.

**F2 — `Vanishing Verse` targeting excludes opponent lands.** `1787193226` seq19. Option
rendered as `Cast Vanishing Verse {b}{w} - the only legal targets are YOUR OWN right now
(warning: this would harm your own side...)` while the opponent controlled `Forest #1
[tapped]; Forest #2 [tapped]; Forest #3 [tapped]; ... Island [tapped]` — four monocolored
permanents. Card text is `"Exile target monocolored permanent."` Model:
*"So why does the option say 'only legal targets are YOUR OWN'? Maybe the game logic
considers lands not valid targets for Vanishing Verse?"* The model held three copies dead
in hand for the rest of the game as a result. Suspect the target filter is restricted to
nonland permanents.

**F3 — Acererak re-opens dungeon *selection* mid-run.** `1787193226` seq20. Log said
`You venture into Lost Mine of Phandelver: venture step 4 of that run`, then the very next
decision was `VENTURE - CHOOSE A DUNGEON to enter`. Model: *"This contradicts the log
saying 'You venture into Lost Mine of Phandelver: venture step 4 of that run'."*
Either the trigger is starting a new run instead of continuing the existing one, or the
prompt fails to say a new run is being started.

### RENDER / LOG DEFECTS

**F4 — Counter and life log lines print a stale post-value.** Two confirmed shapes,
both in `1787165715`:

```
- Counter added to Orc army: -1/-1 (now 5/5) [from Contagious Nim]
- Counter added to Orc army: -1/-1 (now 4/4) [from Contagious Nim]
```
The *first* counter of every infect batch prints the value **before** the batch
(5/5 → 5/5 → 4/4; later: 8/8 → 8/8 → 7/7 → 6/6). Same class on life:
```
- Opponent's life -2 (now 6)
- Opponent's life -8 (now 6)
```
and in `1787200425`: `Opponent's life -1 (now 12)` / `Opponent's life -2 (now 12)`.
Both lines print the post-*batch* total, so the running total is unrecoverable from the
log.

**F5 — Multi-counter amass events print as a single `+1/+1` line.** Same file:
`+1/+1 (now 3/3)` → `+1/+1 (now 5/5)` (two counters, one line, "+1/+1" label);
`+1/+1 (now 6/6)` → `+1/+1 (now 8/8)`. With Mauhur out, the doubling *is* applied but is
invisible in the annotation.

**F6 — The cast-line preview does not include Mauhur's doubling, but the strategy guide
promises it does.** `1787165715` seq10 and seq13. Rendered options:
`Cast Mordor Muster {1}{b} {right now: Army 2/2 -> 3/3}` and
`Cast Swarming of Moria {2}{r} {right now: Army 4/4 -> 6/6}` — both computed without the
+1 replacement, with Mauhur on the battlefield. The guide text the model quotes back:
*"the cast line prints the exact result ... and that number is CORRECT, Mauhur's doubling
and the new-token case included."* Model, seq10: *"There is a contradiction between the
Strategy Guide ... and the actual Choice Text provided in this turn."* seq13:
*"Why does the option say 6/6? Maybe the preview doesn't account for Mauhur?"*
This one contradiction consumed ~40% of a 19.9k trace and ~35% of a 21.5k trace.
Fix the preview or fix the guide sentence — the guide is currently making a false promise.

**F7 — Poison-counter line absent for the first half of an Infect game.** `1787178826`
seqs 4–12 have **no** `Poison counters (opponent):` line; it appears from seq15 onward.
Model, seq8: *"looking at the provided '--- CURRENT SITUATION ---' block, there is NO
poison counter line shown. This is unusual given the strategy guide instructions."*
An Infect deck was flying blind on its own win condition for 9 decisions. Likely the line
is suppressed at 0 counters — it should print `0 of 10` instead.

**F8 — Duplicate card names are indexed on the battlefield but not in hand.** THE dominant
churn driver: 45/146 traces (31%) argue about it, in 5 of 7 games. Battlefield renders
`Swamp #1; Swamp #2`, `Forest #1`, `Ornithopter #1`; hand renders bare `Swamp`, `Forest`,
`Ichorclaw Myr` twice with nothing distinguishing them. The model cannot tell a legitimate
second copy from a render bug and litigates it every turn:
- `1787172915` seq4: *"One might be a typo in the prompt description or I drew it twice."*
- `1787172915` seq7: *"There is a contradiction between the Log and the Current Situation
  block regarding the hand."* (~35% of a 13.6k trace)
- `1787178826` seq9: *"Possibility A: I have two copies. ... Possibility B: The log is
  inconsistent."* (~55% of a 10.8k trace)
- `1787207626` seq28: *"Okay, there is a contradiction in the prompt text provided. It
  lists Pollywog in hand *and* in battlefield."* (recurs seq23–28, six consecutive traces)
**Cheapest high-value fix in this batch:** index hand duplicates the same way
(`Ichorclaw Myr #1 / #2`) or annotate `(copy 2 of 4 in deck)`.

**F9 — `[tapped]` in a semicolon list is misparsed as applying to the neighbour.**
`1787178826` seq22, board line
`... Swamp #2; Ichorclaw Myr ... ; Forest #3; Forest #4 [tapped]; Forest #5 [tapped]; ...`
Model: *"The board listing shows Forest #3, #4, #5 tapped. That leaves Swamp #1, Forest
#1, Forest #2, Swamp #2 untapped? That's 4 lands. Where is the 5th?"* — Forest #3 is
untapped; the model attached the following item's tag to it, then disbelieved the
`Mana available: 5 total` line. Repeats twice in the same trace. `mana_tap` confusion of
this class appears in 109/146 traces under a loose match, 11/146 under a strict one.

**F10 — Battlefield type tags drop "creature" for artifact creatures.** Hand:
`Silver Myr {2} (1/1) [artifact creature]`. Battlefield: `Silver Myr {2} (1/1) [artifact]`
— while the header counts it (`creatures: 1`). Same for `Ichorclaw Myr ... [artifact]
[infect]`, `Scarecrone {3} (1/2) [artifact]`. Model re-derives creature-ness by hand each
time.

**F11 — Ability tags are raw engine keywords and are incomplete.** `1787207626` seq28:
`Emrakul, the Aeons Torn {15} (15/15) [flying, nofizzle, protectionfromcoloredspells]` —
**Annihilator 6 is missing entirely** and the tags are unspaced internal tokens. Model:
*"(Annihilator 6 not listed in text summary but standard for Emrakul, though the text
provided says 'flying, nofizzle, protectionfromcoloredspells')"*. It then spent ~2k chars
re-deriving protection semantics from memory because `nofizzle` /
`protectionfromcoloredspells` are not Oracle keywords.

**F12 — Mutate-pile annotation promises combined abilities but does not list them.**
`1787207626`: `[mutated pile - ONE merged creature (top card's name + P/T shown above,
plus the combined abilities of every card in the pile); 2 cards underneath: Migratory
Greathorn, Arboreal Grazer]`. The abilities are named as present but never printed, so the
model enumerated the underneath cards' abilities from memory (and correctly concluded "no
trample", but only after ~1.5k chars).

**F13 — Card-text slot filled with token provenance instead of rules text.**
`1787193226` seq15, target list: `3. Goblin (1/1) [your battlefield] - "(red) source:
Lost Mine of Phandelver"`.

**F14 — Card-text blobs are truncated mid-sentence and are then echoed into the answer.**
`{card text: "... If you don't control an Army,..."}`. Because the instruction says
*"copy a real number and name from the list"*, the model reasons at length about whether
to echo the whole blob and often does:
`CHOICE: 1 (Cast Mordor Muster {1}{b} {right now: Army 2/2 -> 3/3} {card text: "You draw a
card and you lose 1 life. -- Amass Orcs 1. (Put a +1/+1 counter ...)"})`. This is the main
source of the 53 `post_answer_overrun` events. Give options a short stable label and put
card text on a separate, non-copied line.

**F15 — DFC/modal-land choice echoed with no context.** `1787193226` seq12, log line
`Choose an option for Brightclimb Pathway: -> Play Land` appearing at turn 7 for a Pathway
that entered at turn 5. Model: *"This is confusing. Brightclimb Pathway is a land card. If
it's on the battlefield, I don't play it from hand. ... seems like a glitch or artifact of
the simulation."*

**F16 — `Dungeons completed (you): 1` with no current-run/room-position line.**
`1787193226` seq19: the model saw "1 completed" and "venture step 4 of that run" and could
not tell which dungeon it was in or which rooms remained: *"Step 5 options are not listed
in the current prompt, but standard Lost Mine path often leads to Dark Pool or Goblin
Lair."* It then invented a room table and planned against it. (Room-position is already
docketed; the **new** residual is that the *completed-count* and the *in-progress run*
are rendered as if they were the same fact.)

### PROMPT-CAUSED CONFUSIONS

**P1 — "PLAN: only if your plan changed" is the single largest churn generator.**
Present in 142/146 traces under a loose match; the *first* articulation is genuine
diagnostic, everything after is oscillation. Two distinct unresolved ambiguities:
1. *No prior plan exists* (first decision of a game). `1787165715` seq1: *"Since there is
   no last stated plan, technically it hasn't 'changed'. But to be helpful ... I should
   probably define the plan"* — then flips 8 more times over ~8k chars before answering.
   `1787186026` seq2 (a **budget-hit** trace) is the same loop.
2. *Executing a plan vs changing it.* `1787165715` seq10: *"Does executing a step count as
   a change? Usually, no."* → ~9k chars of the 19.9k trace.
**Fix:** state the rule mechanically — e.g. "write PLAN whenever any sentence of your last
plan is now false or done; on the first decision of a game, always write PLAN."

**P2 — Land-drop availability is never stated, so the model re-derives it every turn.**
Decision menus are per-type, and nothing in the situation block says whether the land drop
is still available. 34/146 traces argue about it.
`1787200425` seq3: *"There is no option to play a land here."*
`1787178826` seq6: *"It is NOT listed in the choices provided ... This implies the land
drop for the turn has been handled or is not an option in this specific menu snapshot."*
`1787172915` seq7: *"If the choice list doesn't have 'Play Land', I assume the land step is
done or skipped."*
**Fix:** one line — `Land drop: already used this turn` / `still available`.

**P3 — The stale-plan note fires when the plan is on track.** 30/146 traces carry
`(note: the actions your plan names are no longer among the options available right now -
the game state has advanced past that plan...)` while the model is executing that very
plan. `1787178826` seq8: *"My last plan said 'Play land each turn, cast infect creatures
early...'. I played a land this turn. Now I am casting a creature. This aligns with the
plan. The note says 'actions your plan names are no longer among the options'. Maybe it
means 'Play Land' isn't an option anymore because I already played it?"*
`1787200425` seq3 shows the same misfire. The note appears to trigger on literal
option-string matching rather than on plan viability.

**P4 — The model treats `YOUR PLAN (as you last stated it)` as an external directive that
overrides its own analysis.** `1787207626` seq28 (attackers) is the clean case: it worked
out that attacking into a 15/15 Emrakul loses its whole board, then attacked anyway —
*"Given the prompt asks me to 'Declare attackers', and provides a 'Plan' suggesting I
should attack, I should follow the plan. The plan is the user's stated strategy."*
Also *"if the Plan says 'Attack', I should execute the Plan."* This directly defeats the
"your plan may be STALE or WRONG" instruction. The label needs to read as self-authored
and explicitly non-binding.

**P5 — `Phase: X | It is the opponent's turn` reads as a contradiction.**
`1787200425` seq29: *"Usually, you don't get a Main Phase 2 on the opponent's turn. ...
This is contradictory."* And when the model is the attacker, `Phase: Blockers | It is your
turn` (seq12): *"This is strange phrasing. ... This likely means 'It is the opponent's
Declare Blockers step'."* Say who is acting: `opponent's Main phase 2 — you have priority`.

**P6 — Format-instruction conflict on target prompts.** `1787193226` seq15: the target
block says *"answer with the chosen TARGET's name (not 'Lost Mine of Phandelver')"* while
the global footer says *"its name in parentheses ... action name exactly as listed"*.
Model: *"This is slightly contradictory ('Target's name' vs 'action name exactly as
listed')."*

**P7 — Only one castable copy is offered when the hand holds several, with no note.**
`1787178826` seq8 (two Ichorclaw Myr, 4 mana, one option): *"I have 4 mana. I can cast one
or two Ichorclaw Myrs ({2} each). But the options only list one 'Cast Ichorclaw Myr'. ...
I have to pick one action."* A line saying "you will be asked again after this resolves"
would end the class.

### DECK / CARD-POOL OBSERVATIONS

**F17 — `Relic Putrescence` is a dead card in the Infect deck.** `1787178826` seq22:
`Cast Relic Putrescence {2}{b} - the only legal targets are YOUR OWN right now {card text:
"Enchant artifact -- Whenever enchanted artifact becomes tapped, its controller gets a
poison counter."}` — the opponent (a Selesnya-ish deck) ran zero artifacts all game, and
the deck's own artifact creatures tap to attack, so the only legal play is self-harm. The
engine's warning annotation worked (the model correctly passed), but the card should
probably not be in the list. The annotation itself is a **good** emitter — keep it.

### TRACE-LEVEL PATHOLOGIES (budget-relevant structure)

1. **PLAN-line oscillation** — median ~6 restatements of the same "is this a change?"
   question per trace; up to 14 in `1787165715` seq10. Largest single lever on volume.
2. **Terminal format re-verification** — the last 1–3k chars of most traces are
   "copy the option name exactly / don't write reasoning / one line or two", repeated.
   Directly caused by F14's long option labels.
3. **Contradiction re-litigation** — an unresolved render contradiction (F8, F6) is
   raised, provisionally resolved ("trust the CURRENT SITUATION block"), then re-raised
   verbatim 2–4 more times. Resolving the render kills the whole tail.
4. **Duplicate-decision grinding** — `1787200425` re-decided the same Scarecrone
   activation 18 times at full reasoning cost. Any engine loop (F1) multiplies budget by
   the loop length.
5. **Mana re-arithmetic** — "can I pay {1}{B} with two Swamps? yes" re-derived 3–5 times
   per trace despite the `Mana available` line and the "already checked your lands"
   guarantee. The guarantee is stated in the guide but not on the option line itself.

---

## 3. Recommendation

**Supported reasoning budget: ~4,500 tokens (≈18,000 chars); 5,000 with headroom.**
That is above the observed diagnostic max (4,600 tokens) and above p90 (~3,750), and cuts
~40% of the current volume, all of it churn. It will truncate essentially no
development-diagnostic content.

Two caveats before turning the dial:
- Verify what `reasoning_budget_hit` is measured against — it fired at 12,058 and 29,027
  `reasoning_chars` in the same batch.
- The budget is not what is producing long traces today (2/146 hits). Fixing **F8**
  (index hand duplicates), **P1** (mechanical PLAN rule), **P2** (land-drop status line)
  and **F14** (short option labels) will cut more volume than any cap, and will do it by
  removing the confusion rather than by cutting the model off mid-thought.
