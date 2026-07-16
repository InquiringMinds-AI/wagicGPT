# Wave-15 reviewer brief (shared context for all 7 deck agents + synthesis)

First cycle under the PER-DECK rotation rule: deck109 and deck131 rotated out after wave-14
validated their seats clean; deck17 (Faeries) and deck21 (Goblin Gang) entered GUIDELESS.
This corpus is the first with: the target-cancel use-after-free fix (aad32bc7f), the
absent-echo staleness route + attackers name reconcile (d0f88326b), and the Galvanic Blast
metalcraft fix (3cb901295) — all already validated at wave-14; treat them as working
infrastructure unless you find counter-evidence.

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-040138` — 21 games,
  round-robin over decks 44,110,133,135,140,17,21. Binary /tmp/wagic-aad32bc7f. Wave-11
  guides for the 5 veterans (frozen); decks 17/21 played with NO guide (core prompt only).
- Win table: 110:5/6, 140:5/6, 17:3/6, 21:3/6, 135:2/6, 133:2/6, 44:1/6. 0 raw timeouts,
  9 life-adjudicated. 0 segfaults (the aad32bc7f crash fix survived live).
- Latency regime unchanged (Spark @0.75): med 11.4s / p90 93s — adjudications are a latency
  artifact, evaluate adjudicated POSITIONS not the adjudication count.

## KNOWN CORPUS-WIDE (do not re-diagnose)

- Fallbacks 5%: 47 unparsed (reasoning rambles, guard working), 15 stale_echo (the absent-echo
  staleness route — wave-14 inspection showed these are CORRECT catches of menu confusion;
  spot-check your seat's instances, flag only a WRONGFUL downgrade: an echo that DID name an
  offered option yet got routed to heuristic), 1 defer.
- empty_reply extinct (3rd corpus). Name-echo compliance ~100% standing.
- The "Mana available" line HIDES dual-land second colors (open ledger; deck140's evidence) —
  do not trust it as ground truth for affordability reasoning; the offered-options list is
  authoritative.

## Seat assignments

**Veterans (guides FROZEN since wave-11 — adopt-a-revision bar HIGH, decision-level classes
only). Each owns its open ledger items:**
- deck44 (1/6 this corpus — investigate the drop): forced-sub-menu bare-list representation
  (3 wave-13 repros); auto-tap stranding is RETIRED as misdiagnosis (do not relitigate).
- deck110: equip churn (outcome-delta no-op marker), blocker over-block class (wave-13
  deck135 s17 shape), model rules gaps (damage vs toughness, Obliterator sacrifice); verify
  Galvanic 4-damage improved its play/win pattern.
- deck133: CRASH-FIX VALIDATION at this seat (its Fatal Push target-decline caused the
  wave-14 segfault — confirm target-decline situations now resolve via heuristic without
  crash, tag any 'cancelCurrentAction' anomalies); self-Fatal-Push stack-target recurrence.
- deck135 (0/6 wave-14, 2/6 now): Force async response-window race (ledger #4 — tag
  RESIDUAL-RACE with seq if an answered Force cast produces no execution); board-line
  annotation contradiction (stale [attacking] + inverted freeze, wave-13 s19 shape).
- deck140: Rakdos X auto-underpay (X-announce never surfaced, wave-13 vs109 s43); count
  the Mana-available dual-hiding instances.

**Guideless first-pass (decks 17 and 21) — DIFFERENT output contract:** you are writing the
INITIAL guide. Follow the skill's guideless-first-pass doctrine: the corpus shows what the
core prompt alone gets right (do NOT re-teach that) and where deck-specific knowledge is
LOAD-BEARING (teach exactly that). Read the decklist deeply (every card's rules text via the
translog option lines), identify the deck's engine and win path, and write the guide for what
QWEN needs, not what a strong human would want said. deck17 = Faeries (flash/tempo);
deck21 = Goblin Gang (swarm aggro). Also do the standard per-decision failure-class analysis —
your findings.md drives the guide content.

## Doctrine (BINDING)

Win-rate is context only; the unit of analysis is the DECISION with seq repros. Layer-routing
enforced (engine items → notes ledger, never guide prose). Single-seat items cannot add/cut
core lines. Per-deck rotation rule: a veteran whose guide takes no modification AND whose seat
surfaces no new work signal THIS cycle is a rotation candidate — state your seat's verdict
explicitly in notes.md.

## Output contract (per deck agent, sequential, in strategy-design/wave15/deckN/)

findings.md -> strategy.txt (veterans: full guide, byte-identical if frozen; newcomers: the
NEW initial guide) -> skill.md (deltas or "none") -> general-suggestions.md -> notes.md
(engine items + rotation verdict). CURRENT skill = strategy-design/wave13/
strategy-writing-skill.md. Veterans also read their wave-13 deckN continuity files.
