# deck123 → synthesis: TWO render / core-prompt proposals (wave 62)

Both are new emitters, not restatements. Deliberately NOT filed: the answer-line-before-reasoning
seam (my review's HIGH-2, a fresh game-losing instance at `146v123` s18) — that is ledger D9 and
wave-59 deck123 P1 already, and re-proposing it is the busywork pattern.

## P1 — D2 fixed `attackTotalLine` under an opposing life LOOP. The BLOCKERS emitters were not fixed, and they still print the bounded ceiling D2 removed.

**The seam.** Lane X gave `attackTotalLine` the `oppLifeLoop && blockGain > 0` fail-closed branch:
with both halves of a life loop on their side, the bounded `blockGain` figure is withdrawn along
with the kill claim it fed. Two other emitters compute the same bounded quantity and were left
alone: the blockers header's `best case with every blocker assigned: you would be at N - one legal
assignment that reaches it: …`, and the per-B-row clause
`{their attacker's lifelink, this block: they gain N from this block only, and this attacker deals
nothing to your life}`.

**What it cost, with seqs** (corpus `matchups-20260905-133058`, binary `12be0b79c`,
`1788633085-ai_baka_deck123-0x557b7b88b130-vs-ai_baka_deck126.jsonl`):

- `s76` (t15, `kind: blockers`), seat at 16 life, one 1/1 lifelink Vampire attacking, 83 blockers
  available, Sanguine Bond **and** Exquisite Blood on the opponent's battlefield line. The screen
  printed, in this order: `INCOMING THIS COMBAT: 1 attacker, 1 unblocked damage - you would be at
  15 … - best case with every blocker assigned: you would be at 16 - one legal assignment that
  reaches it: Thraben Doomsayer #1 blocks Vampire #1`; then
  `Your life: 16. Unblocked, these attackers deal up to 1 … - NOT lethal: block only where the
  trade favors you.`; then every B row as `(you kill it, your blocker lives) {their attacker's
  lifelink, this block: they gain 1 from this block only, …}`; then the converter banner, which
  says the opposite: *"ANY nonzero payment on a tag above is fatal, not merely expensive."*
- The seat took the block the header named. `s78` events are eleven
  `You lost 1 life` / `Opponent gained 1 life` pairs: **16 → 0**, them 24 → 40, `s79 gameend`.
  `BLOCKS: none` ends the combat at 15.

The word **"only"** in the row clause and the `best case` of 16 are the two false halves. Primitive
check: Sanguine Bond `mtg.txt:99005 @lifeof(player) from(*[-lifefaker]|*):life:-thatmuch opponent`,
Exquisite Blood `mtg.txt:37967 @lifelostfoeof(player):life:thatmuch controller` — the chain is
engine-real and the log above is it running.

**Ask.** Give both blockers emitters the same `oppLifeLoop` input `attackTotalLine` already takes,
from the same `playerHasLifeLoop()` scan the banner is built from, so header, row and banner cannot
disagree on one screen:
1. On a row whose block grants them any life while `oppLifeLoop` holds, replace
   `they gain N from this block only` with the loop's verdict — the gain is not bounded at N and
   the block is fatal — and suppress or qualify the row's favourable `(you kill it, your blocker
   lives)` fight result, which is true about deaths and silent about life.
2. Exclude every such block from the `best case with every blocker assigned` search, so the line
   cannot name a fatal assignment as the best case. If that leaves no assignment better than
   declining, the line should say so rather than print the seat's current life.

**Scope, honestly.** 1 of 21 blockers windows this corpus, and 1 of 1 in which a life loop was on
the opposing side — rare, decisive, and the mirror of a fix this wave already shipped.

**Prediction (falsifiable).** In a wave-63 game where a lifelink or lifegain-on-block attacker
attacks a seat under an opposing Sanguine Bond + Exquisite Blood pair, no `{… they gain N from this
block only …}` clause renders, and the `best case with every blocker assigned` figure is at or
below the unblocked-damage figure on the same line. Count `from this block only` occurrences under
a printed `life LOOP`: expected 0 of N.

## P2 — `mana of ANY colour pays a generic cost like {2}` renders on 1922 of 1922 nonzero-mana lines and 0 of 177 zero-mana lines.

**The seam.** The clarifier is appended inside the parenthetical that the `Mana available:` emitter
builds from the colour breakdown, so a board with no untapped sources renders the bare form
`Mana available: 0 total (no untapped sources)` and the sentence disappears. Counted over every
`^Mana available:` line in the corpus: nonzero **1922 with / 1922 total**; zero **0 with / 177
total**.

**What it cost, with seqs**
(`1788633083-ai_baka_deck123-0x5647178d3220-vs-ai_baka_deck130.jsonl`, Ponza having destroyed the
seat's lands — `s26` battlefield line reads `1 permanent listed, … 0 are lands`):

- `s26` (t33): `Mana available: 0 total (no untapped sources)`. No clause.
- `s27` (t33), the next window: the reply spends 7,680 bytes deriving that its deck cannot
  function — *"{2} is generic. It must be paid with colorless mana. Colored mana cannot be used for
  generic costs … So if I have no colorless sources, I cannot cast spells with {2} … This deck is
  effectively dead"* — and explicitly re-derives and rejects the true rule: *"'Any color can pay a
  generic cost' is a common misconception."* `post_answer_overrun 10799`, `reply_trimmed_bytes
  3197`, `latency_ms 624468`.
- `s28` (t35), back to one untapped source: the clause is present again.

Three other seats reason from the same false rule in this corpus (`126v125` s73, `126v146` s58,
`152v162` s7), so it is not one seat's quirk — it is a rule the model does not reliably hold and
the prompt normally supplies.

**Ask.** Emit the generic-cost clarifier unconditionally on the `Mana available:` line, including
the zero case (`Mana available: 0 total (no untapped sources; mana of ANY colour pays a generic
cost like {2})`). Roughly 50 bytes, on the 177 screens where a seat with no mana is most likely to
conclude its own deck is uncastable — and it is the screen immediately before the fetch/land
decision that would fix the mana.

**Prediction (falsifiable).** In wave 63, `Mana available: 0 total` lines carrying the clause:
expected N of N (currently 0 of 177). And zero replies corpus-wide containing both "generic" and a
claim that coloured mana cannot pay it (currently 4).
