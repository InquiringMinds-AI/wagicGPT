# deck123 - core-prompt / render proposals (wave 48), layer-routed

Engine BUGS with repros are in `seats/seat-123-130.md` (H1 repeat-N parsing, H2 auto-tap colour
choice, M1 vigilance double-count, M2 edict row). These are RENDER / CORE-PROMPT proposals.
Every item adds a true token and deletes nothing.

**R1 (HIGH, render) - a DRAW FORECAST line beside DRAW PUNISHERS.** The punisher line prices a
draw (`Every card YOU draw costs you 2 life`) but not the number of draws the next draw step
will force. `...deck123-0x55ca87a19a00-vs-...deck162.jsonl` seq 35 (turn 15 upkeep, 9 life): the
opponent's line held Teferi's Puzzle Box (`puts the cards in their hand on the bottom ... draws
that many` - 7 in hand), Howling Mine (+1), Dictate of Kruphix (+1) and two punishers; the draw
step drew 9 at 2 each and the game ended from 9 life. The pilot passed - correctly, nothing in
hand changed it - but the same information two turns earlier is a decision (hold cards in hand
lower? cast Skeins now while the price is 1?). At the deck130 seat the same class killed a game
through Forced Fruition (`...deck130-0x55ce8937fa00-vs-...deck162.jsonl` seq 23: `Whenever an
opponent casts a spell, that player draws seven cards` read as "them"; the pilot cast Rorix at 1
life). Proposal: when any draw punisher is on the board, append one line computed from the same
primitives the punisher line already scans: `DRAW FORECAST: your next draw step draws K cards
(1 + Howling Mine 1 + Dictate of Kruphix 1 + Teferi's Puzzle Box: your hand size 7) = K x N
life; each spell you cast draws 7 (Forced Fruition) = 14 life.` Falsifier for wave 49: 0 deaths
in a draw step whose K x N was at or above the pilot's life when a life-preserving action was
on the previous window's menu.

**R2 (MED, render) - the edict row's count** (= seat file M2, listed here because it is an
annotation, not a bug): `Cast Tribute to Hunger ... - legal targets right now: the opponent
{edict: they control N creatures - they choose which; at 0 this does nothing}`. Fourth corpus
of the guide failing on this count; the number is one field away on the same screen.

**R3 (MED, core prompt) - retire a carried plan that names no option.** `YOUR PLAN (as you last
stated it)` is re-shown until "part of yours is now done or false". A plan with no action in it
- "The game is lost ... passing is the only legal action" - is never done and never false, and
it rode 48 windows at deck130 (`...deck130-0x5589601133c0-vs-...deck125.jsonl` seq 84-131),
producing a self-targeted Hammer of Bogardan (seq 97) and a blank Starstorm (seq 111). Proposal:
drop the plan block (and re-ask for a PLAN) when the carried plan contains no card name and no
option verb from the current menu, or when it has been echoed verbatim K times (K = 5). This is
the pregame plan-suppression fix (wave 46) generalised to the in-game plan.

**R4 (LOW, render) - the cast row could print what the auto-tap leaves in COLOURS.** Companion
to H2 (which is the real fix): `{leaves 3 of your 6 untapped mana sources untapped - {w}{b}
only}`. The pilot cannot choose the tap order; it can at least sequence casts around the result.

**R5 (LOW, render) - `[DRAW PRICE:]` verb agreement** with one punisher name (`Fate Unraveler
punish` -> `punishes`). Cosmetic.
