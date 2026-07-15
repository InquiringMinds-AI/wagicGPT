# Strategy-writing-skill proposals — deck109 wave-10

Two method points, both grounded in the deck131 seq18/19/21 hybrid-mana finding. Neither is a headline
rewrite; both refine existing rungs.

## Proposal 1 (ADOPT candidate — refines the offered=payable / representation-beats-instruction ladder):
**When the pilot fabricates a can't-pay belief over an OFFERED cast, check whether the SPECIFIC
card-cost fact is taught anywhere the pilot reads BEFORE concluding "representation, not wording."**

The wave-5/8 doctrine says: when the pilot reads a correct fact and disobeys, don't escalate wording —
route to representation. deck109 seq21 looks exactly like that wall: the pilot *quotes* the core
`offered=payable` contract and still fails. But the deciding card fact — `{B/R}` is HYBRID, red pays it —
was **not surfaced anywhere the pilot reads correctly**: the option shows `{b/r}` (which the pilot
MISreads as `{b}{r}`), and the deck guide had no cost note for that card. So the GUIDE rung was UNTRIED.
The generic interface contract ("if listed, it's payable") does NOT inoculate against a specific
mechanic misread — the pilot can hold the contract as true AND hold the mechanic-belief as true and
spiral on the contradiction rather than resolve it.

Authoring test (extends the confirmed-fired ladder): before routing an offered=payable / can't-pay
fabrication to representation or core, ask **"is the SPECIFIC card-cost fact taught where the pilot reads
it?"** If the guide teaches the parallel fact for a SIBLING card and the pilot OBEYS it (deck109 casts
Boros Reckoner `{R/W}` every window because the guide says "three Mountains cast it"), the cheapest
correct move is to add the same card-specific fact for the misfiring card — that attacks the ROOT
false belief before it forms. Only after the guide rung is tried-and-insufficient does the option-line
annotation (representation) or a core sub-clause become the next rung. Grounded: deck109 seq18/19/21;
sibling precedent Boros Reckoner obeyed 100% this corpus.

## Proposal 2 (ADOPT candidate — refines the max_tokens/truncation diagnostic):
**Distinguish a SPIRAL/LOOP truncation from a LENGTH-CUT truncation — they have different owners.**

The corpus-wide `unparsed_reply` note treats all cap-hits as "reply cut at 2048, self-healing." deck109
shows two mechanically distinct causes: (a) deck140 s16 / deck135 s7 = a genuine deliberation cut at the
token wall, self-healing (heuristic matched, or next window recovered); (b) deck131 s21 = a **reasoning
LOOP** — the model re-states the same contradiction a dozen times ("Maybe the option is a trick? ...
Perhaps the Mana available line is misleading? ...") and burns the whole budget without progressing. A
bigger token cap fixes (a) but NOT (b) — a loop fills any budget. The reviewer signal: grep the
truncated reply for repeated near-identical sentences / self-questioning ("Maybe ... Or maybe ... But
wait"); a loop indicates a false-belief-vs-contract contradiction to fix at the SOURCE, whereas a clean
cut indicates a length knob. Grounded: deck131 s21 (loop) vs deck140 s16 (clean cut) same corpus.

## Not proposed
- No change to the pause-aware demotion terminus: it worked as written (high-life gang-block class
  OBSERVED OBEYED 2/2 -> eligible to consider, but held at full prominence on thin sample + untested
  sibling branches; Obliterator absolute KEPT on zero windows).
- No change to the reply-protocol layer doctrine: the ATTACK:-last protocol killed the wave-9 non-numeric
  `none` intent-collapse cleanly (30/30) — the fix worked; nothing to add to the method.
