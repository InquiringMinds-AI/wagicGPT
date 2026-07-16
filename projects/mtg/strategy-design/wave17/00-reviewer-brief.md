# Wave-17 reviewer brief (shared context for all 7 deck agents + synthesis)

Third per-deck-rotation cycle. deck44 and deck133 rotated out; deck49 (Dragons) and deck35
(Slivers) entered GUIDELESS. decks 17/21 play their FIRST corpus under their REVISED guides
(wave-16 revisions: 17 = walls-aren't-flyers root fix + race branch; 21 = no-blocker race
math). deck62 plays its FIRST GUIDED corpus (initial guide shipped wave-16, capability-table
lead). Binary 7cdcf9c73 adds the wave-16 engine batch: (a) PLAN-carry stale-intent caveat
("(note: the actions your plan names are no longer among the options available right now...)"
— injected on 319 decisions this corpus); (b) legalCasts normalEntry fix (unpayable hardcasts
no longer offered when the alt-cost is knocked out); (c) own-target-only BAD-effect
suppression (damage/destroy casts with only own legal targets are not offered on the GPT menu).

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-151559` — 21 games, round-robin
  over 110,135,17,21,62,49,35. Binary /tmp/wagic-7cdcf9c73.
- Win table: 17:5/6, 110:4/6, 62:4/6, 49:3/6, 21:2/6, 35:2/6, 135:1/6. 0 timeouts, only 3
  life-adjudicated (best yet). 0 segfaults (4th clean corpus).
- Fallbacks 3.3% (34/1021) — BEST EVER: 18 unparsed (HALVED from wave-16's 40 — the PLAN
  caveat is the prime suspect for the improvement; reviewers at high-caveat seats confirm),
  13 stale_echo, 3 defer, 0 empty_reply.

## KNOWN CORPUS-WIDE (do not re-diagnose)

- **Force surface CLOSED (deck135)**: tapped-out free Force FIRED (s6 vs deck21: "(no untapped
  sources)" + exile-pitch cast offered, targeted Volcanic Hammer; two full cast->resolve cycles
  in that game). No unpayable hardcast offers seen. deck135's reviewer verifies the remaining
  Force decisions were played WELL (right targets, right timing), not just legally.
- **E6 PROMOTED — Mox Opal "Cast Card Normally" label now has measured cost** (deck110): 3
  stale_echoes (s25/s28/s29, game vs deck17) + 1 unparsed ramble (s24 "This is a puzzle") are
  all the model failing to recognize the degenerate menu label "Cast Card Normally with Mox
  Opal {Mox's own rules text}". Staleness route caught them (correct downgrades), but each is
  a wasted round trip. deck110's reviewer: characterize what menu the engine was ACTUALLY
  showing (what cast was being paid?) with seq repros to drive the engine fix.
- stale_echo 13: deck49 owns 7 (guideless newcomer — spot-check ALL of them: wrongful
  downgrade = an echo that DID name an offered option), deck110 4 (3 are the Mox item above),
  deck135 2 (s22/s23 vs deck62 — the model echoed "Snow-Covered Forest" when the sub-menu
  offered Island/Scrying Sheets; correct catches of model misread, same shape as wave-16 s33).
- deck135 unparsed cluster (s29/s31/s38 vs deck62): multi-target sub-menus ("Choose target 2
  of exactly 4") confuse the model into rambling. Possible representation item — deck135's
  reviewer characterizes the prompt shape and what the model needed.
- Hybrid annotation exposure: deck17 saw 35, deck21 8, deck49 1. deck17's reviewer measures
  effect (misread/decline class vs prior waves).

## Seat assignments

- **deck110:** H1 canary (PLAN-caveat validation — did the self-Blast/stale-plan spiral class
  die? cite before/after); OWNS the promoted E6 Mox item (characterize + seq repros); equip
  churn watch (E2); 3 defers this corpus — what were they?
- **deck135 (1/6):** Force-played-WELL check (see above); stale [attacking] tag (3rd wave
  watch); multi-target sub-menu unparsed cluster (s29/s31/s38); construction-vs-play split for
  the 1/6 record — is this deck structurally weak in this pool, and does its guide need a
  revision or is the seat clean?
- **deck17 — REVISED-GUIDE VALIDATION (5/6, best seat):** wave-16 revision = walls-aren't-
  flyers root fix + race branch. Did the revised rules fire? Which taught classes
  died/persist/mutated (Step 0-ter)? Hybrid annotation effect (35 exposures).
- **deck21 — REVISED-GUIDE VALIDATION (2/6):** wave-16 revision = no-blocker race math. Same
  Step 0-ter charge. 2/6 again — is the record floor structural (pool hostile to swarm) or are
  taught classes still dying? Only 1 blockers decision all corpus — why?
- **deck62 — FIRST-GUIDE VALIDATION (4/6, up from 2/6 guideless):** capability-table lead
  guide. Did its rules fire (which classes died/persist)? Standard first-guide-validation
  revision expected.
- **deck49 — GUIDELESS FIRST PASS (Dragons, 3/6):** Step 0-bis doctrine. Owns its 7
  stale_echoes. Write the initial guide (format-match a live guide).
- **deck35 — GUIDELESS FIRST PASS (Slivers, 2/6):** Step 0-bis doctrine. Slivers = lord-stack
  tribal; watch whether the model understands cumulative sliver buffs. Write the initial guide.

## Doctrine (BINDING)

Win-rate is context; the unit of analysis is the DECISION with seq repros. Layer-routing
enforced. Single-seat items cannot add/cut core lines. Per-deck rotation: state your seat's
verdict explicitly in notes.md (no-guide-mod AND no-new-work-signal = rotation candidate).

## Output contract (per deck agent, sequential, in strategy-design/wave17/deckN/)

findings.md -> strategy.txt (veterans/guided: byte-identical if frozen, REVISED if warranted;
newcomers 49/35: the NEW initial guide) -> skill.md -> general-suggestions.md -> notes.md
(engine items + rotation verdict). CURRENT skill = strategy-design/wave16/
strategy-writing-skill.md. Guided seats read their wave-16 deckN continuity files.
