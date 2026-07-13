# Wave-4 deck131 — working notes (catch-all)

Everything useful that doesn't live in findings.md / strategy.txt / skill.md /
general-suggestions.md. Organized by audience.

## For the deck builder (deck131 rebuild lever)

- **The manabase contradicts the win condition.** 14 Island / 8 Mountain, but every card
  that wins (3 Guttersnipe {2}{R}, 3 Young Pyromancer {1}{R}) is red. The durdle losses
  are downstream of this one number. Cheapest fix: swap ~4 Islands for Mountains (10M/12I)
  — the blue spells are mostly single-{U} costs (Downsize {U}, Rift {1}{U}, Epiphany
  {2}{U}, Aetherize {3}{U}); only Counterspell {U}{U}, Dissipate {1}{U}{U}, Opportunity
  {4}{U}{U} are blue-hungry, and they're support.
- **Threat density is 6/60.** The guide now teaches the deck to play its threatless hands
  coherently, but 2/12 wins is close to the deck's structural ceiling. If a rebuild is on
  the table: Spellheart Chimera (already in the #SB, perfectly on-theme — power scales
  with instants/sorceries in graveyard) for 2 of the 4 Aetherize; 4 Prism Ring is the
  other obvious flex slot (it gains life and nothing else — it appeared in the durdle
  losses as mana-sink filler).
- **Against lifegain (deck140) this deck cannot win at any pilot quality** — a 2-damage
  clock vs +10/turn. Fine if intended (deck tiers are a product feature), but both vs140
  games will timeout every run and burn ~35 turns of wall clock each.

## For the engine / harness owner

- **Rules-facts audit of the core prompt is worth a pass.** G1 (first strike vs the
  "simultaneous damage" line) is one instance of a class: the prompt states absolute
  rules facts that keywords in the card pool break. Others to check before they bite:
  DEATHTOUCH breaks "dies if damage >= toughness" (any damage kills); TRAMPLE breaks
  chump-block math (the chump prevents almost nothing — and my proposed G3 chump guidance
  interacts with this); LIFELINK changes race arithmetic. Suggest auditing the rules
  paragraph against the distinct keyword set actually present in the demo deck pool, and
  a first-strike/deathtouch probe deck (per the proven probe pattern) to validate any
  adopted combat-facts text.
- **Translog gaps that still cost review time** (wave-3 #8 partially done — chosen_text,
  gameend, latency all landed and were load-bearing this wave; thank you):
  - Still no game-id/opponent/result header record; pairing seat files to results.tsv is
    epoch arithmetic (seat epoch = start_epoch+2..6) and needs a confirm-by-card-names
    pass when games start close together.
  - `options` is an INT (count), not the list — the offered actions must be regexed out
    of `prompt`. A structured options array would make offered-vs-taken scripts trivial.
  - TIMEOUT games write no `gameend` record, so end-of-game life/board for the two vs140
    games had to come from the last decision record. A timeout-adjudication record
    (final life totals, turn) would let reviewers score timeouts without reconstruction.
- **Corpus design:** 6 games/deck/run is fine for behavior review, far too thin for
  win-rate claims (see fleet rare-event A/B memory). Cross-wave metrics that ARE
  comparable at this n: draw-casts/game, attack-participation rate (creatures declared /
  creatures listed, per attackers ask), block-quality (blocks with named gain / total
  blocks), cast-nothing rate. Win-rate is additionally confounded across waves because
  every deck's guide changed simultaneously.

## For the next deck131 reviewer

- **Fast triage: sort losses by OPPONENT life at gameend.** Opp near 20 = the deck never
  had a plan running — go read the hand/draw stream before judging the pilot. Opp at
  5-12 = a real game was lost — go read the combat and targeting decisions. This one
  sort ordered my whole review.
- **Read every `blockers` record in full** — they are rare (~1 per game), cheap, and were
  the highest-signal records in the corpus. Sample `priority` records instead of reading
  them (numerous, mostly correct passes).
- **Before accusing a rule violation, grep the record's `prompt` for whether the option
  was OFFERED** — my "Island over Mountain" suspicion dissolved when the prompts showed
  no Mountain in hand until the turn it was played (ep1783922461).
- Digest script pattern that worked: one line per record filtered to
  attackers/blockers/casts/land-drops with turn + life totals — a readable game arc in
  ~30 lines/game. Then targeted greps (per-card offered-vs-taken, draw-cast counts)
  across the file set for the aggregate claims.

## For the model-experiments track (qwen35 as pilot)

- **Concrete named rules keep near-perfect compliance** (wave-3 #9 reconfirmed): Prism
  Ring->blue, Mountain-first, draw-on-your-main, cast-payoff-first were all executed. The
  leverage remains: convert judgment into named-card directives.
- **It sustains standing per-turn behaviors well**: "attack every turn" ran 8+ consecutive
  turns unprompted in both wins. Multi-turn PLAN pursuit is no longer the visible
  weakness it was in wave 3.
- **Current weakness profile**, in order: (1) subset answers to bundled asks (declares
  one attacker of four — a reply-format failure, not strategy); (2) reflex-blocking
  without reading the attacker's rules text (first strike, destroy riders); (3) no
  spontaneous rules-text lookup during combat math generally — it uses stats
  (power/toughness) unless the prompt/guide pushes the text at it.
- **Parse/infra reliability is now excellent**: 0-1 fallbacks per game across my 12 games
  (worst: 1 empty_reply in each 100+-decision timeout game). Reply-format experiments can
  assume the transport layer is clean.

## On the wave process

- The findings->guide->skill->general split worked; the one structural suggestion:
  reviewers should receive the RESULT PAIRING (seat file -> game -> outcome) as input
  rather than each re-deriving it — it's identical toil across all seven reviewers.
- The A/B discipline (deck131's draw-spell rewrite, measured 1->8, reconfirmed holding at
  ~1.0/game this wave) is the model to repeat: one variable, a countable behavior, and a
  recheck the following wave. My guide's headline change (blocking) has an equally
  countable metric ready: blocks-with-named-gain / total blocks, and attack-participation
  rate for the declare-all fix.
