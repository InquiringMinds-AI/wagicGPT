# Wave-22 deployment manifest

Consumed by the integrator. **Synthesis does NOT deploy anything** — this manifest is the verified instruction set.
Every byte-copy claim was `cmp`-verified against the LIVE deployed file `bin/Res/ai/baka/deckN_strategy.txt`
(`bin/Res` is gitignored — deploys need `git add -f` if committed). Binary this corpus validated: `/tmp/wagic-1d8ed8194`.

## Guide files — deploy actions

| deck | deployed (B) | seat (B) | Δ | `cmp` verdict | ACTION |
|------|-------------:|---------:|---:|---------------|--------|
| 135 | 13357 | 13357 | 0 | **IDENTICAL** | **NONE** — frozen; deployed guide already == seat (canary in place) |
| 62 | 8893 | 8893 | 0 | **IDENTICAL** | **NONE** — frozen round 6; deployed == seat (canary in place) |
| 102 | 8201 | 8201 | 0 | **IDENTICAL** | **NONE — ALREADY DEPLOYED** by the reviewer mid-wave; deployed == seat, verified live-revised (see note) |
| 27 | 6959 | 7497 | +538 | DIFFERS (6 diff-lines) | **DEPLOY** `wave22/deck27/strategy.txt` → `bin/Res/ai/baka/deck27_strategy.txt` |
| 109 | 7711 | 8430 | +719 | DIFFERS (72 diff-lines) | **DEPLOY** `wave22/deck109/strategy.txt` → `bin/Res/ai/baka/deck109_strategy.txt` |
| 44 | 12084 | 12010 | −74 | DIFFERS (17 diff-lines) | **DEPLOY** `wave22/deck44/strategy.txt` → `bin/Res/ai/baka/deck44_strategy.txt` |
| 140 | 7364 | 8567 | +1203 | DIFFERS (54 diff-lines) | **DEPLOY** `wave22/deck140/strategy.txt` → `bin/Res/ai/baka/deck140_strategy.txt` |

## Per-deck notes

- **deck135 — NO ACTION (frozen).** Seat `strategy.txt` is byte-identical to the deployed guide; zero decision leak this
  corpus. The guide stays deployed as a canary as deck135 rotates out.
- **deck62 — NO ACTION (frozen round 6).** Byte-identical to deployed; every taught class held; N9 witness role
  discharged. Stays deployed as a canary as deck62 rotates out.
- **deck102 — NO ACTION; the revision is ALREADY LIVE (verified).** The reviewer deployed its two edits mid-wave and the
  seat file is a copy of the deployed result — `cmp` deployed == seat (8201 B, IDENTICAL). VERIFIED that deployed ==
  REVISED (not a coincidental freeze): the deployed guide contains the added Tergrid-DFC/Lantern awareness (3 Lantern/
  Flip mentions) AND the now-FALSE painland clause ("the Mana available line does NOT warn you") is REMOVED (0
  occurrences), matching the two claimed edits (painland-crutch DEMOTE + delete-now-false-clause; Tergrid-DFC ADD). No
  deploy action needed; flag NONE — deployed == seat holds. deck102 STAYS in the pool.
- **deck27 — DEPLOY (+6 diff-lines).** The single surgical edit: a free-kills-first multi-attacker block-ALLOCATION
  sentence (the guide-lane fix for the answer-first commit-lock harmful instance, vs109 s15). Deck-specific (deck27 is a
  real-blocker deck); not core. deck27 STAYS.
- **deck109 — DEPLOY (crutch-RETIREMENT; 72 diff-lines, +719 B net).** Retires two now-engine-carried representation
  crutches (the hybrid-mana lecture — engine renders `[hybrid: each {b/r} pays with B or R ...]`; and the pillar-cost
  correction — engine renders `Cast Pillar of Flame {r}`), replacing them with lean strategic content (net larger despite
  "retirement" because the crutches are replaced, not just deleted). deck109 STAYS one more corpus to CONFIRM the retired
  crutches were truly redundant, not load-bearing under a different draw — deploy is a prerequisite for that confirmation.
- **deck44 — DEPLOY (crutch-demotion; 17 diff-lines, −74 B net).** Three diffs: (1) DEMOTE the GFTT friendly-only
  walkthrough 5→3 lines (engine annotation `(warning: this would harm your own side ...)` confirmed fired 28× / obeyed
  0/28); (2) DELETE the redundant SITUATIONS bullet duplicating the engine-annotated self-GFTT case (engine-carried +
  never actually decided a game → fails the SITUATIONS-slot test); (3) ADD a Cruel-Edict has-a-creature clause (observed
  muddle vs140 s6/s21). Terminal (the demotion the rotation existed to perform); deck44 rotates out after, guide stays as
  canary.
- **deck140 — DEPLOY (crutch-demotion + NEW forced-discard rule; 54 diff-lines, +1203 B net).** Demotes two belief-kill
  crutches now carried by the printed `(creatures: N)` count (tapped-still-counts + artifacts-don't-lower-C), KEEPS the
  shroud targeting-fact (a count dissolves "how many" but not "how the spell interacts"), and ADDS the sanctioned interim
  forced-discard/sacrifice rule (the loss-causing seam vs102 the frozen guide never covered — its durable fix is the
  representation-layer detrimental-self-choice header, engine ledger #1). **Deploy this THIS wave regardless of rotation**
  (reviewer + synthesis both: it fixes a real, previously-uncovered, game-deciding seam). deck140 STAYS one more corpus
  (synthesis moderation — validate the new rule at-seat + it is the top-ledger witness).

## Non-guide deploys / config

- **No `general-strategy.txt`** — core prompt is PASS (17th consecutive). No core-prompt file to deploy.
- **repetition_penalty (decode-sampler config, `~/.Wagic/ai/gpt/endpoints.txt`): RECOMMENDED to ENABLE a modest pilot for
  the wave-23 corpus** (2nd consecutive wave recommending; wider evidence — now the common upstream cause of the wave's
  truncation-fed parser events). This is a CONFIG toggle, not a code or prompt deploy; owner/roster call. Keep the
  max_reply_tokens guard. See synthesis-notes.md → Core prompt section.
- **Engine/harness ledger items are NOT deploys** — they are code work for the between-wave step (synthesis-notes.md →
  WAVE-23 ENGINE LEDGER). None become prompt text.

## Rotation deploy consequence

Rotate-out seats (135, 62, 44) keep their guides DEPLOYED byte-identical as canaries — no removal, only "stop
re-reviewing." Rotate-in seats for wave-23: **137** (guideless — needs a Step-0-bis initial guide authored before the
corpus), **133** and **131** (guided-never-cycled — their EXISTING `deck133_strategy.txt` / `deck131_strategy.txt` are
already deployed; they enter as re-validation seats, no new deploy). WAVE-23 POOL: 27, 102, 109, 140, 137, 133, 131.
