# Suggested improvements to the GENERAL strategy prompt

Target file: `bin/Res/ai/gpt/system_prompt.txt` (shared by every deck; also read
by the weak qwen executor, so keep edits imperative and keep total length in
check). Grounded in systemic failures observed piloting deck131 and reading its
self-play logs — these are NOT deck-specific, they are how qwen mis-applies the
current wording across decks.

## The headline finding

qwen failed at instant-speed play in two OPPOSITE directions in the same games:
it **hoarded blowouts it never cast** (sat on Aetherize / never overloaded
Cyclonic Rift while losing) AND **fired reactive spells into nothing** (cast a
-4/-0 combat trick on an empty board; cast a counter on an empty stack at the
opponent's end step). Both trace to ONE misleading bullet plus two missing
concepts. Fixing that bullet is the highest-value change and adds almost no length.

## Suggestion 1 (most important): rewrite the "act at the latest moment" bullet

**Current line 19:**
> - Act at the latest useful moment. Hold instants, removal, and tricks until you
>   must use them or until the opponent's turn or end step - waiting costs nothing
>   and gives you information. Play a land before your spells for mana flexibility.
>   When you expect the opponent to hold an answer, prefer committing a lesser
>   threat before your best card. Pass priority (choose 0) whenever there is
>   nothing worth doing right now.

**Problem:** "until the opponent's turn or end step" reads to qwen as an
INSTRUCTION to cast its held spell on the end step — so it dumps counters and
tricks on the opponent's end step with no target. And "waiting costs nothing" +
"pass whenever there is nothing worth doing" over-license passing, so a deck with
answers durdles to death. The bullet never says a reactive card NEEDS a target.

**Proposed replacement:**
> - Act at the latest useful moment - but a reactive card (counter, removal,
>   combat trick) only does something when it has a TARGET: a counter needs a
>   spell on the stack, a combat trick or blocker-shrinker needs a creature in
>   combat. Hold such cards until that target appears; if it does not appear this
>   turn, keep holding. NEVER cast a reactive card into an empty board or empty
>   stack just to spend mana. On your own turn, if your best cards are instants,
>   leave mana untapped so you can use them on the opponent's turn. Play a land
>   before your spells. Pass (choose 0) when you are holding a specific answer for
>   a likely play, or genuinely have no useful action. But passing turn after turn
>   while your life or board slips is losing by inaction: if you hold no answer
>   worth waiting on, develop a threat or spend a resource NOW rather than pass.

**Why it's worth it:** it directly kills the two biggest cross-deck errors, is
roughly the same length as what it replaces, and every clause is executable from
the board + action list. This one edit is the core recommendation.

## Suggestion 2: add one short "who acts when" tempo line

qwen does not grasp that a control/tempo deck acts on the OPPONENT'S turn, not its
own. It treated its own main phase as the time to cast everything. The prompt
teaches "hold instants" but never says WHEN the holding pays off.

**Add (short), near the interaction advice:**
> - Most interaction happens on the opponent's turn: counter their spell as they
>   cast it, shrink or bounce their attackers after they attack. On your turn,
>   develop and leave interaction mana up.

Justify: one sentence, high leverage — it converts the abstract "hold instants"
into a concrete "wait for their turn," which qwen can actually follow.

## Suggestion 3: sharpen the blocking guidance against under-blocking

qwen repeatedly declined ALL blocks and took large, sometimes near-lethal chunks
while holding creatures that could have blocked or chumped. The current combat
bullet (line 21) leans toward NOT blocking ("take the damage when your blockers
are worth more alive"; "chump-block only to buy a turn you genuinely need"),
which a passive model over-applies.

**Add one clause to the blocking sentence in line 21:**
> ... take the damage when your blockers are worth more alive than the life saved;
>   **but do block when the trade is favorable or free, and chump to stay alive
>   when an unblocked attack threatens to kill you or take you dangerously low.**

Justify: no new bullet, just balances an existing one qwen reads one-sidedly.

## Suggestion 4 (optional, only if length allows): name the modal-choice trap

qwen made bad menu/modal picks (chose the wrong color for a "choose a color"
permanent every game). This is minor and partly deck-specific, so include ONLY if
budget remains:
> - On a "choose one / choose a color / choose X" menu, pick the option that best
>   fits YOUR deck's cards, not a default.

If length is tight, cut this — Suggestions 1-3 carry the weight.

## What NOT to change / cut

- The "decide who is the beatdown" bullet (line 17) is good and load-bearing —
  keep it. qwen's failure wasn't that concept's absence; it was the tempo/timing
  execution above. Deck guides make the beatdown role concrete per matchup.
- Don't lengthen the "common errors" list (line 23): it already NAMES the right
  errors ("firing tricks too early or at the wrong target," "tapping out when you
  needed interaction held up"). Naming them didn't stop qwen because the positive
  instruction in line 19 contradicted them. Fix the instruction (Suggestion 1),
  not the error list.

## Net length impact

Suggestions 1 and 3 are in-place rewrites (near-zero net add). Suggestion 2 adds
~2 lines; Suggestion 4 (optional) ~1. The prompt stays about the same size while
correcting the systemic tempo/interaction failures — a good trade, since attention
spent on a misleading line is worse than attention spent on a correct short one.
