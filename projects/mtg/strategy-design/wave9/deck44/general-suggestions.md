# General suggestions (core prompt / engine / harness) — deck44 wave 9

Frozen-spine seat. No CORE-PROMPT edit proposed (single seat cannot cut core; the counter tightening
is a per-deck guide change, not a core sentence). Engine/harness items below.

## Engine / representation

1. **The own-targets annotation WORKS — extend it to the ZERO-legal-target case.** ("the only legal
   targets are YOUR OWN right now" fired 47× on deck44's Go for the Throat, 0/47 self-target casts,
   and shortened the pilot's plans.) The one remaining edge: when a targeted-removal option has ZERO
   legal targets (the pilot holds no creature of its own AND the opponent has none), the annotation
   cannot fire and the option still renders BARE (`1. Cast Go for the Throat {1}{b}`), where the pilot
   still gets confused (repro vs131 s54: a 900+ char plan looping on "Young Pyromancer is an artifact?
   No... Wait..."). Add a `[no legal target]` marker (or suppress the option) for the empty-target
   case, mirroring the own-targets marker. This would let the guide's GFTT no-target fallback be fully
   demoted next cycle. Repro files: `1784046898-...-deck44` s51/s54 (own=False, GFTT bare).

2. **Block-seam has no "punisher / deals-0 / not-lethal" surface (carried, unresolved).** The wave-8
   Archmage-blocks-Ornithopter reflexive block did not RECUR this wave only because vs110 had no
   blocker-declaration phase — the representation gap is unfixed. Durable fix: annotate a blocker
   option with `[deals 0]` / `[not lethal]` / a punisher rider. Same family as the Phyrexian-Obliterator
   punisher-rider ask. (deck109/deck133/deck140 reviewers are the seats with live block windows.)

3. **Empty `[counters:]` bracket still renders** on Sleep-Cursed Faerie / permanents once counters
   reach 0 (carried from waves 5-8). Cosmetic; wastes tokens; suppress when the list is empty. Seen in
   vs110/vs131 GFTT prompts (`Sleep-Cursed Faerie {u} (3/3) [flying] [counters: 1x st...]`).

## Harness / data-quality

- **`gamestart` remains the win here** — my_deck/opp_deck let me map deck44 = deck0 and identify all 6
  opponents directly. The per-seat jsonl FILENAME still lacks the opponent token and start-epoch order
  still != game order, so game->file mapping still cross-references results.tsv life/turn. Adding the
  opponent token to the jsonl name removes the last manual step. (Repeated ask; low effort, high value
  for every reviewer.)
- **Reviewers must grep ALL `game-*.stderr`** (every seat), not just their deck, to catch migrated
  validation-drops — still true, class now near-zero (0 for deck44).

## Not a core edit — recorded for the synthesis agent
The counter-priority inversion (vs110: let Steel Overseer resolve, counter Galvanic Blast) is
addressed per-deck (guide category #2 urgency + value-floor face-burn carve-out). If a SECOND
affinity-matched seat shows the same "let the un-answerable growing artifact resolve" inversion, that
is a convergence candidate for a REPRESENTATION rider on artifact options ("[unremovable] [grows +1/+1
each turn]") rather than N per-deck guide edits — flag but do not promote on one seat.
