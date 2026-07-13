# General (core-prompt) suggestions from the deck-110 review (wave 4)

Evidence base: deck110's 12 games / 487 decisions across `matchups-20260712-192832`
and `matchups-20260712-230636` (record 9-2-1t, best in pool — so this seat's evidence
is strongest on what WORKS; its two losses and one timeout supply the leak evidence).
Citations: `<epoch>` + seq. Each item states core-vs-per-deck-vs-engine explicitly.
Live core prompt read in full before proposing (`bin/Res/ai/gpt/system_prompt.txt`).

---

## 1. The mana/affordability teaching: KEEP ALL THREE LAYERS THIS WAVE — no cut yet, compression scheduled

**Current lines.** The contract lives at three layers: the core prompt's rules
paragraph ("the game taps your lands AUTOMATICALLY ... 'Mana in your pool: (none)' is
the normal state"), the core closing block ("Every listed choice is legal AND PAYABLE
right now ..."), and the board representation (`Mana available: {u}{r}{r} from N
untapped sources (tapped automatically when you cast)`).

**Audit result.** Wave-3's corpus-killer is dead: cast-nothing-from-mana-delusion fell
from 44-88% of casting decisions per game to 7 borderline cases in 487 decisions — and
several of those 7 are CORRECT declines (e.g. `1783903101` seq26 refuses the {B}{B}
attach line with no black source; ability lines, unlike the cast list, can carry
colors the deck cannot pay — the model handled it right). I found zero instances of a
listed CAST declined for a false mana reason.

**Proposal: change NOTHING this wave.** The turnaround is one wave old and this
teaching is its engine; redundancy across layers is cheap insurance against a
regression we cannot yet rule out. Schedule for wave 5: if the delusion rate stays at
floor corpus-wide, compress the closing-block sentence ("Never decline a listed action
because you believe it is illegal or that you cannot pay for it; if you truly could
not, it would not be listed" carries the whole load) and trim the rules-paragraph
duplication. **Classification: core + representation, both already shipped — this item
is a propagation verdict, not a change request.** Per-deck guides can now drop their
long mana blocks down to the one quote-and-forbid line (mine did).

## 2. Blocking-while-racing: replace "dangerously low" with a runnable numeric check

**Current line (combat paragraph):**
> "When blocking: block when the trade is favorable or free, and chump to stay alive
> when an unblocked attack could kill you or take you dangerously low; take the damage
> when your blockers are worth more alive than the life saved."

**Observed failure.** Both deck110 losses are lost races vs deck109 (fast red aggro),
and the r2 loss dies exactly in this line's gap: `1783920257` seq39-40, T9, my life 8,
four attackers (6+ power) incoming, an untapped Steel Overseer available — the model
passes, reasoning "Steel Overseer is too weak to trade favorably without dying" (the
line's own trade-quality framing), takes 8→2, dead next turn. "Dangerously low" and
"worth more alive" are judgment words; the executor resolved them with a trade-value
heuristic at the moment survival math should have owned the decision.

**Proposed replacement:**
> "When blocking, run the survival check FIRST: add up the unblocked attackers' power;
> if taking it all would put you at 8 or less, block to cut the total — chump with any
> creature you do not need to win on your own next turn; a dead blocker that keeps
> your life above the opponent's reach is a good trade. Only when you would stay above
> 8 do you choose blocks by trade quality: block when the trade is favorable or free,
> and take the damage when your blockers are worth more alive than the life saved."

**Why.** Sequences the two decision modes (survive, then value) instead of listing
them as parallel clauses, and keys the mode switch to a number the executor can
compute from the board. The 8 threshold matches the prompt's existing burn-at-face
threshold — one number, reused, is easier for a weak executor than two. This aligns
with deck131's runnable-checks rewrite of the same line if that lands — merge the two
rather than shipping both. **Classification: CORE — being-raced is deck-agnostic and
symmetric (every seat faces it); my per-deck race section keeps only the deck-specific
half (go wider than the red deck, named threats).**

## 3. A being-raced posture floor: SMALL core addition, the rest is per-deck

**Gap.** The core prompt's role paragraph covers "decide who is the beatdown" and
says the race-loser must be the aggressor — but nothing tells the CURRENT aggressor
what to do when the opponent races BACK and their clock is faster. deck110's losses
are the aggressor losing a race it kept playing as the unopposed beatdown (narrow
deployment, all-out attacks, no blocks). deck131/135 reviewers report the adjacent
passivity failures.

**Proposed addition (one sentence, in the beatdown paragraph, after "This role can
flip mid-game."):**
> "Re-check the race every turn: if the OPPONENT's clock is now faster — they kill you
> in fewer turns than you kill them — your attacks stay but your spare creatures
> switch to blocking, and 'spare' means not needed for your own lethal."

**Why.** The observed failure is not knowing the role flipped from "unopposed
beatdown" to "racing from behind" (`1783920257`: opponent at 5 but hitting for 6+/turn
while deck110 held one attacker). One sentence names the re-check and the concrete
behavioral change. The rest of what fixed my deck (deploy WIDE before equipping, named
enemy threats) is deck-specific knowledge — **classification: this sentence CORE;
deployment-width and matchup triggers PER-DECK.** If the merger finds #2's survival
check sufficient, drop this item first — #2 fires at the seam where my losses actually
happened (blocking), #3 is the upstream posture.

## 4. Repeated-activation waste: the core line exists and LOST — strengthen with the perception cue; the real fix is representation/engine

**Current line (activation paragraph):**
> "...never repeat an activation whose effect is already in place (if the equipment is
> already attached, the creature's displayed power already shows the bonus; equipping
> it again gains nothing)."

**Observed failure.** This exact line was live in all 12 games and deck110 still made
27 re-equips onto already-equipped creatures (9 of them with replies echoing the
line's own vocabulary — "to ensure the bonus" — while violating it). Worst:
`1783903101` T19 and T23, five consecutive equips each, the board printing
`Etched Champion (6/2) (printed 2/2)` the whole time. The line states the fact but
gives no perception procedure, and the engine re-offers the no-op line every window.

**Proposed replacement (same sentence, made runnable):**
> "...and take the SAME activation line at most once per turn unless each use visibly
> adds something new (a counter, a token, mana you spend this turn). Before any Equip
> or attach line, read the target's stats: TWO power numbers — like '(6/2) (printed
> 2/2)' — mean a bonus is already on it and the equipment is almost certainly attached;
> equipping the same creature again gains nothing."

**Why.** Converts fact into procedure (read the stat block) plus a countable brake
(once per turn) whose exception covers legitimately-repeatable activations
(firebreathing, token engines). The `(printed X/Y)` tag is a general representation
feature, so the cue is deck-agnostic. **Classification: CORE for this strengthened
line, but the primary fix is ENGINE/REPRESENTATION, in order of preference: (a) board
line shows attachments explicitly — `Memnite (5/1) (printed 1/1) [equipped: Cranial
Plating]` and `Cranial Plating [attached to Memnite]` — which kills the whole failure
class for every equipment deck at the source; (b) the legal-actions list suppresses or
annotates re-offered no-op lines ("already attached"). Wave-3 flagged (b); two waves
later it is still the top representation gap. A caveat for (a)-style cues: lords and
counters also produce the (printed) tag, so the attachment marker is strictly better
than the power-delta cue — ship the marker, then simplify the prompt line to point at
it.**

## 5. Line-by-line audit across 12 games — what is FOLLOWED (do not touch) and residual noise

**Followed, with direct evidence (protect these in any merge):**
- Lethal check run in both windows: clean wins close with double Galvanic Blast to the
  face then exact-lethal attacks (`1783921985` T7: 11→7→3→dead; `1783915940` T5/T7).
- "Once the opponent is at 8 or less always fire damage at their face": all 14 Blast
  casts audited; face-targets throughout, the few creature targets are correct blocker
  removal (`1783905283` T7, a 3/3 flier stopping the equipped attacker).
- Act-floor + closed "Cast nothing" exception: cast-nothing collapsed corpus-wide for
  this seat; games end T7-13 by kill instead of the 40-min cap.
- Plan/number binding + stale-plan grounding: zero mismatches in 487 decisions
  (wave 3: recurring). The "if an action is still offered, it has NOT been done" line
  did NOT generalize to the equip loop, though — see #4 for why (the equip line being
  re-offered is the engine re-offering a DONE action; the grounding line teaches the
  opposite inference for it, a small contradiction the attachment marker would erase).
- Phyrexian/hybrid cost teaching: 16 Vault Skirge casts, no cost confusion.
- Mulligan default: no bad keeps or verbal-reason mulls observed.

**Misread/lost:** the two lines rewritten in #2 and #4 (judgment-word blocking;
fact-without-procedure activation brake).

**Dead weight (from this seat's evidence only — weak signal, one deck):** none I can
prove; deck110 exercises no counterspells, sweepers, or fetches, so I mark those lines
untested-not-dead. Defer any cuts to seats whose decks exercise them.

**Engine/representation flags (not core-prompt text), consolidated:**
- Attachment markers on board lines (#4a) — highest-value single change I can name.
- No-op activation suppression/annotation in the legal list (#4b, wave-3 carryover).
- Mana-ability activations surface as `Cast Card Normally with Mox Opal` / `Play Land
  with Mox Opal` (`1783920257` seq4) — mislabeled; should read like "Tap Mox Opal for
  mana". Until fixed, per-deck defusal lines carry it (mine does, flagged removable).
