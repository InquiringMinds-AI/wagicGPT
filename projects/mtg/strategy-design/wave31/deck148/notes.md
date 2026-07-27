# deck148 (Kor Army) dev notes - wave 31 (CONFIRMING corpus / EXIT wave)

Corpus `matchups-20260727-025026`, binary `cf7cb2e53`, -T 3000, REPPENALTY=1.05. Record 4/6.
Guide FROZEN and cmp-verified byte-identical (deployed == wave30 copy == wave31 copy).
Seat health: 187 decisions, **0 fallbacks**, **0 crash signatures in all six stderr files**
(h1/N-139h gate contribution), 0 parser contradictions, 0 adjudications.

---

# PART A - ENGINE / REPRESENTATION LEDGER (four new items, all with seq repros)

The equipment archetype stresses a surface no other seat in the pool loads as hard: many small
same-named permanents (Kor Ally tokens) wearing many same-named attachments. Three of the four
items below only appear under that load, which is the argument for closing them with a targeted
probe deck rather than by holding this rotation seat.

## N-148a (REPRESENTATION, structural) - EQUIPMENT never appears in the host creature's forward attachment list
`describeAttachments()` (`src/AIPlayerGPT.cpp:376`) finds attachments by scanning for
`att->auraParent == host` (line 388). The forward-render block at line 686-693 documents the fact
that defeats this: **"Equipment keeps its host in `target` (excluded from `auraParent` by the
engine); auras in `auraParent`."** So an Equipment is never enumerated by `describeAttachments`,
and a creature's board line never carries a `{attached: ...}` list for its gear.

**Measured:** across all six deck148 seat logs there are **112 reverse `[attached to: X]` renders
and 0 forward `{attached:` renders**. Every equipped-creature board line in the corpus omits its
own equipment.

**Repro (any deck148 log; cleanest at vs122 seq26 t12):**
`Kor Duelist {w} (10/1) (printed 1/1) [double strike]; Captain's Claws #1 {2} [artifact] [attached
to: Kor Duelist]; ... Captain's Claws #2 {2} [artifact] [attached to: Kor Duelist]; ... Cranial
Plating {2} [artifact] [attached to: Kor Duelist]`
- the Duelist line shows the CORRECT modified P/T `(10/1) (printed 1/1)` and the granted
`[double strike]`, so the pilot is not blind - but to learn WHAT is on the Duelist it must scan the
whole battlefield string and reverse-map three separate artifact lines.

**Severity:** low on its own (the P/T and keyword renders carry the deciding facts), but it is the
precondition that makes N-148b bite. **Fix direction:** extend `describeAttachments` to also match
`att->target == host` for cards with `hasType(Subtypes::TYPE_EQUIPMENT)`/`"fortification"`, mirroring
the type test the reverse block at line 686-690 already performs. Cheap and symmetric.

## N-148b (REPRESENTATION, caused a WRONG COMMIT) - the reverse `[attached to: X]` omits `instanceHandle(host)`
`src/AIPlayerGPT.cpp:693` renders `out << " [attached to: " << host->getDisplayName() << "]"` -
**without** `instanceHandle(host)`. Every other surface has the handle: `instanceHandle()`
(line 414) is applied to battlefield card names and target lines precisely so "a bare name cannot
bind a board-snapshot entry to an offered line". The attachment relationship is the one place it
was not applied, and it is exactly the place a token deck collides.

**Repro - deck148 vs152, seq30 and seq31 (t18), the deciding turn of a game lost by 2 life.**
- seq30 board: three identical `Kor ally #1 (1/1); Kor ally #2 (1/1); Kor ally #3 (1/1)`, plus
  `Cranial Plating {2} [artifact]` (unattached), 6 artifacts total. Model equips the Plating to a
  Kor ally. Correct and well-reasoned.
- seq31 board, one decision later: `Kor ally #2 (7/1) (printed 1/1)` and
  `Cranial Plating {2} [artifact] [attached to: Kor ally]` - **no `#2`**. There are three Kor
  allies. The model cannot bind the Plating to the 7/1 and mis-attributes the buff, writing
  verbatim: *"The 7/1 Kor ally (currently equipped with Captain's Claws) is my primary threat...
  Cranial Plating grants it +6/+0, making it a 13/1. This is lethal damage (13 > 11)."*
  It re-equips the SAME Cranial Plating (spending its last {1}), gains nothing, and commits to a
  PLAN whose stated math is *"This deals 15 total damage, reducing the opponent from 11 to -4,
  winning the game immediately."*
- Actual swing at seq32: 7 + 1 + 1 = **9**. Opponent survived at 2 and killed deck148 on the
  crack-back (Tovolar's Huntmaster 6/6 + Briarbridge Tracker 5/4 + two Wolves). Final -2 to 2.

**Classification:** this is the wave-26 method-headline-5 **clause (e)** shape verbatim - a WRONG
COMMIT produced by a representation ambiguity beneath the seam (same-named permanents, no instance
handle), not by spiral length and not by a guide gap. Per that clause the fix is at the
representation layer. It is also the second live instance of the clause, which is worth recording:
the wave-26 witness was two Lovestruck Beasts at a blockers B-line; this is three Kor allies at an
attachment render. Two independent instances of "identical names, missing handle -> wrong commit"
argues the handle should be applied at **every** surface that names a permanent, as an invariant,
not per-surface.

**Fix direction:** `out << " [attached to: " << host->getDisplayName() << instanceHandle(host) << "]"`.
One expression. Same for `att->getDisplayName()` at line 390 once N-148a lands (two Captain's Claws
on one host is a real board state in this deck - see vs122 seq26).

## N-148c (ENGINE, rules divergence) - Stone Haven Pilgrim's granted LIFELINK never produces life
Primitive (`borderline.txt`), verified verbatim:
`auto=@combat(attacking) source(this) restriction{type(*[artifact;enchantment]|myBattlefield)~morethan~0}:1/1 ueot && lifelink ueot`
`text=Whenever Stone Haven Pilgrim attacks, if you control an artifact or enchantment, Stone Haven
Pilgrim gets +1/+1 and gains lifelink until end of turn.`

**The `1/1 ueot` half fires; the `lifelink ueot` half does not.** Two independent witnesses, each
verified by life-total arithmetic against the narration (the renderer DOES emit `Your life +N` -
opponent lifegain lines appear in these same logs, e.g. vs152 `Opponent's life +3 (now 15)` from
Intrepid Adversary):

1. **vs158, narration turn 5** (deck148 log `1785143744-...-vs-ai_baka_deck158`). Pilgrim (2/2) +
   Captain's Claws (+1/+0) attacks with Claws on the battlefield -> `Damage: 4 dealt by Stone Haven
   Pilgrim to the opponent`. The 4 confirms the +1/+1 resolved (2+1+1). Life before = 19; next
   recorded change is turn 6 `Your life -2 ... -1 (now 16)` = 19-3. **No +4.** Same game turn 7:
   Pilgrim deals 5 (2+1 Claws+1 Bone Saw+1 trigger), life 16 -> 13 on 3 damage taken. **No +5.**
2. **vs146, narration turn 12.** Pilgrim + Claws, artifacts in play, blocked by a Goblin ->
   `Damage: 4 dealt by Stone Haven Pilgrim to Goblin` (2+1+1 confirms the trigger). Life before 13;
   turn 13 `Your life -1 (now 12)`. **No +4.**

**Do NOT stopgap the guide.** The deployed guide's "it hits for 3 and gains you 3" is faithful to
both the Oracle text AND the engine's own `text=`/`auto=`; the defect is in execution of the
`&& lifelink ueot` clause. Per wave-30 method headline 3, rewriting guide prose to describe an
engine bug creates a stopgap the eventual fix must invert. Guide untouched (see findings.md
section 4).

**Fix-agent brief note:** the same `X ueot && lifelink ueot` construct appears widely (21 hits in
`borderline.txt`, 29 in `mtg.txt` for `lifelink ueot`), including on activated abilities
(`{1}{W}:vigilance && lifelink ueot`). Determine whether the failure is (a) the `&&` chain dropping
the second clause when the first is a P/T pump with an explicit `ueot`, (b) granted-lifelink not
consulted by the combat damage life-gain path, or (c) trigger-vs-damage ordering - and fix the
CLASS, not the card. A `[AI]` fixture on Stone Haven Pilgrim + any artifact is a two-line repro.

## N-148d (REPRESENTATION, single witness) - a creature in HAND carries no explicit creature marker; produced a false mulligan
`typeTag()` (`src/AIPlayerGPT.cpp:293`) returns `""` for creatures by design - the header comment
scopes it to "non-creature option/target lines" - and creature-ness is signalled implicitly by the
`(P/T)` parenthetical instead. Every OTHER card type in a hand line gets an explicit bracket:
`[artifact]`, `[enchantment]`, `[instant]`, `(land: taps for {W})`. The result is an asymmetry
where the single most important pre-game fact for an aggro deck ("is there a creature in this
hand?") is the one fact with no label.

**Repro - deck148 vs146 seq1 (t0, mulligan).** Hand rendered:
`Bone Saw [artifact]; Kor Duelist {w} (1/1); Bladed Pinions {2} [artifact]; Plains (land: taps for
{W}); Emeria, the Sky Ruin (land: taps for {W}); Plains (land: taps for {W}); Bone Saw [artifact]`
Model reply: **"This hand has zero creatures... the lack of a body makes this a dead hand"** ->
Mulligan. Kor Duelist is a turn-1 castable creature with two untapped-source Plains; by the guide's
own explicit mulligan rule this hand is a KEEP.

**Precedent for the fix is in the source already.** The `landTag` comment at line ~313 records the
same failure with the same cause: *"deck93 wave-27: mulliganed the identical 3-Swamp opening hand 5
of 6 games claiming 'zero lands'"* - resolved by adding an explicit `(land: taps for {W})` tag.
This is that defect's creature analogue. **Fix direction:** give creatures an explicit marker on
hand/choice lines - e.g. `(1/1 creature)` or a `[creature]` tag - keeping the P/T. Note the
counter-evidence for calibration: in the SAME corpus at vs137 seq1/seq2 the model read
creature-presence correctly twice from the identical render, so the surface is readable but not
robustly so. Single witness -> propose, do not over-claim; the deck93 precedent is what makes it
worth acting on cheaply.

## Standing items - status at this seat
- **L-18-1 planning tax**: recurs as a LENGTH phenomenon (6 replies >10k ch, max 14,120 at vs139
  seq21 t9; max latency 210.5 s) with **zero** correctness or termination cost, and exclusively on
  ubiquitous low-stakes seams (casts, priority passes). Confirms the wave-25 headline-5 relocation
  ceiling; keep as a latency WATCH, add no per-seat floor. N-18e did not need to fire.
- **g4 tapped-tag**: clean third corpus, 11 blocker decisions, no misread.
- **Equip option/target surfaces**: options bind attach target + equip cost + card text correctly;
  the `Artifacts in play: you N | opponent N` line is present and the model uses it for Cranial
  Plating math (vs152 seq30 counted 6 artifacts correctly). The only defect in this family is
  N-148a/b above.

---

# PART B - THE CONSTRUCTION HAND-OFF (for the ROSTER OWNER)

**Status: the guide track on deck148 is COMPLETE and the guide is frozen.** Three corpora
(1/6 Step-0, 2/6 first-guided, 4/6 confirming) with the same 60 cards and, for the last two, a
byte-identical guide. Every teach fires; one is quoted back verbatim by the pilot. The remaining
lever is the decklist. Everything below is grounded in an observed failure in this corpus or the
last, with the game named.

**Current list (60):** 21 lands (18 Plains across five printings, 2 Kabira Crossroads,
1 Emeria, the Sky Ruin), 20 creatures (4 Armament Master, 4 Kitesail Apprentice, 4 Kor Duelist, 4 Stone
Haven Outfitter, 2 Stone Haven Pilgrim, 2 Zamriel), 2 Oath of Gideon, **17 equipment** (4 Bone Saw,
4 Captain's Claws, 3 Sword of Vengeance, 2 Bladed Pinions, 2 Cranial Plating, 2 Argentum Armor).

### HAND-OFF 1 - TRIM THE LOW-IMPACT EQUIPMENT GLUT (highest confidence, two corpora)
17 equipment against 20 creatures is more gear than bodies to carry it, and the corpus shows the
weakest slots doing nothing while the best ones win games.
- **What is actually winning:** Captain's Claws. It is the deck's real engine - `@combat(attacking)
  source(mytgt):token(Kor Ally,...)` makes a body EVERY attack, which is simultaneously the clock,
  the Armament Master fodder, and the Cranial Plating count. vs122: two Claws on one Kor Duelist
  produced six Kor Ally tokens and a 20-to-(-8) kill without taking a point of damage. vs158: Claws
  token accumulation is what beat the corpus's best deck after removal ate the original carrier.
  **This is a 4-of that would happily be more.**
- **Bone Saw x4 (+1/+0, cast {0}, equip {1})** is close to a blank outside two narrow roles: turning
  on Kor Duelist's double strike / Kitesail's flying, and padding artifact count for Cranial
  Plating. In vs137 the deck's ENTIRE equipment contribution for 18 turns was one Bone Saw, and when
  it was destroyed at turn 11 the Duelist dropped from 4 damage a turn to 1 and the game was lost
  with the opponent on 1 life.
- **Bladed Pinions x2 (equip {2}, flying + first strike, no stat boost)** was bottomed at the
  mulligan in the one game it appeared in a keepable hand (vs146 seq3) - the pilot correctly judged
  it the most expendable card in a six-card hand.
- **Argentum Armor x2 ({6} cast, {6} equip)** was drawn twice (vs158, vs152) and cast **zero** times
  in 12 guided games. A 12-mana commitment in a deck that wants the game over by turn 10-14 is
  uncastable by construction; it is functionally two blank cards.
- **Suggested direction:** cut Argentum Armor x2, Bladed Pinions x2, and 2 Bone Saw = **6 slots**.
  Aim at roughly 11-12 equipment / 24-25 creatures. The failure this fixes is concrete: wave-30
  vs139 flooded with FIVE equipment against 2 bodies and died at 0.

### HAND-OFF 2 - THE ENGINE NEEDS TO SURVIVE (carried from wave 30, now with a decision caveat)
Armament Master is a bare 2/2 with no protection, no haste and no evasion, and it is the card the
whole plan is priced on (`gear = 1 -> lord 2/2` on every other Kor, `gear = 2 -> 4/4`). Its
lifetime across 12 guided games: cast 5 times, geared **once** (vs146 - which it won outright, a
12-damage swing at turn 10). The other four: died bare as a 2/2 to combat (wave-30 vs136), traded
in combat (wave-31 vs152 seq10 - a DECISION, see below), and killed by removal before untapping
(wave-31 vs152, Fateful Absence).
- **Construction fix directions:** cheap protection the deck can hold up ({W} indestructibility /
  hexproof), a haste enabler so AM can be geared and swung the turn it lands, or a lord that is not
  a 2/2 (a resilient or recursive payoff).
- **Honest split from this corpus:** ONE of the five failures was not construction - vs152 seq10 the
  pilot voluntarily traded an ungeared AM for a 2/2 Wolf token at 20 life. That is being fixed in
  the SKILL layer (a payoff-preservation teach category), not by you. The other four are yours.

### HAND-OFF 3 - ZERO INTERACTION, ZERO REACH, ZERO CARD ADVANTAGE (the ceiling itself)
This is what makes the deck ceilinged rather than merely unlucky. In both wave-31 losses the deck
got the opponent to **2 life (vs152)** and **1 life (vs137)** and had no card in 60 that could
finish or survive.
- vs152: the opponent untapped and deployed Tovolar's Huntmaster (6/6) + two Wolf tokens onto a
  board already holding Briarbridge Tracker (5/4 vigilance) and a Teferi. deck148 had no removal, no
  reach, no lifegain and no trick; it attacked for 9 into 11 and died to the crack-back.
- vs137: six consecutive Plains off the top (turns 7-17) with no way to convert flood into anything.
  Emeria, the Sky Ruin needs SEVEN Plains and reanimates one creature per upkeep - too slow to be
  the answer, and it entered tapped on turn 1 in vs152 costing a turn of development.
- **Highest-value adds, in order:** (1) 2-4 slots of cheap white interaction (a {1} or {W} removal /
  tap-down / protection spell) - the single change most likely to convert the razor losses;
  (2) a reach outlet so the last 2-3 damage does not require an unblocked attacker; (3) card
  advantage beyond Stone Haven Outfitter's death-draw - the flood games have no mana sink at all.
- **A cheap mana-sink note:** with 21 lands and a curve topping at {3} (excluding the uncastable
  Argentum Armor), the deck has nothing to do with mana from turn 6 on. Even a single 2-of "spend
  five mana, draw/make a body" would flatten the vs137 flood profile.

### HAND-OFF 4 - CARDS THE POOL NEVER LET IT USE (information, not a recommendation)
Across 12 guided games: **Zamriel, Seraph of Steel** (2 copies) was never cast; **Kabira
Crossroads** (2 copies) was never played; **Argentum Armor** (2) never cast. That is 6 of 60 cards
with zero live data. Zamriel in particular is on paper the answer to hand-off 2 (`this(variable
{controllerturn}>0) lord(creature[geared]|mybattlefield) indestructible` - your geared team is
indestructible on your turn) but at {2}{W}{W} in a deck that wants to have won by then, it never
arrived. If you keep it, the list needs to be able to reach turn 4-5 alive; if the deck stays a
turn-9 killer, those slots are better spent per hand-off 1/3.

---

# PART C - ROTATION

**ROTATE OUT.** Guide frozen, teaches validated on a second guided corpus under a materially
different pool, no unexercised guide surface, no guide edit warranted, zero fallbacks/crashes. The
wave-30 exit gate ("one confirming corpus, then freeze + hand off") is satisfied exactly as
specified. Add deck148 to the **construction-terminal-adjacent frozen list** (converged guide,
construction handed off) - distinct from the converged-canary list per the wave-29 rule that
terminal-for-different-reasons seats stay on separate lists.

Residual work created by this seat, all of it OUTSIDE the guide-review rotation:
- **Engine lane:** N-148a, N-148b, N-148c, N-148d (above). N-148b and N-148c are the two worth a
  fix agent; both are cheap and both have exact repros. N-148a/b are best VALIDATED by a probe deck
  (a token+equipment stack forces the name-collision surface that no other roster deck loads) -
  which is the reason they do not hold a rotation slot, the same reasoning that rotated 136 out.
- **Skill lane:** the payoff-permanent-preservation teach category (skill.md proposal 1) and the
  ceilinged-exit pool-composition clause (proposal 2).
- **Roster-owner lane:** Part B.

If the roster owner rebuilds the list per Part B, the result is a materially different deck and
should re-enter as a fresh Step-0 -> first-guided arc under a new seat, not as a continuation of
this one.
