# Bug-list item 1 — HUMAN EDICT PROMPT, observed on the GUI

**Verdict: PASS.** Twice, in one windowed game, a Tribute to Hunger cast BY the Baka AI at the
human seat stopped the engine, presented the sacrifice choice to the human, sacrificed exactly
the creature the human clicked — a creature that was NOT the lowest-toughness one either time —
and paid the caster life equal to that creature's toughness. Nothing was auto-resolved.

Binary: `projects/mtg/bin/wagic`, built 2026-09-15 01:26 from master `982394151` (the accepted
`badbba7d3` engine). Repo untouched apart from this file.

## What was launched

```
systemd-run --user --scope -q -p MemoryMax=4G -p MemorySwapMax=0 -- \
  env WAGIC_AI=baka WAGIC_TRANSCRIPT_TRACE=1 ./wagic        # from projects/mtg/bin
```
Windowed (NOT headless — the human seat needs the GUI), moved to Hyprland workspace 9 and
fullscreened; driven with ydotool press-hold-release clicks. Play → 1 Player → Classic →
Single Game → deck "EDICT GUI PROBE" → **Evil Twin** (both seats play the same list).

Throwaway human deck `bin/User/player/deck4.txt` (deleted afterwards; `bin/` is gitignored and
the collection is not enforced):

```
Swamp (PSAL) *14   Tribute to Hunger (ISD) *8
Ornithopter (MRD) *4   Phyrexian Walker (VIS) *4   Steel Wall (MRD) *4
Shield Sphere (ALL) *4   Memnite (SOM) *2
```

The creatures are 0-cost/1-cost artifact creatures of **distinct toughness** (Ornithopter 0/2,
Phyrexian Walker 0/3, Steel Wall 0/4, Shield Sphere 0/6), so the human's pick is observable and
the pick can be read off the life the caster gains. The opening hand held no land and three
0-cost creatures, so turn 1 put Ornithopter (0/2), Phyrexian Walker (0/3) and Shield Sphere (0/6)
on the human battlefield with nothing else to do but pass.

Human seat = `p1` in the transcript trace; the Baka AI = `p2`.

## Observation 1 — turn 7, three candidates, human picks the 0/6

AI casts (its own words, `~/.gatelogs/edict-gui/gui.log:780-807`):

```
 AI wants to play card.
- Next card to play: Tribute to Hunger
AIPlayerBaka: AI attempting to pay a mana cost.
-  Target: Tribute to Hunger
-  Cost: {2}{b}
[transcript-trace] click p2.battlefield[0] -> 1 swamp   (x3, the three Swamps tapped)
[transcript-trace] click p2.hand[0] -> 0 tribute to hunger | zone now: tribute to hunger swamp swamp tribute to hunger swamp
ACTIONSTACK Add spell
Action added to stack: Tribute to Hunger
Resolving Action on stack: Tribute to Hunger
...
CHECKING Spell            <-- line 818: the engine stops here
```

**(a) The engine asked the human.** The log produces nothing further for as long as the human
does nothing: after `CHECKING Spell` the next line in the file is the human's own click. On
screen (`~/.gatelogs/edict-gui/s13.png`) the granted sacrifice ability is offered to the human
seat as a menu — header bar reads `(opponent's turn - you play) Main phase 2`, the menu entry is
the effect's own name `ToughLife` — and once it is taken the human's three creatures are the
selectable set while the AI's creatures grey out. The choice is the human's: the engine did not
resolve it, did not pick, and did not move on.

**(b) The human picked a creature that is not the lowest toughness.** The board was
Ornithopter 0/2, Phyrexian Walker 0/3, Shield Sphere 0/6. The click went to Shield Sphere, the
**highest** toughness — the exact opposite of the pre-fix lowest-toughness autopick:

```
GAMEOBSERVER Click
Action added to stack: StackAbility.  (Source: )
[transcript-trace] click p1.battlefield[2] -> 1 shield sphere | zone now: ornithopter phyrexian walker shield sphere
Resolving Action on stack: StackAbility.  (Source: )
```

**(c) Exactly that creature was sacrificed and the caster gained its toughness.**
`s15.png`: the human battlefield is Ornithopter 0/2 + Phyrexian Walker 0/3 — Shield Sphere gone —
and the AI's life reads **26**, the human's **20**. Life before the spell: **20 / 20**
(`s13.png`). 20 → 26 = **+6 = Shield Sphere's toughness**, not +2 (Ornithopter, the old autopick)
and not +3. The human gained nothing, which is correct for Tribute to Hunger ("**You** gain life").

## Observation 2 — turn 9, two candidates, human picks the 0/3

Same shape, repeated with the two survivors (`gui.log:907-927`):

```
 AI wants to play card.
- Next card to play: Tribute to Hunger
-  Cost: {2}{b}
[transcript-trace] click p2.hand[1] -> 0 tribute to hunger | zone now: swamp tribute to hunger swamp
Action added to stack: Tribute to Hunger
Resolving Action on stack: Tribute to Hunger
```

The `ToughLife` menu is presented to the human again (`s16.png`, `(opponent's turn - you play)
Main phase 1`, human board Ornithopter 0/2 + Phyrexian Walker 0/3, life 20 / 26). The human
clicked Phyrexian Walker — again **not** the lowest toughness:

```
GAMEOBSERVER Click
Action added to stack: StackAbility.  (Source: )
[transcript-trace] click p1.battlefield[1] -> 1 phyrexian walker | zone now: ornithopter phyrexian walker
Resolving Action on stack: StackAbility.  (Source: )
```

`s17.png`: Phyrexian Walker is gone, **Ornithopter 0/2 survives**, AI life **26 → 29** (+3 =
Phyrexian Walker's toughness), human life 20.

## Independent confirmation from the transcript

The game's own replay dump carries the life totals in the engine's save format, and the untap
snapshots bracket both edicts:

```
#transcript turn turn=9  life=20/26      (after observation 1)
#transcript turn turn=10 life=20/29      (after observation 2)
```

The dump replays clean — the first verified **human-seat** transcript replay:

```
WAGIC_REPLAY: replayed .../transcript.txt -> turn 10 phase 1 life 20/29
```
(no `REPLAY DIVERGED`). Evidence kept outside the repo at `~/.gatelogs/edict-gui/`
(`gui.log`, `transcript.txt`, `s13/s15/s16/s17.png`).

## What could NOT be observed

* **The re-choice branch** (`AADynamic::resolve`'s `W82-EA` arm, the `gEdictRechoiceArmed`
  path) never fired. Both observations rode the ORDINARY path: the granted
  `notaTarget(creature|mybattlefield) sacrifice` ability is handed to the targeted player and
  that player answers its chooser. The `W82-EA: edict re-opens the sacrifice choice for ...`
  DebugTrace does not appear anywhere in the log. Making the stored victim stale (control
  change / phase-out / CANTBESACRIFIED inside the granted ability's window) needs a constructed
  board that random GUI play will not hand you; that branch remains pinned only by
  `w82ea_edict_rechoice_is_the_players.txt` and `w84gc_edict_seat_is_the_targeted_player.txt`.
* **Devour Flesh** was not cast by the AI in this game (the second deck revision dropped it to
  keep the creature count high). Its script is the same family with `targetcontroller` instead
  of `targetopponent`, so the lifegain seat differs; that half is untested on the GUI.
* **The Vita/PSP build.** This is the desktop SDL GUI. The same engine code drives the Vita
  chooser, but no console package was built or run for this check.
* **One-candidate and zero-candidate boards** were never reached (the human always had 2+
  creatures at resolution), so the inline "only legal answer" and "nothing to sacrifice" arms
  were not exercised here.
