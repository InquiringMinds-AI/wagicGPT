# deck131 wave-5 -> core-prompt / general-strategy suggestions

Baseline: `strategy-design/wave4/general-strategy.txt` (composed into
`bin/Res/ai/gpt/system_prompt.txt`).

## Verdict: essentially PASS. No core-prompt change proposed from this deck's corpus.

Every general-layer feature exercised by deck131 performed correctly this wave:
- The COMBAT block (attack floor, per-creature tests, no-blockers survival check) drove 100%
  attacker participation and correct "no blockers" calls; nothing to add.
- The act-floor + anti-deferral held (draw spells cast on curve).
- The interface contract (offered = payable, lists complete) caused no misfires; no desyncs, no
  fallbacks in any of the 6 games.
- The mulligan land-count default was correctly specialized by the deck guide (red-source), not
  overridden by core.

Per the layer-routing epistemics (single-seat evidence cannot cut OR add a core line; burden of
proof is on core), nothing here clears that bar. Details below, each routed explicitly.

## Routed to PER-DECK (NOT core): the "reactions need a target" prior has a cast-triggers-payoff
exception.

The core prompt says (pure-reactions bullet): "never fire one [a counterspell/trick/removal that
cannot hit a player] at nothing just to spend mana." That prior is CORRECT deck-agnostically and
must stay. deck131 is the exception because Guttersnipe
(`auto=@movedTo(instant,sorcery|mystack):damage:2 opponent`) and Young Pyromancer turn the CAST
ITSELF into a payoff (2 damage / a token), so a "wasted" reaction is still 2 to the face. The
pilot obeyed the core prior and chose "Cast nothing" with Guttersnipe out at least 3 times
(vs44 seq 10 & 24; vs140 seq 22), leaving damage on the table.

Routing: this stays in the DECK GUIDE as an override-by-name (done in wave-5 strategy.txt: "the
general rule 'do not fire a reaction with no target' is OFF while Guttersnipe is on your
battlefield"). It does NOT go to core: only deck131 in the current pool runs cast-triggers-payoff
permanents (verified: none of decks 44/109/110/133/135/140 have a Guttersnipe/Young-Pyromancer
class card), so the failure is not deck-agnostic. If future decks add such permanents (Talrand,
Electrostatic Field, Firebrand Archer, Murmuring Mystic, etc.), reconsider a general line of the
form "a permanent that triggers off YOU casting a spell makes every cast valuable - the
no-target-reaction rule does not apply while it is out"; until then it is guide-local.

## Routed to ENGINE/REPRESENTATION (see notes.md, not core): mana-ability mislabel.

vs110 seq 8 offered, on a priority window, "Cast Card Normally with Mountain {card text: R}" -
a land's mana ability presented as a spell cast. This is the known deck110-class representation
defect (mana-ability activations labeled as casts), not a prompt-instruction problem; a core
sentence cannot fix a mislabeled option. Filed in notes.md as a persistent representation item.

## Routed to INFRA (see notes.md): the vs135 timeout was winning-but-slow, core-clean.

deck131 was AHEAD at the cap (opp 8, self 23), attacking every turn; 86 decisions / 53 priority
windows at ~9.5 s median hit the 2400 s cap. The core prompt correctly did NOT drive passivity
here - no anti-passivity change is warranted (it would regress against a non-problem). This is
decision-density x latency economics, filed as infra.
