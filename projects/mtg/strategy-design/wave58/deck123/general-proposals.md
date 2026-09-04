# deck123 — general-guide / render proposals (wave 58)

## G1 (HIGH, RENDER) — R295's residual: the seat's OWN turn still has no incoming-power fact, and the count it needs is "creatures that WILL untap", not "creatures able to attack right now".

Wave-57 R295 / ledger D9 was written from "a seat at 3 life facing 7 power at **Main 1** sees no
number". Lane C shipped the fix gated on the **opponent's turn** (`wave58/lane-C.md`: the forecast
form covers their upkeep/draw/main 1, and "untap, cleanup and the post-end steps stay silent, as
does the seat's own turn"). The window class the original defect named is therefore still silent,
and deck123 lost two games in it.

Two repros, two different window classes, both on the pilot's own turn:

1. `1788504699-ai_baka_deck123-...-vs-ai_baka_deck162.jsonl` **s23** — Attackers, turn 12, own
   life **3**. The full `Combat: declare ALL attackers` block prints, per attacker, only
   `[held back, it CANNOT block: Fog Bank (flying), Master of the Feast (flying)]` and
   `[their untapped blockers: Fog Bank (0/2) (neither dies ...)]`. Their board held Master of the
   Feast (5/5 flier) and Fate Unraveler (3/4), both `[tapped - cannot attack or block this turn]`.
   The seat sent all three untapped bodies for a non-lethal 13, and `s24` then printed
   `INCOMING THIS COMBAT: 2 attackers, 8 unblocked damage - you would be at -7; this KILLS you ...
   no block saves you`. Every fact in that s24 line was already determinable at s23.
2. `1788504687-ai_baka_deck123-...-vs-ai_baka_deck152.jsonl` **s12** — Main phase 1, own turn,
   life **5**, facing Katilda (3/3), Elite Spellbinder (5/3 flier), Intrepid Adversary (5/3
   lifelink) and Briarbridge Tracker (4/5 vigilance) = 17 power, all but the Tracker `[tapped]`.
   No incoming fact on the menu. The seat tapped out for a 3/3 and died at -12 the next turn.

**Ask.** Render an outgoing/crack-back fact on the pilot's own turn-adjacent windows, sourced
from the creatures that will be able to attack on the opponent's NEXT turn — which on both repros
means counting `[tapped - cannot attack or block this turn]` bodies, because a
currently-able-to-attack count renders **0** on the exact board that kills the pilot. At the
Attackers ask it additionally needs the delta the decision controls: what that total becomes
against the blockers the pilot would have left if it sends this list. Keep D6's under-claim rule
(upper bound over able attackers, no trample carry-over claim) and add no cap on any legal
choice — this is a true number added to a window, nothing removed.

**Second half of the same window, cheap.** The per-attacker tag is one-sided: `[held back, it
CANNOT block: ...]` enumerates only what the body could *not* block. At `vs162 s23` the fact that
decided the game was what each body **could** block (Keeper 7/7 vs the 5/5; a 4/4 vs the 3/4) and
it is nowhere on the screen — while `s24`'s blockers ask computed exactly that, one window too
late. Emitting the `may block` side of the same computation at the attackers ask costs no new
evaluation.

## G2 (MED, RENDER) — a cast menu never says why a hand card is absent, and the pilot spends whole windows naming it.

7 `named_row_reask` fallbacks in deck123's 6 games; 6 of them in one game
(`...-vs-ai_baka_deck126.jsonl` s8, s14, s19, s22, s26, s29) and all 6 named
`CHOICE: n (Cast Thraben Doomsayer)`. At **s28** the frame reads `Mana available: 2 total`, the
hand line lists `Thraben Doomsayer {1}{w}{w} (2/2) [creature]; Bloodline Keeper {2}{b}{b} (3/3)
[creature]`, and the cast menu's only cast row is a Lightning Greaves — correct, and unexplained.
The `[RE-ASK] "Cast Thraben Doomsayer" is not on this list. Answer with a number from 1 to 3.`
recovery worked every time (7/7 legal answers, no wrong cast followed), so this is a cost item,
not a correctness one: one wasted window each, in the game where this deck was already 12 turns
behind.

**Ask.** On any casting menu, name the hand cards omitted for mana and the shortfall — e.g.
`not on this menu for mana: Thraben Doomsayer {1}{w}{w} (needs 3, you have 2); Bloodline Keeper
{2}{b}{b} (needs 4)`. The land-drop line already sets the precedent for telling the model why
something is not among the choices ("The land drop is its OWN decision ... so its absence from
the choices below does not mean it is gone").
