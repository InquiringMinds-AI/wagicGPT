# deck137 -- development notes (engine/harness ledger) -- wave 28

Binary ad8930961, corpus matchups-20260726-050449.

## N-137-CONVOKE-LOXODON [RESOLVED -- verify-and-close] e1 fix confirmed live
The wave-27 CONVOKE-NONRESOLUTION defect is CLOSED for Venerated Loxodon (fixed-cost convoke).
Post-fix binary ad8930961: 0 Loxodon defers this corpus (wave-27 had 23). End-to-end proof:
  1785067119-ai_baka_deck137-vs-deck93.jsonl seq35 t10 -- model chooses "Cast Venerated Loxodon
  with its convoke cost", NO defer follows, narration `Venerated Loxodon: hand -> stack ->
  battlefield`, seq37 t11 it blocks as a live 4/4. Fixed-cost convoke resolves via BOTH convoke
  and plain paths (plain also resolved: vs146 seq27). Close this ledger item for the fixed-cost
  case; keep the March X-case open (below).

## N-137-MARCH-XCONVOKE [OPEN -- HIGH -- already ledgered as "March-via-convoke X-announcement gap"]
March of the Multitudes resolved 0 tokens / 17 cast attempts this corpus (wave-27: 0/17,
UNCHANGED). This is the deeper X-spell-convoke defect the ledger already tracks; recording
sightings only, per brief (do not re-derive).
NEW MECHANISM DETAIL worth adding to the ledger entry: the PLAIN X-cast fails too, not just
convoke. Trace: model picks `Cast March of the Multitudes {g}{w}{w}{x}` (plain) -> engine offers a
`Cast Card Normally` follow-up menu -> engine then RE-OFFERS the convoke variant -> model picks
`with its convoke cost` -> defer -> 0 tokens. Reproduced:
  vs18  (1785066936-...-vs-deck18):  seq21->22->23->24(defer)   ; seq27->30->31(defer)
  vs136 (1785070723-...-vs-deck136): seq30->31->32->33(defer)   ; seq36->37->38->39(defer)
  vs134 (1785069022-...-vs-deck134): seq21..44, 10 defers t4-t8 (re-pick loop, all convoke)
So the defect is the X-ANNOUNCEMENT + convoke interaction, NOT the convoke tap-payment alone --
the plain X-cast route lands back in the same broken convoke sub-path. The engine fix for March
must handle X-announcement floating remainder from convoke, not just re-run the Loxodon-style
fixed-cost fix. deck137 is the SOLE seat that exercises convoke -- keep it in pool as the only
observation post until this resolves.
IMPACT: March is deck137's go-wide + lifelink STABILIZE payoff. Its death is the mechanistic root
of both losses (vs18, vs136 -- own life fell while opp held; no lifelink flood to stabilize).
Fixing this is the single highest-value engine item for deck137's win-rate.

## N-137-LOXCONVOKE-SILENT-NOOP [WATCH -- LOW] silent convoke no-op with no defer
1785068831-...-vs-deck146.jsonl seq14 t4: model chooses "Cast Venerated Loxodon with its convoke
cost (4/4)"; NO defer record, NO battlefield arrival (board at seq20 t6 shows creatures:2, no
Loxodon). Loxodon re-cast PLAIN at seq27 t8 and resolved. Possibly a legitimate insufficient-
convoke decline (the offered option was uncastable at t4 with only ~2 creatures + few lands) that
the engine dropped silently rather than a defect -- but if buildCastSpell (c5) only offers
CASTABLE options, an offered-then-silently-dropped convoke cast is worth a look. LOW; watch for
recurrence. Distinct from the wave-27 defer shape (that one logged a defer; this one is silent).

## Corpus health at deck137 seat (clean)
0 unparsed_reply, 0 empty_reply, 0 retracted_choice, 0 stale_echo, 0 priority fallback across all
6 logs. All 16 fallbacks = deferred_to_heuristic, all = March convoke (N-137-MARCH-XCONVOKE).
Blockers: 4 records, all short+legal, 0 illegal gang-piles (wave-27 had 2). c4 parser gate PASS.
e4 land-identity tags render clean at the GW two-color seat, no prompt-noise.

## ROTATION (honest read)
deck137 does NOT rotate THIS wave: it holds a live HIGH engine item (N-137-MARCH-XCONVOKE) that
is observable ONLY here, plus a guide revision shipped this wave (soften obsolete prefer-plain
steering for the now-fixed Loxodon convoke). Comprehension has long converged and e1-Loxodon is
now closed here. NAMED EXIT PATH for the roster owner: once N-137-MARCH-XCONVOKE is fixed AND
validated at this seat (March creates tokens, closing-speed losses resolve), deck137's new-work
well is exhausted -- comprehension converged + closing-speed resolves to CONSTRUCTION (non-March
go-wide/lifelink redundancy is a deck-build lever, not a guide or engine one). At that point it is
a clean rotation candidate on the next convoke-clean corpus. Until then: KEEP.
