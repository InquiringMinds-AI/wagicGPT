# deck146 seat — proposal for the general guide (`wave81/general-strategy.md`)

One proposal. Evidence is the deck125 game,
`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260917-191141-final/1789690316-ai_baka_deck146-0x555bfc3f8960-vs-ai_baka_deck125.jsonl`.

## P-146-1: a counter test that is not scoped to one card

**What the corpus shows.** deck146's guide carries a COUNTER TEST, but it is filed inside cast-order
entry 2 and therefore reaches only Lolth and Kaya. Against deck125 the seat deferred Lolth correctly
(seq23) and then, over the following eighteen turns, walked eight other spells into the same open
mana one per turn — Nadaar (seq74), Acererak (seq95, seq98), Emeria's Call hard-cast for
`{4}{w}{w}{w}` into 15 untapped `{u}{w}` sources while its own land row sat on the menu (seq128),
Silverquill Command (seq130), Triumphant Adventurer (seq151), Silverquill Silencer (seq189) and
Soul Shatter (seq227). `opp_countered_spells` for that game is 12; `counter_intel_rendered` is 1319,
so the intel was on the screen at essentially every window.

**Proposed general rule (decision-time, both facts printed):** when the opponent's line names a
counterspell they can still cast AND `Their untapped sources` covers its cost, a window whose chosen
row would spend your whole turn's mana on ONE object is the wrong window: take the cheapest row that
leaves your own sources up, or the land row, and put the expensive object on a later window where a
second row also gets taken. The two facts the rule keys to — the counter's presence and their
untapped-source count — are exactly the two deck146's entry-2 test already uses, so this generalises
a rule the corpus shows working rather than inventing one.

**What it is NOT.** Not "hold every threat": the seat that holds everything is the seat that discards
it (deck146 seq48, seq60). The rule fires only on a row that spends the whole turn on one object,
which is the shape every countered spell in this game had.
