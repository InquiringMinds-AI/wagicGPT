# Wave-52 step-1 lane brief (from wave51/engine-ledger.md — THE DOCKET; read your items there IN FULL)

Base: master after lanes I (4bdca64bf: opponent-turn cast windows — D5 DONE) and H (d58aeba6b:
Battlement over-tap — D4 DONE); lane G (D1 livelock / D2 pass-row echo / D3 PUT mis-execution)
in flight at ~/Projects/wagicGPT-w52-g. Baselines: PARSETEST 2022/0; suite 1158/0 + 42 AI/0,
exactly 1 `W43-1 ASSERT`, 0 timeouts (re-confirm on the merged gate log before you start).
Doctrine: enforce legality without constraining choice; every parse/render change ships
PARSETEST cases (positive, must-NOT-match negative, echo shape); every engine change ships a
suite pin; byte-splice Windows-1252 files; MEMORY CAP on every wagic run; WAIT FOR GATES WITH A
FOREGROUND `until [ -f DONE ]; do sleep 10; done` loop (timeout 1200 s) with the run launched
as a detached `systemd-run --user --unit=NAME` (a backgrounded subshell dies with the Bash
call); never a Monitor; never end your turn with the commit pending; `cd` into your worktree
inside every Bash call; ALL scratch/logs/done-files under ~/.gatelogs/ or your worktree's
strategy-design/wave52/ — NEVER in ~ or ~/Projects; state a falsifiable prediction; report what
you did NOT verify.

Lane J — parsing: D6 index/name conflict with BOTH on the menu executes the index (prefer the
  named row when it is unique on the menu; re-ask when ambiguous; PARSETEST from the corpus
  replies); D14 `plan_choice_conflict` scope: single-row windows, no-PLAN replies, and prose
  verdicts ("we must pass") after the coded line.
Lane K — pricing annotations: D7 a Talisman-paid (life-cost mana) cast at 1 life prints
  nothing lethal — `{paying this costs you N life - you would be at 0}`; D9 `{this hits YOUR
  permanent}` on exile/loyalty/bounce rows (Kaya -3 self-exile at 1 life); D10 `[already
  owned:` — stackables (Howling Mine) are NOT dead, legendaries carry the legend-rule clause,
  and a tutor whose target is the OTHER half is not warned against; D11 `[from exile]` rows
  say the cause and the tax (Spellbinder {2}).
Lane L — render/collapse: D8 per-token `Equip/Sacrifice ... targeting Vampire #N` ability
  target menus collapse like the cast-row target clause (36 rows -> 1 ranged row; parser keeps
  accepting any handle); D12 `[repeat: activated this turn N times]` Hive row not re-offered
  in the same window after its animation; D13 an animated land is a creature — the planner
  must not tap it for mana in the same turn it was animated for combat (planner half only;
  the DEFER form is rejected); D15 -1/-1 target rows carry DIES/SURVIVES; D16 fetch rows say
  the Flats makes no mana; D17 `{spends K of M}` on cast rows too.
Not this wave: D18-D22 LOW, D23 log compaction (owner decision), D24-D26 carried, D27 frontend.
