# deck125 (REVELATION FRACTURE, UW draw-go) — wave-61 per-deck review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-092408/`, the six games of the
`...deck125-<ptr>-vs-...` seat. 0 fallbacks in all six; 541 records total.
Every card fact below is checked against `projects/mtg/bin/Res/sets/primitives/` (file:line cited).

## 1. Game by game, and the decisions that decided them

Record **4-2**.

| game | file (deck125 seat) | result | end |
|---|---|---|---|
| v146 | `1788618278-ai_baka_deck125-0x5642c05c5e80-vs-ai_baka_deck146.jsonl` | **LOSS** 0-23 | t19, 29 records |
| v162 | `1788618266-ai_baka_deck125-0x5583f3a623b0-vs-ai_baka_deck162.jsonl` | **LOSS** 0-18 | t15, 48 records |
| v123 | `1788618272-ai_baka_deck125-0x55dea64123d0-vs-ai_baka_deck123.jsonl` | WIN 61-0 | t33, 107 records |
| v152 | `1788618256-ai_baka_deck125-0x564acf67dfd0-vs-ai_baka_deck152.jsonl` | WIN 24 to -12 | t43, 70 records |
| v126 | `1788618276-ai_baka_deck125-0x560a1b6083d0-vs-ai_baka_deck126.jsonl` | WIN 40-0 | t43, 200 records |
| v130 | `1788618289-ai_baka_deck125-0x55652eae9390-vs-ai_baka_deck130.jsonl` | WIN 41-0 | t35, 87 records |

**125v146 — LOSS. The deciding pair is seq 15 (t10) and seq 17 (t12).** Both are
`Cast Sphinx's Revelation ... {X pricing: max affordable X=1 (4 mana total)}` taken at 25 and 26
life. The guide's floor is explicit ("max affordable X=1 / X=2 are both Cast nothing right now, at
any life above 10"). Each cast spent 4 of 4 untapped sources with `Cancel {1}{u}{u}` payable
(`{W} 3, {U} 2` on the mana line) and four counterspells on the hand line. Consequences are in the
log at seq 22: **turn 11 "Opponent cast Triumphant Adventurer / resolved"** and **turn 13
"Opponent cast Barrowin of Clan Undurr / resolved"** — no window offered on either, because the
seat was tapped out. Both are STRATEGY (the row was read correctly and the guide was disobeyed),
and both survivors are among the seven attackers on the lethal turn 19. Barrowin's attack trigger
reanimated Nadaar, Selfless Paladin, whose `auto=this(variable{pdungeoncompleted}>0)
lord(creature|myBattlefield) 1/1` (borderline.txt:75387; text line 75388 = Scryfall) anthemmed the
whole attack; 18 life went to 0.
The last decision (seq 27, Cancel on Lolth, Spider Queen) was correct — planeswalker, counter on
sight — and was not the loss.

**125v162 — LOSS. The deciding decisions are the land drops at seq 4 (t3) and seq 7 (t5), and the
render that made them look right.** See HIGH-1. By seq 26 (t12) the seat had `{W} 5, {U} 1` and its
menu offered ONLY `Cast Path to Exile`; `Cancel {1}{u}{u} [cannot pay now: needs {1}{u}{u}, your 4
untapped sources cannot pay it]` sat twice on the hand line while **Ob Nixilis, the Hate-Twisted**
(seq 26) and **Liliana's Caress** (seq 27) resolved off the stack unanswered, joining two
Underworld Dreams, two Howling Mines and a Dictate of Kruphix. The engine's mana arithmetic is
CORRECT at both windows; the fault is upstream, in the land-drop row.
Two further guide breaks in this game, neither of which would have saved it: seq 33 (t13, 16 life)
and seq 36 (t15, 14 life) both took `Deal 1 damage with Staff of Nin targeting the opponent` while
the prompt printed `DRAW PUNISHERS on the battlefield: theirs - Underworld Dreams, Ob Nixilis, the
Hate-Twisted` and offered `1. Deal 1 damage ... targeting Ob Nixilis ... {right now: takes 1
damage - SURVIVES (loyalty 5, 4 left)}`. Corpus rate on that rule: **0 of 2** (it was 5 of 5 last
corpus; the other 62 pings this corpus had no punisher line and correctly went at the face).
The dying sequence seq 38-46 is clean play, not a misplay: the seat correctly declined nine
Revelation rows carrying `NET -18 life for this cast puts you at -13; this KILLS you`.

**125v152 — WIN.** The Emrakul rule fired correctly once: seq 57 (t40, 19 life, no CONVERTER line)
took `Cast Emrakul, the Aeons Torn`; the extra turn arrived, it attacked, the seat won at 24.
Sweeper discipline held throughout (every `destroys 0 of their creatures` row declined).

**125v126 — WIN, and the CONVERTER carve-out was exercised.** 88 windows carried
`LIFE-TO-DAMAGE CONVERTER on the battlefield: theirs`; no Emrakul row was ever printed, so the
Emrakul half of the carve-out is UNTESTED this corpus. Two counters spent on the combo halves
themselves (seq 33 Cancel on Sanguine Bond, seq 39 Cancel on Exquisite Blood) — exactly the
counter-on-sight list. One break: **seq 93 (t26) Fall of the Gavel on Pride Guardian {w}
(creature 0/3) [defender]** — the guide's "a counter spent on a wall" line, broken once, cost
nothing in a 40-0 win. This game is also where the interface waste is worst (HIGH-2).

**125v123 / 125v130 — WINs.** One break each, neither load-bearing: 125v123 seq 13 (t9, 20 life)
took a Revelation at `max affordable X=2`; 125v130 seq 17 (t10) spent a Cancel on
`Talisman of Impulse {2} (artifact) {target text: "{T}: Add {1}. -- {T}: Add {R} ..."}`, the
mana-rock decline the guide names by name.

## 2. Engine / interface / card items

### HIGH-1 — the land-drop row prints a FALSE unconditional "enters TAPPED" for one of the two checkland idioms (PERCEPTION)

Repro: `1788618266-ai_baka_deck125-0x5583f3a623b0-vs-ai_baka_deck162.jsonl`, seq 8, turn 7.
Own battlefield line reads `Your battlefield (3 permanents ... ): Plains #1-#3 x3`. The row:

```
1. Play Glacial Fortress [enters TAPPED - it makes no mana this turn: "Glacial Fortress enters tapped unless you control a Plains or an Island."]
```

The very next window (seq 9) lists it as an untapped source:
`Those sources, one per untapped card: Plains #1 {w}; Plains #2 {w}; Plains #3 {w}; Glacial Fortress {w} or {u}`.
The bracket is false, and it contradicts the sentence quoted inside itself.

Cause is a script-idiom split, both idioms present in `mtg.txt`:

| card | script | rendered as |
|---|---|---|
| Isolated Chapel `mtg.txt:59411` | `auto=aslongas(plains,swamp\|myBattlefield) tap(noevent) <1 oneshot` | `enters tapped UNLESS its own condition holds` ✅ |
| Seachrome Coast `mtg.txt:101153` | `auto=aslongas(other land\|myBattlefield) tap(noevent) >2 oneshot` | `enters tapped UNLESS its own condition holds` ✅ |
| **Glacial Fortress `mtg.txt:46617-46618`** | `auto=tap(noevent)` / `auto=aslongas(plains,island\|myBattlefield) untap` | **`enters TAPPED - it makes no mana this turn`** ❌ |
| **Drowned Catacomb `mtg.txt:32505-32506`** | same two-line shape | same ❌ |

Lane T C7b reads the `aslongas`/`if(`/`restriction` gate **on the same auto line**; the
tap-then-conditional-untap idiom puts the condition on the NEXT line, so the classifier sees a bare
`tap(noevent)` and asserts the unconditional form. Lane V R3's rule ("a `choice` branch in front of
the tap makes the entry CONDITIONAL") does not cover a conditional `untap` behind the tap.
Scale in this deck alone: **28 of 34** `Play Glacial Fortress` rows across the six games were false
(the 6 true ones are the turn-1 drops with `Your battlefield ... (none)`). Card fact verified
against the primitive `text=` line, which matches the known Oracle text; Scryfall itself was not
queried.
Suggested shape: when a card carries `tap(noevent)` on one line and `aslongas(...) untap` on
another, emit the hedged `enters tapped UNLESS its own condition holds` form (or, better, evaluate
`aslongas(A,B|myBattlefield)` against the caster's battlefield and say which it is — the data is
already in the same serialization that prints `Your battlefield`).

### HIGH-2 — the hold row promises more than it delivers, and the two hold-related brackets contradict each other in the same prompt

Repro: `1788618276-ai_baka_deck125-0x560a1b6083d0-vs-ai_baka_deck126.jsonl`, **seq 50, 51, 52**, all
turn 16, all `Phase: Main phase 1 | It is the opponent's turn`, with **byte-identical** three-row
menus (`Cast Path to Exile {w} {leaves 0 of your 1 untapped mana source untapped ...}` /
`Cast nothing right now` / the hold row). The seat took the hold at seq 50, was re-asked the same
list at seq 51, took it again, and was re-asked a third time at seq 52. The hold row's own text is

```
3. Hold priority: pass now, and do not ask me again - this turn or later - until one of the rows above changes (any change re-opens this window; you give up no cast) {taking this row skips every later window whose rows are identical to these}
```

and the same prompts carry two brackets that cannot both be true:

```
[you declined this exact list 2 times already this turn]      <- engine: the list is unchanged
[hold check: 1 row above is new since the last window at this seam - ...]   <- engine: a row is new
```

Nine such leaks across the six games (held, then an identical row-set re-asked later in the same
turn): v123 seq 26→28 and 48→50, v126 seq 49→51, **50→52**, 61→63, 77→80, 109→111, 119→123,
v130 seq 11→13. There are no *adjacent* leaks (0 of 110 hold takes), so lane V R2 works within a
seam; what fails is the row text's "this turn or later" / "every later window" promise, which is
in fact seam-scoped.

Downstream cost, measurable: 57 windows in these six games carried
`[you declined this exact list N times already this turn]`; the hold was taken at only **21**.
125v126 turn 40 spent windows seq 174-182 on ONE unchanged three-row menu across Upkeep, Draw,
Main 1, Combat begins, Attackers, Combat ends and Main 2, each printing
`[hold check: every row above was also on the menu at the last window at this seam (N windows in a
row now)]` with N climbing to 7; turn 42 repeated it (seq 191-196). That is ~40 wasted decisions in
one game.

### HIGH-3 — the `Cast Sphinx's Revelation` row is the only cast row that hides its tap-out cost, and the best-X badge reads as an endorsement (PERCEPTION, feeding a STRATEGY break)

Repro: `...vs-ai_baka_deck146.jsonl` seq 15 (t10) and seq 17 (t12), and `...vs-ai_baka_deck123.jsonl`
seq 13 (t9):

```
1. Cast Sphinx's Revelation {u}{u}{w}{x} {X pricing: max affordable X=1 (4 mana total); each point of X gains you 1 life and draws you 1 card} [<- best X for this cast: X=1 - largest affordable X - X=1 gains 1 life and draws 1 card; no listed X does more] {card text: "You gain X life and draw X cards."}
```

Two problems on one row.
(a) **No `{leaves N ...}` and no `{spends K of your M ...}` clause**, though the cast taps 4 of 4
sources. Corpus-wide: **9 of 154** Revelation cast rows carry a leaves/spends tail, against
**323 of 634** for every other `Cast ...` row. The guide teaches (correctly, for every other row)
"A row carrying no such clause at all is a row where M is far above K; those are free" — the render
invites exactly that false inference on the one row that most reliably taps the seat out.
(b) The lane-S C10 badge `[<- best X for this cast: X=N ...; no listed X does more]` is printed on
the CAST row, not only on the X menu, and its wording ("best X for this cast", "no listed X does
more") reads as an endorsement of casting. All three floor breaks in this corpus are on badged
rows. (The badge is behaving to spec — it ranks X against X — but on the cast row it is the only
editorial voice on the menu.) The X **menu** itself is clean: **12 of 12** answered option 1.

### MED-1 — `INCOMING THIS COMBAT` cannot see attack triggers, and said "you would be at 9" three windows before a 0

`...vs-ai_baka_deck146.jsonl` seq 27, turn 19, seat at 18 life:
`INCOMING THIS COMBAT: not declared yet - 7 of their creatures can attack, for up to 9 - you would
be at 9`. True for the board as printed. During that combat Barrowin of Clan Undurr's attack
trigger returned Nadaar, Selfless Paladin, whose completed-dungeon anthem (`borderline.txt:75387`)
made the same seven attackers 16 power, and a dark-pool venture took one more. The seat died at 0.
Nothing the seat did off that line was wrong, so this is a narration-honesty item rather than a
decision item — but the forecast is a snapshot presented in resulting-life form, which is the
strongest form of assertion the prompt has.

### MED-2 — `{removes: ...}` on a Path row lists targets the guide will always decline

`...vs-ai_baka_deck162.jsonl` seq 41: `1. Cast Path to Exile {w} ... {removes: Shield Sphere #1,
Shield Sphere #2} - legal targets right now: Shield Sphere #1 (artifact creature 0/6) [defender]
...`. A `[defender]` at 0 power is a row every version of this guide answers "keep the Path". It
was the top row on 20+ windows in that game, including the nine windows in which the seat was
dying one life at a time. LOW-cost, but it is the row that makes an all-dead menu look like a live
one — the "EMPTY menu" problem the guide has to spend a paragraph on.

### LOW-1 — the plan-echo note fires on a menu the plan legitimately does not name

`...vs-ai_baka_deck162.jsonl` seq 9, 16, 26: `YOUR PLAN (... ; "Staff of Nin" is no longer on your
menu)` followed by `(note: this decision's list does not contain the actions your plan names ...)`.
The plan names a card that is uncastable *this window* by design (draw-go). The note is correct but
fires on almost every opponent-turn window of a control deck, and it is the prompt asking the seat
to re-state a plan that has not gone stale.

## 3. Guide verdict — **EDIT**

Three of the corpus's breaks are paid for by a decision that went wrong, and one of them is a game.
Everything else in the guide held at its ceiling and is left alone: 0 land-drop skips, 12 of 12
X-menu option 1s, every `destroys 0 of their creatures` sweeper row declined across six games,
every `deals 0` Lightmine row declined and the two that could kill taken, the Emrakul take at
125v152 seq 57, and the nine `NET ... this KILLS you` Revelation declines at 125v162 seq 38-46.

Revised guide: `wave61/deck125/strategy.txt` (**70,999 bytes**, was 71,000 — in band).

**Edit 1 — the mandated PLAN sentence gets the punisher carve-out.**
before: `"... ping their face every turn; never cast Emrakul while a converter of theirs is printed."`
after: `"... ping their face every turn unless a DRAW PUNISHERS line is printed, and then ping the punisher; never cast Emrakul while a converter of theirs is printed."`
(and the same change in the DECIDING SITUATIONS restatement).
Paid for by **125v162 seq 33 and seq 36**: the plan line is echoed at the top of the ping window,
it is the only guide sentence guaranteed to be in front of the model there, and it said "face".
0 of 2 punisher pings this corpus. The ping section now also names the plan line as the sentence
that will argue against it, and the DECIDING SITUATIONS entry says so.

**Edit 2 — the tap-out gate gets the Revelation exception.**
before: `A row carrying no such clause at all is a row where M is far above K; those are free.`
after: adds that a `Cast Sphinx's Revelation` row almost never prints the clause (145 of 154 this
corpus), that its silence is not the free signal, and that its `max affordable X=N (M mana total)`
M is the whole untapped count — so treat every Revelation row as "casting this taps you out"
unless it prints otherwise.
Paid for by **125v146 seq 15 and seq 17**, where the missing clause is the difference between the
gate firing and not.

**Edit 3 — the X floor stops claiming a ceiling it no longer holds, and names the badge.**
before: `This rule is at its ceiling over three corpora - it does not need re-arguing, only obeying.`
after: names `[<- best X for this cast: X=1 - largest affordable X - ...; no listed X does more]`,
says the badge ranks X against X and does not know this floor, and cites the three breaks
(125v146 seq 15, seq 17; 125v123 seq 13) with the two threats that resolved next turn.
Paid for by the same seqs. The DECIDING SITUATIONS entry carries the badge string too.

**Edit 4 — a checkland paragraph in section #0.**
Added: a row reading `Play Glacial Fortress [enters TAPPED ...: "Glacial Fortress enters tapped
unless you control a Plains or an Island."]` contradicts itself; read the QUOTED sentence against
your own battlefield line, not the bracket; with a Plains or Island already out it enters untapped
and it is the land that fixes your `{U}`. Paid for by **125v162 seq 4 and seq 7** and the
`{W} 5 / {U} 1` board at seq 26-27. This is guide text defending against HIGH-1; it should be
deleted the wave after HIGH-1 is fixed.

**Edit 5 — the hold paragraph gets the corpus number and the leak.**
Added: 57 decline-tagged windows, held on 21; turn 40 and 42 of 125v126 cost five and six identical
windows; and "the hold does not always hold as far as its own sentence promises — you were re-asked
the same three rows at 125v126 seq 50, 51 and 52 — so take it again when it comes back; that costs
nothing."

**Byte budget.** The five edits add ~3.3 KB, paid for by compressing eight passages that are at
their ceiling or stale rather than by cutting any rule: the Emrakul cost history (three rows last
corpus → the one this corpus), the Lightmine Spider anecdote, the cleanup land anecdote, the
second-Staff anecdote (dated to "a wave back" — no second-Staff break this corpus), the window-count
paragraph, the READING THE MANA LINE section, the Forced Fruition section (0 appearances in these
six games; the K-reading mechanic is kept intact), and the sweeper/Path/cleanup restatements. No
rule, threshold, string-test or named card was removed.

## 4. Proposals

None. Nothing here generalises past deck125 that the engine seat will not see with better evidence
(HIGH-1 and HIGH-2 are engine items, not guide items), and no skill amendment is warranted — the
edits above are ordinary applications of existing amendments (a rule that lost a game gets the seq,
a ceiling claim that broke gets retired).

## 5. What I did NOT check

- The other seat's decisions except as context for deck125's losses (146, 162, 123, 126, 152, 130
  seats read only for their game logs).
- Scryfall: no network call was made. All card facts are from
  `bin/Res/sets/primitives/{mtg,borderline}.txt` `text=`/`auto=` lines, cited by line number.
- The engine source. HIGH-1's cause is inferred from the two script idioms plus the two rendered
  outputs, not from reading `landEntersTappedLine`.
- Latency, `async_drops`, `transport`, `deadline_pct`, timeouts and the stderr files — the
  engine seat's census.
- The revised guide was NOT run. No build, no wagic invocation, no git.
- Cleanup-discard ordering was only spot-checked: my row extraction dropped some multi-line rows
  from those asks, so I make no compliance claim about the 17 discard windows.
- Whether the three Revelation floor breaks would have been made WITHOUT the best-X badge. The
  badge is present on declined rows too; the correlation is real, the causation is not established.
