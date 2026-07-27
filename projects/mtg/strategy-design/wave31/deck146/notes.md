# deck146 -- wave-31 engine / harness notes

Corpus `matchups-20260727-025026`, binary cf7cb2e53. Seat logs: `*ai_baka_deck146-*.jsonl`.

---

## RESOLVED THIS WAVE

### N-146g -- planeswalker magnitude annotations: **CLOSED (h6 validated), retire**
Wave-30's bogus `{right now: damage 8}` on Lolth's Cast and loyalty options is gone. Before/after
verbatim and the counts (wave-30: 14/14 of this seat's magnitude annotations were on walker lines;
wave-31: 0 at this seat, 49 still firing at deck158) are in findings.md (a). The fix is surgical --
the eval is alive corpus-wide, it just skips planeswalkers. Kaya's 9-option loyalty menu (vs122
seq35 t12), her first live render in the campaign, is clean.

### N-139h crash gate -- PASS at this seat
0 crash / abort / assert / "core dumped" signatures across all 6 deck146 game stderr files.
Only stderr hit is benign: `game-146v158-1785150409.stderr:80: FATAL: AIStats.cpp:load : can't load
ai/baka/stats/ai_baka_deck158.stats` -- missing stats file for the Step-0 debut deck, not a crash.
(Minor cosmetic: an absent AI stats file is logged at `FATAL:` severity while being fully
recoverable. Downgrading it to a warning would stop it tripping crash greps in every future corpus
that rotates in a new deck. Cosmetic-only, no priority.)

---

## NEW -- N-146h: `Cast Agadeem's Awakening` (MDFC front face, {X} cost) SILENTLY NO-OPS, burning the turn

**Priority: HIGH -- this cost deck146 the vs158 game.** Front-face lane, distinct from N-152a
(which is the BACK-face land placement).

### Repro (deck146 vs158, `1785150413-ai_baka_deck146-0x55db73fa8b20-vs-ai_baka_deck158.jsonl`)

Fires identically twice, offered a third time:

- **seq30, turn 14, kind=ask.** `Mana available: {b}{w} from 7 untapped sources`.
  Options `['Cast Soul Shatter {2}{b}', "Cast Agadeem's Awakening {b}{b}{b}{x}", 'Cast nothing right
  now']`. Model picks option 2.
- **seq31, turn 14, kind=ask.** Prompt now reads
  `Mana available: (no untapped sources) | Already in pool: {b}{b}{b}{w}{w}{w}{w}`
  and every land renders `[tapped]`. Options
  `['Cast Card Normally', 'Flip Side [display toggle only - no game effect: switches which face...]',
  'Decline - do nothing']`. Model picks option 1, `Cast Card Normally`.
- **Result: nothing.** The entire narration for that turn is one line:
  `- Choose an option for Agadeem's Awakening: -> Cast Card Normally`.
  No `hand -> stack`, no `stack -> graveyard`, no life loss, no creature returned. The card is
  **still in hand at seq32** (`Your hand: Soul Shatter {2}{b}; Agadeem's Awakening {b}{b}{b}{x};
  Silverquill Silencer {b}{w}`).
- **seq32/seq33, turn 16** -- identical sequence, identical single narration line, identical
  outcome. Same at seq34 turn 18, where the model finally chose `Decline - do nothing` (seq35).

Game impact: three main phases and ~21 mana consumed with the opponent on 5 life, while a castable
`Silverquill Silencer` sat in hand. deck146 went 11 -> 7 -> -1 and lost at t19.

### Root-cause hypothesis (worth checking first)

Primitive (`Res/sets/primitives/*.txt`):

```
name=Agadeem's Awakening
auto=this(X=0) name(Return any creatures from graveyard) moveTo(myBattlefield) target(creature[manacost=0]|myGraveyard)
auto=this(X=1) ... target(creature[manacost=0]|myGraveyard) && ability$!... target(creature[manacost=1]|myGraveyard)!$ controller
... (X=2 .. X=n)
mana={B}{B}{B}{X}
```

**There is no ANNOUNCE_X step anywhere in the trace.** The seat never sees an X prompt: it goes
Cast -> face-chooser -> nothing. If X defaults to 0, the only live branch is `this(X=0)` whose sole
target predicate is `creature[manacost=0]|myGraveyard`. deck146's graveyard at turn 15 held Nadaar
(MV 3), Silverquill Silencer (MV 2) and Barrowin (MV 4) -- **no MV-0 creature**, so the ability has
zero legal targets and the cast aborts before reaching the stack, leaving the card in hand.

This is the same family as h2's convoke work (`maxAnnounceableX` -- a missing X-cap silently
suppressing ANNOUNCE_X and producing a silent no-op). Suggested checks, in order:
1. Does the AI cast path reach ANNOUNCE_X at all for an `{X}` cost on an MDFC front face? (Compare
   against a non-MDFC {X} spell to isolate whether MDFC-ness or {X} is the trigger.)
2. If X is announced as 0, the abort is arguably correct MTG-wise -- but then the option should not
   be **offerable** with a full untap, and the mana must not be committed to the pool before the
   legality check. Either gate the offer on "some X yields a legal target", or fail LOUDLY
   (re-ask / deferred_to_heuristic) rather than consuming the phase silently.
3. Whatever the resolution, the silent-consume is the worst part: a chosen action that produces zero
   narration and zero state change is indistinguishable from a hang to the model, and it re-chose the
   same dead action on the next two turns because nothing told it the first attempt had failed.

### Second, independent witness for N-152a (MDFC back-face land) at a NON-152 seat

vs158 **seq29, turn 14**: `Your hand: Soul Shatter {2}{b}; Agadeem's Awakening {b}{b}{b}{x}; Plains
(land: taps for {W})`; the land-drop ask offered exactly
`['Play Plains', 'Hold Plains - do not play it now']`. Agadeem's-as-a-land is absent, matching the
primitive's gate:

```
restriction=compare(isflipped)~equalto~0
autohand={0}:restriction{can play land,compare(isflipped)~equalto~1} name(Agadeem, the Undercrypt) ... forcetype(land)
```

The land play requires `isflipped==1` while the front face is `isflipped==0`. This confirms h4's
"structurally cannot place the back face via AI seams" read is deck-independent, and it means every
MDFC-carrying guide in the campaign (deck146 carries 3: Emeria's Call x2, Agadeem's Awakening x1,
plus Pelakka Predation x2) is at risk of teaching an unofferable land drop. deck146's guide did
exactly that; corrected this wave in strategy.txt.

---

## NEW -- N-146i: `Cast <card>` option lines carry NO card text (representation)

**Priority: MEDIUM.** Mechanism behind the Acererak belief-defect in findings.md (b)/(d).

At this seat, **0 of 163** `Cast ...` option lines carry a `{card text: ...}` annotation, while
activated-ability and loyalty options carry the full text (e.g. vs122 seq31:
`1. -3: create spiders with Lolth, Spider Queen [cost: Counters] {card text: "Whenever a creature
you control dies, put a loyalty counter on Lolth, Spider Queen. -- 0: You draw a card and you lose 1
life. -- -3: Create..."}`). So the model gets rules text for abilities it activates but not for
spells it casts.

The cost of that asymmetry, concretely: `2. Cast Acererak the Archlich {2}{b} (5/5)` shows a printed
(5/5) and nothing else, while the card's actual behaviour is
`return Acererak the Archlich to its owner's hand and venture into the dungeon` (primitive
`borderline.txt`, `auto=if type(Tomb of Annihilation[dungeoncompleted]|myzones)~equalto~0 then
name(Return to hand) moveTo(hand)`). Across 3 games the model produced **59 reply passages** planning
to attack or block with a creature that never stays on the battlefield, and at vs148 seq18 t8 it
declined an offered `Cast Vanishing Verse` on the opponent's Kor lord to cast it, reasoning verbatim
*"He is a 5/5 threat that forces the opponent to either sacrifice a creature... This immediately
improves my board state"* -- all false, and the resulting board swung 12 damage the next turn.

Note this is a *self-bounce ETB on a permanent spell*, which is close kin to the wave-26 R-ADV-FACE
family (an option's (P/T) describing something other than what you actually deploy). Suggested fix
direction, cheapest first:

1. **Narrow, high-value:** annotate Cast options for permanents whose ETB immediately moves them out
   of play, e.g. `2. Cast Acererak the Archlich {2}{b} (5/5) {on resolve: returns to your hand -
   you do not keep the body}`. This is the same shape as the existing ETB pay-or-tap and PT-pump
   delta annotations, and it targets exactly the belief the render currently creates.
2. **Broad:** put `{card text: ...}` on Cast option lines as it already is on ability lines. More
   tokens, but it would close this whole class rather than one card, and it removes the need for
   guides to carry card text that the engine already has.

Until an engine fact ships, deck146's guide carries the corrective teach (Acererak block in
strategy.txt). Per the crutch-lifecycle doctrine that teach should be DEMOTED once (1) or (2) lands
and live acceptance confirms the misfire stops recurring.

---

## OBSERVATION (no action) -- h5 does not cover toggles inside a card-face chooser

deck146's priority-decision count fell 24 (wave-30) -> 18 (wave-31), consistent with N-152b's
display-toggle auto-pass. But `'Flip Side [display toggle only - no game effect: switches which face
...]'` still reaches the model at vs158 seq31/33/35, because there it sits **alongside real options**
(`Cast Card Normally`, `Decline`) in the MDFC face chooser rather than being the only non-pass
option. That is correct behaviour -- the window is not display-toggle-only -- and the model ignored
the toggle all three times. Recording it only so a future reader does not read it as an h5 miss.

## CARRIED, not exercised at this seat this corpus
Convoke divergence C; N-122a Felidar probe; N-139g under-card statics; Rankle mode-bleed;
L-18-1 planning tax; N-134b/d; L2 cross-zone dup-name; ETB header card-name; N8; Azcanta; undying;
commit-lock; prose-reversal; untapped-tagging; I-4.
