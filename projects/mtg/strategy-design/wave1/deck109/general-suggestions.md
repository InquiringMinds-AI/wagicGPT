# Suggestions for the GENERAL strategy prompt (system_prompt.txt)

Vantage: writing/reviewing deck109 (Hellrider mono-red aggro) exposed one real,
generalizable misfire in the shared prompt, plus two cheap executable additions that
help every deck. Constraint respected: the general prompt must stay short, so each
addition below is paired with a justification and an offsetting cut. Line numbers
refer to the current `bin/Res/ai/gpt/system_prompt.txt`.

---

## Suggestion 1 (HIGHEST PRIORITY) - "hold interaction" makes aggro hoard its reach

**The observed bug.** In the deck109 self-play games, qwen sat on Pillar of Flame and
Searing Spear turn after turn while the opponent stayed alive - in two games it held
2-3 burn spells unused and the opponent's life plateaued for many turns. The cause is
line 19's instruction, read literally by a weak pilot: "Hold instants, removal, and
tricks until you must use them ... **waiting costs nothing** and gives you
information." For a burn/aggro deck that phrase is false: the burn is not an answer to
hold, it is the clock, and waiting costs damage. This generalizes - many decks run
spells that are damage/reach, not reactive answers, and the current wording tells the
pilot to hoard all of them.

**Where:** line 19, first sentence.

**Current:**
> Act at the latest useful moment. Hold instants, removal, and tricks until you must
> use them or until the opponent's turn or end step - waiting costs nothing and gives
> you information.

**Replace with:**
> Act at the latest useful moment with true ANSWERS - counters, and removal you are
> saving for a specific threat: hold them until you must use them or until the
> opponent's turn, since waiting on an answer costs nothing and gives you information.
> But not every spell is an answer to hold. Direct-damage and burn are part of your
> CLOCK, not reactions. When you are the aggressor, a burn spell held is damage you
> failed to deal - spend it to pressure or finish, usually at the opponent's face,
> rather than hoarding it.

**Why it's worth the words:** this is the difference between an aggro deck closing and
stalling out - a whole-game-swinging error, and it recurred. It also sharpens the
answer/reach distinction for every archetype.

**Optional companion edit (line 5):** after "hold instant-speed interaction for the
opponent's threats," add "- but treat burn and direct damage as part of your offense,
not something to sit on." Only if length allows; Suggestion 1 alone mostly covers it.

---

## Suggestion 2 - add an executable LETHAL check (cross-deck, cheap)

Line 23 already lists "missing lethal" as a common error but never tells the pilot HOW
to check for it. A weak model needs the arithmetic spelled out - it can evaluate a
concrete sum against the board even when it can't reason strategically. This is
exactly how deck109's Hellrider turns (attackers + 1-per-attacker trigger + burn)
should be counted, and the same shape works for any deck with a clock.

**Where:** line 21 ("Combat is arithmetic"), inserted after the first sentence.

**Add:**
> Each turn, check for LETHAL: add the power of the attackers that will connect, plus
> damage from any "when this attacks / when this deals damage" triggers, plus burn you
> can send to the opponent's face. If that total is at least the opponent's life, take
> it - attack with everyone and spend the burn; never leave lethal on the table to
> keep cards in hand.

**Why:** turns an already-named error (line 23) into an action. Small, high-leverage,
helps aggro and midrange alike.

---

## Suggestion 3 - tell the faster deck to keep attacking

Line 17 correctly identifies who should be the aggressor but stops at "deploy threats,
attack." The behavioral failure mode for a weak pilot is subtler: it holds creatures
home for defense or waits for a "clean" attack, slowing the very clock it needs.

**Where:** line 17, appended to the sentence that begins "The side that would LOSE the
race must be the aggressor..."

**Add:**
> As the aggressor, attack every turn with everything that isn't needed to survive a
> lethal swing back; do not hold creatures home to block or wait for a perfect attack
> - a faster clock, not clean trades, wins the race. Trading a cheap attacker into a
> blocker to push damage or clear a path is fine when you are ahead in the race.

**Why:** pairs with the beatdown call already there and prevents the "durdle on
defense while ahead" leak. Keep it this tight.

---

## Offsetting CUT - trim the bluffing paragraph (line 20)

To hold total length roughly flat, cut down line 20. It is the longest strategy bullet
and the least executable by this pilot: qwen only outputs an action number from the
legal list and cannot meaningfully model "what the opponent believes" or orchestrate a
represented-but-empty bluff. The one executable idea in it (open mana represents a
trick) is worth one sentence; the rest is high-skill meta-game the weak executor won't
act on.

**Current (line 20):**
> Control the opponent through the information you show them. They see your untapped
> mana, your attacks, and your board; your hand stays hidden unless a card reveals it.
> Leaving mana open represents an instant (counter, trick, removal) and can deter an
> attack or push the opponent into a worse line even when you hold nothing. Make some
> plays for their effect on what the opponent believes, not only their direct board
> effect - and back the representation with the real card when you have it, so the
> bluff stays credible.

**Replace with:**
> Leaving mana open represents an instant (counter, trick, removal) and can deter the
> opponent even when your hand is empty - but back the bluff with a real card when you
> have one, or it stops being credible.

**Net length impact:** Suggestions 1-3 add ~8 lines; this cut removes ~4. Net ~+4
lines for two whole-game-swinging fixes and an executable lethal check - a good trade.
If tighter budget is needed, Suggestion 3 can be compressed further, but keep
Suggestion 1 in full - it is the one that was actually costing games.

---

## Not recommended

- Do not add per-archetype sections to the general prompt - that belongs in the
  per-deck `{STRATEGY_GUIDE}`, which is where the deck-specific fixes should live.
  Keep the shared prompt to priors that apply to every deck.
- Do not reorder the whole STRATEGY list; the targeted edits above are lower-risk than
  a restructure.
