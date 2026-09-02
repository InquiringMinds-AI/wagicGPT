# Wave-53 review carry-list (assembled 2026-09-02 at corpus launch)

Corpus: tenth fair-hand corpus. Binary = hermetic `make -B` of master after the eight wave-53
lane merges (M 614312289, O 0ba5ae2eb, N 3ab5c27c4, Q ba33a9adb, T 00549b391, S 6a4d5e119,
P 1057c7c9d, R 1bb08b2b1; merge head c628dd531 — gate numbers in the corpus-log/anchor). Model:
qwen36-35b-a3b on Spark :8084. Guides = wave-52 reviewer revisions (e68b7c7c6) + boundary pass
(2d83e169f). Same pool/recipe as wave 52 (-j 21, -T 0, WAGIC_GPT_TIMEOUT=900, MAXTOKENS=6000,
REPPENALTY=1.05).

## Discipline (owner doctrine 2026-08-26)
Reviewers own guides; synthesis owns skill + general; the core loop is the purpose. Adjudicate
render predictions by the emitter's actual string. Baselines from wave 52: 21/21 natural,
3,384 decisions, 15 fallbacks (6 reached Baka), max seat 353, max prompt 31,551, 464 `- Paid`
receipts / 0 no-source, 884 opponent-turn casting asks / 68 casts, 760/3,300 decisions in runs
of 3+ consecutive byte-identical declined menus (23.0%).

## Standing obligations from wave 52 (synthesis B-findings + skill #168/#177)
- #168 three-number literal audit per re-keyed literal: old-string renders (expect 0),
  new-string renders, takes under the new string. Literals that CHANGED this wave (lanes O/P):
  `[DRAW PRICE: ... - you would be at K; this KILLS you]`; `{kills 0 of the N legal targets at
  -1/-1}` / `{kills: Name}` on cast rows; `{paying this taps: <defender> - it cannot block on
  their turn}` (replaces `cannot attack this turn` for defenders); `- and their <name> takes N
  off YOU: life A -> B` on edict gain rows; `[second copy: legal, but its effect is already on
  the battlefield and a second copy changes nothing]` + the Lantern partial form; stack lines
  `ability: <source>'s <effect> (aimed at <victim>) [from your/their <source>]`; the edict
  such-clause `- a sacrifice is already on the stack aimed at this permanent...`; the HOLD row
  `Hold priority - do not ask me again this turn unless the board changes` (LAST row, opponent's
  turn only) and `[you declined this exact list N times already this turn]`; PLAN header
  `(as you last stated it, N windows ago on turn T)`. Re-key guide sentences to these (B4/B5/B9
  obligations name the guides: deck126 D69 -> D6 form; deck123 second-Alarm sentence -> D11 form;
  deck123 A52-12 -> HOLD row).
- #177 mechanical prose-vs-bullet diff across ALL SEVEN guides before scoring.
- deck130 mulligan section is #132-UNTESTED (0 ships in two corpora): report offers/ships/bottoms.
- Report opponent-turn declines and casts SEPARATELY at every seat, plus `hold_windows_skipped`.

## Set B - engine changes since the wave-52 corpus, each with its falsifiable prediction
- M (D1/D19/D23, blockers seam): `multiblock_reask` records with no follower and no
  `chosen_text` 0/N (was 1/3); every blockers ask followed in stderr by exactly one `declared
  blocks from N assignment(s)`; new note `multiblock_first_wins` where a conflicted reply held
  a legal pairing; `blocker_handle_in_attacker_slot` noted; BLOCKS format line carries the
  one-per-blocker clause.
- N (D2/D9/D12, window economy): opponent-turn casting windows per game down >= 30% with
  opponent-turn casts >= 90% of 68; End/Cleanup/Attackers/Combat-ends 0 casts; consecutive
  identical-declined runs < 10% of decisions (was 23.0%); FALSIFIER: `hold_windows_skipped`
  ~0 (row never taken) or casts < 90% with large skips (re-opener set too narrow). `CHOICE: n
  (Pass)` on a `0. Pass priority` menu executes row 0 N/N (note `pass_row_named`). PLAN blocks
  without an age stamp 0/N; `plan_choice_conflict` records with `coded_answers >= 2` carry
  `latched_coded_line`.
- O (D3/D5/D6/D13, pricing): `[DRAW PRICE:` rows with damage >= life carry `this KILLS you`
  N/N (was 0/41), casts from such rows 0; targeted-spell cast rows with knowable fixed damage /
  -N/-N carry a kill count or victim N/N (was 0/85), removal cast where every target survives
  0; defender-naming tap rows printing `cannot attack` 0/N (was 22), `cannot block on their
  turn` N/N; converter drain folded onto `they gain N` rows. CARRY: fight rows (`powerstrike
  eachother` arrives payload-stripped) — no forecast, not a failure of this lane.
- P (D4/D7/D8/D11/D14, render/state): bare `ability: <effect>` stack lines 0/N; edict rows
  naming a permanent already on the stack without the such-clause 0/N (was 2/3); no ask menu
  > 12 rows collapsing to <= 2 shapes, max non-log segment < 12 KB; `{castable from exile}`
  naming its source 17/17 (was 4/17); `[second copy:` takes on non-stacking scripts 0 (was
  4/14 Intruder Alarm), Lantern never called dead; battlefield header counts lands.
- Q (D10/D24, observability): empty replies at >= 95% of the deadline stamped `fallback:
  "timeout"` + `retry: 1` with the two stderr lines N/N; `empty_reply` within 1 s of the wall
  0 (was 3); Baka-executed opening keeps from a wall miss 0 (was 2); every `choice: -1` +
  fallback record followed by exactly one `kind: "recovery"` record; D20 declined on
  measurement (0 adjacent runs). NOTE for the harvest script: `recovery` is a NEW record kind.
- T (Baka blocking, Vita report): aggregate-lethal SURVIVAL sweep in `chooseBlockers`. Corpus
  exposure only through Baka fallback blocks — report any `blockers` fallback record's outcome;
  otherwise UNTESTED here (owner's Vita play is the test).
- S (hand-refresh perf, Vita report): human-seat only, no corpus exposure; the batch oracle is
  cross-checked by the suite. UNTESTED in corpus by construction.
- R (Go-Shintai `may pay({1})` interrupt fixed for `&&`-wrapped creature mana producers; the
  AI livelock on Dominating Vampire's ETB target is OPEN — investigation only): the pay-interrupt
  fix also affects Baka's `may` answers — report any `may`-prompt fallback. Dominating Vampire is
  not in the pool: UNTESTED.
- Wave-52 owner batches, Vita evolve, Baka pump-targeting fix (5ca11d6e2), option-preserving
  autotap (c1a0bf814, human-only): still UNTESTED in corpus — say so, do not infer.

## Open docket carried (not shipped this wave)
D15-D18, D21-D22, D25-D26 (LOW), D27 log-window (owner decision), D28 frontend (owner item);
NEW from lane T: replay drift — AI seats share `GameObserver::randomGenerator` with the shuffles
and are passive during replay (separate RNG per AI seat is the fix); NEW from lane S:
`kMaxOptSources` 14 makes the option-preserving autotap a no-op on 16+-producer boards (raise
or restructure so the owner's spec applies late); NEW from lane R: `planPayment`/`potentialMana`
cannot see `&&`-wrapped producers (the same blindness, out of lane scope), and the Dominating
Vampire livelock candidate (a `may` stranded by `testDestroy`'s `menuObject` guard while
`chooseTarget` declines to answer for a seat that is not `currentlyActing()`).
