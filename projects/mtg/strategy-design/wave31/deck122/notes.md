# deck122 -- wave 31 engine/harness notes (seq repros)

Corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260727-025026`, binary cf7cb2e53.
Seat logs: `*ai_baka_deck122-*.jsonl`; stderr `game-122v*.stderr`, `game-1{37,39,46,48}v122-*.stderr`.

---

## N-122c (TOP PRIORITY, CROSS-SEAT) -- the attacker tapped-tag renders a self-contradiction, and it LOST a game

**Render string:** every declared attacker is printed as
`<Name> (P/T) ... [tapped - cannot attack or block this turn] [attacking]`.

This is the wave-30 **g4 tapped-tag reword** (validated in wave 30 on non-combat tapped
creatures, "read correctly 131x at deck137"). On the DECLARED-ATTACKER path the same
wording asserts the opposite of the adjacent tag: an attacking creature is tapped by
attacking, so the reworded text tells the model the attacker "cannot attack this turn"
while the next tag says `[attacking]`. Method headline 2 exactly: the fix is PATH-SCOPED
and the untested sibling path inverts it.

**Scale:** the exact string `[tapped - cannot attack or block this turn] [attacking]`
occurs **331 times across the 42 corpus logs** (deck122-vs-137: 49; deck146-vs-122: 30;
deck122-vs-158: 22; deck152-vs-122: 20; deck122-vs-148: 18; deck122-vs-152: 16; ...).
**Four separate seats' models verbalized the paradox in-reply**:
`1785152439-ai_baka_deck122-...-vs-ai_baka_deck158.jsonl`,
`1785138633-ai_baka_deck152-...-vs-ai_baka_deck122.jsonl`,
`1785152316-ai_baka_deck152-...-vs-ai_baka_deck158.jsonl`,
`1785148242-ai_baka_deck137-...-vs-ai_baka_deck158.jsonl`.

**REPRO (game-losing):** `1785152439-ai_baka_deck122-0x55b7f0340740-vs-ai_baka_deck158.jsonl`,
**seq39, turn 13**, kind=blockers, life 8 vs 7, options=4.
Prompt attacker line: `A1. Orc army (19/19) deals 19`; board line for the same creature:
`Orc army (19/19) (printed 0/0) [counters: 19x +1/+1] [tapped - cannot attack or block this turn] [attacking]`.
Reply (11,498 chars) verbatim:
- "Wait, the log says 'tapped - cannot attack or block this turn' but also '[attacking]'.
  In Magic, a tapped creature cannot attack. If it is attacking, it must be untapped. ...
  This is a contradiction in the log description."
- "Given the explicit warning 'tapped - cannot attack', I will assume it does NOT attack.
  ... So, BLOCKS: B1:A2, B2:A3. This leaves A1 unblocked, but since it's tapped, it deals
  0 damage."

The answer-first line had been the CORRECT `BLOCKS: B1:A2, B2:A3, B3:A1, B4:A1`; the
paradox-driven self-correction dropped both chump blockers off the 19/19.
**Realized damage** 19 (Army) + 2 (Mauhur; the lone Soul Warden block is illegal vs menace
and was correctly refused at damage time) + 3 (Olog-hai trample over the 1/1) = 24;
8 - 24 = **-16** = the recorded `WAGIC_SELFPLAY_RESULT winner=1 life0=-16 life1=7 turn=13`.
Either 0/1 on the Army absorbs all 19 and the seat survives at 4+ with the opponent at 7.
stderr confirms only two toggles were applied: `Defenser Toggle: Soul Warden` /
`Defenser Toggle: Suture Priest`, then
`AIPlayerGPT: declared blocks from 2 assignment(s) in one reply`
(`game-122v158-1785152435.stderr:1100`).

**Fix direction:** never emit the "cannot attack or block this turn" clause on a creature
that is a declared attacker or blocker. Suggested: for combat participants render
`[tapped - attacking]` / `[tapped - blocking]`, and reserve
`[tapped - cannot attack or block this turn]` for tapped non-combatants only. Verify by
before/after render quote on an attacking token PLUS a block decision that correctly
chumps a large attacker (a representation-ledger item closes only on both, per method
headline 3).

---

## N-122d -- `truncated_abandoned` at the blockers seam DISCARDS a valid answer-first line and defaults to no-blocks

Both of this seat's fallbacks (2 of the corpus's 6) are the same shape, and in both the
model had ALREADY complied with the answer-first protocol before the token budget ran out.

**Repro A:** `1785147991-ai_baka_deck122-0x562bf3cb8160-vs-ai_baka_deck137.jsonl`
**seq22, turn 10**, kind=blockers, options=3, latency 210,738 ms, reply 11,611 chars,
`fallback=truncated_abandoned`.
Reply line 1: `BLOCKS: B1:A3, B2:A4`. Truncated tail re-derives the same pick ("Finks
blocks Emmara ... Pridemate blocks Flaxen ... THREE creatures die. BA triggers 3 times.
Net Life: 19 - 2 + 3 = 20"). Harness took `no blockers`
(`game-137v122-1785147987.stderr:1339`:
`AIPlayerGPT: truncated-abandoned block commit -> safe no-blocks default`).
Cost: 19 -> 14 life, 3 Blood Artist drains forgone; game still won by adjudication 16-12.

**Repro B:** `1785152439-ai_baka_deck122-0x55b7f0340740-vs-ai_baka_deck158.jsonl`
**seq26, turn 9**, kind=blockers, options=4, latency 141,086 ms, reply 11,266 chars,
`fallback=truncated_abandoned`.
Reply line 1: `BLOCKS: B3:A2, B1:A1, B2:A3`. Tail: "the plan is to block all three
attackers with Soul Warden, Kitchen Finks, and Suture Priest respectively", computed net
-4. Harness took `no blockers` (`game-122v158-1785152435.stderr:790`).
Cost: 13 damage (35 -> 22) instead of the model's -4, and the opponent stayed at 14
instead of 11, three turns before the -16 loss.

**Two separable defects:**
1. **The answer-first line is not salvaged on truncation.** The whole point of the
   answer-first protocol is that a legal answer survives a long deliberation. When the
   reply is truncated, the head `BLOCKS:` line should be honored if it parses to a legal
   assignment; the current path abandons the reply wholesale. (Guard: if the truncated
   tail contains a LATER, different, complete assignment, the existing line-anchored
   self-correction rules apply; here head and tail AGREED in both cases, so a
   "head-line-if-parseable-on-truncation" rule is strictly safe on this evidence.)
2. **The `no-blocks` default is the worst-case default at the blockers seam.** Declining
   all blocks maximizes incoming damage, which for a lifegain/control seat is maximally
   damaging and is also the adjudication tiebreaker. Prefer falling back to the Baka
   heuristic's own block assignment over a blanket decline.

Note the tail is not a hard character cap: 13,283-char (vs146 seq?) and 13,689-char
(vs148) replies at this same seat parsed fine. It is the token budget landing
mid-deliberation on the longest multi-attacker enumerations (latency 141-211s).

---

## N-122b -- no casting-decision ask offered while payable spells sat in hand (2 consecutive turns)

`1785147562-ai_baka_deck122-0x55e932d7a320-vs-ai_baka_deck148.jsonl` +
`game-148v122-1785147558.stderr`.

- **Turn 11 (deck122's turn):** the ONLY deck122 event in stderr is
  `AIPlayerGPT: declared attack (0 of 1) in one reply` (line 729). No land ask (correct --
  no land in hand) and **no casting decision**. seq16 (t11 attackers) prompt shows
  `Mana available: {b}{w} from 3 untapped sources`, battlefield `Isolated Chapel;
  Scrubland; Plains`, hand `Vampire Nighthawk {1}{b}{b}; Blood Artist {1}{b}; Vampire
  Nighthawk {1}{b}{b}; Divinity of Pride`. `Blood Artist {1}{b}` is trivially payable
  (Chapel or Scrubland for {B} + either other land); `Vampire Nighthawk {1}{b}{b}` is
  payable as Plains->{1}, Chapel->{B}, Scrubland->{B}.
- **Turn 13:** only `AIPlayerGPT: Land drop: play Plains now? -> chose 1 of 2` (seq18),
  then Turn 14 and the loss at -8. Same hand, now with a 4th land. No casting decision.
- Contrast: the same seat WAS offered Blood Artist in other games (vs158 seq19 t8, seq28
  t10 both cast it), so the card is offerable in general -- the gap is state-specific.
- Also visible at every land drop in this stderr:
  `AIPlayerBaka: Card was a land and ai cant play any more lands this turn.` immediately
  after the GPT land pick, i.e. the land-drop pick is re-attempted after the drop is
  already consumed. Probably benign logging noise, but it sits on the same code path and
  is worth checking while investigating the missing cast ask.
- **Mechanism unconfirmed** (candidate: `AIPlayerGPT::FindCardToPlay` candidate
  validation rejecting payable casts before the ask is built -- the same family as convoke
  divergence C, which manifests as a silent defer rather than a missing ask). Reported as
  OBSERVED with repro, not diagnosed.
- Consequence: at 8 and then 2 life with a 2/3 deathtouch/lifelink blocker in hand, the
  seat was never asked. Part of the vs148 loss is engine-lane.

---

## N-122a -- Felidar Sovereign alt-win probe: DISPOSITION = between-wave probe (does NOT hold the seat)

- Ability is well-formed in `bin/Res/sets/primitives/mtg.txt`:
  `name=Felidar Sovereign` / `auto=@each my upkeep:this(controllerlife > 39) wingame`.
- **Never on the battlefield in any of the 6 wave-31 games.** Present in hand exactly once
  (vs146 opening seven) and bottomed at the London mulligan (seq3, `PUT: 2`).
- 40+ life reached WITHOUT it again: vs152 crossed 40 at seq19 (t9) and finished at **63**,
  passing five of its own upkeeps at 40+. The win-check therefore had zero opportunities
  to fire, corpus-wide, for the SECOND consecutive corpus.
- **Conclusion: the alt-win firing remains LIVE-UNVALIDATED and is a stacked-probe job
  (deck199-style: Felidar + gainers, no other win route).** Two corpora of random pool
  draws are sufficient evidence that the pool cannot co-present card + threshold. Carry as
  a between-wave engine task; it is not a reason to hold a rotation slot.

---

## Minor / cosmetic

- **Clue token annotation garbled + mis-sourced.**
  `1785138633-ai_baka_deck122-...-vs-ai_baka_deck152.jsonl` seq27 (t11) and seq28 (t12):
  `1. Draw 1 with Clue [cost: {2}, Sacrifice] {card text: "() source: Serra Ascendant"}`
  -- empty parenthesised card text and a source attribution to a creature that had already
  left the battlefield (`Your battlefield (creatures: 0)` in the same prompt). The model
  used the option correctly anyway (seq28 sacrificed it to draw). Low priority.
- **Crash gate:** zero crash/abort signatures in `game-122v152-*.stderr` and
  `game-122v158-*.stderr` (and none in the four opponent-hosted games at this seat) ->
  N-139h validated at this seat.
- Null at this seat this wave: g1 (convoke), g2/h3 (tutor/reveal), g3 (mutate), g5
  (dungeons -- opponent-side only), g6/h6 (walker magnitude -- Sorin never cast), h4/h5
  (MDFC / display-toggle priority).
