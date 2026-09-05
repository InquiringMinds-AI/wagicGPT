# deck152 (Bant Humans) — wave-61 per-deck review

Corpus: /home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-092408/
Seat files (short names used below):
- G1 = 1788618252-ai_baka_deck152-0x5559f1a82f70-vs-ai_baka_deck126.jsonl
- G2 = 1788618253-ai_baka_deck152-0x55693b1aff30-vs-ai_baka_deck162.jsonl
- G3 = 1788618256-ai_baka_deck152-0x564ace345ef0-vs-ai_baka_deck125.jsonl
- G4 = 1788618270-ai_baka_deck152-0x55965d5e0020-vs-ai_baka_deck130.jsonl
- G5 = 1788618287-ai_baka_deck152-0x55d6eaf1a0a0-vs-ai_baka_deck123.jsonl
- G6 = 1788618294-ai_baka_deck152-0x561c52e74020-vs-ai_baka_deck146.jsonl

Seat totals: 311 decisions, **0 fallbacks**, 24 HOLD takes, 0 re-asks visible, 0 hangs.
Record 3-3 (results.tsv).

## 1. Game by game

**G1 vs deck126 — LOSS 0 to 51 (t18).** Kept a two-land seven (G1 seq1) whose only lands were
Barkchannel Pathway and Lair of the Hydra, both {G}-only; the hand was Luminarch Aspirant,
2x Brutal Cathar and Teferi, all {W}/{U}. The seat drew a third land **never** — turns 5-17 the
land-drop ask reads "you have no land you could play right now" (seq 6 onward). It cast Ranger
Class (seq 5), attacked with the resulting Wolf every turn, and discarded five spells it could
not pay for (seq 10, 12, 13, 14, 15). Every decision in this game is correct on its own surface;
this is colour screw, not guide effect. NOT a guide item.

**G2 vs deck162 — LOSS -4 to 8 (t14). The deciding decision is t11, and it is ENGINE.**
At seq 23 (t11 Main 1, life 12 vs 11) the board is Katilda 1/1 + Elite Spellbinder 3/1 flier,
both listed "able to attack right now"; the opponent's only untapped body is Shield Sphere 0/6
and Master of the Feast is "[tapped - cannot attack or block this turn]". The seat cast Intrepid
Adversary (seq 23) and then answered "add 3 counters" (seq 24). The engine paid the three
{1}{W} instalments with **Elite Spellbinder and Katilda themselves** (log in seq 25: "Paid {1}{w}
with Elite Spellbinder; Mistgate Pathway", "Paid {1}{w} with Katilda, Dawnhart Prime; Overgrown
Farmland"), so **turn 11 has no Attackers ask at all** — the seat's own PLAN at seq 23/24 was
"Attack with Elite Spellbinder and Katilda to deal 4 damage, putting opponent at 7". Neither the
cast row (seq 23 row 2) nor the counter ask (seq 24) printed a `{paying this taps: ...}` clause;
see HIGH-3. Next turn Ob Nixilis -2 destroyed the Adversary and the six mana bought nothing.
The remaining decisions (seq 29 attack, seq 30/31 casts) were made at 2 life under a
"DRAW FORECAST: ... 2 life to the punishers" that was already lethal on the next draw step; the
game was gone by then.

**G3 vs deck125 — LOSS -12 to 24 (t43).** UW control: Essence Scatter on Sigarda (t21), Supreme
Verdict (t22, t36), Path to Exile on Sigarda (t33), Fall of the Gavel on Intrepid Adversary
(t31), Sphinx's Revelation + Elixir, then a hard-cast Emrakul (t41). The seat had the opponent
at **6 life on t21** (G3 seq 61) and never got another creature to stick. Two real items:
(a) its only remaining mana sink, Lair of the Hydra, is bricked at 1/1 (HIGH-2) — at seq 115
it held 13 mana and the opponent was at 19; (b) it animated the Lair in **Main phase 2** three
times (seq 74, 80, 115) for 6 mana and zero damage, against its own guide's "animate it in
MAIN 1 only" line, which it even recites inside the seq-115 reply. The Emrakul annihilator
choosers (seq 120-125, 131-136) rendered well and were answered sensibly.

**G4 vs deck130 — WIN 17 to 0 (t11).** Clean curve: Katilda t3, Briarbridge Tracker t5, Clue
cracked in upkeep t7 (seq 11), Luminarch Aspirant t7, alpha strikes seq 24 and seq 38. No
misplays found.

**G5 vs deck123 — WIN 20 to -2 (t9).** Two Luminarch Aspirants plus Elite Spellbinder, targeted
counters onto the attackers each combat (seq 10, 13, 14, 18, 19), three attacks (seq 11, 15, 20).
Textbook execution of #2 RULE.

**G6 vs deck146 — WIN 9 to -2 (t24).** Ranger Class into Tracker into Tovolar's Huntmaster;
Teferi resolved (seq 31) and its +1/-2 were used correctly (seq 32-37, 43, 66); Brutal Cathar
exiled twice. The one loose decision is seq 16 "Play no land right now" at t12 — the only land
offered was one that would have been useful later, and the seat played Overgrown Farmland six
windows later anyway (seq 18), so the drop was not lost. LOW.

## 2. Engine / interface / card items

**HIGH-1 — Conditional "enters tapped" lands are rendered with a FALSE unconditional verdict.**
Repro: G1 seq 4, t3, one other land on the battlefield, the land-drop row reads
`1. Play Lair of the Hydra [enters TAPPED - it makes no mana this turn: "If you control two or
more other lands, Lair of the Hydra enters tapped."]`. The primitive is
`auto=if compare(type:land:myBattlefield)~morethan~2 then tap(noevent)`
(borderline.txt:64291) — with two lands total it does NOT enter tapped, and the very next
window (G1 seq 5/6) shows it as an untapped source paying for Ranger Class.
Same defect with the opposite polarity on Overgrown Farmland
(`auto=if type(other land|mybattlefield)~lessthan~2 then tap(noevent)`, borderline.txt:81143):
G3 seq 12, t7, three other lands, row reads "enters TAPPED - it makes no mana this turn"; the
log at G3 seq 13 shows "You played Overgrown Farmland" and the same turn's
"Mana available: 5 total ... Overgrown Farmland {g} or {w}". The render prints the ETB sentence
without evaluating its condition against the board. Corpus-wide: **30 provably false rows of the
172 rendered enters-TAPPED land rows** (11 of them in deck152's own seats: G1 seq 2/4,
G2 seq 16/22/27, G3 seq 12/85, G6 seq 12/18/22/29); Hive of the Eye Tyrant and Deserted Beach
show the same shape in other seats. Cost: the seat under-counts its own mana at the exact
window where it is deciding what it can cast.

**HIGH-2 — Lair of the Hydra only ever offers the 1/1 rung, while printing X/X card text.**
The primitive defines twenty separate rungs, `{1}{G}` 1/1 through `{20}{G}` 20/20
(borderline.txt:64293-64312). Across the ENTIRE corpus the animate row is rendered
**387 times and every single one says "becomes a 1/1 hydra" `[cost: {1}{g}]`** — no 2/2..20/20
row ever appears, at any mana total. Repro: G3 seq 115, t39 Main 2, `Mana available: 13 total`,
opponent at 19, menu row 1 = `becomes a 1/1 hydra with Lair of the Hydra [cost: {1}{g}]`, whose
own printed card text says `{X}{G}: ... becomes an X/X green Hydra creature`. The row and its
card text contradict each other and the model spent the whole decision on it (see MED-1). For a
deck whose late game is "flood out and animate the manland", this deletes the only win
condition; G3 was 20 turns of exactly that.

**HIGH-3 — Repeat-payment modal asks price nothing, so they silently tap creatures out of combat.**
Repro: G2 seq 24, t11 Main 1. The ask is `Choose an option for Intrepid Adversary:` with rows
"add 1 counter" .. "add 20 counters" and a NOTE about per-counter charging, but **no mana line,
no total, and no `{paying this taps: <names> - they cannot attack this turn}` clause** — even
though answering "add 3 counters" spent 6 more mana and consumed the seat's last two sources,
which were Elite Spellbinder and Katilda (Katilda grants every Human "{T}: add mana",
borderline.txt:61435-61439; Elite Spellbinder is a Human Cleric, borderline.txt:33387). The cast
row that opens the modal (G2 seq 23 row 2) is priced only at the BASE cost {1}{W}, so its
clause is correctly absent — the whole extra bill is invisible. The engine already emits this
clause elsewhere (48 times corpus-wide; G2 seq 21 row 1 prints both
`{paying this taps: Elite Spellbinder - it cannot attack this turn}` and
`{tapping Katilda, Dawnhart Prime now: it cannot attack this turn}`), and the guide's #2a RULE
is built entirely on reading it. Result: the guide's stop cannot fire, and t11's whole combat
step vanished. Fix shape: carry the running "mana left / paying this taps" accounting into the
modal repeat-payment ask, and add the projected total to the cast row that opens it.

**MED-1 — An over-long PLAN line is accepted, truncated, and then quoted back for many windows.**
Repro: G3 seq 115. The reply protocol says "then a PLAN: line ... Write nothing else"; the model
wrote ~3 KB of deliberation into the PLAN and the record ends
`[+1106 bytes written past the PLAN line trimmed from this record]`. That text then became the
seat's plan: G3 seq 120 and 121 (five and six windows later) quote back
`YOUR PLAN (as you last stated it, 5 windows ago on turn 39): ... Actually, we have 12 mana. We
cast the 1/1 Hydra for {1}{G}. We have 10 mana left. We can animate it again? [...the rest of
your plan was not carried]`. A byte cap on the PLAN carried forward, or a "your PLAN was too
long to keep" notice, would stop a stuck seat poisoning its own later prompts.

**MED-2 — "Night" is offered as a sacrificeable permanent and silently absorbs one sacrifice.**
Repro: G3 seq 120, the Emrakul annihilator-6 chooser, option 15:
`Night [your battlefield] - "As it becomes night, transform all daybound permanents. ..."`.
It is NOT in the battlefield line printed three lines above it (16 permanents, Night absent).
The seat picked it at G3 seq 135, and the resolution log (G3 seq 137, turn 43) shows six targets
named — "Plains #1, Lair of the Hydra, Hengegate Pathway #1, Plains #2, Night, Overgrown
Farmland" — but only **five** "was put into your graveyard" lines. The day/night designation is
not a permanent; offering it makes annihilator under-count and puts a phantom object on a
forced-sacrifice list.

**LOW-1 — Modal counter menus enumerate 20 rows regardless of payable mana.**
G2 seq 23 row 2 prints `{modes live right now: don't add any counter, add 1 counter, ... add 20
counters}` and G2 seq 24 lists all 21 rows with 8 mana on the table. Wasteful, and the NOTE has
to spend a paragraph explaining that over-asking is safe. Capping the list at what the mana can
actually buy (and saying so) would be shorter and truer.

## 3. Guide verdict: KEEP as is

deck152_strategy.txt (67,114 bytes, inside the 41-71 KB band) is not what lost these games.
- Two of the three losses are not guide-addressable: G1 is a two-land, wrong-colour draw with
  no third land in seventeen turns; G3 is a control deck resolving Essence Scatter, two Supreme
  Verdicts, Path to Exile, Fall of the Gavel and Emrakul.
- The one loss with a decision in it (G2 t11) was caused by a MISSING render clause, not a
  missing rule: **#2a RULE already tells the seat to read `{paying this taps: ...}` and pass**,
  and the clause was absent from the ask that did the tapping (HIGH-3). The seat followed the
  guide and still lost the combat. Writing more guide text against an invisible cost cannot help.
- The one clean strategy violation — animating the Lair in Main 2 (G3 seq 74, 80, 115) — is
  against a rule the guide ALREADY states ("animate it in MAIN 1 only") and which the model
  **recites verbatim inside the offending reply** at seq 115 ("the rule says 'animated in Main 1
  only... with spare mana only'. We are in Main 2"). Restating a rule the model quotes while
  breaking it is noise; and with HIGH-2 open the Lair line is worth ~nothing either way.
- The guide's live rules were followed and paid: 0 fallbacks in 311 decisions; the Pathway
  colour-face rule was exercised correctly (G1 seq 3, G3 seq 5, G5 seq 5); the land drop was
  taken every time it was offered and playable; #3 DEPLOY FLOOR case (c) held (no second-Katilda
  legend-rule cast was taken under a healthy copy); the three wins (G4, G5, G6) are straight
  executions of #2 RULE and the Teferi section.

No general-guide proposals and no strategy-writing-skill proposals: every item above is engine
or render, and none of them is a gap in how strategy guides are written.

## 4. What I did NOT check
- Only deck152's six seat files were read in full; opponent seats were used only through the
  shared game log embedded in deck152's prompts, so mirrored render defects in other seats are
  unverified except where I counted them corpus-wide (the enters-TAPPED and hydra-rung censuses).
- No lane-{R,S,T,U,V} prediction was adjudicated — that is the engine seat's job.
- No Scryfall cross-check was made; all card facts here are read off the primitives cited by
  file and line, which is what the engine and the model actually use.
- I did not read src/ to locate the code producing HIGH-1/2/3 — the items are stated as
  observable render defects with repros, not as diagnosed code paths.
- Latency/stale-drop/transport counters were not audited beyond the seat's own fallback count.
- Mulligan quality was not second-guessed beyond checking that the kept hands' land counts match
  what RULE -1 prescribes; hands are real and I did not model the shuffles.
