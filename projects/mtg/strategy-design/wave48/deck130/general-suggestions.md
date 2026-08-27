# deck130 - core-prompt / render proposals (wave 48), layer-routed

Engine BUGS with repros are in `seats/seat-123-130.md`. These are render / core-prompt
proposals; every one adds a true token and deletes nothing.

**R1 (HIGH, render) - DRAW FORECAST, including per-SPELL forced draws.** Shared with the deck123
file (R1 there). deck130's instance: `1787833213-ai_baka_deck130-0x55ce8937fa00-vs-ai_baka_
deck162.jsonl` seq 23 - `Forced Fruition {4}{u}{u} [enchantment] {effect: "Whenever an opponent
casts a spell, that player draws seven cards."}` on their line, `Every card YOU draw costs you 2
life` printed, the pilot's plan *"causing THEM to draw 7"*, and `Cast Rorix Bladewing` at 1
life. The punisher line prices a draw; it does not say that casting a spell IS seven draws here.
Proposed line: `DRAW FORECAST: your next draw step draws K cards (...) = K x N life; each spell
you cast draws 7 (Forced Fruition) = 14 life.` The `{effect:}` text is on the screen already;
the forecast is the arithmetic the pilot got wrong.

**R2 (MED, core prompt) - retire the carried plan when it names no option** (= deck123 R3;
the 48-window echo is this seat's, `...deck130-0x5589601133c0-vs-...deck125.jsonl` seq 84-131).

**R3 (MED, render) - the blockers header's `M more able to attack` must exclude attackers**
(= seat file ENGINE M1; listed here because it is lane P's own string): `2 of them are attacking
right now, 3 more able to attack right now` on a line with two non-attacking creatures, the third
being a `[vigilance] [attacking]` body (`...deck123-0x5615ef06def0-vs-...deck152.jsonl` seq 26).

**R4 (LOW, render) - ATTACK grammar tolerance is right; consider a receipt.** `ATTACK: A1,
A2-A4, A5-A7` (vs152 seq 24) named three labels that did not exist and the engine took the four
real ones silently. Correct legality behaviour; a one-line narration receipt (`you named A5-A7,
which are not on the list - ignored`) would let the pilot's next plan stop counting eight
attackers it does not have (its seq 24 plan counted "2 Siege-Gang Commanders and 6 Goblins").

**R5 (LOW, render) - `Mana available: 11 total (untapped sources, ...` still drops the number
word above ten** - 85 emissions at this pair, all at deck130 vs125 turns 20+ (carried wave-46
L1 / wave-47 L2).
