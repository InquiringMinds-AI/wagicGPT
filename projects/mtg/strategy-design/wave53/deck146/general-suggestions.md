# Core-prompt / render proposals from the deck146 seat (wave 53)

Layer-routed: everything below is a change to what the prompt PRINTS or to how a window is
offered. Engine defects (the hang, the dead-row re-offer, the identical-option ask) are in the
seat file's engine list, not here.

## R-1 (HIGH, render). A MODAL CAST ROW PRINTS NO PER-MODE LEGALITY, SO A "CHOOSE TWO" CARD IS
## PRICED FROM THE NARRATION LOG.

Every `Cast Silverquill Command {2}{b}{w}` row in the corpus (28 of them) prints exactly two
annotations: `{leaves N of your M untapped mana sources untapped}` and `{card text: "Choose two
-- Target creature gets +3/+3 and gains flying until end of turn. -- Return target creature card
with mana value 2 or less from your graveyard to the battlefield. -- Target player draws a card
and loses 1 life. -- Target opponent sacrifices a creature."}`. Which of the four modes is live
right now is nowhere on the row; the pilot must reconstruct its own graveyard by reading eighty
turns of interleaved narration. It got it wrong: `146v125` s240 asserts *"my graveyard has no
creature cards with mana value 2 or less"* with two such cards named in the log directly above.

Targeted rows already carry this class of annotation - lane O put `{kills 0 of the N legal
targets at -1/-1}` / `{kills: <name>}` on cast rows (129 + 40 renders corpus-wide), and edict
rows carry `{right now: they sacrifice <name> (MV 15, their highest)}` (1,586 `{right now:`
renders corpus-wide). **Ask:** extend the same treatment to modal spells - one clause listing
which printed modes have a legal object right now and which do not, e.g.
`{modes live right now: return (Triumphant Adventurer, Silverquill Silencer); draw. Dead: pump
(you control 0 creatures), sacrifice (they control 0 creatures)}`. This is the same information
the engine must already compute to build the mode sub-menu, and it generalises past this card to
every Choose-N spell in the pool.

## R-2 (MED, render). WITHHOLD - OR AT LEAST STOP RE-OFFERING - A ROW THE ENGINE HAS ITSELF
## MARKED DEAD.

`at 0 this does nothing` rendered **614 times corpus-wide / 396 at deck146 alone** and was taken
**2 times corpus-wide, 0 at my seats**. At `146v125` the single dead `Cast Soul Shatter` row
accounts for the great majority of the seat's 425 `Cast nothing right now` answers: 139 of the
165 own-turn declines that held "another live cast row" held only that one. The annotation is
correct and the pilot obeyed it 396 times out of 396. What it costs is a menu slot, a decision,
and (with the re-ask economy below) a model call at every phase stop.

**Ask:** when a cast row's own annotation says the spell does nothing, do not put it on the menu.
If it must stay for legality/optionality reasons, at minimum it should not be the row that keeps
a priority window open. This was wave-52 E-3 at the same seat; it is restated here with a bigger
denominator, not a new claim.

## R-3 (HIGH, prompt wording). THE HOLD ROW'S VERB COLLIDES WITH THE PILOT'S OWN WORD FOR IT, AND
## THAT COLLISION IS 8 OF THE CORPUS'S 15 FALLBACKS.

Lane N's row renders as `Hold priority - do not ask me again this turn unless the board changes`.
Corpus: **1,173 renders, 29 takes (2.5%)**; 21 of those 29 are one seat in one game, and
`hold_windows_skipped` is 102 across 40 gameends with 38 of them at 0. Meanwhile every one of the
seven `plan_choice_conflict` records at `146v125` (s150, s371, s392, s398, s470, s491, s555) is
the same collision: the coded line names the HOLD row and the PLAN sentence says *"I will pass
priority"* / *"I pass"*. The detector re-asks; three of the re-asks flipped to `pass` (row 0) and
three re-confirmed HOLD, so it is a real disagreement in half the cases and a false positive in
the other half. The pilot does not read "hold priority" and "pass priority" as different acts.

**Ask, in order of cheapness:** (a) reword the row with the verb the model already uses -
`Pass priority, and do not ask me again this turn unless the board changes` - so the row reads as
a variant of the answer it is already giving rather than a fourth, alien option; and (b) treat
"pass" in the PLAN as consistent with that row so the conflict detector stops re-asking it. The
128/128 pass at `CHOICE: 0` on menus carrying `0. Pass priority` shows the parser side is fine;
this is a vocabulary problem.

## R-4 (LOW, render). THE MULLIGAN COVERAGE LINE COLLAPSES TWO DIFFERENT CAUSES INTO ONE STRING.

`Playing every land in this hand would not cover any spell in it` is printed both for "you have
too few lands" and for "you have enough lands of the wrong colour". Six of the seven mulligan
decisions on my seats that met that line were correct; the one that was not (`152v162` s1, 2
lands making {G}{G} against five white spells) was the colour case, and the reply's reason was
the colour. **Ask:** append the cause - `... (you have 2 lands; no spell in it is castable off
{G} alone)` versus `... (1 land is not enough for your cheapest spell at mana value 2)`. The
engine already has both facts on adjacent lines. See also the mulligan's cost: the ask took
595 s, 222 s, 198 s, 187 s and 171 s at five different seats, the largest single-decision
latencies in the corpus.
