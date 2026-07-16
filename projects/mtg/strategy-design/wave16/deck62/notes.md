# deck62 notes — engine/harness/card-script items + rotation verdict (wave 16)

Seat: deck62 (Enchantresses), GUIDELESS first pass. 147 decisions across 6 games. Fallbacks at
this seat: **unparsed_reply 12, stale_echo 5** (~11.6% of decisions) — HIGH, and concentrated on
combat and aura-stat-math records (14 replies exceeded 8k chars). The driver is REASONING TAX on
card mechanics, not a parser defect; the new guide's capability table + "trust the printed [X/Y]"
should cut it materially. Re-measure the fallback rate at this seat next corpus as the guide's
primary validation signal.

## Engine / card-script items (NOT guide prose)

- **N1 (card behavior, verify-not-fix) — Fists of Ironwood can enchant the OPPONENT's creature and
  grant IT trample.** vs110 s4: with creatures:0, the only legal Fists target was the opponent's
  Ornithopter, so casting it gifted the enemy trample. Per Oracle this is CORRECT ("Enchant
  creature", any creature) — so this is a GUIDE-steering item (done: the guide says don't cast a
  pump aura when your only target is the enemy), NOT an engine bug. Logged per the
  verify-oracle-text discipline; no code change.

- **N2 (representation, tax driver — convergent with deck135 board-rung) — the pumped creature's
  final P/T is rendered on the battlefield line, yet qwen recomputes it from the aura stack every
  time.** vs21 s19 (won game): 10.4k chars re-deriving a printed [9/10] Birds. vs135 s20: 11.6k ->
  unparsed while recomputing a printed [7/7] Yavimaya. The board serialization is CORRECT; the
  model distrusts it. Candidate representation nudge (cross-seat, low priority): the option/board
  line could annotate a pumped creature as "[current: 9/10, includes all auras/anthems]" to signal
  the number is final. Held pending a second stat-stacking seat.

- **N3 (representation — [tapped] on own creature at own combat) — qwen agonized over whether its
  own [tapped] Birds could attack on its own turn** (vs21 s19). This is the known tapped/untap
  board-state confusion; the creature untaps at the start of its controller's turn but the snapshot
  showed [tapped]. If the snapshot is taken pre-untap-step it is stale for the pilot's mental model.
  Low priority, corroborates the board-representation ledger; the guide blunts it ("if the list
  offers a creature as an attacker, it can attack").

- **N4 (harness/tax) — attackers records unparse when the board is wide and stat-math is involved**
  (vs135 s20, vs17 s19, vs44 s29, all 11-13k chars -> unparsed_reply -> heuristic). Not a new parser
  bug — the model rambles past a parseable ATTACK: line under math load. The guide's "attack from
  the list, don't re-derive eligibility/stats" is the intended mitigation; measure whether these
  unparses drop next corpus. No harness change proposed.

## Rotation verdict
**NOT a rotation candidate — NEW GUIDE SHIPPED, must be VALIDATED next corpus.** deck62 entered
guideless at 2/6 and produced its INITIAL guide this wave (guide-mod = yes; new-work-signal = yes:
a full capability table + engine-sequencing rule + aura-targeting rule + attack floor). Per the
rotation rule (no-guide-mod AND no-new-work-signal = rotation candidate), deck62 fails BOTH gates
and stays in the pool. Its guide is a hypothesis: next corpus must check that the hallucinated-
removal class, the enchantress-mis-sequencing, and the combat-tax unparses all drop, and that the
voltron/attack lines fire without the 10k-char agonizing.
