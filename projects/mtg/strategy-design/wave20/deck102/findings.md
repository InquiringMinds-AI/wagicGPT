# deck102 (Tergrid, discard/sacrifice) — findings (wave 20, GUIDELESS FIRST PASS / Step 0-bis)

Corpus: `matchups-20260719-151849`, binary `95e89c204`, 6 games round-robin (vs 35/14/62/27/135/49).
Record **1/6** — the only "win" was an ADJUDICATION over deck27 at the turn cap (opp at 3 life);
the other five were losses, all by getting RACED to 0 (final my_life: -2, 0 vs62, -4 vs135, -1 vs49;
vs14 ground to a stalled near-loss). 6 fallbacks. This is the expected guideless baseline — the value
here is the qwen×core interaction with no guide confound.

Deck is a **100-card singleton** (99 listed cards + Tergrid shuffled into the library; libraries start
at 93 = 100-7, life 20 = classic highlander rules, NOT 40-life commander). Every card effect below was
read from the engine `text=`/`auto=` (primitives), cross-checked against the translog option lines.

---

## HEADLINE 1 - engine-leverage answer: NO. Tergrid never came online in any game.

**Tergrid, God of Fright appears in ZERO prompts, ZERO board states, ZERO events across all 6 games.**
Never cast, never offered, never drawn. Cause is **variance, not an engine gap**: Tergrid is a single
copy in a 100-card singleton; ~15 cards seen/game over 6 games gives P(never drawn) ~ (0.85)^6 ~ 38% -
an unlucky-but-ordinary whiff. (Verified in the LIBRARY, not a stranded command zone: library 93 at
turn 1 = 100-7, and the deck lists 99 non-commander cards, so the 100th is Tergrid shuffled in.)

Tergrid's real engine (`text=`): *"Whenever an opponent sacrifices a nontoken permanent or discards a
permanent card, you may put that card onto the battlefield under your control."* 4/4 menace, {3}{B}{B}.
The `auto=` excludes instants/sorceries from the discard trigger, so it steals discarded/sac'd
**permanents** (creatures, artifacts, enchantments, planeswalkers, lands) only.

**Consequence - the deciding structural fact of this corpus:** with the payoff absent, the model fired
the deck's ENABLERS (Hymn to Tourach, Thoughtseize, Smallpox, Pox, Oppression, Dreadhorde Invasion,
Liliana's Triumph) as raw disruption **with no payoff to convert them** - often paying SYMMETRIC costs
(Headline 2). The deck degenerated into a slow, clockless attrition pile and got raced. The guide cannot
make the model draw Tergrid, but it can (a) make the model prioritize+protect it the moment it IS drawn,
and (b) make the deck functional in the ~62% of games Tergrid never shows.

**Closest the model came to the theme:** vs27 s12 cast **Animate Dead targeting Glen Elendra Liege in the
OPPONENT's graveyard** - the exact creature its own Thoughtseize (s5-s7) had put there. A poor-man's-
Tergrid steal via reanimation. That game (Tourach + reanimated Liege + Sword of Feast and Famine,
attacking s14/s19/s27) was the ONLY competitive one -> the adjudication "win." The lesson is loud:
**when the model deploys a threat and attacks, this deck is fine; when it durdles on symmetric
disruption, it loses.**

---

## HEADLINE 2 - top self-inflicted loss class: SYMMETRIC mass-effects fired while behind.

Pox and Smallpox are **symmetric** (`auto=life:-... controller` AND `... opponent`; each player loses
life, discards, and sacrifices creatures/lands). Without Tergrid (to steal the opponent's half) or a
board lead, they are mutual destruction, and the model treated them as free one-sided disruption:

- **vs35 s18 t9:** `Cast Pox {b}{b}{b} {right now: life -7, life -3}` at **L8/19**. Paid life it could
  not afford while already behind 8-to-19, sacrificed its own lands/creatures, died turn 10.
- **vs49 s11 t7:** `Cast Pox {b}{b}{b} {right now: life -7, life -2}` at **L6/20** - at 6 life, against a
  mono-red DRAGON aggro deck, near-empty board. Symmetric self-immolation into an aggressor; dead turn 10.
- **vs62 s7 / vs35 s8 / vs14 s24:** Smallpox at parity/behind - each player loses 1, discards, sacs a
  creature AND a land. On the draw vs tempo/aggro, trading your land+creature+1 life symmetrically only
  accelerates the race the model is already losing.

The option line RENDERS the self-cost (`{right now: life -7, ...}`) and the model cast anyway - so this
is not a representation gap, it is a missing deck-fact: **these are payoff-cards without the payoff, and
self-harm while behind.** They belong to Tergrid, a board lead, or an empty-hand reset - never to "I have
nothing else to do, I'll disrupt."

---

## HEADLINE 3 - DURDLE / no clock is the losing spine (attack-floor + deploy-floor failures).

The deck HAS real threats/reach (Archon of Cruelty 6/6 flyer bomb, Sheoldred 6/6, Butcher of Malakir 5/4
flyer, Steel Hellkite 5/5 flyer, Phyrexian Obliterator, Korlash [=swamps], Gray Merchant [devotion
drain], Corrupt [X=swamps, any target], Syr Konrad [death/discard pinger], Liliana Dreadhorde General).
In the 5 losses the model deployed cheap disruption + rocks + 1/1s, **never landed a real threat, and
never built a meaningful clock**, while opponent life sat near 19-20:

- vs62: opponent stayed at **19 the entire game** (model 20->0). Leaden Myr, 3x Dreadhorde Invasion,
  Smallpox, Liliana's Triumph, Mind Stone - zero pressure. Dead turn 11.
- vs35: Tinybones (1/2), Sol Ring, Smallpox, Hymn, Liliana of the Dark Realms (used -3 to SHRINK a
  blocker, not to ramp toward a threat), Pox. No beater, no attack. Dead turn 10.
- vs49: Elvish Doomsayer (1/1), Hymn, Pox, Arcane Signet. Dead turn 10.
- **vs135 s22/s23 t9 at L14/13 (parity):** `Cast nothing right now` then **`no attackers`** with Tourach,
  Crypt Ghast, Leaden Myr available (2x Witch were summoning-sick). Passive at parity -> raced 14->3->dead
  over two turns. A textbook attack-floor miss.

The core carries a general aggression floor; it did NOT fire because the model does not know THIS deck HAS
a clock - it reads the deck as pure disruption. The guide must name the finishers and state that
disruption is not damage.

---

## DISCARD-PICK REVEAL SURFACE (owned this wave, first review ever): mechanics SOUND, prompt MISLEADS, quality OK.

The `reveal` kind is **Thoughtseize** (`text=Target player reveals their hand. You choose a nonland card
from it. That player discards that card. You lose 2 life.`). All 3 reveal records are Thoughtseize
resolving. **Prompt shape (record):** a "TARGET CHOICE for Thoughtseize" ask (pick the player) resolves
first; then a `reveal` record framed as *"Reveal: you looked at the top N cards of your library. Decide
... which go to 'choose discards'; every card you do NOT pick goes to 'look'."* with an ELIGIBILITY block
tagging each card `[eligible for "choose discards"]` (nonland) or `[does NOT qualify - goes to "look"]`
(land), full rules text per card, and a `PUT: n, m` reply protocol.

### Per-record judgment

| vs  | revealed hand                                             | model PUT           | discarded (events)                                             | pick quality |
|-----|----------------------------------------------------------|---------------------|----------------------------------------------------------------|--------------|
| 14  | Boomerang, Unsummon, Island, Boomerang, Island           | 1,4 (both Boomerang)| ONLY ONE Boomerang -> graveyard; other Boomerang+Unsummon kept | acceptable (Boomerang is a fine strip; over-selected) |
| 27  | Glen Elendra Liege, Zombie Outlander, Walking Dead, Swamp | 1 (Liege)           | Glen Elendra Liege -> graveyard                                | **optimal** - the anthem-lord flyer, single clean pick |
| 135 | 2x Prismatic Vista, Arcum's Astrolabe, Misty Rainforest, Ice-Fang Coatl, Into the North | 2,4,6 | ONLY Arcum's Astrolabe -> graveyard; Coatl+Into the North kept | arguable - Ice-Fang Coatl (flash flyer/draw/deathtouch) is the real threat; over-selected |

### Mechanical integrity - CLEAN (ENGINE-R1 same-tick finalize works here)

The pick LANDS: in all 3, the model's **lowest-index selected eligible card** went to graveyard (hand ->
graveyard fires exactly once), the rest return to hand. No wrong-zone drop, no zombie, no optiontwo-sweep.
Thoughtseize's `auto=` carries `target(<1>*[-land]|reveal)` - the `<1>` **enforces pick-exactly-ONE**,
which is correct Oracle behavior. So the engine discarding one card is RIGHT; the model's over-selection
(2 of 3 records) is silently truncated to the first eligible card.

### The DEFECT is REPRESENTATION, not resolution (-> notes.md)

The prompt misrepresents a pick-ONE effect two ways: (1) it calls the **opponent's revealed HAND** "the
top N cards of your library" - false; (2) the "which go to 'choose discards' (subset)" framing implies
MULTIPLE cards can be discarded, driving over-selection. Over-selection is harmless ONLY when the true
best card is the lowest eligible index (all 3 happened to work) - but it is a **latent quality bug**: if
the pilot's true #1 is not the lowest index, over-picking discards the WRONG card (the pilot loses the
choice the card exists to give). Durable fix is the prompt; the guide teaches the pick-ONE fact interim.

**Tergrid interaction to teach:** a discarded PERMANENT (vs27 Glen Elendra Liege, vs135 Arcum's Astrolabe)
would be **STOLEN onto your battlefield if Tergrid were out** - so with Tergrid, Thoughtseize a PERMANENT
(not an instant/sorcery), because you keep it.

---

## THE 6 FALLBACKS - all one class: the losing-position GIANT-REPLY spiral.

| seq | game  | kind     | fallback   | turn / life        | reply len |
|-----|-------|----------|------------|--------------------|-----------|
| 31  | vs14  | ask      | unparsed   | t13 L13/15         | 707 (after s30 priority rl14538 pass) |
| 33  | vs14  | priority | unparsed   | t13 L13/15         | **14332** |
| 39  | vs14  | priority | stale_echo | t15 L11/13         | 7002 |
| 15  | vs62  | ask      | unparsed   | t7 L7/19 (raced)   | **15358** |
| 24  | vs27  | ask      | unparsed   | t9 L10/8 (prison)  | **13917** |
| 26  | vs135 | blockers | unparsed   | t12 L3/9 (lethal)  | **14082** |

Every fallback is a **>13k-char truncation** (5 unparsed + 1 stale_echo) in a **behind / stalled /
complex-board** spot - the model re-derives the whole board hunting impossible lethal or survival, blows
the token budget, and the baka heuristic answers. This is the wave-18 LOSING-POSITION OFF-CASE SPIRAL.
Two deck-specific amplifiers: (1) this deck's own prison pieces (No Mercy, Painful Quandary, Oppression,
Megrim, Necrogen Mists) build LARGE stalled boards -> longer derivations; (2) vs135 s26 is the COMBAT face
(blocker spiral at L3 facing lethal; `chosen_text` empty, baka fell back and blocked). Fix is a GUIDE
off-case BEHAVIOR branch (behind & cannot win/stabilize this turn -> best single play + pass; do NOT
re-derive the whole board), NOT a token-budget raise (that makes the spiral longer). Beyond the fallbacks,
benign giants clustered at the same seams (vs27 s16 13614, s22 9189, s23 14340; vs14 s30 14538) - the tax
gauge is giant-reply COUNT and it localizes to the behind/stalled off-case.

## Secondary / watch items

- **Repeated cast of a singleton:** vs135 "Cast Witch of the Moors" chosen 3x (s20/s21/s24) but only ONE
  `Witch: stack -> battlefield` all game; vs62 "Cast Dreadhorde Invasion" 3x (s5/s6/s16) with no clean
  battlefield event in narration. In a singleton a card casts once - this is a re-offer / non-resolving-
  cast anomaly worth an engine probe (-> notes.md, low confidence).
- **Liliana of the Dark Realms -3** (vs35 s14/s15) used to -X/-X a blocker - defensible removal, but her
  swamp-ramp toward the deck's swamp-matters payoffs (Korlash, Crypt Ghast, Corrupt) was never pursued;
  a symptom of the no-clock durdle, not a distinct bug.
