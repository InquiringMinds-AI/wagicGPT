# Wave-40 lane B (decision seam) — regression fixture and arrival evidence

Docket items **#2a**, **#2b**, **#8** from `wave39/engine-ledger.md`.

## The fixture

Probe decks (owner-directed shape: an {X} spell with exactly its coloured pips available and
zero slack). They live in `decks/`; they were installed into `bin/Res/ai/baka/` for the runs and
REMOVED afterwards, so the live roster is back to its real contiguous end (deck164).

- **deck198** "Zero-Slack Blaze Probe" — `Blaze {X}{R}` x20 + `Mountain` x40. On the seat's very
  first turn it has exactly one Mountain: the only announceable X is **0**, i.e. zero slack.
  Turn 3 (one more Mountain) is the **same-game control** at slack 1, the shape the ledger's
  in-corpus repro used (deck125 vs139 seq 10-16 veto at slack 0; seq 18-19 honoured at slack 1).
- **deck199** "sparring partner" — Forests + Grizzly Bears; slow, ends the game.
- **deck197** "Dual-Land Blaze Probe" — `Blaze {X}{R}` x20 + `Volcanic Island` x40. Every land
  carries TWO mana abilities, which is the trigger for #8.

Run shape (both seats GPT, standing 35B serve):

```
WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 \
WAGIC_AI=gpt WAGIC_SELFPLAY_DECK0=198 WAGIC_SELFPLAY_DECK1=199 \
WAGIC_GPT_URL=http://100.116.136.74:8084 WAGIC_GPT_MODEL=qwen36-35b-a3b ./wagic
```

(under the standing `systemd-run --user --scope -p MemoryMax=4G -p MemorySwapMax=0` cap.)

## Evidence (`translogs/`)

| file | binary | what it shows |
|---|---|---|
| `A-2a-RED-prefix-deck198.jsonl` | eabf463b1 (pre-fix) | t0 seq3 real model call picks Blaze -> **seq4 `validation_reject_reask_exhausted`**; again in Main phase 2 (seq5/seq6). Control at t2 (slack 1) honoured, X=1. |
| `B-2a-GREEN-postfix-deck198.jsonl` | post-fix | t0 seq3 real model call (1658 ms) picks Blaze -> seq4 target -> **cast COMMITTED at X=0**; narration in seq5's prompt reads "You cast Blaze / Your Blaze resolved / You targeted The opponent with Blaze's Deal 0 Damage ability". **Zero defer records in the whole game.** |
| `C-2b-forced-reject-one-record-per-turn.jsonl` | post-fix + a throwaway forced-reject build | validation forced to reject every pick: **exactly ONE `validation_reject_reask_exhausted` per turn**, and NO second cast ask later in the same turn (pre-fix had two in turn 0). The heuristic then answers the window, as designed. |
| `D-8-RED-prefix-deck197-doubleask.jsonl` | eabf463b1 (pre-fix) | **4 double-asks in one game** — the same `Casting decision` question re-issued back to back, the second at the POST-TAP `Mana available:` figure (6->3, 4->1, 5->2, 5->2). Each is a full extra model call. |
| `E-8-GREEN-postfix-deck197.jsonl` | post-fix | **0 double-asks.** One cast ask per window, `Mana available:` ladder 2,3,4,5,6. |

## #8 root-cause measurement (Baka, no inference; deck197 vs deck199, 2 games each)

|  | X-cost payment plans | of those, with a DUPLICATE source | aborted cast plans (refused payment click) | Blaze casts reaching the stack |
|---|---|---|---|---|
| before | 90 | **66** | **11** | 13 |
| after | 135 | **0** | **0** | 17 |
