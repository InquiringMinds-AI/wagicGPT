# Wave-51 step-1 lane brief (from wave50/engine-ledger.md — THE DOCKET; read your items there IN FULL)

Base: master after lanes A (42f2eff2b: Soul Shatter/Flare of Malice/Riveteers Charm scripts —
D1 DONE) and B (df8ff7641: affordable AI casts now route through ManaEngine::planPayment —
D2 DONE) + the five owner Vita fixes. Baselines: PARSETEST 1930/0; suite 1154/0 + 39 AI/0,
exactly 1 `W43-1 ASSERT`, 0 timeouts (confirm on the gate log ~/.gatelogs/w51-gate.log).
Doctrine: enforce legality without constraining choice; every parse/render change ships
PARSETEST cases (positive, must-NOT-match negative, echo shape); every engine change ships a
suite pin; byte-splice Windows-1252 files; MEMORY CAP on every wagic run; WAIT FOR GATES WITH A
FOREGROUND `until [ -f DONE ]; do sleep 10; done` loop (timeout 1200 s) — never a Monitor, never
end your turn with the commit pending; state a falsifiable prediction; report what you did NOT
verify. `cd` into your worktree inside every Bash call — cwd drifts.

Lane C — parsing / re-asks: D3 in-range index + off-menu name -> one `[RE-ASK]` (the
  `named_row_reask` family), never `stale_echo` to Baka (3/3 fallbacks; Baka wrong twice);
  D4 `x0` on the repeat row = a pass, not one activation; CHOICE count vs PLAN `pass`
  conflict -> one `plan_choice_conflict` re-ask (4 windows).
Lane D — instrumentation / records: D5 `- Paid` receipt on land-animation and other
  activation payments (3/9 missing — the instrument for D2); D17 receipt separator vs comma
  names; D18 JSONL `turn` off-by-one (4th corpus); D10 Soul Shatter-class edict rows carry the
  `{right now: <highest-MV creature/planeswalker> is sacrificed}` fact (ships with D1's script).
Lane E — row pricing: D6 activated-ability damage rows print `{right now: takes N - DIES /
  SURVIVES}` like spell rows (0/49; two wasted shots); D7 Hive/Lair Upkeep-only offers get an
  annotation (`[this animation lasts until end of turn; offered again in your main phase]`) —
  the defer form is REJECTED; D8 Tutor / second-copy `[already owned: <name> on your
  battlefield]` tags; D9 Hammer-return-class rows priced against the hand (`{returns X; you
  hold Y}`).
Lane F — narration/back faces: D11 `{feeds: ...}` converter-count line incl. discard
  punishers (Liliana's Caress); D16 bare `// <name>` back faces via `{target text:}` (third
  emitter path); D30 measurement only: log share of the largest prompts (report, no change).
Not this wave: D19-D29 residuals, D31 frontend grouping (owner item).
