# Wave 70 — deck 146 (Orzhov dungeon midrange) per-deck review

Seat files: `matchups-20260906-224849/*ai_baka_deck146-<ptr>-vs-*.jsonl` (6 games, 329 decisions,
0 fallbacks, 0 truncations, 0 `action_before_plan`, 15 records with `off_protocol_bytes>0`).
Record note: the native reasoning is in the field **`reasoning`** (with `reasoning_chars`), NOT
`reasoning_content` as the brief states — `reasoning_content` does not exist in any record of this
corpus (0 of 2,119). Every deck-146 decision carried reasoning (min 330 / median 5,318 / max 18,367
chars). Seat latency p50 83.4 s, p95 173.5 s, max 354.1 s.

## 1. Game by game

| # | Opponent | Result | Turn | Final life |
|---|---|---|---|---|
| 1 | 152 | WIN | 21 | 20 / -10 |
| 2 | 126 | WIN | 19 | 21 / -1 |
| 3 | 123 | WIN | 13 | 20 / 0 |
| 4 | 162 | WIN | 13 | 14 / 0 |
| 5 | 130 | WIN | 21 | 11 / -1 |
| 6 | 125 | **LOSS** | 34 | 0 / 16 |

Deck 146 went 5-1, the best record in the corpus. The five wins were clean curve-outs: land every
turn, a venture body, Lolth/Kaya on the turn they were payable, and removal fired on the row that
named a real target. Nothing in those five games needed the guide changed.

**Wins — the decisions that decided them.**
- vs 152 (seq 15, 19): two Soul Shatters cast on rows reading "they sacrifice Intrepid Adversary"
  / "Luminarch Aspirant" — cast-order entry 1, correct. Kaya -3 on Intrepid Adversary (seq 28) and
  Lolth -3 for Spiders (seq 29) on the turn each landed; the Spiders closed it (seq 39, 45, 53).
- vs 126 (seq 6, 10): both Silverquill Silencers named the halves of their drain pair
  (Sanguine Bond, then Exquisite Blood) off the guide's pair rule, before either was cast;
  Pelakka Predation then stripped the revealed Sanguine Bond (seq 15). Textbook.
- vs 123 (seq 25/26): Vanishing Verse on Intruder Alarm — an enchantment, off the target list, not
  a creature reflex.
- vs 162 (seq 26): switched to Tomb of Annihilation only after `Dungeons completed (you): 1` at
  21 vs 12 life — exactly the guide's "racing and clearly ahead" gate. Cost 5 life, won on T13.
- vs 130 (seq 20): Silverquill Command mode 4 with the render's edict verdict
  ("they control 1 creature - Rorix Bladewing is sacrificed") — the only mode that answered a
  6/5 flier the deck cannot block.

**Loss vs 125** (`1788752943-ai_baka_deck146-...-vs-ai_baka_deck125.jsonl`). Deck 125 is
creatureless permission-control: 2x Staff of Nin, 3x Sphinx's Revelation, Elixir of Immortality,
Supreme Verdict, Final Judgment, 5 counterspells. Most of this is variance and matchup, not guide:
Barrowin was countered on T7 and T9, Adventurer on T11 and T23, Acererak on T27 and T29; both
Angel pairs were wiped (Supreme Verdict T14, Final Judgment T20); Kaya — the guide's stated only
answer to a colourless artifact — was revealed off a Lost Mine "Place on top" room on T13 (seq 24
log) and then shuffled away by the opponent's own Path to Exile land-search on T14. Three Soul
Shatters sat dead in hand for eleven turns, correctly (every row read "at 0 this does nothing").

The one decision in the game that was the model's own error is **seq 104, turn 29**, and the
reasoning channel names it outright:

> "Animating Hive now doesn't change combat this turn because it's Main Phase 1, and creatures
> can't attack the turn they enter play unless they have haste. Hive doesn't have haste."

That is a rules error, not a misread surface. The Hive had been on the battlefield since turn 15;
animating a land you already control does not make it summoning sick. Row 2 of that priority menu
was live and payable (9 untapped sources, 8 of them not that Hive), and row 1 — the *tapped* Hive —
carried the engine's own `[this land is TAPPED: animated, it still cannot attack this turn]` and
`{right now: does nothing this turn}`, so the render was correct and discriminating; the model
supplied the false rule itself. It answered `CHOICE: 3 (Hold priority)` and attacked with nothing
on a turn it was at 6 life against a 2-damage-per-upkeep clock. 3 damage lost; it lost the race by
16. STRATEGY (guide item), not ENGINE.

## 2. Engine / interface / card items

**HIGH — none.** No fallback, no truncation, no parser refusal, no false verdict, no missing fact
at decision time in 329 decisions. Every card fact I checked verified: Hive of the Eye Tyrant and
Silverquill Command primitives (`bin/Res/sets/primitives/borderline.txt:53391`, `:102739`) match
Scryfall Oracle text exactly, including the Command's flying rider and the "of their choice" edict.

**MED 1 — the log narrates a modal spell as RESOLVED before its own mode ask.**
`1788752960-ai_baka_deck146-...-vs-ai_baka_deck130.jsonl` seq 20 (turn 15). The GAME LOG in the
prompt for the mode question ends:
```
- You cast Silverquill Command
- Your Silverquill Command resolved and went to your graveyard
```
and the very next thing asked is `Choose an option for Silverquill Command:` (rows 1-9). The model
burned reasoning on the contradiction — "the log says it already resolved? That's weird... it's a
prompt artifact... I will just pick option 4 as instructed" — and answered correctly anyway. The
narration lies about ordering at every modal-spell seam; repro is any Silverquill Command cast.

**MED 2 — the "manland animation is asked at its own window" promise is silently false when the
animation is unpayable.** Cast menus print *"THIS IS THE CASTING SEAM ONLY... the activated
abilities of your permanents (a manland animation among them) ... are asked at their own windows."*
The model took that literally and wrote animation plans at three windows where the row would never
appear: vs-126 seq 20 (4 sources, one of them the Hive → only 3 other sources for a 4-mana
activation) and vs-125 seq 44 and 45 (4 then 3 sources, same shape). Each was a turn that then went
to combat with only a 1/1 Goblin attacking. The rung ceiling that explains it
(`Hive of the Eye Tyrant's OWN mana is not counted toward this activation`) is printed only ON the
priority row, i.e. only when it is already affordable — the model can never read it in the window
where it would change the plan. Fix candidate: put the ceiling note in the hand/battlefield line for
a manland, or annotate the cast menu's promise with "when payable". (I cover this from the guide
side below; the engine side is the cheaper fix.)

**LOW — `async_drops: 5` in the vs-125 gameend counters** (only game of the six with any). No
visible effect on the transcript; flagged for the engine seat, not diagnosed here.

**Reply-shape census (all 15 `off_protocol_bytes>0` records in this seat).** One shape only: the
first line is the plan sentence with the literal `PLAN:` prefix missing; the action line is always
present and always parsed. Byte counts 25-119. Quotes:
- vs-152 seq 20: `Attack with both to reduce opponent to 14 life and advance the game.` / `ATTACK: Triumphant Adventurer, Goblin`
- vs-126 seq 42: `Attack with all three creatures to guarantee lethal damage regardless of blocking.` / `CHOICE: 1 (Idyllic Tutor)`
- vs-125 seq 45: `Animate Hive of the Eye Tyrant for {3}{B}, attack with Goblin and the animated 3/3 menace, end turn.` / `CHOICE: 3 (Cast nothing right now)`
In 4 of the 15 the unprefixed plan describes the whole intended sequence while the action line is
its first step (seq 42/45 above) — the two-line contract is satisfied in substance, only the label
is dropped. No record put an action before a plan. Nothing here cost a decision.

## 3. Guide verdict: EDIT

Small, one subject: the Hive. Everything else in the guide was either exercised and correct
(cast order, Soul Shatter's dead-row test, Verse's target list, the pair-naming Silencer rule, the
Tomb gate, the mulligan keeps) or never came up. Live guide 19,954 bytes → revised 19,927 bytes,
under the 20,000 ceiling. `wave70/deck146/strategy.txt`.

**Edit 1 — ADD, in the HIVE OF THE EYE TYRANT block (the bytes this rule earned):**
- before: `- HIVE OF THE EYE TYRANT: {3}{B} - FOUR mana - for a 3/3 menace attacker until end of turn. Before / answering "becomes beholder" run these in order...`
- after: same, plus
  `A Hive you already controlled as the turn began IS NOT SUMMONING SICK: animate it in your own main phase and it ATTACKS THIS TURN. It needs no haste, and no window ever gives it haste.`
- paid for by: vs-125 seq 104 (turn 29) — the reasoning quoted above declines a payable animation
  on the haste rule and attacks with nothing at 6 life. The guide's CHECK ladder tells the model
  *when* to animate but never states that the animated land can attack, so a model that arrives
  with the summoning-sickness prior has nothing on the page to overturn it.

**Edit 2 — ADD, same block (the bytes this rule earned):**
- after: `AFFORDABILITY: its own mana cannot pay for it, so read "Mana available: N" as N MINUS ONE here (minus one more for a second Hive you also mean to attack with) - FOUR OTHER untapped sources or the row is not offered at all, and a plan naming an animation you cannot pay for is a turn spent doing nothing.`
- paid for by: vs-126 seq 20 (opponent at 7 life, 4 sources incl. the Hive, plan named the
  animation, turn ended with a lone Goblin swing), vs-125 seq 44 and seq 45 (same shape, turn 17).

**Edit 3 — CUT, `CHECK 3` (the `{b}`-source special case):** subsumed word-for-word by Edit 2's
general rule; keeping both would leave two tests for one fact. Freed 165 bytes.

**Edit 4 — CUT, WORKED SITUATIONS bullets 1 and 3** (the Soul Shatter MV-0 example and the
duplicate-Kaya cleanup example): each restates its own section's rule with no new discrimination —
the SOUL SHATTER bullet already carries the tied-at-MV-0 test verbatim, and CLEANUP DISCARD rule (2)
already carries `{you already control one: <name>}`. Freed 405 bytes. The middle bullet (the
Shield-Sphere / Pride-Guardian tag combination) is kept: it composes three separate rules and is
not derivable from any one of them.

Neither addition is a general-guide or skill matter — both are facts about one card in this deck —
so no `general-proposals.md` or `skill-proposals.md` is written.

## 4. What I did NOT check

- The opponents' seat files beyond using them for board context; I did not audit deck 125's play.
- The 5 `async_drops` in the vs-125 game — not traced to specific seams.
- Mulligan and bottoming quality: all six games were untouched-7 keeps, so the mulligan block of
  the guide got no exercise at all and I make no claim about it.
- Blocking: deck 146 was never attacked into a decision worth reviewing in five of six games, so
  the BLOCK LADDER is untested this corpus from this seat.
- Corpus-wide census, the wave-70 lane predictions, and the latency-vs-wave-69 ratio — engine seat.
