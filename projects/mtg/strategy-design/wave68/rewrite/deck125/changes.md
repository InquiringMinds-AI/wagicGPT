# deck125 guide rewrite — changes
## (a) Rules KEPT (48, grouped by decision kind)
LAND DROP (2): take one every turn, tapped or not, "Play no land right now" never; untapped row over
tapped, else the colour you have fewest of.
PLAN LINE (2): the one fixed sentence with no appended per-card verdict; a printed row outranks it.
TAP-OUT GATE (10): own-turn spends only; price = cheapest counterspell on the hand line; read M-K off
"{spends K of your M ...}" and ignore the card that clause names; Essence Scatter counts against
every opponent; a Revelation row taps you out unless it prints otherwise; four open shapes (sweeper
at K>=2, life<=5 lifegain, counter above the row's M, FIRST Staff with none in play, a SECOND Staff
gated); no counter in hand = open; floating mana is spendable but is never "open mana".
STAFF / PINGS (5): ping with every Staff in every window incl. upkeep; ping a named DRAW PUNISHER
until its row stops printing SURVIVES; otherwise only at 1 lethal damage, else the player; an Elixir
row at library<=16 outranks the ping; the second-copy tag means both copies work.
EMRAKUL (4): take a printed row (it outranks the gate); OFF under a converter line or "{their
converter:" row; their converter never converts your lifegain; with Emrakul out attack every window,
block unless "your blocker dies", decline a sweeper tailed "1 of yours".
COUNTERS (7): counter power>=2 creatures; let walls/[defender]/power-0 resolve at any life; counter
power 0-1 bodies with "{T}:"/"Whenever ... attacks"/"At the beginning of"/"venture"; land destruction
countered at <=5 of your lands and resolved at 6+; let lands, P/T-less artifacts, mana rocks and pure
draw/search resolve (being ahead is no exception); the counter-on-sight list, which outranks the
artifact line; count the rows and spend a LAST counter only on that list or a power-2+ creature.
PATH (3): decide at the CAST row, ordering lifelink-under-converter > repeating text > power with
flying breaking ties; never at an all-defender/power-0-1 list above 12 life (tapped is not safe);
life<=12 with a power-2+ candidate means Path the biggest, that window.
SWEEPERS (2): read the header's SECOND number K — K=0 is cast nothing and outranks every other
reason; K>=2 sweep, K=1 defender/small nothing, K=1 power2+ Path if held else power>=4 (or life<=10
with no other answer).
LIGHTMINE (2): cast when a counted creature's printed toughness <= N (N=0 never); survival override
when their attacking power >= your life.
PUNISHER BOARDS (4): draw-punisher line = no Revelation and no optional draw, Elixir instead;
discard-punisher line + CLEANUP PRICE tail = cast nothing, and the tail understates; Forced Fruition
(counter on the stack, cast nothing beside a punisher, library<=14 stop); counterspell carve-out at
K>=15 or any K>1 when the countered spell has its own draw text.
REVELATION / X (6): low-life floor (life<=6, no draw-punisher line) outranks everything; attacker-
power override; size floor max-affordable-X>=3 with the badge unable to override it; empty-menu
clause; stop drawing once their life <=10 with a Staff out; the ask's printed library ceiling beats
arithmetic and the row's "L-N left"; option-1 life bill names the surviving X or nothing.
ELIXIR (1): cast at a spare {1}; activate at life<=15 or library<=16, else gated.
WINDOWS / CLEANUP / MULLIGAN (5+): decline empty-stack windows; take the hold row under the "declined
N times" tag; cleanup send order 1-7 with the 1-3 tier and the land-before-counterspell absolute; tag
sorting incl. untagged-is-not-dead and the lone sweeper/Path exception; Liliana's Caress 2-life cost;
hand-over-seven spend rule; the four mulligan branches and the bottoming order.

## (b) DROPPED by category (estimates against the 71,499-byte live guide)
- Game/seq/corpus citations and "WHAT THIS COST" ledgers: ~18,000 B.
- Narration of past play and compliance counts: ~14,000 B.
- Re-statements of rules already given in their own section: ~9,000 B.
- Rationale/persuasion prose: ~7,500 B.
- Meta-commentary about the guide itself, its bands and its own reliability: ~2,500 B.
- Two rules cut for the 20,000 ceiling: the Tribute-to-Hunger / Devour-Flesh edict-refund carve-out
  on Emrakul (take only at their life<=15), and the Revelation full-hand hold. Both are narrow and
  are covered in outcome by the converter rule and the size floor.

## (c) Rules not restatable as condition->action
None. Every surviving rule keys on a string the prompt prints or a card fact in this deck.

## (d) Verification
- `stat -c %s strategy.txt` = **19978** (ceiling 20000).
- `/usr/bin/grep -ciE 'corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid
  (for )?by|[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]'` = **0**.
