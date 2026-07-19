# Wave-19 reviewer brief (shared context for all 7 deck agents + synthesis)

Fifth per-deck-rotation cycle. deck21 rotated out (second converged exemplar); deck27
(Zombies) entered GUIDELESS. deck14 (Deep Blue) plays its FIRST GUIDED corpus (wave-18 wrote
its initial guide). decks 35/62/49 play revised-guide round N per their wave-18 continuity;
110/135 continue on frozen guides.

Binary 72b05535d — LARGE delta since wave-18's f664539a3/ed7d579e4:
(a) **wave-19 engine batch (4becc83be)**: combat name tolerance (parseBlockAssignments
    name→label second pass; ineligible-only ATTACK→none), per-pairing TRADE OUTCOME
    annotations on block options ("(both die)" etc.), alt-cast pitch surfacing ("this exiles
    your ONLY eligible card, X"), salvageLoopedChoice (last well-formed CHOICE re-parsed).
(b) **GPT REVEAL SEAM (f4a2ea891)**: AIPlayerGPT now DRIVES interactive reveal/surveil via
    Player::decideReveal (one bundled ask, translog kind 'reveal'). LIVE in this corpus:
    18 reveal decisions, ALL at the deck135 seat. Scry is deliberately still on the aicode
    heuristic path.
(c) **suppression→annotation replacement (ed7d579e4)**: both effectBadOrGood suppressions
    are now warning ANNOTATIONS per the owner's binding ruling.
(d) **bug-grind + Oracle sweep (both waves)**: hundreds of card-script corrections landed,
    including the planeswalker-damage errata (burn can now target planeswalkers) — card
    behavior may differ from earlier corpora; treat "the card did something new" as likely
    CORRECT now (verify vs Oracle before flagging).

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260719-092058` — 21 games,
  round-robin over 110,135,62,49,35,14,27. Binary /tmp/wagic-72b05535d.
- Win table: 110:5/6, 49:5/6, 35:3/6, 62:2/6, 27:2/6, 135:2/6, 14:2/6.
  0 timeouts, 0 draws, 5 life-adjudicated at cap (adj), 0 segfaults (6th clean corpus).
- Fallbacks 2.4% (27/1146): 21 unparsed, 6 stale_echo, 0 defer, 0 empty.
  Per-seat: 135:9, 62:7, 14:4, 27:3, 35:2, 110:2, **49:0 (second consecutive zero)**.
- TARGET CHOICE framing shown on 449 decisions.
- Decision kinds: ask 725, attackers 157, priority 110, blockers 62, gamestart 42,
  gameend 32, reveal 18.

## VALIDATION FOCUS (wave-19 engine batch — every reviewer)

- **Combat name tolerance**: block-assignment fallbacks should have COLLAPSED at the seats
  that bled them (110/14/62 last wave). If your seat still discarded a block reply, get the
  seq — it's a new shape, not the old one.
- **TRADE OUTCOME annotations**: first corpus with "(both die)"-class annotations on block
  options. Did block quality move? Any spot where the annotation MISLED (wrong outcome
  printed)? seq required.
- **Pitch surfacing** (deck135 primarily): the Force-class "exiles your ONLY eligible card"
  warning — did the model stop blind-pitching its finisher?
- **salvageLoopedChoice**: unparsed 21 — how many were loop-salvaged vs lost? Characterize
  what still rambles.

## NEW SURFACE — REVEAL SEAM (deck135's reviewer owns this)

All 18 reveal decisions sit at deck135 (Glacial Revelation / snow package). For EACH:
compare `chosen_text` against the engine `events` that follow ("revealed X goes to
hand/graveyard/library") — the ⚠ UNVERIFIED residual is NON-CONTIGUOUS / skip-first partial
subset ordering (one suspect observation pre-corpus: model chose indices 2,3; engine moved
index 1). Any mismatch: seq + both sides verbatim. Also verdict the Azcanta-class question:
does the seam CLOSE the old optionone auto-decline item for reveal-to-hand cards, or do
fingerprints (reveal happened, nothing reached hand without a model decline) persist?

## KNOWN CORPUS-WIDE (do not re-diagnose)

- Scry is still heuristic (aicode) for GPT BY DESIGN — do not flag scry quality.
- The 5 adj games ended at the turn cap by life adjudication — check whether YOUR seat was
  durdling (0-attacker turns, no plan) in those, but the cap itself is not a bug.
- deck49's zero-fallback streak: quantify what it does RIGHT only if it teaches a portable
  class; no victory laps.

## Seat assignments

- **deck110 (5/6, frozen guide):** record jumped 3/6→5/6 — engine batch or matchups?
  Equip-churn standing watch (5th wave). 2 unparsed — characterize.
- **deck135 (2/6, frozen guide):** OWNS THE REVEAL SEAM CHECK (above). 9 fallbacks
  (worst seat) — the old giant-reply tax or a new shape? Pitch-surfacing validation.
  Record 3/6→2/6 with 2 of 3 losses adjudicated — durdle read.
- **deck62 (2/6, revised round 3):** Blanchwood recount-ban + A-list attacks — carryover
  check; 7 unparsed — same pocket as last wave (computed-P/T distrust)?
- **deck49 (5/6, revised guide):** 2/6→5/6 guided round 2 — which guide rules fired?
  Zero fallbacks again. Top-heavy-curve pre-flag: confirmed dead or still latent?
- **deck35 (3/6, revised guide):** pool-lead lost (4/6→3/6). FC1 chump-durdle stayed dead?
  0-attacker durdle signature at the adj games?
- **deck14 (2/6) — FIRST GUIDED (Deep Blue):** wave-18's initial guide validation — which
  taught classes fired/died? TEMPO countermand doctrine read. 4 unparsed.
- **deck27 (2/6) — GUIDELESS FIRST PASS (Zombies):** Step 0-bis doctrine. Recursion/grind
  read; write the initial guide (format-match a live guide). 3 unparsed — characterize.

## Doctrine (BINDING)

Win-rate is context; the unit of analysis is the DECISION with seq repros. Layer-routing
enforced. Single-seat items cannot add/cut core lines. NEVER propose hard-suppressing a
legal play on a strategy judgment (owner ruling — annotations only). Per-deck rotation:
state your seat's verdict explicitly in notes.md (no-guide-mod AND no-new-work-signal =
rotation candidate).

## Output contract (per deck agent, sequential, in strategy-design/wave19/deckN/)

findings.md -> strategy.txt (byte-identical if frozen, REVISED if warranted; newcomer 27:
the NEW initial guide) -> skill.md -> general-suggestions.md -> notes.md (engine items +
rotation verdict). CURRENT skill = strategy-design/wave18/strategy-writing-skill.md. Guided
seats read their wave-18 deckN continuity files.
