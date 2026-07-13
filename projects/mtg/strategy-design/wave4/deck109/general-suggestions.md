# General-layer (core prompt) proposals from the deck109 wave-4 review

Corpus: `matchups-20260712-192832` + `matchups-20260712-230636`, 12 games, 8/12, 0 timeouts.
Deck109 has the pool's best guide-adherence numbers (67/67 combats attacked, burn-to-face 35:4,
unleash 14/14, cast-nothing 7% of 174 casting asks), so this audit is primarily evidence of
which core lines are EARNING their keep — plus three concrete proposals. Each proposal states
layer (core / per-deck / engine) with reasoning. Citations: result+opponent+run, `sN` = seq in
the deck109 seat translog (epochs in wave4/deck109/findings.md).

---

## G1. [ADD — reply-protocol layer (code-appended), with a one-word suffix hardening] The reply-format contract, stated once with a RIGHT/WRONG pair

**Gap.** No text anywhere teaches the reply FORMAT beyond the per-decision suffix "Reply with
the number of your choice, then your PLAN: line." The model satisfies that sentence while
violating what the parser needs — it echoes the option line ("3. Cast Krenko's Command {1}{r}")
or writes prose containing digits before its number, and the parser grabs a stray digit from
the echoed text (`{2}{r}{r}`→2, `(3/3)`→3, "life 2"→2). 8 desyncs in deck109's 12 games alone,
one game-losing (L135 r1 s24: exact-lethal Pillar consumed onto a 1/1 instead of the face).
Every desync had a non-conforming reply head; every conforming `N\nPLAN:` reply parsed clean.

**Proposed text** (verbatim block):

> HOW TO REPLY. The FIRST CHARACTER of your reply must be the number of your choice, as a bare
> number on a line by itself. Do not repeat the option's text, its mana cost like {2}{r}{r}, or
> stats like (3/3) - the game reads the first number it sees, and extra text makes it take the
> WRONG action. Then write "PLAN:" and your short plan on the next line.
>   RIGHT:  2
>           PLAN: Cast Hellrider, then attack with everything.
>   WRONG:  2. Cast Hellrider {2}{r}{r} (3/3)
>   WRONG:  I choose to cast Hellrider. 2.
> For combat replies the same rule applies to the "A1, A3" / "B1:A2" forms: the list is the
> first thing in your reply.

**Where, and why there.** In the code-appended reply protocol (the interface section appended
after `{STRATEGY_GUIDE}`), NOT in each deck guide and not buried mid-prompt: (a) the contract
is a property of the harness — it must co-evolve with the parser, and the protocol text ships
from code, the same place the parser lives; (b) it is seat- and deck-symmetric — one copy
serves all seven decks and frees ~10 lines of every guide's budget (deck109's wave-4 guide
carries an interim copy, tagged for removal once this lands); (c) appended-after-guide is the
most attention-proximal static position to the decision. ALSO harden the per-decision suffix
by one word: "Reply with ONLY the number of your choice, then your PLAN: line." — the suffix
is the recency-position reminder; the protocol block is the teaching.

**Layer verdict:** protocol/core. The parser fix itself (first standalone integer at reply
head, validate in [1, options], never scan prose) is ENGINE and is filed separately; this text
shrinks the failure surface regardless and remains correct after the parser is fixed.

## G2. [NO CHANGE — validated, freeze it] The mana representation + affordability block is done

**Current text.** "…your untapped lands ARE your available mana." / "Every listed choice is
legal AND PAYABLE right now… Never decline a listed action because you believe… you cannot pay
for it."

**Audit result.** Wave-3's #1 leak is gone at this layer: 0 genuine "my lands are tapped from
attacking" delusions across 174 casting asks (surface matches on a scan are all correct
reasoning about the OPPONENT's untapped blockers or true 1-mana shortfalls); no false declines
of listed actions in 12 games; cast-nothing rate 7%. The `Mana available: {r}{r} from N
untapped sources (tapped automatically when you cast)` situation line plus these sentences
closed it together. **Proposal: none.** Do not add more mana prose — the battle is won and
every added line taxes all seats. (Per-deck guides can now compress their own mana blocks to a
pointer at the `Mana available:` line, as deck109's wave-4 guide does.)

## G3. [ADD — core, INTERIM, remove after engine fix] The cast-happened verification anchor

**Gap.** The engine can silently no-op a chosen cast (deck109: "Cast Hellrider" consumed with
no effect 16x in one loss, 43x in another — mana unspent, card still in hand, phase advanced;
the same action resolved normally in a win, so it is board-state-conditional). The pilot has no
instruction for noticing this, so it re-picks the same action all game — the core's own
(correct!) "deploy your win conditions / a plan deferred every turn is a pass" priors actively
feed the loop.

**Proposed text** (one sentence pair, in the interface section near "The lists are also
complete"):

> A cast that really happened shows in the GAME LOG as the card moving "hand -> stack". If your
> last choice was a cast and the card is STILL in your hand with your mana unspent, the cast
> did not happen: spend this decision on a DIFFERENT castable card and try the stuck one again
> next turn - repeating the identical pick in the same turn changes nothing.

**Layer verdict:** core, INTERIM. Argument for core over per-deck: the defect is a harness
property, seat-symmetric, and only OBSERVED on Hellrider — nothing says it is Hellrider-only,
and per-deck placement protects exactly one deck against a deck-agnostic failure mode. Argument
for interim: this is mitigation for an open engine bug (filed with A/B repro: L131 r1 s27 no-op
vs W110 r1 s17 success); once the engine fix lands, remove the lines — they cost every seat two
sentences and the `hand -> stack` check becomes dead weight. Tag it in the prompt file with a
comment if the format allows, otherwise in the revision notes.

## G4. [AUDIT — existing core lines, followed / misread / dead-weight, from the pool's most obedient seat]

**Earning their keep (followed, with counts — do not weaken):**
- **"be the aggressor by default… attack every turn with everything"** — 67/67 attacker
  decisions declared attackers, zero "none" (all 12 games). The free-case sentence ("if the
  opponent has NO untapped creatures, attack with every creature") is visibly echoed in plans
  (e.g. W133 r1 s14-16, L135 r1 s25).
- **"Direct damage and burn are part of your CLOCK… send them at the face"** — resolved burn
  targets 35 face : 4 enemy-creature. The wave-3 hold-instants misfire is gone.
- **"summoning sickness is never a reason to keep a creature in your hand"** — fixed; the model
  now casts and annotates "it will attack next turn" (L131 r1 s5, L133 r2 s13).
- **Lethal check "TWICE every turn"** — fires: exact burn lethal computed at L135 r1 s23
  (executed s23, lost only to the s24 parse desync), alpha-strike lethal at W110 r1 s20.
- **"Your last stated plan… may be STALE or WRONG: trust the board"** — observed working as
  designed: self-corrections mid-plan ("Wait, I misread the board state. My Hellrider is in my
  hand, not on the battlefield" — L131 r1 s40). Wave-3's stale-plan spiral did not recur.
- **"a cost is often just HOW A CARD WORKS… Do not refuse a cost"** — all 7 forced-Sacrifice
  menus (Cruel Edict / Phyrexian Obliterator) picked the right own-side permanent (Goblins and
  Mountains sacrificed, Boros Reckoner kept, every time: L140 r1 s6/s10/s17, L133 r2 s20/s21,
  W133 r1 s23, W140 r2 s51).
- **"never point your own damage… options marked [your battlefield]"** — intent-level obedience
  is near-total: of 9 own-side target picks in 12 games, 7 were forced sacrifices (correct), 1
  was a parser desync (not the model), leaving ONE genuine slip (below).
- **"your chosen number must EXECUTE your own plan"** — the one genuine violation: W140 r2 s21,
  a "Choose the target for Boros Reckoner" bounce menu where the plan said "dealing 2 damage to
  the opponent" (option 1) but the reply led with "3" (its own Reckoner — apparently read as
  "select the Reckoner's ability"). One instance; the existing sentence covers it; no text
  change proposed. Per-deck note filed for Boros Reckoner decks instead (name the bounce menu:
  "when the menu says 'Choose the target for Boros Reckoner', pick the OPPONENT").
- **Mulligan paragraph** — followed in all observed keep decisions; no bad keeps in 12 games.

**Misread (engine-shaped, not text-shaped):** opponent-turn forced menus arrive with no
attribution — "Choose the target for Sacrifice" during the opponent's main phase caused one
confused reply about a "critical phase mismatch" (L140 r1 s6, reply had no leading number).
The model cannot tell WHO is forcing the sacrifice or WHY. **Engine ask, not prompt text:**
include the forcing card in the menu header ("Choose the target for Sacrifice (opponent's
Cruel Edict)") — representation, one string, kills the confusion at the source. Until then no
core-prompt sentence can fix it (the prompt cannot know the forcing card either).

**Dead-weight: none provable from this seat.** Candidates like the X-cost sentence and the
fetchland-sacrifice example were simply never exercised by deck109 (no X spells, no fetches) —
absence of evidence from a mono-Mountain aggro seat, not evidence of waste. Defer dead-weight
cuts to seats whose decks exercise those lines.

---

**Merge note for the synthesis agent:** G1 will likely be proposed by several decks (the desync
mechanism is seat-independent). Merge to ONE protocol block + the one-word suffix change, and
strip the interim copies from per-deck guides (deck109's wave-4 guide flags its own copy for
this removal).
