# deck126 (Sanguine Blood) guide rewrite — Amendment 332

## (a) Rules KEPT (condition -> action)
1. Land drop: "Play <land>" row listed -> take one, every turn, any life; pick the missing colour.
2. Mana line is a generic FLOOR (+1 per defender past the first); coloured pips come only from lands
   and Lantern, counted off the source sub-list (Bond needs two {b}).
4. Hand card with no "Cast" row is not castable; a lone Battlement mana row -> pass unless you can
   name the spell you cast with it now.
6. Both halves out -> take the first win entry (Tribute at 1+, Pride Guardian block, any wall block
   under Perimeter Captain, Staff ping at opponent, lifelink Vampire attack).
7. A priced win row is still taken while its own "you would be at N" is 1 or more.
8. DRAW FORECAST N at or above your life -> lethal draw step, act now. The loop starter must be your
   own card: both halves out with no starter -> cast Staff/Sorin/Wall of Omens/any wall each turn.
10. Dead-row list (Tribute at 0, second-copy tag, dead-Tutor tag) -> hold row, not decline.
11. Pair = two different names; second copies do nothing; a countered copy is not on your line, and
    Bond alone is a clock while Blood alone starts nothing -> prefer Bond when holding neither.
13. Cast a half the turn it appears; Tutor only when a half is missing from hand AND battlefield,
    then pick the half not printed on either line (neither -> Bond). Silverquill "[named: <half>]"
    -> take the other half, in the search and at the cast.
16. Tribute is an edict; the row's own text decides (N creatures / named victim / at 0). It fires
    on: both halves out; a named non-defender victim (first window, over both halves); unblockable
    damage at 12 or less life; their only creature unblockable; a lethal CRACK-BACK line; or life
    needed to survive.
18. Tribute holds on a [defender] victim unless life <= 10 or both halves out; ON THE STACK already
    carries your Tribute -> decline; zero stop at "at 0 this does nothing".
20. Blocking STEP 1: lives/dies parenthesis outranks the STOPS-N tag; carve-outs (enchantment out,
    header <= 8, lifelink Vampire); "both die" is a take; converter clause -> every blocker blocks.
22. STEP 2: one blocker per blockable attacker, menace needs two or none, biggest "deals N"
    tiebreak, Captain 2 per blocking defender / Pride Guardian 3, life on declaration; at a "this
    KILLS you" header, biggest "deals N" per blocker and ignore lives/dies. "BLOCKS: none" only
    when STEP 1 benches every blocker; never concede.
25. Attackers: defenders never attack; collapsed tag / GANG BLOCK stops; surplus-Vampire rule; both
    halves out overrides every stop; Lightmine Field -> ATTACK: none until Sorin's -2 emblem is on.
26. Cast order 0-10 (survival floor, missing half, Tutor, Captain, walls, Staff, Sorin, Lantern,
    Staff/Sorin promotion with both halves out, Tribute at N creatures, decline).
27. Lantern above walls only with a combo piece in hand and mana available <= 4; one Lantern only.
28. Answer protocol: one CHOICE line, name matches row, number in range, RE-ASK handling; hold row
    conditions, and decline only when you want to be re-asked.
30. Staff ping at the opponent every upkeep; Sorin +1 each turn, -2 emblem once vs 1-damage boards.
31. Silencer tag arithmetic (3 life + a card) and the three cast cases.
32. Draw-punisher board (a) forecast-metered vs (b) cast-metered.
33. Cleanup discard order (Tribute last, never against a token-maker); mulligan keep/ship lines and
    the bottoming preference order.

## (b) DROPPED, by category (byte estimates, from the 71,483-byte live guide)
- Game narration / "you lost a game to this" / life-total post-mortems: ~24,000
- Citations, filenames and per-window counts of past behaviour: ~9,000
- Threefold repetition of the same rule (body + cast-list entry + situations): ~11,000
- Meta-commentary about the guide, its size band, and its own rule cross-references: ~3,500
- "WHAT THIS COST" ledgers and second-person history of past answers: ~3,000
- Long re-derivations of engine behaviour already printed by the prompt: ~2,000  (total ~51,500)

## (c) Rules that could NOT be restated as condition -> action
- None dropped for that reason. Two were re-keyed from history to board state: the Tribute
  "second window of the same turn" stop (now the ON THE STACK section) and the repeat-attacker
  Lightmine stop (now the log line "your attacker died before combat damage").

## (d) Verification
- `stat -c %s strategy.txt` = **19952** (ceiling 20000)
- `grep -ciE 'corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for )?by|[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]'` = **0**
