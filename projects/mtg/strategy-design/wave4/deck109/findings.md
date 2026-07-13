# Wave-4 review — deck109 (Hellrider, mono-red aggro)

**Corpus:** two runs, `matchups-20260712-192832` (run1) and `matchups-20260712-230636` (run2).
**Record:** 3/6 (run1) then 5/6 (run2) = **8/12, zero timeouts** — one of the two best decks.
Games cited by result + opponent + deck109 seat-file epoch (`sN` = record `seq`):

| epoch | run | result | opp | end |
|---|---|---|---|---|
| 1783902515 | r1 | WIN | 110 | T9, o0 |
| 1783905418 | r1 | **LOSS** | 135 | T14, me-14/o1 |
| 1783907829 | r1 | WIN | 133 | T10, o-5 |
| 1783908401 | r1 | **LOSS** | 140 | T50, me11/**o181** |
| 1783910062 | r1 | WIN | 44 | T13, o-1 |
| 1783910629 | r1 | **LOSS** | 131 | T26, me0/o18 |
| 1783915600 | r2 | WIN | 131 | T11, o-3 |
| 1783917446 | r2 | WIN | 135 | T15, o0 |
| 1783917615 | r2 | **LOSS** | 133 | T13, me-4/o5 |
| 1783917730 | r2 | WIN | 44 | T13, o-1 |
| 1783918764 | r2 | WIN | 140 | T19, o-1 |
| 1783920257 | r2 | WIN | 110 | T10, o0 |

**Headline:** wave-3's dominant behavioral leak (the "attacking taps my lands" mana delusion) is
**FIXED** by a new representation line, and the deck now plays its aggro game cleanly (attacks
every combat, casts ~93% of the time, throws burn at the face). **All four losses trace to two
ENGINE/HARNESS bugs, not to model judgment or the guide** — and both bugs are outside the
guide's reach. The guide's remaining opportunities are small (one matchup DO-NOT, minor clock
tightening, and mitigations that make the deck less exposed to the two engine bugs).

---

## The 3 most consequential findings

### 1. [GAME-LOSING · ENGINE-PARSE · PERSISTS from wave-3 §1] The reply parser grabs a stray digit from echoed option text instead of the leading option number

This is wave-3 finding §1, now seen **8 times across 5 games** and with the mechanism nailed.
When the model's reply does **not** begin with the clean `N` + newline + `PLAN:` form — i.e. it
echoes the option text or writes prose before `PLAN:` — the parser extracts a digit from
*inside that text* (a mana cost, a power/toughness, or a life total) rather than the option
number. Every one of the 8 desyncs had a "plan-less head". Proof by the echoed strings:

- **L135 r1 s24 — this LOST the game.** Pillar of Flame target menu, opponent at **2 life,
  Pillar deals 2 = exact lethal.** Reply: `1\nCast Pillar of Flame targeting the opponent
  (player, life 2) to deal 2 damage and win the game immediately.` Leading token is `1`
  (option 1 = "The opponent (player, life 2)"). Logged `choice=2` = **Ice-Fang Coatl** — the
  parser took a `2` from "life 2"/"2 damage". The exact-lethal Pillar hit a 1/1 instead of the
  face; opponent's 14/14 Abominable Treefolk killed us next turn (me 2 → -14). (Contrast s23,
  one record earlier: reply `1\nPLAN: Cast Pillar...` — clean `PLAN:` form — parsed correctly.)
- **L140 r1 s45/s46/s49/s50 — false "cast nothing".** Reply `1. Cast Hellrider {2}{r}{r} (3/3)\n\nPLAN:...`
  → parser took the `2` from `{2}{r}{r}` → `choice=2` = **"Cast nothing right now."** The model
  wanted to cast; the engine recorded a pass. Four such passes in the game we lost by durdling.
- **W135 r2 s27:** reply `3. Cast Krenko's Command {1}{r}\n\nPLAN:...` → took `1` from `{1}{r}`
  → `choice=1` = Boros Reckoner (wanted Krenko's, option 3).
- **W135 r2 s29:** reply `4. Boros Reckoner (3/3) [your battlefield]...` → took `3` from `(3/3)`
  → `choice=3` = **"Yourself (player, life 19)"** — a targeting menu, so the parser aimed a
  spell at **our own face** (survived only because the game was already won).
- **W135 r2 s32:** reply `2. Cast Krenko's Command...` → `choice=1`.

**Root cause is in AIPlayerGPT reply parsing, not the guide.** Fix there: take the FIRST
standalone integer at the very start of the reply, validate it in `[1, options]`, and **never**
scan option-text/prose for digits. Add L135 s24 and W135 r2 s29 as regression fixtures.
**Guide-side mitigation (partial, worth doing):** the system prompt already says "Reply with the
number of your choice, then your PLAN: line" — but the model frequently violates it by echoing
the option text first. Reinforce a rigid output contract ("Reply with ONLY the bare number on
the first line — do NOT repeat the option's text or its mana cost — then `PLAN:` on the next
line"). This shrinks the parser's failure surface even before the parser is fixed.

### 2. [GAME-LOSING · ENGINE-CAST-NOOP · NEW] Selecting "Cast Hellrider" silently does nothing under a near-empty board — and the model dutifully re-selects it forever

Distinct from finding 1 (here the parse is **correct**). When "Cast Hellrider" is chosen, the
cast can be a complete **no-op**: mana is not spent, Hellrider stays in hand, the phase simply
advances. The game log shows Hellrider going only `library -> hand`, **never** `hand -> stack`.
The model — correctly obeying the guide's "cast it every turn" — re-selects it every main phase
and bleeds out.

- **L131 r1 — lost entirely to this.** Turns 11–25, the model chose "Cast Hellrider" (correctly
  parsed, `chosen_text = "Cast Hellrider {2}{r}{r}"`) **16 times**. Proof it no-ops: within each
  turn the "Mana available: … from N untapped sources" count is **identical** in Main phase 1 and
  Main phase 2 (T17: 6 and 6; T19: 7 and 7; T21: 7 and 7 …) — a real cast would have dropped it by
  4. Opponent's life sat frozen at **17 for 11 straight turns** while our life bled 18→0 to
  Guttersnipe pings. Board was all Mountains the whole time (Hellrider + Boros Reckoner + others
  stranded in hand). Died T26. (s20–s41.)
- **L140 r1 — major contributor.** "Cast Hellrider" chosen and no-op'd **43 times** (plus the 4
  parse-desyncs from finding 1). In the *same game* **Boros Reckoner cast fine**
  (`hand->stack->battlefield`, attacked, died) — so this is **Hellrider-specific here, not a dead
  board or a countering opponent.** With its only reach/ping engine uncastable, the deck could not
  outrace a lifegain deck; opponent climbed to **181 life** by T50.

**It is not a flat "Hellrider can't be cast" block:** Hellrider resolved normally in **W110 r1
s17** (`hand->stack->battlefield`, then alpha-struck with 6 creatures). The difference correlates
with **board width** — it resolved with 5 other creatures out; it no-ops in L131/L140 when the
board is creature-empty (0–1 creatures). **Root cause needs engine inspection** (candidate: the
cast path for a haste creature with an on-attack trigger when there is nothing else to attack
with). Fully outside the guide. Flag for engine work with L131 s27 (no-op) vs W110 r1 s17
(success) as the A/B repro.

### 3. [FIXED · wave-3 #1 leak resolved] The "attacking taps my lands" mana delusion is gone — thanks to a new representation line; propagate it

Wave-3's single biggest lever was the model believing its lands were tapped from attacking and
passing its second main phase. The prompt now carries:

> `Mana available: {r}{r}{r} from 3 untapped sources (tapped automatically when you cast)`

and the model reads it. Evidence it landed:
- **Cast-nothing rate is 7%** (13 of 174 casting asks) — the durdle problem is essentially gone.
- A precise scan for genuine "my Mountains are tapped from attacking → pass" reasoning with
  untapped sources present returns **≈0 real instances** (the surface hits are all the model
  *correctly* reasoning about the OPPONENT's untapped blockers, or correctly noting it can't
  afford a 2–4-drop with 1 mana).
- The guide's own wording now echoes back in plans as the *correct* belief, e.g. W44 r1 s12:
  "lands untap at start of turn, attacking doesn't tap lands."

This is a representation win — recommend the reviewer keep the deck-guide's "COUNT YOUR MANA"
block (it reinforces the same fact and does no harm) but stop treating the delusion as the #1
threat; it has been superseded by the two engine bugs above. **Worth flagging to other decks'
reviewers as a general-layer success to rely on.**

---

## Supporting findings

### 4. [QUALITY · matchup-specific] Phyrexian Obliterator (deck133) — the model feeds it and self-sacrifices
**L133 r2 (loss).** Deck133 fielded Phyrexian Obliterator (5/5 trample; *whenever a source an
opponent controls deals damage to it, that player sacrifices that many permanents*). The model
repeatedly **attacked 1/1 Goblins into it and blocked it** (s17/s18 attack-in; s19 double-block),
each time dealing damage to the Obliterator and triggering **its own** sacrifices (s15/s16/s20/s21
"Choose the target for Sacrifice" — it correctly sac'd Mountains, but the permanent loss is what
ground us out, me 18→-4). This is a genuine hard interaction, not a durdle. Candidate DO-NOT line
(deck133 is in the pool): *"Against Phyrexian Obliterator (deck133's 5/5), do NOT deal damage to
it — never attack into it and never block it; race around it and send burn at the face only."*
Note we WON the r1 vs133 game (Obliterator not drawn), so weight it as a matchup note, not Rule #1.

### 5. [QUALITY] Late closes persist on the wins — the clock leak wave-3 flagged is smaller but not gone
Most wins are on-curve, but a few still close 1–3 turns late for finding-1/2 reasons rather than
hoarding. W110 r1 is the model-play ideal (Hellrider resolved, alpha strike, dead T9). The
burn-hoarding wave-3 flagged is **much improved**: across the corpus, resolved damage/target picks
go **face 35× vs enemy-creature 4×** — burn is being spent as a clock, not saved as removal.

### 6. [POSITIVE — propagate to other guides] What the guide + general prompt got RIGHT
- **Aggression is total and correct:** **67 of 67** attacker decisions declared attackers, **zero**
  "none." The deck never sat home. (e.g. wide swings L135 r1 s15, W133 r1, W110 r1 s20 six
  attackers.)
- **Burn-face default holds** (35 face / 4 creature, above) — the wave-3 "burn is a clock" framing
  and the face-by-default rule are working.
- **Unleash counter always taken** (14/14 where offered) — consistent, no hesitation.
- **Development every turn** — MP1 land + a cast nearly every early turn; the MP2 checklist fires.
- The **new `chosen_text` translog field** (wave-3 §6 ask) is present and made this whole review
  far faster — it exposed both engine bugs immediately by showing intended-vs-consumed action.

---

## Wave-3 findings status

- **§1 reply-parse desync — PERSISTS (now quantified + mechanism identified).** See finding 1: 8×
  across 5 games; the trigger is a plan-less/option-echoing reply and the parser pulling a digit
  from mana costs / P&T / life numbers. Still game-losing (L135).
- **§2 `choice:-1` / empty replies / silent fallback — NOT OBSERVED this corpus.** 0 fallback
  records, 0 `choice==-1`, 0 empty non-gameend replies across all 12 games. Either fixed or did
  not occur here.
- **§3 "Untapped lands / available mana" representation line — IMPLEMENTED → delusion FIXED.**
  See finding 3. This is the corpus's biggest positive delta from wave-3.
- **§5 blockers seam untested — NOW EXERCISED (partially).** 4 `blockers` records appeared
  (vs135 r2, vs133 r2), vs 0 in wave-3. Still thin; the L133 Obliterator blocks (finding 4) are the
  one place blocking judgment mattered and were questionable.
- **§6 translog improvements — `chosen_text` IMPLEMENTED** (finding 6); `Mana available` count
  line also added.
- **Hoarding / hold-instants (wave-3 general S1, deck-guide BURN section) — IMPROVED.** Burn now
  goes face by default; isolated hold moments remain but no game was lost to hoarding this corpus.
- **Summoning-sickness casting veto (wave-3 general S5) — LARGELY FIXED** (7% cast-nothing; the
  model casts creatures the turn it can, e.g. cast Gore-House while noting it can't yet attack).

## Net guidance for the reviewer who revises the guide next
The two losing bugs (findings 1 & 2) are **engine/harness**, not guide — surface them to the
engine owner with the cited repro records. The guide itself is in good shape; the only genuinely
guide-shaped changes are: (a) a rigid **output-format reinforcement** ("bare number only on line
1, no option text, then `PLAN:`") to shrink the parser's failure surface (finding 1); (b) the
**Phyrexian Obliterator DO-NOT** (finding 4); (c) optionally, avoid making Hellrider a marquee
"cast it every turn no matter what" instruction until the cast no-op is fixed — because the guide
is currently doing exactly what it should and the engine is eating it (finding 2). Do NOT
re-escalate the mana-delusion content — that battle is won (finding 3).
