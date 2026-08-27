# deck125 - render / core-prompt proposals (wave 50), layer-routed

Engine items are in `wave50/seats/seat-125-126.md`; these are render/prompt items.

G1 (MED, render) - a creature cast row gets a converter tag when the opponent's line holds a
life-gain converter or the log holds an opposing edict. `deck125 vs126` seq 42: `Cast Emrakul, the
Aeons Torn {15} (15/15) {leaves 0 ...}` beside an opponent line printing `Exquisite Blood {4}{b}
[enchantment] {effect: "Whenever an opponent loses life, you gain that much life."}`; the guide's
rule was in the reply's plan and still lost to the row. Lane U's LOOP clause fires only with BOTH
halves out. Proposal: on cast rows for a creature, when the opponent controls Exquisite Blood /
Sanguine Bond or has resolved an edict this game, append `{their <name> turns this creature's
death into life for them}`; the rows already carry `{right now: ...}` facts for sweepers and
edicts, this is the same grammar on the other side of the table.

G2 (LOW, render) - `destroys 1 of their creature` (singular after "1 of their") on the Supreme
Verdict / Final Judgment row (`deck125 vs152` seq 13, `vs146` seq 46, `vs162` seq 16-20). "1 of
their creatures" or "their 1 creature".

G3 (LOW, render) - the `discard` ask's numbered list prints bare names (`1. Tribute to Hunger`,
`2. Tribute to Hunger`, ...) without cost or type; the bottoming ask prints `(land: taps for ...)`
and costs. Same list grammar on both (`deck125 vs123` seq 52: nine bare names, the seat had to
map "which is the Lightmine" from memory; `deck126 vs123` seq 7). Cheap and consistent with the
hand line.

G4 (INFO, prompt) - the Staff gate's re-derivation (W22) needs the cost of each counter in hand,
which the hand line already prints (`Essence Scatter {1}{u} [instant]`). No render change; noted
so the cost stays on the hand line.
