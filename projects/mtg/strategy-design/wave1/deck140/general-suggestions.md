# Suggested improvements to the GENERAL strategy guide

Target: `Res/ai/gpt/system_prompt.txt` (the shared prior every deck receives).
Basis: piloting deck140 and studying its 6 game logs. These are systemic fixes
that should help across decks, not deck140-specific advice. Length is a cost,
so each addition is justified and paired with a cut/compression to stay
roughly length-neutral.

## The measured problem

Across deck140's 6 games the pilot chose "cast nothing" on 82% of its casting
decisions and cast its card-advantage engine and all three finishers ZERO
times. It hoarded answers and threats in hand, played only lands, and either
stalled games at 20-22 life or died holding the exact answer. This is not a
deck140 quirk — it is the weak executor over-applying the general guide's
"hold interaction / act at the latest useful moment / pass when nothing is
urgent" advice to EVERY decision, including its own proactive turns where
waiting costs tempo and leaves win conditions rotting in hand.

The root wording is the current bullet (paraphrased): *"Act at the latest
useful moment. Hold instants, removal, and tricks until you must use them...
waiting costs nothing and gives you information... Pass priority (choose 0)
whenever there is nothing worth doing right now."* For a strong player this is
correct nuance. For qwen it reads as "doing nothing is the safe default,"
and "cast nothing right now" becomes its reflex answer.

## FIX 1 (most important) — split "your turn" from "their turn"

Replace the "Act at the latest useful moment" bullet with wording that
distinguishes proactive development (do it on your turn) from reactive holding
(only for instant-speed cards on their turn). The weak pilot cannot make that
distinction unless it is stated explicitly.

Proposed replacement bullet:

> - Your turn is for ACTING; their turn is for reacting. On your own turn,
>   after playing a land, advance your plan: deploy a threat, cast an engine,
>   or remove a creature that is pressuring you. A spell sitting in your hand
>   does nothing — you only win by resolving your spells. "Waiting costs
>   nothing" is true ONLY for instant-speed cards (counters, combat tricks,
>   instant removal) — hold THOSE for the opponent's turn or the moment you
>   must use them. It is NOT true for your own development: passing your turn
>   with castable threats or needed answers in hand is a losing habit. Choose
>   0 to pass only when you genuinely have no play that advances your plan or
>   stops a real threat — never as a default.

Why this works for a weak model: it gives a clean binary (my turn = act,
their turn = the holding rule applies), names the ONE category that "wait"
belongs to (instants), and explicitly demotes "pass" from default to
last-resort. It keeps the real strategic content (hold true interaction) while
removing the misread.

## FIX 2 — tell the pilot to know and pursue its win condition

The pilot never deployed its finishers or card engine and durdled equal-life
games to a standstill. Add one short bullet (it generalizes: every deck has a
way it wins that a weak model won't infer on its own):

> - Know how your deck wins. Identify your win conditions and card-advantage
>   engines from your decklist, and once you are not about to die, DEPLOY them
>   — do not sit on a stalled, even board. A game you play to a passive
>   standstill is a game you fail to win.

Why: passivity has two failure modes — not answering threats (Fix 1) and not
advancing your own plan (Fix 2). Fix 1 alone still leaves a pilot that
stabilizes and then does nothing. This closes that.

## FIX 3 — light touch: match the answer to the threat

Deck140's pilot fired a 2-damage sweeper into a 4/4 (killing nothing that
mattered), then spent a second sweeper. Removal-selection detail belongs in
deck guides, but one general line helps every deck: append to the existing
combat/interaction guidance:

> Before spending removal or a sweeper, confirm it actually kills the
> creature(s) that matter; don't spend two cards to do one card's job.

This is a compression-friendly single clause, not a new bullet.

## Cuts / compressions to pay for the additions

- CUT or shrink the "Control the opponent through the information you show
  them" bullet (bluffing, representing instants with open mana, making plays
  for their effect on opponent belief). It is the least executable line in the
  guide for a weak model — multi-step theory-of-mind qwen cannot act on — and
  it actively REINFORCES passivity ("leave mana open to represent a trick" =
  another reason to do nothing). Removing it both frees space and removes a
  passivity nudge. If kept, cut it to a single clause: "Leaving mana untapped
  can represent an instant and deter an attack." Net: this cut roughly funds
  Fix 1's added length.
- The "Common errors to avoid" list already ends with "playing the wrong
  role... tapping out when you needed interaction... overextending into a
  sweeper." Add "passing your turn with useful spells in hand" and "never
  deploying your win condition" to that list as two short items — this
  reinforces Fixes 1-2 in the vocabulary the guide already uses, at ~12 words.

## Net effect on length

Fix 1 replaces one bullet with a similar-length bullet. Fix 2 adds one short
bullet, paid for by cutting/shrinking the information/bluffing bullet. Fix 3
and the error-list additions are clauses, not bullets. The guide stays about
the same size while removing its single most misleading line for a weak pilot
and adding the two behaviors the logs show it most lacks: act on your turn,
and deploy your win condition.

## What NOT to change

Keep the "decide who is the beatdown" and "two resources: cards and mana"
bullets — they are abstract but central, and the deck guides lean on them.
Keep the cost-vs-gain closing paragraph. Do not add removal-selection tables
or deck-type taxonomies to the general guide; that is deck-guide territory and
would bloat the shared prior.
