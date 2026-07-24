# deck59 — engine / representation / data notes (wave 24)

## ROTATION VERDICT
NEWCOMER (guideless Step-0, first pass) → does NOT rotate. An initial guide was
written this wave (new-work signal by construction). Re-validate next corpus (Step-0-ter),
focused on: did the hybrid-mana tax/veto collapse, and did the pilot start sweeping to
stabilize when behind (the loss lever).

## Representation — persist is well-surfaced (NO fix needed)
Own persist creatures render fully:
`Kitchen Finks {1}{g/w}{g/w} (2/1) (printed 3/2) [persist] [counters: 1x -1/-1]`
i.e. current P/T + printed P/T + `[persist]` tag + explicit `[counters: 1x -1/-1]`. The
pilot can see reduced stats AND that a body already carries a -1/-1 (so it will not
persist again). Cast options also carry the correct hybrid annotation
`[hybrid: each {g/w} pays with G or W - total N mana]`. Do not re-diagnose either as a
gap in a future wave.

## Minor representation items (low priority, not guide-fixable)
1. **Persist RETURN is not narrated.** When a persist creature dies and returns it appears
   in `events` only as a generic `Your <Card>: battlefield -> graveyard` then `Your
   <Card>: stack -> battlefield`, with no "Persist triggers / returns with a -1/-1
   counter" line. The death→return is not linked, so a log-tail skim can read a dead
   creature reappearing as anomalous. Repro: any game with a combat/sweeper death of a
   counter-free persist creature (e.g. deck109 file, Safehold Elite around T7-T13).
2. **Block-outcome annotations ignore persist.** Lines like `Kitchen Finks (3/2) [persist]
   - may block A1 (both die)` say "both die" without noting the persist creature returns —
   understating the value of chump/trade blocks for THIS deck. A `(both die - yours
   persists back)` variant would be more accurate. Repro: deck133 file blockers records
   (s7/s12/s17/s33), deck109 s14/s18.

## Data-integrity observation (flag to synthesis)
3 of deck59's 6 games (vs deck137 Selesnya Adventures, deck27 Undead Infiltrator, deck133
Phyrexian Asphodel) have **NO `gameend` record in EITHER perspective log**. The other 3
(deck131, deck109, deck102) do. Per the brief these games completed (win table lists 59:
3/6; the two timeout-draws are 27v137 and 102v133, neither a deck59 seat). Either gameend
is not always written, or the run was cut before these three resolved. Not a pilot issue;
flagging so a future reviewer does not read the missing gameend as a deck59 defect.

## Card-script / Oracle cross-checks (all engine-faithful; no action)
All ten cards match their real Oracle text as rendered (persist wording, wither, fear,
Kitchen Finks ETB lifegain, Heartmender upkeep -1/-1 removal, the three sweepers). Costs
diverge from paper only in the expected hybrid-notation way (engine writes {G/W}, {B/G},
{W/B}; the deck's lands make them all castable off W/B). No card-script bug found this
seat. Heartmender's `auto=@each my upkeep:all(creature|mybattlefield) counter(-1/-1,-1)`
correctly removes a -1/-1 from EACH of your creatures (verified via the persisted-body
P/T recovering across upkeeps).

## Model/latency
No hard fallbacks at deck59 (1 benign `choice_source: prose` parse, deck131 s18 blockers).
Reply-length median 1,220 chars but 33 giants >4,000 / 16 >6,000, driven by the hybrid-mana
re-derivation and losing-position spirals; latency p90 ~90s, max 216s. The guide's rule #1
should collapse the hybrid share of this tax — measure giant-count next corpus.
